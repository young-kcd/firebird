# The contents of this file are subject to the Interbase Public
# License Version 1.0 (the "License"); you may not use this file
# except in compliance with the License. You may obtain a copy
# of the License at http://www.Inprise.com/IPL.html
#
# Software distributed under the License is distributed on an
# "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
# or implied. See the License for the specific language governing
# rights and limitations under the License.
#
# The Original Code was created by Inprise Corporation
# and its predecessors. Portions created by Inprise Corporation are
# Copyright (C) Inprise Corporation.
#
# All Rights Reserved.
# Contributor(s): ______________________________________.
# depends.mak - lock
# Created by 'make depends.mak'
# Created on 1998-11-17
driver.o: driver.c
driver.o: source/interbase/include/iberror.h
driver.o: source/lock/lock.h
driver.o: source/lock/source/jrd/common.h
driver.o: source/lock/source/jrd/file_params.h
driver.o: source/jrd/common.h
driver.o: source/jrd/gds.h
driver.o: source/jrd/isc.h
driver.o: source/jrd/thd.h
lock.o: lock.c
lock.o: source/lock/lock.h
lock.o: source/lock/lock_proto.h
lock.o: source/lock/source/jrd/common.h
lock.o: source/lock/source/jrd/file_params.h
lock.o: source/jrd/codes.h
lock.o: source/jrd/common.h
lock.o: source/jrd/fil.h
lock.o: source/jrd/gds_proto.h
lock.o: source/jrd/isc.h
lock.o: source/jrd/isc_i_proto.h
lock.o: source/jrd/isc_proto.h
lock.o: source/jrd/isc_s_proto.h
lock.o: source/jrd/sch_proto.h
lock.o: source/jrd/thd.h
lock.o: source/jrd/thd_proto.h
lock.o: source/jrd/time.h
manager.o: manager.c
manager.o: source/lock/lock_proto.h
manager.o: source/jrd/blk.h
manager.o: source/jrd/common.h
manager.o: source/jrd/dsc.h
manager.o: source/jrd/gdsassert.h
manager.o: source/jrd/isc.h
manager.o: source/jrd/jrd.h
manager.o: source/jrd/lck.h
manager.o: source/jrd/thd.h
pls.o: lock.h
pls.o: pls.c
pls.o: plserver.h
pls.o: source/jrd/common.h
pls.o: source/jrd/file_params.h
plserver.o: plserver.c
plserver.o: plserver.h
plserver.o: source/jrd/common.h
print.o: print.c
print.o: prtv3_proto.h
print.o: source/lock/lock.h
print.o: source/lock/source/jrd/common.h
print.o: source/lock/source/jrd/file_params.h
print.o: source/jrd/blk.h
print.o: source/jrd/common.h
print.o: source/jrd/dsc.h
print.o: source/jrd/fil.h
print.o: source/jrd/file_params.h
print.o: source/jrd/gds_proto.h
print.o: source/jrd/gdsassert.h
print.o: source/jrd/isc.h
print.o: source/jrd/isc_proto.h
print.o: source/jrd/isc_s_proto.h
print.o: source/jrd/jrd.h
print.o: source/jrd/lck.h
print.o: source/jrd/thd.h
print.o: source/jrd/time.h
printv3.o: fparamv3.h
printv3.o: lockv3.h
printv3.o: printv3.c
printv3.o: prtv3_proto.h
printv3.o: source/jrd/common.h
printv3.o: source/jrd/isc.h
printv3.o: source/jrd/thd.h
printv3s4.o: fparamv3.h
printv3s4.o: lockv3s4.h
printv3s4.o: printv3s4.c
printv3s4.o: prtv3_proto.h
printv3s4.o: source/jrd/common.h
reset.o: lock.h
reset.o: reset.c
reset.o: source/jrd/common.h
reset.o: source/jrd/file_params.h
ulimit.o: ulimit.c
