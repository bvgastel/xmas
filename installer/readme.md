Create a non static Qt installer
================================
* build and deploy Qt project as release
* copy xmas/lib/ content to xmas/installer/packages/data/
* copy xmas/bin/xmdmain.exe to xmas/installer/packages/data/
* try to run xmas/installer/packages/data/xmdmain.exe
* copy every missing Qt dll to xmas/installer/packages/data/ until no warnings
* modify installer framework bin path in xmas/installer/create-win-x86.cmd
* e.g. run xmas/installer/create-win-x86.cmd script
* xmas/installer/xmd-windows-x86-v1.0.0.exe is ready to install as standalone