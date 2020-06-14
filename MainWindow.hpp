#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QtNetwork/QNetworkAccessManager>
#include <QHash>
#include <QMenu>

constexpr auto ZERO_KELVIN = 273.15;

QT_BEGIN_NAMESPACE
namespace Ui {
	class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
	Q_OBJECT

signals:
	void updateTime();
	void updateWeatherIcon(QString);
	void updateTemperature(int);


private:
	void initMenu();
	void initIcons();
	void initMainWindow();
	void initButtons();
	void initGearButton();
	void initTime();
	void initWeather();
	void initTemp();
	void initBrowser();
	void initVideo();

	void getWeather();
	void saveSettings();
	void loadSettings();

public slots:
	void updateWeatherIconSlot(QString icnName);
	void updateTempSlot(int temp);
	void handleWeatherNetworkData(QNetworkReply *networkReply);
	void showMenu();

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private slots:
	void on_browserButton_clicked();

	void on_videoButton_clicked();

private:
	Ui::MainWindow *		ui;
	QString					formats[4];
	QHash<QString, QString> icons;
	QString					city{"Brasov"};
	QString					rtcUrl;
	QString					selCamera;
	QNetworkAccessManager	netMan;
	QMenu					sysMenu;
	int						screenHeight;
	int						screenWidth;
	int						timeFormatIdx{0};
};
#endif // MAINWINDOW_HPP
