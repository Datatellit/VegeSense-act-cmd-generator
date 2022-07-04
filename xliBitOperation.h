//------------------------------------------------------------------
// DTIT Common Functions: Bitmap Operation
//------------------------------------------------------------------
#ifndef __XLI_BIT_OPERATION_H__
#define __XLI_BIT_OPERATION_H__

#ifndef BIT
#define BIT(n)                    ( 1<<(n) )
#endif

#define BITTEST(var,pos)          (((var)>>(pos)) & 0x0001)
#define BITMASK(pos)              (0x0001 << (pos))
#define BITSET(var,pos)           ((var) | BITMASK(pos))
#define BITUNSET(var,pos)         ((var) & (~BITMASK(pos)))
#define _BV(x)                    (1<<(x))

// Create a bitmask of length len.
#define BIT_MASK(len)             ( BIT(len)-1 )
// Create a bitfield mask of length starting at bit 'start'.
#define BF_MASK(start, len)       ( BIT_MASK(len)<<(start) )

// Prepare a bitmask for insertion or combining.
#define BF_PREP(x, start, len)    ( ((x)&BIT_MASK(len)) << (start) )
// Extract a bitfield of length len starting at bit 'start' from y.
#define BF_GET(y, start, len)     ( ((y)>>(start)) & BIT_MASK(len) )
// Insert a new bitfield value x into y.
#define BF_SET(y, x, start, len)  ( y= ((y) &~ BF_MASK(start, len)) | BF_PREP(x, start, len) )

#endif /* __XLI_BIT_OPERATION_H__ */
