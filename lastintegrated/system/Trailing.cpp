

    // File:- trailing.cpp


    // 20/09/95 09:31

 //============================================================================

#include "stdafx.h"
        //#include <stdlib.h>
        //#include <stdio.h>
        //#include <string.h>


 // Whitespace strip routine - expects a string in, strips the space
// from THAT string, and returns the original address of the string.
// I.e. it modifies the original string; does not create a new string
// Required for NameConcat routine
 //----------------------------------------------------------------------------
char* StripTrailingWhitespace(char* String)
{
	// Get the index of the last character in the string
	int StringPos = strlen(String) - 1;
	if (StringPos > -1)
	{
		// Move back towards string start until start or nonspace found
		while(StringPos > -1 && String[StringPos] == ' ') 
		{
			StringPos--;
		}

		// Truncate string at last position plus one
		ASSERT (StringPos > -2);		// Can't insert before start of string
		String[StringPos + 1] = '\0';
	}
	// And return its address for possible later use
	return(String);
}


 // new line strip routine - expects a string in, strips the newline char
// from THAT string, and returns the original address of the string.
// I.e. it modifies the original string; does not create a new string
 //----------------------------------------------------------------------------
char* StripTrailingNewLine(char* String)
{
	// Get the index of the last character in the string
	int StringPos = strlen(String) - 1;
	if (StringPos > -1)
	{
		// Move back towards string start until start or nonspace found
		while(StringPos > -1 && String[StringPos] == '\n') 
		{
			StringPos--;
		}

		// Truncate string at last position plus one
		ASSERT (StringPos > -2);		// Can't insert before start of string
		String[StringPos + 1] = '\0';

		// And return its address for possible later use
	}
	return(String);
}
