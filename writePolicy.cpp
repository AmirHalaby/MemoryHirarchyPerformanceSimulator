#include "memorySimulator.h"


int  searchInTagArray_L(unsigned long& Row_LCache, unsigned long& Way,
	unsigned long& addressTag_L, long& addressIndexL,
	vector<vector<unsigned long> >& TagArray_L) {
	int TagInRow = (int)addressIndexL / Row_LCache;
	for (int i = 0; i < Way; i++)
	{
		if (TagArray_L[TagInRow][i] == addressTag_L)
			return 1;

	}
	return 0;
}

vector<unsigned long> getTempBlock(unsigned long& Row_LCache, unsigned long& Way,
	unsigned long& addressTag_L, long& addressIndexL,
	vector<vector<unsigned long> >& TagArray_L, vector<vector<unsigned long> >& Cache_L, long blockSize, int random) {

	vector<unsigned long> block;
	block.resize(blockSize);

	int x = 0;
	int TagInRow = (int)addressIndexL / Row_LCache;
	x = (int)random * blockSize;
	for (int j = 0; j < blockSize; j++)
	{
		block[j] = Cache_L[TagInRow][x];
		x++;

	}


	return block;

}

void deleteBlock(vector< vector<bitset<1>> >& DirtyBit, vector< vector<bitset<1>> >& ValidBit, vector<vector<unsigned long> >& Cache,
	int randomNumber, unsigned long Row_LCache, long blocksize, long& addressIndexL) {

	int TagInRow = (int)addressIndexL / Row_LCache;
	DirtyBit[TagInRow][randomNumber] = false;
	ValidBit[TagInRow][randomNumber] = false;

	int x = (int)randomNumber * blocksize;
	for (int j = 0; j < blocksize; j++)
	{
		Cache[TagInRow][x] = 0;
		x++;

	}



}


vector<unsigned long> setBlockAndGetOldBlock(vector<unsigned long>& block, unsigned long& Row_LCache, unsigned long& Way, unsigned long& addressTag_L, long& addressIndexL, vector<vector<unsigned long> >& TagArray_L,
	vector<vector<unsigned long> >& Cache_L, long blockSize,
	vector< vector<bitset<1>> >& ValidBit_L, vector< vector<bitset<1>> >& dirtyBit_L, int locationBlock,
	vector<vector<unsigned long> >& CounterLRU_L,
	vector <struct Node* >& root, vector <bitset<1> >& arr, ReplacementPolicy ReplacementPolicy) {


	int TagInRow = (int)addressIndexL / Row_LCache;

	// get block


	vector<unsigned long> tempBock;
	tempBock.resize(blockSize);


	tempBock = getTempBlock(Row_LCache, Way, addressTag_L, addressIndexL, TagArray_L, Cache_L, blockSize, locationBlock);



	deleteBlock(dirtyBit_L, ValidBit_L, Cache_L,
		locationBlock, Row_LCache, blockSize, addressIndexL);



	//set block

	int counter = 0;

	for (int i = 0; i < Way; i++)
	{
		if (ValidBit_L[TagInRow][i] == false)
		{
			TagArray_L[TagInRow][i] = addressTag_L;
			ValidBit_L[TagInRow][i] = true;
			counter = (int)i * blockSize;
			for (int j = 0; j < blockSize; j++)
			{
				Cache_L[TagInRow][counter] = block[j];

				counter++;
			}
			if (ReplacementPolicy == ReplacementPolicy::LRU) {
				UpDateLRUArray(CounterLRU_L, i, Way, addressIndexL, Row_LCache);
			}


		}
	}

	return tempBock;

}

bool UpDateBlock(vector<unsigned long>& block, unsigned long& Row_LCache, unsigned long& Way,
	unsigned long& addressTag_L, long& addressIndexL,
	vector<vector<unsigned long> >& TagArray_L,
	vector<vector<unsigned long> >& Cache_L, long blockSize,
	vector< vector<bitset<1>> >& ValidBit_L, vector< vector<bitset<1>> >& dirtyBit_L) {


	int counter = 0;
	int TagInRow = (int)addressIndexL / Row_LCache;
	int randomNumber;
	for (int i = 0; i < Way; i++)
	{
		if (TagArray_L[TagInRow][i] == addressTag_L) {

			ValidBit_L[TagInRow][i] = true;
			dirtyBit_L[TagInRow][i] = true;

			counter = (int)i * blockSize;
			for (int j = 0; j < blockSize; j++)
			{
				Cache_L[TagInRow][counter] = block[j];

				counter++;

			}
			return true;
		}
	}


	return false;


}

bool setBlockInEmptyCache(vector<unsigned long>& block, unsigned long& Row_LCache, unsigned long& Way,
	unsigned long& addressTag_L, long& addressIndexL,
	vector<vector<unsigned long> >& TagArray_L,
	vector<vector<unsigned long> >& Cache_L, long blockSize,
	vector< vector<bitset<1>> >& ValidBit_L, vector< vector<bitset<1>> >& dirtyBit_L, string type,
	vector< vector<unsigned long> >& CounterLRU_L, vector <struct Node* >& root, vector <bitset<1> >& arr, ReplacementPolicy ReplacementPolicy) {


	//בפונקציה הזאת חסר שימוש בראנדום
	int counter = 0;
	int x = 0;
	int TagInRow = (int)addressIndexL / Row_LCache;
	int location;


	for (int i = 0; i < Way; i++)
	{
		if (TagArray_L[TagInRow][i] == addressTag_L) {
			if (dirtyBit_L[TagInRow][i] == false)
			{
				if (ReplacementPolicy == ReplacementPolicy::PseudoLRU) {
					counter = (int)(i)*blockSize;
					for (int j = 0; j < blockSize; j++)
					{
						if (block[j] != 0 && (type == "W" || type == "w")) {
							x++;
						}

						Cache_L[TagInRow][counter] = block[j];
						counter++;

					}
				}

				else
				{


					TagArray_L[TagInRow][i] = addressTag_L;
					ValidBit_L[TagInRow][i] = true;
					counter = (int)i * blockSize;
					for (int j = 0; j < blockSize; j++)
					{
						if (block[j] != 0 && (type == "W" || type == "w")) {
							x++;
						}

						Cache_L[TagInRow][counter] = block[j];
						counter++;

					}
				}
				if (x > 1) {
					dirtyBit_L[TagInRow][i] = true;
				}

				if (ReplacementPolicy == ReplacementPolicy::LRU) {
					UpDateLRUArray(CounterLRU_L, i, Way, addressIndexL, Row_LCache);
				}


				return true;
			}

			return true;

		}


	}
	for (int i = 0; i < Way; i++)
	{

		if (ValidBit_L[TagInRow][i] == false)
		{
			if (ReplacementPolicy == ReplacementPolicy::PseudoLRU) {
				root[TagInRow] = UpDatePLRUArray(root[TagInRow], arr, Way);
				location = checkReplacementPolicy(Row_LCache, addressIndexL, Way, addressTag_L, ValidBit_L, ReplacementPolicy, CounterLRU_L, arr);

				TagArray_L[TagInRow][location] = addressTag_L;
				ValidBit_L[TagInRow][location] = true;
				counter = (int)location * blockSize;
				for (int j = 0; j < blockSize; j++)
				{
					if (block[j] != 0 && (type == "W" || type == "w")) {
						x++;
					}

					Cache_L[TagInRow][counter] = block[j];
					counter++;

				}
			}

			else
			{


				TagArray_L[TagInRow][i] = addressTag_L;
				ValidBit_L[TagInRow][i] = true;
				counter = (int)i * blockSize;
				for (int j = 0; j < blockSize; j++)
				{
					if (block[j] != 0 && (type == "W" || type == "w")) {
						x++;
					}

					Cache_L[TagInRow][counter] = block[j];
					counter++;

				}
			}
			if (x > 1) {
				dirtyBit_L[TagInRow][i] = true;
			}

			if (ReplacementPolicy == ReplacementPolicy::LRU) {
				UpDateLRUArray(CounterLRU_L, i, Way, addressIndexL, Row_LCache);
			}


			return true;
		}
	}

	for (int j = 0; j < Row_LCache; j++)
	{
		for (int i = 0; i < Way; i++)
		{


			if (TagArray_L[j][i] == addressTag_L) {

				if (dirtyBit_L[j][i] == false)

				{

					if (ReplacementPolicy == ReplacementPolicy::PseudoLRU) {
						//					root[TagInRow] = UpDatePLRUArray(root[TagInRow], arr, Way);
						//					location = checkReplacementPolicy(Row_LCache, addressIndexL, Way, addressTag_L, ValidBit_L, ReplacementPolicy, CounterLRU_L, arr);
						//					TagArray_L[TagInRow][location] = addressTag_L;
						//					ValidBit_L[TagInRow][location] = true;
						counter = (int)(i)*blockSize;
						for (int h = 0; h < blockSize; h++)
						{
							if (block[h] != 0) {
								x++;
							}

							Cache_L[j][counter] = block[h];
							counter++;

						}
					}

					else
					{


						TagArray_L[j][i] = addressTag_L;
						ValidBit_L[j][i] = true;
						counter = (int)i * blockSize;
						for (int j = 0; j < blockSize; j++)
						{
							if (block[j] != 0 && (type == "W" || type == "w")) {
								x++;
							}

							Cache_L[j][counter] = block[j];
							counter++;

						}
					}
					if (x > 1) {
						dirtyBit_L[j][i] = true;
					}

					if (ReplacementPolicy == ReplacementPolicy::LRU) {
						UpDateLRUArray(CounterLRU_L, i, Way, addressIndexL, Row_LCache);
					}


					return true;
				}

				return true;

			}


		}

	}
	return false;


}


bool setBlock(vector<unsigned long>& block, unsigned long& Row_LCache, unsigned long& Way,
	unsigned long& addressTag_L, long& addressIndexL,
	vector<vector<unsigned long> >& TagArray_L,
	vector<vector<unsigned long> >& Cache_L, long blockSize,
	vector< vector<bitset<1>> >& ValidBit_L, vector< vector<bitset<1>> >& dirtyBit_L, string type,
	vector< vector<unsigned long> >& CounterLRU_L, vector <struct Node* >& root, vector <bitset<1> >& arr, ReplacementPolicy ReplacementPolicy) {

	int counter = 0;
	int x = 0;
	int TagInRow = (int)addressIndexL / Row_LCache;
	int location;


	for (int i = 0; i < Way; i++)
	{
		if (TagArray_L[TagInRow][i] == addressTag_L) {
			return false;


		}


	}
	for (int i = 0; i < Way; i++)
	{
		if (ValidBit_L[TagInRow][i] == false)
		{


			TagArray_L[TagInRow][i] = addressTag_L;
			ValidBit_L[TagInRow][i] = true;
			counter = (int)i * blockSize;
			for (int j = 0; j < blockSize; j++)
			{
				if (block[j] != 0 && (type == "W" || type == "w")) {
					x++;
				}

				Cache_L[TagInRow][counter] = block[j];
				counter++;

			}

			if (x > 1) {
				dirtyBit_L[TagInRow][i] = true;
			}

			if (ReplacementPolicy == ReplacementPolicy::LRU) {
				UpDateLRUArray(CounterLRU_L, i, Way, addressIndexL, Row_LCache);
			}


			return true;
		}
	}


	return false;


}

// delete the dirty bit !!!
bool setBlockWT(vector<unsigned long>& block, unsigned long& Row_LCache, unsigned long& Way,
	unsigned long& addressTag_L, long& addressIndexL,
	vector<vector<unsigned long> >& TagArray_L,
	vector<vector<unsigned long> >& Cache_L, long blockSize,
	vector< vector<bitset<1>> >& ValidBit_L, vector< vector<bitset<1>> >& dirtyBit_L, string type, vector< vector<unsigned long> >& CounterLRU_L) {

	int counter = 0;
	int x = 0;
	int TagInRow = (int)addressIndexL / Row_LCache;
	int randomNumber;


	for (int i = 0; i < Way; i++)
	{
		if (TagArray_L[TagInRow][i] == addressTag_L) {
			return false;
		}
	}
	for (int i = 0; i < Way; i++)
	{
		if (ValidBit_L[TagInRow][i] == false)
		{
			TagArray_L[TagInRow][i] = addressTag_L;
			ValidBit_L[TagInRow][i] = true;
			counter = (int)i * blockSize;
			for (int j = 0; j < blockSize; j++)
			{
				if (block[j] != 0 && (type == "W" || type == "w")) {
					x++;
				}

				Cache_L[TagInRow][counter] = block[j];

				counter++;

			}

			UpDateLRUArray(CounterLRU_L, i, Way, addressIndexL, Row_LCache);
			return true;
		}
	}


	return false;


}


void  UpDateCacheWB(unsigned long& Row_LCache, unsigned long& Way,
	unsigned long& addressTag_L, long& addressIndexL,
	vector<vector<unsigned long> >& TagArray_L, vector< vector<bitset<1>> >& dirtyBit,
	vector<vector<unsigned long> >& LCache, unsigned long& blockSize, long  Offset,
	unsigned long& L_Cycles, long& taccess) {

	taccess = taccess + L_Cycles;
	int TagInRow = (int)addressIndexL / Row_LCache;
	int x;
	for (int i = 0; i < Way; i++)
	{
		x = (int)Offset + blockSize * i;
		if (TagArray_L[TagInRow][i] == addressTag_L)
		{
			dirtyBit[TagInRow][i] = true;
			LCache[TagInRow][x] = addressTag_L;
			break;
		}
	}
}



bool  UpDateCacheWT(unsigned long& Row_LCache, unsigned long& Way,
	unsigned long& addressTag_L, long& addressIndexL,
	vector<vector<unsigned long> >& TagArray_L,
	vector<vector<unsigned long> >& LCache, unsigned long& blockSize, long& addressOffset,
	long& CyclesSimulator, unsigned long& L1_Cycles) {

	int TagInRow = (int)addressIndexL / Row_LCache;
	int culumn;
	for (int i = 0; i < Way; i++)
	{
		if (TagArray_L[TagInRow][i] == addressTag_L)
		{
			culumn = addressOffset + i * blockSize;
			LCache[TagInRow][culumn] = addressTag_L;
			CyclesSimulator = CyclesSimulator + L1_Cycles;
			return true;
		}
	}
	return false;
}





vector<unsigned long> getBlock(unsigned long& Row_LCache, unsigned long& Way,
	unsigned long& addressTag_L, long& addressIndexL,
	vector<vector<unsigned long> >& TagArray_L, vector<vector<unsigned long> >& Cache_L, long blockSize, vector<vector<unsigned long> >& CounterLRU_L) {

	vector<unsigned long> block;
	block.resize(blockSize);

	int x = 0;
	int TagInRow = (int)addressIndexL / Row_LCache;
	for (int i = 0; i < Way; i++)
	{
		if (TagArray_L[TagInRow][i] == addressTag_L)
		{
			x = (int)i * blockSize;
			for (int j = 0; j < blockSize; j++)
			{
				block[j] = Cache_L[TagInRow][x];
				x++;

			}
			UpDateLRUArray(CounterLRU_L, i, Way, addressIndexL, Row_LCache);

			return block;
			break;
		}

	}
	return block;

}



void setBlockToMainMemory(vector< unsigned long >& block, vector< vector<unsigned long> >& memory, vector<unsigned long>& TagArrayMemory,
	unsigned long& blockSize, unsigned long& addressTag, long& addressOffset) {

	int cont = 0;
	while (true)
	{

		if (TagArrayMemory[cont] == addressTag)
		{

			for (int j = 0; j < blockSize; j++) {
				memory[cont][j] = block[j];

			}

			break;

		}
		cont++;

	}

}



vector<unsigned long> getBlockFromMainMemory(unsigned long& blockSize, unsigned long& addressTag, long& addressOffset
	, vector< vector< long>>& openPage, int  OffsetSize,
	unsigned long& DDR_Cycles_PageAndColumnIsOpen, unsigned long& DDR_Cycles_PageOpen, unsigned long& DDR_Cycles_PageNotOpen, long& CyclesSimulator, AddressMapping AddressMapping) {

	vector<unsigned long> block;
	block.resize(blockSize);

	for (int i = 0; i < blockSize; i++)
	{
		block[i] = 0;
	}
	block[addressOffset] = addressTag;

	upDatePage(openPage, addressTag, OffsetSize,
		DDR_Cycles_PageAndColumnIsOpen, DDR_Cycles_PageOpen, DDR_Cycles_PageNotOpen, CyclesSimulator, AddressMapping);

	return block;
}

bool checkDirtyBit(vector<vector<bitset<1>> >& ValidBit_L, vector<vector<bitset<1>> >& DirtyBit_L,
	long& addressIndexL, unsigned long& Row_LCache, int radomNum) {

	int TagInRow = (int)addressIndexL / Row_LCache;
	if (DirtyBit_L[TagInRow][radomNum] == true) {
		return true;
	}
	else {
		return false;
	}
}







void UpDateCache(vector<unsigned long>& block, unsigned long& Row_L1Cache,unsigned long& addressTagMemory, long& addressIndexL1,
	vector<vector<unsigned long> >& TagArray_L1,vector<vector<unsigned long> >& Cache_L1,vector< vector<bitset<1>> >& ValidBit_L1, vector< vector<bitset<1>> >& DirtyBit_L1,
	unsigned long& Row_L2Cache, long& addressIndexL2,vector<vector<unsigned long> >& TagArray_L2,vector<vector<unsigned long> >& Cache_L2,
	vector< vector<bitset<1>> >& ValidBit_L2, vector< vector<bitset<1>> >& DirtyBit_L2,unsigned long& Row_L3Cache, long& addressIndexL3, vector<vector<unsigned long> >& TagArray_L3,
	vector<vector<unsigned long> >& Cache_L3,vector< vector<bitset<1>> >& ValidBit_L3, vector< vector<bitset<1>> >& DirtyBit_L3, string type,
	unsigned long& addressOffset,vector< vector<unsigned long> >& CounterLRU_L1, vector< vector<unsigned long> >& CounterLRU_L2,vector< vector<unsigned long> >& CounterLRU_L3,
	vector <struct Node* >& root1, vector <struct Node* >& root2, vector <struct Node* >& root3,vector<bitset<1> >& arr1, vector<bitset<1> >& arr2, vector<bitset<1> >& arr3,
	vector< vector< long>>& openPage, long row, long bnak, long column, long columnSize, int OffsetSize,long& CyclesSimulator,int bus, config config_param) {

	int n, n2, n3;
	unsigned long tempLocation, tempLocation3;
	vector<unsigned long> tempBlock, tempblock3;
	int TagInRow;

	// set on L1 cache
	CyclesSimulator = CyclesSimulator + config_param.L1_Cycles;
	if (!setBlockInEmptyCache(block, Row_L1Cache, config_param.NWaySetAssociativeCacheL1, addressTagMemory,
		addressIndexL1, TagArray_L1, Cache_L1, config_param.blocksize, ValidBit_L1, DirtyBit_L1, type, CounterLRU_L1, root1, arr1, config_param.ReplacementPolicy)) {

		CyclesSimulator = CyclesSimulator + config_param.L2_Cycles * bus;

		TagInRow = (int)addressIndexL1 / Row_L1Cache;
		root1[TagInRow] = UpDatePLRUArray(root1[TagInRow], arr1, config_param.NWaySetAssociativeCacheL1);

		int location = checkReplacementPolicy(Row_L1Cache, addressIndexL1, config_param.NWaySetAssociativeCacheL1, addressTagMemory,
			ValidBit_L1, config_param.ReplacementPolicy, CounterLRU_L1, arr1);

		if (checkDirtyBit(ValidBit_L1, DirtyBit_L1, addressIndexL1, Row_L1Cache, location)) {
			// return true if dirty bit is 1
			// get block

			n = addressIndexL1 / Row_L1Cache;
			tempLocation = TagArray_L1[n][location];
			tempBlock = setBlockAndGetOldBlock(block, Row_L1Cache, config_param.NWaySetAssociativeCacheL1, addressTagMemory,
				addressIndexL1, TagArray_L1, Cache_L1, config_param.blocksize, ValidBit_L1, DirtyBit_L1, location, CounterLRU_L1,
				root1, arr1, config_param.ReplacementPolicy);




			// set on L2 cache
			if (!setBlockInEmptyCache(tempBlock, Row_L2Cache, config_param.NWaySetAssociativeCacheL2, tempLocation,
				addressIndexL2, TagArray_L2, Cache_L2, config_param.blocksize, ValidBit_L2, DirtyBit_L2, type, CounterLRU_L2, root2, arr2, config_param.ReplacementPolicy)) {

				CyclesSimulator = CyclesSimulator + config_param.L3_Cycles * bus;


				TagInRow = (int)addressIndexL2 / Row_L2Cache;
				root2[TagInRow] = UpDatePLRUArray(root2[TagInRow], arr2, config_param.NWaySetAssociativeCacheL2);

				int location = checkReplacementPolicy(Row_L2Cache, addressIndexL2, config_param.NWaySetAssociativeCacheL2, addressTagMemory,
					ValidBit_L2, config_param.ReplacementPolicy, CounterLRU_L2, arr2);


				if (!UpDateBlock(tempBlock, Row_L2Cache, config_param.NWaySetAssociativeCacheL2, tempLocation,
					addressIndexL2, TagArray_L2, Cache_L2, config_param.blocksize, ValidBit_L2, DirtyBit_L2)) {
					// return true if scssecful
					// else return false
					if (checkDirtyBit(ValidBit_L2, DirtyBit_L2, addressIndexL2, Row_L2Cache, location)) {
						// return true if dirty bit is 1
						// get block

						n3 = addressIndexL2 / Row_L2Cache;
						tempLocation3 = TagArray_L2[n][location];
						tempblock3 = setBlockAndGetOldBlock(block, Row_L2Cache, config_param.NWaySetAssociativeCacheL2, addressTagMemory,
							addressIndexL2, TagArray_L2, Cache_L2, config_param.blocksize, ValidBit_L2, DirtyBit_L2, location, CounterLRU_L2,
							root2, arr2, config_param.ReplacementPolicy);

						// set on L3 cache
						if (!setBlockInEmptyCache(tempblock3, Row_L3Cache, config_param.NWaySetAssociativeCacheL3, addressTagMemory,
							addressIndexL3, TagArray_L3, Cache_L3, config_param.blocksize, ValidBit_L3, DirtyBit_L3, type, CounterLRU_L3, root3, arr3, config_param.ReplacementPolicy)) {

							TagInRow = (int)addressIndexL3 / Row_L3Cache;
							root3[TagInRow] = UpDatePLRUArray(root3[TagInRow], arr3, config_param.NWaySetAssociativeCacheL3);

							int location = checkReplacementPolicy(Row_L3Cache, addressIndexL3, config_param.NWaySetAssociativeCacheL3, addressTagMemory,
								ValidBit_L3, config_param.ReplacementPolicy, CounterLRU_L3, arr3);

							if (!UpDateBlock(tempblock3, Row_L3Cache, config_param.NWaySetAssociativeCacheL3, tempLocation3,
								addressIndexL3, TagArray_L3, Cache_L3, config_param.blocksize, ValidBit_L3, DirtyBit_L3)) {
								// return true if scssecful
								// else return false

								if (checkDirtyBit(ValidBit_L3, DirtyBit_L3, addressIndexL3, Row_L3Cache, location)) {
									// return true if dirty bit is 1
									// get block


									tempBlock = setBlockAndGetOldBlock(tempblock3, Row_L3Cache, config_param.NWaySetAssociativeCacheL3, addressTagMemory,
										addressIndexL3, TagArray_L3, Cache_L3, config_param.blocksize, ValidBit_L3, DirtyBit_L3, location, CounterLRU_L3,
										root3, arr3, config_param.ReplacementPolicy);

									//									upDatePage(openPage,  addressOffset  , OffsetSize);
									upDatePage(openPage, addressOffset, OffsetSize,
										config_param.DDR_Cycles_PageAndColumnIsOpen, config_param.DDR_Cycles_PageOpen, config_param.DDR_Cycles_PageNotOpen, CyclesSimulator, config_param.AddressMapping);

								}

								else
								{
									deleteBlock(DirtyBit_L3, ValidBit_L3, Cache_L3,
										location, Row_L3Cache, config_param.NWaySetAssociativeCacheL3, addressIndexL3);

									setBlock(tempblock3, Row_L3Cache, config_param.NWaySetAssociativeCacheL3, tempLocation3,
										addressIndexL3, TagArray_L3, Cache_L3, config_param.blocksize, ValidBit_L3, DirtyBit_L3, "R", CounterLRU_L3, root3, arr3, config_param.ReplacementPolicy);
								}

							}

						}

					}
					else
					{
						//						CyclesSimulator = CyclesSimulator + L3_Cycles * bus;
						deleteBlock(DirtyBit_L2, ValidBit_L2, Cache_L2,
							location, Row_L2Cache, config_param.NWaySetAssociativeCacheL2, addressIndexL2);

						setBlock(tempBlock, Row_L2Cache, config_param.NWaySetAssociativeCacheL2, tempLocation,
							addressIndexL2, TagArray_L2, Cache_L2, config_param.blocksize, ValidBit_L2, DirtyBit_L2, type, CounterLRU_L2, root2, arr2, config_param.ReplacementPolicy);
					}

				}

			}

		}
		else
		{
			//			CyclesSimulator = CyclesSimulator + L2_Cycles * bus;

			deleteBlock(DirtyBit_L1, ValidBit_L1, Cache_L1,
				location, Row_L1Cache, config_param.NWaySetAssociativeCacheL1, addressIndexL1);

			setBlock(block, Row_L1Cache, config_param.NWaySetAssociativeCacheL1, addressTagMemory,
				addressIndexL1, TagArray_L1, Cache_L1, config_param.blocksize, ValidBit_L1, DirtyBit_L1, type, CounterLRU_L1, root1, arr1, config_param.ReplacementPolicy);
		}

	}

}

void ResetDirtyBit(vector< vector<bitset<1>> >& dirtyBit1, vector< vector<bitset<1>> >& dirtyBit2,
	vector< vector<bitset<1>> >& dirtyBit3, unsigned long& Way1, unsigned long& Way2, unsigned long& Way3, unsigned long Row_L1Cache,
	unsigned long Row_L2Cache, unsigned long Row_L3Cache) {


	for (int i = 0; i < Row_L1Cache; i++)
	{
		for (int j = 0; j < Way1; j++)
		{
			dirtyBit1[i][j] = false;
		}
	}
	for (int i = 0; i < Row_L2Cache; i++)
	{
		for (int j = 0; j < Way2; j++)
		{
			dirtyBit2[i][j] = false;
		}
	}
	for (int i = 0; i < Row_L3Cache; i++)
	{
		for (int j = 0; j < Way3; j++)
		{
			dirtyBit3[i][j] = false;
		}
	}
}


void writeAllocate(vector<unsigned long>& block, unsigned long& Row_L1Cache, unsigned long& addressTagMemory, long& addressIndexL1, vector<vector<unsigned long> >& TagArray_L1, vector<vector<unsigned long> >& Cache_L1, vector< vector<bitset<1>> >& ValidBit_L1, vector< vector<bitset<1>> >& DirtyBit_L1,
	unsigned long& Row_L2Cache, long& addressIndexL2, vector<vector<unsigned long> >& TagArray_L2, vector<vector<unsigned long> >& Cache_L2, vector< vector<bitset<1>> >& ValidBit_L2, vector< vector<bitset<1>> >& DirtyBit_L2,
	unsigned long& Row_L3Cache, long& addressIndexL3, vector<vector<unsigned long> >& TagArray_L3, vector<vector<unsigned long> >& Cache_L3, vector< vector<bitset<1>> >& ValidBit_L3, vector< vector<bitset<1>> >& DirtyBit_L3, string type, long& addressOffset,
	vector< vector<unsigned long> >& CounterLRU_L1, vector< vector<unsigned long> >& CounterLRU_L2, vector< vector<unsigned long> >& CounterLRU_L3, vector <struct Node* >& root1, vector <struct Node* >& root2, vector <struct Node* >& root3,
	vector<bitset<1> >& arr1, vector<bitset<1> >& arr2, vector<bitset<1> >& arr3, unsigned long hit_L1, unsigned long hit_L2, unsigned long hit_L3,
	WritePolicy WritePolicy, vector< vector< long>>& openPage, long row, long bnak, long column, long columnSize, int OffsetSize, long& CyclesSimulator, int bus, config config_param) {


	int TagInRow = 0;
	int n, n2, n3;
	unsigned long tempLocation, tempLocation2, tempLocation3;
	vector<unsigned long> tempBlock;

	if (config_param.WritePolicy == WritePolicy::WriteThrough) {
		ResetDirtyBit(DirtyBit_L1, DirtyBit_L2, DirtyBit_L3,
			config_param.NWaySetAssociativeCacheL1, config_param.NWaySetAssociativeCacheL1, config_param.NWaySetAssociativeCacheL1,
			Row_L1Cache, Row_L2Cache, Row_L3Cache);
	}
	if (hit_L1 == 0) {
		// UpDate the Cache with the new block in the L1_cache L2_cache L3_cache 
		UpDateCache(block, Row_L1Cache, addressTagMemory,
			addressIndexL1, TagArray_L1, Cache_L1, ValidBit_L1, DirtyBit_L1,
			Row_L2Cache, addressIndexL2, TagArray_L2, Cache_L2, ValidBit_L2, DirtyBit_L2,
			Row_L3Cache, addressIndexL3, TagArray_L3, Cache_L3, ValidBit_L3, DirtyBit_L3, type,
			addressTagMemory, CounterLRU_L1, CounterLRU_L2, CounterLRU_L3
			, root1, root2, root3, arr1, arr2, arr3,
			openPage, row, bnak, column, columnSize, OffsetSize, CyclesSimulator, bus, config_param);
	}

	if (WritePolicy == WritePolicy::WriteThrough) {
		ResetDirtyBit(DirtyBit_L1, DirtyBit_L2, DirtyBit_L3,
			config_param.NWaySetAssociativeCacheL1, config_param.NWaySetAssociativeCacheL2, config_param.NWaySetAssociativeCacheL3,
			Row_L1Cache, Row_L2Cache, Row_L3Cache);
	}
	if (hit_L1 == 0 && hit_L2 == 0)
	{

		// set on L2 cache
		CyclesSimulator = CyclesSimulator + config_param.L2_Cycles;
		if (!setBlockInEmptyCache(block, Row_L2Cache, config_param.NWaySetAssociativeCacheL2, addressTagMemory,
			addressIndexL2, TagArray_L2, Cache_L2, config_param.blocksize, ValidBit_L2, DirtyBit_L2, type, CounterLRU_L2, root2, arr2, config_param.ReplacementPolicy)) {

			CyclesSimulator = CyclesSimulator + config_param.L3_Cycles * bus;

			TagInRow = (int)addressIndexL2 / Row_L2Cache;
			root2[TagInRow] = UpDatePLRUArray(root2[TagInRow], arr2, config_param.NWaySetAssociativeCacheL2);

			int location = checkReplacementPolicy(Row_L2Cache, addressIndexL2, config_param.NWaySetAssociativeCacheL2, addressTagMemory,
				ValidBit_L2, config_param.ReplacementPolicy, CounterLRU_L2, arr2);

			if (checkDirtyBit(ValidBit_L2, DirtyBit_L2, addressIndexL2, Row_L2Cache, location)) {
				// return true if dirty bit is 1
				// get block
				n = addressIndexL2 / Row_L2Cache;
				tempLocation3 = TagArray_L2[n][location];
				tempBlock = setBlockAndGetOldBlock(block, Row_L2Cache, config_param.NWaySetAssociativeCacheL2, addressTagMemory,
					addressIndexL2, TagArray_L2, Cache_L2, config_param.blocksize, ValidBit_L2, DirtyBit_L2, location, CounterLRU_L2,
					root2, arr2, config_param.ReplacementPolicy);

				if (!setBlock(tempBlock, Row_L3Cache, config_param.NWaySetAssociativeCacheL3, tempLocation3,
					addressIndexL3, TagArray_L3, Cache_L3, config_param.blocksize, ValidBit_L3, DirtyBit_L3, type, CounterLRU_L3, root3, arr3, config_param.ReplacementPolicy)) {

					TagInRow = (int)addressIndexL3 / Row_L3Cache;

					if (!UpDateBlock(tempBlock, Row_L3Cache, config_param.NWaySetAssociativeCacheL3, tempLocation3,
						addressIndexL3, TagArray_L3, Cache_L3, config_param.blocksize, ValidBit_L3, DirtyBit_L3)) {
						// return true if scssecful
						// else return false

						root3[TagInRow] = UpDatePLRUArray(root3[TagInRow], arr3, config_param.NWaySetAssociativeCacheL3);

						int location = checkReplacementPolicy(Row_L3Cache, addressIndexL3, config_param.NWaySetAssociativeCacheL3, addressTagMemory,
							ValidBit_L3, config_param.ReplacementPolicy, CounterLRU_L3, arr3);

						if (checkDirtyBit(ValidBit_L3, DirtyBit_L3, addressIndexL3, Row_L3Cache, location)) {
							// return true if dirty bit is 1
							// get block

							block = setBlockAndGetOldBlock(tempBlock, Row_L3Cache, config_param.NWaySetAssociativeCacheL3, addressTagMemory,
								addressIndexL3, TagArray_L3, Cache_L3, config_param.blocksize, ValidBit_L3, DirtyBit_L3, location, CounterLRU_L3,
								root3, arr3, config_param.ReplacementPolicy);
							upDatePage(openPage, addressTagMemory, OffsetSize,
								config_param.DDR_Cycles_PageAndColumnIsOpen, config_param.DDR_Cycles_PageOpen, config_param.DDR_Cycles_PageNotOpen, CyclesSimulator, config_param.AddressMapping);


						}

						else
						{
							deleteBlock(DirtyBit_L3, ValidBit_L3, Cache_L3,
								location, Row_L3Cache, config_param.blocksize, addressIndexL3);

							setBlock(tempBlock, Row_L3Cache, config_param.NWaySetAssociativeCacheL3, tempLocation3,
								addressIndexL3, TagArray_L3, Cache_L3, config_param.blocksize, ValidBit_L3, DirtyBit_L3, type, CounterLRU_L3, root3, arr3, config_param.ReplacementPolicy);
						}

					}
				}
			}
			else
			{
				deleteBlock(DirtyBit_L2, ValidBit_L2, Cache_L2,
					location, Row_L2Cache, config_param.blocksize, addressIndexL2);

				setBlock(block, Row_L2Cache, config_param.NWaySetAssociativeCacheL2, addressTagMemory,
					addressIndexL2, TagArray_L2, Cache_L2, config_param.blocksize, ValidBit_L2, DirtyBit_L2, type, CounterLRU_L2, root2, arr2, config_param.ReplacementPolicy);
			}



		}


	}

	if (WritePolicy == WritePolicy::WriteThrough) {
		ResetDirtyBit(DirtyBit_L1, DirtyBit_L2, DirtyBit_L3,
			config_param.NWaySetAssociativeCacheL1, config_param.NWaySetAssociativeCacheL2, config_param.NWaySetAssociativeCacheL3,
			Row_L1Cache, Row_L2Cache, Row_L3Cache);
	}
	if (hit_L1 == 0 && hit_L2 == 0 && hit_L3 == 0)
	{

		// set on L3 cache
		CyclesSimulator = CyclesSimulator + config_param.L3_Cycles;
		if (!setBlockInEmptyCache(block, Row_L3Cache, config_param.NWaySetAssociativeCacheL3, addressTagMemory,
			addressIndexL3, TagArray_L3, Cache_L3, config_param.blocksize, ValidBit_L3, DirtyBit_L3, type, CounterLRU_L3, root3, arr3, config_param.ReplacementPolicy)) {

			TagInRow = (int)addressIndexL3 / Row_L3Cache;
			root3[TagInRow] = UpDatePLRUArray(root3[TagInRow], arr3, config_param.NWaySetAssociativeCacheL3);

			int location = checkReplacementPolicy(Row_L3Cache, addressIndexL3, config_param.NWaySetAssociativeCacheL3, addressTagMemory,
				ValidBit_L3, config_param.ReplacementPolicy, CounterLRU_L3, arr3);

			if (checkDirtyBit(ValidBit_L3, DirtyBit_L3, addressIndexL3, Row_L3Cache, location)) {
				// return true if dirty bit is 1
				// get block

				n = addressIndexL3 / Row_L3Cache;
				tempLocation3 = TagArray_L3[n][location];
				tempBlock = setBlockAndGetOldBlock(block, Row_L3Cache, config_param.NWaySetAssociativeCacheL3, addressTagMemory,
					addressIndexL3, TagArray_L3, Cache_L3, config_param.blocksize, ValidBit_L3, DirtyBit_L3, location, CounterLRU_L3,
					root3, arr3, config_param.ReplacementPolicy);

				upDatePage(openPage, tempLocation3, OffsetSize,
					config_param.DDR_Cycles_PageAndColumnIsOpen, config_param.DDR_Cycles_PageOpen, config_param.DDR_Cycles_PageNotOpen, CyclesSimulator, config_param.AddressMapping);

			}
			else
			{

				deleteBlock(DirtyBit_L3, ValidBit_L3, Cache_L3,
					location, Row_L3Cache, config_param.blocksize, addressIndexL3);

				setBlock(block, Row_L3Cache, config_param.NWaySetAssociativeCacheL3, addressTagMemory,
					addressIndexL3, TagArray_L3, Cache_L3, config_param.blocksize, ValidBit_L3, DirtyBit_L3, type, CounterLRU_L3, root3, arr3, config_param.ReplacementPolicy);
			}

		}


	}

}



void writeThrough(vector<unsigned long>& block, unsigned long& Row_L1Cache,
	unsigned long& addressTagMemory, long& addressIndexL1,
	vector<vector<unsigned long> >& TagArray_L1,
	vector<vector<unsigned long> >& Cache_L1,
	vector< vector<bitset<1>> >& ValidBit_L1, vector< vector<bitset<1>> >& DirtyBit_L1,

	unsigned long& Row_L2Cache, long& addressIndexL2,
	vector<vector<unsigned long> >& TagArray_L2,
	vector<vector<unsigned long> >& Cache_L2,
	vector< vector<bitset<1>> >& ValidBit_L2, vector< vector<bitset<1>> >& DirtyBit_L2,

	unsigned long& Row_L3Cache, long& addressIndexL3, vector<vector<unsigned long> >& TagArray_L3,
	vector<vector<unsigned long> >& Cache_L3,
	vector< vector<bitset<1>> >& ValidBit_L3, vector< vector<bitset<1>> >& DirtyBit_L3, string type,
	long& addressOffset,
	vector< vector<unsigned long> >& CounterLRU_L1, vector< vector<unsigned long> >& CounterLRU_L2,
	vector< vector<unsigned long> >& CounterLRU_L3,
	vector <struct Node* >& root1, vector <struct Node* >& root2, vector <struct Node* >& root3,
	vector<bitset<1> >& arr1, vector<bitset<1> >& arr2, vector<bitset<1> >& arr3,
	unsigned long hit_L1, unsigned long hit_L2, unsigned long hit_L3,
	vector< vector< long>>& openPage, long row, long bnak, long column, long columnSize, int OffsetSize,
	long& CyclesSimulator,
	int& bus, config configParam) {

	// write through
	int TagInRow = 0;
	int location;


	if (hit_L1)
	{
		UpDateCacheWT(Row_L1Cache, configParam.NWaySetAssociativeCacheL1,
			addressTagMemory, addressIndexL1, TagArray_L1, Cache_L1, configParam.blocksize, addressOffset,
			CyclesSimulator, configParam.L1_Cycles);
	}
	if (hit_L1 || hit_L2)
	{
		UpDateCacheWT(Row_L2Cache, configParam.NWaySetAssociativeCacheL2,
			addressTagMemory, addressIndexL2, TagArray_L2, Cache_L2, configParam.blocksize, addressOffset,
			CyclesSimulator, configParam.L2_Cycles);
	}
	if (hit_L1 || hit_L2 || hit_L3)
	{
		UpDateCacheWT(Row_L3Cache, configParam.NWaySetAssociativeCacheL3,
			addressTagMemory, addressIndexL3, TagArray_L3, Cache_L3, configParam.blocksize, addressOffset,
			CyclesSimulator, configParam.L3_Cycles);
	}



	upDatePage(openPage, addressTagMemory, OffsetSize,
		configParam.DDR_Cycles_PageAndColumnIsOpen, configParam.DDR_Cycles_PageOpen, configParam.DDR_Cycles_PageNotOpen, CyclesSimulator, configParam.AddressMapping);






}

