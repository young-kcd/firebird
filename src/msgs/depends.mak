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
# depends.mak - msgs
# Created by 'make depends.mak'
# Created on 1998-11-17
build_file.o: build_file.c
build_file.o: source/interbase/include/iberror.h
build_file.o: source/jrd/common.h
build_file.o: source/jrd/fil.h
build_file.o: source/jrd/gds.h
build_file.o: source/jrd/gds_proto.h
build_file.o: source/jrd/msg.h
build_file.o: source/jrd/thd.h
change_msgs.o: change_msgs.c
change_msgs.o: source/interbase/include/iberror.h
change_msgs.o: source/jrd/common.h
change_msgs.o: source/jrd/gds.h
check_msgs.o: check_msgs.c
check_msgs.o: source/interbase/include/iberror.h
check_msgs.o: source/jrd/common.h
check_msgs.o: source/jrd/fil.h
check_msgs.o: source/jrd/gds.h
check_msgs.o: source/jrd/gds_proto.h
check_msgs.o: source/jrd/thd.h
enter_msgs.o: enter_msgs.c
enter_msgs.o: source/interbase/include/iberror.h
enter_msgs.o: source/jrd/common.h
enter_msgs.o: source/jrd/gds.h
modify_msgs.o: modify_msgs.c
modify_msgs.o: source/interbase/include/iberror.h
modify_msgs.o: source/jrd/common.h
modify_msgs.o: source/jrd/gds.h
