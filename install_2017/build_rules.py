#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
Copy up the VITACG visual studio plug in to sdks/visualstudio

Copyright 1995-2014 by Rebecca Ann Heineman becky@burgerbecky.com

It is released under an MIT Open Source license. Please see LICENSE
for license details. Yes, you can use it in a
commercial title without paying anything, just give me a credit.
Please? It's not like I'm asking you for money!
"""

from __future__ import absolute_import, unicode_literals

import os
import sys
from makeprojects import build

# Unused arguments
# pylint: disable=W0613


def rules(command, working_directory, **kargs):
    """
    Main entry point for build_rules.py.

    When ``makeprojects``, ``cleanme``, or ``buildme`` is executed, they will
    call this function to perform the actions required for build customization.

    The parameter ``working_directory`` is required, and if it has no default
    parameter, this function will only be called with the folder that this
    file resides in. If there is a default parameter of ``None``, it will be
    called with any folder that it is invoked on. If the default parameter is a
    directory, this function will only be called if that directory is desired.

    The optional parameter of ``root``` alerts the tool if subsequent processing
    of other ``build_rules.py`` files are needed or if set to have a default
    parameter of ``True``, processing will end once the calls to this
    ``rules()`` function are completed.

    Commands are 'build', 'clean', 'prebuild', 'postbuild', 'project',
    'configurations'

    Arg:
        command: Command to execute.
        working_directory: Directory for this function to operate on.
        root: If True, stop execution upon completion of this function
        kargs: Extra arguments specific to each command.
    Return:
        Zero on no error or no action.
    """

    if command == 'prebuild':

        # The installer needs the exe files built first, do so.
        stripcomments_dir = os.path.join(
            os.path.dirname(working_directory),
            'stripcomments')
        return build(
            working_directory=stripcomments_dir,
            args=[])

    return 0


# If called as a command line and not a class, perform the build
if __name__ == "__main__":
    sys.exit(rules('prebuild', os.path.dirname(os.path.abspath(__file__))))
