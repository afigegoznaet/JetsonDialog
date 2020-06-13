#include "MainWindow.hpp"
#include "./ui_MainWindow.h"
#include <QScreen>
#include <QTime>
#include <QTimer>
#include <QSettings>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QHostInfo>
#include <QJsonArray>
#include <QLineEdit>
#include <QWidgetAction>
#include <QWebEngineView>
#include <QtNetwork/QNetworkInterface>
#include <QtNetwork/QNetworkReply>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), ui(new Ui::MainWindow), sysMenu(this) {
	ui->setupUi(this);
	QScreen *screen = QGuiApplication::primaryScreen();
	QRect	 screenGeometry = screen->geometry();
	screenHeight = screenGeometry.height();
	loadSettings();
	initMenu();
	initMainWindow();
	initButtons();
	getWeather();

	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SIGNAL(updateTime()));
	timer->start(1800000); // half an hour
}

void MainWindow::initMenu() {
	QFont font("Segoe UI");
	font.setBold(true);
	font.setPixelSize(int(screenHeight / 35));
	sysMenu.setFont(font);
	sysMenu.setStyleSheet(
		"QMenu{\
			color: #fffff1;\
			background-color: rgba(0, 0, 0, 15%);\
		}\
		QMenu::item:selected {\
			color: #ffffff;\
			background: rgba(255, 255, 255, 25%);\
		}");
	sysMenu.setMinimumWidth(int(screenHeight / 20));
	sysMenu.setAttribute(Qt::WA_TranslucentBackground, true);
	sysMenu.setWindowFlags(sysMenu.windowFlags() | Qt::FramelessWindowHint);

	sysMenu.addAction(QHostInfo::localHostName());
	const auto localAddress = [] {
		const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
		for (const QHostAddress &address : QNetworkInterface::allAddresses()) {
			if (address.protocol() == QAbstractSocket::IPv4Protocol
				&& address != localhost)
				return address.toString();
		}
		return QString{"127.0.0.1"};
	};
	sysMenu.addAction(localAddress());
	sysMenu.addSeparator();

	QLineEdit *townEdit = new QLineEdit(city, &sysMenu);
	connect(townEdit, &QLineEdit::editingFinished, this, [this, townEdit] {
		city = townEdit->text();
		sysMenu.hide();
		getWeather();
	});

	townEdit->setStyleSheet(
		"color: #fffff1; background-color: rgba(0, 0, 0, 15%);");
	townEdit->setFont(font);
	const auto townAction = new QWidgetAction(this);
	townAction->setDefaultWidget(townEdit);
	sysMenu.addAction(townAction);
	sysMenu.addSeparator();

	const auto quitAction = new QAction(tr("&Quit"), this);
	connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

	sysMenu.addAction(quitAction);
}

void MainWindow::initIcons() {
	icons.insert("01d", ":/icons/weather-sunny.png");
	icons.insert("01n", ":/icons/weather-sunny.png");
	icons.insert("02d", ":/icons/weather-sunny-very-few-clouds.png");
	icons.insert("02n", ":/icons/weather-sunny-very-few-clouds.png");
	icons.insert("03d", ":/icons/weather-few-clouds.png");
	icons.insert("03n", ":/icons/weather-few-clouds.png");
	icons.insert("04d", ":/icons/weather-overcast.png");
	icons.insert("04n", ":/icons/weather-overcast.png");
	icons.insert("09d", ":/icons/weather-showers.png");
	icons.insert("09n", ":/icons/weather-showers.png");
	icons.insert("10d", ":/icons/weather-showers.png");
	icons.insert("10n", ":/icons/weather-showers.png");
	icons.insert("11d", ":/icons/weather-thundershower.png");
	icons.insert("11n", ":/icons/weather-thundershower.png");
	icons.insert("13d", ":/icons/weather-snow.png");
	icons.insert("13n", ":/icons/weather-snow.png");
	icons.insert("50d", ":/icons/weather-fog.png");
	icons.insert("50n", ":/icons/weather-fog.png");
}

void MainWindow::initMainWindow() {
	setWindowTitle("Main Window");
	setContextMenuPolicy(Qt::NoContextMenu);
	setWindowFlags(Qt::FramelessWindowHint | Qt::MSWindowsFixedSizeDialogHint);
	setWindowState(Qt::WindowFullScreen);
	centralWidget()->setStyleSheet(
		"border-image: url(:/res/galaxy.jpg) 0 0 0 0 stretch stretch;");
}
void MainWindow::initButtons() {
	initGearButton();
	initTime();
	initWeather();
	initTemp();
	initBrowser();
	initVideo();
	initIcons();
}

void MainWindow::initGearButton() {
	QImage gearIcon(":/res/gear.png");
	gearIcon.invertPixels();
	ui->sysButton->setStyleSheet(
		"color: #fffff1; border-image: url(:/res/0.png) ");
	ui->sysButton->setIcon(QPixmap::fromImage(gearIcon));
	ui->sysButton->setIconSize({screenHeight / 10, screenHeight / 10});
	connect(ui->sysButton, SIGNAL(clicked()), this, SLOT(showMenu()));
}

void MainWindow::showMenu() { sysMenu.exec(QCursor::pos()); }

void MainWindow::initTime() {
	formats[0] = "H:mm";
	formats[1] = "H:mm:ss";
	formats[2] = "h:mm ap";
	formats[3] = "h:mm:ss a";
	QFont font("Segoe UI");
	font.setBold(true);
	font.setPixelSize(int(screenHeight / 15));
	ui->timeWidget->setFont(font);
	ui->timeWidget->setStyleSheet(
		"color: #fffff1; border-image: url(:/res/0.png) ");
	ui->timeWidget->setAttribute(Qt::WA_TranslucentBackground, true);
	ui->timeWidget->setWindowOpacity(1);
	ui->timeWidget->setText(
		QTime::currentTime().toString(formats[timeFormatIdx]));
	QTimer *timer = new QTimer(this);
	ui->timeWidget->setFocusPolicy(Qt::NoFocus);
	ui->timeWidget->setFlat(true);
	connect(this, &MainWindow::updateTime, this, [this] {
		ui->timeWidget->setText(
			QTime::currentTime().toString(formats[timeFormatIdx]));
	});
	connect(timer, SIGNAL(timeout()), this, SIGNAL(updateTime()));

	connect(ui->timeWidget, &QPushButton::clicked, this, [this] {
		timeFormatIdx++;
		timeFormatIdx &= 3;
		emit updateTime();
	});

	timer->start(1000);
}

void MainWindow::initWeather() {
	QFont font("Segoe UI");
	font.setBold(true);
	font.setPixelSize(int(screenHeight / 30));
	ui->weatherIcon->setFont(font);
	ui->weatherIcon->setStyleSheet(
		"color: #fffff1; border-image: url(:/res/weather-unknown.png) ");
	ui->weatherIcon->setFixedWidth(screenHeight / 10);
	ui->weatherIcon->setFixedHeight(screenHeight / 10);
}
void MainWindow::initTemp() {
	QFont font("Segoe UI");
	font.setBold(true);
	font.setPixelSize(int(screenHeight / 30));
	ui->tempLabel->setFont(font);
	ui->tempLabel->setStyleSheet(
		"color: #fffff1; border-image: url(:/res/0.png) ");
	// ui->tempLabel->setFixedWidth(screenHeight / 10);
	// ui->tempLabel->setFixedHeight(screenHeight / 10);
}

void MainWindow::initBrowser() {
	QFont font("Segoe UI");
	font.setBold(true);
	font.setPixelSize(int(screenHeight / 30));
	ui->browserButton->setFont(font);
	ui->browserButton->setStyleSheet(
		"color: #fffff1; border-image: url(:/res/0.png) ");
	ui->browserButton->setText("Launch \n Browser");
}
void MainWindow::initVideo() {
	QFont font("Segoe UI");
	font.setBold(true);
	font.setPixelSize(int(screenHeight / 30));
	ui->videoButton->setFont(font);
	ui->videoButton->setStyleSheet(
		"color: #fffff1; border-image: url(:/res/0.png) ");
	ui->videoButton->setText("Launch \n Self \n Video");
}

void MainWindow::updateWeatherIconSlot(QString icnName) {
	QPixmap icn{":/res/weather-unknown.png"};
	if (!icons.count(icnName)) {
		ui->weatherIcon->setStyleSheet(
			"color: #fffff1; border-image: url(:/res/weather-showers.png) ");
	} else {
		QString icon =
			QString::asprintf("color: #fffff1; border-image: url(%s) ",
							  icons[icnName].toStdString().c_str());
		ui->weatherIcon->setStyleSheet(icon);
		ui->weatherIcon->setPixmap(QIcon(icon).pixmap(ui->weatherIcon->size()));
	}
}

void MainWindow::updateTempSlot(int temp) {
	ui->tempLabel->setText(QString::asprintf("%d°C", temp));
}


void MainWindow::getWeather() {
	QUrl	  url("http://api.openweathermap.org/data/2.5/weather");
	QUrlQuery query;

	query.addQueryItem("q", city);
	query.addQueryItem("mode", "json");
	query.addQueryItem("APPID", "36496bad1955bf3365448965a42b9eac");
	url.setQuery(query);

	QNetworkReply *rep = netMan.get(QNetworkRequest(url));
	// connect up the signal right away
	connect(rep, &QNetworkReply::finished, this,
			[this, rep]() { handleWeatherNetworkData(rep); });
}

void MainWindow::handleWeatherNetworkData(QNetworkReply *networkReply) {

	if (!networkReply)
		return;

	if (networkReply->error()) {
		updateWeatherIconSlot("");
		ui->tempLabel->setText("Unknown location");
	} else {

		QJsonDocument document =
			QJsonDocument::fromJson(networkReply->readAll());

		if (document.isObject()) {
			QJsonObject obj = document.object();
			QJsonObject tempObject;
			QJsonValue	val;

			if (obj.contains(QStringLiteral("weather"))) {
				val = obj.value(QStringLiteral("weather"));
				QJsonArray weatherArray = val.toArray();
				val = weatherArray.at(0);
				tempObject = val.toObject();
				updateWeatherIconSlot(tempObject.value("icon").toString());
			}
			if (obj.contains(QStringLiteral("main"))) {
				val = obj.value(QStringLiteral("main"));
				tempObject = val.toObject();
				val = tempObject.value(QStringLiteral("temp"));
				updateTempSlot(val.toDouble() - ZERO_KELVIN);
			}
		}
	}
}

void MainWindow::saveSettings() {
	QSettings settings("Jetson", "Controller");
	settings.setValue("ClockStyle", timeFormatIdx);
	settings.setValue("City", city);
}

void MainWindow::loadSettings() {
	QSettings settings("Jetson", "Controller");
	timeFormatIdx = settings.value("ClockStyle", 0).toInt();
	city = settings.value("City", "Brasov").toString();
}

MainWindow::~MainWindow() {
	saveSettings();
	delete ui;
}

void MainWindow::on_browserButton_clicked() {
	QWebEngineView view;
	view.setUrl(QUrl("http://test.webrtc.org"));
	view.resize(this->size());
	view.show();
}
