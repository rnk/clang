/*===---- msvcrt_picker.h - C runtime picker -------------------------------===
 *
 * Copyright (c) 2013 Reid Kleckner
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 *===-----------------------------------------------------------------------===
 */

#ifndef __MSVCRT_PICKER_H
#define __MSVCRT_PICKER_H

#ifdef _DLL
# ifdef _DEBUG
#  define __MSVCRT_LIBRARY "msvcrtd"
# else
#  define __MSVCRT_LIBRARY "msvcrt"
# endif
#else
# ifdef _DEBUG
#  define __MSVCRT_LIBRARY "libcmtd"
# else
#  define __MSVCRT_LIBRARY "libcmt"
# endif
#endif

/* This will cause link.exe to automatically link the usual C runtime. */
#pragma comment(lib, __MSVCRT_LIBRARY)

/* This provides POSIX compatibility (maps 'open' to '_open'), which most users
 * want.  MSVC has a switch to turn off this autolinking, but it's not
 * implemented in clang yet.
 */
#pragma comment(lib, "OLDNAMES")

#endif /* __MSVCRT_PICKER_H */
