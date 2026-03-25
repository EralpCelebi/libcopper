/**
 * @file slibc.c
 * @author Eralp Çelebi <eralp.celebi.personal@gmail.com>
 * @brief Base implementation for the C standard library.
 * @date 07-03-2026
 * @copyright Copyright (c) 2026 Eralp Çelebi
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 3 of the License, or (at your option) any later
 * version. This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details. You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 * 
 *
 * Please note that the functions defined here are stupid-unsafe. Make sure you
 * do not let them act like they want to!
 */

#include <copper.h>

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Returns the size of a wide-string.
 *
 * @param String The wide-string to be used.
 * @return `size_t` The number of `wchar_t`'s in the string.
 */
size_t wcstrlen(const wchar_t* restrict Source) {
    size_t Index = 0;

    for (; Source[Index] != L'\0'; Index++) {}
    return Index;
}

/**
 * @brief Returns the size of a string.
 *
 * @param [in] String The input string.
 * @return `size_t` The number of characters in the string.
 */
size_t strlen(const char* restrict Source) {
    size_t Index = 0;

    for (; Source[Index] != '\0'; Index++) {}
    return Index;
}

/**
 * @brief Copies a chunk of memory from one buffer to the other.
 *
 * @param [in] Destination The target memory address.
 * @param [in] Source The source memory address.
 * @param [in] Size Amount of data to be copied over in bytes.
 */
void memcpy(void* Destination, const void* restrict Source, size_t Size) {
    for (size_t Index = 0; Index < Size; Index++) {
        ((uint8_t*)Destination)[Index] = ((uint8_t*)Source)[Index];
    }
}

/**
 * @brief Sets the value of a memory chunk to the specified value.
 *
 * @param [out] Destination The target memory address.
 * @param [in]  Size Amount of data to be mutated in bytes.
 * @param [in]  X Desired value to be mutated to.
 */
void memset(void* Destination, uint8_t Source, size_t Size) {
    for (size_t Index = 0; Index < Size; Index++) {
        ((uint8_t*)Destination)[Index] = Source;
    }
}

/**
 * @brief Compares memory contents of two regions.
 *
 * @param First First memory region.
 * @param Second Second memory region.
 * @param Size The size of the region in bytes.
 * @return Returns the sum of the differences of the compared regions.
 */
int memcmp(const char* restrict First, const char* restrict Second, size_t Size) {
    int Compare = 0;

    if (strlen(First) != strlen(Second)) return -1;

    for (size_t Index = 0; First[Index] != '\0' && Second[Index] != '\0' && Index < Size; Index++) {
        Compare += *First - *Second;
    }

    return Compare;
}


/**
 * @brief Converts numeric values into alpha-numeric characters.
 *
 * @param [in] Value The numeric value to be converted.
 * @param [out] Buffer The string buffer to be used as output.
 * @param [in] Base The base of the number.
 */
void itoa(size_t Value, char* Buffer, uint8_t Base) {
    size_t Index       = 0;
    size_t ReturnIndex = 0;
    char   Destination[0x200];    /// TODO: Memory allocation mock is required here.

    memset(&Destination, 0, 0x200);

    if (Value == 0) {
        Buffer[0] = '0';
        Buffer[1] = '\0';
        return;
    }

    while (Value > 0) {
        size_t Digit         = Value % Base;
        Destination[Index++] = (Digit < 10) ? ('0' + Digit) : ('A' + (Digit - 10));
        Value /= Base;
    }

    while (Index > 0) {
        Buffer[ReturnIndex++] = Destination[--Index];
    }

    Buffer[ReturnIndex] = '\0';
}

/**
 * @brief Convers a C-string to a wide-string.
 *
 * @param Destination Destination `wchar_t*` buffer.
 * @param Source Source `char*` buffer.
 * @return `size_t` The number of characters converted.
 */
size_t mbstowcs(wchar_t* Destination, const char* restrict Source) {
    size_t Index = 0;

    for (; Source[Index] != '\0'; Index++) {
        Destination[Index] = (wchar_t)Source[Index];
    }

    Destination[Index] = '\0';

    return Index;
}

/**
 * @brief Compares two `wchar_t*` strings against each other.
 *
 * @param [in] First The first string to compare.
 * @param [in] Second The second string to compare.
 * @return `int` Numeric difference of the two strings.
 */
int wcstrcmp(const wchar_t* restrict First, const wchar_t* restrict Second) {
    int Compare = 0;

    if (wcstrlen(First) != wcstrlen(Second)) return -1;

    for (size_t Index = 0; First[Index] != L'\0' && Second[Index] != L'0'; Index++) {
        Compare += First - Second;
    }

    return Compare;
}

/**
 * @brief Compares two `char*` strings against each other.
 *
 * @param [in] First The first string to compare.
 * @param [in] Second The second string to compare.
 * @return `int` Numeric difference of the two strings.
 */
int strcmp(const char* restrict First, const char* restrict Second) {
    int Compare = 0;

    if (strlen(First) != strlen(Second)) return -1;

    for (size_t Index = 0; First[Index] != '\0' && Second[Index] != '\0'; Index++) {
        Compare += *First - *Second;
    }

    return Compare;
}

/**
 * @brief Formats given arguments into a destination buffer. Accepts a `va_list`
 * instead of variadic arguments.
 *
 * @param [out] Destination A `char*` that points to a user allocated buffer to
 * hold the resulting string.
 * @param [in] Format A `char*` that points to the format string.
 * @param [in] Args A `va_list` Contains the pre-initialized variadic arguments.
 */
void vsprintf(char* Destination, const char* restrict Format, va_list Args) {
    char NumericBlock[0x200];    /// TODO: Memory allocation mock is required here.

    memset(&NumericBlock, 0, 0x200);

    size_t Pen     = 0;
    size_t Index   = 0;
    size_t Numeric = 0;
    size_t Length  = 0;

    char*    Message     = NULL;
    wchar_t* WideMessage = NULL;

    uint16_t Space     = 0;
    bool     Direction = true;

    for (; Format[Index] != '\0'; Index++) {
        Space     = 0;
        Direction = true;
        if (Format[Index] != '%') {
            Destination[Pen++] = Format[Index];
            continue;
        }

        if (Format[Index] == '%' && Format[Index + 1] == '\0') {
            Destination[Pen++] = '%';
            break;
        }

    _Replace:
        Index++;
        switch (Format[Index]) {
            case '%': Destination[Pen++] = '%'; break;
            case '-': Direction = !Direction; goto _Replace;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                Space *= 10;
                Space += Format[Index] - '0';
                goto _Replace;
            case 's':
                Message = va_arg(Args, char*);
                Length  = strlen(Message);

                if (Direction) {
                    for (; Space > Length; Space--) {
                        Destination[Pen++] = ' ';
                    }
                }

                for (size_t MessageIndex = 0; Message[MessageIndex] != '\0'; MessageIndex++) {
                    Destination[Pen++] = (char)Message[MessageIndex];
                }

                if (!Direction) {
                    for (; Space > Length; Space--) {
                        Destination[Pen++] = ' ';
                    }
                }

                break;

            case 'w':
                WideMessage = va_arg(Args, wchar_t*);
                Length      = wcstrlen(WideMessage);

                if (Direction) {
                    for (; Space > Length; Space--) {
                        Destination[Pen++] = ' ';
                    }
                }

                for (size_t MessageIndex = 0; WideMessage[MessageIndex] != '\0'; MessageIndex++) {
                    Destination[Pen++] = (char)WideMessage[MessageIndex];
                }

                if (!Direction) {
                    for (; Space > Length; Space--) {
                        Destination[Pen++] = ' ';
                    }
                }

                break;

            case 'd':
                Numeric = va_arg(Args, size_t);
                itoa(Numeric, (char*)&NumericBlock, 10);

                Length = strlen((char*)&NumericBlock);

                if (Direction) {
                    for (; Space > Length; Space--) {
                        Destination[Pen++] = '0';
                    }
                }

                for (size_t NumericIndex = 0; NumericBlock[NumericIndex] != '\0'; NumericIndex++) {
                    Destination[Pen++] = NumericBlock[NumericIndex];
                }

                if (!Direction) {
                    for (; Space > Length; Space--) {
                        Destination[Pen++] = ' ';
                    }
                }
                break;

            case 'b':
                Numeric = va_arg(Args, size_t);
                itoa(Numeric, (char*)&NumericBlock, 2);

                Length = strlen((char*)&NumericBlock);

                if (Direction) {
                    for (; Space > Length; Space--) {
                        Destination[Pen++] = '0';
                    }
                }

                for (size_t NumericIndex = 0; NumericBlock[NumericIndex] != '\0'; NumericIndex++) {
                    Destination[Pen++] = NumericBlock[NumericIndex];
                }

                if (!Direction) {
                    for (; Space > Length; Space--) {
                        Destination[Pen++] = ' ';
                    }
                }

                break;

            case 'x':
                Numeric = va_arg(Args, size_t);
                itoa(Numeric, (char*)&NumericBlock, 16);

                Length = strlen((char*)&NumericBlock);

                if (Direction) {
                    for (; Space > Length; Space--) {
                        Destination[Pen++] = '0';
                    }
                }
                for (size_t NumericIndex = 0; NumericBlock[NumericIndex] != '\0'; NumericIndex++) {
                    Destination[Pen++] = NumericBlock[NumericIndex];
                }

                if (!Direction) {
                    for (; Space > Length; Space--) {
                        Destination[Pen++] = ' ';
                    }
                }

                break;
            default: break;
        }
    }

    Destination[Pen] = '\0';
}

/**
 * @brief Formats given arguments into a destination buffer.
 *
 * @param [out] Destination A `char*` that points to a user allocated buffer to
 * hold the resulting string.
 * @param [in] Format A `char*` that points to the format string.
 * @param [in] ... Variadic arguments used for formatting.
 */
void sprintf(char* Destination, const char* restrict Format, ...) {
    va_list Args;
    va_start(Args, Format);

    vsprintf(Destination, Format, Args);

    va_end(Args);
}

#if defined (CONFIG_PROVIDE_JOURNAL)

/**
 * @brief Prints the provided string using the journaling dispatch callbacks.
 *
 * @param Source A reference to the string.
 */
void puts(const char* restrict Source) {
        REQUIRE(Source != NULL, EINVAL);
        Journal_Dispatch(Source);
}

/**
 * @brief Prints a formatted string using the journaling dispatch callbacks.
 *
 * @param Format Formatting string to be used as a template.
 * @param Arguments Variadic arguments list provided for the formatting.
 */
void vprintf(const char *restrict Format, va_list Arguments) {
        REQUIRE(Format != NULL, EINVAL);

        // TODO: Replace this with the global allocator.
        char Destination[0x200] = { 0 };
        
        vsprintf(Destination, Format, Arguments);
        puts(Destination);
}

/**
 * @brief Prints a formatted string using the journaling dispatch callbacks and
 * variadic arguments.
 *
 * @param Format Formatting string to be used as a template.
 */
void printf(const char* restrict Format, ...) {
        va_list Arguments;
        va_start(Arguments, Format);

        vprintf(Format, Arguments); 

        va_end(Arguments);
}

#endif
