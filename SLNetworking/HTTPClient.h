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
#include <bb/device/HardwareInfo>
#include <bb/data/JsonDataAccess>
#include <QUrl>

using namespace bb::data;


namespace SL {

template <class T>
class HTTPClient : public QObject {
public:
	static T *SharedInstance();

	// managing HTTP headers
	void setDefaultHeaderValue(const QString &headerField, const QString &value);
	void setAuthorizationHeader(const QString &username, const QString &password);

	// managing requests
	QNetworkRequest requestToPath(const QString &path);

	QNetworkAccessManager *accessManager(void) const;

	// debugging
	void log(void);

	QString baseURL(void) const {
		return _baseURL;
	}

	void setBaseURL(QString baseURL) {
		_baseURL = baseURL;
	}
protected:
	HTTPClient(const QString &baseURL);
	virtual ~HTTPClient();

private:
	HTTPClient(const HTTPClient&) : QObject(NULL) {}
	HTTPClient& operator=(const HTTPClient&) { return *this; }

	QNetworkAccessManager *_accessManager;
	QMap<QString, QString> _defaultHeaders;
	QString _baseURL;
};



template <class T>
HTTPClient<T>::HTTPClient(const QString &baseURL) : QObject(), _accessManager(new QNetworkAccessManager(this)), _defaultHeaders(), _baseURL(baseURL)
{
	// TODO: set previous authorization
	/*
	if (self.authorizationUsername && self.authorizationPassword) {
		[self setAuthorizationHeaderWithUsername:self.authorizationUsername password:self.authorizationPassword];
	}
	 */
}

template <class T>
HTTPClient<T>::~HTTPClient()
{

}

template <class T>
T *HTTPClient<T>::SharedInstance()
{
	static T *instance = new T();
	return instance;
}

template <class T>
void HTTPClient<T>::setDefaultHeaderValue(const QString &headerField, const QString &value)
{
	_defaultHeaders[headerField] = value;
}

template <class T>
void HTTPClient<T>::setAuthorizationHeader(const QString &username, const QString &password)
{
	QString headerField = "Authorization";
	QString value = "Basic " + QByteArray(QString("%1:%2").arg(username).arg(password).toAscii()).toBase64();
	setDefaultHeaderValue(headerField, value);
}

template <class T>
QNetworkRequest HTTPClient<T>::requestToPath(const QString &path)
{
	QString URL = QString(_baseURL);
	if (QUrl(path).host().length() == 0) {
		URL = URL.append(path);
	} else {
		URL = path;
	}
	QNetworkRequest request(URL);

	// apply default header values
	QMap<QString, QString>::const_iterator i = _defaultHeaders.constBegin();
	while (i != _defaultHeaders.constEnd()) {
		request.setRawHeader(i.key().toUtf8(), i.value().toUtf8());
		++i;
	}

	return request;
}

template <class T>
QNetworkAccessManager *HTTPClient<T>::accessManager(void) const
{
	return _accessManager;
}

template <class T>
void HTTPClient<T>::log(void)
{
	qDebug() << "HTTPClient:";
	qDebug() << _defaultHeaders;
	qDebug() << _baseURL;
}

} /* namespace SL */
#endif /* HTTPCLIENT_H_ */
