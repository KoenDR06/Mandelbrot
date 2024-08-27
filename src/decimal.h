#include <iostream>
#include <complex>

class decimal {
	public:
		bool sign = false;
		unsigned long long int number = 0;
		decimal(bool s, unsigned long long n) {
			sign = s;
			number = n;
		}
        decimal() {
            sign = false;
            number = 0;
        }
		explicit decimal(double d) {
		    unsigned long long bits = *reinterpret_cast<unsigned long long*>(&d);
            int signBit = bits >> 63;
            int exponent = ((bits >> 52) & 0x7FF) - 1023;
            unsigned long long mantissa = (bits & 0xFFFFFFFFFFFFF) + 4503599627370496;

			if (exponent > 0) {
				throw std::invalid_argument("double constructor out of range");
			}

			if (signBit == 1) {
				sign = true;
			}

			number = mantissa << 11;
			number >>= -1 * exponent + 1;
		}
        explicit decimal(long long int n) {
            if (n > 3 || n < -3) {
                throw std::invalid_argument("integer constructor out of range");
            }

            if (n < 0) {
                sign = true;
                n *= -1;
            }
            number = n << 62;
        }

		decimal operator+(decimal other) const {
			decimal result;
			if (!this->sign && !other.sign) {           // Both are negative
				result.number = this->number + other.number;
				result.sign = false;
			} else if (!this->sign) {                   // this is negative, other is positive
				result.number = this->number - other.number;
				if (this->number >= other.number) {
					result.sign = false;
				} else {
					result.sign = true;
				}
			} else if (!other.sign) {                   // this is positive, other is negative
				result.number = other.number - this->number;
				if (other.number >= this->number) {
					result.sign = false;
				} else {
					result.sign = true;
				}
			} else {                                    // Both are positive
				result.number = this->number + other.number;
				result.sign = true;
			}

			return result;
		}
		decimal operator-(decimal other) const {
            other.sign = !other.sign;
			return decimal(this->sign, this->number) + other;
		}
		decimal operator*(decimal other) const {
            decimal result;
            result.sign = !this->sign != !other.sign;

            __int128 res = static_cast<__int128>(this->number) * static_cast<__int128>(other.number);
            result.number = static_cast<unsigned long long>(res >> 62);

            return result;
        }
        decimal operator/(decimal other) const {
            decimal result;
            result.sign = this->sign xor result.sign;

            result.number = (this->number / other.number) << 62;

            return result;
        }
        bool operator==(decimal other) const {
            if (this->sign == other.sign && this->number == other.number) {
                return true;
            }
            return false;
        }

        [[nodiscard]] double to_double() const {
            double result = 0;

            int i = 63;
            while (i >= 0) {
                int bit = (this->number >> i--) & 0x1;
                if (bit == 1) {
                    result += std::exp(std::log(2) * (i-61));
                }
            }
            if (sign) {
                result *= -1;
            }
            return result;
        }
};

std::ostream &operator<<(std::ostream &os, decimal const &dec) {
    return os << dec.to_double();
}