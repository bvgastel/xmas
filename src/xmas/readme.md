# xmas basic directory

This directory contains basic xmas source files

## design decisions

### Containers

We use Qt containers for the following reasons:

1. Qt containers work the same on all platforms
2. Qt containers work with copy-on-write. 
3. Qt containers have java like iterators.
4. Qt containers can use QDataStream for streaming.

Additionally, for some platforms (mobile) Qt containers
are the only option.

## build instructions


