/*
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
#include <stdio.h>
#include <stdlib.h>

#define CANT_MAP_CHARACTER	0
#define UNICODE_REPLACEMENT_CHARACTER	0xFFFD

class codepage_map {
  public:
	virtual unsigned short to_unicode(unsigned short c) {
		return 0;
	} virtual unsigned short from_unicode(unsigned short c) {
		return 0;
	} char *name;
};

class narrow_codepage_map:public codepage_map { public:
	unsigned short *to_unicode_map;
	unsigned char *from_unicode_mapping_array;
	unsigned short *from_unicode_map;
	unsigned long sizeof_to_unicode_map;
	unsigned long sizeof_from_unicode_mapping_array;
	unsigned long sizeof_from_unicode_map;

	  narrow_codepage_map(void) {
		to_unicode_map = NULL;
		from_unicode_map = NULL;
		from_unicode_mapping_array = NULL;
		sizeof_to_unicode_map = 0;
		sizeof_from_unicode_map = 0;
		sizeof_from_unicode_mapping_array = 0;
		name = NULL;
	};

	narrow_codepage_map(unsigned short *_to,
						unsigned char *_from_array,
						unsigned short *_from_map,
						unsigned long sizeof_to,
						unsigned long sizeof_from_array,
						unsigned long sizeof_from_map, char *_name) {
		to_unicode_map = _to;
		from_unicode_mapping_array = _from_array;
		from_unicode_map = _from_map;
		sizeof_to_unicode_map = sizeof_to;
		sizeof_from_unicode_mapping_array = sizeof_from_array;
		sizeof_from_unicode_map = sizeof_from_map;
		name = _name;
		test_codepage();
	};

	unsigned short to_unicode(unsigned short c) {
		if (c >= 256)
			fprintf(stderr, "%s: to_unicode(%d) not supported\n", name, c);
		return to_unicode_map[c];
	};

	unsigned short from_unicode(unsigned short c) {
		return from_unicode_mapping_array[from_unicode_map[c / 256] +
										  (c % 256)];
	};

	void test_codepage(void) {
		if (sizeof_to_unicode_map != 256 * sizeof(to_unicode_map[0]))
			printf("%s: to_unicode_map too small! %d entries!\n", name,
				   sizeof_to_unicode_map / sizeof(to_unicode_map[0]));

		if (sizeof_from_unicode_map != 256 * sizeof(from_unicode_map[0]))
			printf("%s: from_unicode_map too small! %d entries!\n", name,
				   sizeof_from_unicode_map / sizeof(from_unicode_map[0]));

		for (int i = 0; i <= 255; i++) {
			unsigned short uch;
			unsigned char ch2;
			  uch = to_unicode((unsigned char) i);
			if (uch == CANT_MAP_CHARACTER)
				  continue;
			  ch2 = from_unicode(uch);
			if (ch2 != i) {
				printf
					("%s: Roundtrip mapping error: Character %02x -> Unicode %04x -> Char %02x\n",
					 name, i, uch, ch2);

				/* Find the Character in the from_unicode_mapping_array */
				for (int j = 0; j < sizeof_from_unicode_mapping_array; j++)
					if (from_unicode_mapping_array[j] == i) {
						/* Mapping table is wrong - recommend a fix for it */
						printf
							("%s: Recommend from_unicode_map[0x%02x] be set to %d\n",
							 name, uch / 256, j - (uch % 256));
						break;
				} if (j == sizeof_from_unicode_mapping_array) {
					/* Oops - found a character that does exists in the character set
					   but not in unicode - an obvious error! */

					printf
						("%s: Roundtrip error: No Unicode maps to codepage character 0x%X\n",
						 name, i);

				}
			}
		}

		for (i = 0; i <= 255; i++) {
			if (from_unicode_map[i] + 0xFF >=
				sizeof_from_unicode_mapping_array /
				sizeof(from_unicode_mapping_array[0])) {
				printf
					("%s: from_unicode_map bounds error at position %02x00\n",
					 name, i);
				continue;
			}

			for (int j = 0; j <= 255; j++) {
				unsigned short uch, uch2;
				unsigned char ch2;

				uch = i * 256 + j;
				ch2 = from_unicode(uch);
				if (ch2 == CANT_MAP_CHARACTER)
					continue;
				uch2 = to_unicode(ch2);
				if (uch != uch2) {
					printf
						("%s: roundtrip mapping error: Unicode %04x -> ch %02x -> Unicode %04x\n",
						 name, uch, ch2, uch2);

					for (int k = 0; k <= 255; k++)
						if (to_unicode_map[k] == uch) {
							/* Can map this character from charset to unicode */
							/* Assume fix was printed out above */
						}
					if (k > 255) {
						/* This unicode doesn't exist in charset */
						/* Mapping table is wrong - recommend a fix for it */
						printf
							("%s: recommend from_unicode_map[0x%02x] be set to %d\n",
							 name, uch / 256, 0);
					}
				}
			}
		}

		printf("Test of %s completed\n", name);
	};
};

class wide_codepage_map:public codepage_map {

  public:
	unsigned short *to_unicode_mapping_array;
	unsigned short *to_unicode_map;
	unsigned short *from_unicode_mapping_array;
	unsigned short *from_unicode_map;
	unsigned long sizeof_to_unicode_mapping_array;
	unsigned long sizeof_to_unicode_map;
	unsigned long sizeof_from_unicode_mapping_array;
	unsigned long sizeof_from_unicode_map;

	  wide_codepage_map(void) {
		to_unicode_map = NULL;
		to_unicode_mapping_array = NULL;
		from_unicode_map = NULL;
		from_unicode_mapping_array = NULL;
		sizeof_to_unicode_map = 0;
		sizeof_to_unicode_mapping_array = 0;
		sizeof_from_unicode_map = 0;
		sizeof_from_unicode_mapping_array = 0;
		name = NULL;
	};

	wide_codepage_map(unsigned short *_to_array,
					  unsigned short *_to,
					  unsigned short *_from_array,
					  unsigned short *_from_map,
					  unsigned long sizeof_to_array,
					  unsigned long sizeof_to,
					  unsigned long sizeof_from_array,
					  unsigned long sizeof_from_map, char *_name) {
		to_unicode_mapping_array = _to_array;
		to_unicode_map = _to;
		from_unicode_mapping_array = _from_array;
		from_unicode_map = _from_map;
		sizeof_to_unicode_mapping_array = sizeof_to_array;
		sizeof_to_unicode_map = sizeof_to;
		sizeof_from_unicode_mapping_array = sizeof_from_array;
		sizeof_from_unicode_map = sizeof_from_map;
		name = _name;
		test_codepage();
	};

	unsigned short to_unicode(unsigned short c) {
		return to_unicode_mapping_array[to_unicode_map[c / 256] + (c % 256)];
	};

	unsigned short from_unicode(unsigned short c) {
		return from_unicode_mapping_array[from_unicode_map[c / 256] +
										  (c % 256)];
	};

	void test_codepage(void) {
		if (sizeof_to_unicode_map != 256 * sizeof(to_unicode_map[0]))
			printf("%s: to_unicode_map too small! %d entries!\n",
				   name, sizeof_to_unicode_map / sizeof(to_unicode_map[0]));

		if (sizeof_from_unicode_map != 256 * sizeof(from_unicode_map[0]))
			printf("%s: from_unicode_map is too small! %d entries!\n",
				   name,
				   sizeof_from_unicode_map / sizeof(from_unicode_map[0]));


/* Map from Each possible character sequence to Unicode */

		for (int i = 0; i <= 255; i++) {
			if (to_unicode_map[i] + 0xFF >=
				sizeof_to_unicode_mapping_array /
				sizeof(to_unicode_mapping_array[0])) {
				printf
					("%s: to_unicode_map array bounds error at position %02x00\n",
					 name, i);
				continue;
			};

			for (int j = 0; j <= 255; j++) {
				unsigned short uch;
				unsigned short ch2;
				unsigned short wide_ch;
				wide_ch = 256 * i + j;
				uch = to_unicode(wide_ch);
				if (uch == CANT_MAP_CHARACTER)
					continue;
				if (uch == UNICODE_REPLACEMENT_CHARACTER)
					continue;
				ch2 = from_unicode(uch);
				if (ch2 != wide_ch) {
					printf
						("%s: roundtrip mapping error: Character %04x -> Unicode %04x -> Char %04x\n",
						 name, wide_ch, uch, ch2);

					/* Find the Character in the from_unicode_mapping_array */
					for (int k = 0;
						 k <
						 sizeof_from_unicode_mapping_array /
						 sizeof(from_unicode_mapping_array[0]); k++)
						if (from_unicode_mapping_array[k] == wide_ch) {
							/* Mapping table is wrong - recommend a fix for it */
							printf
								("%s: Recommend from_unicode_map[0x%02x] be set to %d\n",
								 name, uch / 256, k - (uch % 256));
							break;
						}

					if (k ==
						sizeof_from_unicode_mapping_array /
						sizeof(from_unicode_mapping_array[0])) {
						/* Oops - found a character that does exists in the character set
						   but not in unicode - an obvious error! */

						printf
							("%s: Roundtrip error: No Unicode maps to codepage character 0x%X\n",
							 name, wide_ch);

					}
				}
			}
		}

		for (i = 0; i <= 255; i++) {
			if (from_unicode_map[i] + 0xFF >=
				sizeof_from_unicode_mapping_array /
				sizeof(from_unicode_mapping_array[0])) {
				printf
					("%s: From_unicode_map array bounds error at position %02x00\n",
					 name, i);
				continue;
			}

			for (int j = 0; j <= 255; j++) {
				unsigned short uch, uch2;
				unsigned short ch2;

				uch = i * 256 + j;
				ch2 = from_unicode(uch);
				if (ch2 == CANT_MAP_CHARACTER)
					continue;
				uch2 = to_unicode(ch2);
				if (uch != uch2) {
					printf
						("%s: roundtrip mapping error: Unicode %04x -> ch %02x -> Unicode %04x\n",
						 name, uch, ch2, uch2);

					for (int k = 0;
						 k <
						 sizeof_to_unicode_mapping_array /
						 sizeof(to_unicode_mapping_array[0]); k++)
						if (to_unicode_map[k] == uch) {
							/* Can map this character from charset to unicode */
							/* Assume fix was printed out above */
						}
					if (k <
						sizeof_to_unicode_mapping_array /
						sizeof(to_unicode_mapping_array[0])) {
						/* This unicode doesn't exist in charset */
						/* Mapping table is wrong - recommend a fix for it */
						printf
							("%s: Recommend from_unicode_map[0x%02x] be set to %d\n",
							 name, uch / 256, 0);
					}
				}
			}
		}

		printf("Test of %s completed\n", name);
	};
};

class pair_codepage_map {
  private:
	int need_name;
	char *subheading;

	void print_name(char *msg) {
		if (need_name == 0) {
			printf("Comparision of %s and %s\n", old_map->name,
				   new_map->name);
			this->need_name++;
		};
		if (subheading != msg) {
			printf("%s\n", msg);
			this->subheading = msg;
		};
	};

  public:
	codepage_map * old_map;
	codepage_map *new_map;

	pair_codepage_map(codepage_map * _old, codepage_map * _new) {
		need_name = 0;
		subheading = NULL;
		old_map = _old;
		new_map = _new;
	};

	pair_codepage_map(narrow_codepage_map * _old, narrow_codepage_map * _new) {
		need_name = 0;
		subheading = NULL;
		old_map = (codepage_map *) _old;
		new_map = (codepage_map *) _new;
	};

	void compare_to_unicode(void) {
		for (int i = 0; i < 256; i++)
			for (int j = 0; j < 256; j++) {
				unsigned short cp = 256 * i + j;
				if (old_map->to_unicode(cp) != new_map->to_unicode(cp)) {
					print_name("Codepage to Unicode Test");
					printf("%04x\tOLD: %04x\tNEW: %04x\n",
						   cp, old_map->to_unicode(cp),
						   new_map->to_unicode(cp));
				};
			};
	};

	void compare_from_unicode(void) {
		for (int i = 0; i < 256; i++)
			for (int j = 0; j < 256; j++) {
				unsigned short unicode = 256 * i + j;
				if (old_map->from_unicode(unicode) !=
					new_map->from_unicode(unicode)) {
					print_name("Unicode to codepage Test");
					printf("%04x\tOLD: %04x\tNEW: %04x\n", unicode,
						   old_map->from_unicode(unicode),
						   new_map->from_unicode(unicode));
				};
			};
	};

	void compare(void) {
		compare_to_unicode();
		compare_from_unicode();
	};

};

/*
typedef unsigned short	USHORT;
typedef unsigned char	UCHAR;

*/
#define UCHAR	unsigned char
#define USHORT	unsigned short
#define FAR_VARIABLE
#define CONST
#define _JRD_COMMON_H_


#ifdef XXX

pair_codepage_map *get_w1250(void)
{
	narrow_codepage_map *old_map, *new_map;
	{
#include "previous/cs_w1250.h"
		old_map =
			new narrow_codepage_map(to_unicode_map,
									from_unicode_mapping_array,
									from_unicode_map, sizeof(to_unicode_map),
									sizeof(from_unicode_mapping_array),
									sizeof(from_unicode_map),
									"previous/cs_w1250.h");
	}

	{
#include "new/cs_w1250.h"
		new_map =
			new narrow_codepage_map(to_unicode_map,
									from_unicode_mapping_array,
									from_unicode_map, sizeof(to_unicode_map),
									sizeof(from_unicode_mapping_array),
									sizeof(from_unicode_map),
									"new/cs_w1250.h");
	}
	return new pair_codepage_map((codepage_map *) old_map,
								 (codepage_map *) new_map);
};

pair_codepage_map *get_w1251(void)
{
	narrow_codepage_map *old_map, *new_map;
	{
#include "previous/cs_w1251.h"
		old_map =
			new narrow_codepage_map(to_unicode_map,
									from_unicode_mapping_array,
									from_unicode_map, sizeof(to_unicode_map),
									sizeof(from_unicode_mapping_array),
									sizeof(from_unicode_map),
									"previous/cs_w1251.h");
	}

	{
#include "new/cs_w1251.h"
		new_map =
			new narrow_codepage_map(to_unicode_map,
									from_unicode_mapping_array,
									from_unicode_map, sizeof(to_unicode_map),
									sizeof(from_unicode_mapping_array),
									sizeof(from_unicode_map),
									"new/cs_w1251.h");
	}
	return new pair_codepage_map(old_map, new_map);
};

pair_codepage_map *get_w1252(void)
{
	narrow_codepage_map *old_map, *new_map;
	{
#include "previous/cs_w1252.h"
		old_map =
			new narrow_codepage_map(to_unicode_map,
									from_unicode_mapping_array,
									from_unicode_map, sizeof(to_unicode_map),
									sizeof(from_unicode_mapping_array),
									sizeof(from_unicode_map),
									"previous/cs_w1252.h");
	}

	{
#include "new/cs_w1252.h"
		new_map =
			new narrow_codepage_map(to_unicode_map,
									from_unicode_mapping_array,
									from_unicode_map, sizeof(to_unicode_map),
									sizeof(from_unicode_mapping_array),
									sizeof(from_unicode_map),
									"new/cs_w1252.h");
	}
	return new pair_codepage_map(old_map, new_map);
};

pair_codepage_map *get_w1253(void)
{
	narrow_codepage_map *old_map, *new_map;
	{
#include "previous/cs_w1253.h"
		old_map =
			new narrow_codepage_map(to_unicode_map,
									from_unicode_mapping_array,
									from_unicode_map, sizeof(to_unicode_map),
									sizeof(from_unicode_mapping_array),
									sizeof(from_unicode_map),
									"previous/cs_w1253.h");
	}

	{
#include "new/cs_w1253.h"
		new_map =
			new narrow_codepage_map(to_unicode_map,
									from_unicode_mapping_array,
									from_unicode_map, sizeof(to_unicode_map),
									sizeof(from_unicode_mapping_array),
									sizeof(from_unicode_map),
									"new/cs_w1253.h");
	}
	return new pair_codepage_map(old_map, new_map);
};

pair_codepage_map *get_w1254(void)
{
	narrow_codepage_map *old_map, *new_map;
	{
#include "previous/cs_w1254.h"
		old_map =
			new narrow_codepage_map(to_unicode_map,
									from_unicode_mapping_array,
									from_unicode_map, sizeof(to_unicode_map),
									sizeof(from_unicode_mapping_array),
									sizeof(from_unicode_map),
									"previous/cs_w1254.h");
	}

	{
#include "new/cs_w1254.h"
		new_map =
			new narrow_codepage_map(to_unicode_map,
									from_unicode_mapping_array,
									from_unicode_map, sizeof(to_unicode_map),
									sizeof(from_unicode_mapping_array),
									sizeof(from_unicode_map),
									"new/cs_w1254.h");
	}
	return new pair_codepage_map(old_map, new_map);
};

pair_codepage_map *get_437(void)
{
	narrow_codepage_map *old_map, *new_map;
	{
#include "previous/cs_437.h"
		old_map =
			new narrow_codepage_map(to_unicode_map,
									from_unicode_mapping_array,
									from_unicode_map, sizeof(to_unicode_map),
									sizeof(from_unicode_mapping_array),
									sizeof(from_unicode_map),
									"previous/cs_437.h");
	}

	{
#include "new/cs_437.h"
		new_map =
			new narrow_codepage_map(to_unicode_map,
									from_unicode_mapping_array,
									from_unicode_map, sizeof(to_unicode_map),
									sizeof(from_unicode_mapping_array),
									sizeof(from_unicode_map), "new/cs_437.h");
	}
	return new pair_codepage_map(old_map, new_map);
};

pair_codepage_map *get_850(void)
{
	narrow_codepage_map *old_map, *new_map;
	{
#include "previous/cs_850.h"
		old_map =
			new narrow_codepage_map(to_unicode_map,
									from_unicode_mapping_array,
									from_unicode_map, sizeof(to_unicode_map),
									sizeof(from_unicode_mapping_array),
									sizeof(from_unicode_map),
									"previous/cs_850.h");
	}

	{
#include "new/cs_850.h"
		new_map =
			new narrow_codepage_map(to_unicode_map,
									from_unicode_mapping_array,
									from_unicode_map, sizeof(to_unicode_map),
									sizeof(from_unicode_mapping_array),
									sizeof(from_unicode_map), "new/cs_850.h");
	}
	return new pair_codepage_map(old_map, new_map);
};

pair_codepage_map *get_852(void)
{
	narrow_codepage_map *old_map, *new_map;
	{
#include "previous/cs_852.h"
		old_map =
			new narrow_codepage_map(to_unicode_map,
									from_unicode_mapping_array,
									from_unicode_map, sizeof(to_unicode_map),
									sizeof(from_unicode_mapping_array),
									sizeof(from_unicode_map),
									"previous/cs_852.h");
	}

	{
#include "new/cs_852.h"
		new_map =
			new narrow_codepage_map(to_unicode_map,
									from_unicode_mapping_array,
									from_unicode_map, sizeof(to_unicode_map),
									sizeof(from_unicode_mapping_array),
									sizeof(from_unicode_map), "new/cs_852.h");
	}
	return new pair_codepage_map(old_map, new_map);
};

pair_codepage_map *get_857(void)
{
	narrow_codepage_map *old_map, *new_map;
	{
#include "previous/cs_857.h"
		old_map =
			new narrow_codepage_map(to_unicode_map,
									from_unicode_mapping_array,
									from_unicode_map, sizeof(to_unicode_map),
									sizeof(from_unicode_mapping_array),
									sizeof(from_unicode_map),
									"previous/cs_857.h");
	}

	{
#include "new/cs_857.h"
		new_map =
			new narrow_codepage_map(to_unicode_map,
									from_unicode_mapping_array,
									from_unicode_map, sizeof(to_unicode_map),
									sizeof(from_unicode_mapping_array),
									sizeof(from_unicode_map), "new/cs_857.h");
	}
	return new pair_codepage_map(old_map, new_map);
};

pair_codepage_map *get_860(void)
{
	narrow_codepage_map *old_map, *new_map;
	{
#include "previous/cs_860.h"
		old_map =
			new narrow_codepage_map(to_unicode_map,
									from_unicode_mapping_array,
									from_unicode_map, sizeof(to_unicode_map),
									sizeof(from_unicode_mapping_array),
									sizeof(from_unicode_map),
									"previous/cs_860.h");
	}

	{
#include "new/cs_860.h"
		new_map =
			new narrow_codepage_map(to_unicode_map,
									from_unicode_mapping_array,
									from_unicode_map, sizeof(to_unicode_map),
									sizeof(from_unicode_mapping_array),
									sizeof(from_unicode_map), "new/cs_860.h");
	}
	return new pair_codepage_map(old_map, new_map);
};

pair_codepage_map *get_861(void)
{
	narrow_codepage_map *old_map, *new_map;
	{
#include "previous/cs_861.h"
		old_map =
			new narrow_codepage_map(to_unicode_map,
									from_unicode_mapping_array,
									from_unicode_map, sizeof(to_unicode_map),
									sizeof(from_unicode_mapping_array),
									sizeof(from_unicode_map),
									"previous/cs_861.h");
	}

	{
#include "new/cs_861.h"
		new_map =
			new narrow_codepage_map(to_unicode_map,
									from_unicode_mapping_array,
									from_unicode_map, sizeof(to_unicode_map),
									sizeof(from_unicode_mapping_array),
									sizeof(from_unicode_map), "new/cs_861.h");
	}
	return new pair_codepage_map(old_map, new_map);
};

pair_codepage_map *get_863(void)
{
	narrow_codepage_map *old_map, *new_map;
	{
#include "previous/cs_863.h"
		old_map =
			new narrow_codepage_map(to_unicode_map,
									from_unicode_mapping_array,
									from_unicode_map, sizeof(to_unicode_map),
									sizeof(from_unicode_mapping_array),
									sizeof(from_unicode_map),
									"previous/cs_863.h");
	}

	{
#include "new/cs_863.h"
		new_map =
			new narrow_codepage_map(to_unicode_map,
									from_unicode_mapping_array,
									from_unicode_map, sizeof(to_unicode_map),
									sizeof(from_unicode_mapping_array),
									sizeof(from_unicode_map), "new/cs_863.h");
	}
	return new pair_codepage_map(old_map, new_map);
};

pair_codepage_map *get_865(void)
{
	narrow_codepage_map *old_map, *new_map;
	{
#include "previous/cs_865.h"
		old_map =
			new narrow_codepage_map(to_unicode_map,
									from_unicode_mapping_array,
									from_unicode_map, sizeof(to_unicode_map),
									sizeof(from_unicode_mapping_array),
									sizeof(from_unicode_map),
									"previous/cs_865.h");
	}

	{
#include "new/cs_865.h"
		new_map =
			new narrow_codepage_map(to_unicode_map,
									from_unicode_mapping_array,
									from_unicode_map, sizeof(to_unicode_map),
									sizeof(from_unicode_mapping_array),
									sizeof(from_unicode_map), "new/cs_865.h");
	}
	return new pair_codepage_map(old_map, new_map);
};

pair_codepage_map *get_cyrl(void)
{
	narrow_codepage_map *old_map, *new_map;
	{
#include "previous/cs_cyrl.h"
		old_map =
			new narrow_codepage_map(to_unicode_map,
									from_unicode_mapping_array,
									from_unicode_map, sizeof(to_unicode_map),
									sizeof(from_unicode_mapping_array),
									sizeof(from_unicode_map),
									"previous/cs_cyrl.h");
	}

	{
#include "new/cs_cyrl.h"
		new_map =
			new narrow_codepage_map(to_unicode_map,
									from_unicode_mapping_array,
									from_unicode_map, sizeof(to_unicode_map),
									sizeof(from_unicode_mapping_array),
									sizeof(from_unicode_map),
									"new/cs_cyrl.h");
	}
	return new pair_codepage_map(old_map, new_map);
};

pair_codepage_map *get_next(void)
{
	narrow_codepage_map *old_map, *new_map;
	{
#include "previous/cs_next.h"
		old_map =
			new narrow_codepage_map(to_unicode_map,
									from_unicode_mapping_array,
									from_unicode_map, sizeof(to_unicode_map),
									sizeof(from_unicode_mapping_array),
									sizeof(from_unicode_map),
									"previous/cs_next.h");
	}

	{
#include "new/cs_next.h"
		new_map =
			new narrow_codepage_map(to_unicode_map,
									from_unicode_mapping_array,
									from_unicode_map, sizeof(to_unicode_map),
									sizeof(from_unicode_mapping_array),
									sizeof(from_unicode_map),
									"new/cs_next.h");
	}
	return new pair_codepage_map(old_map, new_map);
};

#endif /* XXX */

#ifdef XXX
pair_codepage_map *get_gb2312(void)
{
	wide_codepage_map *old_map, *new_map;
	{
#include "../intl/cs_gb2312.h"
		old_map = new wide_codepage_map(to_unicode_mapping_array,
										to_unicode_map,
										from_unicode_mapping_array,
										from_unicode_map,
										sizeof(to_unicode_mapping_array),
										sizeof(to_unicode_map),
										sizeof(from_unicode_mapping_array),
										sizeof(from_unicode_map),
										"previous/cs_gb2312.h");
	}

	{
#include "new/cs_gb2312.h"
		new_map = new wide_codepage_map(to_unicode_mapping_array,
										to_unicode_map,
										from_unicode_mapping_array,
										from_unicode_map,
										sizeof(to_unicode_mapping_array),
										sizeof(to_unicode_map),
										sizeof(from_unicode_mapping_array),
										sizeof(from_unicode_map),
										"new/cs_gb2312.h");
	}

	return new pair_codepage_map((codepage_map *) old_map,
								 (codepage_map *) new_map);
};

pair_codepage_map *get_jis(void)
{
	wide_codepage_map *old_map, *new_map;
	{
#include "../intl/cs_jis_0208_1990.h"
		old_map = new wide_codepage_map(to_unicode_mapping_array,
										to_unicode_map,
										from_unicode_mapping_array,
										from_unicode_map,
										sizeof(to_unicode_mapping_array),
										sizeof(to_unicode_map),
										sizeof(from_unicode_mapping_array),
										sizeof(from_unicode_map),
										"previous/cs_jis_0208_1990.h");
	}

	{
#include "new/cs_jis_0208_1990.h"
		new_map = new wide_codepage_map(to_unicode_mapping_array,
										to_unicode_map,
										from_unicode_mapping_array,
										from_unicode_map,
										sizeof(to_unicode_mapping_array),
										sizeof(to_unicode_map),
										sizeof(from_unicode_mapping_array),
										sizeof(from_unicode_map),
										"new/cs_jis_0208_1990.h");
	}

	return new pair_codepage_map((codepage_map *) old_map,
								 (codepage_map *) new_map);
};


pair_codepage_map *get_sjis(void)
{
	wide_codepage_map *old_map, *new_map;
	{
#include "new/cs_sjis.h"
		old_map = new wide_codepage_map(sjis_to_unicode_mapping_array,
										sjis_to_unicode_map,
										sjis_from_unicode_mapping_array,
										sjis_from_unicode_map,
										sizeof(sjis_to_unicode_mapping_array),
										sizeof(sjis_to_unicode_map),
										sizeof
										(sjis_from_unicode_mapping_array),
										sizeof(sjis_from_unicode_map),
										"previous/cs_sjis.h");
	}

	{
#include "new/cs_sjis.h"
		new_map = new wide_codepage_map(sjis_to_unicode_mapping_array,
										sjis_to_unicode_map,
										sjis_from_unicode_mapping_array,
										sjis_from_unicode_map,
										sizeof(sjis_to_unicode_mapping_array),
										sizeof(sjis_to_unicode_map),
										sizeof
										(sjis_from_unicode_mapping_array),
										sizeof(sjis_from_unicode_map),
										"new/cs_sjis.h");
	}

	return new pair_codepage_map((codepage_map *) old_map,
								 (codepage_map *) new_map);
};


pair_codepage_map *get_ksc5601(void)
{
	wide_codepage_map *old_map, *new_map;
	{
#include "../intl/cs_ksc5601.h"
		old_map = new wide_codepage_map(to_unicode_mapping_array,
										to_unicode_map,
										from_unicode_mapping_array,
										from_unicode_map,
										sizeof(to_unicode_mapping_array),
										sizeof(to_unicode_map),
										sizeof(from_unicode_mapping_array),
										sizeof(from_unicode_map),
										"previous/cs_ksc5601.h");
	}

	{
#include "new/cs_ksc5601.h"
		new_map = new wide_codepage_map(to_unicode_mapping_array,
										to_unicode_map,
										from_unicode_mapping_array,
										from_unicode_map,
										sizeof(to_unicode_mapping_array),
										sizeof(to_unicode_map),
										sizeof(from_unicode_mapping_array),
										sizeof(from_unicode_map),
										"new/cs_ksc5601.h");
	}

	return new pair_codepage_map((codepage_map *) old_map,
								 (codepage_map *) new_map);
};
#endif


pair_codepage_map *get_big5(void)
{
	wide_codepage_map *old_map, *new_map;
	{
#include "../intl/cs_big5.h"
		old_map = new wide_codepage_map(to_unicode_mapping_array,
										to_unicode_map,
										from_unicode_mapping_array,
										from_unicode_map,
										sizeof(to_unicode_mapping_array),
										sizeof(to_unicode_map),
										sizeof(from_unicode_mapping_array),
										sizeof(from_unicode_map),
										"previous/cs_big5.h");
	}

	{
#include "new/cs_big5.h"
		new_map = new wide_codepage_map(to_unicode_mapping_array,
										to_unicode_map,
										from_unicode_mapping_array,
										from_unicode_map,
										sizeof(to_unicode_mapping_array),
										sizeof(to_unicode_map),
										sizeof(from_unicode_mapping_array),
										sizeof(from_unicode_map),
										"new/cs_big5.h");
	}

	return new pair_codepage_map((codepage_map *) old_map,
								 (codepage_map *) new_map);
};







int main(int argc, char *argv[])
{
#ifdef XXX
	pair_codepage_map *gb2312 = get_gb2312();
	gb2312->compare();
	pair_codepage_map *jis = get_jis();
	jis->compare();
	pair_codepage_map *sjis = get_sjis();
	sjis->compare();
	pair_codepage_map *ksc5601 = get_ksc5601();
	ksc5601->compare();
#endif

	pair_codepage_map *big5 = get_big5();
	big5->compare();

#ifdef XXX
	pair_codepage_map *w1250 = get_w1250();
	w1250->compare();
	pair_codepage_map *w1251 = get_w1251();
	w1251->compare();
	pair_codepage_map *w1252 = get_w1252();
	w1252->compare();
	pair_codepage_map *w1253 = get_w1253();
	w1253->compare();
	pair_codepage_map *w1254 = get_w1254();
	w1254->compare();
	pair_codepage_map *d437 = get_437();
	d437->compare();
	pair_codepage_map *d850 = get_850();
	d850->compare();
	pair_codepage_map *d852 = get_852();
	d852->compare();
	pair_codepage_map *d857 = get_857();
	d857->compare();
	pair_codepage_map *d860 = get_860();
	d860->compare();
	pair_codepage_map *d861 = get_861();
	d861->compare();
	pair_codepage_map *d863 = get_863();
	d863->compare();
	pair_codepage_map *d865 = get_865();
	d865->compare();
	pair_codepage_map *cyrl = get_cyrl();
	cyrl->compare();
	pair_codepage_map *next = get_next();
	next->compare();
#endif /* XXX */
}
