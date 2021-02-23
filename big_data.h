#ifndef _BIG_DATA_H_
#define _BIG_DATA_H_

// ！！！针对至多512bit位的大数公钥(即加减乘除都限制在1024位以内)
// 输入输出均用10进制字符串表示

#include <iostream>
using namespace std;
class BigData {
public:
	// 注意！！！虽然这里可放1024bit位，实际上请输入至多512bit位的数,1024是为了便于表示运算的中间结果
	unsigned int data_[32] = { 0 };  // 存放2^32进制的大数，这样32*32位就可以表示1024位的大数，低数组存放低位
	int bit32_num_ = 0;  // 此大数的"位数"，从1开始，注意不是二进制bit位数，每一"位"代表2^32即32个bit位
	int bit_num_ = 0;  // 此大数的二进制位数

public:
	BigData();
	BigData(unsigned int n);
	// n - 1 = (2 ^ r) * d
	BigData(unsigned int r, BigData d);
	// 64字节 转为 16 * 32位的大数 低位低字节
	BigData(const unsigned char a[64]);
	~BigData();

	// 根据大数计算大数的r和d，大数n - 1 = (2 ^ r) * d
	void GetRD(unsigned int& r, BigData& d);

	// 更新大数的位数
	void SetBitNum();

	// 大数左移(增大)
	BigData operator<<(int n);

	// 大数右移(减小)
	BigData operator>>(int n);

	// 大数大于
	bool operator>(const BigData& a);

	// 大数相等
	bool operator==(const BigData& a);

	// 大数加法
	BigData operator+(const BigData& a);

	// 大数减法(只考虑减去不比自己大的数)
	BigData operator-(const BigData& a);

	// 大数除法
	BigData operator/(BigData& a);

	// 大数取模
	BigData operator%(BigData a);

	// 大数乘法
	BigData operator*(const BigData& a);

	// 快速幂模
	static BigData PowerMod(BigData a, BigData power, BigData& mod);

};


#endif // !_BIG_DATA_H_