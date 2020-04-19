#ifndef _DETZFILE_H_
#define _DETZFILE_H_

#include "../detCase.h"

class decZFileReader;
class decMemoryFileReader;
class decMemoryFileWriter;
class decMemoryFile;
class decZFileWriter;

// class detZFile
class detZFile : public detCase{
private:
	decMemoryFile *pMemoryFileCompressed;
	char *pTestBuffer;
	
	decMemoryFileWriter *pMemoryFileWriter;
	decZFileWriter *pZWriter;
	
	decMemoryFileReader *pMemoryFileReader;
	decZFileReader *pZReader;
	
public:
	detZFile();
	~detZFile();
	void Prepare();
	void Run();
	void CleanUp();
	const char *GetTestName();
	
private:
	void pTestZFile();
	
	void pOutputCompressedToFile();
	void pOutputTestBufferToFile( int size );
	
	void pCreateZWriter();
	void pDestroyZWriter();
	void pCreateZReader();
	void pDestroyZReader();
};

#endif

