#!/bin/sh
# Strips png files of content that causes linux to issue the warning:
#
# 		"libpng warning: iCCP: Not recognizing known sRGB profile that has been edited"
#
# The convert utility is part of ImageMagick
#
find . -name "*.png" -print0 | while read -d $'\0' file; do convert "$file" -strip "$file"; done
