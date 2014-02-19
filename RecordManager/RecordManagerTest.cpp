#include <cstdio>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include "RecordManager.h"

using namespace std;

void PrintTableHeader(_RMTableHeader& Header)
{
	puts("-----------------TableHeader--------------------");
	printf("Total size: %u\n", sizeof(_RMTableHeader));
	printf("Attrs: %u\n", Header.Attrs);
	printf("AttrsSize: %u\n", Header.AttrsSize);

	printf("AttrNameArray: ");
	for(size_t i=0; i<Header.Attrs; i++)
		printf("%s ", Header.AttrNameArray[i]);
	putchar('\n');
	printf("AttrSizeArray: ");
	for(size_t i=0; i<Header.Attrs; i++)
		printf("%u ", Header.AttrSizeArray[i]);
	putchar('\n');
	printf("AttrTypeArray: ");
	for(size_t i=0; i<Header.Attrs; i++)
		printf("%u ", Header.AttrTypeArray[i]);
	putchar('\n');
	printf("AttrIsPrimaryArray: ");
	for(size_t i=0; i<Header.Attrs; i++)
		printf("%u ", Header.AttrIsPrimaryArray[i]);
	putchar('\n');
	printf("AttrIsUniqueArray: ");
	for(size_t i=0; i<Header.Attrs; i++)
		printf("%u ", Header.AttrIsUniqueArray[i]);
	putchar('\n');
	printf("BlockTuples: %u\n", Header.BlockTuples);
	printf("BlockTupleSize: %u\n", Header.BlockTupleSize);
	printf("FirstBlockPtr: %u\n", Header.FirstBlockPtr);
	printf("FirstFreeBlockPtr: %u\n", Header.FirstFreeBlockPtr);
	puts("-----------------End of TableHeader--------------");
}

void PrintTuples(const vector< vector<string> >& Tuples)
{
	printf("Query %u tuples in this loop.\n", Tuples.size());
	for(size_t i=0; i<Tuples.size(); i++){
		for(size_t j=0; j<Tuples[i].size(); j++){
			printf("%s ", Tuples[i][j].c_str());
		}
		putchar('\n');
	}
}


int main(int argc, char** argv)
{
	RMTable tb;
	RMTable::Header Header;

	const char* AttrNameArray[] = {"aa", "bb", "cc"};

	const bool AttrIsPrimaryArray[] = {true, false, false};
	const bool *AttrIsUniqueArray = AttrIsPrimaryArray;

	size_t AttrSizeArray[] = {sizeof(int), sizeof(float), 10*sizeof(char)};
	// define an attribute size array, if is String then please consider '\0'

	RMTable::AttrType AttrTypeArray[] = {RMIntType, RMFloatType, RMStringType};
	// define an attribute type array, the order is important

	int x0 = 215;
	float x1 = 100.123f;
	char x2[10] = "abcde";

	const void* ItemPtrArray[] = {&x0, &x1, x2};
	const char* ItemStringArray[] = {"215", "100.123", "abcde"};
	vector<string> ItemStringVector;
	for(size_t i=0; i<3; i++)
		ItemStringVector.push_back(string(ItemStringArray[i]));

	bool eof;
	vector< vector<string> > Tuples;

	puts("Craeting Table...");
	tb.Create( "Test", 3,
			AttrNameArray,
			AttrSizeArray,
			AttrIsPrimaryArray,
			AttrIsUniqueArray,
			AttrTypeArray);
	Header = tb.GetHeader();
	PrintTableHeader(Header);
	puts("Craeting Table...done");
//	getchar();

	puts("Loading Table...");
	tb.Load("Test");
	Header = tb.GetHeader();
	PrintTableHeader(Header);
	puts("Loading Table...done");
//	getchar();

	puts("Inserting...");
	tb.InsertTupleBegin();
	for(int i=0; i<1000000; i++){
		size_t BlockIndex;
		x0++; x1 += 0.10f;

		tb.InsertTuple(ItemPtrArray, BlockIndex);
		//tb.InsertTuple(ItemStringArray,BlockIndex);
		//tb.InsertTuple(ItemStringVector,BlockIndex);
	}
	tb.InsertTupleEnd(true);
	puts("Inserting...done");
//	getchar();

	puts("Querying all tuples...");
	puts("--------------------------------------------");
	tb.QueryBegin();
	eof = false;
	while( ! eof ){
		tb.QueryAllTuples(eof, Tuples);
		PrintTuples(Tuples);
	}
	tb.QueryEnd(true);
	puts("Querying all tuples...done");
//	getchar();

	puts("--------------------------------------------");
	size_t IAttrs = 1;
	const char* IAttrNameArray[] = {"aa","bb","cc"};
	const char *IAttrsRelStringArray[] = {">=", "<=", "="};
	const char *IItemStringArray2[] = {"222","1000.8", "abcde"};
	puts("Querying tuples...\nConditions:" );
	for(size_t i=0; i<IAttrs; i++)
		printf("[%s] %s %s ",
				IAttrNameArray[i],
				IAttrsRelStringArray[i],
				IItemStringArray2[i]);
	putchar('\n');
	tb.QueryBegin();
	eof = false;
	while( !eof ){
		tb.QueryTuples(IAttrs,
				IAttrNameArray,
				IAttrsRelStringArray,
				IItemStringArray2,
				eof,
				Tuples);
		PrintTuples(Tuples);
	}
	tb.QueryEnd(true);
	puts("Querying tuples...done" );
//	getchar();

	puts("--------------------------------------------");
	puts("Deleting tuples...");
	size_t DAttrs = 2;
	const char* DAttrNameArray[] = {"aa","aa","cc"};
	const char *DAttrsRelStringArray[] = {"<=", ">", "="};
	const char *DItemStringArray[] = {"1000","219", "abcde"};

	for(size_t i=0; i<DAttrs; i++)
		printf("[%s] %s %s ",
				DAttrNameArray[i],
				DAttrsRelStringArray[i],
				DItemStringArray[i]);
	putchar('\n');
	tb.DeleteTuples(DAttrs,DAttrNameArray,DAttrsRelStringArray,DItemStringArray);
	puts("Deleting tuples...done");
//	getchar();


	puts("--------------------------------------------");
	puts("Querying all tuples...");
	tb.Load("Test");
	tb.QueryBegin();
	eof = false;
	while( ! eof ){
		tb.QueryAllTuples(eof, Tuples);
		PrintTuples(Tuples);
	}
	tb.QueryEnd(true);
	puts("Querying all tuples...done");
//	getchar();


	puts("----------------------------------------------");
	puts("Droping table..press ENTER to continue");
//	getchar();
//	tb.Drop();
	puts("drop table done");
	return 0;
}
