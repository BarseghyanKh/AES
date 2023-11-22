#include "byte.h"

namespace my_cryptography_lib {

	byte byte::get_inverse() const {
		byte a = extended_euclidean()[2];


		return a;
	}

	std::vector<byte> byte::extended_euclidean() const {
		byte h(*this);
		if (h == byte(0)) {
			// To do
			return { identity, byte(1), byte(0) };
		}

		byte s2(1), s1(0), t2(0), t1(1), q, r, s, t, d;

		// 3.1
		auto result = h.id_divisor();
		q = result.first;
		r = result.second;
		//auto [q, r] =
		// 3.2
		s = s2 - q * s1; // s = s2 - q * s1
		t = t2 - q * t1; // t = t2 - q * t1;
		// 3.3
		byte g = h;
		h = r;
		// 3.4
		s2 = s1;
		s1 = s;
		t2 = t1;
		t1 = t;

		while (h != byte(0)) {
			// 3.1
			q = g / h; // q = g div h;
			r = g - h * q; //r = g - h * q;
			// 3.2
			s = s2 - q * s1; // s = s2 - q * s1
			t = t2 - q * t1; // t = t2 - q * t1;
			// 3.3
			g = h;
			h = r;
			// 3.4
			s2 = s1;
			s1 = s;
			t2 = t1;
			t1 = t;
		}
		d = g;
		s = s2;
		t = t2;
		return { d, s, t };
	}

	std::vector<byte> byte::extended_euclidean(const byte& rhs) const {
		byte g(*this);
		byte h(rhs);
		if (h == byte(0)) {
			return { g, byte(1), byte(0) };
		}

		byte s2(1), s1(0), t2(0), t1(1), q, r, s, t, d;


		while (h != byte(0)) {
			// 3.1
			q = g / h; // q = g div h;
			r = g - h * q; //r = g - h * q;
			// 3.2
			s = s2 - q * s1; // s = s2 - q * s1
			t = t2 - q * t1; // t = t2 - q * t1;
			// 3.3
			g = h;
			h = r;
			// 3.4
			s2 = s1;
			s1 = s;
			t2 = t1;
			t1 = t;
		}
		d = g;
		s = s2;
		t = t2;
		return { d, s, t };
	}

	byte& byte::operator*=(const byte& rhs) {
		const std::size_t bit_count = 8;
		byte temp(m_value);

		if ((rhs.m_value & 1) == 0) {
			m_value = 0;
		}

		for (std::size_t i = 1; i < bit_count; ++i) {
			temp.xtime();
			//std::cout << temp;

			m_value = m_value ^ (((rhs.m_value >> i) & 1) ? temp.m_value : 0);
		}
		return *this;
	}

	std::ostream& operator<<(std::ostream& out, const byte& rhs)
	{
		const int first = static_cast<int>(rhs.m_value >> 4);
		const int second = static_cast<int>(rhs.m_value & 15);
		const std::string t = "0123456789abcdef";
		out << t[first] << t[second];
		return out;
	}

	std::pair<byte, byte> byte::id_divisor() const {
		if (*this == 0) {
			exit(0);
		}

		byte r(identity);
		byte h(*this);

		std::size_t id_degree = 8;
		byte q(0);

		std::size_t digree_difference = id_degree - h.deg();
		r.m_value ^= (h.m_value << digree_difference);
		q.m_value ^= (1 << digree_difference);

		while (r.deg() >= h.deg()) {
			digree_difference = r.deg() - h.deg();
			r.m_value ^= (h.m_value << digree_difference);
			q.m_value ^= (1 << digree_difference);
		}
		return std::make_pair(q, r);
	}

	byte& byte::operator/=(const byte& rhs) {
		if (rhs == 0) {
			exit(0);
		}
		byte result(0);
		while (this->deg() >= rhs.deg()) {
			std::size_t digree_difference = static_cast<std::size_t>(deg() - rhs.deg());
			result.m_value ^= (1 << digree_difference);
			m_value ^= (rhs.m_value << digree_difference);
		}
		m_value = result.m_value;
		return *this;
	}

	byte byte::apply_matrix_for_S_box() const {
		std::vector<byte> computed(matrix_for_S_box);
		std::uint8_t b = m_value;
		return std::accumulate(computed.begin(), computed.end(), byte(c),
			[&b](byte accumulator, byte current) {
				return (b & 1) ? (b >>= 1, accumulator + current) :
					(b >>= 1, accumulator);
			});
	}


	const byte byte::c = 0x63;
	const std::vector<byte> byte::matrix_for_S_box = { 0x1f, 0x3e, 0x7c, 0xf8, 0xf1,0xe3,0xc7,0x8f };
	const uint8_t byte::identity = 0x1b;


}


