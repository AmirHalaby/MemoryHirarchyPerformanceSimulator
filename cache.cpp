
#include "memorySimulator.h"

Cache::Cache(config config_param) {
		this->config_params = config_param;

		//Associoativity is Two Way Set Associative Cache
		this->blockOffset_L1 = (unsigned long)log2(config_params.blocksize);
		this->setIndex_L1 = (unsigned long)log2(config_params.L1CacheSize / config_param.blocksize);
		this->tag_L1 = 32 - setIndex_L1 - blockOffset_L1;
		unsigned long Offset_L1 = (unsigned long)pow(2, blockOffset_L1);
		this->indexRow_L1 = (unsigned long)pow(2, setIndex_L1) / config_param.NWaySetAssociativeCacheL1;
		this->validBit_L1.resize(this->indexRow_L1);
		this->DirtyBit_L1.resize(this->indexRow_L1);
		this->TagArray_L1.resize(this->indexRow_L1);
		for (int i = 0; i < this->indexRow_L1; i++) {
			this->validBit_L1[i].resize(config_param.NWaySetAssociativeCacheL1);
			this->DirtyBit_L1[i].resize(config_param.NWaySetAssociativeCacheL1);
			this->TagArray_L1[i].resize(config_param.NWaySetAssociativeCacheL1);
		}

		this->blockOffset_L2 = (unsigned long)log2(config_params.blocksize);
		this->setIndex_L2 = (unsigned long)log2(config_params.L2CacheSize / config_param.blocksize);
		this->tag_L2 = 32 - setIndex_L2 - blockOffset_L2;
		unsigned long Offset_L2 = (unsigned long)pow(2, blockOffset_L2);
		this->indexRow_L2 = (unsigned long)pow(2, setIndex_L2) / config_param.NWaySetAssociativeCacheL2;
		this->validBit_L2.resize(this->indexRow_L2);
		this->DirtyBit_L2.resize(this->indexRow_L2);
		this->TagArray_L2.resize(this->indexRow_L2);
		for (int i = 0; i < this->indexRow_L2; i++) {
			this->validBit_L2[i].resize(config_param.NWaySetAssociativeCacheL2);
			this->DirtyBit_L2[i].resize(config_param.NWaySetAssociativeCacheL2);
			this->TagArray_L2[i].resize(config_param.NWaySetAssociativeCacheL2);
		}


		this->blockOffset_L3 = (unsigned long)log2(config_params.blocksize);
		this->setIndex_L3 = (unsigned long)log2(config_params.L3CacheSize / config_param.blocksize);
		this->tag_L3 = 32 - setIndex_L3 - blockOffset_L3;
		unsigned long Offset_L3 = (unsigned long)pow(2, blockOffset_L3);
		this->indexRow_L3 = (unsigned long)pow(2, setIndex_L3) / config_param.NWaySetAssociativeCacheL3;
		this->validBit_L3.resize(this->indexRow_L3);
		this->DirtyBit_L3.resize(this->indexRow_L3);
		this->TagArray_L3.resize(this->indexRow_L3);
		for (int i = 0; i < this->indexRow_L3; i++) {
			this->validBit_L3[i].resize(config_param.NWaySetAssociativeCacheL3);
			this->DirtyBit_L3[i].resize(config_param.NWaySetAssociativeCacheL3);
			this->TagArray_L3[i].resize(config_param.NWaySetAssociativeCacheL3);

		}
		unsigned long blocksize1 = config_params.blocksize * config_param.NWaySetAssociativeCacheL1;
		unsigned long blocksize2 = config_params.blocksize * config_param.NWaySetAssociativeCacheL2;
		unsigned long blocksize3 = config_params.blocksize * config_param.NWaySetAssociativeCacheL3;
		this->cache_L1 = createCache(this->indexRow_L1, blocksize1);
		this->cache_L2 = createCache(this->indexRow_L2, blocksize2);
		this->cache_L3 = createCache(this->indexRow_L3, blocksize3);


	}

	vector< vector<unsigned long> >  Cache::getCache_L1() {
		return this->cache_L1;
	}
	vector< vector<unsigned long> >  Cache::getCache_L2() {
		return this->cache_L2;
	}
	vector< vector<unsigned long> >  Cache::getCache_L3() {
		return this->cache_L3;
	}




	vector< vector<unsigned long> >  Cache::getTagArray_L1() {
		return this->TagArray_L1;
	}
	vector< vector<unsigned long> >  Cache::getTagArray_L2() {
		return this->TagArray_L2;
	}
	vector< vector<unsigned long> >  Cache::getTagArray_L3() {
		return this->TagArray_L3;
	}



	vector< vector<bitset<1>> >  Cache::getValidBit_L1() {
		return this->validBit_L1;
	}
	vector< vector<bitset<1>> >  Cache::getValidBit_L2() {
		return this->validBit_L2;
	}
	vector< vector<bitset<1>> >  Cache::getValidBit_L3() {
		return this->validBit_L3;
	}
	vector< vector<bitset<1>> >  Cache::getDirtyBit_L1() {
		return this->DirtyBit_L1;
	}
	vector< vector<bitset<1>> >  Cache::getDirtyBit_L2() {
		return this->DirtyBit_L2;
	}
	vector< vector<bitset<1>> > Cache::getDirtyBit_L3() {
		return this->DirtyBit_L3;
	}
	unsigned long Cache::getRow_L1Cache() {
		return this->indexRow_L1;
	}
	unsigned long Cache::getRow_L2Cache() {
		return this->indexRow_L2;
	}
	unsigned long Cache::getRow_L3Cache() {
		return this->indexRow_L3;
	}

	vector<vector<unsigned long> > Cache::createCache(unsigned long& indexRow,unsigned long& blockSize) {
		vector<vector<unsigned long> > myCache;
		myCache.resize((unsigned long)indexRow);
		for (int i = 0; i < indexRow; i++) {
			myCache[i].resize(blockSize);
		}
		return myCache;
	}

	vector<string> Cache::getBits(bitset<32> addr) {
		string stringAddr = addr.to_string();
		unsigned long memoryOffset = this->blockOffset_L3 + this->setIndex_L3;
		vector<string> bits(9);
		bits[0] = stringAddr.substr(0, this->tag_L1);
		bits[1] = stringAddr.substr(this->tag_L1, this->setIndex_L1);
		bits[2] = stringAddr.substr(this->tag_L1 + this->setIndex_L1, this->blockOffset_L1);

		bits[3] = stringAddr.substr(0, this->tag_L2);
		bits[4] = stringAddr.substr(this->tag_L2, this->setIndex_L2);
		bits[5] = stringAddr.substr(this->tag_L2 + this->setIndex_L2, this->blockOffset_L2);

		bits[6] = stringAddr.substr(0, this->tag_L3);
		bits[7] = stringAddr.substr(this->tag_L3, this->setIndex_L3);
		bits[8] = stringAddr.substr(this->tag_L3 + this->setIndex_L3, this->blockOffset_L3);

		//		bits[9] = stringAddr.substr(0, this->tag_L3);
		//		bits[10] = stringAddr.substr(this->tag_L3, memoryOffset);

		return bits;
	}


