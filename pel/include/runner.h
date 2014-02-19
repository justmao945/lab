/*
 * File: runner.h
 * Provides C style interface to manage PEL.
 * All functions are start with 'pel_', and 'EL' for types.
 */

#ifndef PEL_RUNNER_H
#define PEL_RUNNER_H

#ifdef __cplusplus
extern "C" {
#endif


/*
 * PEL context type
 */
#define EL_CTX_MEM     0
#define EL_CTX_SQLITE  1
#define EL_CTX_MYSQL   2


/* 
 * PEL context.
 */
struct ELCtx;


/* 
 * Prepared PEL statement 
 */
struct ELStmt;


/* 
 * Create PEL context.
 * @type Type of underlying storage.
 * @return New PEL context, and must be destroyed after using.
 */
struct ELCtx* pel_CreateCtx( int type );

/* 
 * Destroy context.
 */
void pel_DestroyCtx( struct ELCtx* ctx );

/* 
 * Prepare string format PEL statement.
 * @ctx
 * @stmt
 * @return
 */
struct ELStmt* pel_Prepare( struct ELCtx* ctx, const char* stmt );

/*
 * Prepare PEL statements from a file.
 * @ctx
 * @filename
 * @return
 */
struct ELStmt* pel_PrepareFile( struct ELCtx* ctx, const char* filename );

/* 
 * Destroy prepared PEL statement.
 */
void pel_Finalize( struct ELStmt* stmt );

/* 
 * Evaluate a statement created by pel_Prepare().
 */
int pel_Step( struct ELStmt* stmt );


/* access object */


#ifdef __cplusplus
} /* ~extern */
#endif

#endif
