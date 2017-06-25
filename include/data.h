#ifndef INCLUDE_DATA_H_
#define INCLUDE_DATA_H_

#include <string>
#include <fstream>

#include <unistd.h>


class Data {
public:
	Data() {}
	~Data() { ifs.close(); }

	void Open();

	char* filename;
	std::ifstream ifs;
};

#endif
