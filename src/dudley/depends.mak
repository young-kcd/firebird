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
# depends.mak - dudley
# Created by 'make depends.mak'
# Created on 1998-11-17
ddl.o: ddl.c
ddl.o: ddl.h
ddl.o: ddl_proto.h
ddl.o: exe_proto.h
ddl.o: expan_proto.h
ddl.o: extra_proto.h
ddl.o: hsh_proto.h
ddl.o: lex_proto.h
ddl.o: parse.h
ddl.o: parse_proto.h
ddl.o: source/interbase/include/iberror.h
ddl.o: source/jrd/build_no.h
ddl.o: source/jrd/common.h
ddl.o: source/jrd/dsc.h
ddl.o: source/jrd/fil.h
ddl.o: source/jrd/gds.h
ddl.o: source/jrd/gds_proto.h
ddl.o: source/jrd/license.h
ddl.o: source/jrd/thd.h
ddl.o: source/jrd/why_proto.h
ddl.o: trn_proto.h
exe.o: ddl.h
exe.o: ddl_proto.h
exe.o: exe.c
exe.o: exe_proto.h
exe.o: gener_proto.h
exe.o: hsh_proto.h
exe.o: lex_proto.h
exe.o: source/interbase/include/iberror.h
exe.o: source/intl/charsets.h
exe.o: source/jrd/acl.h
exe.o: source/jrd/build_no.h
exe.o: source/jrd/common.h
exe.o: source/jrd/dsc.h
exe.o: source/jrd/fil.h
exe.o: source/jrd/flags.h
exe.o: source/jrd/gds.h
exe.o: source/jrd/gds_proto.h
exe.o: source/jrd/intl.h
exe.o: source/jrd/isc_f_proto.h
exe.o: source/jrd/license.h
exe.o: source/jrd/obj.h
exe.o: source/jrd/thd.h
exe.o: source/wal/walf_proto.h
expand.o: ddl.h
expand.o: ddl_proto.h
expand.o: expan_proto.h
expand.o: expand.c
expand.o: hsh_proto.h
expand.o: parse.h
expand.o: parse_proto.h
expand.o: source/interbase/include/iberror.h
expand.o: source/jrd/common.h
expand.o: source/jrd/dsc.h
expand.o: source/jrd/fil.h
expand.o: source/jrd/gds.h
expand.o: source/jrd/gds_proto.h
expand.o: source/jrd/thd.h
expr.o: ddl.h
expr.o: ddl_proto.h
expr.o: expr.c
expr.o: expr_proto.h
expr.o: lex_proto.h
expr.o: parse.h
expr.o: parse_proto.h
expr.o: source/interbase/include/iberror.h
expr.o: source/intl/charsets.h
expr.o: source/jrd/acl.h
expr.o: source/jrd/common.h
expr.o: source/jrd/dsc.h
expr.o: source/jrd/gds.h
expr.o: source/jrd/intl.h
extract.o: ddl.h
extract.o: ddl_proto.h
extract.o: extra_proto.h
extract.o: extract.c
extract.o: source/interbase/include/iberror.h
extract.o: source/jrd/acl.h
extract.o: source/jrd/common.h
extract.o: source/jrd/dsc.h
extract.o: source/jrd/fil.h
extract.o: source/jrd/flags.h
extract.o: source/jrd/gds.h
extract.o: source/jrd/gds_proto.h
extract.o: source/jrd/isc.h
extract.o: source/jrd/misc.h
extract.o: source/jrd/obj.h
extract.o: source/jrd/thd.h
extract.o: source/wal/wal.h
generate.o: ddl.h
generate.o: ddl_proto.h
generate.o: gener_proto.h
generate.o: generate.c
generate.o: source/interbase/include/iberror.h
generate.o: source/jrd/acl.h
generate.o: source/jrd/common.h
generate.o: source/jrd/dsc.h
generate.o: source/jrd/fil.h
generate.o: source/jrd/gds.h
generate.o: source/jrd/gds_proto.h
generate.o: source/jrd/thd.h
generate.o: trn_proto.h
hsh.o: ddl.h
hsh.o: ddl_proto.h
hsh.o: hsh.c
hsh.o: hsh_proto.h
hsh.o: parse.h
hsh.o: source/jrd/common.h
hsh.o: source/jrd/dsc.h
lex.o: ddl.h
lex.o: ddl_proto.h
lex.o: hsh_proto.h
lex.o: lex.c
lex.o: lex_proto.h
lex.o: parse.h
lex.o: source/interbase/include/iberror.h
lex.o: source/jrd/common.h
lex.o: source/jrd/dsc.h
lex.o: source/jrd/fil.h
lex.o: source/jrd/gds.h
lex.o: source/jrd/gds_proto.h
lex.o: source/jrd/kanji.h
lex.o: source/jrd/thd.h
parse.o: ddl.h
parse.o: ddl_proto.h
parse.o: exe_proto.h
parse.o: expr_proto.h
parse.o: hsh_proto.h
parse.o: lex_proto.h
parse.o: parse.c
parse.o: parse.h
parse.o: parse_proto.h
parse.o: source/interbase/include/iberror.h
parse.o: source/jrd/acl.h
parse.o: source/jrd/common.h
parse.o: source/jrd/dsc.h
parse.o: source/jrd/fil.h
parse.o: source/jrd/flags.h
parse.o: source/jrd/gds.h
parse.o: source/jrd/gds_proto.h
parse.o: source/jrd/isc.h
parse.o: source/jrd/isc_f_proto.h
parse.o: source/jrd/kanji.h
parse.o: source/jrd/misc.h
parse.o: source/jrd/thd.h
parse.o: source/wal/wal.h
trn.o: ddl.h
trn.o: ddl_proto.h
trn.o: gener_proto.h
trn.o: lex_proto.h
trn.o: source/gpre/prett_proto.h
trn.o: source/interbase/include/iberror.h
trn.o: source/jrd/build_no.h
trn.o: source/jrd/common.h
trn.o: source/jrd/dsc.h
trn.o: source/jrd/fil.h
trn.o: source/jrd/gds.h
trn.o: source/jrd/gds_proto.h
trn.o: source/jrd/gdsassert.h
trn.o: source/jrd/license.h
trn.o: source/jrd/thd.h
trn.o: trn.c
trn.o: trn_proto.h
