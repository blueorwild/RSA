#include <string>
#include <QByteArray>
#include <iostream>
#include "rsa_ui.h"

RsaUi::RsaUi(QWidget* parent) {
	ui_ = new Ui::Form();
	ui_->setupUi(this);
	event_handler_ = new EventHandler;
	ciphertext_ = new unsigned char[1];
	ciphertext_len_ = 0;
}
RsaUi::~RsaUi() {
	delete ui_;
	delete event_handler_;
	delete [] ciphertext_;
}

void RsaUi::Init() {
	connect(ui_->pushButton, SIGNAL(clicked()), this, SLOT(Encode()));
	connect(ui_->pushButton_2, SIGNAL(clicked()), this, SLOT(Decode()));
	connect(ui_->pushButton_3, SIGNAL(clicked()), this, SLOT(GenerateKey()));

	connect(event_handler_, SIGNAL(GenerateKey(QString&, QString&, QString&, QString&)), 
		this, SLOT(ShowKey(QString&, QString&, QString&, QString&)));
	connect(event_handler_, SIGNAL(EncodeResult(const unsigned char*)), this, SLOT(ShowCiphertext(const unsigned char*)));
	connect(event_handler_, SIGNAL(DecodeResult(QString&)), this, SLOT(ShowCleartext(QString&)));
}

void RsaUi::Encode() {
	QString text = ui_->lineEdit->text();
	if (text.isEmpty()) return;
	ciphertext_len_ = text.toStdString().length() << 6;
	event_handler_->Encode(text);
}

void RsaUi::Decode() {
	if (ciphertext_len_ == 0) return;  // 初始值
	event_handler_->Decode(ciphertext_, ciphertext_len_);
}

void RsaUi::GenerateKey() {
	QString P_bits = ui_->lineEdit_2->text();
	if (P_bits.isEmpty()) return;
	QString Q_bits = ui_->lineEdit_3->text();
	if (Q_bits.isEmpty()) return;
	QString E = ui_->lineEdit_4->text();
	if (E.isEmpty()) return;

	event_handler_->GenerateKey(P_bits.toInt(), Q_bits.toInt(), E.toInt());
}

void RsaUi::ShowKey(QString& P, QString& Q, QString& N, QString& D) {
	ui_->textBrowser_2->setPlainText(P);
	ui_->textBrowser_3->setPlainText(Q);
	ui_->textBrowser_4->setPlainText(N);
	ui_->textBrowser_5->setPlainText(D);
}

void RsaUi::ShowCiphertext(const unsigned char* ciphertext) {
	delete[] ciphertext_;
	ciphertext_ = new unsigned char[ciphertext_len_];
	char* tmp = new char[ciphertext_len_];
	for (int i = 0; i < ciphertext_len_; ++i) {
		if (ciphertext[i] == '\0') tmp[i] = ' ';
		else tmp[i] = ciphertext[i];
		ciphertext_[i] = ciphertext[i];
	}
	QString text("由于\\0字符会导致显示结束，这里以空格代替\n");
	text.append(QString(QByteArray(tmp, ciphertext_len_)));
	ui_->textBrowser->setText(text);

	delete[] tmp;
	delete[] ciphertext;
}

void RsaUi::ShowCleartext(QString& cleartext) {
	ui_->textBrowser_6->setPlainText(cleartext);
}