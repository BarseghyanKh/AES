#ifndef BYTE_H
#define BYTE_H

#include "headers.h"
//#include "random_functions.h"


namespace my_cryptography_lib {
	class s_box;
	class byte {
		friend std::ostream& operator<<(std::ostream& out, const byte& rhs);
		std::vector<byte> extended_euclidean() const;
		std::vector<byte> extended_euclidean(const byte& rhs) const;
	public:
		byte(uint8_t value = 0x00)
			:m_value(value){}
		explicit byte(int value)
			:m_value(static_cast<std::uint8_t>(value)) {}
		byte& operator+=(const byte& rhs) {
			m_value ^= rhs.m_value;
			return *this;
		}
		byte operator+(const byte& rhs)  const{
			byte result = *this;
			return result += rhs;
		}
		byte& operator-=(const byte& rhs) {
			return (*this) += rhs;
		}
		byte operator-(const byte& rhs) {
			byte result = *this;
			return result -= rhs;
		}
		byte& xtime() {
			m_value = (m_value << 1) ^ ((m_value >> 7) ? identity : 0);
			return *this;
		}
		byte& operator*=(const byte& rhs);
		byte operator*(const byte& rhs) const {
			byte result(*this);
			return result *= rhs;
		}
		byte& operator<<=(const std::size_t count) {
			m_value <<= count;
			return *this;
		}
		byte operator<<(const std::size_t count) const {
			byte temp(*this);
			return temp <<= count;
		}
		std::pair<byte, byte> id_divisor() const;
		byte& operator/=(const byte& rhs); 
		byte operator/(const byte& rhs) const {
			byte result(*this);
			return result /= rhs;
		}
		bool operator<(const byte& rhs)  const{
			return m_value < rhs.m_value;
		}
		bool operator<=(const byte& rhs) const {
			return m_value <= rhs.m_value;
		}
		bool operator>(const byte& rhs) const {
			return m_value > rhs.m_value;
		}
		bool operator>=(const byte& rhs) const {
			return m_value >= rhs.m_value;
		}
		bool operator==(const byte& rhs) const {
			return m_value == rhs.m_value;
		}
		bool operator!=(const byte& rhs) const {
			return m_value != rhs.m_value;
		}

		byte& operator&=(const byte& rhs) {
			m_value &= rhs.m_value;
			return *this;
		}
		byte operator&(const byte rhs) const {
			byte temp(*this);
			return temp &= rhs;
		}
		explicit operator int() {
			return static_cast<int>(m_value);
		}
		int deg() const {
			std::uint8_t temp = m_value;
			int count = 0;
			while (temp != 0) {
				temp >>= 1;
				++count;
			}
			return count - 1;
		}
		byte get_inverse() const;
		byte apply_matrix_for_S_box() const;
		std::uint8_t get() const {
			return m_value;
		}

		byte SubByte() const;
		byte InverseSubByte() const;
	private:
		static const uint8_t identity;
		std::uint8_t m_value;
		static const std::vector<byte> matrix_for_S_box;
		static const byte c;

		static const s_box* box_ptr;
		static const s_box* inverse_box_ptr;
	};
}

#endif // BYTE_H