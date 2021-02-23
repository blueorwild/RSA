#pragma execution_character_set("utf-8")
#ifndef _RSA_UI_H_
#define _RSA_UI_H_

#include <QObject>
#include <QWidget>

#include "ui_rsa.h"
#include "event_handler.h"

class RsaUi : public QWidget {
	Q_OBJECT
private:
	Ui::Form* ui_;   // ����.ui�ļ�������
	EventHandler* event_handler_;

	int ciphertext_len_;  // ���ĳ���
	unsigned char* ciphertext_;  // ���ģ���ʾ��QString���ͺ�̨����(unsigned char*)�ֿ���

public:
	RsaUi(QWidget* parent = NULL);
	~RsaUi();

	// ����ǰ���ź�
	void Init();

signals:

private slots:
	// ����rsa��һ��Կ
	void GenerateKey();
	// ����
	void Encode();
	// ����
	void Decode();

public slots:
	void ShowKey(QString& P, QString& Q, QString& N, QString& D);
	void ShowCiphertext(const unsigned char* ciphertext);
	void ShowCleartext(QString& cleartext);

};

#endif // !_RSA_UI_H_

