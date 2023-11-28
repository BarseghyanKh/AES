#include "testers.h"

using namespace my_cryptography_lib;


byte get_inverse(byte g, byte m) {
	return 0;
}

int main() {

	try {
		testers t;
		t.test_shiftRows();

		//t.mod_inverse();
		//t.euclidean_extended();
		//t.id_divisor();
		//t.div_test();

		/*t.aes();

		t.byte();
		t.word()*/
	}
	catch (const std::exception& e){
		std::cerr << "Exception: " << e.what() << std::endl;
	}


	return 0;
}





