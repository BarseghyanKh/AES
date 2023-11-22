#include "s_box_helper.h"

namespace my_cryptography_lib {

	void s_box_helper::print(const value_type& matrix)  {
		//std::copy(matrix.begin(), matrix.end(), )

		std::for_each(matrix.begin(), matrix.end(), [](auto& row) {
			std::copy(row.begin(), row.end(), std::ostream_iterator<byte>(std::cout, " "));
			std::cout << std::endl;
			});
	}
	void s_box_helper::print_box()  {
		print(get_box());
	}
	void s_box_helper::print_inverse_box()  {
		print(get_inverse_box());
	}
	byte s_box_helper::apply_matrix_for_S_box(byte value) {
		const byte c = 0x63;
		const std::vector<byte> matrix_for_S_box = { 0x1f, 0x3e, 0x7c, 0xf8, 0xf1,0xe3,0xc7,0x8f };
		std::vector<byte> computed(matrix_for_S_box);
		std::uint8_t b = value.get();
		return std::accumulate(computed.begin(), computed.end(), byte(c),
			[&b](byte accumulator, byte current) {
				return (b & 1) ? (b >>= 1, accumulator + current) :
					(b >>= 1, accumulator);
			});
	}
	s_box_helper::value_type s_box_helper::get_box()  {
		value_type result_box(16, std::vector<byte>(16));
		for (int i = 0; i < result_box.size(); ++i) {
			for (int j = 0; j < result_box[0].size(); ++j) {
				byte inverse_byte = byte((i << 4) ^ j).get_inverse();
				result_box[i][j] = apply_matrix_for_S_box(inverse_byte);
			}
		}
		return result_box;
	}
	s_box_helper::value_type s_box_helper::get_inverse_box() {
		value_type result_box(16, std::vector<byte>(16));
		for (int i = 0; i < result_box.size(); ++i) {
			for (int j = 0; j < result_box[0].size(); ++j) {
				byte inverse_byte = byte((i << 4) ^ j).get_inverse();

				byte indices = apply_matrix_for_S_box(inverse_byte);
				int value = static_cast<int>(indices);
				int indexi = value / 16;
				int indexj = value % 16;
				result_box[indexi][indexj] = byte((i << 4) ^ j);
			}
		}
		return result_box;
	}

}