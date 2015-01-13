# xmas.pro
# Contains the overall project file for the xmas project

TEMPLATE = subdirs
SUBDIRS =   xmas \
            xmd \
            xmc \
            test

# xmasmain must be last

CONFIG += ordered
SUBDIRS += testmain \
           xmasmain \
           xmcmain


# set the subdirectories for the libraries

libxmas.subdir = xmas
libxmd.subdir = xmd
libxmc.subdir = xmc
libtest.subdir = test

# add this to have qmake determine dependencies with
# the libraries correctly.

test.depends = xmas xmd
xmd.depends = xmas
xmc.depends = xmas xmd


