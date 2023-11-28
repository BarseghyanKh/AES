#ifndef S_BOX_H
#define S_BOX_H

#include "s_box_helper.h"

namespace my_cryptography_lib {

	struct is_inverse {
		bool is_inverse_matrix;
		explicit is_inverse(bool value):is_inverse_matrix(value){}
	};

	class s_box {
		using value_type = std::vector<std::vector<byte>>;
		friend std::ostream& operator<<(std::ostream& out, const s_box& rhs);
	public:
		s_box(is_inverse is_matrix_inverse = is_inverse(false));

		s_box(const s_box&) = delete;
		s_box(s_box&&) = delete;
		s_box& operator=(const s_box&) = delete;
		s_box& operator=(s_box&&) = delete;

		byte operator[](byte index) const;
	private:
		std::vector<std::vector<byte>> matrix;
	};

}
#endif // S_BOX_H