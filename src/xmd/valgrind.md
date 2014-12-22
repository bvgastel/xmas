Valgrind
========

Valgrind [1] is an instrumentation framework for building dynamic analysis tools.
There are Valgrind tools that can automatically detect many memory management
and threading bugs, and profile your programs in detail. You can also use Valgrind
to build new tools.

The Valgrind distribution currently includes six production-quality tools: a memory
error detector, two thread error detectors, a cache and branch-prediction profiler,
a call-graph generating cache and branch-prediction profiler, and a heap profiler.
It also includes three experimental tools: a stack/global array overrun detector, a
second heap profiler that examines how heap blocks are used, and a SimPoint basic
block vector generator. It runs on the following platforms: X86/Linux, AMD64/Linux,
ARM/Linux, ARM64/Linux, PPC32/Linux, PPC64/Linux, PPC64BE/Linux, S390X/Linux,
MIPS32/Linux, MIPS64/Linux, ARM/Android (2.3.x and later), X86/Android (4.0 and later),
MIPS32/Android, X86/Darwin and AMD64/Darwin (Mac OS X 10.9, with limited support for 10.8).


xMAS memory analysis
--------------------

Valgrind, and specifically the MemCheck tool are used to detect memory leaks in
the XMAS design tool. Unfortunately, MemCheck reports many errors not caused by
the design tool but by supporting libraries (Qt and the libraries Qt depends on).
A suppression file can be passed to valgrind to filter these error messages.
The file qt54.supp contains all irrelevant errors caused by the minimal Qt application.

You can use this file like this:

valgrind --leak-check=full --suppressions=qt54.supp ./XMD

If the supplied suppression file is not adequate for your system (different platforms
and versions of libraries may lead to different errors) you can create your own
suppression file.

To do so:

  1. Create a minimal Qt Application (e.g. using QtCreator)
  2. Run valgrind and redirect the output to a log file:
     - valgrind --leak-check=full --error-limit=no --gen-suppressions=all --log-file="vg.log" ./{BINARY-NAME}
     where {BINARY-NAME} is the executable name of the minimal Qt Application
  3. Edit the log file and remove all lines starting with "=="
     - sed 's/==.*//' < vg.log > qt54.supp

   
Note:

Valgrind is only available on Linux and MacOS. For memory analysis on Windows
other tools are required (e.g. those provided by Visual Studio). Alternatively
QtCreator on Windows has an option te run Valgrind remotely on a Linux system.

References:
-----------
[ 1]    http://www.valgrind.org
