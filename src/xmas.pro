# xmas.pro
# Contains the overall project file for the xmas project

TEMPLATE = subdirs
SUBDIRS = 	xmas \
			xmd \
			xmc \
			test

# xmasmain must be last

CONFIG += ordered
SUBDIRS += xmasmain

