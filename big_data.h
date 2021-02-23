#ifndef _BIG_DATA_H_
#define _BIG_DATA_H_

// �������������512bitλ�Ĵ�����Կ(���Ӽ��˳���������1024λ����)
// �����������10�����ַ�����ʾ

#include <iostream>
using namespace std;
class BigData {
public:
	// ע�⣡������Ȼ����ɷ�1024bitλ��ʵ��������������512bitλ����,1024��Ϊ�˱��ڱ�ʾ������м���
	unsigned int data_[32] = { 0 };  // ���2^32���ƵĴ���������32*32λ�Ϳ��Ա�ʾ1024λ�Ĵ������������ŵ�λ
	int bit32_num_ = 0;  // �˴�����"λ��"����1��ʼ��ע�ⲻ�Ƕ�����bitλ����ÿһ"λ"����2^32��32��bitλ
	int bit_num_ = 0;  // �˴����Ķ�����λ��

public:
	BigData();
	BigData(unsigned int n);
	// n - 1 = (2 ^ r) * d
	BigData(unsigned int r, BigData d);
	// 64�ֽ� תΪ 16 * 32λ�Ĵ��� ��λ���ֽ�
	BigData(const unsigned char a[64]);
	~BigData();

	// ���ݴ������������r��d������n - 1 = (2 ^ r) * d
	void GetRD(unsigned int& r, BigData& d);

	// ���´�����λ��
	void SetBitNum();

	// ��������(����)
	BigData operator<<(int n);

	// ��������(��С)
	BigData operator>>(int n);

	// ��������
	bool operator>(const BigData& a);

	// �������
	bool operator==(const BigData& a);

	// �����ӷ�
	BigData operator+(const BigData& a);

	// ��������(ֻ���Ǽ�ȥ�����Լ������)
	BigData operator-(const BigData& a);

	// ��������
	BigData operator/(BigData& a);

	// ����ȡģ
	BigData operator%(BigData a);

	// �����˷�
	BigData operator*(const BigData& a);

	// ������ģ
	static BigData PowerMod(BigData a, BigData power, BigData& mod);

};


#endif // !_BIG_DATA_H_