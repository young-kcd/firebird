#ifndef GSEC_PROTO_H
#define GSEC_PROTO_H

#ifdef SUPERSERVER
int		GSEC_main(SVC service);
#endif

// Output reporting utilities
void	GSEC_print_status(const ISC_STATUS*);
void	GSEC_error_redirect(const ISC_STATUS*, USHORT, TEXT *, TEXT *);
void	GSEC_error(USHORT, TEXT *, TEXT *, TEXT *, TEXT *, TEXT *);
void	GSEC_print(USHORT, TEXT *, TEXT *, TEXT *, TEXT *, TEXT *);
void	GSEC_print_partial(USHORT, TEXT *, TEXT *, TEXT *, TEXT *, TEXT *);

#endif // GSEC_PROTO_H

