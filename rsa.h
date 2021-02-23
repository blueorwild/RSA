#ifndef _RSA_H_
#define _ESA_H_

#include <math.h>
#include <vector>
#include <random>
#include <time.h>

#include "big_data.h"

using std::default_random_engine;
using std::uniform_int_distribution;

// RSA�㷨 ��Ҫ��������P��Q������E(E����������������˵)
// ��Կ(N, E) => N=P*Q
// ˽Կ(N, D) => M=ŷ��(N), D=E����M�ĳ˷���Ԫ, 
// E����������(1<E<M,E��M����,������E=M-1,��Կ=��Կ )

// ��a����,b���ģ���ҪתΪ���֣��Ҳ��ܴ���N��
// ���ܣ� b = a ^ E % N
// ���ܣ� a = b ^ D % N

using std::vector;

class Rsa {
private:
	// ����Ĵ�д��ĸ�������˼�μ���������RSA�Ľ���
	BigData prime_num_P_;  // RSA�㷨��������һ
	BigData prime_num_Q_;  // RSA�㷨����������
	BigData public_key_E_;    // RSA�㷨��������(��Կ֮һ)

	BigData public_key_N_;    // RSA�㷨�Ĺ�Կ֮��
	BigData private_key_D_;   // RSA�㷨��˽Կ֮һ
	BigData oula_M_;       // N��ŷ������ֵ

	// ������Ԫ(˽Կ)
	void SetInverseElement();

	// ����ָ��λ��������
	// �����������Ϊn, n-1 = (2^r)*d
	BigData CreatePrime(int bit_num);

	// miller-robin��������Ƿ�Ϊ����
	// �������Ϊn, n-1 = (2^r)*d, ���Դ���Ϊk
	bool IsPrime(BigData n, int r, BigData d, int k);

public:
	Rsa(unsigned int e, int p_bits, int q_bits);
	~Rsa();
	// ��ȡ��������
	BigData GetPrimeP();
	BigData GetPrimeQ();
	// ��ȡ��ԿN/E
	BigData GetPublicKeyN();
	BigData GetPublicKeyE();
	// ��ȡ˽ԿD
	BigData GetPrivateKeyD();

	// ����(����512λ�Ĺ�ԿN�����㣬��ÿһ�ֽ����ļ��ܺ������Ϊ64�ֽ� 16 * 32λ�Ĵ���)
	unsigned char* Encode(const char* cleartext);
	// ����
	char* Decode(const unsigned char* ciphertext, int ciphertext_len);
};

#endif // ! _RSA_H_
