#ifndef WORD_H
#define WORD_H


#include "byte.h"

namespace my_cryptography_lib {

	class word {
		friend std::ostream& operator<<(std::ostream& out, const word& rhs);
	public:

		word();
		word(std::initializer_list<byte> bytes);
		word& operator+=(const word& rhs);

		word operator+(const word& rhs) const;

		word& operator*=(const word& rhs);

		word operator*(const word& rhs) const;
		word modular_product(const word& rhs) const;

		byte& operator[](std::size_t index);


	private:
		std::vector<byte> m_value;
	};
}

#endif // WORD_H