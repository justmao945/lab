
#ifndef STACK_TRACE_H
#define STACK_TRACE_H

#ifdef __cplusplus
namespace stdx {
extern "C" {
#endif /* end of __cplusplus */

/* 
 * Call to print stack trace
 */
void pstack();

/*
 * Call to print stack trace and raise @signo
 */
void pstack1(int signo);

/* 
 * Register printStackTrace to fatal error signals(only contains standard signals).
 * This function should be called before any functions, generally following the main().
 */
void rstack();

/*
 * Restore previous signal handlers.
 */
void urstack();

#ifdef __cplusplus
} /* end of extern "C" */
} /* end of stdx */
#endif

#endif /* end of STACK_TRACE_H */
