/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/


#include "WebPage.hpp"
//#include "webview.h"
#include <QAuthenticator>
#include <QMessageBox>
#include <QStyle>
#include <QTimer>
#include <QWebEngineCertificateError>
#include <QWebEngineProfile>

WebPage::WebPage(QObject *parent)
	: QWebEnginePage(QWebEngineProfile::defaultProfile(), parent) {

	connect(this, &QWebEnginePage::featurePermissionRequested, this,
			&WebPage::handleFeaturePermissionRequested);
}

inline QString questionForFeature(QWebEnginePage::Feature feature) {
	switch (feature) {
	case QWebEnginePage::Geolocation:
		return WebPage::tr("Allow %1 to access your location information?");
	case QWebEnginePage::MediaAudioCapture:
		return WebPage::tr("Allow %1 to access your microphone?");
	case QWebEnginePage::MediaVideoCapture:
		return WebPage::tr("Allow %1 to access your webcam?");
	case QWebEnginePage::MediaAudioVideoCapture:
		return WebPage::tr("Allow %1 to access your microphone and webcam?");
	case QWebEnginePage::MouseLock:
		return WebPage::tr("Allow %1 to lock your mouse cursor?");
	case QWebEnginePage::DesktopVideoCapture:
		return WebPage::tr("Allow %1 to capture video of your desktop?");
	case QWebEnginePage::DesktopAudioVideoCapture:
		return WebPage::tr(
			"Allow %1 to capture audio and video of your desktop?");
	case QWebEnginePage::Notifications:
		return WebPage::tr("Allow %1 to show notification on your desktop?");
	}
	return QString();
}

void WebPage::handleFeaturePermissionRequested(const QUrl &securityOrigin,
											   Feature	   feature) {
	QString title = tr("Permission Request");
	QString question = questionForFeature(feature).arg(securityOrigin.host());
	if (!question.isEmpty()
		&& QMessageBox::question(view()->window(), title, question)
			   == QMessageBox::Yes)
		setFeaturePermission(securityOrigin, feature, PermissionGrantedByUser);
	else
		setFeaturePermission(securityOrigin, feature, PermissionDeniedByUser);
}
