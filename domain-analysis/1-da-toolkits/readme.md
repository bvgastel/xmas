Toolkits
========

1. User Interface
2. Language
3. Concurrency toolkit
4. Development environment
5. Build tool
6. Modeling toolkit
7. Testing toolkit

High level requirements
=======================

1. Platform independence
2. C++ tool integration
3. JSON integration
4. Dynamic tool integration

% User Interface Toolkit Domain Analysis
% Guus Bonnema
% 18 october 2014
% 


# High level requirements for UI toolkit

: High Level Requirements table

-------------------------------------------------------------------------------------------
 HL Requirement    Specific requirement												  
 ---------------   ----------------------------------------------------------------------
 Platform
 Independence      		- _A1_ Defined platforms are linux, mac and ms windows                
 					
 						- _A2_ The UI tool must run on the defined platforms

						- _A3_ A development platform should be available for 
							each defined platform.

 				        - _A4_ The UI should support 2D development.

                        - _A5_ The GUI should present itself identical on defined platforms   
                          apart from the platform look and feel.                              

 C++ tool 		        - _B1_ The UI tool should enable integration with C++ tools           
 integration _B_ 	   	  (i.e. not hinder)                                                 

 JSON			        - no requirements for JSON
 integration _C_ 																		  

 Dynamic tool	        - _D1_ The UI tool should enable dynamic integration with             
 integration _D_          C++ tools (i.e. not hinder)                                         
-------------------------------------------------------------------------------------------

# Software Requirements

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

:	_ac-design-001_ The UI has the standard GUI items available (menu, widgets, dialogs, windows)
:	_ac-design-002_ The UI has 2D drawing capabilities (lines and objects)
:	_ac-design-003_ The UI can synchronize different views of the same objects. For example,
	displaying a circle and (in a different window) the syntax for creating the circle.

