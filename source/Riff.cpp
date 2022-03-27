/*
AppleWin : An Apple //e emulator for Windows

Copyright (C) 1994-1996, Michael O'Brien
Copyright (C) 1999-2001, Oliver Schmidt
Copyright (C) 2002-2005, Tom Charlesworth
Copyright (C) 2006-2007, Tom Charlesworth, Michael Pohoreski

AppleWin is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

AppleWin is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with AppleWin; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/* Description: RIFF funcs
 *
 * Author: Various
 */

#include "StdAfx.h"
#include "Riff.h"

namespace
{
	RiffOutput g_RiffOutput;  // default output
}

RiffOutput::RiffOutput()
	: myRiffFile(INVALID_HANDLE_VALUE)
	, myTotalOffset(0)
	, myDataOffset(0)
	, myTotalNumberOfBytesWritten(0)
	, myNumChannels(0)
{
}

RiffOutput::~RiffOutput()
{
	FinishWriteFile();
}

int RiffOutput::InitWriteFile(const char* pszFile, unsigned int sample_rate, unsigned int NumChannels)
{
	FinishWriteFile(); // avoid leaks if the file was already open

	myRiffFile = CreateFile(pszFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (myRiffFile == INVALID_HANDLE_VALUE)
		return 1;

	myTotalNumberOfBytesWritten = 0;
	myNumChannels = NumChannels;

	//

	UINT32 temp32;
	UINT16 temp16;

	DWORD dwNumberOfBytesWritten;

	WriteFile(myRiffFile, "RIFF", 4, &dwNumberOfBytesWritten, NULL);

	temp32 = 0;				// total size
	myTotalOffset = SetFilePointer(myRiffFile, 0, NULL, FILE_CURRENT);
	WriteFile(myRiffFile, &temp32, 4, &dwNumberOfBytesWritten, NULL);

	WriteFile(myRiffFile, "WAVE", 4, &dwNumberOfBytesWritten, NULL);

	//

	WriteFile(myRiffFile, "fmt ", 4, &dwNumberOfBytesWritten, NULL);

	temp32 = 16;			// format length
	WriteFile(myRiffFile, &temp32, 4, &dwNumberOfBytesWritten, NULL);

	temp16 = 1;				// PCM format
	WriteFile(myRiffFile, &temp16, 2, &dwNumberOfBytesWritten, NULL);

	temp16 = NumChannels;		// channels
	WriteFile(myRiffFile, &temp16, 2, &dwNumberOfBytesWritten, NULL);

	temp32 = sample_rate;	// sample rate
	WriteFile(myRiffFile, &temp32, 4, &dwNumberOfBytesWritten, NULL);

	temp32 = sample_rate * 2 * NumChannels;	// bytes/second
	WriteFile(myRiffFile, &temp32, 4, &dwNumberOfBytesWritten, NULL);

	temp16 = 2 * NumChannels;	// block align
	WriteFile(myRiffFile, &temp16, 2, &dwNumberOfBytesWritten, NULL);

	temp16 = 16;			// bits/sample
	WriteFile(myRiffFile, &temp16, 2, &dwNumberOfBytesWritten, NULL);

	//

	WriteFile(myRiffFile, "data", 4, &dwNumberOfBytesWritten, NULL);

	temp32 = 0;				// data length
	myDataOffset = SetFilePointer(myRiffFile, 0, NULL, FILE_CURRENT);
	WriteFile(myRiffFile, &temp32, 4, &dwNumberOfBytesWritten, NULL);

	return 0;
}

int RiffOutput::FinishWriteFile()
{
	if(myRiffFile == INVALID_HANDLE_VALUE)
		return 1;

	//

	UINT32 temp32;

	DWORD dwNumberOfBytesWritten;
	
	temp32 = myTotalNumberOfBytesWritten - (myTotalOffset + 4);
	SetFilePointer(myRiffFile, myTotalOffset, NULL, FILE_BEGIN);
	WriteFile(myRiffFile, &temp32, 4, &dwNumberOfBytesWritten, NULL);

	temp32 = myTotalNumberOfBytesWritten - (myDataOffset + 4);
	SetFilePointer(myRiffFile, myDataOffset, NULL, FILE_BEGIN);
	WriteFile(myRiffFile, &temp32, 4, &dwNumberOfBytesWritten, NULL);

	int result = CloseHandle(myRiffFile);
	myRiffFile = INVALID_HANDLE_VALUE;

	return result;
}

int RiffOutput::PutSamples(const short* buf, unsigned int uSamples)
{
	if(myRiffFile == INVALID_HANDLE_VALUE)
		return 1;

	//

	DWORD dwNumberOfBytesWritten;

	BOOL bRes = WriteFile(
		myRiffFile,
		buf,
		uSamples * sizeof(short) * myNumChannels,
		&dwNumberOfBytesWritten,
		NULL);

	myTotalNumberOfBytesWritten += dwNumberOfBytesWritten;

	return 0;
}


int RiffInitWriteFile(const char* pszFile, unsigned int sample_rate, unsigned int NumChannels)
{
	return g_RiffOutput.InitWriteFile(pszFile, sample_rate, NumChannels);
}

int RiffFinishWriteFile()
{
	return g_RiffOutput.FinishWriteFile();
}

int RiffPutSamples(const short* buf, unsigned int uSamples)
{
	return g_RiffOutput.PutSamples(buf, uSamples);
}
