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

	// ���һ�����Ƿ�Ϊ����
	bool IsPrime(int num);
	// ��һ������BigDataתΪʮ�������ַ�����ǰ׺"0X"��
	const char* BigDataToDex(BigData& a);

public:
	EventHandler(QObject* parent = NULL);
	~EventHandler();
	// �����ź�
	void Init();

	// ����rsa��һ��Կ
	void GenerateKey(int p_bits, int q_bits, unsigned int e);
	// ����
	void Encode(QString& cleartext);
	// ����
	void Decode(const unsigned char* ciphertext, int ciphertext_len);

signals:
	void GenerateKey(QString& P, QString& Q, QString& N, QString& D);
	void EncodeResult(const unsigned char* ciphertext);
	void DecodeResult(QString& cleartext);

public slots:

};


#endif // !_EVENT_HANDLER_H_

