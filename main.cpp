#include "MainWindow.hpp"
#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <QApplication>
#include <string>
#include <iostream>

int main(int argc, char *argv[]) {

	for (int i = 1; i < argc; i++) {
		std::string arg{argv[i]};
		std::cout << arg << '\n';
	}
	QApplication a(argc, argv);

	QWebEngineSettings::defaultSettings()->setAttribute(
		QWebEngineSettings::PluginsEnabled, true);
	MainWindow w;
	w.show();
	return a.exec();
}
