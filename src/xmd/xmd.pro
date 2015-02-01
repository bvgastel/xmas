# xmas.pro
# Contains the overall project file for the xmas project

TEMPLATE = subdirs
SUBDIRS =   \
            xmd \

# xmdmain must be last

CONFIG += ordered
SUBDIRS += \
           xmdmain \


# set the subdirectories for the libraries

libxmd.subdir = xmd

