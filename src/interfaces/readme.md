# Interfaces and plugins


## Rules of engagement for plugins

### plugin library : standalone
When developing plugins you depend on interfaces.
The main rule for plugins is that each plugin library must
be a stand alone plugin library with only one plugin class.
Your plugin class *may* refer to on declarations in other libraries.
Your plugin class *may not* refer to any implementation outside
the plugin library. This is a very strict rule. Ignoring it will
lead to the QPluginLoader not recognizing the library as a plugin library.

### Interface : accessing implementation from plugin
All classes in the interface have definitions. Some classes have
implementations in the interface. The plugin cannot refer to
these classes directly. But, if the plugin creates and uses a
class A in its library, then class A *can* use the implemented
class in the interface library.

### Redirecting std::out and std::err in threads
If the plugin starts a thread, it *cannot* redirect standard output for this
thread, because the thread uses the standard output of the process. Any redirection
should take place from the process main thread.

### Redirecting std::out and std::err from processes
If the plugin starts a new process, then for this process it can safely redirect the
standard output and send it back to the caller using a signal. This way
each process will have an independant stream of messages that the caller can connect
to using the signals.
