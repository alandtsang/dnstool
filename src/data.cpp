#include <fstream>
#include <cstdlib>

#include "data.h"

void Data::Open() {
	ifs.open(filename, std::ifstream::in);
	if (!ifs) {
        fprintf(stdout, "Error: %s in not exist\n", filename);
        exit(EXIT_FAILURE);
    }

	ifs.seekg (0, ifs.end);
    length_ = ifs.tellg();
    ifs.seekg (0, ifs.beg);

    buffer = new char[length_];
    ifs.read(buffer, length_);
    ifs.close();

	return;
}
