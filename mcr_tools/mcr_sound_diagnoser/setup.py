#!/usr/bin/env python

from distutils.core import setup
from catkin_pkg.python_setup import generate_distutils_setup

d = generate_distutils_setup(
    packages=['mcr_sound_diagnoser'],
    package_dir={'mcr_sound_diagnoser': 'ros/src/mcr_sound_diagnoser'}
)

setup(**d)
