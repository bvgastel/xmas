Prepare a Linux platform
========================


Development From Fedora
-----------------------

Download and install :

- fltk-1.3.3		from the site of fltk (currently http://www.fltk.org).
					if Fedora presents this version in her repo, yum install suffices
- cmake 2.8			probably from the fedora repo with yum install.

Choose and install an IDE. Some of us use Code::Blocks:

- codeblocks			use yum install from Fedora

If you want to compile codeblocks from source, be sure to read their instruction
carefully. Currently they require WxWidget compiled as one large library.

Other Linux versions:

Use your package manager to install the required software (see Fedora).

Cross-compiling from Linux to Windows
-------------------------------------

From fltkgeneral forum:

>> I was recently able to cross compile using mingw on Linux and then testing under wine.
>> FLTK compiled without a hitch (used mingw32-configure and mingw32-make). 
>> The only problem was figuring out static libraries to pull in. I had to add these ld flags to my apps makefile
>>     -lgdi32\
>>    -lcomdlg32\
>>    -static-libgcc\
>>    -static-libstdc++\
>>    -mwindows

Thus we can recompile from linux to windows.
