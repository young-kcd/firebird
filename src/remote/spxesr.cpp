/*
 *	PROGRAM:	JRD Remote Interface
 *	MODULE:		spxesr.c
 *	DESCRIPTION:	Windows SPX communications callback function
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

#include <windows.h>
#include <dos.h>
#include "../jrd/common.h"

/* 
 * Need to undef FAILURE from common.h (#define FAILURE 1) and MIN from 
 * common.h. (#define MIN(a,b)  (((a) < (b)) ? (a) : (b)).  They get 
 * redefined by the netware include files, included with spxwin.h, as 
 * #define FAILURE  -1 and #define MIN(a,b)  ((a) < (b) ? (a) : (b)).
*/
#ifdef FAILURE
#undef FAILURE
#endif

#ifdef MIN(a,b)
#undef MIN(a,b)
#endif

#include "../remote/spxwin.h"

extern spx_port_handle *head_of_spx_port_list;

/* turn off stack checking for this routine */
#pragma option -w
#pragma option -N-

void process_async_data()
{
/**************************************
*
* p r o c e s s _ a s y n c _ d a t a
*
* Event service routine called by spx driver.
* This routine must be in a fixed code segment,
* and so it should be in it's own source file so
* that you reduce the size of the segment which 
* will use up precious fixed memory.
*
**************************************/
	spx_port_handle FAR *handle;
	ECB FAR *ecb;
	ECB FAR *rx_ecb;
	UCHAR FAR *rx_mem;
	int i;

	ecb = MK_FP(_ES, _SI);

/* match up the ecb with a port handle */
	handle = head_of_spx_port_list;
	while (handle != 0) {
		rx_mem = handle->rx_pool;
		i = 0;
		while (i < RX_POOL_SIZE) {
			rx_mem += MAX_DATA;
			rx_ecb = (ECB *) rx_mem;
			if (rx_ecb == ecb)
				break;
			rx_mem += sizeof(ECB);
			rx_mem += sizeof(SPXHeader);

			i++;
		}
		if (rx_ecb == ecb)
			break;
		handle = handle->next;
	}

/* post the message */
	if (handle)
		PostMessage(handle->window_handle, WM_USER + 1, 0, 0);
}
