#include <iostream>

class decimal {
	public:
		bool sign = false;
		unsigned long long number = 0;
		decimal(bool s, unsigned long long n) {
			sign = s;
			number = n;
		}
		decimal(double d) {
		    unsigned long long bits = *reinterpret_cast<unsigned long long*>(&d);
			int signBit = (bits >> 63) & 0x1;
			int exponent = ((bits >> 52) & 0x7FF) - 1023;
			unsigned long long mantissa = (bits & 0xFFFFFFFFFFFFF) + 4503599627370496;

			if (exponent > 0) {
				throw std::invalid_argument("double constructor out of range");
			}

			if (signBit == 1) {
				sign = true;
			}

			number = mantissa << 11;
			number >>= -1 * exponent;
		}
		decimal() {
			sign = false;
			number = 0;
		}


		static decimal add(decimal a, decimal b) {
			decimal result;
			if (a.sign == false && b.sign == false) {
				result.number = a.number + b.number;
				result.sign = false;
			} else if (a.sign == false && b.sign == true) {
				result.number = a.number - b.number;
				if (a.number >= b.number) {
					result.sign = false;
				} else {
					result.sign = true;
				}
			} else if (a.sign == true && b.sign == false) {
				result.number = b.number - a.number;
				if (b.number >= a.number) {
					result.sign = false;
				} else {
					result.sign = true;
				}
			} else if (a.sign == true && b.sign == true) {
				result.number = a.number + b.number;
				result.sign = true;
			}

			return result;
		}

		static decimal sub(decimal a, decimal b) {
			b.sign = !b.sign;
			return add(a, b);
		}

		static decimal mul(decimal a, decimal b) {
			decimal result;
			result.sign = !a.sign != !b.sign;

		    __int128 res = static_cast<__int128>(a.number) * static_cast<__int128>(b.number);
			result.number = static_cast<unsigned long long>(res >> 63);

			return result;
		}
};
