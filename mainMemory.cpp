#include "memorySimulator.h"

mainMemory::mainMemory(config config_param) {
		this->config_params = config_param;
		this->bank = 3;
		this->channel = 2;
		this->rank = 1;

		this->blockOffset = (unsigned long)log2(config_params.blocksize);
		this->setIndex = (unsigned long)log2(config_params.L3CacheSize / config_param.blocksize);
		this->tag = 32 - blockOffset;


		// Row interLeaving

		this->ByteInbus = 3;
		this->column = 11;
		this->row = 14;

		// cache block interLeaving
		/*
		*
		*
		**/
		this->openPages = createOpenPages(bank, channel, rank);
	}
	vector< vector< long>>  mainMemory::createOpenPages(unsigned long& bankInRank, unsigned long& channel, unsigned long& ranks) {
		vector< vector< long>>  myMemory;
		unsigned long size = pow(2, bankInRank) * channel * ranks;
		myMemory.resize((unsigned long)size);
		for (int i = 0; i < size; i++) {
			myMemory[i].resize(2);
		}
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < 2; j++) {
				myMemory[i][j] = -1;
			}
		}
		return myMemory;
	}
	vector<string> mainMemory::getBits1(bitset<32> addr) {
		string stringAddr = addr.to_string();
		vector<string> bits(2);
		bits[0] = stringAddr.substr(0, this->tag);
		bits[1] = stringAddr.substr(this->tag, this->blockOffset);

		return bits;
	}

	unsigned long mainMemory::getBank() {
		return this->bank;
	}
	unsigned long mainMemory::getChannel() {
		return this->channel;
	}
	unsigned long mainMemory::getRank() {
		return this->rank;
	}

	unsigned long mainMemory::getCoulmnSize() {
		return pow(2, this->column);
	}



	vector<string> mainMemory::getBits(bitset<32> addr) {
		string stringAddr = addr.to_string();
		vector<string> bits(5);
		bits[0] = stringAddr.substr(0, this->row);
		bits[1] = stringAddr.substr(this->row, 1);
		bits[2] = stringAddr.substr(this->row, this->bank);
		bits[3] = stringAddr.substr(this->row + this->bank, this->column);
		bits[4] = stringAddr.substr(this->row + this->bank + this->column, this->ByteInbus);

		return bits;
	}
	vector< vector< long>> 	mainMemory::getPage() {
		return this->openPages;
	}




void upDatePage(vector< vector< long>>& openPage, unsigned long& tagAddress, int  OffsetSize,
	unsigned long& DDR_Cycles_PageAndColumnIsOpen, unsigned long& DDR_Cycles_PageOpen
	, unsigned long& DDR_Cycles_PageNotOpen, long& CyclesSimulator, AddressMapping AddressMapping)
{

	unsigned long tagSize = 32 - OffsetSize;

	string tag1, offset1, address;
	if (tagSize == 28)
	{
		bitset<28> tag;
		bitset<4> offset;
		tag = bitset<28>(tagAddress);
		offset = bitset<4>(0);

		tag1 = tag.to_string();
		offset1 = offset.to_string();
		address = tag1 + offset1;

	}

	if (tagSize == 27)
	{
		bitset<27> tag;
		bitset<5> offset;
		tag = bitset<27>(tagAddress);
		offset = bitset<5>(0);

		tag1 = tag.to_string();
		offset1 = offset.to_string();
		address = tag1 + offset1;

	}

	if (tagSize == 26)
	{
		bitset<26> tag;
		bitset<6> offset;
		tag = bitset<26>(tagAddress);
		offset = bitset<6>(0);

		tag1 = tag.to_string();
		offset1 = offset.to_string();
		address = tag1 + offset1;

	}
	if (tagSize == 25)
	{
		bitset<25> tag;
		bitset<7> offset;
		tag = bitset<25>(tagAddress);
		offset = bitset<7>(0);

		tag1 = tag.to_string();
		offset1 = offset.to_string();
		address = tag1 + offset1;

	}
	int bank1 = 3;
	int row1 = 14;
	int column1 = 11;
	int channel1 = 1;
	int hightCloumn = 8;
	int lowColumn = 3;
	vector<string> bits(6);

	if (AddressMapping == AddressMapping::RowInterleaving)
	{
		bits[0] = address.substr(0, row1);
		bits[1] = address.substr(row1, 1);
		bits[2] = address.substr(row1, channel1 + bank1);
		bits[3] = address.substr(row1 + channel1 + bank1, column1);
	}


	if (AddressMapping == AddressMapping::CacheBlockInterleaving)
	{
		bits[0] = address.substr(0, row1);
		bits[1] = address.substr(row1, 1);
		bits[2] = address.substr(row1 + channel1, hightCloumn);
		bits[3] = address.substr(row1 + channel1 + hightCloumn, bank1);
		bits[4] = address.substr(row1 + channel1 + hightCloumn + bank1, lowColumn);
		bits[3] = bits[3] + bits[1];
		bits[5] = bits[2] + bits[4];

	}

	char* ptr;
	long  row = strtol(bits[0].c_str(), &ptr, 2);
	long  channel = strtol(bits[1].c_str(), &ptr, 2);
	long  bank = strtol(bits[2].c_str(), &ptr, 2);
	long  column = strtol(bits[3].c_str(), &ptr, 2);

	if (AddressMapping == AddressMapping::CacheBlockInterleaving)
	{
		bank = strtol(bits[3].c_str(), &ptr, 2);
		column = strtol(bits[5].c_str(), &ptr, 2);
	}



	// if the row and the column is open
	if (openPage[bank][0] == row && openPage[bank][1] == column)
	{
		// time !!
		CyclesSimulator = CyclesSimulator + DDR_Cycles_PageAndColumnIsOpen;
	}


	// chack if the page is open 
	if (openPage[bank][0] != row)
	{
		openPage[bank][0] = row;
		openPage[bank][1] = column;
		CyclesSimulator = CyclesSimulator + DDR_Cycles_PageNotOpen;

	}
	// chack if the correct column is open in the correct page
	if (openPage[bank][1] != column)
	{
		openPage[bank][1] = column;
		CyclesSimulator = CyclesSimulator + DDR_Cycles_PageOpen;

	}

}
