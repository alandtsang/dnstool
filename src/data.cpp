#include <fstream>

#include "data.h"

void Data::Open() {
	ifs.open(filename, std::ifstream::in);

	ifs.seekg (0, ifs.end);
    length_ = ifs.tellg();
    ifs.seekg (0, ifs.beg);

    buffer = new char[length_];
    ifs.read(buffer, length_);
    ifs.close();

	return;
}
