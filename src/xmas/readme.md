# xmas basic directory

This directory contains basic xmas source files

## design decisions

### Containers

We use Qt containers for 2 reasons:

1. Qt containers work the same on all platforms
2. Qt containers work with copy-on-write. 

Additionally, for some platforms (mobile) Qt containers
are the only option.

## build instructions


