Development
-----------

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
