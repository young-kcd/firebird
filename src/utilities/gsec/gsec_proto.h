#ifndef GSEC_PROTO_H
#define GSEC_PROTO_H

#ifdef SUPERSERVER
extern int GSEC_main(SVC service);
#endif

/* Output reporting utilities */
extern void GSEC_print_status(ISC_STATUS *);
extern void GSEC_error_redirect(ISC_STATUS *, USHORT, TEXT *, TEXT *);
extern void GSEC_error(USHORT, TEXT *, TEXT *, TEXT *, TEXT *, TEXT *);
extern void GSEC_print(USHORT, TEXT *, TEXT *, TEXT *, TEXT *, TEXT *);
extern void GSEC_print_partial(USHORT, TEXT *, TEXT *, TEXT *, TEXT *, TEXT *);

#endif // GSEC_PROTO_H
