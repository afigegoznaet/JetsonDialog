#include "MainWindow.hpp"
//#include <QWebEngineProfile>
//#include <QWebEngineSettings>
#include <QApplication>
#include <QString>
#include <iostream>
#include <QSettings>

int main(int argc, char *argv[]) {

	QSettings settings("Jetson", "Controller");

	for (int i = 1; i < argc; i++) {
		QString arg{argv[i]};
		std::cout << '\n';
		const auto args = arg.split('=');
		if (0 == args.first().compare("--browser-url")) {
			std::cout << "Browser url:\n" << args.at(1).toStdString() << '\n';
			settings.setValue("WEBRTC", args.at(1));
		}
		if (0 == args.first().compare("--vcamera")) {
			std::cout << "Camera:\n" << args.at(1).toStdString() << '\n';
			settings.setValue("CAM", args.at(1));
		}
		if (0 == args.first().compare("--city")) {
			std::cout << "City:\n" << args.at(1).toStdString() << '\n';
			settings.setValue("City", args.at(1));
		}
	}
	std::cout << std::endl;
	QApplication a(argc, argv);
	/*
		QWebEngineSettings::defaultSettings()->setAttribute(
			QWebEngineSettings::PluginsEnabled, true);
			*/
	MainWindow w;
	w.show();
	return a.exec();
}
