# xmas.pro
# Contains the overall project file for the xmas project

TEMPLATE = subdirs
SUBDIRS =   \
            xmd \
            xmc \ 
    bitpowder \
    datamodel \

# xmdmain must be last

CONFIG += ordered
SUBDIRS += \
           xmdmain \
           xmcmain


# set the subdirectories for the libraries

libxmd.subdir = xmd
libxmc.subdir = xmc

