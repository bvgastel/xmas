# xmv.pro
# Contains the overall project file for the xmas project

TEMPLATE = subdirs
SUBDIRS =   \
    datamodel \
    vt

# xmdmain must be last

CONFIG += ordered
SUBDIRS += \
           xmvmain

