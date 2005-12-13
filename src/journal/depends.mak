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
# depends.mak - journal
# Created by 'make depends.mak'
# Created on 1998-11-17
archive.o: archive.c
archive.o: source/jrd/common.h
archive.o: source/jrd/dsc.h
archive.o: source/jrd/isc.h
archive.o: source/jrd/jrn.h
archive.o: source/jrd/jrn_proto.h
archive.o: source/jrd/llio.h
archive.o: source/jrd/llio_proto.h
archive.o: source/jrd/misc.h
archive.o: source/jrd/thd.h
archive.o: source/wal/wal.h
console.o: conso_proto.h
console.o: console.c
console.o: gjrn_proto.h
console.o: journal.h
console.o: misc_proto.h
console.o: source/jrd/blk.h
console.o: source/jrd/common.h
console.o: source/jrd/dsc.h
console.o: source/jrd/gdsassert.h
console.o: source/jrd/isc.h
console.o: source/jrd/isc_f_proto.h
console.o: source/jrd/jrd.h
console.o: source/jrd/jrn.h
console.o: source/jrd/thd.h
gjrn.o: conso_proto.h
gjrn.o: gjrn.c
gjrn.o: gjrn_proto.h
gjrn.o: journal.h
gjrn.o: misc_proto.h
gjrn.o: rebui_proto.h
gjrn.o: serve_proto.h
gjrn.o: source/interbase/include/iberror.h
gjrn.o: source/jrd/build_no.h
gjrn.o: source/jrd/common.h
gjrn.o: source/jrd/fil.h
gjrn.o: source/jrd/gds.h
gjrn.o: source/jrd/gds_proto.h
gjrn.o: source/jrd/license.h
gjrn.o: source/jrd/thd.h
misc.o: gjrn_proto.h
misc.o: journal.h
misc.o: misc.c
misc.o: misc_proto.h
misc.o: source/interbase/include/iberror.h
misc.o: source/jrd/build_no.h
misc.o: source/jrd/common.h
misc.o: source/jrd/fil.h
misc.o: source/jrd/gds.h
misc.o: source/jrd/gds_proto.h
misc.o: source/jrd/jrn.h
misc.o: source/jrd/license.h
misc.o: source/jrd/ods.h
misc.o: source/jrd/thd.h
misc.o: source/jrd/time.h
oldr.o: misc_proto.h
oldr.o: oldr.c
oldr.o: oldr_proto.h
oldr.o: source/jrd/common.h
oldr.o: source/jrd/llio.h
oldr.o: source/jrd/llio_proto.h
oldr.o: source/jrd/old.h
rebuild.o: conso_proto.h
rebuild.o: gjrn_proto.h
rebuild.o: journal.h
rebuild.o: misc_proto.h
rebuild.o: oldr_proto.h
rebuild.o: rebui_proto.h
rebuild.o: rebuild.c
rebuild.o: source/interbase/include/iberror.h
rebuild.o: source/jrd/btr.h
rebuild.o: source/jrd/build_no.h
rebuild.o: source/jrd/common.h
rebuild.o: source/jrd/dsc.h
rebuild.o: source/jrd/fil.h
rebuild.o: source/jrd/gds.h
rebuild.o: source/jrd/gds_proto.h
rebuild.o: source/jrd/isc.h
rebuild.o: source/jrd/isc_f_proto.h
rebuild.o: source/jrd/jrn.h
rebuild.o: source/jrd/license.h
rebuild.o: source/jrd/llio.h
rebuild.o: source/jrd/llio_proto.h
rebuild.o: source/jrd/misc.h
rebuild.o: source/jrd/ods.h
rebuild.o: source/jrd/old.h
rebuild.o: source/jrd/pio.h
rebuild.o: source/jrd/thd.h
rebuild.o: source/jrd/time.h
rebuild.o: source/wal/wal.h
rebuild.o: source/wal/walr_proto.h
server.o: conso_proto.h
server.o: gjrn_proto.h
server.o: journal.h
server.o: misc_proto.h
server.o: serve_proto.h
server.o: server.c
server.o: source/interbase/include/iberror.h
server.o: source/jrd/build_no.h
server.o: source/jrd/common.h
server.o: source/jrd/dsc.h
server.o: source/jrd/fil.h
server.o: source/jrd/gds.h
server.o: source/jrd/gds_proto.h
server.o: source/jrd/isc.h
server.o: source/jrd/isc_f_proto.h
server.o: source/jrd/isc_proto.h
server.o: source/jrd/isc_s_proto.h
server.o: source/jrd/jrn.h
server.o: source/jrd/license.h
server.o: source/jrd/llio.h
server.o: source/jrd/llio_proto.h
server.o: source/jrd/misc.h
server.o: source/jrd/pio.h
server.o: source/jrd/thd.h
server.o: source/jrd/time.h
server.o: source/wal/wal.h
server.o: source/wal/walf_proto.h
