/*
 * HTTPClient.h
 *
 *  Created on: 24.03.2013
 *      Author: oliver
 */

#ifndef HTTPCLIENT_H_
#define HTTPCLIENT_H_

#include <QtCore/QObject>
#include <QtNetwork>
#include "HTTPRequest.h"



namespace SL {

class HTTPClient : public QObject {
	Q_OBJECT

public:
	static HTTPClient *SharedInstance();

	// managing HTTP headers
	void setDefaultHeaderValue(const QString &headerField, const QString &value);
	void setAuthorizationHeader(const QString &username, const QString &password);

	// managing requests
	QNetworkRequest requestToPath(const QString &path);

	QNetworkAccessManager *accessManager(void) const;

	// debugging
	void log(void);

private:
	HTTPClient();
	virtual ~HTTPClient();
	HTTPClient(const HTTPClient&) : QObject(NULL) {}
	HTTPClient& operator=(const HTTPClient&) { return *this; }

	QNetworkAccessManager *_accessManager;
	QMap<QString, QString> _defaultHeaders;
	QString _baseURL;
};

} /* namespace SL */
#endif /* HTTPCLIENT_H_ */
