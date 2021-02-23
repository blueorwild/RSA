#ifndef _RSA_H_
#define _ESA_H_

#include <math.h>
#include <vector>
#include <random>
#include <time.h>

#include "big_data.h"

using std::default_random_engine;
using std::uniform_int_distribution;

// RSA算法 需要输入质数P、Q，整数E(E有限制条件，后续说)
// 公钥(N, E) => N=P*Q
// 私钥(N, D) => M=欧拉(N), D=E关于M的乘法逆元, 
// E的限制条件(1<E<M,E与M互素,另外若E=M-1,则公钥=密钥 )

// 设a明文,b密文（需要转为数字，且不能大于N）
// 加密： b = a ^ E % N
// 解密： a = b ^ D % N

using std::vector;

class Rsa {
private:
	// 具体的大写字母代表的意思参见上述关于RSA的解释
	BigData prime_num_P_;  // RSA算法所需质数一
	BigData prime_num_Q_;  // RSA算法所需质数二
	BigData public_key_E_;    // RSA算法所需整数(公钥之一)

	BigData public_key_N_;    // RSA算法的公钥之二
	BigData private_key_D_;   // RSA算法的私钥之一
	BigData oula_M_;       // N的欧拉函数值

	// 计算逆元(私钥)
	void SetInverseElement();

	// 生成指定位数的素数
	// 先生成随机数为n, n-1 = (2^r)*d
	BigData CreatePrime(int bit_num);

	// miller-robin方法检测是否为素数
	// 待检测数为n, n-1 = (2^r)*d, 测试次数为k
	bool IsPrime(BigData n, int r, BigData d, int k);

public:
	Rsa(unsigned int e, int p_bits, int q_bits);
	~Rsa();
	// 获取两个素数
	BigData GetPrimeP();
	BigData GetPrimeQ();
	// 获取公钥N/E
	BigData GetPublicKeyN();
	BigData GetPublicKeyE();
	// 获取私钥D
	BigData GetPrivateKeyD();

	// 加密(按照512位的公钥N来计算，即每一字节明文加密后的密文为64字节 16 * 32位的大数)
	unsigned char* Encode(const char* cleartext);
	// 解密
	char* Decode(const unsigned char* ciphertext, int ciphertext_len);
};

#endif // ! _RSA_H_
