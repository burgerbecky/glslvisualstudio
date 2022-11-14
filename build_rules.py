#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
Copy the GLSL visual studio plugin to sdks/visualstudio

Copyright 1995-2022 by Rebecca Ann Heineman becky@burgerbecky.com

It is released under an MIT Open Source license. Please see LICENSE
for license details. Yes, you can use it in a
commercial title without paying anything, just give me a credit.
Please? It's not like I'm asking you for money!
"""

from __future__ import absolute_import, unicode_literals

import os
import sys
from burger import clean_directories, clean_files

# ``cleanme`` will process any child directory with the clean() function if
# True.
CLEANME_GENERIC = True

# ``cleanme`` will assume only the function ``clean()`` is used if True.
CLEANME_PROCESS_PROJECT_FILES = False

# Process listed folders using their rules before processing this folder.
DEPENDENCIES = ['stripcomments', 'install_pre_2017', 'install_2017',
    'install_2019', 'install_2022']

########################################


def clean(working_directory):
    """
    Delete temporary files.

    This function is called by ``cleanme`` to remove temporary files.

    On exit, return 0 for no error, or a non zero error code if there was an
    error to report.

    Args:
        working_directory
            Directory this script resides in.

    Returns:
        None if not implemented, otherwise an integer error code.
    """

    clean_directories(
        working_directory,
        ('bin', 'temp', 'obj', 'Properties', '.vs', '.vscode'))
    clean_files(working_directory, ('Key.snk', '*.user', '*.suo'))
    return 0


# If called as a command line and not a class, perform the build
if __name__ == "__main__":
    sys.exit(clean(os.path.dirname(os.path.abspath(__file__))))
