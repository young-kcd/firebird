#ifndef GSEC_PROTO_H
#define GSEC_PROTO_H

#ifdef SUPERSERVER
int		GSEC_main(Jrd::Service* service);
#endif

// Output reporting utilities
void	GSEC_print_status(const ISC_STATUS*);
void	GSEC_error_redirect(const ISC_STATUS*, USHORT, const TEXT*, const TEXT*);
void	GSEC_error(USHORT, const TEXT*, const TEXT*, const TEXT*, const TEXT*,
	const TEXT*);
void	GSEC_print(USHORT, const TEXT*, const TEXT*, const TEXT*, const TEXT*,
	const TEXT*);
void	GSEC_print_partial(USHORT, const TEXT*, const TEXT*, const TEXT*,
	const TEXT*, const TEXT*);

#endif // GSEC_PROTO_H

