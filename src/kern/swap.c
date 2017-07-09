/**
 * @file swpa.c
 *
 * This file describes byte swap routines.
 */

#include <Lazuli/common.h>

#include <Lazuli/sys/swap.h>

/**
 * Union to help swapping operations.
 */
typedef union {
  u16 u16value;  /** < Holds an u16 value                     */
  u8  bytes[2];  /** < Holds an u16 value as an array of u8   */
}Swap;


u16
Swap16(u16 u)
{
  Swap s;
  s.u16value = u;
  return (s.bytes[0] << 8 | s.bytes[1]);
}
