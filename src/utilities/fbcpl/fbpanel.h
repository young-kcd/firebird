/*
 *  PROGRAM:        Firebird 1.5 control panel applet
 *  MODULE:         fbpanel.h
 *
 * The contents of this file are subject to the Independant Developer's 
 * Public License Version 1.0 (the "License"); you may not use this
 * file except in compliance with the License. You may obtain a copy
 * of the License at http://www.ibphoenix.com/idpl.html
 *
 * Software distributed under the License is distributed on an
 * "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
 * or implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code was created by Paul Reeves
 * Copyright (C) 2003 Paul Reeves
 *
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 *
 *
 */


#if !defined(_FBPanel_)
#define _FBPanel_

#include "ctrlpan.h"

class CFBPanel : public CControlPanel
{
public:
    virtual LONG OnInquire(UINT uAppNum, NEWCPLINFO* pInfo); 
    virtual LONG OnDblclk(HWND hwndCPl, UINT uAppNum, LONG lData); 

};

#endif