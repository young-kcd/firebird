#ifndef _PYXIS_SAVE_PROTO_H
#define _PYXIS_SAVE_PROTO_H

#ifdef __cplusplus
extern "C" {
#endif

OBJ PYXIS_relation_fields(SLONG **, SLONG **, TEXT *);
OBJ PYXIS_relation_form(WIN , OBJ );
int PYXIS_store_form(SLONG **, SLONG **, TEXT *, OBJ );
int PYXIS_define_forms_relation(SLONG **);

#ifdef __cplusplus
};
#endif


#endif
