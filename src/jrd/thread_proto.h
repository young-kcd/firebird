
#ifndef JRD_THREAD_PROTO_H
#define JRD_THREAD_PROTO_H
//
//
#include "../jrd/thd.h"
#include "../jrd/sch_proto.h"
#include "../jrd/thd_proto.h"

#ifdef MULTI_THREAD
#ifdef SUPERSERVER
inline void THREAD_ENTER() {
	SCH_enter();
}
inline void THREAD_EXIT() {
	SCH_exit();
}
//inline bool THREAD_VALIDATE() {
//	return SCH_validate();
//}
#define SWEEP_THREAD
//#define GARBAGE_THREAD
#else // SUPERSERVER
inline void THREAD_ENTER() {
	gds__thread_enter();
}
inline void THREAD_EXIT(){
	gds__thread_exit();
}
//inline bool THREAD_VALIDATE() {
//	return true;
//}
#define AST_THREAD
#endif // SUPERSERVER
inline void THREAD_SLEEP(ULONG msecs) {
	THD_sleep(msecs);
}
inline void THREAD_YIELD() {
	THD_yield();
}
#else // MULTI_THREAD
inline void THREAD_ENTER() {
}
inline void THREAD_EXIT() {
}
//inline bool THREAD_VALIDATE() {
//	return true;
//}
inline void THREAD_SLEEP(ULONG msecs) {
	THD_sleep(msecs);
}
inline void THREAD_YIELD() {
	THD_yield();
}
#endif // MULTI_THREAD

#endif // JRD_THREAD_PROTO_H
