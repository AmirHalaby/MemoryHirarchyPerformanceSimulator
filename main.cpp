#include "memorySimulator.h"

int main(int argc, char* argv[]) {

		ConverTrace converTrace;
		config cacheconfig;
		ifstream cache_params; //ifstream  מסוג  cache_params יוצרת אוביקט בשם 
		string dummyLine;
		cache_params.open("C:\\Users\\AA\\source\\repos\\MemoryHirarchyPerformanceSimulator\\MemoryHirarchyPerformanceSimulator\\cacheconfig.txt");
		unsigned int tmp;
		while (!cache_params.eof())  // read config file ifstream 
	{
			cache_params >> dummyLine;
			cache_params >> cacheconfig.blocksize;

			cache_params >> dummyLine;
			cache_params >> cacheconfig.L1CacheSize;
			cache_params >> cacheconfig.L2CacheSize;
			cache_params >> cacheconfig.L3CacheSize;

			cache_params >> dummyLine;
			cache_params >> cacheconfig.NWaySetAssociativeCacheL1;
			cache_params >> dummyLine;
			cache_params >> cacheconfig.NWaySetAssociativeCacheL2;
			cache_params >> dummyLine;
			cache_params >> cacheconfig.NWaySetAssociativeCacheL3;

			cache_params >> dummyLine;
			cache_params >> tmp;
			// 0 is write No Allocate ;
			// 1 is write  Allocate
			cacheconfig.writeAllocatePolicy = static_cast<writeAllocatePolicy>(tmp);

			cache_params >> dummyLine;
			cache_params >> tmp;
			// 0 is write through 
			// 1 is write Back  
			cacheconfig.WritePolicy = static_cast<WritePolicy>(tmp);

			cache_params >> dummyLine;
			cache_params >> tmp;
			// in 0 the Replacement Policy is Random  
			// in 1 the Replacement Policy is LRU  
			// in 2 the Replacement Policy is PseudoLRU 
			cacheconfig.ReplacementPolicy = static_cast<ReplacementPolicy>(tmp);
			cache_params >> dummyLine;
			cache_params >> cacheconfig.L1_Cycles;
			cache_params >> cacheconfig.L2_Cycles;
			cache_params >> cacheconfig.L3_Cycles;
			cache_params >> dummyLine;
			cache_params >> cacheconfig.DDR_Cycles_PageAndColumnIsOpen;
			cache_params >> dummyLine;
			cache_params >> cacheconfig.DDR_Cycles_PageOpen;
			cache_params >> dummyLine;
			cache_params >> cacheconfig.DDR_Cycles_PageNotOpen;
			cache_params >> dummyLine;
			cache_params >> cacheconfig.bus;

			cache_params >> dummyLine;
			cache_params >> tmp;
			// in 0 the AddressMapping Policy is RowInter leaving  
			// in 1 the AddressMapping Policy is CacheBlock Inter leaving  
			cacheconfig.AddressMapping = static_cast<AddressMapping>(tmp);
		}
		// 0 is write No Allocate ;
		// 1 is write  Allocate
		// in 0 the Replacement Policy is Random  
		// in 1 the Replacement Policy is LRU  
		// in 2 the Replacement Policy is PseudoLRU 
		// 0 is write through 
		// 1 is write Back  
		// in 0 the AddressMapping Policy is RowInter leaving  
		// in 1 the AddressMapping Policy is CacheBlock Inter leaving  



		Cache cacheObj(cacheconfig);
		mainMemory memoryObj1(cacheconfig);

		ifstream traces;
		ofstream tracesout;
		string outname;
		outname = string("C:\\Users\\AA\\source\\repos\\MemoryHirarchyPerformanceSimulator\\MemoryHirarchyPerformanceSimulator\\traceout.txt");
		traces.open("C:\\Users\\AA\\source\\repos\\MemoryHirarchyPerformanceSimulator\\MemoryHirarchyPerformanceSimulator\\AddressAndTypeFile.txt");
		tracesout.open(outname.c_str());
		string line;
		string accesstype;  // the Read/Write access type from the memory trace;
		string xaddr;       // the address from the memory trace store in hex;
		unsigned int addr;  // the address from the memory trace store in unsigned int;
		bitset<32> accessaddr; // the address from the memory trace store in the bitset;

		int L1AcceState = 0; // L1 access state variable, can be one of NA, RH, RM, WH, WM
		int L2AcceState = 0; // L2 access state variable, can be one of NA, RH, RM, WH, WM
		int L3AcceState = 0; // L3 access state variable, can be one of NA, RH, RM, WH, WM
		int MemoryAcceState = 0; // Main Memory access state variable, can be one of NA, RH, RM, WH, WM
		int location = 0;

		vector< vector<unsigned long> > Cache_L1 = cacheObj.getCache_L1();
		vector< vector<unsigned long> > Cache_L2 = cacheObj.getCache_L2();
		vector< vector<unsigned long> > Cache_L3 = cacheObj.getCache_L3();

		vector< vector<unsigned long> > TagArray_L1 = cacheObj.getTagArray_L1();
		vector< vector<unsigned long> > TagArray_L2 = cacheObj.getTagArray_L2();
		vector< vector<unsigned long> > TagArray_L3 = cacheObj.getTagArray_L3();
		int bus1 = cacheconfig.blocksize / (cacheconfig.bus * 8);


		//	vector< vector<unsigned long> > memory = memoryObj.getMemory();
		//	vector<unsigned long>  TagArrayMemory = memoryObj.GetTagArrayMemory();

			// create the pages mode and update the row and the column to -1
		vector< vector< long>> openPage = memoryObj1.getPage();

		long columnSize = memoryObj1.getCoulmnSize();
		long Taccess = 0;
		long L1HitCounter = 0;
		long L2HitCounter = 0;
		long L3HitCounter = 0;
		long DDRHitCounter = 0;
		long L1MissCounter = 0;
		long L2MissCounter = 0;
		long L3MissCounter = 0;

		//	if (cacheconfig.WritePolicy == WritePolicy::WriteBack) {

		vector< vector<bitset<1>> > DirtyBit_L1 = cacheObj.getDirtyBit_L1();
		vector< vector<bitset<1>> > DirtyBit_L2 = cacheObj.getDirtyBit_L2();
		vector< vector<bitset<1>> > DirtyBit_L3 = cacheObj.getDirtyBit_L3();
		//}
		vector< vector<bitset<1>> > ValidBit_L1 = cacheObj.getValidBit_L1();
		vector< vector<bitset<1>> > ValidBit_L2 = cacheObj.getValidBit_L2();
		vector< vector<bitset<1>> > ValidBit_L3 = cacheObj.getValidBit_L3();

		vector< vector<unsigned long> > CounterLRU_L1 = cacheObj.getTagArray_L1();
		vector< vector<unsigned long> > CounterLRU_L2 = cacheObj.getTagArray_L2();
		vector< vector<unsigned long> > CounterLRU_L3 = cacheObj.getTagArray_L3();

		vector < Node* > CounterPLRU_L1;
		vector < Node* > CounterPLRU_L2;
		vector < Node* > CounterPLRU_L3;

		vector <bitset<1>> tempPLPU1;
		vector <bitset<1>> tempPLPU2;
		vector <bitset<1>> tempPLPU3;

		tempPLPU1.resize(log2(cacheconfig.NWaySetAssociativeCacheL1));
		tempPLPU2.resize(log2(cacheconfig.NWaySetAssociativeCacheL2));
		tempPLPU3.resize(log2(cacheconfig.NWaySetAssociativeCacheL3));

		unsigned long Row_L1Cache = cacheObj.getRow_L1Cache();
		unsigned long Row_L2Cache = cacheObj.getRow_L2Cache();
		unsigned long Row_L3Cache = cacheObj.getRow_L3Cache();


		firstDateLRU(CounterLRU_L1, CounterLRU_L2, CounterLRU_L3,
			cacheconfig.NWaySetAssociativeCacheL1, cacheconfig.NWaySetAssociativeCacheL2, cacheconfig.NWaySetAssociativeCacheL3
			, Row_L1Cache, Row_L2Cache, Row_L3Cache);

		createTreePLRU(CounterPLRU_L1, CounterPLRU_L2, CounterPLRU_L3, Row_L1Cache, Row_L2Cache, Row_L3Cache,
			cacheconfig.NWaySetAssociativeCacheL1, cacheconfig.NWaySetAssociativeCacheL2, cacheconfig.NWaySetAssociativeCacheL3);

		int count = 0;


		if (traces.is_open() && tracesout.is_open()) {
			while (getline(traces, line)) {   // read mem access file and access Cache
				count++;

				istringstream iss(line);
				if (!(iss >> accesstype >> xaddr)) { break; }
				stringstream saddr(xaddr);
				saddr >> std::hex >> addr;
				unsigned long hit_L1 = 0, hit_L2 = 0, hit_L3 = 0;
				accessaddr = bitset<32>(addr);
				vector<string> bits = cacheObj.getBits(accessaddr);
				string strTagL1 = bits[0];
				string strIndexL1 = bits[1];
				string strOffsetL1 = bits[2];
				string strTagL2 = bits[3];
				string strIndexL2 = bits[4];
				string strOffsetL2 = bits[5];
				string strTagL3 = bits[6];
				string strIndexL3 = bits[7];
				string strOffsetL3 = bits[8];

				vector<string> bits1 = memoryObj1.getBits1(accessaddr);
				string strTagMemory = bits1[0];
				string strOffsetMemory = bits1[1];
				int OffsetSize = strOffsetMemory.length();
				vector<string> bitsMem = memoryObj1.getBits(accessaddr);
				string strRow = bitsMem[0];
				string strChannel = bitsMem[1];
				string strBank = bitsMem[2];
				string strColumn = bitsMem[3];
				string strByteInBus = bitsMem[4];
				strBank = strChannel + strBank;
				int n, n2, n3;
				unsigned long tempLocation, tempLocation2, tempLocation3;
				long addressIndexL1, addressIndexL2, addressIndexL3;
				vector<unsigned long> block;
				vector<unsigned long> tempBlock;
				char* ptr;
				long  addressTag_L1 = strtol(strTagL1.c_str(), &ptr, 2);
				long  addressTag_L2 = strtol(strTagL2.c_str(), &ptr, 2);
				long  addressTag_L3 = strtol(strTagL3.c_str(), &ptr, 2);
				unsigned long  addressTagMemory = strtol(strTagMemory.c_str(), &ptr, 2);
				long  Offset = strtol(strOffsetL1.c_str(), &ptr, 2);
				long  row = strtol(strRow.c_str(), &ptr, 2);

				long  column = strtol(strColumn.c_str(), &ptr, 2);
				long  bank = strtol(strBank.c_str(), &ptr, 2);
				int TagInRow;
				if (strIndexL1 != "") {
					addressIndexL1 = strtol(strIndexL1.c_str(), &ptr, 2);
				}
				else {
					addressIndexL1 = 0;
				}
				if (strIndexL2 != "") {
					addressIndexL2 = strtol(strIndexL2.c_str(), &ptr, 2);
				}
				else {
					addressIndexL2 = 0;
				}
				if (strIndexL3 != "") {
					addressIndexL3 = strtol(strIndexL3.c_str(), &ptr, 2);
				}
				else {
					addressIndexL3 = 0;
				}

				if (searchInTagArray_L(Row_L1Cache, cacheconfig.NWaySetAssociativeCacheL1, addressTagMemory, addressIndexL1, TagArray_L1))
				{
					// hit
					hit_L1 = 1;

				}
				else
				{
					//miss
					hit_L1 = 0;
				}
				if (hit_L1 == 0) {
					// goto L2
					if (searchInTagArray_L(Row_L2Cache, cacheconfig.NWaySetAssociativeCacheL2, addressTagMemory, addressIndexL2, TagArray_L2))
					{
						// hit
						hit_L2 = 1;
					}
				}
				else
				{
					//miss
					hit_L2 = 0;

				}
				if (hit_L2 == 0 && hit_L1 == 0) {
					// goto L2

					if (searchInTagArray_L(Row_L3Cache, cacheconfig.NWaySetAssociativeCacheL3, addressTagMemory, addressIndexL3, TagArray_L3))
					{
						// hit
						hit_L3 = 1;

					}

				}
				else
				{
					//miss
					hit_L3 = 0;

				}

				// access the L1 and L2 and L3 Cache according to the trace;
				if (accesstype.compare("R") == 0) {
					//Implement by you:
					// read access to the L1 Cache,
					//  and then L2 ,
					//  and then L3 (if required),
					//  update the L1 and L2 and L3 access state variable;
					if (hit_L1 == 0 && hit_L2 == 0 && hit_L3 == 0) {

						DDRHitCounter++;
						L1MissCounter++;
						L2MissCounter++;
						L3MissCounter++;
						L1AcceState = RM;
						L2AcceState = RM;
						L3AcceState = RM;

						// get block from main memory and update on cacheL1;

						block = getBlockFromMainMemory(cacheconfig.blocksize, addressTagMemory, Offset, openPage, OffsetSize,
							cacheconfig.DDR_Cycles_PageAndColumnIsOpen, cacheconfig.DDR_Cycles_PageOpen, cacheconfig.DDR_Cycles_PageNotOpen, Taccess, cacheconfig.AddressMapping);

						writeAllocate(block, Row_L1Cache, addressTagMemory, addressIndexL1, TagArray_L1, Cache_L1,
							ValidBit_L1, DirtyBit_L1, Row_L2Cache, addressIndexL2, TagArray_L2, Cache_L2, ValidBit_L2, DirtyBit_L2,
							Row_L3Cache, addressIndexL3, TagArray_L3, Cache_L3, ValidBit_L3, DirtyBit_L3, "R", Offset, CounterLRU_L1, CounterLRU_L2, CounterLRU_L3
							, CounterPLRU_L1, CounterPLRU_L2, CounterPLRU_L3, tempPLPU1, tempPLPU2, tempPLPU3, hit_L1, hit_L2, hit_L3, cacheconfig.WritePolicy, openPage, row, bank, column, columnSize, OffsetSize, Taccess, bus1, cacheconfig);


					}
					else if (hit_L1 == 0 && hit_L2 == 0 && hit_L3 == 1) {

						L1MissCounter++;
						L2MissCounter++;
						L3HitCounter++;

						L1AcceState = RM;
						L2AcceState = RM;
						L3AcceState = RH;

						// get block from main memory and update on cacheL1
						block = getBlock(Row_L3Cache, cacheconfig.NWaySetAssociativeCacheL3, addressTagMemory,
							addressIndexL3, TagArray_L3, Cache_L3, cacheconfig.blocksize, CounterLRU_L3);


						writeAllocate(block, Row_L1Cache, addressTagMemory, addressIndexL1, TagArray_L1, Cache_L1, ValidBit_L1, DirtyBit_L1,
							Row_L2Cache, addressIndexL2, TagArray_L2, Cache_L2, ValidBit_L2, DirtyBit_L2, Row_L3Cache, addressIndexL3, TagArray_L3, Cache_L3, ValidBit_L3, DirtyBit_L3, "R", Offset, CounterLRU_L1, CounterLRU_L2, CounterLRU_L3
							, CounterPLRU_L1, CounterPLRU_L2, CounterPLRU_L3, tempPLPU1, tempPLPU2, tempPLPU3, hit_L1, hit_L2, hit_L3, cacheconfig.WritePolicy, openPage, row, bank, column, columnSize, OffsetSize, Taccess, bus1, cacheconfig);


					}
					else if (hit_L1 == 0 && hit_L2 == 1) {

						L1MissCounter++;
						L2HitCounter++;
						L1AcceState = RM;
						L2AcceState = RH;
						L3AcceState = NA;

						// get block from main memory and update on cacheL1


						block = getBlock(Row_L2Cache, cacheconfig.NWaySetAssociativeCacheL2, addressTagMemory,
							addressIndexL2, TagArray_L2, Cache_L2, cacheconfig.blocksize, CounterLRU_L2);

						writeAllocate(block, Row_L1Cache, addressTagMemory, addressIndexL1, TagArray_L1, Cache_L1, ValidBit_L1, DirtyBit_L1, Row_L2Cache, addressIndexL2, TagArray_L2, Cache_L2, ValidBit_L2, DirtyBit_L2,
							Row_L3Cache, addressIndexL3, TagArray_L3, Cache_L3, ValidBit_L3, DirtyBit_L3, "R", Offset, CounterLRU_L1, CounterLRU_L2, CounterLRU_L3
							, CounterPLRU_L1, CounterPLRU_L2, CounterPLRU_L3, tempPLPU1, tempPLPU2, tempPLPU3, hit_L1, hit_L2, hit_L3, cacheconfig.WritePolicy, openPage, row, bank, column, columnSize, OffsetSize, Taccess, bus1, cacheconfig);
						Taccess = Taccess + cacheconfig.L3_Cycles;


					}
					else if (hit_L1 == 1) {

						L1HitCounter++;
						L1AcceState = RH;
						L2AcceState = NA;
						L3AcceState = NA;

						Taccess = Taccess + cacheconfig.L1_Cycles;
						int TagInRow = (int)addressIndexL1 / Row_L1Cache;
						for (int i = 0; i < cacheconfig.NWaySetAssociativeCacheL1; i++)
						{
							if (ValidBit_L1[TagInRow][i] == false)
							{
								UpDateLRUArray(CounterLRU_L1, i, cacheconfig.NWaySetAssociativeCacheL1, addressIndexL1, Row_L1Cache);

							}

						}
					}

				}
				else {
					/**************write*********************write************************write********************************/
					/**************write*********************write************************write********************************/
					/**************write*********************write************************write********************************/
					/**************write*********************write************************write********************************/
					/**************write*********************write************************write********************************/
					//Implement by you:
					// write access to the L1 Cache,
					//and then L2 (if required),
					//and then L3 (if required),
					//update the L1 and L2 and L3 access state variable;
					if (hit_L1 == 0 && hit_L2 == 0 && hit_L3 == 0) {

						L1AcceState = WM;
						L2AcceState = WM;
						L3AcceState = WM;
						DDRHitCounter++;
						L1MissCounter++;
						L2MissCounter++;
						L3MissCounter++;

						//					Taccess = Taccess + cacheconfig.Memory_Cycles;

											// write no allocate
											// only write on Memory
						if (cacheconfig.writeAllocatePolicy == writeAllocatePolicy::writeNoAllocate) {

							upDatePage(openPage, addressTagMemory, OffsetSize,
								cacheconfig.DDR_Cycles_PageAndColumnIsOpen, cacheconfig.DDR_Cycles_PageOpen, cacheconfig.DDR_Cycles_PageNotOpen, Taccess
								, cacheconfig.AddressMapping);
						}
						//write allocate
						//  write on Memory
						// and write on L1 L2 L3 cache

						if (cacheconfig.writeAllocatePolicy == writeAllocatePolicy::WriteAllocate) {


							block = getBlockFromMainMemory(cacheconfig.blocksize, addressTagMemory, Offset, openPage, OffsetSize,
								cacheconfig.DDR_Cycles_PageAndColumnIsOpen, cacheconfig.DDR_Cycles_PageOpen, cacheconfig.DDR_Cycles_PageNotOpen, Taccess, cacheconfig.AddressMapping);

							// set block from memory to L1 L2 L3 cache
							// check  Replacement Policy 
							// UpDate the Cache with the new block in the L1_cache L2_cache L3_cache 

							writeAllocate(block, Row_L1Cache, addressTagMemory, addressIndexL1, TagArray_L1, Cache_L1, ValidBit_L1, DirtyBit_L1, Row_L2Cache, addressIndexL2, TagArray_L2, Cache_L2, ValidBit_L2, DirtyBit_L2,
								Row_L3Cache, addressIndexL3, TagArray_L3, Cache_L3, ValidBit_L3, DirtyBit_L3, "W", Offset, CounterLRU_L1, CounterLRU_L2, CounterLRU_L3
								, CounterPLRU_L1, CounterPLRU_L2, CounterPLRU_L3, tempPLPU1, tempPLPU2, tempPLPU3, hit_L1, hit_L2, hit_L3, cacheconfig.WritePolicy, openPage, row, bank, column, columnSize, OffsetSize, Taccess, bus1, cacheconfig);
						}
					}
					else if (hit_L1 == 0 && hit_L2 == 0 && hit_L3 == 1) {

						L1AcceState = WM;
						L2AcceState = WM;
						L3AcceState = WH;
						L1MissCounter++;
						L2MissCounter++;
						L3HitCounter++;


						// write back

						if (cacheconfig.WritePolicy == WritePolicy::WriteBack) {

							UpDateCacheWB(Row_L3Cache, cacheconfig.NWaySetAssociativeCacheL3,
								addressTagMemory, addressIndexL3, TagArray_L3, DirtyBit_L3, Cache_L3, cacheconfig.blocksize, Offset,
								cacheconfig.L3_Cycles, Taccess);

							block = getBlock(Row_L3Cache, cacheconfig.NWaySetAssociativeCacheL3, addressTagMemory,
								addressIndexL3, TagArray_L3, Cache_L3, cacheconfig.blocksize, CounterLRU_L3);
						}

						// write through

						if (cacheconfig.WritePolicy == WritePolicy::WriteThrough) {

							block = getBlock(Row_L3Cache, cacheconfig.NWaySetAssociativeCacheL3, addressTagMemory,
								addressIndexL3, TagArray_L3, Cache_L3, cacheconfig.blocksize, CounterLRU_L3);

							writeThrough(block, Row_L1Cache, addressTagMemory, addressIndexL1, TagArray_L1, Cache_L1, ValidBit_L1, DirtyBit_L1, Row_L2Cache, addressIndexL2, TagArray_L2, Cache_L2, ValidBit_L2, DirtyBit_L2,
								Row_L3Cache, addressIndexL3, TagArray_L3, Cache_L3, ValidBit_L3, DirtyBit_L3, "W", Offset, CounterLRU_L1, CounterLRU_L2, CounterLRU_L3, CounterPLRU_L1, CounterPLRU_L2, CounterPLRU_L3, tempPLPU1, tempPLPU2, tempPLPU3, hit_L1, hit_L2, hit_L3,
								openPage, row, bank, column, columnSize, OffsetSize, Taccess, bus1, cacheconfig);
						}

						if (cacheconfig.writeAllocatePolicy == writeAllocatePolicy::WriteAllocate) {
							// set block from memory to L1 L2 L3 cache
							// check  Replacement Policy 
							// UpDate the Cache with the new block in the L1_cache L2_cache L3_cache 
							writeAllocate(block, Row_L1Cache, addressTagMemory, addressIndexL1, TagArray_L1, Cache_L1, ValidBit_L1, DirtyBit_L1, Row_L2Cache, addressIndexL2, TagArray_L2, Cache_L2, ValidBit_L2, DirtyBit_L2,
								Row_L3Cache, addressIndexL3, TagArray_L3, Cache_L3, ValidBit_L3, DirtyBit_L3, "W", Offset, CounterLRU_L1, CounterLRU_L2, CounterLRU_L3
								, CounterPLRU_L1, CounterPLRU_L2, CounterPLRU_L3, tempPLPU1, tempPLPU2, tempPLPU3, hit_L1, hit_L2, hit_L3, cacheconfig.WritePolicy, openPage, row, bank, column, columnSize, OffsetSize, Taccess, bus1, cacheconfig);
						}


					}
					else if (hit_L1 == 0 && hit_L2 == 1) {

						L1AcceState = WM;
						L2AcceState = WH;
						L3AcceState = NA;
						L1MissCounter++;
						L2HitCounter++;

						// write back
						if (cacheconfig.WritePolicy == WritePolicy::WriteBack) {

							UpDateCacheWB(Row_L2Cache, cacheconfig.NWaySetAssociativeCacheL2,
								addressTagMemory, addressIndexL2, TagArray_L2, DirtyBit_L2, Cache_L2, cacheconfig.blocksize, Offset,
								cacheconfig.L2_Cycles, Taccess);

							block = getBlock(Row_L2Cache, cacheconfig.NWaySetAssociativeCacheL2, addressTagMemory,
								addressIndexL2, TagArray_L2, Cache_L2, cacheconfig.blocksize, CounterLRU_L2);

						}
						// write through

						if (cacheconfig.WritePolicy == WritePolicy::WriteThrough) {
							block = getBlock(Row_L2Cache, cacheconfig.NWaySetAssociativeCacheL2, addressTagMemory,
								addressIndexL2, TagArray_L2, Cache_L2, cacheconfig.blocksize, CounterLRU_L2);

							writeThrough(block, Row_L1Cache, addressTagMemory,
								addressIndexL1, TagArray_L1, Cache_L1, ValidBit_L1, DirtyBit_L1,
								Row_L2Cache, addressIndexL2, TagArray_L2, Cache_L2, ValidBit_L2, DirtyBit_L2,
								Row_L3Cache, addressIndexL3, TagArray_L3, Cache_L3, ValidBit_L3, DirtyBit_L3, "W",
								Offset, CounterLRU_L1, CounterLRU_L2, CounterLRU_L3
								, CounterPLRU_L1, CounterPLRU_L2, CounterPLRU_L3, tempPLPU1, tempPLPU2, tempPLPU3, hit_L1, hit_L2, hit_L3,
								openPage, row, bank, column, columnSize, OffsetSize, Taccess, bus1, cacheconfig);
						}
						if (cacheconfig.writeAllocatePolicy == writeAllocatePolicy::WriteAllocate) {

							writeAllocate(block, Row_L1Cache, addressTagMemory, addressIndexL1, TagArray_L1, Cache_L1, ValidBit_L1, DirtyBit_L1, Row_L2Cache, addressIndexL2, TagArray_L2, Cache_L2, ValidBit_L2, DirtyBit_L2,
								Row_L3Cache, addressIndexL3, TagArray_L3, Cache_L3, ValidBit_L3, DirtyBit_L3, "W", Offset, CounterLRU_L1, CounterLRU_L2, CounterLRU_L3
								, CounterPLRU_L1, CounterPLRU_L2, CounterPLRU_L3, tempPLPU1, tempPLPU2, tempPLPU3, hit_L1, hit_L2, hit_L3, cacheconfig.WritePolicy, openPage, row, bank, column, columnSize, OffsetSize, Taccess, bus1, cacheconfig);
						}
					}
					else if (hit_L1 == 1) {
						L1AcceState = WH;
						L3AcceState = NA;
						L2AcceState = NA;
						L1HitCounter++;
						// write back
						if (cacheconfig.WritePolicy == WritePolicy::WriteBack) {

							UpDateCacheWB(Row_L1Cache, cacheconfig.NWaySetAssociativeCacheL1,
								addressTagMemory, addressIndexL1, TagArray_L1, DirtyBit_L1, Cache_L1, cacheconfig.blocksize, Offset,
								cacheconfig.L1_Cycles, Taccess);
						}


						// write through
						if (cacheconfig.WritePolicy == WritePolicy::WriteThrough) {

							block = getBlock(Row_L1Cache, cacheconfig.NWaySetAssociativeCacheL1, addressTagMemory,
								addressIndexL1, TagArray_L1, Cache_L1, cacheconfig.blocksize, CounterLRU_L1);

							writeThrough(block, Row_L1Cache, addressTagMemory, addressIndexL1, TagArray_L1, Cache_L1, ValidBit_L1, DirtyBit_L1, Row_L2Cache, addressIndexL2, TagArray_L2, Cache_L2, ValidBit_L2, DirtyBit_L2,
								Row_L3Cache, addressIndexL3, TagArray_L3, Cache_L3, ValidBit_L3, DirtyBit_L3, "W", Offset, CounterLRU_L1, CounterLRU_L2, CounterLRU_L3
								, CounterPLRU_L1, CounterPLRU_L2, CounterPLRU_L3, tempPLPU1, tempPLPU2, tempPLPU3, hit_L1, hit_L2, hit_L3, openPage, row, bank, column, columnSize, OffsetSize, Taccess, bus1, cacheconfig);
						}
					}
				}
				if (cacheconfig.printdata == 0)
				{
					tracesout << "  L1 Cache : " << L1AcceState
					<< "  L2 Cache : " << L2AcceState
					<< "  L3 Cache : " << L3AcceState
					<< " Main Memory : " << MemoryAcceState
					<< " Cycles" << Taccess << endl;
				}
			}
			tracesout << "  L1 Cache Hit: " << L1HitCounter
				<< "  L1 Cache Miss: " << L1MissCounter << endl
				<< "  L2 Cache : " << L2HitCounter
				<< "  L1 Cache Miss: " << L2MissCounter << endl
				<< "  L3 Cache : " << L3HitCounter
				<< "  L1 Cache Miss: " << L3MissCounter << endl
				<< " Main Memory : " << DDRHitCounter
				<< " Cycles" << Taccess << endl;

			traces.close();
			tracesout.close();
		}
		else cout << "\n Unable to open trace or traceout file ";


		return 0;


}