# xmd.pro
# Contains the overall project file for the xmas project

TEMPLATE = subdirs

# xmdmain must be last

CONFIG += ordered
SUBDIRS += \
    xmd \
    xmdtest \
    xmdmain

