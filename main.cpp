#include <iostream>

#include "BFK.h"

int main(int argc, char* argv[]) {

	std::cout << argv[1] << std::endl;

	if (argc > 1) {

		BFK bfk(argv[1]);

		if (!bfk) {
			std::cout << "Couldn't open the file!";
			return 1;
		}
		bfk.run();
		if (bfk.is_imbalanced()) {
			std::cout << "Imbalanced Brackets!\n";
		}
	}
	return 0;
}