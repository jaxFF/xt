#ifndef SHARED_H
#define SHARED_H

#define ZeroStruct(Struct) ZeroSize(&Struct, sizeof(Struct))
inline void ZeroSize(void* Ptr, size_t Size) {
    u8* Byte = (u8*)Ptr;
    // todo(jax): @Speed: Check performance of the two code blocks
#if 0
    for (size_t i = 0; i < Size; ++i) {
        *Byte++ = 0;
    }
#else
    while (Size--) {
        *Byte++ = 0;
    }
#endif
}

#define CopyArray(Dest, Source, Count) MemoryCopy((Dest), (Source), (Count)*sizeof(*(Source)))
inline void* MemoryCopy(void* _Dest, void* _Source, size_t Size) {
    if (_Source == 0) {
        ZeroSize(_Dest, Size);
        return _Dest;
    }

	u8* Source = (u8*)_Source;
	u8* Dest = (u8*)_Dest;

	while (Size--) {
		*Dest++ = *Source++;
	}

	return _Dest;
}

inline u8* Advance(string* String, umm Count) {
    u8 *Result = 0;
    
    if(String->Count >= Count) {
        Result = String->Data;
        String->Data += Count;
        String->Count -= Count;
    } else {
        String->Data += String->Count;
        String->Count = 0;
    }
    
    return(Result);    
}

inline b32 IsSpacing(char C) {
    b32 Result = ((C == ' ') || (C == '\t') || (C == '\v') ||(C == '\f'));
    return(Result);
}

inline b32 IsEndOfLine(char Value) {
    b32 Result = ((Value == '\n') || (Value == '\r'));
    return Result;
}


inline b32 IsWhitespace(char Value, bool IncludeEOL = false) {
    b32 Result = ((Value == ' ') || (Value == '\t') || (Value == '\v') || (Value == '\f') || (IncludeEOL ? IsEndOfLine(Value) : 1));
    return Result;
}

inline b32 IsAlphabetical(char Value) {
    b32 Result = (((Value >= 'a') && (Value <= 'z')) || ((Value >= 'A') && (Value <= 'Z')));
    return Result;
}

inline b32 IsNumeric(char Value) {
    b32 Result = ((Value >= '0') && (Value <= '9'));
    return Result;
}

inline char* GetNextLine(char** Contents) {
    char* Text = *Contents;
    if (!*Text) {
        return nullptr;
    }

    char* Line = Text;
    while (*Text && (*Text != '\n') && (*Text != '\r')) {
        ++Text;
    }

    char* End = Text;
    ++End;
    if (*Text == '\r') {
        if (*End == '\n') {
            ++End;
        }

        *Text = '\0';
    }

    *Contents = End;

    return Line;
}

inline int StringLength(char* String) {
    int Count = 0;
    while (*String++) {
        ++Count;
    }
    return Count;
}

inline char* Substring(char* Source, char* String)  {
	while (*Source) {
        char *Begin = Source;
        char *Pattern = String;
        
        // If first character of sub string match, check for whole string
        while (*Source && *Pattern && *Source == *Pattern) {
            Source++;
            Pattern++;
        }

        // If complete sub string match, return starting address 
        if (!*Pattern)
            return Begin;
                
        Source = Begin + 1;
	}
	return NULL;
}

// note: _Char is expected to be an ASCII character
inline char* FindFirstChar(char* String, int _Char) {
    char Char = (char)_Char;
    while (*String != Char) {
        if (!*String++) {
            return NULL;
        }
    }
    return (char*)String;
}

inline char* StringCopy(char* String) {
    char* Result = (char*)malloc(sizeof(char) * (StringLength(String) + 1));
    MemoryCopy(Result, String, sizeof(char) * (StringLength(String) + 1));

    return Result;
}

// todo(jax): Consider using while loops for Append and StringCopy?? Query performance benefit.

inline char* Append(char* Dest, size_t DestSize, char* Source) {
    // todo(jax): Do we need to terminate annd or allocate needed memory?
    char* Result = (char*)malloc(DestSize + (1 + StringLength(Source) * sizeof(char)));
    MemoryCopy(Result, Dest, DestSize);
    MemoryCopy(Dest + DestSize, Source, StringLength(Source));

    umm Size = DestSize + StringLength(Source);
    Dest[Size] = 0;

    return Dest;
}

inline bool StringsMatch(char* A, char* B) {
    while (*A && *B) {
        if (*A != *B){
            return false;
        }

        ++A;
        ++B;
    }

    if (*A != *B){
        return false;
    } else {
        return true;
    }
}

inline void CatStrings(size_t SourceACount, char* SourceA, size_t SourceBCount, char* SourceB, size_t DestCount, char* Dest) {
	// todo(jax): Dest bounds checking!
	for (int Index = 0; Index < SourceACount; ++Index) {
		*Dest++ = *SourceA++;
	}

	for (int Index = 0; Index < SourceBCount; ++Index) {
		*Dest++ = *SourceB++;
	}

	*Dest++ = 0;
}

inline string make_string(char* String) {
    string Result = {};
    Result.Data = (u8*)String;
    Result.Count = StringLength(String);

    return Result;
}


#if 1
void __cdecl __va_start(va_list*, ...);
#define _crt_va_start(ap, x) ((void)(__va_start(&ap, x)))
#define _crt_va_arg(ap, t)                                               \
        ((sizeof(t) > sizeof(__int64) || (sizeof(t) & (sizeof(t) - 1)) != 0) \
            ? **(t**)((ap += sizeof(__int64)) - sizeof(__int64))             \
            :  *(t* )((ap += sizeof(__int64)) - sizeof(__int64)))
#define _crt_va_end(ap)        ((void)(ap = (va_list)0))
#else
#define _crt_va_start(ap, v)    va_start(ap, v)
#define _crt_va_arg(ap, t)      va_arg(ap, t)
#define _crt_va_end(ap)         va_end(ap)
#endif

inline int32 StringToI32_(char** AtInit) {
	int32 Result = {};

	char* At = *AtInit;
	while ((*At >= '0') && (*At <= '9')) {
		Result *= 10;
		Result += (*At - '0');
		++At;
	}

	*AtInit = At;

    return Result;
}

inline int32 StringToI32(char* At) {
	char* Ignored = At;
	int32 Result = StringToI32_(&Ignored);
	return Result;
}

struct format_dest;

void OutChar(format_dest* Dest, char Value);
void OutChars(format_dest* Dest, char* Value);

void U64ToASCII(format_dest* Dest, u64 Value, u32 Base, char* Digits);
void F64ToASCII(format_dest* Dest, f64 Value, u32 Precision);

// note(jax): This function serves as a replacement to `stdio.h` sprintf()
umm FormatArgList(umm DestSize, char* DestInit, char* Format, va_list ArgList);
inline umm Format(umm DestSize, char* Dest, char* Format, ...) {
    va_list ArgList;
    _crt_va_start(ArgList, Format);
    umm Result = FormatArgList(DestSize, Dest, Format, ArgList);
    _crt_va_end(ArgList);
    
    return Result;
}

inline int _crt_sprintf(char* Buffer, char* Format, ...) {
	int Result;

	va_list ArgList;
	__crt_va_start(ArgList, Format);
	Result = _vsprintf_s_l(Buffer, sizeof(Buffer), Format, NULL, ArgList);
	__crt_va_end(ArgList);

	return Result;
}

inline int _crt_printf(char* Format, ...) {
    char Buffer[1024];
	int Result;

	va_list ArgList;
	__crt_va_start(ArgList, Format);
	Result = _vsprintf_s_l(Buffer, 1024, Format, NULL, ArgList);
	__crt_va_end(ArgList);

    fwrite(Buffer, sizeof(char), StringLength(Buffer), stdout);
    fflush(stdout);
	return Result;
}

inline umm ae_sprintf(char* Dest, char* Format, ...) {
    va_list ArgList;
    _crt_va_start(ArgList, Format);
    umm Result = FormatArgList(sizeof(Dest), Dest, Format, ArgList);
    _crt_va_end(ArgList);

    return Result;
}

inline umm ae_sprintf(char* Dest, umm Size, char* Format, ...) {
    va_list ArgList;
    _crt_va_start(ArgList, Format);
    umm Result = FormatArgList(Size, Dest, Format, ArgList);
    _crt_va_end(ArgList);

    return Result;
}


inline umm ae_sprintf(char* Dest, char* Format, va_list ArgList) {
    umm Result = FormatArgList(sizeof(Dest), Dest, Format, ArgList);

    return Result;
}

inline umm ae_sprintf(char* Dest, umm Size, char* Format, va_list ArgList) {
    umm Result = FormatArgList(Size, Dest, Format, ArgList);

    return Result;
}

inline umm ae_printf(char* Format, ...) {
    char Buffer[1024];

    va_list ArgList;
    _crt_va_start(ArgList, Format);
    umm Result = FormatArgList(1024, Buffer, Format, ArgList);
    _crt_va_end(ArgList);

    fwrite(Buffer, sizeof(char), StringLength(Buffer), stdout);
    fflush(stdout);
    return Result;
}

inline umm ae_printf(char* Format, va_list ArgList) {
    char Buffer[1024];
    umm Result = FormatArgList(1024, Buffer, Format, ArgList);

    fwrite(Buffer, sizeof(char), StringLength(Buffer), stdout);
    fflush(stdout);
    return Result;
}

#endif