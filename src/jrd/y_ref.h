#ifndef JRD_Y_REF_H
#define JRD_Y_REF_H

/*
 *  The contents of this file are subject to the Mozilla Public
 *  License Version 1.1 (the "License"); you may not use this file
 *  except in compliance with the License. You may obtain a copy of
 *  the License at http://www.mozilla.org/MPL/
 *  Alternatively, the contents of this file may be used under the
 *  terms of the GNU General Public License Version 2 or later (the
 *  "GPL"), in which case the provisions of the GPL are applicable
 *  instead of those above. You may obtain a copy of the Licence at
 *  http://www.gnu.org/copyleft/gpl.html
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Relevant for more details.
 *
 *  This file was created by members of the firebird development team.
 *  All individual contributions remain the Copyright (C) of those
 *  individuals.  Contributors to this file are either listed here or
 *  can be obtained from a CVS history command.
 *
 *  All rights reserved.
 *
 *  Contributor(s):
 *       Alex Peshkoff <peshkoff@hotmail.ru> *
 *
 */

/*
 * This file is used for one and only one purpose:
 * distinguish places, where internals of struct why_hndl 
 * is known, and where only the fact, that such structure
 * exists, is required. In latter case we include y_ref
 * instead of y_handle.
 *
 * This file (or y_handle.h) must be included BEFORE ibase.h !
 */

#define FRBRD struct why_hndl
#endif  // JRD_Y_REF_H
