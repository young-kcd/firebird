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
PKG:SharedControl:
description        = "ibase component description file"
distTreeRoot       = /usr/gds.sco6/custom
distTreeRootSHARED = /usr/gds.sco6/custom
distTreeRootCLIENT = /usr/gds.sco6/custom

DIR:SharedControl:SHARED:./cntl:
mode  = 0755
owner = root
group = sys
flags = 

FILE:SharedControl:SHARED:./cntl/ccs:
mode  = 0755
owner = root
group = sys
flags = 

PKG:ibase:
description        = "InterBase"
dependencies       = 
distTreeRootSHARED = $DEFAULT_DISTTREEROOT_SHARED
distTreeRootCLIENT = $DEFAULT_DISTTREEROOT_CLIENT

DIR:ibase:SHARED:./bin:
mode  = 0755
owner = root
group = bin
flags = 

FILE:ibase:SHARED:./bin/gpre:
mode  = 0555
owner = root
group = bin
flags = 
exportPath = ./bin/gpre

FILE:ibase:SHARED:./bin/gds_pipe:
mode  = 0555
owner = root
group = bin
flags = 
exportPath = ./bin/gds_pipe

FILE:ibase:SHARED:./bin/gfix:
mode  = 0555
owner = root
group = bin
flags = 
exportPath = ./bin/gfix

FILE:ibase:SHARED:./bin/gds_drop:
mode  = 6555
owner = root
group = bin
flags = 
exportPath = ./bin/gds_drop

FILE:ibase:SHARED:./bin/gds_lock_mgr:
mode  = 6555
owner = root
group = bin
flags = 
exportPath = ./bin/gds_lock_mgr

FILE:ibase:SHARED:./bin/gbak:
mode  = 0555
owner = root
group = bin
flags = 
exportPath = ./bin/gbak

FILE:ibase:SHARED:./bin/gdef:
mode  = 0555
owner = root
group = bin
flags = 
exportPath = ./bin/gdef

FILE:ibase:SHARED:./bin/iblicense:
mode  = 0555
owner = root
group = bin
flags = 
exportPath = ./bin/iblicense

FILE:ibase:SHARED:./bin/isql:
mode  = 0555
owner = root
group = bin
flags = 
exportPath = ./bin/isql

FILE:ibase:SHARED:./bin/gds_lock_print:
mode  = 0555
owner = root
group = bin
flags = 
exportPath = ./bin/gds_lock_print

FILE:ibase:SHARED:./bin/qli:
mode  = 0555
owner = root
group = bin
flags = 
exportPath = ./bin/qli

FILE:ibase:SHARED:./bin/gds_inet_server:
mode  = 6555
owner = root
group = bin
flags = 
exportPath = ./bin/gds_inet_server

FILE:ibase:SHARED:./bin/gsplit:
mode  = 0555
owner = root
group = bin
flags = 
exportPath = ./bin/gsplit

FILE:ibase:SHARED:./bin/gstat:
mode  = 0555
owner = root
group = bin
flags = 
exportPath = ./bin/gstat

FILE:ibase:SHARED:./bin/gds_relay:
mode  = 6555
owner = root
group = bin
flags = 
exportPath = ./bin/gds_relay

FILE:ibase:SHARED:./bin/gsec:
mode  = 0555
owner = root
group = bin
flags = 
exportPath = ./bin/gsec

FILE:ibase:SHARED:./bin/isc4.gbak:
mode  = 0555
owner = root
group = bin
flags = 
exportPath = ./bin/isc4.gbak

DIR:ibase:SHARED:./examples:
mode  = 0755
owner = root
group = bin
flags = 

FILE:ibase:SHARED:./examples/README:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/README

FILE:ibase:SHARED:./examples/align.h:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/align.h

FILE:ibase:SHARED:./examples/api1.c:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/api1.c

FILE:ibase:SHARED:./examples/api10.c:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/api10.c

FILE:ibase:SHARED:./examples/api11.c:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/api11.c

FILE:ibase:SHARED:./examples/api12.c:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/api12.c

FILE:ibase:SHARED:./examples/api13.c:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/api13.c

FILE:ibase:SHARED:./examples/api14.e:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/api14.e

FILE:ibase:SHARED:./examples/api15.c:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/api15.c

FILE:ibase:SHARED:./examples/api16.c:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/api16.c

FILE:ibase:SHARED:./examples/api16t.c:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/api16t.c

FILE:ibase:SHARED:./examples/api2.c:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/api2.c

FILE:ibase:SHARED:./examples/api3.c:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/api3.c

FILE:ibase:SHARED:./examples/api4.c:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/api4.c

FILE:ibase:SHARED:./examples/api5.c:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/api5.c

FILE:ibase:SHARED:./examples/api6.c:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/api6.c

FILE:ibase:SHARED:./examples/api7.c:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/api7.c

FILE:ibase:SHARED:./examples/api8.c:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/api8.c

FILE:ibase:SHARED:./examples/api9.c:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/api9.c

FILE:ibase:SHARED:./examples/api9f.c:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/api9f.c

FILE:ibase:SHARED:./examples/api9f.sql:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/api9f.sql

FILE:ibase:SHARED:./examples/apifull.c:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/apifull.c

FILE:ibase:SHARED:./examples/dyn1.e:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/dyn1.e

FILE:ibase:SHARED:./examples/dyn2.e:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/dyn2.e

FILE:ibase:SHARED:./examples/dyn3.e:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/dyn3.e

FILE:ibase:SHARED:./examples/dyn4.e:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/dyn4.e

FILE:ibase:SHARED:./examples/dyn5.e:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/dyn5.e

FILE:ibase:SHARED:./examples/dynfull.e:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/dynfull.e

FILE:ibase:SHARED:./examples/employe2.sql:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/employe2.sql

FILE:ibase:SHARED:./examples/employee.gbk:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/employee.gbk

FILE:ibase:CLIENT:./examples/employee.gdb:
mode  = 0666
owner = root
group = bin
flags = 
exportPath = ./examples/employee.gdb

FILE:ibase:SHARED:./examples/example.h:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/example.h

FILE:ibase:SHARED:./examples/ib_udf.sql:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/ib_udf.sql

FILE:ibase:SHARED:./examples/intlemp.gbk:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/intlemp.gbk

FILE:ibase:CLIENT:./examples/intlemp.gdb:
mode  = 0666
owner = root
group = bin
flags = 
exportPath = ./examples/intlemp.gdb

FILE:ibase:SHARED:./examples/makefile:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/makefile

FILE:ibase:SHARED:./examples/stat1.e:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/stat1.e

FILE:ibase:SHARED:./examples/stat10.e:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/stat10.e

FILE:ibase:SHARED:./examples/stat11.e:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/stat11.e

FILE:ibase:SHARED:./examples/stat12.e:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/stat12.e

FILE:ibase:SHARED:./examples/stat12t.e:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/stat12t.e

FILE:ibase:SHARED:./examples/stat2.e:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/stat2.e

FILE:ibase:SHARED:./examples/stat3.e:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/stat3.e

FILE:ibase:SHARED:./examples/stat4.e:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/stat4.e

FILE:ibase:SHARED:./examples/stat5.e:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/stat5.e

FILE:ibase:SHARED:./examples/stat6.e:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/stat6.e

FILE:ibase:SHARED:./examples/stat7.e:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/stat7.e

FILE:ibase:SHARED:./examples/stat8.e:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/stat8.e

FILE:ibase:SHARED:./examples/stat9.e:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/stat9.e

FILE:ibase:SHARED:./examples/udf.sql:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/udf.sql

FILE:ibase:SHARED:./examples/udflib.c:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./examples/udflib.c

DIR:ibase:SHARED:./help:
mode  = 0755
owner = root
group = bin
flags = 

FILE:ibase:SHARED:./help/help.gbak:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./help/help.gbak

FILE:ibase:CLIENT:./help/help.gdb:
mode  = 0666
owner = root
group = bin
flags = 
exportPath = ./help/help.gdb

DIR:ibase:SHARED:./include:
mode  = 0755
owner = root
group = bin
flags = 

FILE:ibase:SHARED:./include/gds.h:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./include/gds.h

FILE:ibase:SHARED:./include/perf.h:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./include/perf.h

FILE:ibase:SHARED:./include/ibase.h:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./include/ibase.h

FILE:ibase:SHARED:./include/iberror.h:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./include/iberror.h

FILE:ibase:SHARED:./include/ib_util.h:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./include/ib_util.h

FILE:ibase:SHARED:./include/interbase.ada:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./include/interbase.ada

FILE:ibase:SHARED:./inetd.conf.isc:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./inetd.conf.isc

FILE:ibase:SHARED:./install:
mode  = 0555
owner = root
group = bin
flags = 
exportPath = ./install

FILE:ibase:CLIENT:./interbase.log:
mode  = 0666
owner = root
group = sys
flags = 
exportPath = ./interbase.log

FILE:ibase:SHARED:./interbase.msg:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./interbase.msg

FILE:ibase:SHARED:./ReleaseNotes.txt:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./ReleaseNotes.txt

FILE:ibase:SHARED:./ReleaseNotes.html:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./ReleaseNotes.html

DIR:ibase:SHARED:./intl:
mode  = 0755
owner = root
group = bin
flags = 
exportPath = ./intl

FILE:ibase:CLIENT:./isc4.gdb:
mode  = 0666
owner = root
group = bin
flags = 
exportPath = ./isc4.gdb

FILE:ibase:CLIENT:./isc_config:
mode  = 0664
owner = root
group = bin
flags = 
exportPath = ./isc_config

FILE:ibase:SHARED:./isc_ins_hlp.dat:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./isc_ins_hlp.dat

DIR:ibase:SHARED:./lib:
mode  = 0755
owner = root
group = bin
flags = 

FILE:ibase:SHARED:./lib/gdsintl:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./lib/gdsintl

FILE:ibase:SHARED:./lib/gds_pyxis.a:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./lib/gds_pyxis.a

FILE:ibase:SHARED:./lib/gds.so.0:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./lib/gds.so.0

FILE:ibase:SHARED:./lib/gds.a:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./lib/gds.a

FILE:ibase:SHARED:./lib/ib_udf:
mode  = 0555
owner = root
group = bin
flags = 
exportPath = ./lib/ib_udf

FILE:ibase:SHARED:./lib/ib_util.so:
mode  = 0555
owner = root
group = bin
flags = 
exportPath = ./lib/ib_util.so

FILE:ibase:SHARED:./services.isc:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./services.isc

FILE:ibase:SHARED:./license.txt:
mode  = 0444
owner = root
group = bin
flags = 
exportPath = ./license.txt
