struct format_dest {
    umm Size;
    char* At;
};

void OutChar(format_dest* Dest, char Value) {
    if (Dest->Size) {
        --Dest->Size;
        *Dest->At++ = Value;
    }
}

void OutChars(format_dest* Dest, char* Value) {
    // note(jax): Not particularily speedy@
    while (*Value) {
        OutChar(Dest, *Value++);
    }
}

#define ReadVarArgUnsignedInteger(Length, ArgList) ((Length) == 8) ? _crt_va_arg(*ArgList, u64) : (u64)_crt_va_arg(*ArgList, u32)
#define ReadVarArgSignedInteger(Length, ArgList) ((Length) == 8) ? _crt_va_arg(*ArgList, s64) : (s64)_crt_va_arg(*ArgList, s32)
#define ReadVarArgFloat(Length, ArgList) _crt_va_arg(*ArgList, f64)

static char DecChars[] = "0123456789";
static char LowerHexChars[] = "0123456789abcdef";
static char UpperHexChars[] = "0123456789ABCDEF";

void U64ToASCII(format_dest* Dest, u64 Value, u32 Base, char* Digits) {
    Assert(Base != 0);

    char* Start = Dest->At;
    do {
        u64 DigitIndex = (Value % Base);
        char Digit = Digits[DigitIndex];
        OutChar(Dest, Digit);

        Value /= Base;
    } while (Value != 0);
    char* End = Dest->At;

    while (Start < End) {
        --End;
        char Temp = *End;
        *End = *Start;
        *Start = Temp;
        ++Start;
    }
}

void F64ToASCII(format_dest* Dest, f64 Value, u32 Precision) {
    if (Value < 0) {
        OutChar(Dest, '-');
        Value = -Value;
    }

    u64 IntegerPart = (u64)Value;
    Value -= (f64)IntegerPart;
    U64ToASCII(Dest, IntegerPart, 10, DecChars);

    OutChar(Dest, '.');

    // tood(jax): Note that this is NOT an accurate way to do this!
    for (u32 PrecisionIndex = 0; PrecisionIndex < Precision; ++PrecisionIndex) {
        Value *= 10.0f;
        u32 Integer = (u32)Value;
        Value -= (f32)Integer;
        OutChar(Dest, DecChars[Integer]);
    }
}

// note(jax): This function serves as a replacement to `stdio.h` sprintf()
umm FormatArgList(umm DestSize, char* DestInit, char* Format, va_list ArgList) {
    format_dest Dest = { DestSize, DestInit };
    if (Dest.Size) {
        char* At = Format;
        while (At[0]) {
            if (*At == '%') {
                ++At;

                b32 ForceSign = false;
                b32 PadWithZeros = false;
                b32 LeftJustify = false;
                b32 PositiveSignIsBlank = false;
                b32 AnnotateIfNotZero = false;

                // note(jax): Handle the flags
                b32 Parsing = true;

                Parsing = true;
                while (Parsing) {
                    switch (*At) {
                    case '+': { ForceSign = true; } break;
                    case '0': { PadWithZeros = true; } break;
                    case '-': { LeftJustify = true; } break;
                    case ' ': { PositiveSignIsBlank = true; } break;
                    case '#': { AnnotateIfNotZero = true; } break;
                    default: { Parsing = false; } break;
                    }

                    if (Parsing) {
                        ++At;
                    }
                }

                // note(jax): Handle the width
                b32 WidthSpecified = false;
                int32 Width = 0;
                if (*At == '*') {
                    Width = _crt_va_arg(ArgList, int);
                    WidthSpecified = true;
                    ++At;
                }
                else if ((*At >= '0') && (*At <= '9')) {
                    Width = StringToI32(At);
                    WidthSpecified = true;
                    //? ++At;
                }

                // note(jax): Handle the precision
                b32 PrecisionSpecified = false;
                int32 Precision = 0;
                if (*At == '.') {
                    ++At;

                    if (*At == '*') {
                        Precision = _crt_va_arg(ArgList, int);
                        PrecisionSpecified = true;
                        ++At;
                    }
                    else if ((*At >= '0') && (*At <= '9')) {
                        Precision = StringToI32(At);
                        PrecisionSpecified = true;
                        //++At;
                    }
                    else {
                        Assert(!"Malformed precision specifier!");
                    }
                }

                // todo(jax): Right now our routine doesn't allow non-specified 
                // precisions, so we just set non-specified precisions to a specific value.
                if (!PrecisionSpecified) {
                    Precision = 6;
                }

                // note(jax): Handle the length
                u32 IntegerLength = 4;
                u32 FloatLength = 8;
                // todo(jax): Actually set different values here.
                if ((At[0] == 'h') && (At[1] == 'h')) {
                    At += 2;
                }
                else if ((At[0] == 'l') && (At[1] == 'l')) {
                    At += 2;
                }
                else if (*At == 'h') {
                    ++At;
                }
                else if (*At == 'l') {
                    IntegerLength = 8;
                    ++At;
                }
                else if (*At == 'j') {
                    ++At;
                }
                else if (*At == 'z') {
                    ++At;
                }
                else if (*At == 't') {
                    ++At;
                }
                else if (*At == 'L') {
                    ++At;
                }

                char TempBuffer[64];
                char* Temp = TempBuffer;
                format_dest TempDest = { ArrayCount(TempBuffer), Temp };
                char* Prefix = "";
                b32 IsFloat = false;

                switch (*At) {
                case 'd':
                case 'i': {
                    s64 Value = ReadVarArgSignedInteger(IntegerLength, &ArgList);
                    b32 WasNegative = (Value < 0);
                    if (WasNegative) {
                        Value = -Value;
                    }

                    U64ToASCII(&TempDest, (u64)Value, 10, DecChars);
                    // todo(jax): Make this a common routine once floating poiunt is available.
                    if (WasNegative) {
                        Prefix = "-";
                    }
                    else if (ForceSign) {
                        Assert(!PositiveSignIsBlank); // note(jax): Not a problem, but probably shouldn't be specified.
                        Prefix = "+";
                    }
                    else if (PositiveSignIsBlank) {
                        Prefix = " ";
                    }
                } break;

                case 'u': {
                    u64 Value = ReadVarArgUnsignedInteger(IntegerLength, &ArgList);
                    U64ToASCII(&TempDest, Value, 10, DecChars);
                } break;

                case 'o': {
                    u64 Value = ReadVarArgUnsignedInteger(IntegerLength, &ArgList);
                    U64ToASCII(&TempDest, Value, 8, DecChars);
                    if (AnnotateIfNotZero && (Value != 0)) {
                        Prefix = "0";
                    }
                } break;

                case 'x': {
                    u64 Value = ReadVarArgUnsignedInteger(IntegerLength, &ArgList);
                    U64ToASCII(&TempDest, Value, 16, LowerHexChars);
                    if (AnnotateIfNotZero && (Value != 0)) {
                        Prefix = "0x";
                    }
                } break;

                case 'X': {
                    u64 Value = ReadVarArgUnsignedInteger(IntegerLength, &ArgList);
                    U64ToASCII(&TempDest, Value, 16, UpperHexChars);
                    if (AnnotateIfNotZero && (Value != 0)) {
                        Prefix = "0X";
                    }
                } break;

                    // todo(jax): Support other kinds of floating point prints
                    // Currently we only do basic decimal output.
                case 'f':
                case 'F':
                case 'e':
                case 'E':
                case 'g':
                case 'G':
                case 'a':
                case 'A': {
                    f64 Value = ReadVarArgFloat(FloatLength, &ArgList);
                    F64ToASCII(&TempDest, Value, Precision);
                    IsFloat = true;

                    // @Speed @Cleanup todo(jax): If we still have more floats in temp, increase 
                    // the size again. This is a very, very bad hack and is NOT shippable! (shame!)
                    if (Temp) {
                        Dest.Size += FloatLength;
                    }
                } break;

                case 'c': {
                    // todo(jax): How much are we supposed to read here?
                    int Value = _crt_va_arg(ArgList, int);
                    OutChar(&TempDest, (char)Value);
                } break;

                case 's': {
                    char* String = _crt_va_arg(ArgList, char*);

                    // todo(jax): Obey precision, width, etc.

                    Temp = String;
                    if (PrecisionSpecified) {
                        TempDest.Size = 0;
                        for (char* Scan = String; *Scan && (TempDest.Size < Precision); ++Scan) {
                            ++TempDest.Size;
                        }
                    }
                    else {
                        TempDest.Size = StringLength(String);
                    }

                    TempDest.At = String + TempDest.Size;
                } break;

                case 'p': {
                    void* Value = _crt_va_arg(ArgList, void*);
                    U64ToASCII(&TempDest, *(umm*)&Value, 16, LowerHexChars);
                } break;

                case 'n': {
                    int* TabDest = _crt_va_arg(ArgList, int*);
                    *TabDest = (int)(Dest.At - DestInit);
                } break;

                case '%': {
                    OutChar(&Dest, '%');
                } break;

                default: {
                    Assert(!"Unrecognized format specifier!");
                } break;
                }

                if (TempDest.At - Temp) {
                    smm UsePrecision = Precision;
                    if (IsFloat || !PrecisionSpecified) {
                        UsePrecision = (TempDest.At - Temp);
                    }

                    smm PrefixLength = StringLength(Prefix);
                    smm UseWidth = Width;
                    smm ComputedWidth = UsePrecision + PrefixLength;
                    if (UseWidth < ComputedWidth) {
                        UseWidth = ComputedWidth;
                    }

                    if (PadWithZeros) {
                        Assert(!LeftJustify); // note(jax): Not a problem, but no way to do it?
                        LeftJustify = false;
                    }

                    if (!LeftJustify) {
                        while (UseWidth > (UsePrecision + PrefixLength)) {
                            OutChar(&Dest, PadWithZeros ? '0' : ' ');
                            --UseWidth;
                        }
                    }

                    for (char* Pre = Prefix; *Pre && UseWidth; ++Pre) {
                        OutChar(&Dest, *Pre);
                        --UseWidth;
                    }

                    if (UsePrecision > UseWidth) {
                        UsePrecision = UseWidth;
                    }

                    while (UsePrecision > (TempDest.At - Temp)) {
                        OutChar(&Dest, '0');
                        --UsePrecision;
                        --UseWidth;
                    }

                    while (UsePrecision && (TempDest.At != Temp)) {
                        OutChar(&Dest, *Temp++);
                        --UsePrecision;
                        --UseWidth;
                    }

                    if (LeftJustify) {
                        while (UseWidth) {
                            OutChar(&Dest, ' ');
                            --UseWidth;
                        }
                    }
                }

                if (*At) {
                    ++At;
                }
            }
            else {
                OutChar(&Dest, *At++);
            }
        }

        if (Dest.Size) {
            Dest.At[0] = 0;
        }
        else {
            Dest.At[-1] = 0;
        }
    }

    umm Result = Dest.At - DestInit;
    return Result;
}