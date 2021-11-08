#pragma once

#ifndef memorySimulator_H_
#define memorySimulator_H_


/*
Cache Simulator
Level one L1 and level two L2 and three L3 cache parameters are read from file (block size, line per set and set per cache).
The 32 bit address is divided into tag bits (t), set index bits (s) and block offset bits (b)
s = log2(#sets)   b = log2(block size)  t=32-s-b
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>MAIN
#include <iomanip>
#include <stdlib.h>
#include <cmath>
#include <bitset>
#include <stdio.h>
#include <map>
#include <queue>
#include <deque>

//access state:
#define NA 0 // "no action"
#define RH 1 //"read hit"
#define RM 2 //"read miss"
#define WH 3 //"Write hit"
#define WM 4 //"write miss"
using namespace std;

enum class ReplacementPolicy
{
	Random,
	LRU,
	PseudoLRU
};

enum class writeAllocatePolicy
{
	writeNoAllocate,
	WriteAllocate
};


enum class WritePolicy
{
	WriteThrough,
	WriteBack
};
enum class AddressMapping
{
	RowInterleaving,
	CacheBlockInterleaving
};




// in 0 the AddressMapping Policy is RowInter leaving  
// in 1 the AddressMapping Policy is CacheBlock Inter leaving  
	
struct config {
	unsigned long blocksize;
	unsigned long L1CacheSize;
	unsigned long L2CacheSize;
	unsigned long L3CacheSize;
	unsigned long NWaySetAssociativeCacheL1;
	unsigned long NWaySetAssociativeCacheL2;
	unsigned long NWaySetAssociativeCacheL3;
	// 0 is write No Allocate ;
	// 1 is write  Allocate
	writeAllocatePolicy writeAllocatePolicy;
	// 0 is write through 
	// 1 is write Back  
	WritePolicy WritePolicy;
	// in 0 the Replacement Policy is Random  
	// in 1 the Replacement Policy is LRU  
	// in 2 the Replacement Policy is PseudoLRU 
	ReplacementPolicy ReplacementPolicy;
	unsigned long L1_Cycles;
	unsigned long L2_Cycles;
	unsigned long L3_Cycles;
	unsigned long DDR_Cycles_PageAndColumnIsOpen;
	unsigned long DDR_Cycles_PageOpen;
	unsigned long DDR_Cycles_PageNotOpen;
	unsigned long bus;
	// in 0 the AddressMapping Policy is RowInter leaving  
	// in 1 the AddressMapping Policy is CacheBlock Inter leaving  
	AddressMapping AddressMapping;
	unsigned long printdata;

};

struct Node {
	bitset<1> key;
	Node* left;
	Node* right;
};


class ConverTrace {
public:           
	ConverTrace();
private:
	string intToHex(string registerData, string sheftRegister);
};


class Cache {
	config config_params;
	unsigned long setIndex_L1, tag_L1, blockOffset_L1, setIndex_L2, tag_L2, blockOffset_L2, setIndex_L3, tag_L3, blockOffset_L3;
	unsigned long indexRow_L1, indexRow_L2, indexRow_L3;
	vector<vector<unsigned long> > cache_L1;
	vector<vector<unsigned long> > cache_L2;
	vector<vector<unsigned long> > cache_L3;
	vector< vector<bitset<1>> >  validBit_L1, validBit_L2, validBit_L3;
	vector< vector<bitset<1>> >  DirtyBit_L1, DirtyBit_L2, DirtyBit_L3;
	vector< vector<unsigned long> >  TagArray_L1, TagArray_L2, TagArray_L3;

public:
	Cache(config config_param);
	vector< vector<unsigned long> >  getCache_L1();
		vector< vector<unsigned long> >  getCache_L2();
		vector< vector<unsigned long> >  getCache_L3();
		vector< vector<unsigned long> >  getTagArray_L1();
		vector< vector<unsigned long> >  getTagArray_L2();
		vector< vector<unsigned long> >  getTagArray_L3();
		vector< vector<bitset<1>> >  getValidBit_L1();
		vector< vector<bitset<1>> >  getValidBit_L2();
		vector< vector<bitset<1>> >  getValidBit_L3();
		vector< vector<bitset<1>> >  getDirtyBit_L1();
		vector< vector<bitset<1>> >  getDirtyBit_L2();
		vector< vector<bitset<1>> >  getDirtyBit_L3();
		unsigned long getRow_L1Cache();
		unsigned long getRow_L2Cache();
		unsigned long getRow_L3Cache();
		vector<vector<unsigned long> > createCache(unsigned long& indexRow, unsigned long& blockSize);
		vector<string> getBits(bitset<32> addr);
};

class mainMemory {
	config config_params;
	vector< vector< long>>   openPages;
	unsigned long bank, channel, rank;
	unsigned long blockOffset, setIndex, tag, columnMem;
	unsigned long ByteInbus, column, row;

public:
	mainMemory(config config_param);
	vector< vector< long>>  createOpenPages(unsigned long& bankInRank, unsigned long& channel, unsigned long& ranks);
	vector<string> getBits1(bitset<32> addr);
	unsigned long getBank();
	unsigned long getChannel();
	unsigned long getRank();
	unsigned long getCoulmnSize();
	vector<string> getBits(bitset<32> addr);
	vector< vector< long>> 	getPage();
};

void upDatePage(vector< vector< long>>& openPage, unsigned long& tagAddress, int  OffsetSize,unsigned long& DDR_Cycles_PageAndColumnIsOpen, 
	unsigned long& DDR_Cycles_PageOpen, unsigned long& DDR_Cycles_PageNotOpen, long& CyclesSimulator, AddressMapping AddressMapping);
struct Node* newNode(bitset<1> value);
struct Node* insertValue(struct Node* root, bitset<1> value, queue<Node*>& q);
Node* createTree(vector<bitset<1>> arr, int n);
struct Node* UpDateValue(int n, int x, struct Node* root, vector<bitset<1>>& arr1);
Node* UpDatePLRUArray(struct Node* root, vector<bitset<1>>& arr1, int Way);
void UpDateLRUArray(vector< vector<unsigned long> >& CounterLRU, int num, int Way, long& addressIndexL, unsigned long& Row_LCache);
int checkReplacementPolicy(unsigned long& Row_LCache, long& addressIndexL, unsigned long& Way,
	unsigned long& addressTag_L, vector< vector<bitset<1>> >& ValidBit_L, ReplacementPolicy  Policy,
	vector< vector< unsigned long> >& CounterLRU_L, vector < bitset<1>> arr);
void firstDateLRU(vector< vector<unsigned long> >& CounterLRU_L1, vector< vector<unsigned long> >& CounterLRU_L2,
	vector< vector<unsigned long> >& CounterLRU_L3,
	unsigned long& Way1, unsigned long& Way2, unsigned long& Way3,
	unsigned long Row_L1Cache, unsigned long Row_L2Cache, unsigned long Row_L3Cache);
void createTreePLRU(vector<struct Node*>& root1, vector<struct Node*>& root2, vector<struct Node*>& root3,
	unsigned long Row_L1Cache, unsigned long Row_L2Cache, unsigned long Row_L3Cache, long way1, long way2, long way3);
void getCounterPLRU_L(vector< vector<bitset<1>> >& CounterPLRU_L1, long way, unsigned long Row_L1Cache,
	vector< vector<bitset<1>> >& CounterPLRU_L2, unsigned long Row_L2Cache,
	vector< vector<bitset<1>> >& CounterPLRU_L3, unsigned long Row_L3Cache);
int  searchInTagArray_L(unsigned long& Row_LCache, unsigned long& Way,unsigned long& addressTag_L, long& addressIndexL,vector<vector<unsigned long> >& TagArray_L);
vector<unsigned long> getTempBlock(unsigned long& Row_LCache, unsigned long& Way,unsigned long& addressTag_L, long& addressIndexL,
	vector<vector<unsigned long> >& TagArray_L, vector<vector<unsigned long> >& Cache_L, long blockSize, int random);
void deleteBlock(vector< vector<bitset<1>> >& DirtyBit, vector< vector<bitset<1>> >& ValidBit, vector<vector<unsigned long> >& Cache,
				int randomNumber, unsigned long Row_LCache, long blocksize, long& addressIndexL);
vector<unsigned long> setBlockAndGetOldBlock(vector<unsigned long>& block, unsigned long& Row_LCache, unsigned long& Way, unsigned long& addressTag_L, long& addressIndexL, vector<vector<unsigned long> >& TagArray_L,
	vector<vector<unsigned long> >& Cache_L, long blockSize,
	vector< vector<bitset<1>> >& ValidBit_L, vector< vector<bitset<1>> >& dirtyBit_L, int locationBlock,
	vector<vector<unsigned long> >& CounterLRU_L,
	vector <struct Node* >& root, vector <bitset<1> >& arr, ReplacementPolicy ReplacementPolicy);
bool UpDateBlock(vector<unsigned long>& block, unsigned long& Row_LCache, unsigned long& Way,unsigned long& addressTag_L, long& addressIndexL,
	vector<vector<unsigned long> >& TagArray_L,vector<vector<unsigned long> >& Cache_L, long blockSize,
	vector< vector<bitset<1>> >& ValidBit_L, vector< vector<bitset<1>> >& dirtyBit_L);
bool setBlockInEmptyCache(vector<unsigned long>& block, unsigned long& Row_LCache, unsigned long& Way,unsigned long& addressTag_L, long& addressIndexL,vector<vector<unsigned long> >& TagArray_L,
	vector<vector<unsigned long> >& Cache_L, long blockSize,vector< vector<bitset<1>> >& ValidBit_L, vector< vector<bitset<1>> >& dirtyBit_L, string type,
	vector< vector<unsigned long> >& CounterLRU_L, vector <struct Node* >& root, vector <bitset<1> >& arr, ReplacementPolicy ReplacementPolicy);

bool setBlock(vector<unsigned long>& block, unsigned long& Row_LCache, unsigned long& Way, unsigned long& addressTag_L, long& addressIndexL,
	vector<vector<unsigned long> >& TagArray_L, vector<vector<unsigned long> >& Cache_L, long blockSize, vector< vector<bitset<1>> >& ValidBit_L, vector< vector<bitset<1>> >& dirtyBit_L, string type,
	vector< vector<unsigned long> >& CounterLRU_L, vector <struct Node* >& root, vector <bitset<1> >& arr, ReplacementPolicy ReplacementPolicy);

// delete the dirty bit !!!
bool setBlockWT(vector<unsigned long>& block, unsigned long& Row_LCache, unsigned long& Way,unsigned long& addressTag_L, long& addressIndexL,
	vector<vector<unsigned long> >& TagArray_L,	vector<vector<unsigned long> >& Cache_L, long blockSize,
	vector< vector<bitset<1>> >& ValidBit_L, vector< vector<bitset<1>> >& dirtyBit_L, string type, vector< vector<unsigned long> >& CounterLRU_L);

void  UpDateCacheWB(unsigned long& Row_LCache, unsigned long& Way,unsigned long& addressTag_L, long& addressIndexL,vector<vector<unsigned long> >& TagArray_L,
	vector< vector<bitset<1>> >& dirtyBit,vector<vector<unsigned long> >& LCache, unsigned long& blockSize, long  Offset,unsigned long& L_Cycles, long& taccess);
bool  UpDateCacheWT(unsigned long& Row_LCache, unsigned long& Way,unsigned long& addressTag_L, long& addressIndexL,vector<vector<unsigned long> >& TagArray_L,
	vector<vector<unsigned long> >& LCache, unsigned long& blockSize, long& addressOffset,long& CyclesSimulator, unsigned long& L1_Cycles);
vector<unsigned long> getBlock(unsigned long& Row_LCache, unsigned long& Way,unsigned long& addressTag_L, long& addressIndexL,
	vector<vector<unsigned long> >& TagArray_L, vector<vector<unsigned long> >& Cache_L, long blockSize, vector<vector<unsigned long> >& CounterLRU_L);
void setBlockToMainMemory(vector< unsigned long >& block, vector< vector<unsigned long> >& memory, vector<unsigned long>& TagArrayMemory,
	unsigned long& blockSize, unsigned long& addressTag, long& addressOffset);
vector<unsigned long> getBlockFromMainMemory(unsigned long& blockSize, unsigned long& addressTag, long& addressOffset, vector< vector< long>>& openPage, int  OffsetSize,
	unsigned long& DDR_Cycles_PageAndColumnIsOpen, unsigned long& DDR_Cycles_PageOpen, unsigned long& DDR_Cycles_PageNotOpen, long& CyclesSimulator, AddressMapping AddressMapping);
bool checkDirtyBit(vector<vector<bitset<1>> >& ValidBit_L, vector<vector<bitset<1>> >& DirtyBit_L,long& addressIndexL, unsigned long& Row_LCache, int radomNum);
void UpDateCache(vector<unsigned long>& block, unsigned long& Row_L1Cache, unsigned long& addressTagMemory, long& addressIndexL1,vector<vector<unsigned long> >& TagArray_L1, vector<vector<unsigned long> >& Cache_L1,
	vector< vector<bitset<1>> >& ValidBit_L1, vector< vector<bitset<1>> >& DirtyBit_L1, unsigned long& Row_L2Cache, long& addressIndexL2,
	vector<vector<unsigned long> >& TagArray_L2, vector<vector<unsigned long> >& Cache_L2, vector< vector<bitset<1>> >& ValidBit_L2, vector< vector<bitset<1>> >& DirtyBit_L2,
	unsigned long& Row_L3Cache, long& addressIndexL3, vector<vector<unsigned long> >& TagArray_L3, vector<vector<unsigned long> >& Cache_L3,vector< vector<bitset<1>> >& ValidBit_L3, vector< vector<bitset<1>> >& DirtyBit_L3, string type, unsigned long& addressOffset,
	vector< vector<unsigned long> >& CounterLRU_L1, vector< vector<unsigned long> >& CounterLRU_L2, vector< vector<unsigned long> >& CounterLRU_L3,vector <struct Node* >& root1, vector <struct Node* >& root2, vector <struct Node* >& root3,
	vector<bitset<1> >& arr1, vector<bitset<1> >& arr2, vector<bitset<1> >& arr3, vector< vector< long>>& openPage, long row, long bnak, long column, long columnSize, int OffsetSize,long& CyclesSimulator,int bus, config config_param);
void ResetDirtyBit(vector< vector<bitset<1>> >& dirtyBit1, vector< vector<bitset<1>> >& dirtyBit2,
					vector< vector<bitset<1>> >& dirtyBit3, unsigned long& Way1, unsigned long& Way2, unsigned long& Way3, unsigned long Row_L1Cache,unsigned long Row_L2Cache, unsigned long Row_L3Cache);
void writeAllocate(vector<unsigned long>& block, unsigned long& Row_L1Cache, unsigned long& addressTagMemory, long& addressIndexL1, vector<vector<unsigned long> >& TagArray_L1, vector<vector<unsigned long> >& Cache_L1, vector< vector<bitset<1>> >& ValidBit_L1, vector< vector<bitset<1>> >& DirtyBit_L1,
	unsigned long& Row_L2Cache, long& addressIndexL2, vector<vector<unsigned long> >& TagArray_L2, vector<vector<unsigned long> >& Cache_L2, vector< vector<bitset<1>> >& ValidBit_L2, vector< vector<bitset<1>> >& DirtyBit_L2,
	unsigned long& Row_L3Cache, long& addressIndexL3, vector<vector<unsigned long> >& TagArray_L3, vector<vector<unsigned long> >& Cache_L3, vector< vector<bitset<1>> >& ValidBit_L3, vector< vector<bitset<1>> >& DirtyBit_L3, string type, long& addressOffset,
	vector< vector<unsigned long> >& CounterLRU_L1, vector< vector<unsigned long> >& CounterLRU_L2, vector< vector<unsigned long> >& CounterLRU_L3, vector <struct Node* >& root1, vector <struct Node* >& root2, vector <struct Node* >& root3,
	vector<bitset<1> >& arr1, vector<bitset<1> >& arr2, vector<bitset<1> >& arr3, unsigned long hit_L1, unsigned long hit_L2, unsigned long hit_L3,
	WritePolicy WritePolicy, vector< vector< long>>& openPage, long row, long bnak, long column, long columnSize, int OffsetSize, long& CyclesSimulator, int bus, config config_param);


void writeThrough(vector<unsigned long>& block, unsigned long& Row_L1Cache,unsigned long& addressTagMemory, long& addressIndexL1,vector<vector<unsigned long> >& TagArray_L1,vector<vector<unsigned long> >& Cache_L1,
	vector< vector<bitset<1>> >& ValidBit_L1, vector< vector<bitset<1>> >& DirtyBit_L1,
	unsigned long& Row_L2Cache, long& addressIndexL2,vector<vector<unsigned long> >& TagArray_L2,vector<vector<unsigned long> >& Cache_L2,
	vector< vector<bitset<1>> >& ValidBit_L2, vector< vector<bitset<1>> >& DirtyBit_L2,
	unsigned long& Row_L3Cache, long& addressIndexL3, vector<vector<unsigned long> >& TagArray_L3,vector<vector<unsigned long> >& Cache_L3,
	vector< vector<bitset<1>> >& ValidBit_L3, vector< vector<bitset<1>> >& DirtyBit_L3, string type,long& addressOffset,
	vector< vector<unsigned long> >& CounterLRU_L1, vector< vector<unsigned long> >& CounterLRU_L2,vector< vector<unsigned long> >& CounterLRU_L3,
	vector <struct Node* >& root1, vector <struct Node* >& root2, vector <struct Node* >& root3,
	vector<bitset<1> >& arr1, vector<bitset<1> >& arr2, vector<bitset<1> >& arr3,unsigned long hit_L1, unsigned long hit_L2, unsigned long hit_L3,
	vector< vector< long>>& openPage, long row, long bnak, long column, long columnSize, int OffsetSize,long& CyclesSimulator,int& bus, config configParam);



#endif // GAMESTATE_H_


