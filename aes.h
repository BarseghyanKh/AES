#ifndef AES_H
#define AES_H

#include "headers.h"
#include "word.h"
#include "s_box.h"

namespace my_cryptography_lib {

	const s_box box(is_inverse(true));
	const s_box inverse_box(is_inverse(true));

	template <std::size_t T>
	struct AES_type {
		static_assert(std::is_same<T, T>::value, "Using the generic template is not allowed.");
	};

	template <>
	struct AES_type<128> {
		static constexpr std::size_t Nb = 4;
		static constexpr std::size_t Nk = 4;
		static constexpr std::size_t Nr = 10;
	};
	template <>
	struct AES_type<192> {
		static constexpr std::size_t Nb = 4;
		static constexpr std::size_t Nk = 6;
		static constexpr std::size_t Nr = 12;
	};
	template <>
	struct AES_type<256> {
		static constexpr std::size_t Nb = 4;
		static constexpr std::size_t Nk = 8;
		static constexpr std::size_t Nr = 14;
	};

	template <std::size_t Bit_counts>
	class Aes {
	private:
		// AES type 128, 192, 256
		static constexpr AES_type<Bit_counts> m_value = {};

		std::vector<word> plaintext;
		std::vector<word> cyphertext;
	public:
		Aes()
			//:plaintext{ word(), word(), word(), word() }
		{
			std::generate_n(back_inserter(plaintext), m_value.Nk, []() {return word(); });
		}
		void print_plaintext() const {
			std::copy(plaintext.begin(), plaintext.end(), std::ostream_iterator<word>(std::cout << "\n"));
		}

		std::vector<word> cipher() {
			std::vector<word> state(plaintext.begin(), plaintext.begin() + m_value.Nb);
		}

		void RotWord() {}
		void ShiftRows() {}


		std::vector<word> SubBytes(std::vector<word> state) {
			std::vector<word> state_result;
			state_result[0] = state[0];


			for (int i = 0; i < state.size(); ++i) {

			}

			std::size_t byte_count_in_word = 4;
			for(int i = 0; i < byte_count_in_word; ++i){
				word w;
				w[i] = box[state[0][i]];
			}
			
		}
		void SubWord() {}
	};

}
#endif