# xmas_common.pri
# Includes common configuration for all subdirectory .pro files

INCLUDEPATH += . ..
WARNINGS += -Wall

TEMPLATE = lib

# The following keeps the generated files separate from
# the source files.

UI_DIR = uics
MOC_DIR = mocs
OBJECTS_DIR = objs

