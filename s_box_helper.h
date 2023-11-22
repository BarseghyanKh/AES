#ifndef S_BOX_HELPER_H
#define S_BOX_H_HELPER_H
#include "headers.h"
#include "byte.h"

namespace my_cryptography_lib {

	class s_box_helper {
		using value_type = std::vector<std::vector<byte>>;
	public:

		s_box_helper() = delete;
		s_box_helper(const s_box_helper&) = delete;
		s_box_helper(s_box_helper&&) = delete;
		s_box_helper& operator=(const s_box_helper&) = delete;
		s_box_helper& operator=(s_box_helper&&) = delete;

		

		static void print_box() ;
		static void print_inverse_box() ;

		static value_type get_box() ;
		static value_type get_inverse_box() ;
	private:
		static byte apply_matrix_for_S_box(byte value);
		static void print(const value_type& matrix);

		
	};

}
#endif // S_BOX_H_HELPER_H