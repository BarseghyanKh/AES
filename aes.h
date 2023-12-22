#ifndef AES_H
#define AES_H

#include "headers.h"
#include "word.h"
#include "s_box.h"

namespace my_cryptography_lib {

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

		std::vector<word> cypher_key;
		std::vector<word> plaintext;
		std::vector<word> cyphertext;
	public:
		Aes()
			//:plaintext{ word(), word(), word(), word() }
		{
			std::generate_n(back_inserter(plaintext), m_value.Nk, []() {return word(); });
			std::generate_n(back_inserter(cypher_key), m_value.Nk, []() {return word(); });
		}

		Aes(const std::vector<byte>& input, const std::vector<byte>& key) {
			plaintext = convert_byte_to_word_array(input);
			cypher_key = convert_byte_to_word_array(key);
		}
		void print_plaintext() const {
			std::copy(plaintext.begin(), plaintext.end(), std::ostream_iterator<word>(std::cout << "\n"));
		}

		void print_state(const std::vector<word>& state) {
			std::copy(state.begin(), state.end(), std::ostream_iterator<word>(std::cout, "\n"));
			std::cout << std::endl;
		}
		std::vector<word> cipher() {
			std::vector<word> w = KeyExpnsion();
			std::vector<word> state(plaintext.begin(), plaintext.begin() + m_value.Nb);
			std::vector<word> round_key(w.begin(), w.begin() + m_value.Nb);
			AddRountKey(state, round_key);
			for (int round = 1; round < m_value.Nr; ++round) {
				SubBytes(state);
				ShiftRows(state);
				MixColumns(state);
				std::copy(w.begin() + (round * m_value.Nb), w.begin() + (round + 1) * m_value.Nb, round_key.begin());
				AddRountKey(state, round_key);
			}
			SubBytes(state);
			ShiftRows(state);
			std::copy(w.begin() + (m_value.Nr * m_value.Nb), w.begin() + (m_value.Nr + 1) * m_value.Nb, round_key.begin());
			AddRountKey(state, round_key);
			return state;
		}

		void ShiftRows(std::vector<word>& state) {
			std::vector<word> result(state.size());
			int word_size = 4;
			for (int r = 0; r < word_size; ++r) {
				for (int c = 0; c < state.size(); ++c) {
					result[c][r] = state[(c + r) % m_value.Nb][r];
				}
			}
			std::swap(state, result);
		}
		void SubBytes(std::vector<word>& state) {
			std::vector<word> state_result;
			state_result.reserve(state.size());
			std::transform(state.begin(), state.end(), std::back_inserter(state_result), std::mem_fn(&word::SubWord));
			std::swap(state, state_result);
		}
		void MixColumns(std::vector<word>& state) {
			std::vector<word> state_result;
			word a({ 0x02, 0x01, 0x01, 0x03 });
			std::transform(state.begin(), state.end(), std::back_inserter(state_result),
				[&a](const word& w) { return a.modular_product(w); });
			std::swap(state, state_result);
		}

		word RotWord(const word& w) {
			return w.RotWord();
		}
		word SubWord(const word& w) {
			return w.SubWord();
		}

		void AddRountKey(std::vector<word>& state, std::vector<word> round_key) {
			
			transform(state.begin(), state.end(), round_key.begin(), state.begin(),
				std::plus<word>());
		
		}

		std::vector<word> KeyExpnsion() {
			std::vector<word> Rcon = generate_Rcon();
			word temp;
			std::vector<word> w;
			w.reserve(m_value.Nb * (m_value.Nr + 1));
			std::copy(cypher_key.begin(), cypher_key.end(), std::back_inserter(w));

			int i = m_value.Nk;
			while (i < m_value.Nb * (m_value.Nr + 1)){
				temp = w[i - 1];
				if (0 == i % m_value.Nk) {
					temp = SubWord(RotWord(temp)) + Rcon[i / m_value.Nk - 1];
				}
				else if (m_value.Nk > 6 && i % m_value.Nk == 4) {
					temp = SubWord(temp);
				}
				w.push_back(w[i - m_value.Nk] + temp);
				++i;
			}
			return w;
		}

		std::vector<word> generate_Rcon() {
			std::vector<word> Rcon;
			int length = m_value.Nb * (m_value.Nr + 1) / m_value.Nk + 1;
			Rcon.reserve(length);
			word w{ 0x01, 0x00, 0x00, 0x00 };
			std::generate_n(std::back_inserter(Rcon), length, [&w]() {
				word temp = w;
				w[0].xtime();
				return temp; });
			return Rcon;
		}
	private:
		static std::vector<word> convert_byte_to_word_array(const std::vector<byte>& byte_array) {
			word w;
			std::vector<word> result;
			result.reserve(byte_array.size() / 4);
			for (int i = 0; i < byte_array.size(); ++i) {
				if (i != 0 && (i % 4 == 0)) {
					result.push_back(w);
				}
				w[i % 4] = byte_array[i];
			}
			result.push_back(w);
			return result;
		}
	};

}
#endif