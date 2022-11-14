# GLSL Visual Studio 2010+ Integration

**Or, how do I include OpenGL source in my project?**

Copyright 2015-2022 by Rebecca Ann Heineman becky@burgerbecky.com

It is released under an MIT Open Source license. Please see LICENSE for license details. Yes, you can use it in a commercial title without paying anything, just give me a credit.

Please? It's not like I'm asking you for money!

## What is GLSL Visual Studio 2010+ Integration?

This is a plug in for Visual Studio 2010 or higher that will allow files with the GLSL extension to automatically be processed with a tool that will remove all white space and comments and then create a C/C++ header that can be directly included in a C++ program for use with OpenGL.

The tool will be listed as "Comment stripper tool"

## What does it require?

Visual Studio 2010, 2012, 2013 or any other MSBuild compatible IDE.

## How do I use it?

Copy the contents of the "plugin" folder to the folder *C:\Program Files (x86)\MSBuild\Microsoft.Cpp\v4.0\BuildCustomizations*

Find this line in your .vcxproj file *&lt;Import Project="$(VCTargetsPath)\Microsoft.Cpp.props"/&gt;*

Insert this after it

```xml
<ImportGroup Label="ExtensionSettings"/>
 <Import Project="$(VCTargetsPath)\BuildCustomizations\glsl.props" />
</ImportGroup>
```

Find this line near the end of your .vcxproj file *&lt;Import Project="$(VCTargetsPath)\Microsoft.Cpp.targets" /&gt;*

Insert this after it.

```xml
<ImportGroup Label="ExtensionTargets"/>
 <Import Project="$(VCTargetsPath)\BuildCustomizations\glsl.targets" />
</ImportGroup>
```

## This is great/sucks! But, how can I change/rewrite/steal the tool?

The source code to the tool is in the folder "source". It will require Burgerlib for some subroutines. <https://github.com/Olde-Skuul/KitchenSink> has the binaries of the library and <https://github.com/Olde-Skuul/burgerlib> has the source to the library.

## To build the tool on windows

1. Install Visual Studio 2022
2. Install the Visual Studio SDK
3. Install the Windows SDK 10 (Any version should do)
4. Install the ARM and ARM64 C++ compilers V143
5. In the Visual Studio extensions, install Microsoft Visual Studio Installer Projects 2022
6. Install Python (Prefer 3)
7. pip install makeprojects
8. Ensure python scripts are in the PATH so python scripts can be run from the command line
9. Set the directory to the folder these instructions reside, usually "GLSL"
10. buildme

## To build the tool on macOS for Xcode

1. Install XCode
2. Install Python (Prefer 3 using brew or macports)
3. pip install makeprojects
4. Ensure python scripts are in the PATH so python scripts can be run from the terminal
5. Set the directory to the folder "stripcomments"
6. buildme
7. Rename the executable in the bin folder to "stripcomments"
8. move the tool into the folder that contains tools, /opt/local/bin is an example
