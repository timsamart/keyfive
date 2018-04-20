========================================================================
    CONSOLE APPLICATION : KeyFive Project Overview
========================================================================

This Application searches for a midi-input port and uses the five keys defined in the config.ini to implement a five finger single hand writing system. 
While the application is running you can switch to any application and start typing with the midi keyboard or similar device.
The Idea is to have some custom midi device just for the right or left hand.

here follows the pattern. To switch between the to keyboards (numbers and letters) just double tap all five fingers.
from Thumb left to right (right handed) 

				numbers		letters
|0|0|0|0|x|		-		i
|0|0|0|x|0|		8		o
|0|0|0|x|x|		+		ö
|0|0|x|0|0|		4		t
|0|0|x|0|x|		non		ü
|0|0|x|x|0|		non		n
|0|0|x|x|x|		non		l
|0|x|0|0|0|		2		e
|0|x|0|0|x|		non		b
|0|x|0|x|0|		0		y
|0|x|0|x|x|		non		j
|0|x|x|0|0|		6		s
|0|x|x|0|x|		/		v
|0|x|x|x|0|		*		h
|0|x|x|x|x|		non		d
|x|0|0|0|0|		1		a
|x|0|0|0|x|		non		x
|x|0|0|x|0|		9		y
|x|0|0|x|x|		non		f
|x|0|x|0|0|		5		m
|x|0|x|0|x|		non		w
|x|0|x|x|0|		non		c
|x|0|x|x|x|		non		p
|x|x|0|0|0|		3		u
|x|x|0|0|x|		DEL 		DEL
|x|x|0|x|0|		non		q
|x|x|0|x|x|		BCKSPC		BCKSPC
|x|x|x|0|0|		7		r
|x|x|x|0|x|		non		z
|x|x|x|x|0|		non		g
|x|x|x|x|x|		space		space
|x|x|x|x|x|		change keyboard		x2


KeyFive.vcxproj
    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

KeyFive.vcxproj.filters
    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the association between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).

KeyFive.cpp
    This is the main application source file.

/////////////////////////////////////////////////////////////////////////////
Other standard files:



/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////
