#ifndef _PYXIS_PHASE3_H_
#define _PYXIS_PHASE3_H_

extern int pyxis__compile_map(STATUS *, OBJ *, MAP *, USHORT *, SCHAR *);
extern int pyxis__compile_menu(WIN *, MENU *, USHORT *, TEXT *);
extern int pyxis__compile_sub_map(STATUS *, MAP *, MAP *, USHORT *, SCHAR *);
extern int pyxis__create_window(WIN *, USHORT *, TEXT *, USHORT *, USHORT *);
extern int pyxis__delete(OBJ *);
extern int pyxis__delete_window(SLONG *);
extern int pyxis__drive_form(STATUS *, SLONG **, SLONG *, WIN *, MAP *,
                      UCHAR *, UCHAR *);
extern int pyxis__drive_menu(WIN *, MENU *, USHORT *, SCHAR *, USHORT *,
              TEXT *, USHORT *, USHORT *, TEXT *, SLONG *);
extern int pyxis__fetch(STATUS *, SLONG **, SLONG *, MAP *, UCHAR *);
extern int pyxis__get_entree(MENU *, USHORT *, TEXT *, SLONG *, USHORT *);
extern int pyxis__initialize_menu(MENU *);
extern int pyxis__insert(STATUS *, SLONG **, SLONG *, MAP *, UCHAR *);
extern int pyxis__load_form(STATUS *, SLONG *, SLONG *, SLONG *, SSHORT *,
                                   SCHAR *);
extern OBJ pyxis__menu(WIN *, MENU *, USHORT *, TEXT *);
extern int pyxis__menu_d(WIN *, MENU *, USHORT *, TEXT *, OBJ *);
#ifdef __cplusplus
extern "C" int pyxis__pop_window(WIN *);
#else
extern int pyxis__pop_window(WIN *);
#endif
extern int pyxis__put_entree(MENU *, USHORT *, TEXT *, SLONG *);
extern int pyxis__reset_form(STATUS *, MAP *);
extern int pyxis__suspend_window(SLONG *);


#endif
