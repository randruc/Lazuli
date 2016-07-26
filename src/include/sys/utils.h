/**
 * @file utils.h
 *
 * This file contains some useful macros.
 * It can be included independently, without dependency conflicts.
 *
 * The content of this file is not related to the current project. 
 */


#ifndef UTILS_H
#define UTILS_H

#ifdef __cplusplus
/** 
 * Open C++ header file declarations
 */    
#define _EXTERN_C_DECL_BEGIN  extern "C" {

/** 
 * Close C++ header file declarations
 */    
#define _EXTERN_C_DECL_END    }
#else /* __cplusplus */
#define _EXTERN_C_DECL_BEGIN
#define _EXTERN_C_DECL_END
#endif /* __cplusplus */

/**
 * Macro used to allow arithmetic on void pointers.
 * Arithmetic on void pointers isn't allowed in standard C.
 *
 * @param X The void pointer on which to do arithmetic.
 */
#define ALLOW_ARITHM(X) ((char *)X)

#endif /* UTILS_H */
