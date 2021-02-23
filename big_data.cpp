

#include "big_data.h"


BigData::BigData() {
}
BigData::BigData(unsigned int n) {
	if (n != 0) {
		data_[0] = n;
		bit32_num_ = 1;
		for (int i = 31; i >= 0; i--) {
			if ((n >> i) == 1) {
				bit_num_ = i + 1;
				break;
			}
		}
	}
}
BigData::BigData(unsigned int r, BigData d) {
	BigData res;
	res.data_[r / 32] = 1 << (r % 32);
	res.SetBitNum();
	res = res * d;
	res.data_[0] |= 1;  // +1
	SetBitNum();
	*this = res;
}
BigData::BigData(const unsigned char a[64]) {
	for (int i = 0; i < 16; ++i) {
		data_[i] |= a[i << 2];
		data_[i] |= a[(i << 2) + 1] << 8;
		data_[i] |= a[(i << 2) + 2] << 16;
		data_[i] |= a[(i << 2) + 3] << 24;
	}
	SetBitNum();
}
BigData::~BigData() {

}

void BigData::GetRD(unsigned int& r, BigData& d) {
	r = 0;
	BigData tmp = *this;
	tmp.data_[0] -= 1;
	for (int i = 0; i < 16; ++i) {
		for (int j = 0; j < 32; ++j) {
			if (((tmp.data_[i] >> j) & 1) == 1) {
				r += j;
				d = tmp >> r;
				d.SetBitNum();
				return;
			}
		}
		r += 32;
	}
}

void BigData::SetBitNum() {
	// 更新位数
	int len = 32;
	for (; len >= 1; len--) if (data_[len - 1] != 0) break;
	bit32_num_ = len;
	if (len == 0) {
		bit_num_ = 0;
		return;
	}
	int num = (bit32_num_ - 1) * 32;
	for (int i = 31; i >= 0; i--)
		if ((data_[bit32_num_ - 1] >> i) == 1) {
			bit_num_ = num + i + 1;
			return;
		}
}

BigData BigData::operator<<(int n) {
	BigData res = *this;
	if (n == 0) return res;

	// 先块内移位
	int mod = n % 32;
	if (mod != 0) {
		unsigned int last_over_bits = 0;  // 分别是上块、本块溢出的移位
		unsigned int this_over_bits = 0;
		for (int i = 0; i < bit32_num_; i++) {
			this_over_bits = data_[i] >> (32 - mod);
			res.data_[i] = (data_[i] << mod) | last_over_bits;
			last_over_bits = this_over_bits;
		}
		if (bit32_num_ < 32 && last_over_bits != 0)
			res.data_[bit32_num_] = last_over_bits;
	}

	// 在块间移位(得倒着来不然会覆盖)
	res.SetBitNum();
	int quotient = n / 32;
	if (quotient != 0) {
		for (int i = res.bit32_num_ - 1; i >= 0; i--)  res.data_[i + quotient] = res.data_[i];
		for (int i = 0; i < quotient; i++) res.data_[i] = 0;
	}

	// 更新位数
	res.SetBitNum();
	return res;
}

BigData BigData::operator>>(int n) {
	BigData res = *this;
	if (n == 0) return res;

	// 先块内移位
	int mod = n % 32;
	if (n != 0) {
		unsigned int last_over_bits = 0;  // 分别是上块、本块溢出的移位
		unsigned int this_over_bits = 0;
		for (int i = bit32_num_ - 1; i >= 0; i--) {
			this_over_bits = data_[i] << (32 - mod);
			res.data_[i] = (data_[i] >> mod) | last_over_bits;
			last_over_bits = this_over_bits;
		}
	}
	// 在块间移位(从低到高)
	res.SetBitNum();
	int quotient = n / 32;
	if (quotient != 0) {
		if (quotient >= res.bit32_num_) return BigData();
		for (int i = 0; i < bit32_num_; i++) res.data_[i] = res.data_[i + quotient];
	}

	// 更新位数
	res.SetBitNum();
	return res;
}

bool BigData::operator>(const BigData& a) {
	if (bit_num_ > a.bit_num_) return true;
	if (bit_num_ < a.bit_num_) return false;
	for (int i = bit32_num_ - 1; i >= 0; i--) {
		if (data_[i] > a.data_[i]) return true;
		if (data_[i] < a.data_[i]) return false;
	}
	// 两个大数相等
	return false;
}

bool BigData::operator==(const BigData& a) {
	if (bit_num_ != a.bit_num_) return false;
	for (int i = bit32_num_ - 1; i >= 0; i--)
		if (data_[i] != a.data_[i]) return false;
	return true;
}

BigData BigData::operator+(const BigData& a) {
	BigData c;
	unsigned long long int carry = 0;  // 每次的进位
	unsigned long long int c_64;  // 32位加32位需要64位来存储
	for (int i = 0; i < 32; i++) {
		c_64 = (unsigned long long int)data_[i] + (unsigned long long int)a.data_[i] + carry;
		if (c_64 > 0xffffffff) carry = 1;
		else carry = 0;
		c.data_[i] = (unsigned int)c_64;
	}
	// 更新位数
	c.SetBitNum();
	return c;
}

BigData BigData::operator-(const BigData& a) {
	BigData c;
	unsigned int carry = 0;  // 每次的借位
	for (int i = 0; i < bit32_num_; i++) {
		if ((data_[i] > a.data_[i]) || ((data_[i] == a.data_[i]) && carry == 0)) {
			c.data_[i] = data_[i] - a.data_[i] - carry;
			carry = 0;
		}
		else {  // 否则需要借位
			unsigned int tmp = (unsigned int)0xffffffff - a.data_[i];
			c.data_[i] = tmp + data_[i] + (unsigned int)1 - carry;  // 这样可以保证在32位内
			carry = 1;
		}
	}
	// 更新位数
	c.SetBitNum();
	return c;
}

BigData BigData::operator/(BigData& a) {
	// 循环尝试。（移位，减法……）
	BigData res, tmp_a;
	BigData tmp = *this;
	while (!(a > tmp)) {
		int bit_num = tmp.bit_num_ - a.bit_num_;

		tmp_a = a << bit_num;
		if (tmp_a > tmp) {
			bit_num -= 1;
			tmp_a = a << bit_num;
		}
		tmp = tmp - tmp_a;
		BigData zero;
		zero.data_[bit_num / 32] = 1 << (bit_num % 32);
		res = res + zero;
	}
	return res;
}

BigData BigData::operator%(BigData a) {
	// 循环尝试。（移位，减法……）
	BigData tmp_a;
	BigData res = *this;
	while (!(a > res)) {
		int bit_num = res.bit_num_ - a.bit_num_;
		tmp_a = a << bit_num;
		if (tmp_a > res) {
			tmp_a = a << (--bit_num);
		}
		res = res - tmp_a;
	}
	res.SetBitNum();
	return res;
}

BigData BigData::operator*(const BigData& a) {
	// 移位然后做加法
	BigData res;
	int shift_num = 0;
	for (int i = 0; i < a.bit32_num_; ++i)
		for (int j = 0; j < 32; ++j, ++shift_num)
			if (((a.data_[i] >> j) & 1) == 1)
				res = res + (*this << shift_num);
	res.SetBitNum();
	return res;
}

BigData BigData::PowerMod(BigData a, BigData power, BigData& mod) {
	BigData res(1); // res初始为1
	BigData zero;       // 0
	BigData one = res;  // 1
	while (power > zero) {
		if ((power.data_[0] & 1) == 0) {  // 偶数
			a = (a * a) % mod;
			power = power >> 1;
		}
		else {
			res = (res * a) % mod;
			power = power - one;
		}
	}
	return res;
}
