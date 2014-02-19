#include <cstring>
#include <cstdlib>
#include <sstream>
#include "RecordManager.h"

static const char* _RMExt = ".rcd";

RMTable::RMTable()
{
	_Header.Clear();
}


RMTable::~RMTable()
{
}

_RMTableHeader
RMTable::GetHeader()
{
	return _Header;
}

size_t
RMTable::AttrNameToIndex(const char* AttrName)
{
  return _AttrNameToIndex(AttrName);
}

bool
RMTable::IsAttrUnique(const char* AttrName)
{
	size_t Ix = _AttrNameToIndex(AttrName);
	if(Ix == _RMAttrNa){
		_RM_DEBUG("Attribute not exist.");
		return false;
	}
	return _Header.AttrIsUniqueArray[Ix];
}

bool
RMTable::IsAttrPrimary(const char* AttrName)
{
	size_t Ix = _AttrNameToIndex(AttrName);
	if(Ix == _RMAttrNa){
		_RM_DEBUG("Attribute not exist.");
		return false;
	}
	return _Header.AttrIsPrimaryArray[Ix];
}

bool
RMTable::IsAttrExists(const char* AttrName)
{
	return _AttrNameToIndex(AttrName) != _RMAttrNa;
}

bool
RMTable::Create(
		const char* TableName,
		const size_t Attrs,
		const char** AttrNameArray,
		const size_t *AttrSizeArray,
		const bool *AttrIsPrimaryArray,
		const bool *AttrIsUniqueArray,
		const RMAttrType* AttrTypeArray)
{
	_TableName = TableName;
	_Header.Attrs = Attrs;

	for(size_t i=0; i<Attrs; i++)
		std::strncpy(_Header.AttrNameArray[i], AttrNameArray[i], _RMAttrNameMaxLength);

	_AttrNameIndexMap.clear();
	for(size_t i=0; i<_Header.Attrs; i++)
		_AttrNameIndexMap[_Header.AttrNameArray[i]] = i;

	std::memcpy(_Header.AttrSizeArray, AttrSizeArray, Attrs*sizeof(size_t));
	std::memcpy(_Header.AttrTypeArray, AttrTypeArray, Attrs*sizeof(size_t));

	_Header.AttrsSize = 0;
	for(size_t i=0; i<Attrs; i++)
		_Header.AttrsSize += AttrSizeArray[i];

	std::memcpy(_Header.AttrIsPrimaryArray, AttrIsPrimaryArray,Attrs*sizeof(bool));
	std::memcpy(_Header.AttrIsUniqueArray, AttrIsUniqueArray,Attrs*sizeof(bool));

	_Header.BlockTupleSize = _Header.AttrsSize + _RMBlockTuple::FlagSize();
	_Header.BlockTuples = (_BlockSize - _RMBlockHeader::Size()) / _Header.BlockTupleSize;
	_Header.FirstBlockPtr = _RMTableHeaderPtr + 1;
	_Header.FirstFreeBlockPtr = _Header.FirstBlockPtr;

	_Block.Clear();
	_Header.Store(_Buffer.Data);
	_Buffer.SetFile((_TableName + _RMExt).c_str());
	_Buffer.CreateFile();
	_Buffer.Write(_RMTableHeaderPtr);
	return true;
}

bool
RMTable::Load(const char* TableName)
{
	bool r, eof;
	_TableName = TableName;
	_Block.Clear();
	_BlockPtr = _RMTableHeaderPtr;

	_Buffer.SetFile( (_TableName + _RMExt).c_str() );
	r = _Buffer.Read(_RMTableHeaderPtr, eof);
	if(eof){
		_RM_DEBUG("EOF error");
		return false;
	}
	_Header.Load(_Buffer.Data);

	_AttrNameIndexMap.clear();
	for(size_t i=0; i<_Header.Attrs; i++)
		_AttrNameIndexMap[_Header.AttrNameArray[i]] = i;

	return r;
}

bool
RMTable::Drop()
{
	return _Buffer.DropFile();
}

bool
RMTable::Store()
{
	bool r;
	_Header.Store(_Buffer.Data);
	r = _Buffer.Write(_RMTableHeaderPtr);
	_Block.Store(_Buffer.Data);
	r = r && _Buffer.Write(_BlockPtr);
	return r;
}

bool
RMTable::InsertTupleBegin()
{
	return _FindGenFreeBlock(_Header.FirstFreeBlockPtr);
}


bool
RMTable::InsertTuple(
    const void **ItemPtrArray,
    size_t& BlockIndex)
{
	bool r ;
	if(_Block.GetHeader().InvalidTuples > 0){
		r = _Block.InsertTuple(ItemPtrArray);
		BlockIndex = _BlockPtr;
	}
	else{		// becuase _BlockPtr is full and must find free after _BlockPtr
		InsertTupleEnd(true);  // store current block
		_FindGenFreeBlock(_BlockPtr+1);
		_Header.FirstFreeBlockPtr = _BlockPtr;
		InsertTuple(ItemPtrArray, BlockIndex);
	}
	return r;
}


bool
RMTable::InsertTuple(
    const char **ItemStringArray,
    size_t& BlockIndex)
{
	return	InsertTuple(
			_ItemStringArrayToPtrArray(_Header.Attrs,0,ItemStringArray),
			BlockIndex);
}

bool
RMTable::InsertTuple(
    const std::vector<std::string>& ItemStringVector,
    size_t& BlockIndex)
{
	return InsertTuple(_ItemStringVectorToArray(ItemStringVector), BlockIndex);
}

bool
RMTable::InsertTupleEnd(bool IsDone)
{
	return IsDone && _Block.Store(_Buffer.Data) && _Buffer.Write(_BlockPtr);
}

bool
RMTable::DeleteTuples(
		const size_t Attrs,
		const size_t *AttrIndexArray,
		const RMAttrsRelation *AttrsRelationArray,
		const void **ItemPtrArray)
{
	bool r, eof;
	_BlockPtr = _Header.FirstBlockPtr;
	do{
		r = _Buffer.Read(_BlockPtr, eof);
		if(eof) // at the end of file
			break;
		else if(r){
			_Block.Load(_Buffer.Data, &_Header);
			if( _Block.GetHeader().InvalidTuples != _Header.BlockTuples ){
				_Block.Gen(true);
				_Block.DeleteTuples(
						Attrs,
						AttrIndexArray,
						AttrsRelationArray,
						ItemPtrArray);
				if(_Block.GetHeader().InvalidTuples != 0
						&& _Header.FirstFreeBlockPtr > _BlockPtr ){
					// update first free block
					_Header.FirstFreeBlockPtr = _BlockPtr;
				}
				_Block.Store(_Buffer.Data);
				_Buffer.Write(_BlockPtr);
			}
			_BlockPtr++;
		}
		else{
			_RM_DEBUG("Buffer read error");
			break;
		}
	}while(1);

	return true;
}


bool 
RMTable::DeleteTuples(
		const size_t Attrs,
		const char **AttrNameArray,
		const char **AttrsRelStringArray,
		const char **ItemStringArray)
{
	size_t *AttrIndexArray = _AttrNameToIndexArray(Attrs,AttrNameArray);
	return DeleteTuples(
					Attrs,
					AttrIndexArray,
					_AttrsRelStringArrayToRelationArray(Attrs, AttrsRelStringArray),
					_ItemStringArrayToPtrArray(Attrs,AttrIndexArray,ItemStringArray));
}

bool 
	RMTable::DeleteTuples(
		const size_t Attrs,
		const std::vector<std::string>& AttrNameVector,
		const std::vector<std::string>& AttrsRelStringVector,
		const std::vector<std::string>& ItemStringVector)
{
	return DeleteTuples(
					Attrs,
					_AttrNameVectorToArray(AttrNameVector),
					_AttrsRelStringVectorToArray(AttrsRelStringVector),
					_ItemStringVectorToArray(ItemStringVector));
}

bool
RMTable::DeleteBlockTuples(
		const size_t BlockIndex,
		const size_t Attrs,
		const size_t *AttrIndexArray,
		const RMAttrsRelation *AttrsRelationArray,
		const void **ItemPtrArray)
{
	bool eof;
	return _Buffer.Read(BlockIndex, eof) &&
			!eof &&
			_Block.LoadGen(_Buffer.Data, &_Header, true) &&
			_Block.DeleteTuples(
					Attrs,
					AttrIndexArray,
					AttrsRelationArray,
					ItemPtrArray) &&
			_Block.Store(_Buffer.Data) &&
			_Buffer.Write(_BlockPtr);
}

bool
RMTable::DeleteBlockTuples(
		const size_t BlockIndex,
		const size_t Attrs,
		const char **AttrNameArray,
		const char **AttrsRelStringArray,
		const char **ItemStringArray)
{
	size_t *AttrIndexArray = _AttrNameToIndexArray(Attrs,AttrNameArray);
	return DeleteTuples(
					Attrs,
					AttrIndexArray,
					_AttrsRelStringArrayToRelationArray(Attrs, AttrsRelStringArray),
					_ItemStringArrayToPtrArray(Attrs,AttrIndexArray,ItemStringArray));
}


bool
RMTable::DeleteBlockTuples(
		const size_t BlockIndex,
		const size_t Attrs,
		const std::vector<std::string>& AttrNameVector,
		const std::vector<std::string>& AttrsRelStringVector,
		const std::vector<std::string>& ItemStringVector)
{
	return DeleteTuples(
					Attrs,
					_AttrNameVectorToArray(AttrNameVector),
					_AttrsRelStringVectorToArray(AttrsRelStringVector),
					_ItemStringVectorToArray(ItemStringVector));
}



bool
RMTable::QueryBegin()
{
	_BlockPtr = _Header.FirstBlockPtr;
	return true;
}

bool
RMTable::QueryEnd(const bool IsDone)
{
	_BlockPtr = _Header.FirstBlockPtr;
	return IsDone;
}

bool
RMTable::QueryAllTuples(
    bool& eof,
    std::vector<const char*>& Tuples)
{
	bool r;
	Tuples.clear();
	do{
		r = _Buffer.Read(_BlockPtr, eof);
		if(eof){  // if is EOF then break
			break;
		}
		else if(r){ // check if the block is empty
			_Block.Load(_Buffer.Data, &_Header);
			_BlockPtr++;
			if(_Block.GetHeader().InvalidTuples != _Header.BlockTuples){
				_Block.Gen(true);
				_Block.QueryAllTuples(Tuples);
				break;
			}
		}
		else{
			_RM_DEBUG("Buffer read error");
			break;
		}
	}while(1);

	return true;
}

bool 
RMTable::QueryAllTuples(
    bool& eof,
    std::vector< std::vector<std::string> >& Tuples)
{
	std::vector<const char*> TempTuples;
	Tuples.clear();
	return
		QueryAllTuples(eof, TempTuples) && !eof &&
		_QueryToReadable(TempTuples, Tuples);
}

bool
RMTable::QueryTuples(
		const size_t Attrs,
		const size_t *AttrIndexArray,
		const RMAttrsRelation *AttrsRelationArray,
		const void **ItemPtrArray,
		bool& eof,
		std::vector<const char*>& Tuples )
{
	bool r;
	Tuples.clear();
	do{
		r = _Buffer.Read(_BlockPtr, eof);
		if(eof){  // if is EOF then break
			break;
		}
		else if(r){ // check if the block is empty
			_Block.Load(_Buffer.Data, &_Header);
			_BlockPtr++;
			if(_Block.GetHeader().InvalidTuples != _Header.BlockTuples){
				_Block.Gen(true);
				_Block.QueryTuples(
							Attrs,
							AttrIndexArray,
							AttrsRelationArray,
							ItemPtrArray,
							Tuples);
				break;
			}
		}
		else{
			_RM_DEBUG("Buffer read error");
			break;
		}
	}while(1);

	return true;
}



bool 
RMTable::QueryTuples(
		const size_t Attrs,
		const char **AttrNameArray,
		const char **AttrsRelStringArray,
		const char **ItemStringArray,
		bool& eof,
		std::vector< std::vector<std::string> >& Tuples)
{
	std::vector<const char*> Temp;
	Tuples.clear();
	size_t *AttrIndexArray = _AttrNameToIndexArray(Attrs,AttrNameArray);
	return QueryTuples(
						Attrs,
						AttrIndexArray,
						_AttrsRelStringArrayToRelationArray(Attrs, AttrsRelStringArray),
						_ItemStringArrayToPtrArray(Attrs,AttrIndexArray,ItemStringArray),
						eof,
						Temp) && !eof &&
				_QueryToReadable(Temp, Tuples);
}


bool 
RMTable::QueryTuples(
		const size_t Attrs,
		const std::vector<std::string>& AttrNameVector,
		const std::vector<std::string>& AttrsRelStringVector,
		const std::vector<std::string>& ItemStringVector,
		bool& eof,
		std::vector< std::vector<std::string> >& Tuples)
{
	return QueryTuples(
						Attrs,
						_AttrNameVectorToArray(AttrNameVector),
						_AttrsRelStringVectorToArray(AttrsRelStringVector),
						_ItemStringVectorToArray(ItemStringVector),
						eof,
						Tuples);
}

bool
RMTable::QueryBlockTuples(
		const size_t BlockIndex,
		const size_t Attrs,
		const size_t *AttrIndexArray,
		const RMAttrsRelation *AttrsRelationArray,
		const void **ItemPtrArray,
		std::vector<const char*>& Tuples )
{
	bool eof;
	return
			_Buffer.Read(BlockIndex, eof) &&
			!eof &&
			_Block.LoadGen(_Buffer.Data, &_Header, true) &&
			_Block.QueryTuples(
					Attrs,
					AttrIndexArray,
					AttrsRelationArray,
					ItemPtrArray,
					Tuples);
}


bool 
RMTable::QueryBlockTuples(
	const size_t BlockIndex,
	const size_t Attrs,
	const char **AttrNameArray,
	const char **AttrsRelStringArray,
	const char **ItemStringArray,
	std::vector< std::vector<std::string> >& Tuples )
{
	std::vector<const char*> Temp;
	size_t *AttrIndexArray = _AttrNameToIndexArray(Attrs,AttrNameArray);
	return QueryBlockTuples(
						BlockIndex,
						Attrs,
						AttrIndexArray,
						_AttrsRelStringArrayToRelationArray(Attrs, AttrsRelStringArray),
						_ItemStringArrayToPtrArray(Attrs,AttrIndexArray,ItemStringArray),
						Temp) &&
				_QueryToReadable(Temp, Tuples);
}


bool 
RMTable::QueryBlockTuples(
	const size_t BlockIndex,
	const size_t Attrs,
	const std::vector<std::string>& AttrNameVector,
	const std::vector<std::string>& AttrsRelStringVector,
	const std::vector<std::string>& ItemStringVector,
	std::vector< std::vector<std::string> >& Tuples )
{
	return QueryBlockTuples(
						BlockIndex,
						Attrs,
						_AttrNameVectorToArray(AttrNameVector),
						_AttrsRelStringVectorToArray(AttrsRelStringVector),
						_ItemStringVectorToArray(ItemStringVector),
						Tuples);
}

/////////////////////////////////////////////////////////////////////////////
/////////   private routines
////////////////////////////////////////////////////////////////////////////

bool
RMTable::_FindGenFreeBlock(const size_t StartBlockPtr)
{
	bool eof, r;
	_BlockPtr = StartBlockPtr;
	do{
		r = _Buffer.Read(_BlockPtr, eof);
		if(eof){  // if is EOF then create an new block
			_Block.CreateGen(&_Header, false); // gen invalid list
			break;
		}
		else if(r){ // check if the block is full
			_Block.Load(_Buffer.Data, &_Header);
			if(_Block.GetHeader().InvalidTuples > 0){
				_Block.Gen(false); // gen invalid list
				break;
			}
			else{
				_BlockPtr++;
			}
		}
		else{
			_RM_DEBUG("Buffer read error");
			break;
		}
	}while(1);
	return r;
}


size_t
RMTable::_AttrNameToIndex(const char* AttrName)
{
	std::map<std::string, size_t>::const_iterator cit
		= _AttrNameIndexMap.find(AttrName);
	if(cit != _AttrNameIndexMap.end())
		return cit->second;
	else
		return _RMAttrNa;
}

size_t*
RMTable::_AttrNameToIndexArray(
		const size_t Attrs,
		const char** AttrNameArray)
{
	for(size_t i=0; i<Attrs; i++){
		size_t ix = _AttrNameToIndex(AttrNameArray[i]);
		if(ix == _RMAttrNa){
			_RM_DEBUG("Attribute not eixsts.");
			break;
		}
		_AttrIndexArray[i] = ix;
	}
	return _AttrIndexArray;
}

const char**
RMTable::_AttrNameVectorToArray(
		const std::vector<std::string>& AttrNameVector)
{
	for(size_t i=0; i<AttrNameVector.size(); i++){
		_AttrNameArray[i] = AttrNameVector[i].c_str();
	}
	return _AttrNameArray;
}

const void**
RMTable::_ItemStringArrayToPtrArray(
		const size_t Attrs,
		const size_t *AttrIndexArray,
		const char **ItemStringArray)
{
	size_t IntArraySize = 0, FloatArraySize = 0, ItemPtrArraySize = 0;
	size_t AttrsX = Attrs;
	size_t TypeIndex;

	if( AttrIndexArray == 0 ) // the case insert operation
		AttrsX = _Header.Attrs;

	for(size_t i=0; i<AttrsX; i++){
		if (AttrIndexArray == 0) // the case insert operation
			TypeIndex = i;
		else
			TypeIndex = AttrIndexArray[i];

		switch(_Header.AttrTypeArray[TypeIndex]){
			case RMIntType:
				_IntArray[IntArraySize] = std::atoi(ItemStringArray[i]);
				_ItemPtrArray[ItemPtrArraySize++] = _IntArray + IntArraySize;
				IntArraySize ++;
				break;
			case RMFloatType:
				_FloatArray[FloatArraySize] = static_cast<float>(std::atof(ItemStringArray[i]));
				_ItemPtrArray[ItemPtrArraySize++] = _FloatArray + FloatArraySize;
				FloatArraySize ++;
				break;
			case RMStringType:
				_ItemPtrArray[ItemPtrArraySize++] = ItemStringArray[i];
				break;
			default:
				_RM_DEBUG("Item type Error");
				return false;
		}
	}
	return _ItemPtrArray;
}

const char**
RMTable::_ItemStringVectorToArray(
		const std::vector<std::string>& ItemStringVector)
{
	for(size_t i=0; i<ItemStringVector.size(); i++)
		_ItemStringArray[i] = ItemStringVector[i].c_str();
	return _ItemStringArray;
}

bool 
RMTable::_QueryToReadable(
	const std::vector<const char*>& SrcTuples,
	std::vector< std::vector<std::string> >& DestTuples)
{
	std::stringstream ss;
	std::vector<std::string> Tuple;

	DestTuples.reserve(_Header.BlockTuples);
	Tuple.reserve(_Header.Attrs);

	DestTuples.clear();
	for(size_t i=0; i<SrcTuples.size(); i++){
		const char* off = SrcTuples[i];
		Tuple.clear();
		for(size_t j=0; j<_Header.Attrs; j++){
			ss.clear();
			ss.str("");
			switch(_Header.AttrTypeArray[j]){
				case RMIntType:
					ss << *(const int*)(off);
					break;
				case RMFloatType:
					ss << *(const float*)(off);
					break;
				case RMStringType:
					ss << off;
					break;
				default:
					_RM_DEBUG("Type Error");
					break;
			}
			off += _Header.AttrSizeArray[j];
			ss.flush();
			Tuple.push_back(ss.str());
		}
		DestTuples.push_back(Tuple);
	}
	return true;
}



const RMAttrsRelation *
RMTable::_AttrsRelStringArrayToRelationArray(
		const size_t Attrs,
		const char**AttrsRelStringArray)
{
	size_t RelationArraySize = 0;
	for(size_t i=0; i<Attrs; i++){
		switch(std::strlen(AttrsRelStringArray[i])){
			case 1:  // > or < or =
			{
				switch(AttrsRelStringArray[i][0]){
					case '>':
						_RelationArray[RelationArraySize] = RMRelGreater;
						break;
					case '<':
						_RelationArray[RelationArraySize] = RMRelLess;
						break;
					case '=':
						_RelationArray[RelationArraySize] = RMRelEqual;
						break;
					default:
						_RM_DEBUG("convert error");
						break;
				}
				break;
			}
			case 2: // == or != or >= or <= or <>
			{
				if(AttrsRelStringArray[i][1] == '='){
					switch(AttrsRelStringArray[i][0]){
						case '>':
							_RelationArray[RelationArraySize] = RMRelGreaterOrEqual;
							break;
						case '<':
							_RelationArray[RelationArraySize] = RMRelLessOrEqual;
							break;
						case '=':
							_RelationArray[RelationArraySize] = RMRelEqual;
							break;
						case '!':
							_RelationArray[RelationArraySize] = RMRelNotEqual;
							break;
						default:
							_RM_DEBUG("convert error");
							break;
					}
				}
				else if(AttrsRelStringArray[i][1] == '>' && AttrsRelStringArray[i][0] == '<')
					_RelationArray[RelationArraySize] = RMRelNotEqual;
				else
					_RM_DEBUG("convert error");
				break;
			}
			default:
				_RM_DEBUG("convert error");
				break;
		}
		RelationArraySize++;
	}
	return _RelationArray;
}


const char** 
RMTable::_AttrsRelStringVectorToArray(
		const std::vector<std::string>& AttrsRelStringVector)
{
	for(size_t i=0; i<AttrsRelStringVector.size(); i++){
		_RelStringArray[i] = AttrsRelStringVector[i].c_str();
	}
	return _RelStringArray;
}
