#include "s_box.h"

namespace my_cryptography_lib {
	s_box::s_box(is_inverse is_matrix_inverse)
		:matrix(is_matrix_inverse.is_inverse_matrix ? 
			s_box_helper::get_inverse_box() : s_box_helper::get_box())
	{}
	byte s_box::operator[](byte index) const {
		const int value = static_cast<int>(index);
		const int indexi = value / 16;
		const int indexj = value % 16;
		return matrix[indexi][indexj];
	}
	std::ostream& operator<<(std::ostream& out, const s_box& rhs) {
		std::for_each(rhs.matrix.begin(), rhs.matrix.end(), [&](auto& row) {
			std::copy(row.begin(), row.end(), std::ostream_iterator<byte>(out, " "));
			std::cout << std::endl;
			});
		return out;
	}
}