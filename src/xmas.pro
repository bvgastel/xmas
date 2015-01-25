# xmas.pro
# Contains the overall project file for the xmas project

TEMPLATE = subdirs
SUBDIRS =   \
            xmas \
            xmd \
            xmc 

# xmdmain must be last

CONFIG += ordered
SUBDIRS += \
           xmdmain \
           xmcmain


# set the subdirectories for the libraries

libxmas.subdir = xmas
libxmd.subdir = xmd
libxmc.subdir = xmc

