#include "BufferX.h"

/***************************************************************
 *  Class BufferX
 ***************************************************************/
BufferX::BufferX()
{
	_DataPoolSize = 0;
	_DataPool = new(std::nothrow) char*[_BufferBlocks];
	for(size_t i=0; i<_BufferBlocks; i++)
		_DataPool[i] = new(std::nothrow) char[_BlockSize];

	Data = new(std::nothrow) char[_BlockSize];
}

BufferX::~BufferX()
{
	for(size_t i=0; i<_BufferBlocks; i++)
		delete[] _DataPool[i];
	delete[] _DataPool;
	delete[] Data;
}


bool
BufferX::_ReplaceLast(const size_t BlockIndex, _MapValueType& NewValue)
{
	if( _LRUList.size() >= _BufferBlocks ) {// LRU replacement
		size_t LastBlockIndex = _LRUList.back();
		_LRUList.pop_back();

		_MapType::iterator LastIt =	_LRUListMap.find(LastBlockIndex);
		if( LastIt != _LRUListMap.end() ){ // replace last block
			NewValue.second = LastIt->second.second; // copy _DataPool addr
			_LRUListMap.erase(LastIt);
		}
		else{
			_BF_DEBUG("Can't find block");
			return false;
		}
	}
	else{ // use new data pool
		NewValue.second = _DataPool[_DataPoolSize++];
	}

	_LRUList.push_front(BlockIndex);
	NewValue.first = _LRUList.begin();
	_LRUListMap[BlockIndex] = NewValue;
	return true;
}

bool
BufferX::SetFile(const char* FileName)
{
	if(_FileName != FileName){
		_FileName = FileName;
		_LRUList.clear();
		_LRUListMap.clear();
		_DataPoolSize = 0;
	}
	return true;
}


bool
BufferX::Read( const size_t BlockIndex, bool& eof)
{
	if(_FileName.empty()){
		_BF_DEBUG("File name is empty.");
		return false;
	}


	_MapType::iterator It =	_LRUListMap.find(BlockIndex);

	if( It == _LRUListMap.end() ){ // Not found in buffer
		FILE* fp = std::fopen(_FileName.c_str(), "rb");
		if(fp == 0){
			_BF_DEBUG("File does not exist");
			return false;
		}
		std::fseek(fp, _BlockSize*BlockIndex, SEEK_SET);
		size_t r = std::fread(Data, sizeof(char), _BlockSize, fp);
		eof = r != _BlockSize;
		std::fclose(fp);

		if(!eof){
			_MapValueType NewValue;
			_ReplaceLast(BlockIndex, NewValue);
			std::memcpy(NewValue.second, Data, _BlockSize);
		}
	}
	else{ // Found in buffer
		eof = false;
		std::memcpy(Data, It->second.second, _BlockSize);
	}

	return true;
}

bool
BufferX::Write(const size_t BlockIndex)
{
	if(_FileName.empty()){
		_BF_DEBUG("File name is empty.");
		return false;
	}

	_MapType::iterator It =	_LRUListMap.find(BlockIndex);

	if( It != _LRUListMap.end() ){  // Make rencent block to list head
		_LRUList.erase( It->second.first );
		_LRUList.push_front(BlockIndex);
		It->second.first = _LRUList.begin();
		std::memcpy(It->second.second, Data, _BlockSize);
	}
	else{ // add new block
		_MapValueType NewValue;
		_ReplaceLast(BlockIndex, NewValue);
		std::memcpy(NewValue.second, Data, _BlockSize);
	}

	FILE* fp = std::fopen(_FileName.c_str(), "rb+");
	if(fp == 0){
		_BF_DEBUG("Can't open file");
		return false;
	}

	std::fseek(fp, _BlockSize*BlockIndex, SEEK_SET);
	size_t r = std::fwrite(Data, sizeof(char), _BlockSize, fp);
	std::fclose(fp);
	return r == _BlockSize;
}

bool
BufferX::CreateFile()
{
	if(_FileName.empty()){
		_BF_DEBUG("File name is empty.");
		return false;
	}

	return CreateFile(_FileName.c_str());
}

bool
BufferX::DropFile()
{
	if(_FileName.empty()){
		_BF_DEBUG("File name is empty.");
		return false;
	}
	return DropFile(_FileName.c_str());
}

bool
BufferX::CreateFile(const char* FileName)
{
	if(FileName == 0){
		_BF_DEBUG("File name is empty.");
		return false;
	}


	FILE* fp = std::fopen(FileName, "wb");
	if(fp == 0){
		_BF_DEBUG("Can't open file");
		return false;
	}
	else{
		std::fclose(fp);
	}

	return true;
}

bool
BufferX::DropFile(const char* FileName)
{
	if(FileName == 0){
		_BF_DEBUG("File name is empty.");
		return false;
	}

	return std::remove(FileName);
}
