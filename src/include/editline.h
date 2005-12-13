#ifndef INCLUDE_EDITLINE_H
#define INCLUDE_EDITLINE_H

/*
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * You may obtain a copy of the Licence at
 * http://www.gnu.org/licences/lgpl.html
 * 
 * As a special exception this file can also be included in modules
 * with other source code as long as that source code has been 
 * released under an Open Source Initiative certificed licence.  
 * More information about OSI certification can be found at: 
 * http://www.opensource.org 
 * 
 * This module is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public Licence for more details.
 * 
 * This module was created by members of the firebird development 
 * team.  All individual contributions remain the Copyright (C) of 
 * those individuals and all rights are reserved.  Contributors to 
 * this file are either listed below or can be obtained from a CVS 
 * history command.
 *
 *  Created by:  Mark O'Donohue <mark.odonohue@ludwig.edu.au>
 *
 *  Contributor(s):
 * 
 *
 *  $Id: editline.h,v 1.1 2002-10-07 01:29:11 skywalker Exp $
 *
 */


// This file defines the ansi headers for the editline library which
// did not have it's own ansi header. This is an external module
// which we have included to replace readline which does not
// have a compatible licence for us to use.  MOD 7-Oct-2002

#ifdef __cplusplus
extern "C" {
#endif

extern char *readline(const char*);
extern void add_history(char*);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
