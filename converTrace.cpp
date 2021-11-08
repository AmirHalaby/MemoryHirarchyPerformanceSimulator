#include "memorySimulator.h"


ConverTrace::ConverTrace() {

		ifstream traces;
		ofstream AddressAndTypeFile;
		string outname;
		outname = string("C:\\Users\\AA\\source\\repos\\MemoryHirarchyPerformanceSimulator\\MemoryHirarchyPerformanceSimulator\\AddressAndTypeFile.txt");

		traces.open("C:\\Users\\AA\\source\\repos\\MemoryHirarchyPerformanceSimulator\\MemoryHirarchyPerformanceSimulator\\trace.txt");
		AddressAndTypeFile.open(outname.c_str());

		string line;

		string x, registerName1, oldData, y, newDate;
		string z, t, commandType, assemblyCommand;
		// Initialize array

		string registerName[32]{ "zero" , "ra" , "sp" , "gp" , "tp"
								, "t0" , "t1" , "t2"
								, "s0" , "s1"
								, "a0", "a1" ,"a2" , "a3" , "a4" , "a5" , "a6" , "a7"
								, "s2" , "a3" , "a4" , "a5" , "s6" , "s7" , "s8" , "a9" , "a10" , "a11"
								, "t3" , "t4" , "t5" , "t6" };
		string registerData[32];
		registerData[0] = "00000000";
		int iTemp = 0;
		int iTemp1 = 0;
		int count = 0;
		if (traces.is_open() && AddressAndTypeFile.is_open()) {
			while (getline(traces, line)) {   // read mem access file and access Cache
				int lineSize = line.length();
				istringstream iss(line);

				if (lineSize > 70) {

					if (!(iss >> x >> y >> z >> y >> commandType >> assemblyCommand)) { break; }
					string r1, r2, num;
					string command = commandType.substr(0, 1);
					string type;

					if (command == "s") type = { "W" };
					if (command == "l")type = { "R" };

					if (commandType == "ld" || commandType == "lw"
						|| commandType == "lb" || commandType == "lh"
						|| commandType == "sd" || commandType == "sw"
						|| commandType == "sb" || commandType == "sh") {


						if (assemblyCommand.length() == 8) {
							string size;
							string data;
							string r1 = assemblyCommand.substr(0, 2);
							string sheftRegister = assemblyCommand.substr(3, 1);
							string r2 = assemblyCommand.substr(5, 2);
							if (commandType == "sb")  size = "b";
							if (commandType == "sw")  size = "w";
							if (commandType == "sh")  size = "hw";

							for (int i = 0; i < 32; i++) {
								if (r1 == registerName[i])
								{
									if (registerData[i] == "")
									{
										data = "00000";
									}
									else
									{
										data = registerData[i];
									}
									break;
								}
							}
							for (int i = 0; i < 32; i++) {
								if (r2 == registerName[i])
								{
									// << " " << size << " " << data 
									AddressAndTypeFile << type << " " << intToHex(registerData[i], sheftRegister) << endl;
									break;
								}
							}

						}
						if (assemblyCommand.length() == 9) {
							string size, data;
							string r1 = assemblyCommand.substr(0, 2);
							string sheftRegister = assemblyCommand.substr(3, 2);
							string r2 = assemblyCommand.substr(6, 2);

							if (commandType == "sb")  size = "b";
							if (commandType == "sw")  size = "w";
							if (commandType == "sh")  size = "hw";

							for (int i = 0; i < 32; i++) {
								if (r1 == registerName[i])
								{
									data = intToHex(registerData[i], sheftRegister);
									break;
								}
							}

							for (int i = 0; i < 32; i++) {
								if (r2 == registerName[i])
								{
									//  size << data <<
									AddressAndTypeFile << type << " " << intToHex(registerData[i], sheftRegister) << endl;
									break;
								}
							}
						}
					}
				}
				if (lineSize < 60) {
					if (!(iss >> x >> registerName1 >> oldData >> y >> newDate)) { break; }
					string Date;
					if (newDate.length() == 16)
					{
						Date = newDate;
					}
					else
					{
						Date = "00000000" + newDate;
					}
					for (int i = 0; i < 32; i++) {
						if (registerName1 == registerName[i])
						{
							registerData[i] = Date;
							break;
						}
					}
				}
			}



		}

	}
string ConverTrace::intToHex(string registerData, string sheftRegister) {
		string updateRegister;
		int sheftRegisterInt;
		updateRegister = registerData;
		std::stringstream str;
		str << updateRegister;
		int value;
		str >> std::hex >> value;
		sheftRegisterInt = stoi(sheftRegister);
		value = value + sheftRegisterInt;
		std::stringstream stream;
		stream << std::hex << value;
		std::string result(stream.str());

		if (result.length() == 4) {
			result = "0x0000" + result;
		}
		if (result.length() == 5) {
			result = "0x000" + result;
		}
		if (result.length() == 6) {
			result = "0x00" + result;
		}
		if (result.length() == 7) {
			result = "0x0" + result;
		}
		if (result.length() == 8) {
			result = "0x" + result;
		}
		if (result.length() == 16)
		{
			result = "0x" + result;
		}


		return result;

	}

