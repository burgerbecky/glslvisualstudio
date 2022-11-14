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

# ``cleanme`` will assume only the function ``clean()`` is used if True.
CLEANME_PROCESS_PROJECT_FILES = False

# Process ``build_rules.py`` in the parent folder if True.
CLEANME_CONTINUE = True

# Build the folders listed before processing this folder.
BUILDME_DEPENDENCIES = ['../stripcomments']
