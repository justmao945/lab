#ifndef BUFFERX_H_
#define BUFFERX_H_

#include <new>
#include <map>
#include <list>
#include <cstring>
#include <string>
#include <cstdlib>
#include <cstdio>

#ifdef DEBUG
#define _BF_DEBUG(msg) \
	fprintf(stderr,__FILE__" :%d [%s] Error: "msg"\n", __LINE__,__FUNCTION__)
#else
#define _BF_DEBUG(msg) {}
#endif

static const size_t _BlockSize = 4096;
// sizeof a block, count with byte

static const size_t _BufferBlocks = 100;
// use 100 blocks to do a buffer

/***************************************************************
 *  Class BufferX
 *  Buffer is to store a copy of data. And before use
 *  non-static routines be sure have executed SetTable()
 ***************************************************************/

class BufferX{
	typedef std::map<size_t, std::pair<std::list<size_t>::iterator,char*> > _MapType;
	typedef std::pair<std::list<size_t>::iterator,char*> _MapValueType;

	std::string _FileName;

	std::list<size_t> _LRUList;
	_MapType _LRUListMap;
	// size_t is BlockIndex
	// void * ptr to _LRUList[i]
	// char* ptr to _DataPool[j]
	char **_DataPool;
	size_t _DataPoolSize;

	bool _ReplaceLast(const size_t BlockIndex, _MapValueType& Value);

public:
	char* Data;  // the place to store the data

	BufferX();
	~BufferX();

	bool SetFile(const char* FileName);
	// Be sure the first routine to run

	bool Read(const size_t BlockIndex, bool& eof);
	// read BlockIndex into Data, return eof

	bool Write(const size_t BlockIndex);
	// write Data into BlockIndex

	bool CreateFile();
	// create an new table

	bool DropFile();
	// Drop the table

	static bool CreateFile(const char* FileName);
	// static method to create an new table

	static bool DropFile(const char* FileName);
	// static method to drop the table
};

#endif
