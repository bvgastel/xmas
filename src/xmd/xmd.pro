# xmd.pro
# Contains the overall project file for the xmas project

TEMPLATE = subdirs

# xmdmain must be last

CONFIG += ordered
SUBDIRS += \
    plugins \
    xmd \
    xmdtest \
    xmdmain

# set the subdirectories for the libraries

# libxmd.subdir = xmd
