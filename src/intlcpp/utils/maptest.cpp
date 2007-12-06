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

#include "firebird.h"
#include "../jrd/common.h"


/*
fixed
#include "cs_w1254.h"
#include "cs_cyrl.h"
#include "cs_865.h"
#include "cs_863.h"
#include "cs_861.h"
#include "cs_860.h"
#include "cs_857.h"
#include "cs_w1251.h"
#include "cs_437.h"
#include "cs_850.h"
#include "cs_852.h"
#include "cs_w1250.h"
#include "cs_w1252.h"
#include "cs_w1253.h"

fixed with errors
#include "cs_next.h" -- NeXT has two characters mapping to the same
			Unicode (FE & FF).  So round trip is not possible.
			They map to FFFD, which is the replacement character
			so this is not an error

No errors
#include "../intlcpp/cs_iso8859_1.h"
*/

/* 
-- Multibyte character sets --
#include "../intlcpp/cs_big5.h"
#include "../intlcpp/cs_gb2312.h"
#include "../intlcpp/cs_jis_0208_1990.h"
#include "../intlcpp/cs_ksc5601.h"
#include "../intlcpp/csjis2_p.h"
*/


#define UNICODE_INDEX(u)	(((u) % 256) + from_unicode_map [((u) / 256)])
#define UNICODE_LOOKUP(u)	from_unicode_mapping_array [ UNICODE_INDEX(u) ]

main()
{
	unsigned short uch, uch2;
	int i, j, k;
	unsigned char ch2;

	if (sizeof(to_unicode_map) != 256 * sizeof(to_unicode_map[0]))
		ib_printf("The to_unicode_map is too small! %d entries!\n",
				  sizeof(to_unicode_map) / sizeof(to_unicode_map[0]));

	if (sizeof(from_unicode_map) != 256 * sizeof(from_unicode_map[0]))
		ib_printf("The from_unicode_map is too small! %d entries!\n",
				  sizeof(from_unicode_map) / sizeof(from_unicode_map[0]));

	for (i = 0; i <= 255; i++) {
		uch = to_unicode_map[i];
		if (uch == CANT_MAP_CHARACTER)
			continue;
		ch2 = UNICODE_LOOKUP(uch);
		if (ch2 != i) {
			ib_printf
				("Mapping error: Character %02x -> Unicode %04x (index %3d) -> Char %02x\n",
				 i, uch, UNICODE_INDEX(uch), ch2);

			/* Find the Character in the from_unicode_mapping_array */
			for (j = 0; j < sizeof(from_unicode_mapping_array); j++)
				if (from_unicode_mapping_array[j] == i) {
					/* Mapping table is wrong - recommend a fix for it */
					ib_printf
						("Recommend from_unicode_map[0x%02x] be set to %d\n",
						 uch / 256, j - (uch % 256));
					break;
				}

			if (j == sizeof(from_unicode_mapping_array)) {
				/* Oops - found a character that does exists in the character set
				   but not in unicode - an obvious error! */

				ib_printf
					("Error: Character %d does not exist in the from_unicode_mapping_array\n",
					 i);

			}
		}
	}

	for (i = 0; i <= 255; i++) {
		if (from_unicode_map[i] + 0xFF >= sizeof(from_unicode_mapping_array)) {
			ib_printf("From_unicode array bounds error at position %02x00\n",
					  i);
			continue;
		}

		for (j = 0; j <= 255; j++) {
			uch = i * 256 + j;
			ch2 = UNICODE_LOOKUP(uch);
			if (ch2 == CANT_MAP_CHARACTER)
				continue;
			uch2 = to_unicode_map[ch2];
			if (uch != uch2) {
				ib_printf
					("Mapping error: Unicode %04x -> ch %02x -> Unicode %04x\n",
					 uch, ch2, uch2);

				for (k = 0; k <= 255; k++)
					if (to_unicode_map[k] == uch) {
						/* Can map this character from charset to unicode */
						/* Assume fix was printed out above */
					}
				if (k > 255) {
					/* This unicode doesn't exist in charset */
					/* Mapping table is wrong - recommend a fix for it */
					ib_printf
						("Recommend from_unicode_map[0x%02x] be set to %d\n",
						 uch / 256, 0);
				}
			}
		}
	}

	ib_printf("Test completed\n");
}
