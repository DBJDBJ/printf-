#pragma once
// license and colophon are at eof

#include <cwctype>
#include <cstdint> 
#include <cstdarg> 
#include <string>

#ifndef _HAS_CXX17 
#	error __FILE__ " -- can participate in MSVC C++17 builds only"
#endif

#ifndef _UNICODE
#	error __FILE__ " -- can participate in UNICODE builds only"
#endif

// I personlay think 'extern "C"' here can do no harm
// it just preserves the names.
// That is CL does not mangle them 
#define PRINTF_EXTERN_C  extern "C"

/// <summary>
/// we use namespace mpaland_dbjdbj::wchar
/// to distiguish from mpaland_dbjdbj 
/// which is a narrow (char) version of the same
/// </summary>
namespace mpaland_dbjdbj::wchar {

	/**
	 * Output a character to a custom device like UART, used by the printf() function
	 * This function is declared here only. You have to write your custom implementation somewhere
	 * \param character Character to output
	 */
#ifdef PRINTF_USER_DEFINED_PUTCHAR
	PRINTF_EXTERN_C extern void _putchar(wchar_t character);
#else
	PRINTF_EXTERN_C inline void _putchar(wchar_t character) {
		std::putwchar(character);
	}
#endif

///////////////////////////////////////////////////////////////////////////////
namespace inner {
///////////////////////////////////////////////////////////////////////////////


	// ntoa conversion buffer size, this must be big enough to hold
	// one converted numeric number including padded zeros (dynamically created on stack)
	// 32 byte is a good default
	constexpr inline const auto PRINTF_NTOA_BUFFER_SIZE = 32U;

	// ftoa conversion buffer size, this must be big enough to hold
	// one converted float number including padded zeros (dynamically created on stack)
	// 32 byte is a good default
	constexpr inline const auto PRINTF_FTOA_BUFFER_SIZE = 32U;

	// define this to support floating point (%f)
#define PRINTF_SUPPORT_FLOAT

// define this to support long long types (%llu or %p)
#define PRINTF_SUPPORT_LONG_LONG

// define this to support the ptrdiff_t type (%t)
// ptrdiff_t is normally defined in <stddef.h> as long or long long type
#define  PRINTF_SUPPORT_PTRDIFF_T


///////////////////////////////////////////////////////////////////////////////

// internal flag definitions
	constexpr inline const auto FLAGS_ZEROPAD = (1U << 0U);
	constexpr inline const auto FLAGS_LEFT = (1U << 1U);
	constexpr inline const auto FLAGS_PLUS = (1U << 2U);
	constexpr inline const auto FLAGS_SPACE = (1U << 3U);
	constexpr inline const auto FLAGS_HASH = (1U << 4U);
	constexpr inline const auto FLAGS_UPPERCASE = (1U << 5U);
	constexpr inline const auto FLAGS_CHAR = (1U << 6U);
	constexpr inline const auto FLAGS_SHORT = (1U << 7U);
	constexpr inline const auto FLAGS_LONG = (1U << 8U);
	constexpr inline const auto FLAGS_LONG_LONG = (1U << 9U);
	constexpr inline const auto FLAGS_PRECISION = (1U << 10U);
	constexpr inline const auto FLAGS_WIDTH = (1U << 11U);


	// output function type
	typedef void(*out_fct_type)(wchar_t character, void* buffer, size_t idx, size_t maxlen);


	// wrapper (used as buffer) for output function type
	typedef struct {
		void(*fct)(wchar_t character, void* arg);
		void* arg;
	} out_fct_wrap_type;


	// internal buffer output
	PRINTF_EXTERN_C inline void _out_buffer(wchar_t character, void* buffer, size_t idx, size_t maxlen)
	{
		if (idx < maxlen) {
			((wchar_t*)buffer)[idx] = character;
		}
	}


	// internal null output
	PRINTF_EXTERN_C inline void _out_null(wchar_t character, void* buffer, size_t idx, size_t maxlen)
	{
		(void)character; (void)buffer; (void)idx; (void)maxlen;
	}


	// internal _putchar wrapper
	PRINTF_EXTERN_C inline void _out_char(wchar_t character, void* buffer, size_t idx, size_t maxlen)
	{
		(void)buffer; (void)idx; (void)maxlen;
		// DBJ: the if() bellow effectively stops writing wchar_t(0)?
		// if (character) {
		_putchar(character);
		// }
	}


	// internal output function wrapper
	PRINTF_EXTERN_C inline void _out_fct(wchar_t character, void* buffer, size_t idx, size_t maxlen)
	{
		(void)idx; (void)maxlen;
		// buffer is the output fct pointer
		((out_fct_wrap_type*)buffer)->fct(character, ((out_fct_wrap_type*)buffer)->arg);
	}


	// internal strlen
	// \return The length of the string (excluding the terminating 0)
	PRINTF_EXTERN_C inline unsigned int _strlen(const wchar_t* str)
	{
		const wchar_t* s;
		for (s = str; *s; ++s);
		return (unsigned int)(s - str);
	}

#include <cwctype>
	// internal test if wchar_t is a digit (0-9)
	//  it might be reasonable to restrict your code to just the Roman digits.
	// \return true if wchar_t is a digit
	PRINTF_EXTERN_C inline bool _is_digit(wchar_t ch)
	{
		return (ch >= L'0') && (ch <= L'9');
	}


	// internal ASCII string to unsigned int conversion
	PRINTF_EXTERN_C inline unsigned int _atoi(const wchar_t** str)
	{
		unsigned int i = 0U;
		while (_is_digit(**str)) {
			i = i * 10U + (unsigned int)(*((*str)++) - L'0');
		}
		return i;
	}


	// internal itoa format
	PRINTF_EXTERN_C  inline size_t _ntoa_format(out_fct_type out, wchar_t* buffer, size_t idx, size_t maxlen, wchar_t* buf, size_t len, bool negative, unsigned int base, unsigned int prec, unsigned int width, unsigned int flags)
	{
		const size_t start_idx = idx;

		// pad leading zeros
		while (!(flags & FLAGS_LEFT) && (len < prec) && (len < PRINTF_NTOA_BUFFER_SIZE)) {
			buf[len++] = L'0';
		}
		while (!(flags & FLAGS_LEFT) && (flags & FLAGS_ZEROPAD) && (len < width) && (len < PRINTF_NTOA_BUFFER_SIZE)) {
			buf[len++] = L'0';
		}

		// handle hash
		if (flags & FLAGS_HASH) {
			if (((len == prec) || (len == width)) && (len > 0U)) {
				len--;
				if ((base == 16U) && (len > 0U)) {
					len--;
				}
			}
			if ((base == 16U) && !(flags & FLAGS_UPPERCASE) && (len < PRINTF_NTOA_BUFFER_SIZE)) {
				buf[len++] = L'x';
			}
			if ((base == 16U) && (flags & FLAGS_UPPERCASE) && (len < PRINTF_NTOA_BUFFER_SIZE)) {
				buf[len++] = L'X';
			}
			if (len < PRINTF_NTOA_BUFFER_SIZE) {
				buf[len++] = L'0';
			}
		}

		// handle sign
		if ((len == width) && (negative || (flags & FLAGS_PLUS) || (flags & FLAGS_SPACE))) {
			len--;
		}
		if (len < PRINTF_NTOA_BUFFER_SIZE) {
			if (negative) {
				buf[len++] = L'-';
			}
			else if (flags & FLAGS_PLUS) {
				buf[len++] = L'+';  // ignore the space if the '+' exists
			}
			else if (flags & FLAGS_SPACE) {
				buf[len++] = L' ';
			}
		}

		// pad spaces up to given width
		if (!(flags & FLAGS_LEFT) && !(flags & FLAGS_ZEROPAD)) {
			for (size_t i = len; i < width; i++) {
				out(L' ', buffer, idx++, maxlen);
			}
		}

		// reverse string
		for (size_t i = 0U; i < len; i++) {
			out(buf[len - i - 1U], buffer, idx++, maxlen);
		}

		// append pad spaces up to given width
		if (flags & FLAGS_LEFT) {
			while (idx - start_idx < width) {
				out(L' ', buffer, idx++, maxlen);
			}
		}

		return idx;
	}


	// internal itoa for 'long' type
	PRINTF_EXTERN_C inline size_t _ntoa_long(out_fct_type out, wchar_t* buffer, size_t idx, size_t maxlen, unsigned long value, bool negative, unsigned long base, unsigned int prec, unsigned int width, unsigned int flags)
	{
		wchar_t buf[PRINTF_NTOA_BUFFER_SIZE];
		size_t len = 0U;

		// write if precision != 0 and value is != 0
		if (!(flags & FLAGS_PRECISION) || value) {
			do {
				const wchar_t digit = (wchar_t)(value % base);
				buf[len++] = digit < 10 ? L'0' + digit : (flags & FLAGS_UPPERCASE ? L'A' : L'a') + digit - 10;
				value /= base;
			} while (value && (len < PRINTF_NTOA_BUFFER_SIZE));
		}

		return _ntoa_format(out, buffer, idx, maxlen, buf, len, negative, (unsigned int)base, prec, width, flags);
	}


	// internal itoa for 'long long' type
#if defined(PRINTF_SUPPORT_LONG_LONG)
	PRINTF_EXTERN_C  inline size_t _ntoa_long_long(out_fct_type out, wchar_t* buffer, size_t idx, size_t maxlen, unsigned long long value, bool negative, unsigned long long base, unsigned int prec, unsigned int width, unsigned int flags)
	{
		wchar_t buf[PRINTF_NTOA_BUFFER_SIZE];
		size_t len = 0U;

		// write if precision != 0 and value is != 0
		if (!(flags & FLAGS_PRECISION) || value) {
			do {
				const wchar_t digit = (wchar_t)(value % base);
				buf[len++] = digit < 10 ? L'0' + digit : (flags & FLAGS_UPPERCASE ? L'A' : L'a') + digit - 10;
				value /= base;
			} while (value && (len < PRINTF_NTOA_BUFFER_SIZE));
		}

		return _ntoa_format(out, buffer, idx, maxlen, buf, len, negative, (unsigned int)base, prec, width, flags);
	}
#endif  // PRINTF_SUPPORT_LONG_LONG


#if defined(PRINTF_SUPPORT_FLOAT)
	PRINTF_EXTERN_C  inline size_t _ftoa(out_fct_type out, wchar_t* buffer, size_t idx, size_t maxlen, double value, unsigned int prec, unsigned int width, unsigned int flags)
	{
		wchar_t buf[PRINTF_FTOA_BUFFER_SIZE];
		size_t len = 0U;
		double diff = 0.0;

		// if input is larger than thres_max, revert to exponential
		const double thres_max = (double)0x7FFFFFFF;

		// powers of 10
		static const double pow10[] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000 };

		// test for negative
		bool negative = false;
		if (value < 0) {
			negative = true;
			value = 0 - value;
		}

		// set default precision to 6, if not set explicitly
		if (!(flags & FLAGS_PRECISION)) {
			prec = 6U;
		}
		// limit precision to 9, cause a prec >= 10 can lead to overflow errors
		while ((len < PRINTF_FTOA_BUFFER_SIZE) && (prec > 9U)) {
			buf[len++] = L'0';
			prec--;
		}

		int whole = (int)value;
		double tmp = (value - whole) * pow10[prec];
		unsigned long frac = (unsigned long)tmp;
		diff = tmp - frac;

		if (diff > 0.5) {
			++frac;
			// handle rollover, e.g. case 0.99 with prec 1 is 1.0
			if (frac >= pow10[prec]) {
				frac = 0;
				++whole;
			}
		}
		else if ((diff == 0.5) && ((frac == 0U) || (frac & 1U))) {
			// if halfway, round up if odd, OR if last digit is 0
			++frac;
		}

		// TBD: for very large numbers switch back to native sprintf for exponentials. Anyone want to write code to replace this?
		// Normal printf behavior is to print EVERY whole number digit which can be 100s of characters overflowing your buffers == bad
		if (value > thres_max) {
			return 0U;
		}

		if (prec == 0U) {
			diff = value - (double)whole;
			if (diff > 0.5) {
				// greater than 0.5, round up, e.g. 1.6 -> 2
				++whole;
			}
			else if ((diff == 0.5) && (whole & 1)) {
				// exactly 0.5 and ODD, then round up
				// 1.5 -> 2, but 2.5 -> 2
				++whole;
			}
		}
		else {
			unsigned int count = prec;
			// now do fractional part, as an unsigned number
			while (len < PRINTF_FTOA_BUFFER_SIZE) {
				--count;
				buf[len++] = (wchar_t)(48U + (frac % 10U));
				if (!(frac /= 10U)) {
					break;
				}
			}
			// add extra 0s
			while ((len < PRINTF_FTOA_BUFFER_SIZE) && (count-- > 0U)) {
				buf[len++] = L'0';
			}
			if (len < PRINTF_FTOA_BUFFER_SIZE) {
				// add decimal
				buf[len++] = L'.';
			}
		}

		// do whole part, number is reversed
		while (len < PRINTF_FTOA_BUFFER_SIZE) {
			buf[len++] = (wchar_t)(48 + (whole % 10));
			if (!(whole /= 10)) {
				break;
			}
		}

		// pad leading zeros
		while (!(flags & FLAGS_LEFT) && (flags & FLAGS_ZEROPAD) && (len < width) && (len < PRINTF_FTOA_BUFFER_SIZE)) {
			buf[len++] = L'0';
		}

		// handle sign
		if ((len == width) && (negative || (flags & FLAGS_PLUS) || (flags & FLAGS_SPACE))) {
			len--;
		}
		if (len < PRINTF_FTOA_BUFFER_SIZE) {
			if (negative) {
				buf[len++] = L'-';
			}
			else if (flags & FLAGS_PLUS) {
				buf[len++] = L'+';  // ignore the space if the '+' exists
			}
			else if (flags & FLAGS_SPACE) {
				buf[len++] = ' ';
			}
		}

		// pad spaces up to given width
		if (!(flags & FLAGS_LEFT) && !(flags & FLAGS_ZEROPAD)) {
			for (size_t i = len; i < width; i++) {
				out(' ', buffer, idx++, maxlen);
			}
		}

		// reverse string
		for (size_t i = 0U; i < len; i++) {
			out(buf[len - i - 1U], buffer, idx++, maxlen);
		}

		// append pad spaces up to given width
		if (flags & FLAGS_LEFT) {
			while (idx < width) {
				out(' ', buffer, idx++, maxlen);
			}
		}

		return idx;
	}
#endif  // PRINTF_SUPPORT_FLOAT


	// internal vsnprintf
	PRINTF_EXTERN_C  inline int _vsnprintf(out_fct_type out, wchar_t* buffer, const size_t maxlen, const wchar_t* format, va_list va)
	{
		unsigned int flags, width, precision, n;
		size_t idx = 0U;

		if (!buffer) {
			// use null output function
			out = _out_null;
		}

		while (*format)
		{
			// format specifier?  %[flags][width][.precision][length]
			if (*format != L'%') {
				// no
				out(*format, buffer, idx++, maxlen);
				format++;
				continue;
			}
			else {
				// yes, evaluate it
				format++;
			}

			// evaluate flags
			flags = 0U;
			do {
				switch (*format) {
				case L'0': flags |= FLAGS_ZEROPAD; format++; n = 1U; break;
				case L'-': flags |= FLAGS_LEFT;    format++; n = 1U; break;
				case L'+': flags |= FLAGS_PLUS;    format++; n = 1U; break;
				case L' ': flags |= FLAGS_SPACE;   format++; n = 1U; break;
				case L'#': flags |= FLAGS_HASH;    format++; n = 1U; break;
				default:                                   n = 0U; break;
				}
			} while (n);

			// evaluate width field
			width = 0U;
			if (_is_digit(*format)) {
				width = _atoi(&format);
			}
			else if (*format == L'*') {
				const int w = va_arg(va, int);
				if (w < 0) {
					flags |= FLAGS_LEFT;    // reverse padding
					width = (unsigned int)-w;
				}
				else {
					width = (unsigned int)w;
				}
				format++;
			}

			// evaluate precision field
			precision = 0U;
			if (*format == '.') {
				flags |= FLAGS_PRECISION;
				format++;
				if (_is_digit(*format)) {
					precision = _atoi(&format);
				}
				else if (*format == L'*') {
					precision = (unsigned int)va_arg(va, int);
					format++;
				}
			}

			// evaluate length field
			switch (*format) {
			case L'l':
				flags |= FLAGS_LONG;
				format++;
				if (*format == L'l') {
					flags |= FLAGS_LONG_LONG;
					format++;
				}
				break;
			case L'h':
				flags |= FLAGS_SHORT;
				format++;
				if (*format == L'h') {
					flags |= FLAGS_CHAR;
					format++;
				}
				break;
#if defined(PRINTF_SUPPORT_PTRDIFF_T)
			case L't':
				flags |= (sizeof(ptrdiff_t) == sizeof(long) ? FLAGS_LONG : FLAGS_LONG_LONG);
				format++;
				break;
#endif
			case L'j':
				flags |= (sizeof(intmax_t) == sizeof(long) ? FLAGS_LONG : FLAGS_LONG_LONG);
				format++;
				break;
			case L'z':
				flags |= (sizeof(size_t) == sizeof(long) ? FLAGS_LONG : FLAGS_LONG_LONG);
				format++;
				break;
			default:
				break;
			}

			// evaluate specifier
			switch (*format) {
			case L'd':
			case L'i':
			case L'u':
			case L'x':
			case L'X':
			case L'o':
			case L'b': {
				// set the base
				unsigned int base;
				if (*format == L'x' || *format == L'X') {
					base = 16U;
				}
				else if (*format == L'o') {
					base = 8U;
				}
				else if (*format == L'b') {
					base = 2U;
					flags &= ~FLAGS_HASH;   // no hash for bin format
				}
				else {
					base = 10U;
					flags &= ~FLAGS_HASH;   // no hash for dec format
				}
				// uppercase
				if (*format == L'X') {
					flags |= FLAGS_UPPERCASE;
				}

				// no plus or space flag for u, x, X, o, b
				if ((*format != L'i') && (*format != L'd')) {
					flags &= ~(FLAGS_PLUS | FLAGS_SPACE);
				}

				// convert the integer
				if ((*format == L'i') || (*format == L'd')) {
					// signed
					if (flags & FLAGS_LONG_LONG) {
#if defined(PRINTF_SUPPORT_LONG_LONG)
						const long long value = va_arg(va, long long);
						idx = _ntoa_long_long(out, buffer, idx, maxlen, (unsigned long long)(value > 0 ? value : 0 - value), value < 0, base, precision, width, flags);
#endif
					}
					else if (flags & FLAGS_LONG) {
						const long value = va_arg(va, long);
						idx = _ntoa_long(out, buffer, idx, maxlen, (unsigned long)(value > 0 ? value : 0 - value), value < 0, base, precision, width, flags);
					}
					else {
						const int value = (flags & FLAGS_CHAR) ? (wchar_t)va_arg(va, int) : (flags & FLAGS_SHORT) ? (short int)va_arg(va, int) : va_arg(va, int);
						idx = _ntoa_long(out, buffer, idx, maxlen, (unsigned int)(value > 0 ? value : 0 - value), value < 0, base, precision, width, flags);
					}
				}
				else {
					// unsigned
					if (flags & FLAGS_LONG_LONG) {
#if defined(PRINTF_SUPPORT_LONG_LONG)
						idx = _ntoa_long_long(out, buffer, idx, maxlen, va_arg(va, unsigned long long), false, base, precision, width, flags);
#endif
					}
					else if (flags & FLAGS_LONG) {
						idx = _ntoa_long(out, buffer, idx, maxlen, va_arg(va, unsigned long), false, base, precision, width, flags);
					}
					else {
						const unsigned int value = (flags & FLAGS_CHAR) ? (wchar_t)va_arg(va, unsigned int) : (flags & FLAGS_SHORT) ? (unsigned short int)va_arg(va, unsigned int) : va_arg(va, unsigned int);
						idx = _ntoa_long(out, buffer, idx, maxlen, value, false, base, precision, width, flags);
					}
				}
				format++;
				break;
			}
#if defined(PRINTF_SUPPORT_FLOAT)
			case L'f':
			case L'F':
				idx = _ftoa(out, buffer, idx, maxlen, va_arg(va, double), precision, width, flags);
				format++;
				break;
#endif  // PRINTF_SUPPORT_FLOAT
			case 'c': {
				unsigned int l = 1U;
				// pre padding
				if (!(flags & FLAGS_LEFT)) {
					while (l++ < width) {
						out(' ', buffer, idx++, maxlen);
					}
				}
				// wchar_t output
				out((wchar_t)va_arg(va, int), buffer, idx++, maxlen);
				// post padding
				if (flags & FLAGS_LEFT) {
					while (l++ < width) {
						out(' ', buffer, idx++, maxlen);
					}
				}
				format++;
				break;
			}

			case L's': {
				wchar_t* p = va_arg(va, wchar_t*);
				unsigned int l = _strlen(p);
				// pre padding
				if (flags & FLAGS_PRECISION) {
					l = (l < precision ? l : precision);
				}
				if (!(flags & FLAGS_LEFT)) {
					while (l++ < width) {
						out(L' ', buffer, idx++, maxlen);
					}
				}
				// string output
				while ((*p != 0) && (!(flags & FLAGS_PRECISION) || precision--)) {
					out(*(p++), buffer, idx++, maxlen);
				}
				// post padding
				if (flags & FLAGS_LEFT) {
					while (l++ < width) {
						out(L' ', buffer, idx++, maxlen);
					}
				}
				format++;
				break;
			}

			case L'p': {
				width = sizeof(void*) * 2U;
				flags |= FLAGS_ZEROPAD | FLAGS_UPPERCASE;
#if defined(PRINTF_SUPPORT_LONG_LONG)
				const bool is_ll = sizeof(uintptr_t) == sizeof(long long);
				if (is_ll) {
					idx = _ntoa_long_long(out, buffer, idx, maxlen, (uintptr_t)va_arg(va, void*), false, 16U, precision, width, flags);
				}
				else {
#endif
					idx = _ntoa_long(out, buffer, idx, maxlen, (unsigned long)((uintptr_t)va_arg(va, void*)), false, 16U, precision, width, flags);
#if defined(PRINTF_SUPPORT_LONG_LONG)
				}
#endif
				format++;
				break;
			}

			case L'%':
				out(L'%', buffer, idx++, maxlen);
				format++;
				break;

			default:
				out(*format, buffer, idx++, maxlen);
				format++;
				break;
			}
		}

		// termination
		out(wchar_t(0), buffer, idx < maxlen ? idx : maxlen - 1U, maxlen);

		// return written chars without terminating \0
		return int(idx);
	}


///////////////////////////////////////////////////////////////////////////////
} // inner
///////////////////////////////////////////////////////////////////////////////


	PRINTF_EXTERN_C inline int printf(const wchar_t* format, ...)
	{
		va_list va;
		va_start(va, format);
		wchar_t buffer[1]{};
		const int ret = inner::_vsnprintf(inner::_out_char, buffer, (size_t)-1, format, va);
		va_end(va);
		return ret;
	}


	PRINTF_EXTERN_C inline int sprintf(wchar_t* buffer, const wchar_t* format, ...)
	{
		va_list va;
		va_start(va, format);
		const int ret = inner::_vsnprintf(inner::_out_buffer, buffer, (size_t)-1, format, va);
		va_end(va);
		return ret;
	}


	PRINTF_EXTERN_C  inline int snprintf(wchar_t* buffer, size_t count, const wchar_t* format, ...)
	{
		va_list va;
		va_start(va, format);
		const int ret = inner::_vsnprintf(inner::_out_buffer, buffer, count, format, va);
		va_end(va);
		return ret;
	}


	PRINTF_EXTERN_C  inline int vsnprintf(wchar_t* buffer, size_t count, const wchar_t* format, va_list va)
	{
		return inner::_vsnprintf(inner::_out_buffer, buffer, count, format, va);
	}


	PRINTF_EXTERN_C inline int fctprintf(void(*out)(wchar_t character, void* arg), void* arg, const wchar_t* format, ...)
	{
		va_list va;
		va_start(va, format);
		const inner::out_fct_wrap_type out_fct_wrap = { out, arg };
		const int ret = inner::_vsnprintf(inner::_out_fct, (wchar_t*)&out_fct_wrap, (size_t)-1, format, va);
		va_end(va);
		return ret;
	}

} // namespace mpaland_dbjdbj 

///////////////////////////////////////////////////////////////////////////////
//
// http://www.firstobject.com/wchar_t-string-on-linux-osx-windows.htm
// wchar_t (UNICODE) is the fastest code on windows
// and not so fast code on LINUX 
// thus we will focus on windows UNICODE builds only in a separate file
// and this is the file
//
///////////////////////////////////////////////////////////////////////////////
// \author (c) C VERSION -- Marco Paland (info@paland.com)
//             2014-2018, PALANDesign Hannover, Germany
//         (c) CPP HEADER ONLY VERSION -- Dusan B. Jovanovic ( dbj@dbj.org )
//             2018, dbj.systems, London, UK 
//
// \license The MIT License (MIT)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// \brief Tiny printf, sprintf and (v)snprintf implementation, optimized for speed on
//        embedded systems with a very limited resources. These routines are thread
//        safe and reentrant!
//        Use this instead of the bloated standard/newlib printf cause these use
//        malloc for printf (and may not be thread safe).
//
///////////////////////////////////////////////////////////////////////////////
