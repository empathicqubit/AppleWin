#pragma once

class RiffOutput
{
public:
	RiffOutput();
	~RiffOutput();

	int InitWriteFile(const char* pszFile, unsigned int sample_rate, unsigned int NumChannels);
	int FinishWriteFile();
	int PutSamples(const short* buf, unsigned int uSamples);

private:
	HANDLE myRiffFile;
	DWORD myTotalOffset;
	DWORD myDataOffset;
	DWORD myTotalNumberOfSampleBytesWritten;
	unsigned int myNumChannels;
};

// these operate in the "global" RiffOutput object
int RiffInitWriteFile(const char* pszFile, unsigned int sample_rate, unsigned int NumChannels);
int RiffFinishWriteFile();
int RiffPutSamples(const short* buf, unsigned int uSamples);
