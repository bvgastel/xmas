# xmv.pro
# Contains the overall project file for the xmas project

TEMPLATE = subdirs

# xmdmain must be last

CONFIG += ordered
SUBDIRS += \
    datamodel \
    vt \
    xmvmain


