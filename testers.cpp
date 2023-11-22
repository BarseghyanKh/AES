#include "testers.h"

using namespace my_cryptography_lib;

void testers::attack_test() {
	attack a;
	a.run();
}

void testers::byte_test() {
	byte a = 0x57;
	byte b = 0x83;
	//std::cout << a << b;
	a *= b;
	std::cout << "\n\n";
	std::cout << a;
}

void testers::word_test() {
	word w = { byte(0x57), byte(0x13), byte(0x14), byte(0x17) };
	word res = w.modular_product(w);
	std::cout << res;
}
void testers::aes_test() {
	Aes<128> a;
	a.print_plaintext();
}

void testers::div_test() {
	byte a(0x0e);
	byte b(0x03);
	byte c = a / b;
	byte d(0xff);
	//c = d.id_divisor();
	std::cout << (c == byte(0x02) ? "True" : "False") << std::endl;
	
}

void testers::id_divisor() {
	byte a(0x0e);
	std::pair<byte,byte> c = a.id_divisor();
	std::cout << c.first << " " << c.second << std::endl;
	std::cout << c.first * a  /*+ c.second*/ << std::endl;
	std::cout << (c.first == byte(0x34) ? "True" : "False") << std::endl;

}

void testers::euclidean_extended() {
	byte a(0x0e);
	std::cout << a.get_inverse() << std::endl;

}

void testers::mod_inverse() {
	byte a(0x3d);
	
	byte a_inverse = a.get_inverse();
	byte value = a_inverse.apply_matrix_for_S_box();
	std::cout << value << std::endl;
}

void testers::create_S_box() {


	s_box_helper::print_box();
	std::cout << std::endl;
	s_box_helper::print_inverse_box();
	/*std::vector <std::vector<byte>> S_box(16, std::vector<byte>(16));
	for (int i = 0; i < S_box.size(); ++i) {
		for (int j = 0; j < S_box[0].size(); ++j) {
			S_box[i][j] = byte((i << 4) ^ j).get_inverse().apply_matrix_for_S_box();
		}
	}

	for (int i = 0; i < S_box.size(); ++i) {
		for (int j = 0; j < S_box[0].size(); ++j) {
			std::cout << S_box[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	std::vector <std::vector<byte>> S_box_inverse(16, std::vector<byte>(16));
	for (int i = 0; i < S_box.size(); ++i) {
		for (int j = 0; j < S_box[0].size(); ++j) {
			int value = static_cast<int>(S_box[i][j]);
			int indexi = value / 16;
			int indexj = value % 16;
			S_box_inverse[indexi][indexj] = byte((i << 4) ^ j);
		}
		
	}
	for (int i = 0; i < S_box.size(); ++i) {
		for (int j = 0; j < S_box_inverse[0].size(); ++j) {
			std::cout << S_box_inverse[i][j] << " ";
		}
		std::cout << std::endl;
	}*/
}