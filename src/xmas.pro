# xmas.pro
# Contains the overall project file for the xmas project

TEMPLATE = subdirs
SUBDIRS =   xmas \
            xmd \
            xmc \
            test

# xmdmain must be last

CONFIG += ordered
SUBDIRS += testmain \
           xmdmain \
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


