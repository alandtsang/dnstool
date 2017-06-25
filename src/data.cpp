#include <fstream>

#include "data.h"

void Data::Open() {
	ifs.open(filename, std::ifstream::in);
	return;
}
