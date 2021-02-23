
#include <iostream>
#include "event_handler.h"

EventHandler::EventHandler(QObject* parent):QObject(parent) {
}
EventHandler::~EventHandler() {
}

void EventHandler::Init() {
}

bool EventHandler::IsPrime(int num) {
	int num_sqrt = (int)sqrt(num);
	for (int i = 2; i < num_sqrt; ++i)
		if ((num % i) == 0) return false;
	return true;
}

const char* EventHandler::BigDataToDex(BigData& a) {
	char* str = new char[32 * 8 + 2]();  // 加()初始化'\0'
	str[0] = '0'; str[1] = 'X';
	for (int i = a.bit32_num_ - 1, j = 2; i >= 0; --i) {
		unsigned int cur_num = a.data_[i];
		for (int k = 28; k >= 0; k -= 4) {
			int tmp = ((cur_num << (28 - k)) >> 28);  // 每次取四位
			str[j++] = tmp < 10 ? (char)(tmp + 48) : (char)(tmp - 10 + 65);
		}
	}
	return str;
}

void EventHandler::GenerateKey(int p_bits, int q_bits, unsigned int e) {
	if (p_bits < 0 || p_bits > 256 || q_bits < 0 || q_bits > 256 || !IsPrime(e)) return;
	rsa_ = new Rsa(e, p_bits, q_bits);
	const char* p_str = BigDataToDex(rsa_->GetPrimeP());
	const char* q_str = BigDataToDex(rsa_->GetPrimeQ());
	const char* n_str = BigDataToDex(rsa_->GetPublicKeyN());
	const char* d_str = BigDataToDex(rsa_->GetPrivateKeyD());
	int len = 32 * 8 + 2;
	emit GenerateKey(QString(QByteArray(p_str, len)), QString(QByteArray(q_str, len)),
		QString(QByteArray(n_str, len)), QString(QByteArray(d_str, len)));
	delete[] p_str;
	delete[] q_str;
	delete[] n_str;
	delete[] d_str;
}

void EventHandler::Encode(QString& cleartext) {
	const unsigned char* ciphertext = rsa_->Encode(cleartext.toStdString().c_str());
	emit EncodeResult(ciphertext);
}

void EventHandler::Decode(const unsigned char* ciphertext, int ciphertext_len) {
	const char* cleartext = rsa_->Decode(ciphertext, ciphertext_len);
	emit DecodeResult(QString(QByteArray(cleartext, ciphertext_len >> 6)));
	delete[] cleartext;
}