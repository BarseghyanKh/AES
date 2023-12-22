#ifndef TESTERS_H
#define TESTERS_H


#include "headers.h"
#include "byte.h"
#include "word.h"
#include "aes.h"
#include "random_functions.h"
#include "attacs.h"
//#include "s_box_helper.h"

struct testers {
	void attack_test();
	void byte_test();
	void word_test();
	void aes_test();
	void div_test();
	void id_divisor();
	void euclidean_extended();

	void test_subBytes();
	void test_shiftRows();
	void test_addRoundKey();

	void test_modular_product();
	void test_MixColumns();
	void mod_inverse();
	void create_S_box();
	void test_generate_rcon();
	void test_KeyExpnsion();
	void test_cypher();
};



#endif // TESTERS_H