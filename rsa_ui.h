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
	Ui::Form* ui_;   // 代表.ui文件的内容
	EventHandler* event_handler_;

	int ciphertext_len_;  // 密文长度
	unsigned char* ciphertext_;  // 密文（显示（QString）和后台处理(unsigned char*)分开）

public:
	RsaUi(QWidget* parent = NULL);
	~RsaUi();

	// 关联前端信号
	void Init();

signals:

private slots:
	// 生成rsa的一堆钥
	void GenerateKey();
	// 加密
	void Encode();
	// 解密
	void Decode();

public slots:
	void ShowKey(QString& P, QString& Q, QString& N, QString& D);
	void ShowCiphertext(const unsigned char* ciphertext);
	void ShowCleartext(QString& cleartext);

};

#endif // !_RSA_UI_H_

