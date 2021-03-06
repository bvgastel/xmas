# Unit test tools for GUI

## Modification history


* 18-12-2014 Guus Bonnema: added history, problem definition and conclusion
* 15-12-2014 Stefan Versluys: created GUI testtool comment

## Problem definition

We need to have regression unit testing in order to be able to do
refactoring during development. We check testing libraries for C++
for both GUI and non-GUI.

## Conclusion

Only `Qt` has feasible GUI facilities which we will use if appropriate.
We can use `Qt` for non-GUI as well.

## Detail research

GUI test tools are very specific, in many cases they only
support one platform and aiming to test a specific type of
GUI language  such as java, wpf, web...

An other thing to think about is if using a GUI designer
such as Qt creator then it is most appropriate that Qt test
will be the best choice.

Most of the GUI test tools in the [Wiki GUI testing tools] or 
[Top 22 GUI testing tools] are not open source but commercial
products.

### Qt Test

Qt Test is a framework for unit testing Qt based applications and libraries. Qt Test provides all the functionality commonly found in unit testing frameworks as well as extensions for testing graphical user interfaces.

Qt Test offers functionality for mouse and keyboard simulation.

For further information go to [Qt Test tutorial] or [Qt Test].


# Unit test tools for non-GUI


##### References:
* [Wiki GUI testing tools]
* [Top 22 GUI testing tools]

[Qt Test]:https://qt-project.org/doc/qt-5-snapshot/qtest-overview.html
[Qt Test tutorial]:http://doc.qt.io/qt-5/qtest-tutorial.html
[Selenium]:
[Wiki GUI testing tools]:http://en.wikipedia.org/wiki/List_of_GUI_testing_tools
[Top 22 GUI testing tools]:http://blog.dreamcss.com/tools/gui-testing-tools/
