TEMPLATE = subdirs

SUBDIRS += \
    syntaxcheckerplugin \
    mainplugin \

mainplugin.depends = syntaxcheckerplugin
