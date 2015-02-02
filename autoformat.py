#!/usr/bin/env python

import sys
from os import walk
from subprocess import call

no_ignore = len(sys.argv) > 1 and sys.argv[1] == 'noignore'

if no_ignore:
        print("Warning: Formatting all files, including ignored ones")

ignore = [
    ('src', 'main.cpp')  # Critter brain rewrite
]

for (dirpath, dirnames, filenames) in walk('src'):
    for f in filenames:
        if no_ignore or (dirpath, f) not in ignore:
            call(["clang-format", "-i", "{}/{}".format(dirpath, f)])
        else:
            print("Ignoring {}/{}".format(dirpath, f))

print("Done.")
