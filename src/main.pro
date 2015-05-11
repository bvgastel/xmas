TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += \
    bitpowder \
    xmv \
    interfaces \
    plugins \
    xmd \
    bitpowdertest \
    xmvtest

bitpowdertest.depends = bitpowder
xmvtest.depends = xmv
