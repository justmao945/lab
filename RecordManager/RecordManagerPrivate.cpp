/*
 * RecordManagerPrivate.cpp
 *
 *  Created on: Oct 13, 2011
 *      Author: just
 */

#include <new>
#include <string>
#include <cstring>
#include "RecordManagerPrivate.h"


/***************************************************************
 *  Class _RMTableHeader
 ***************************************************************/
size_t
_RMTableHeader::Size()
{
	return sizeof(_RMTableHeader);
}

bool
_RMTableHeader::Clear()
{
	std::memset(this, 0, Size());
	return true;
}

bool
_RMTableHeader::Load(const char *Src)
{
	std::memcpy(this, Src, Size());
	return true;
}

bool
_RMTableHeader::Store(char *Dest)
{
	std::memcpy(Dest, this, Size());
	return true;
}



/***************************************************************
 *  Class _RMBlockHeader
 ***************************************************************/
size_t
_RMBlockHeader::Size()
{
	return sizeof(_RMBlockHeader);
}

bool
_RMBlockHeader::Clear()
{
	std::memset(this, 0, Size());
	return true;
}

bool
_RMBlockHeader::Load(const char *Src)
{
	std::memcpy(this, Src, Size());
	return true;
}

bool
_RMBlockHeader::Store(char *Dest)
{
	std::memcpy(Dest, this, Size());
	return true;
}



/***************************************************************
 *  Class _RMBlockTuple
 ***************************************************************/
_RMBlockTuple::_RMBlockTuple(const _RMTableHeader* TableHeaderPtr)
{
	Clear();
	_TableHeaderPtr = TableHeaderPtr;
	_Items = 0;
	if(_TableHeaderPtr)
		_Items = new(std::nothrow) char[_TableHeaderPtr->AttrsSize];
}

/*
_RMBlockTuple::_RMBlockTuple(_RMBlockTuple& Tuple)
{
	_Items = 0;
	if(Tuple._TableHeaderPtr){
		_Items = new(std::nothrow) char[Tuple._TableHeaderPtr->AttrsSize];
		_CopyFrom(Tuple);
	}
}
*/

_RMBlockTuple::_RMBlockTuple(const _RMBlockTuple& Tuple)
{
	_Items = 0;
	if(Tuple._TableHeaderPtr){
		_Items = new(std::nothrow) char[Tuple._TableHeaderPtr->AttrsSize];
		_CopyFrom(Tuple);
	}
}

_RMBlockTuple::~_RMBlockTuple()
{
	if(_Items)
		delete[] _Items;
}

void
_RMBlockTuple::_CopyFrom(const _RMBlockTuple& Tuple)
{
	char* OriItems = _Items;
	std::memcpy(this, &Tuple, sizeof(_RMBlockTuple));
	_Items = OriItems;
	if(_TableHeaderPtr){
		if(_Items == 0){
			_RM_DEBUG("_Items is 0");
		}
		else{
			std::memcpy(_Items, Tuple._Items, Tuple._TableHeaderPtr->AttrsSize);
		}
	}
}


bool
_RMBlockTuple::Create()
{
	if(_TableHeaderPtr){
		Clear();
		std::memset(_Items, 0, _TableHeaderPtr->AttrsSize);
		return true;
	}
	return false;
}

bool
_RMBlockTuple::Load(const char* Src)
{
	if(Src == 0){
		_RM_DEBUG("RawSrc is 0");
		return false;
	}

	if(_TableHeaderPtr == 0){
		_RM_DEBUG("_TableHeaderPtr is 0");
		return false;
	}
	Clear();
	std::memcpy(_Items, Src, _TableHeaderPtr->AttrsSize);
	std::memcpy(&_Flag, Src + _TableHeaderPtr->AttrsSize, sizeof(char));
	return true;
}

bool
_RMBlockTuple::Store(char* Dest)
{
	if(Dest == 0){
		_RM_DEBUG("RawDest is 0");
		return false;
	}

	if(_TableHeaderPtr == 0){
		_RM_DEBUG("_TableHeaderPtr is 0");
		return false;
	}

	std::memcpy(Dest, _Items, _TableHeaderPtr->AttrsSize);
	std::memcpy(Dest + _TableHeaderPtr->AttrsSize, &_Flag, sizeof(char));
	_IsCommited = true;
	return true;
}

size_t
_RMBlockTuple::Size()
{
	if(_TableHeaderPtr == 0){
		_RM_DEBUG("_TableHeaderPtr is 0");
		return 0;
	}
	return _TableHeaderPtr->AttrsSize + FlagSize();
}

size_t
_RMBlockTuple::FlagSize()
{
	return sizeof(char);
}

bool
_RMBlockTuple::IsValid()
{
	return _Flag;
}

bool
_RMBlockTuple::SetValid(const bool Valid)
{
	_IsCommited = false;
	_Flag = Valid;
	return true;
}

bool
_RMBlockTuple::IsCommited()
{
	return _IsCommited;
}

void
_RMBlockTuple::Clear()
{
	_Flag = 0;
	_NextPtr = _PrevPtr = 0;
	_IsCommited = true;
}

_RMBlockTuple&
_RMBlockTuple::operator= (const _RMBlockTuple& Tuple)
{
	_CopyFrom(Tuple);
	return *this;
}

const _RMBlockTuple&
_RMBlockTuple::ListInsert(_RMBlockTuple& Head)
{
	_RMBlockTuple* tmp = Head._NextPtr;
	Head._NextPtr = this;
	if(_NextPtr)
		_NextPtr->_PrevPtr = this;
	_PrevPtr = &Head;
	_NextPtr = tmp;
	return *this;
}

void
_RMBlockTuple::ListDelete()
{
	_RMBlockTuple* tmp = _NextPtr;
	if(_NextPtr){
		_NextPtr->_PrevPtr = _PrevPtr;
		_NextPtr = 0;
	}
	if(_PrevPtr){
		_PrevPtr->_NextPtr = tmp;
		_PrevPtr = 0;
	}
}

_RMBlockTuple *
_RMBlockTuple::GetNextPtr()
{
	return _NextPtr;
}

_RMBlockTuple *
_RMBlockTuple::GetPrevPtr()
{
	return _PrevPtr;
}

bool
_RMBlockTuple::_InsertItemsBegin()
{
	if(_TableHeaderPtr == 0){
		_RM_DEBUG("_TableHeaderPtr is 0");
		return false;
	}

	if(IsValid())	{
		_RM_DEBUG("Tuple is valid");
		return false;
	}
	return true;
}

bool
_RMBlockTuple::_InsertItemsEnd(bool IsDone)
{
	if(IsDone)
		_IsCommited = false;
	return IsDone;
}

bool
_RMBlockTuple::InsertItemsFromRaw(const char* RawItems)
{
	bool r = _InsertItemsBegin();
	std::memcpy(_Items, RawItems, _TableHeaderPtr->AttrsSize);
	return _InsertItemsEnd(r);
}

bool
_RMBlockTuple::InsertItems(const void** ItemPtrArray)
{

	bool r = _InsertItemsBegin();

	if(ItemPtrArray == 0)	{
		_RM_DEBUG("ItemPtrArray is 0");
		return false;
	}

	char* ItemOffset = _Items;
	for(size_t i=0; i<_TableHeaderPtr->Attrs; i++){
		std::memcpy(ItemOffset, ItemPtrArray[i], _TableHeaderPtr->AttrSizeArray[i]);
		ItemOffset += _TableHeaderPtr->AttrSizeArray[i];
	}
	return _InsertItemsEnd(r);
}


bool
_RMBlockTuple::Compare(
		const size_t Index,
		const void* Item,
		const RMAttrsRelation Rel)
{
	if(_TableHeaderPtr == 0){
		_RM_DEBUG("_TableHeaderPtr is null");
		return 0;
	}

	size_t Size = _TableHeaderPtr->AttrSizeArray[Index];
	size_t Offset = 0;
	for(size_t i=0; i<Index; i++)
		Offset += _TableHeaderPtr->AttrSizeArray[i];

	int res;
	switch(_TableHeaderPtr->AttrTypeArray[Index]){
		case RMIntType:
			res = *(int*)(_Items+Offset) - *(int*)Item;
			break;
		case RMFloatType:
		{
			float diff = *(float*)(_Items+Offset) - *(float*)Item;
			res = diff > 0 ? 1 : diff < 0 ? -1 : 0;
			break;
		}
		case RMStringType:
			res = std::strncmp(_Items+Offset, (char*)Item, Size);
			break;
		default:
			_RM_DEBUG("Compare type error");
			res = 0;
			break;
	}

	switch(Rel){
		case RMRelNotEqual:
			return res != 0;
		case RMRelEqual:
			return res == 0;
		case RMRelGreater:
			return res > 0;
		case RMRelLess:
			return res < 0;
		case RMRelGreaterOrEqual:
			return res >= 0;
		case RMRelLessOrEqual:
			return res <= 0;
		default:
			_RM_DEBUG("Relation Error");
			return false;
	}
}

const char*
_RMBlockTuple::GetItems()
{
	return _Items;
}

/***************************************************************
 *  Class _RMBlock
 ***************************************************************/
_RMBlock::_RMBlock()
{
	_Raw = new(std::nothrow) char[_BlockSize];
	_Vect.reserve(_BlockSize / sizeof(bool));
	Clear();
}

_RMBlock::~_RMBlock()
{
	delete[] _Raw;
}

bool
_RMBlock::Create(const _RMTableHeader* TableHeaderPtr)
{
	if(TableHeaderPtr == 0){
		_RM_DEBUG("TableHeaderPtr is 0");
		return false;
	}
	Clear();

	_TableHeaderPtr = TableHeaderPtr;
	_Header.InvalidTuples = TableHeaderPtr->BlockTuples;
	return true;
}

bool
_RMBlock::CreateGen(
		const _RMTableHeader* TableHeaderPtr,
		const bool Valid)
{
	return Create(TableHeaderPtr) && Gen(Valid);
}

bool
_RMBlock::Load(
		const char* Src,
		const _RMTableHeader* TableHeaderPtr)
{
	if(TableHeaderPtr == 0)	{
		_RM_DEBUG("TableHeaderPtr is 0");
		return false;
	}

	Clear();
	_TableHeaderPtr = TableHeaderPtr;
	std::memcpy(_Raw, Src, _BlockSize);
	_Header.Load(_Raw);
	return true;
}

bool
_RMBlock::LoadGen(
		const char* Src,
		const _RMTableHeader* TableHeaderPtr,
		const bool Valid)
{
	return Load(Src, TableHeaderPtr) && Gen(Valid);
}

bool
_RMBlock::Store(char *Dest)
{
	char* RawOffset = _Raw;
	bool r = true;

	_Header.Store(RawOffset);
	RawOffset += _Header.Size();

	for(size_t i=0; i<_Vect.size(); i++){
		if( !_Vect[i].IsCommited() )
			r = r && _Vect[i].Store(RawOffset);
		RawOffset += _Vect[i].Size();
	}

	std::memcpy(Dest, _Raw, _BlockSize);
	return r;
}

const _RMBlockHeader&
_RMBlock::GetHeader()
{
	return _Header;
}

void
_RMBlock::Clear()
{
	_TableHeaderPtr = 0;
	_Header.Clear();
	_ListHead.Clear();
	_Vect.clear();
	memset(_Raw, 0, _BlockSize * sizeof(char));
}

bool
_RMBlock::Gen(const bool Valid)
{
	if(_TableHeaderPtr == 0){
		_RM_DEBUG("TableHeaderPtr is 0");
		return false;
	}

	_RMBlockTuple Tuple(_TableHeaderPtr);
	const char* RawOffset = _Raw + _Header.Size();
	bool r = true;

	_Vect.clear();
	if(Valid){
		for(size_t i=0; i<_TableHeaderPtr->BlockTuples; i++){
			r = r && Tuple.Load(RawOffset);
			_Vect.push_back(Tuple);
			if( Tuple.IsValid() )
				_Vect[i].ListInsert(_ListHead);
			RawOffset += Tuple.Size();
		}
	}
	else{
		for(size_t i=0; i<_TableHeaderPtr->BlockTuples; i++){
			r = r && Tuple.Load(RawOffset);
			_Vect.push_back(Tuple);
			if( ! Tuple.IsValid() )
				_Vect[i].ListInsert(_ListHead);
			RawOffset += Tuple.Size();
		}
	}
	return _Vect.size() == _TableHeaderPtr->BlockTuples && r;
}

bool
_RMBlock::_InsertTupleBegin()
{
	if(_Vect.empty()){
		_RM_DEBUG("_Vect is empty");
		return false;
	}

	if(_Header.InvalidTuples == 0) // Block is full, but no error
		return false;

	if(_ListHead.GetNextPtr() == 0){
		_RM_DEBUG("_ListHead.GetNextPtr() is 0");
		return false;
	}

	return true;
}

bool
_RMBlock::_InsertTupleEnd(const bool IsDone)
{
	if(IsDone){
		_ListHead.GetNextPtr()->SetValid(true);
		_ListHead.GetNextPtr()->ListDelete();  // Delete from invalid list
		_Header.InvalidTuples--;
	}
	return IsDone;
}


bool
_RMBlock::InsertTuple(const void** ItemPtrArray)
{
	_RMBlockTuple *Ptr = _ListHead.GetNextPtr();
	return _InsertTupleEnd(
			_InsertTupleBegin() &&
			Ptr->InsertItems(ItemPtrArray) );
}

bool
_RMBlock::DeleteTuples(
		const size_t Attrs,
		const size_t *AttrIndexArray,
		const RMAttrsRelation *AttrsRelationArray,
		const void **ItemPtrArray)
{
	for(_RMBlockTuple* Ptr=_ListHead.GetNextPtr();
			Ptr!=0; Ptr=Ptr->GetNextPtr()){
		size_t i;
		for(i=0; i<Attrs; i++){
			if( ! Ptr->Compare(AttrIndexArray[i], ItemPtrArray[i], AttrsRelationArray[i]))
				break;
		}
		if(i == Attrs){
			// all Items are same, but no need to delete the tuple from valid list
			Ptr->SetValid(false);
			_Header.InvalidTuples++;
		}
	}
	return true;
}

bool
_RMBlock::QueryAllTuples(std::vector<const char*>& Tuples)
{
	Tuples.clear();
	for(_RMBlockTuple *ptr=_ListHead.GetNextPtr();
			ptr != 0; ptr=ptr->GetNextPtr()){
		Tuples.push_back(ptr->GetItems());
	}
	return true;
}

bool
_RMBlock::QueryTuples(
		const size_t Attrs,
		const size_t *AttrIndexArray,
		const RMAttrsRelation *AttrsRelationArray,
		const void **ItemPtrArray,
		std::vector<const char*>& Tuples )
{
	Tuples.clear();
	for(_RMBlockTuple *ptr=_ListHead.GetNextPtr();
			ptr != 0; ptr=ptr->GetNextPtr()){
		size_t i;
		for(i=0; i<Attrs; i++){
			if( ! ptr->Compare(AttrIndexArray[i], ItemPtrArray[i], AttrsRelationArray[i]))
				break;
		}
		if(i == Attrs){
			// all Items are same, so delete the tuple
			Tuples.push_back(ptr->GetItems());
		}
	}
	return true;
}



