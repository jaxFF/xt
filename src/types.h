#ifndef TYPES_H
#define TYPES_H

#include <direct.h>
#include <float.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define global static

typedef unsigned int uint;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef int32 bool32;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef float real32;
typedef double real64;

typedef int8 s8;
typedef int16 s16;
typedef int32 s32;
typedef int64 s64;
typedef bool32 b32;

typedef uint8 u8;
typedef uint16 u16;
typedef uint32 u32;
typedef uint64 u64;

typedef real32 f32;
typedef real64 f64;

typedef uintptr_t umm;
typedef intptr_t smm;

#define S32Min ((s32)0x80000000)
#define S32Max ((s32)0x7fffffff)
#define U16Max 65535
#define U32Max ((u32)-1)
#define U64Max ((u64)-1)
#define F32Max FLT_MAX
#define F32Min -FLT_MAX

#define Minimum(A, B) ((A < B) ? (A) : (B))
#define Maximum(A, B) ((A > B) ? (A) : (B))

#define For(Value) For_e((Value), ArrayCount(Value))
#define For_e(Value, End) For_se((Value), 0, (End))
#define For_se(Value, Start, End) for (int (Value) = (Start); (Value) < (End); ++(Value))

// todo(jax): Should these always be 64-bit?
#define Kilobytes(Value) (((uint64)Value) * 1024LL)
#define Megabytes(Value) (Kilobytes((uint64)Value) * 1024LL)
#define Gigabytes(Value) (Megabytes((uint64)Value) * 1024LL)
#define Terabytes(Value) (Gigabytes((uint64)Value) * 1024LL)

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))

#define AlignPow2(Value, Alignment) ((Value + ((Alignment) - 1)) & ~((Alignment) - 1))
#define Align4(Value) ((Value + 3) & ~3)
#define Align8(Value) ((Value + 7) & ~7)
#define Align16(Value) ((Value + 15) & ~15)

#define Stringize(x) PrimitiveStringize(x)
#define PrimitiveStringize(x) #x

inline b32 IsPow2(u32 Value) {
    return ((Value & ~(Value - 1)) == Value);
}

#define MemoryCopy(Dest, Src, Size) memmove((Dest), (Src), (Size))

// note(jax): Platform-independent way to perform an assertion.
// Flat out writes to zero memory to crash the program.
// todo(jax): Create some sort of assert function that creates a message box 
// like in previous engines I've worked on!
#if ENGINE_SLOW
#define Assert(Expression) if (!(Expression)) { *(int*)0=0; }
#else
#define Assert(Expression)
#endif

#define InvalidCodePath Assert(!"InvalidCodePath")

// A structure that encapsulates a non-terminated buffer
struct string {
    u8* Data;
    umm Count;
};

inline u32 SafeTruncateU32(u64 Value) {
	// todo(jax): Defines for min/max values 
	Assert(Value <= 0xFFFFFFFF);
	u32 Result = (u32)Value;
	return Result;
}

inline u16 SafeTruncateU16(u32 Value) {
	// todo(jax): Defines for min/max values 
    Assert(Value <= 0xFFFF);
    u16 Result = (u16)Value;
    return Result;
}

inline u8 SafeTruncateU8(u64 Value) {
    Assert(Value <= 0xFF);
    u8 Result = (u8)Value;
    return Result;
}

#endif
