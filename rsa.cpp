
#include "big_data.h"
#include "rsa.h"

using std::vector;

// 1000 以内的素数（除2外）
const int a[] = { 3 ,5, 7, 11,13 ,17 ,19 ,23 ,29,
	31, 37, 41 ,43, 47, 53 ,59, 61 ,67, 71,
	73, 79, 83 ,89 ,97 ,101 ,103, 107, 109 ,113,
	127, 131 ,137, 139 ,149 ,151, 157, 163 ,167 ,173,
	179 ,181 ,191, 193 ,197 ,199, 211, 223 ,227 ,229,
	233 ,239 ,241, 251 ,257 ,263, 269, 271 ,277 ,281,
	283 ,293 ,307, 311 ,313 ,317, 331, 337 ,347 ,349,
	353 ,359 ,367, 373 ,379 ,383, 389, 397 ,401 ,409,
	419 ,421 ,431, 433 ,439 ,443, 449, 457 ,461 ,463,
	467 ,479 ,487, 491 ,499 ,503, 509, 521 ,523 ,541,
	547 ,557 ,563, 569 ,571 ,577, 587, 593 ,599 ,601,
	607 ,613 ,617, 619 ,631 ,641, 643, 647 ,653 ,659,
	661 ,673 ,677, 683 ,691 ,701, 709, 719 ,727 ,733,
	739 ,743 ,751, 757 ,761 ,769, 773, 787 ,797 ,809,
	811 ,821 ,823, 827 ,829 ,839, 853, 857 ,859 ,863,
	877 ,881 ,883, 887 ,907 ,911, 919, 929 ,937 ,941,
	947 ,953 ,967, 971 ,977 ,983, 991, 997
};

void Rsa::SetInverseElement() {
	BigData tmp_E = public_key_E_, tmp_M = oula_M_;
	if (tmp_E > tmp_M) tmp_M = tmp_M + tmp_E;

	vector<BigData> quotient;  // 商
	BigData mod = tmp_M % tmp_E;
	while (!(mod == (unsigned int)0)) {
		quotient.push_back(tmp_M / tmp_E);
		tmp_M = tmp_E;
		tmp_E = mod;
		mod = tmp_M % tmp_E;
	}
	BigData x(1), y(quotient[quotient.size() - 1]);
	BigData tmp;
	int len = quotient.size() - 2;
	for (int i = len; i >= 0; --i) {
		tmp = x;
		x = y;
		y = tmp + y * quotient[i];
	}
	if (public_key_E_ > oula_M_) private_key_D_ = x;
	else if (len % 2 == 1) private_key_D_ = oula_M_ - y;
	else private_key_D_ = y;
}

BigData Rsa::CreatePrime(int bit_num) {
	unsigned int r = bit_num - 1;
	BigData d(1);
	BigData n(r, d);
	while (!IsPrime(n, r, d, 5)) { // 一般通过测试5次即可认为是素数
		n = n + 2;
		n.GetRD(r, d);
	}
	return n;
}

bool Rsa::IsPrime(BigData n, int r, BigData d, int k) {
	BigData tmp;
	// 先排除1000以内的
	for (unsigned int i = 0; i < 167; ++i) {
		tmp = BigData(a[i]);
		if ((n % tmp).bit_num_ == 0) return false;
	}
	// 再来其它测试
	default_random_engine rand((unsigned)time(NULL));
	for (int i = 0; i < k; ++i) {
		unsigned int random = rand();
		tmp = BigData::PowerMod(random, d, n);
		if (tmp == 1 || tmp == (n - 1)) continue;  // 此次测试通过
		for (int j = 1; j < r; ++j) {
			tmp = (tmp * tmp) % n;
			if (tmp == (n - 1)) goto test; // 此次测试通过
		}
		return false;
	test:;
	}
	return true;  // 极大概率是素数

}

Rsa::Rsa(unsigned int e, int p_bits, int q_bits) {
	prime_num_P_ = CreatePrime(p_bits);
	prime_num_Q_ = CreatePrime(q_bits);
	public_key_N_ = prime_num_P_ * prime_num_Q_;
	oula_M_ = (prime_num_P_ - 1) * (prime_num_Q_ - 1);
	public_key_E_ = e;
	SetInverseElement();
};
Rsa::~Rsa() {};

BigData Rsa::GetPrimeP() {
	return prime_num_P_;
}
BigData Rsa::GetPrimeQ() {
	return prime_num_Q_;
}
BigData Rsa::GetPublicKeyN() {
	return public_key_N_;
}
BigData Rsa::GetPublicKeyE() {
	return public_key_E_;
}
BigData Rsa::GetPrivateKeyD() {
	return private_key_D_;
}

unsigned char* Rsa::Encode(const char* cleartext) {
	int len = strlen(cleartext);
	unsigned char* ciphertext = new unsigned char[len << 6];
	for (int i = (len << 6) - 1; i >= 0; --i) ciphertext[i] = '\0';
	BigData tmp;
	for (int i = 0; i < len; ++i) {
		tmp = BigData::PowerMod((unsigned int)(unsigned char)cleartext[i], public_key_E_, public_key_N_);
		for (int j = 0; j < tmp.bit32_num_; ++j) {
			// 把32位的数转为4个字节(低位放低字节)
			ciphertext[(i << 6) + (j << 2)] = (unsigned char)tmp.data_[j];
			ciphertext[(i << 6) + (j << 2) + 1] = (unsigned char)(tmp.data_[j] >> 8);
			ciphertext[(i << 6) + (j << 2) + 2] = (unsigned char)(tmp.data_[j] >> 16);
			ciphertext[(i << 6) + (j << 2) + 3] = (unsigned char)(tmp.data_[j] >> 24);
		}
	}
	return ciphertext;
}

char* Rsa::Decode(const unsigned char* ciphertext, int ciphertext_len) {
	int cleartext_len = ciphertext_len >> 6;
	char* cleartext = new char[cleartext_len + 1];
	cleartext[cleartext_len] = '\0';
	BigData tmp;
	for (int i = 0; i < cleartext_len; ++i) {
		tmp = BigData(ciphertext + (i << 6));
		tmp = BigData::PowerMod(tmp, private_key_D_, public_key_N_);  // 转回来的tmp是32位以内的（实际255以内）
		cleartext[i] = (unsigned char)tmp.data_[0];
	}
	return cleartext;
}
