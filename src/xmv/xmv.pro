# xmv.pro
# Contains the overall project file for the xmas project

TEMPLATE = subdirs
SUBDIRS =   \
    bitpowder \
    datamodel

# xmdmain must be last

CONFIG += ordered
SUBDIRS += \
           xmvmain \

