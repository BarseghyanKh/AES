#ifndef AES_H
#define AES_H

#include "headers.h"
#include "word.h"
#include "s_box.h"

namespace my_cryptography_lib {

	const s_box box(is_inverse(false));
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
		Aes(std::vector<byte> input) {
			word w;
			for (int i = 0; i < input.size(); ++i) {
				if (i != 0 && (i % 4 == 0)) {
					plaintext.push_back(w);
				}
				w[i % 4] = input[i];
			}
			plaintext.push_back(w);
		}
		void print_plaintext() const {
			std::copy(plaintext.begin(), plaintext.end(), std::ostream_iterator<word>(std::cout << "\n"));
		}

		std::vector<word> cipher() {
			std::vector<word> round_key = { 
							word({0x2b, 0x7e, 0x15, 0x16}), word({0x28, 0xae, 0xd2, 0xa6}),
							word({0xab, 0xf7, 0x15, 0x88}), word({0x09, 0xcf, 0x4f, 0x3c}) };

			std::vector<word> state(plaintext.begin(), plaintext.begin() + m_value.Nb);
			state =  AddRountKey(state, round_key);
			state =  SubBytes(state);
			state = ShiftRows(state);
			state = MixColumns(state);

			return ShiftRows(state);
		}

		std::vector<word> ShiftRows(std::vector<word> state) {
			std::vector<word> result(state.size());
			int word_size = 4;
			for (int r = 0; r < word_size; ++r) {
				for (int c = 0; c < state.size(); ++c) {
					result[c][r] = state[(c + r) % m_value.Nb][r];
				}
			}
			return result;
		}
		std::vector<word> SubBytes(std::vector<word> state) {
			std::vector<word> state_result;
			for (int i = 0; i < state.size(); ++i) {
				state_result.push_back(SubWord(state[i]));
			}
			return state_result;
		}
		std::vector<word> MixColumns(std::vector<word> state) {
			std::vector<word> state_result;
			word a({ 0x02, 0x01, 0x01, 0x03 });
			std::transform(state.begin(), state.end(), std::back_inserter(state_result),
				[&a](const word& w) { return a.modular_product(w); });
			return state_result;
		}
		
		word RotWord(const word& w) {
			word result;
			for (int i = 0; i < 4; ++i) {
				result[i] = w[(i + 1) % 4];
			}
			return result;
		}
		word SubWord(const word& w) {
			word result;
			for (int j = 0; j < 4; ++j) {
				result[j] = box[w[j]];
			}
			return result;
		}

		std::vector<word> AddRountKey(std::vector<word> state, std::vector<word> round_key) {
			std::vector<word> result;
			transform(state.begin(), state.end(), round_key.begin(), back_inserter(result),
				std::plus<word>());
			return result;
		}
	};

}
#endif