TEMPLATE = subdirs

SUBDIRS += \
    datamodel \
    vt \
    xmvmain

vt.depends = datamodel
xmvmain.depends = vt datamodel

