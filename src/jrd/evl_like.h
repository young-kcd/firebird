/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		evl_like.h
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

static const UCHAR special[256] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0,	/* $%*+- (dollar, percent, star, plus, minus) */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,	/* ?     (question) */
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* @     (at-sign) */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,	/* [     (open square) */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,	/* ~     (tilde) */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

template <class MATCHESTYPE>
bool MATCHESNAME(Jrd::thread_db* tdbb, Jrd::TextType* obj,
				 const MATCHESTYPE* p1, SLONG l1_bytes,
				 const MATCHESTYPE* p2, SLONG l2_bytes)
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
	fb_assert((obj->getCanonicalWidth() == sizeof(MATCHESTYPE)));

	SLONG l1 = l1_bytes / sizeof(MATCHESTYPE);
	SLONG l2 = l2_bytes / sizeof(MATCHESTYPE);

	while (l2-- > 0) {
		const MATCHESTYPE c = *p2++;
		if (c == *(MATCHESTYPE*)obj->getGdmlMatchAnyCanonic()) {
			while ((l2 > 0) && (*p2 == *(MATCHESTYPE*)obj->getGdmlMatchAnyCanonic())) {
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
		if (c != *(MATCHESTYPE*)obj->getGdmlMatchOneCanonic() && c != *p1)
			return false;
		p1++;
	}

	return !l1;
}


template <class SLEUTHTYPE>
bool SLEUTHNAME(Jrd::thread_db* tdbb_dummy, Jrd::TextType* obj, USHORT flags,
				const SLEUTHTYPE* search, SLONG search_len,
				const SLEUTHTYPE* match, SLONG match_len)
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
	fb_assert(obj->getCanonicalWidth() == sizeof(SLEUTHTYPE));

	const SLEUTHTYPE* const end_match = match + (match_len / sizeof(SLEUTHTYPE));
	const SLEUTHTYPE* const end_search = search + (search_len / sizeof(SLEUTHTYPE));

	return SLEUTH_AUX(obj, flags, search, end_search, match, end_match);
}


template <class SLEUTHTYPE>
ULONG SLEUTH_MERGE_NAME(Jrd::thread_db* tdbb_dummy, Jrd::TextType* obj,
						const SLEUTHTYPE* match, SLONG match_bytes,
						const SLEUTHTYPE* control, SLONG control_bytes,
						SLEUTHTYPE* combined, SLONG combined_bytes)
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
	fb_assert(obj->getCanonicalWidth() == sizeof(SLEUTHTYPE));

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
		if (*control == *(SLEUTHTYPE*)obj->getGdmlSubstituteCanonic()) {
			/* Note: don't allow substitution characters larger than vector */
			SLEUTHTYPE** const end_vector =
				vector + (((int)c < FB_NELEM(vector)) ? c : 0);
			while (v <= end_vector)
				*v++ = 0;
			*end_vector = t;
			++control;
			while (control < end_control) {
				c = *control++;
				if ((t > temp && t[-1] == *(SLEUTHTYPE*)obj->getGdmlQuoteCanonic())
					|| ((c != *(SLEUTHTYPE*)obj->getGdmlCommaCanonic()) && (c != *(SLEUTHTYPE*)obj->getGdmlRParenCanonic())))
				{
					*t++ = c;
				}
				else
					break;
			}
			*t++ = 0;
		}
		else if (c == *(SLEUTHTYPE*)obj->getGdmlQuoteCanonic() && control < end_control)
			*comb++ = *control++;
		else if (c == *(SLEUTHTYPE*)obj->getGdmlRParenCanonic())
			break;
		else if (c != *(SLEUTHTYPE*)obj->getGdmlLParenCanonic())
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

			if (comb > combined && comb[-1] == *(SLEUTHTYPE*)obj->getGdmlQuoteCanonic() && *match)
				*comb++ = *match++;
		}

		/* at this point we've got a non-match, but as it might be one of ours, 
		   quote it. */

		else {
			if ((((size_t) c) < sizeof(special)) && special[c] &&
				comb > combined && comb[-1] != *(SLEUTHTYPE*)obj->getGdmlQuoteCanonic())
			{
				*comb++ = *(SLEUTHTYPE*)obj->getGdmlQuoteCanonic();
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


template <class SLEUTHTYPE>
static bool SLEUTH_AUX(
						  Jrd::TextType* obj,
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
	fb_assert(obj->getCanonicalWidth() == sizeof(SLEUTHTYPE));

	while (match < end_match) {
		SLEUTHTYPE c = *match++;
		if ((c == *(SLEUTHTYPE*)obj->getGdmlQuoteCanonic() && (c = *match++)) ||
			((((size_t) c) < sizeof(special)) && !special[c]))
		{
			if (match >= end_match || *match != *(SLEUTHTYPE*)obj->getGdmlMatchAnyCanonic()) {
				if (search >= end_search)
					return false;
				const SLEUTHTYPE d = *search++;
				if (c != d)
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
						if (c != d)
							return false;
					}
					else
						return false;
			}
		}
		else if (c == *(SLEUTHTYPE*)obj->getGdmlMatchOneCanonic())
			if (match >= end_match || *match != *(SLEUTHTYPE*)obj->getGdmlMatchAnyCanonic()) {
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
		else if (c == *(SLEUTHTYPE*)obj->getGdmlClassStartCanonic()) {
			const SLEUTHTYPE* const char_class = match;
			while (*match++ != *(SLEUTHTYPE*)obj->getGdmlClassEndCanonic()) {
				if (match >= end_match)
					return false;
			}
			const SLEUTHTYPE* const end_class = match - 1;
			if (match >= end_match || *match != *(SLEUTHTYPE*)obj->getGdmlMatchAnyCanonic()) {
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
		else if (c == *(SLEUTHTYPE*)obj->getGdmlFlagSetCanonic()) {
			c = *match++;
			if (c == *(SLEUTHTYPE*)obj->getGdmlLowerSCanonic() || c == *(SLEUTHTYPE*)obj->getGdmlUpperSCanonic())
				flags &= ~SLEUTH_insensitive;
		}
		else if (c == *(SLEUTHTYPE*)obj->getGdmlFlagClearCanonic()) {
			c = *match++;
			if (c == *(SLEUTHTYPE*)obj->getGdmlLowerSCanonic() || c == *(SLEUTHTYPE*)obj->getGdmlUpperSCanonic())
				flags |= SLEUTH_insensitive;
		}
	}

	if (search < end_search)
		return false;

	return true;
}


template <class SLEUTHTYPE>
static bool SLEUTH_CLASS_NAME(
								 Jrd::TextType* obj,
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
	fb_assert(obj->getCanonicalWidth() == sizeof(SLEUTHTYPE));

	bool result = true;

	if (*char_class == *(SLEUTHTYPE*)obj->getGdmlNotCanonic()) {
		++char_class;
		result = false;
	}

	while (char_class < end_class) {
		const SLEUTHTYPE c = *char_class++;
		if (c == *(SLEUTHTYPE*)obj->getGdmlQuoteCanonic()) {
			if (*char_class++ == character)
				return true;
		}
		else if (*char_class == *(SLEUTHTYPE*)obj->getGdmlRangeCanonic()) {
			char_class += 2;
			if (character >= c && character <= char_class[-1])
				return result;
		}
		else if (character == c)
			return result;
	}

	return !result;
}
