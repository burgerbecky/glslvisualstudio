/***************************************

    Simple utility to strip all C++ style comments from a text file and output
    it into a C++ include file. Used mostly for converting GLSL files into
    source that can be directly linked into an OpenGL program.

    Copyright (c) 1995-2014 by Rebecca Ann Heineman <becky@burgerbecky.com>

    It is released under an MIT Open Source license. Please see LICENSE for
    license details. Yes, you can use it in a commercial title without paying
    anything, just give me a credit.

    Please? It's not like I'm asking you for money!

***************************************/

#ifndef __STRIPCOMMENTS_H__
#define __STRIPCOMMENTS_H__

#ifndef __BURGER__
#include <burger.h>
#endif

extern void BURGER_API StripComments(
    OutputMemoryStream* pOutput, const char* pInput, uintptr_t uLength);
extern int BURGER_ANSIAPI main(int argc, const char** argv);

#endif
