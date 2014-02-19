#ifndef RECORDMANAGER_H_
#define RECORDMANAGER_H_

#include <string>
#include <map>
#include "RecordManagerPrivate.h"
#include "BufferX.h"


class RMTable{
public:
	typedef _RMTableHeader Header;		// Table header type
	typedef RMAttrType 		AttrType;   // Attribute type type
	typedef RMAttrsRelation AttrsRelation;   // Attributes realation type

	static const size_t BlockSize = _BlockSize;  // block size
	static const size_t MaxAttrs = _RMMaxAttrs;		// max attributes
	static const size_t HeaderPtr = _RMTableHeaderPtr;  // table header ptr
	static const size_t AttrNa = _RMAttrNa;

	RMTable();
	~RMTable();
	// Constructors

	_RMTableHeader GetHeader();
	// return table header

  size_t AttrNameToIndex(const char* AttrName);
  // get name index from name

	bool IsAttrUnique(const char* AttrName);
	bool IsAttrPrimary(const char* AttrName);
	bool IsAttrExists(const char* AttrName);

	bool Create(
			const char* TableName,
			const size_t Attrs,
			const char** AttrNameArray,
			const size_t *AttrSizeArray,
			const bool *AttrIsPrimaryArray,
			const bool *AttrIsUniqueArray,
			const RMAttrType* AttrTypeArray );
	// Create an new table and write to buffer and wirte to disk

	bool Load(const char* TableName);
	// load a table named TableName. Actually only load table header.

	bool Store();
	// stroe all data and buffer write to disk

	bool Drop();
	// drop table, but must load table before

	/////////////////////////////////////////////////////////////////////
	
	bool InsertTupleBegin();
	// find a free block or create a new block to insert tuple

	bool InsertTuple(
      const void **ItemPtrArray,
      size_t& BlockIndex);
	// insert tuple from ItemPtrArray and return inserting block index

	bool InsertTuple(
      const char **ItemStringArray,
      size_t& BlockIndex);
	// insert tuple from a string array ItemStringArray and return inserting position

	bool InsertTuple(
      const std::vector<std::string>& ItemStringVector,
      size_t& BlockIndex);
	// insert tuple from a string vector ItemStringVector and return inserting position

	bool InsertTupleEnd(bool IsDone);
	// store data to buffer and ensure buffer wirte to disk
	
	/////////////////////////////////////////////////////////////////////
	
	bool DeleteTuples(
			const size_t Attrs,
			const size_t *AttrIndexArray,
			const RMAttrsRelation *AttrsRelationArray,
			const void **ItemPtrArray);
	// delete tuples where &&_Items[AttrIndexArray[i]] (R) ItemPtrArray[i]
	// (R) is AttrsRelationArray[i]

	bool DeleteTuples(
			const size_t Attrs,
			const char **AttrNameArray,
			const char **AttrsRelStringArray,
			const char **ItemStringArray);

	bool DeleteTuples(
			const size_t Attrs,
			const std::vector<std::string>& AttrNameVector,
			const std::vector<std::string>& AttrsRelStringVector,
			const std::vector<std::string>& ItemStringVector);

	////////////////////////////////////////////////////////////////////////
	
	bool DeleteBlockTuples(
			const size_t BlockIndex,
			const size_t Attrs,
			const size_t *AttrIndexArray,
			const RMAttrsRelation *AttrsRelationArray,
			const void **ItemPtrArray);
	// delete tuples where &&_Items[AttrIndexArray[i]] (R) ItemPtrArray[i]
	// (R) is AttrsRelationArray[i].
	// This is usually called by IndexManager

	bool DeleteBlockTuples(
			const size_t BlockIndex,
			const size_t Attrs,
			const char **AttrNameArray,
			const char **AttrsRelStringArray,
			const char **ItemStringArray);

	bool DeleteBlockTuples(
			const size_t BlockIndex,
			const size_t Attrs,
			const std::vector<std::string>& AttrNameVector,
			const std::vector<std::string>& AttrsRelStringVector,
			const std::vector<std::string>& ItemStringVector);

	///////////////////////////////////////////////////////////////////////////

	bool QueryBegin();
	// preparing query

	bool QueryAllTuples(
      bool& eof,
      std::vector<const char*>& Tuples);
	// get all tuples from the table. Be sure run after QueryBegin() and before
	// QueryEnd(). This routine will return the query result in a block in one
	// time, and Tuples will be stored in buffer safely at this time.
	// So please run a loop to test eof and get all result in the table.
	// And do not do any thing to change the buffer before store the data to
	// other place. Data in Tuples are raw data need to be parsed.

	bool QueryAllTuples(
      bool& eof,
      std::vector< std::vector<std::string> >& Tuples);
	// get all tuples from the table. Be sure run after QueryBegin() and before
	// QueryEnd(). This routine will return the query result in a block in one
	// time. If eof is true then all tuples are got. And data in Tuples are
	// readable string.

	bool QueryTuples(
			const size_t Attrs,
			const size_t *AttrIndexArray,
			const RMAttrsRelation *AttrsRelationArray,
			const void **ItemPtrArray,
			bool& eof,
			std::vector<const char*>& Tuples );
	// select tuples where &&_Items[AttrIndexArray[i]] == ItemPtrArray[i]
	// and same as QueryAllTuples();

	bool QueryTuples(
			const size_t Attrs,
			const char **AttrNameArray,
			const char **AttrsRelStringArray,
			const char **ItemStringArray,
			bool& eof,
			std::vector< std::vector<std::string> >& Tuples);

	bool QueryTuples(
			const size_t Attrs,
			const std::vector<std::string>& AttrNameVector,
			const std::vector<std::string>& AttrsRelStringVector,
			const std::vector<std::string>& ItemStringVector,
			bool& eof,
			std::vector< std::vector<std::string> >& Tuples);


	bool QueryEnd(const bool IsDone);
	// end query

	//////////////////////////////////////////////////////////////////////////////////

	bool QueryBlockTuples(
			const size_t BlockIndex,
			const size_t Attrs,
			const size_t *AttrIndexArray,
			const RMAttrsRelation *AttrsRelationArray,
			const void **ItemPtrArray,
			std::vector<const char*>& Tuples );
	// only query result in BlockIndex, no need run QueryBegin() and
	// QueryEnd(). Usually called by IndexManager

	bool QueryBlockTuples(
		const size_t BlockIndex,
		const size_t Attrs,
		const char **AttrNameArray,
		const char **AttrsRelStringArray,
		const char **ItemStringArray,
		std::vector< std::vector<std::string> >& Tuples );

	bool QueryBlockTuples(
		const size_t BlockIndex,
		const size_t Attrs,
		const std::vector<std::string>& AttrNameVector,
		const std::vector<std::string>& AttrsRelStringVector,
		const std::vector<std::string>& ItemStringVector,
		std::vector< std::vector<std::string> >& Tuples );


////////////////////////////////////////////////////////////////////////////////
///////   Private
///////////////////////////////////////////////////////////////////////////////
private:
	_RMTableHeader _Header; // table header
	_RMBlock _Block;  			// block
	size_t _BlockPtr; 			// block position
	BufferX _Buffer;  		// buffer
	std::string _TableName; // table name
	std::map<std::string, size_t> _AttrNameIndexMap;
	// Attribute name map to index

	const void* _ItemPtrArray[MaxAttrs];
	int _IntArray[MaxAttrs];
	float _FloatArray[MaxAttrs];
	const char* _ItemStringArray[MaxAttrs];
	size_t _AttrIndexArray[MaxAttrs];
	const char* _AttrNameArray[MaxAttrs];
	AttrsRelation _RelationArray[2*MaxAttrs];
	const char* _RelStringArray[2*MaxAttrs];
	// space only provide twice times of MaxAttrs

	/////////////////////////////////////////////////////////////////////////
	///////   Private routines
	//////////////////////////////////////////////////////////////////

	bool _FindGenFreeBlock(const size_t StartBlockPtr);
	// this function will find and gen a free block
	// after block "StartBlockPtr"

	size_t _AttrNameToIndex(const char* AttrName);

	size_t* _AttrNameToIndexArray(
			const size_t Attrs,
			const char** AttrNameArray);
	// convert attribute array to index array

	const char** _AttrNameVectorToArray(
			const std::vector<std::string>& AttrNameVector);

	const void** _ItemStringArrayToPtrArray(
			const size_t Attrs,
			const size_t *AttrIndexArray,
			const char **ItemStringArray);
	// convert string array to ptr array, result in _ItemPtrArray, depends on
	// _IntArray, _FloatArray and also String in ItemStringArray[x]

	const char** _ItemStringVectorToArray(
			const std::vector<std::string>& ItemStringVector);
	// convert string vecto to array, reuslt in _ItemStringArray and also depends
	// on ItemStringVector

	bool _QueryToReadable(
		const std::vector<const char*>& SrcTuples,
		std::vector< std::vector<std::string> >& DestTuples);
	// convert unreadable tuples into readable result
	// this will take many CPUs...

	const RMAttrsRelation* _AttrsRelStringArrayToRelationArray(
			const size_t Attrs,
			const char**AttrsRelStringArray);
	// convert string attributes relation into AttrsRelation
	// Provide: >, <, ==, !=, >=, <=

	const char** _AttrsRelStringVectorToArray(
			const std::vector<std::string>& AttrsRelStringVector);
	// convert string vector to array
};


#endif	/* RECORDMANAGER_H_ */
