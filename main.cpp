#include "testers.h"

using namespace my_cryptography_lib;


int main() {
	try {
		std::vector<byte> input = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
								0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };
		std::vector<byte> key = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
								0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
			
			
			
		testers t;
		//t.test_inv_cipher(input, key);
		t.test_inv_cipher();
		//t.test_cypher();
		
		//t.test_KeyExpnsion();
		//t.test_generate_rcon();
		//t.test_subBytes();
		/*t.test_MixColumns();
		t.test_shiftRows();

		t.mod_inverse();
		t.euclidean_extended();
		t.id_divisor();
		t.div_test();

		t.aes();

		t.byte();
		t.word()*/
	}
	catch (const std::exception& e){
		std::cerr << "Exception: " << e.what() << std::endl;
	}


	return 0;
}





