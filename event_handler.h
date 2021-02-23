#ifndef _EVENT_HANDLER_H_
#define _EVENT_HANDLER_H_

#include <QObject>
#include <QString>
#include "math.h"
#include <iostream>

#include "big_data.h"
#include "rsa.h"

class EventHandler : public QObject {
	Q_OBJECT
private:
	Rsa* rsa_;

	// 检测一个数是否为质数
	bool IsPrime(int num);
	// 把一个大数BigData转为十六进制字符串（前缀"0X"）
	const char* BigDataToDex(BigData& a);

public:
	EventHandler(QObject* parent = NULL);
	~EventHandler();
	// 关联信号
	void Init();

	// 生成rsa的一堆钥
	void GenerateKey(int p_bits, int q_bits, unsigned int e);
	// 加密
	void Encode(QString& cleartext);
	// 解密
	void Decode(const unsigned char* ciphertext, int ciphertext_len);

signals:
	void GenerateKey(QString& P, QString& Q, QString& N, QString& D);
	void EncodeResult(const unsigned char* ciphertext);
	void DecodeResult(QString& cleartext);

public slots:

};


#endif // !_EVENT_HANDLER_H_

