#include <iostream>
#include "rsa_ui.h"
#include <QApplication>
using namespace std;

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	RsaUi rsa_app;
	rsa_app.Init();
	rsa_app.show();
	return app.exec();
}

