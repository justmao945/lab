/*
 * RecordManagerPrivate.h
 *
 *  Created on: Oct 13, 2011
 *      Author: just
 */

#ifndef RECORDMANAGERPRIVATE_H_
#define RECORDMANAGERPRIVATE_H_

#include <vector>
#include <cstdio>
#include <string>
#include "BufferX.h"

#ifdef DEBUG
#define _RM_DEBUG(msg) \
	fprintf(stderr,__FILE__" :%d [%s] Error: "msg"\n", __LINE__,__FUNCTION__)
#else
#define _RM_DEBUG(msg) {}
#endif

static const size_t _RMMaxAttrs = 32;
// max limit of attributes of a table(or tuple)

static const size_t _RMAttrNa = _RMMaxAttrs + 1;

static const size_t _RMAttrNameMaxLength = 80;
// Max length of attr name

static const size_t _RMTableNameMaxLength = 80;
// Max length of table name

static const size_t _RMTableHeaderPtr = 0;
// default table header position

static const size_t _RMBufferBlocks = 100;
// use 100 blocks to do a buffer

enum RMAttrType{
	RMIntType,  		// int type
	RMFloatType, 		// float type
	RMStringType,		// c format string, end with '\0'
};

enum RMAttrsRelation{
	RMRelNotEqual,
	RMRelEqual,
	RMRelGreater,
	RMRelLess,
	RMRelGreaterOrEqual,
	RMRelLessOrEqual,
};


/***************************************************************
 *  Class _RMTableHeader
 *  TableHeader is to stroe important information of the table.
 *  And it would be stored in the first block in the file.
 ***************************************************************/
struct _RMTableHeader{
	char TableName[_RMTableNameMaxLength];
	size_t Attrs;					// Attributes quantity of table
	size_t AttrsSize;			// All attributes size of a table

	char AttrNameArray[_RMMaxAttrs][_RMAttrNameMaxLength];
	// All attribute name array

	size_t AttrSizeArray[_RMMaxAttrs];
	// An array to store the size of every attribute.
	// For String type, include the size of '\0'.

	RMAttrType AttrTypeArray[_RMMaxAttrs];
	// An array to store the type of every attribute.

	bool AttrIsPrimaryArray[_RMMaxAttrs];
	// record the attr is primary or not

	bool AttrIsUniqueArray[_RMMaxAttrs];
	// record the attr is unique or not

	size_t BlockTuples;		// Tuples quantity of every block
	size_t BlockTupleSize;	// Size of a tuple(include items and flag)

	size_t FirstBlockPtr;  // normally is 1
	size_t FirstFreeBlockPtr; // point to first free block

	static size_t Size();
	// sizeof table header

	bool Clear();
	// set all data to zero

	bool Load(const char* Src);
	// load Src into the table header

	bool Store(char *Dest);
	// store table header into Dest
};



/***************************************************************
 *  Class _RMBlockHeader
 *  BlockHeader is to store some information of a block
 ***************************************************************/
struct _RMBlockHeader{
	size_t InvalidTuples;  // invalid tuples count

	static size_t Size();
	// the size of BlockHeader

	bool Clear();
	// set all data into zero

	bool Load(const char *Src);
	// load Src into block header

	bool Store(char *Dest);
	// store block header into Dest
};



/***************************************************************
 *  Class _RMBlockTuple
 *  BlockTuple is to store the tuple and provide many routines
 *  to do some opeartion. Be sure have an non-zero TableHeaderPtr.
 ***************************************************************/
class _RMBlockTuple{
	char* _Items;			// Items data of Attributes, will be allocated
	char  _Flag;			// Useful flag

	_RMBlockTuple *_NextPtr;
	_RMBlockTuple *_PrevPtr;
	// [In]valid pointers, used in two double linked list

	const _RMTableHeader *_TableHeaderPtr;  // ptr to table header
	bool _IsCommited;
	// the flag to record whether the tuple is commited or not

	void _CopyFrom(const _RMBlockTuple& Tuple);
	// copy another block tuple

	bool _InsertItemsBegin();
	// Do some checks before insertion

	bool _InsertItemsEnd(const bool IsDone);
	// set commited after insertion


public:
	_RMBlockTuple(const _RMTableHeader* TableHeaderPtr=0);
	//_RMBlockTuple(_RMBlockTuple& Tuple);
	_RMBlockTuple(const _RMBlockTuple& Tuple);
	~_RMBlockTuple();

	bool Create();
	// create an new block tuple, filled with zero

	bool Load(const char* Src);
	// load Src into _Items and _Flag

	bool Store(char* Dest);
	// store _Items and _Flag into Dest

	size_t Size();
	// sizeof _Items and _Flag

	static size_t FlagSize();
	// sizeof _Flag, also sizeof(char)

	const _RMBlockTuple& ListInsert(_RMBlockTuple& Prev);
	// insert the tuple into an [in]valid double linked list.
	// PrevInvalid is usually the header of the list

	void ListDelete();
	// delete the tuple from the [in]valid double linked list

	bool IsValid();
	// test the tuple is valid or not

	bool SetValid(const bool Valid);
	// set flag to true or false

	bool IsCommited();
	// test the tuple is commited, if commited will rewrite into
	// the _Items and _Flag

	void Clear();
	// set all data into zero

	_RMBlockTuple& operator= (const _RMBlockTuple& Tuple);
	// same as _CopyFrom()

	bool Compare(
			const size_t Index,
			const void* Item,
			const RMAttrsRelation Rel);
	// compare *Item with the Index(th) item of _Items return
	// ture if satify the relation

	_RMBlockTuple *GetNextPtr();
	_RMBlockTuple *GetPrevPtr();
	// get the ptr

	bool InsertItemsFromRaw(const char* RawItems);
	// copy RawItems into _Items

	bool InsertItems(const void **ItemPtrArray);
	// insert ItemPtrArray into _Items.
	// sizeof ItemPtrArray must be equal to _TableHeaderPtr->Attrs

	const char* GetItems();
	// get _Items
};



/***************************************************************
 *  Class _RMBlock
 *  Block is to store a block. Create() Load() routines only
 *  load the header of the block, if want to insert delete or
 *  other operations, must run Generate() first.
 ***************************************************************/
class _RMBlock{
	char* _Raw;    // will allocte the memory
	_RMBlockHeader _Header;   // Block header
	const _RMTableHeader *_TableHeaderPtr;  // pointer to table header

	_RMBlockTuple _ListHead;
	// the header of invalid or valid tuple list

	std::vector<_RMBlockTuple> _Vect;
	// vector to make a copy tuples

	bool _InsertTupleBegin();
	// do some check of insertion

	bool _InsertTupleEnd(const bool IsDone);
	// decrement of InvalidTuples

public:
	_RMBlock();
	~_RMBlock();

	bool Create(const _RMTableHeader* TableHeaderPtr);
	// create a block only set the _Header without generation

	bool CreateGen(
			const _RMTableHeader* TableHeaderPtr,
			const bool Valid);
	// craete and generate [in]valid list

	bool Load(
			const char* Src,
			const _RMTableHeader* TableHeaderPtr);
	// Load Src into _Header, without generation

	bool LoadGen(
			const char* Src,
			const _RMTableHeader* TableHeaderPtr,
			const bool Valid);
	// Load and generation

	bool Store(char* Dest);
	// store _Header and commited tuples into Dest

	const _RMBlockHeader& GetHeader();
	// return _Header

	void Clear();
	// set all data into zero

	bool Gen(const bool Valid);
	// add valid tuples into vect and list

	bool InsertTuple(const void **ItemPtrArray);
	// insert a tuple from ItemPtrArray

	bool DeleteTuples(
			const size_t Attrs,
			const size_t *AttrIndexArray,
			const RMAttrsRelation *AttrsRelationArray,
			const void **ItemPtrArray);
	// delete tuples where &&_Items[AttrIndexArray[i]] (R) ItemPtrArray[i]
	// (R) is AttrsRelationArray[i]

	bool QueryAllTuples(std::vector<const char*>& Tuples);

	bool QueryTuples(
			const size_t Attrs,
			const size_t *AttrIndexArray,
			const RMAttrsRelation *AttrsRelationArray,
			const void **ItemPtrArray,
			std::vector<const char*>& Tuples );
};


#endif /* RECORDMANAGERPRIVATE_H_ */




