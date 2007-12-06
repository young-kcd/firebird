/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		que.h
 *	DESCRIPTION:	Que manipulation macros
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

#ifndef _JRD_QUE_H_
#define _JRD_QUE_H_

#define QUE_INIT(que) {\
	(que).que_backward = &(que);\
	(que).que_forward = &(que);}

#define QUE_DELETE(node) {\
	(node).que_backward->que_forward = (node).que_forward;\
	(node).que_forward->que_backward = (node).que_backward;}

/* QUE_INSERT adds node to queue head. */

#define QUE_INSERT(que, node) {\
	(node).que_forward = (que).que_forward;\
	(node).que_backward = &(que);\
	(que).que_forward->que_backward = &(node);\
	(que).que_forward = &(node);}

/* QUE_APPEND adds node to queue tail. */

#define QUE_APPEND(que, node) {\
	(node).que_forward = &(que);\
	(node).que_backward = (que).que_backward;\
	(que).que_backward->que_forward = &(node);\
	(que).que_backward = &(node);}

/* QUE_LOOP to visit every node. */

#define QUE_LOOP(que, node) {\
	for (node = (que).que_forward; node != &(que); node = (node)->que_forward)

#define QUE_LOOPA(que, node) {\
	for (node = (que)->que_forward; node != que; node = (node)->que_forward)

#define QUE_NOT_EMPTY(que) ((que).que_forward != &(que))
#define QUE_EMPTY(que) ((que).que_forward == &(que))

#define QUE_MOST_RECENTLY_USED(in_use_que) {\
	QUE_DELETE (in_use_que);\
	QUE_INSERT (bcb->bcb_in_use, in_use_que);}

#define QUE_LEAST_RECENTLY_USED(in_use_que) {\
	QUE_DELETE (in_use_que);\
	QUE_APPEND (bcb->bcb_in_use, in_use_que);}
#endif /* _JRD_QUE_H_ */
