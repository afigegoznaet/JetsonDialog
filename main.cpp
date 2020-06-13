#include "MainWindow.hpp"
#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <QApplication>

int main(int argc, char *argv[]) {

	QApplication a(argc, argv);

	QWebEngineSettings::defaultSettings()->setAttribute(
		QWebEngineSettings::PluginsEnabled, true);
	MainWindow w;
	w.show();
	return a.exec();
}
