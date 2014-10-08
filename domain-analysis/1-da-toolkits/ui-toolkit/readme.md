User Interface Toolkit
======================

High level requirements for UI toolkit
--------------------------------------

+-----------------+-----------------------------------------------------------------------+
| HL Requirement  | Specific requirement												  |
+=================+=======================================================================+
| Platform		  | - _A1_ Defined platforms are linux, max and ms windows                |
| Independence	  | - _A2_ The UI tool must run on the defined platforms                  |
| _A_             | - _A3_ A development environment should be available for              |
| 				  |	  the defined platforms.                                              |
| 				  | - _A4_ The UI should support 2D development.                          |
|                 | - _A5_ The GUI should present itself identical on defined platforms   |
|                 |   apart from the platform look and feel.                              |
+-----------------+-----------------------------------------------------------------------+ 
| C++ tool 		  | - _B1_ The UI tool should enable integration with C++ tools           |
| integration _B_ |		(i.e. not hinder)                                                 |
+-----------------+-----------------------------------------------------------------------+                 
| JSON			  |                                                                       |
| integration _C_ |																		  |
+-----------------+-----------------------------------------------------------------------+
| Dynamic tool	  | - _D1_ The UI tool should enable dynamic integration with             |
| integration _D_ |   C++ tools (i.e. not hinder)                                         |
+-----------------+-----------------------------------------------------------------------+

Software Requirements
---------------------

Development support

:	_ac-dev-001_ UI items work identical on the defined platforms, except for look and feel.
	(_A2_,_A3_)
:   _ac-dev-002_ The build utilities works unaltered on the defined platforms with identical results
	(_A2_, _A3_)
:	_ac-dev-003_ Each defined platform has an acceptable development environment for the UI toolkit
	(_A3_)

Language support

:   _ac-lang-001_ The UI is able to work in a dynamic fashion with C++ modules (_B1_)

UI Design support

:	_ac-design-001_ The UI has the standard GUI items available (menu, widgets, dialogs, windows)(_
:	_ac-design-002_ The UI has 2D drawing capabilities (lines and objects)
:	_ac-design-003_ The UI can synchronize different views of the same objects. For example,
	displaying a circle and (in a different window) the syntax for creating the circle.

