#ifndef INCLUDE_alt_use_sec
#define INCLUDE_alt_use_sec
/*
 *
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
 *       Mark O'Donohue <mark.odonohue@ludwig.edu.au>
 *       John Bellardo  <bellardo@cs.ucsd.edu>
 *
 *
 *  $Id: alt_use_sec.h,v 1.3 2002-01-06 13:06:36 skywalker Exp $
 *
 */


// During the build process in the initial stages the isc4.gdb database does
// not exist, but to build the isc4.gdb database the tools need to be built.
// So the following #define is set/unset in the makefile depending upon which
// stage in the compile process we are up to.
//
// In the earlier stages it is set to "1" then after we have build a set of
// tools mainly gpre_static and gbak_static it is changed to an "undef".
//
// The alt_use_sec.h file is built from alt_use_sec.h.pre using sed 
// See the Makefile.in.jrd for the two places where the .h file is built.
//
// The build is done this way to allow the makefile to have a simple set
// of dependancies to determine the makeup and up to date check for 
// libgds.a/so.

#define PHASE_1_BUILD_NO_SECURITY_DB 

#endif
