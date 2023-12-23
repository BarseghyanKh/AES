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

	template <std::size_t Bits_count>
	class Aes {
	private:
		// AES type 128, 192, 256
		static constexpr AES_type<Bits_count> m_value = {};

		std::vector<word> cypher_key;
		std::vector<word> plaintext;
		std::vector<word> ciphertext;
	public:
		Aes()
			//:plaintext{ word(), word(), word(), word() }
		{
			std::generate_n(back_inserter(plaintext), m_value.Nk, []() {return word(); });
			std::generate_n(back_inserter(cypher_key), m_value.Nk, []() {return word(); });
		}

		Aes(const std::vector<byte>& input, const std::vector<byte>& key) {
			if (Bits_count != key.size() * 8) {
				throw std::invalid_argument("Key size must match template size");
			}
			if (16 != input.size()) {
				throw std::invalid_argument("Plaintext must be 128 bits");
			}


			plaintext = convert_byte_to_word_array(input);
			cypher_key = convert_byte_to_word_array(key);
		}
		void print_plaintext() const {
			std::copy(plaintext.begin(), plaintext.end(), std::ostream_iterator<word>(std::cout << "\n"));
		}

		void print_state(const std::vector<word>& state) const {
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
			ciphertext = state;
			return state;
		}

		void ShiftRows(std::vector<word>& state) const{
			std::vector<word> result(state.size());
			int word_size = 4;
			for (int r = 0; r < word_size; ++r) {
				for (int c = 0; c < state.size(); ++c) {
					result[c][r] = state[(c + r) % m_value.Nb][r];
				}
			}
			std::swap(state, result);
		}
		void SubBytes(std::vector<word>& state) const{
			std::vector<word> state_result;
			state_result.reserve(state.size());
			std::transform(state.begin(), state.end(), std::back_inserter(state_result), std::mem_fn(&word::SubWord));
			std::swap(state, state_result);
		}
		void MixColumns(std::vector<word>& state) const{
			std::vector<word> state_result;
			word a({ 0x02, 0x01, 0x01, 0x03 });
			std::transform(state.begin(), state.end(), std::back_inserter(state_result),
				[&a](const word& w) { return a.modular_product(w); });
			std::swap(state, state_result);
		}

		word RotWord(const word& w) const{
			return w.RotWord();
		}
		word SubWord(const word& w) const{
			return w.SubWord();
		}

		void AddRountKey(std::vector<word>& state, std::vector<word> round_key)const {
			
			transform(state.begin(), state.end(), round_key.begin(), state.begin(),
				std::plus<word>());
		
		}
		std::vector<word> KeyExpnsion() const {
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
		std::vector<word> generate_Rcon() const {
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

		std::vector<word> InvCipher() const{
			std::vector<word> w = KeyExpnsion();
			std::vector<word> state = ciphertext;
			std::vector<word> round_key(w.begin() + (m_value.Nr * m_value.Nb), w.begin() + (m_value.Nr + 1) * m_value.Nb);
			AddRountKey(state, round_key);
			for (int round = m_value.Nr - 1; round > 0; --round) {
				InvShiftRows(state);
				InvSubBytes(state);
				std::copy(w.begin() + (round * m_value.Nb), w.begin() + (round + 1) * m_value.Nb, round_key.begin());
				AddRountKey(state, round_key);
				InvMixColumns(state);
			}
			InvShiftRows(state);
			InvSubBytes(state);
			std::copy(w.begin(), w.begin() + m_value.Nb, round_key.begin());
			AddRountKey(state, round_key);
			return state;
		}
		void InvShiftRows(std::vector<word>& state) const {
			std::vector<word> result(state.size());
			int word_size = 4;
			for (int r = 0; r < word_size; ++r) {
				for (int c = 0; c < state.size(); ++c) {
					result[(c + r) % m_value.Nb][r] = state[c][r];
				}
			}
			std::swap(state, result);
		}
		void InvSubBytes(std::vector<word>& state) const  {
			std::vector<word> state_result;
			state_result.reserve(state.size());
			std::transform(state.begin(), state.end(), std::back_inserter(state_result), std::mem_fn(&word::InverseSubWord));
			std::swap(state, state_result);
		}
		void InvMixColumns(std::vector<word>& state)const  {
			std::vector<word> state_result;
			word a({ 0x02, 0x01, 0x01, 0x03 });
			word a_inverse({ 0x0e, 0x09, 0x0d, 0x0b });
			std::transform(state.begin(), state.end(), std::back_inserter(state_result),
				[&a_inverse](const word& w) { return a_inverse.modular_product(w); });
			std::swap(state, state_result);
		}

		std::vector<word> EqKeyExpnsion() const {
			std::vector<word> Rcon = generate_Rcon();
			word temp;
			std::vector<word> w;
			w.reserve(m_value.Nb * (m_value.Nr + 1));
			std::copy(cypher_key.begin(), cypher_key.end(), std::back_inserter(w));

			int i = m_value.Nk;
			while (i < m_value.Nb * (m_value.Nr + 1)) {
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
			std::vector<word> dw;
			dw.reserve(m_value.Nb * (m_value.Nr + 1));
			std::copy(w.begin(), w.begin() + (m_value.Nr + 1) * m_value.Nb, std::back_inserter(dw));
			for (int round = 1; round < m_value.Nr; ++round) {
				std::vector<word> temp_dw;
				temp_dw.reserve(m_value.Nb);
				std::copy(dw.begin() + (round * m_value.Nb), dw.begin() + (round + 1) * m_value.Nb, std::back_inserter( temp_dw));
				InvMixColumns(temp_dw);
				std::copy(temp_dw.begin(), temp_dw.end(), dw.begin() + (round * m_value.Nb));
			}
			return dw;
		}
		std::vector<word> EqInvCipher() const{
			std::vector<word> dw = EqKeyExpnsion();
			std::vector<word> state = ciphertext;
			std::vector<word> round_key(dw.begin() + (m_value.Nr * m_value.Nb), dw.begin() + (m_value.Nr + 1) * m_value.Nb);
			AddRountKey(state, round_key);
			for (int round = m_value.Nr - 1; round > 0; --round) {
				InvSubBytes(state);
				InvShiftRows(state);
				InvMixColumns(state);
				std::copy(dw.begin() + (round * m_value.Nb), dw.begin() + (round + 1) * m_value.Nb, round_key.begin());
				AddRountKey(state, round_key);
			}
			InvSubBytes(state);
			InvShiftRows(state);
			std::copy(dw.begin(), dw.begin() + m_value.Nb, round_key.begin());
			AddRountKey(state, round_key);
			return state;
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