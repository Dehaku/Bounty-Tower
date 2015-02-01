#!/usr/bin/env python

from os import walk
from subprocess import call

ignore = [
    ('src', 'main.cpp')  # Critter brain rewrite
]

for (dirpath, dirnames, filenames) in walk('src'):
    for f in filenames:
        if (dirpath, f) not in ignore:
            call(["clang-format", "-i", "{}/{}".format(dirpath, f)])

print("Done.")
