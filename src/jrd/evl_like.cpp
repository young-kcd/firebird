#ifndef EVL_LIKE_INCLUDED_BY_EVL_CPP

#pragma message ("Do not compile this file stand-alone. It gets included by evl.cpp!")

#else // EVL_LIKE_INCLUDED_BY_EVL_CPP

/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		evl_like.cpp
 *	DESCRIPTION:	Expression evaluation (international). Obsolete.
 *
 * The contents of this file are subject to the Interbase Public
 * License Version 1.0 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy
 * of the License at http://www.Inprise.com/IPL.html
 *
 * Software distributed under the License is distributed on an
 * "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
 * or implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code was created by Inprise Corporation
 * and its predecessors. Portions created by Inprise Corporation are
 * Copyright (C) Inprise Corporation.
 *
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 */

#define SLEUTH_insensitive	1
#define COND_UPPER(obj, c)	((flags & SLEUTH_insensitive) ?	(obj).to_upper(c) : (c))

#define GDML_MATCH_ONE		'?'
#define GDML_MATCH_ANY		'*'

#define GDML_QUOTE		'@'
#define GDML_NOT		'~'
#define GDML_RANGE		'-'
#define GDML_CLASS_START	'['
#define GDML_CLASS_END		']'
#define GDML_SUBSTITUTE		'='
#define GDML_FLAG_SET		'+'
#define GDML_FLAG_CLEAR		'-'
#define GDML_COMMA		','
#define GDML_LPAREN		'('
#define GDML_RPAREN		')'


bool MATCHESNAME(thread_db* tdbb,
				   TextType obj,
				   const MATCHESTYPE* p1,
				   SSHORT l1_bytes, const MATCHESTYPE* p2, SSHORT l2_bytes)
{
/**************************************
 *
 *	E V L _ ? ? _ m a t c h e s
 *	E V L _ w c _ m a t c h e s
 *	E V L _ n c _ m a t c h e s
 *
 **************************************
 *
 * Functional description
 *	Return true if a string (p1, l1) matches a given pattern (p2, l2).
 *	The character '?' in the pattern may match any single character
 *	in the the string, and the character '*' may match any sequence
 *	of characters.
 *
 *	Wide SCHAR version operates on short-based buffer,
 *	instead of SCHAR-based.
 *
 * Matches is not a case-sensitive operation, thus it has no
 * 8-bit international impact.
 *
 **************************************/
	fb_assert(p1 != NULL);
	fb_assert(p2 != NULL);
	fb_assert((l1_bytes % sizeof(MATCHESTYPE)) == 0);
	fb_assert((l2_bytes % sizeof(MATCHESTYPE)) == 0);

	SSHORT l1 = l1_bytes / sizeof(MATCHESTYPE);
	SSHORT l2 = l2_bytes / sizeof(MATCHESTYPE);

	while (l2-- > 0) {
		const MATCHESTYPE c = *p2++;
		if (c == (MATCHESTYPE) GDML_MATCH_ANY) {
			while ((l2 > 0) && (*p2 == (MATCHESTYPE) GDML_MATCH_ANY)) {
				l2--;
				p2++;
			}
			if (l2 == 0)
				return true;
			while (l1)
			{
				if (MATCHESNAME(tdbb, obj, p1++,
								l1-- * sizeof(MATCHESTYPE), p2,
								l2 * sizeof(MATCHESTYPE)))
				{
					return true;
				}
			}
			return false;
		}
		if (l1-- == 0)
			return false;
		if ((c != (MATCHESTYPE) GDML_MATCH_ONE && c != *p1))
			return false;
		p1++;
	}

	return !l1;
}


bool SLEUTHNAME(thread_db* tdbb_dummy,
				  TextType obj,
				  USHORT flags,
				  const SLEUTHTYPE* search,
				  USHORT search_len, const SLEUTHTYPE* match, USHORT match_len)
{
/**************************************
 *
 *	E V L _ ? ? _ s l e u t h _ c h e c k
 *
 **************************************
 *
 * Functional description
 *	Evaluate the "sleuth" search operator.
 *
 *	Turn the (pointer, byte length) input parameters into
 *	(pointer, end_pointer) for use in SLEUTH_AUX
 *
 **************************************/
	fb_assert((match_len % sizeof(SLEUTHTYPE)) == 0);
	fb_assert((search_len % sizeof(SLEUTHTYPE)) == 0);

	const SLEUTHTYPE* const end_match = match + (match_len / sizeof(SLEUTHTYPE));
	const SLEUTHTYPE* const end_search = search + (search_len / sizeof(SLEUTHTYPE));

	return SLEUTH_AUX(obj, flags, search, end_search, match, end_match);
}


USHORT SLEUTH_MERGE_NAME(thread_db* tdbb_dummy,
						 TextType obj,
						 const SLEUTHTYPE* match,
						 USHORT match_bytes,
						 const SLEUTHTYPE* control,
						 USHORT control_bytes,
						 SLEUTHTYPE* combined, USHORT combined_bytes)
{
/**************************************
 *
 *	E V L _ ? ? _ s l e u t h _ m e r g e
 *
 **************************************
 *
 * Functional description
 *	Merge the matching pattern and control strings to give a cannonical
 *	matching pattern.  Return the length of the combined string. 
 *
 * 	What this routine does is to take the language template, strip off 
 *	the prefix and put it in the output string, then parse the definitions
 *	into an array of character pointers.  The index array is the defined
 *	character.   The routine then takes the actual match pattern and uses
 *	the characters in it to index into the definitions to produce an equivalent
 *	pattern in the cannonical language.
 *
 *	The silly loop setting *v++ to zero initializes the array up to the
 *	highest character defined (also max_op).  Believe it or not, that part
 *	is not a bug.
 *
 **************************************/
	fb_assert(match != NULL);
	fb_assert(control != NULL);
	fb_assert(combined != NULL);

	fb_assert((match_bytes % sizeof(SLEUTHTYPE)) == 0);
	fb_assert((control_bytes % sizeof(SLEUTHTYPE)) == 0);

	const SLEUTHTYPE* const end_match = match + (match_bytes / sizeof(SLEUTHTYPE));
	const SLEUTHTYPE* const end_control = control + (control_bytes / sizeof(SLEUTHTYPE));

	SLEUTHTYPE max_op = 0;
	SLEUTHTYPE* comb = combined;
	SLEUTHTYPE* vector[256];
	SLEUTHTYPE** v = vector;
	SLEUTHTYPE temp[256];
	SLEUTHTYPE* t = temp;

/* Parse control string into substitution strings and initializing string */

	while (control < end_control) {
		SLEUTHTYPE c = *control++;
		if (*control == GDML_SUBSTITUTE) {
			/* Note: don't allow substitution characters larger than vector */
			SLEUTHTYPE** const end_vector =
				vector + (((int)c < FB_NELEM(vector)) ? c : 0);
			while (v <= end_vector)
				*v++ = 0;
			*end_vector = t;
			++control;
			while (control < end_control) {
				c = *control++;
				if ((t > temp && t[-1] == GDML_QUOTE)
					|| ((c != GDML_COMMA) && (c != GDML_RPAREN)))
				{
					*t++ = c;
				}
				else
					break;
			}
			*t++ = 0;
		}
		else if (c == GDML_QUOTE && control < end_control)
			*comb++ = *control++;
		else if (c == GDML_RPAREN)
			break;
		else if (c != GDML_LPAREN)
			*comb++ = c;
	}

	max_op = v - vector;

/* Interpret matching string, substituting where appropriate */

	while (match < end_match) {
		const SLEUTHTYPE c = *match++;

		/* if we've got a defined character, slurp the definition */

        SLEUTHTYPE* p;
		if (c <= max_op && (p = vector[c])) {
			while (*p)
				*comb++ = *p++;

			/* if we've got the definition of a quote character, 
			   slurp the next character too */

			if (comb > combined && comb[-1] == GDML_QUOTE && *match)
				*comb++ = *match++;
		}

		/* at this point we've got a non-match, but as it might be one of ours, 
		   quote it. */

		else {
			if ((((size_t) c) < sizeof(special)) && special[c] &&
				comb > combined && comb[-1] != GDML_QUOTE)
			{
				*comb++ = GDML_QUOTE;
			}
			*comb++ = c;
		}
	}

/* Put in trailing stuff */

	while (control < end_control)
		*comb++ = *control++;

	/* YYY - need to add code watching for overflow of combined */

	return (comb - combined) * sizeof(SLEUTHTYPE);
}


static bool SLEUTH_AUX(
						  TextType obj,
						  USHORT flags,
						  const SLEUTHTYPE* search,
						  const SLEUTHTYPE* end_search,
	const SLEUTHTYPE* match, const SLEUTHTYPE* end_match)
{
/**************************************
 *
 *	s l e u t h _ a u x
 *
 **************************************
 *
 * Functional description
 *	Evaluate the "sleuth" search operator.
 *
 **************************************/
	fb_assert(search != NULL);
	fb_assert(end_search != NULL);
	fb_assert(match != NULL);
	fb_assert(end_match != NULL);
	fb_assert(search <= end_search);
	fb_assert(match <= end_match);

	while (match < end_match) {
		SLEUTHTYPE c = *match++;
		if ((c == GDML_QUOTE && (c = *match++)) ||
			((((size_t) c) < sizeof(special)) && !special[c]))
		{
			c = COND_UPPER(obj, c);
			if (match >= end_match || *match != GDML_MATCH_ANY) {
				if (search >= end_search)
					return false;
				const SLEUTHTYPE d = *search++;
				if (c != COND_UPPER(obj, d))
					return false;
			}
			else {
				++match;
				for (;;)
					if (SLEUTH_AUX
						(obj, flags, search, end_search, match, end_match))
					{
						return true;
					}
					else if (search < end_search) {
						const SLEUTHTYPE d = *search++;
						if (c != COND_UPPER(obj, d))
							return false;
					}
					else
						return false;
			}
		}
		else if (c == GDML_MATCH_ONE)
			if (match >= end_match || *match != GDML_MATCH_ANY) {
				if (search >= end_search)
					return false;
				search++;
			}
			else {
				if (++match >= end_match)
					return true;
				for (;;)
					if (SLEUTH_AUX
						(obj, flags, search, end_search, match,
						 end_match))
					{
						return true;
					}
					else if (++search >= end_search)
						return false;
			}
		else if (c == GDML_CLASS_START) {
			const SLEUTHTYPE* const char_class = match;
			while (*match++ != GDML_CLASS_END) {
				if (match >= end_match)
					return false;
			}
			const SLEUTHTYPE* const end_class = match - 1;
			if (match >= end_match || *match != GDML_MATCH_ANY) {
				if (!SLEUTH_CLASS_NAME
					(obj, flags, char_class, end_class, *search++))
				{
					return false;
				}
			}
			else {
				++match;
				for (;;)
					if (SLEUTH_AUX
						(obj, flags, search, end_search, match,
						 end_match))
					{
						return true;
					}
					else if (search < end_search) {
						if (!SLEUTH_CLASS_NAME
							(obj, flags, char_class, end_class, *search++))
						{
							return false;
						}
					}
					else
						return false;
			}
		}
		else if (c == GDML_FLAG_SET) {
			c = *match++;
			if (c == 's' || c == 'S')
				flags &= ~SLEUTH_insensitive;
		}
		else if (c == GDML_FLAG_CLEAR) {
			c = *match++;
			if (c == 's' || c == 'S')
				flags |= SLEUTH_insensitive;
		}
	}

	if (search < end_search)
		return false;

	return true;
}


static bool SLEUTH_CLASS_NAME(
								 TextType obj,
								 USHORT flags,
								 const SLEUTHTYPE* char_class,
								 const SLEUTHTYPE* const end_class, 
								 SLEUTHTYPE character)
{
/**************************************
 *
 *	s l e u t h _ c l a s s
 *
 **************************************
 *
 * Functional description
 *	See if a character is a member of a class.
 *	Japanese version operates on short-based buffer,
 *	instead of SCHAR-based.
 *
 **************************************/
	fb_assert(char_class != NULL);
	fb_assert(end_class != NULL);
	fb_assert(char_class <= end_class);

	bool result = true;
	character = COND_UPPER(obj, character);

	if (*char_class == GDML_NOT) {
		++char_class;
		result = false;
	}

	while (char_class < end_class) {
		const SLEUTHTYPE c = *char_class++;
		if (c == GDML_QUOTE) {
			if (*char_class++ == character)
				return true;
		}
		else if (*char_class == GDML_RANGE) {
			char_class += 2;
			if (character >= c && character <= char_class[-1])
				return result;
		}
		else if (character == c)
			return result;
	}

	return !result;
}


#endif // EVL_LIKE_INCLUDED_BY_EVL_CPP

