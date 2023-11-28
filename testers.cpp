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

void testers::test_subBytes() {
	std::vector<byte> input = { 0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 
								0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34 };
	Aes<128> a(input);

	std::vector<word> result = a.cipher();

	std::vector<word> golden = { word({0xd4, 0x27, 0x11, 0xae}), word({0xe0, 0xbf, 0x98, 0xf1}),
								 word({0xb8, 0xb4, 0x5d, 0xe5}), word({0x1e, 0x41, 0x52, 0x30}) };
	std::cout << std::equal(result.begin(), result.end(), golden.begin(), golden.end()) << std::endl;
}

void testers::test_shiftRows() {
	std::vector<byte> input = { 0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d,
								0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34 };
	Aes<128> a(input);

	std::vector<word> result = a.cipher();

	std::vector<word> golden = { word({0xd4, 0xbf, 0x5d, 0x30}), word({0xe0, 0xb4, 0x52, 0xae}),
								 word({0xb8, 0x41, 0x11, 0xf1}), word({0x1e, 0x27, 0x98, 0xe5}) };
	std::cout << std::equal(result.begin(), result.end(), golden.begin(), golden.end()) << std::endl;
}

void testers::test_addRoundKey() {
	std::vector<byte> input = { 0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d,
								0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34 };
	Aes<128> a(input);
	std::vector<word> result = a.cipher();

	std::vector<word> golden = { word({0x19, 0x3d, 0xe3, 0xbe}), word({0xa0, 0xf4, 0xe2, 0x2b}),
								 word({0x9a, 0xc6, 0x8d, 0x2a}), word({0xe9, 0xf8, 0x48, 0x08}) };
	std::cout << std::equal(result.begin(), result.end(), golden.begin(), golden.end()) << std::endl;
}