#ifndef _PYXIS_SAVE_H_
#define _PYXIS_SAVE_H_

#ifdef __cplusplus
extern "C" {
#endif

OBJ PYXIS_load_form(int *, SLONG **, SLONG **, OBJ *, USHORT * , TEXT *);
OBJ PYXIS_relation_fields(SLONG **, SLONG **, TEXT *);
int PYXIS_store_form(SLONG **, SLONG **, TEXT *, OBJ);

#ifdef __cplusplus
};
#endif


#endif
