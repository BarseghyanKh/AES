#include "word.h"
#include "random_functions.h"

namespace my_cryptography_lib {

	word::word() {
		// index i corresponds a_i
		const int count = 4;
		std::generate_n(back_inserter(m_value), count, generate_random_byte());
	}
	word::word(std::initializer_list<byte> bytes) {
		m_value.assign(bytes.begin(), bytes.end());
	}

	word& word::operator+=(const word& rhs) {
		std::transform(m_value.begin(), m_value.end(), rhs.m_value.begin(), m_value.begin(),
			std::plus<decltype(m_value)::value_type>());
		return *this;
	}
	word word::operator+(const word& rhs) const {
		word result(*this);
		return result += rhs;
	}
	word& word::operator*=(const word& rhs) {
		const int max_count = 7;
		std::vector<byte> result;
		result.reserve(max_count);

		for (int i = 0; i < max_count; ++i) {
			result.push_back(std::inner_product(m_value.begin(), m_value.begin() + i + 1,
				rhs.m_value.rbegin() + (max_count - i - 1), byte(0)));
		}
		std::fill(m_value.begin(), m_value.end(), 0);
		for (int i = 0; i < result.size(); ++i) {
			int index = i % 4;
			m_value[index] += result[i];
		}
		return *this;
	}
	word word::operator*(const word& rhs) const {
		word result(*this);
		return result *= rhs;
	}
	std::ostream& operator<<(std::ostream& out, const word& rhs) {
		std::copy(rhs.m_value.begin(), rhs.m_value.end(), std::ostream_iterator<byte>(std::cout, " "));
		return out;
	}
	word word::modular_product(const word& rhs) const {
		word result{byte(0x00), byte(0x00), byte(0x00), byte(0x00)};

		for (int i = 0; i < m_value.size(); ++i) {
			for (int j = 0; j < rhs.m_value.size(); ++j) {
				int index = (i + j) % 4;
				result.m_value[index] += (m_value[i] * rhs.m_value[j]);
			}
		}
		return result;
	}
	byte& word::operator[](std::size_t index) {
		if (index < 0 || index >= m_value.size()) {
			throw std::out_of_range("Index out of range in word::operator[] !");
		}
		return m_value[index];
	}
	const byte& word::operator[](std::size_t index)const {
		return m_value[index];
	}
	word word::SubWord() const {
		word result;
		std::transform(m_value.begin(), m_value.end(), result.m_value.begin(), std::mem_fn(&byte::SubByte));
		/*for (int j = 0; j < 4; ++j) {
			result[j] = m_value[j].SubByte();
		}*/
		return result;
	}
	word word::InverseSubWord() const {
		word result;
		std::transform(m_value.begin(), m_value.end(), result.m_value.begin(), std::mem_fn(&byte::InverseSubByte));
		/*for (int j = 0; j < 4; ++j) {
			result[j] = m_value[j].InverseSubByte();
		}*/
		return result;
	}

	word word::RotWord() const {
		word result;
		std::rotate_copy(m_value.begin(), m_value.begin() + 1, m_value.end(), result.m_value.begin());
		return result;
	}
}