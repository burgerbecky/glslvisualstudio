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

#include "stripcomments.h"

static const char g_GenerationHeader[] =
    "/***************************************\n"
    "\n"
    "    This file was generated by the\n"
    "    stripcomments tool\n"
    "\n"
    "***************************************/\n"
    "\n";

/***************************************

    By using a simple state machine, parse the text as if it's a C++ file and
    remove all empty lines, whitespace and comments.

***************************************/

void BURGER_API StripComments(
    OutputMemoryStream* pOutput, const char* pInput, WordPtr uLength)
{
    enum eState {
        REMOVING_WHITESPACE,
        AFTER_TOKEN,
        INSIDE_QUOTE,
        ALLOW_TEXT,
        ASTERISK_COMMENT,
        SLASH_COMMENT
    };

    if (uLength) {
        // Not in an active quote
        Word uQuoteCharacter = 0;
        // Remove whitespace from the start of a line
        eState eCurrentState = REMOVING_WHITESPACE;
        eState eOldAsteriskState = REMOVING_WHITESPACE;
        Word uCharacter;
        do {
            // Pull out a character
            uCharacter = reinterpret_cast<const Word8*>(pInput)[0];
            --uLength;
            ++pInput;
            // Process according to the current state
            switch (eCurrentState) {

            // Dispose of all whitespace, until text appears, then
            // fall through to normal text parsing
            case REMOVING_WHITESPACE:
                if ((uCharacter == ' ') || (uCharacter == '\t') ||
                    (uCharacter == '\r') || (uCharacter == '\n')) {
                    break;
                }
                if ((uCharacter == '/') && (uLength)) {
                    if (reinterpret_cast<const Word8*>(pInput)[0] == '/') {
                        --uLength;
                        ++pInput;
                        eCurrentState = SLASH_COMMENT;
                        break;
                    }
                    if (reinterpret_cast<const Word8*>(pInput)[0] == '*') {
                        ++pInput;
                        --uLength;
                        eCurrentState = ASTERISK_COMMENT;
                        eOldAsteriskState = REMOVING_WHITESPACE;
                        break;
                    }
                }
                eCurrentState = ALLOW_TEXT;

            // Handle normal text processing until a token end is reached
            case ALLOW_TEXT:
                // Entering a quote?
                if ((uCharacter == '\'') || (uCharacter == '"')) {
                    uQuoteCharacter = uCharacter;
                    eCurrentState = INSIDE_QUOTE;
                    pOutput->Append(static_cast<char>(uCharacter));
                    break;
                }
                // Is this an escape sequence?
                if (uCharacter == '\\') {
                    if (uLength) {
                        pOutput->Append('\\');
                        uCharacter = reinterpret_cast<const Word8*>(pInput)[0];
                        --uLength;
                        ++pInput;
                    }
                }
                // In between tokens
                if ((uCharacter == ' ') || (uCharacter == '\t')) {
                    eCurrentState = AFTER_TOKEN;
                    break;
                }
                // Is this an end of line?
                if (uCharacter == '\r') {
                    if (uLength &&
                        reinterpret_cast<const Word8*>(pInput)[0] == '\n') {
                        ++pInput;
                        --uLength;
                    }
                    uCharacter = '\n';
                }
                if (uCharacter == '\n') {
                    pOutput->Append('\n');
                    eCurrentState = REMOVING_WHITESPACE;
                    break;
                }
                if (uCharacter == '/') {
                    if (uLength) {
                        if (reinterpret_cast<const Word8*>(pInput)[0] == '/') {
                            ++pInput;
                            --uLength;
                            eCurrentState = SLASH_COMMENT;
                            pOutput->Append('\n');
                            break;
                        }
                        if (reinterpret_cast<const Word8*>(pInput)[0] == '*') {
                            ++pInput;
                            --uLength;
                            eCurrentState = ASTERISK_COMMENT;
                            eOldAsteriskState = ALLOW_TEXT;
                            break;
                        }
                    }
                }
                pOutput->Append(static_cast<char>(uCharacter));
                break;

            // After a parsed token, and removing whitespace, but will
            // insert a space if another token is found
            case AFTER_TOKEN:
                if ((uCharacter == ' ') || (uCharacter == '\t')) {
                    break;
                }
                if ((uCharacter == '\r') || (uCharacter == '\n')) {
                    pOutput->Append('\n');
                    eCurrentState = REMOVING_WHITESPACE;
                    break;
                }
                if ((uCharacter == '/') && (uLength)) {
                    if (reinterpret_cast<const Word8*>(pInput)[0] == '/') {
                        --uLength;
                        ++pInput;
                        pOutput->Append('\n');
                        eCurrentState = SLASH_COMMENT;
                        break;
                    }
                    if (reinterpret_cast<const Word8*>(pInput)[0] == '*') {
                        ++pInput;
                        --uLength;
                        eCurrentState = ASTERISK_COMMENT;
                        eOldAsteriskState = REMOVING_WHITESPACE;
                        break;
                    }
                }
                pOutput->Append(' ');
                pOutput->Append(static_cast<char>(uCharacter));
                eCurrentState = ALLOW_TEXT;
                break;

            // Is inside a quote
            case INSIDE_QUOTE:
                // Exiting the quote?
                if (uCharacter == uQuoteCharacter) {
                    eCurrentState = ALLOW_TEXT;
                }
                // Is this an escape sequence?
                if (uCharacter == '\\') {
                    if (uLength) {
                        pOutput->Append('\\');
                        uCharacter = reinterpret_cast<const Word8*>(pInput)[0];
                        --uLength;
                        ++pInput;
                    }
                }
                // Is this an end of line?
                if (uCharacter == '\r') {
                    if (uLength &&
                        reinterpret_cast<const Word8*>(pInput)[0] == '\n') {
                        ++pInput;
                        --uLength;
                    }
                    uCharacter = '\n';
                }
                // Output the quoted string
                pOutput->Append(static_cast<char>(uCharacter));
                break;

            // Wait for a "*/" end quote stream
            case ASTERISK_COMMENT:
                if (uCharacter == '*') {
                    if (uLength &&
                        reinterpret_cast<const Word8*>(pInput)[0] == '/') {
                        ++pInput;
                        --uLength;
                        eCurrentState = eOldAsteriskState;
                    }
                }
                break;

            // Parse to end of line for //
            case SLASH_COMMENT:
                // Is there a line continuation character?
                if (uCharacter == '\\') {
                    // While supported by some compilers, this case
                    // shouldn't be found in good source code. It's here only
                    // for completeness and it really should issue an error
                    // condition. Thankfully, visual studio issues a warning for
                    // source containing this case
                    if (uLength) {
                        if (reinterpret_cast<const Word8*>(pInput)[0] == '\r') {
                            ++pInput;
                            --uLength;
                            if (uLength &&
                                reinterpret_cast<const Word8*>(pInput)[0] ==
                                    '\n') {
                                ++pInput;
                                --uLength;
                            }
                            // Stay in the // comment mode
                            break;
                        }

                        if (reinterpret_cast<const Word8*>(pInput)[0] == '\n') {
                            ++pInput;
                            --uLength;
                            // Stay in the // comment mode
                            break;
                        }
                    }
                    // If it got here, it was a false alarm, discard the
                    // character
                }

                // Time to stop discarding text?
                if (uCharacter == '\r') {
                    if (uLength &&
                        reinterpret_cast<const Word8*>(pInput)[0] == '\n') {
                        ++pInput;
                        --uLength;
                    }
                    uCharacter = '\n';
                }
                if (uCharacter == '\n') {
                    // Exit this mode, go back to whitespace removal
                    eCurrentState = REMOVING_WHITESPACE;
                }
                break;
            }
        } while (uLength);
    }
}

/***************************************

    Simple utility to convert a shader source file into a header that can be
    included for direct compilation into an application.

***************************************/

int BURGER_ANSIAPI main(int argc, const char** argv)
{
    ConsoleApp MyApp(argc, argv);

    // Switches

    CommandParameterBooleanTrue CPP("Convert to C++ string", "c");
    CommandParameterString Label("Label override [-l g_ConstCharLabel]", "l");

    const CommandParameter* MyParms[] = {&CPP, &Label};
    argv = MyApp.GetArgv();
    argc = CommandParameter::Process(MyApp.GetArgc(), argv, MyParms,
        sizeof(MyParms) / sizeof(MyParms[0]),
        "Usage: stripcomments InputFile [Outputfile]\n\n"
        "This will strip C++ style comments from a source file.\n"
        "Copyright Rebecca Ann Heineman.\n",
        3, 3);
    Globals::SetErrorCode(10);
    if (argc >= 0) {
        MyApp.SetArgc(argc);
        Filename InputFilename;
        InputFilename.SetFromNative(argv[1]);
        WordPtr uLength;
        const char* pInputBuffer = static_cast<const char*>(
            FileManager::LoadFile(&InputFilename, &uLength));

        if (!pInputBuffer) {
            printf("Can't open %s!", InputFilename.GetNative());

        } else {
            // Perform the strip
            OutputMemoryStream Output;
            StripComments(&Output, pInputBuffer, uLength);
            Free(pInputBuffer);

            Filename OutputFilename;
            OutputFilename.SetFromNative(argv[2]);

            // Convert to a source file?

            if (CPP.GetValue()) {
                pInputBuffer =
                    static_cast<const char*>(Output.Flatten(&uLength));
                Output.Clear();

                // Insert the header
                Output.Append(g_GenerationHeader);

                String BaseName;
                OutputFilename.BaseName(&BaseName);
                // Strip the extension
                char* pExtension =
                    StringCharacterReverse(BaseName.GetPtr(), '.');
                if (pExtension) {
                    pExtension[0] = 0;
                }

                Output.Append("const char ");
                const char* pLabel = Label.GetValue();
                if (pLabel[0]) {
                    Output.Append(pLabel);
                } else {
                    String TempLabel("g_", BaseName.GetPtr());
                    Output.Append(TempLabel.GetPtr());
                }
                Output.Append("[] = ");
                if (!uLength) {
                    Output.Append("\"\"");
                } else {
                    const char* pWork = pInputBuffer;
                    Word bQuoted = FALSE;
                    do {
                        if (!bQuoted) {
                            Output.Append("\n\"");
                            bQuoted = TRUE;
                        }
                        Word uTemp = reinterpret_cast<const Word8*>(pWork)[0];
                        ++pWork;
                        --uLength;
                        if (uTemp == '\r') {
                            if (uLength &&
                                reinterpret_cast<const Word8*>(pWork)[0] ==
                                    '\n') {
                                ++pWork;
                                --uLength;
                            }
                            uTemp = '\n';
                        }
                        if (uTemp == '\n') {
                            Output.Append("\\n\"");
                            bQuoted = FALSE;
                            continue;
                        }
                        if (uTemp == '\"') {
                            Output.Append("\\\"");
                            continue;
                        }
                        if (uTemp == '\\') {
                            Output.Append("\\\\");
                            continue;
                        }
                        Output.Append(static_cast<char>(uTemp));
                    } while (uLength);
                }
                Free(pInputBuffer);
                Output.Append(";\n\n");
            }

            // Save the text file to disk

            Word uResult = Output.SaveFile(&OutputFilename);
            if (uResult) {
                printf("Error in saving %s\n", OutputFilename.GetNative());
            }
            Globals::SetErrorCode(static_cast<int>(uResult));
        }
    }
    return Globals::GetErrorCode();
}
