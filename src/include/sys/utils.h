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
#else
#define _EXTERN_C_DECL_BEGIN
#define _EXTERN_C_DECL_END
#endif

#endif /* UTILS_H */
