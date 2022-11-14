#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
Copy up the glsl visual studio plug in to sdks/visualstudio

Copyright 1995-2022 by Rebecca Ann Heineman becky@burgerbecky.com

It is released under an MIT Open Source license. Please see LICENSE
for license details. Yes, you can use it in a
commercial title without paying anything, just give me a credit.
Please? It's not like I'm asking you for money!
"""

from __future__ import absolute_import, unicode_literals

import os
import sys
from burger import copy_file_if_needed

# pylint: disable=unused-argument

# ``cleanme`` will assume only the function ``clean()`` is used if True.
CLEANME_PROCESS_PROJECT_FILES = False

# Process ``build_rules.py`` in the parent folder if True.
CONTINUE = True

########################################


def postbuild(working_directory, configuration):
    """
    Issue build commands after all IDE projects have been built.

    This function can assume all other build projects have executed for final
    deployment or cleanup

    On exit, return 0 for no error, or a non zero error code if there was an
    error to report.

    Args:
        working_directory
            Directory this script resides in.

        configuration
            Configuration to build, ``all`` if no configuration was requested.

    Returns:
        None if not implemented, otherwise an integer error code.
    """

    # The packaging projects require the executables to be called
    # stripcomments.exe for x86 and stripcomments64.exe for x64

    # After the binaries are built, copy them if needed with their
    # new names

    source_dir = os.path.join(working_directory, 'bin')
    source = os.path.join(source_dir, 'stripcommentsv22w32ltc.exe')
    destination = os.path.join(source_dir, 'stripcomments.exe')
    error = copy_file_if_needed(source, destination)
    if error:
        return error

    source = os.path.join(source_dir, 'stripcommentsv22w64ltc.exe')
    destination = os.path.join(source_dir, 'stripcomments64.exe')
    error = copy_file_if_needed(source, destination)

    return error


# If called as a command line and not a class, perform the build
if __name__ == "__main__":
    sys.exit(postbuild(os.path.dirname(os.path.abspath(__file__)), 'all'))
