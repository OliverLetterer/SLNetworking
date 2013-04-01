/*
 * HTTPClient.cpp
 *
 *  Created on: 24.03.2013
 *      Author: oliver
 */

#include "HTTPClient.h"
#include <QtNetwork>
#include <bb/device/HardwareInfo>
#include <bb/data/JsonDataAccess>

using namespace bb::data;

namespace SL {

HTTPClient::HTTPClient() : QObject(), _accessManager(new QNetworkAccessManager(this)), _defaultHeaders(), _baseURL("http://192.168.186.1:3000/")
{
	setDefaultHeaderValue("Accept", "application/json, application/vnd.ebf.de-v1");
	setDefaultHeaderValue("Content-Type", "application/json");

	QVariantMap deviceInformation;
	bb::device::HardwareInfo currentDevice;
	deviceInformation["name"] = currentDevice.deviceName();
	deviceInformation["type"] = "Blackberry";
	deviceInformation["model"] = currentDevice.modelName();
	deviceInformation["unique_identifier"] = currentDevice.pin();

	// TODO: remove debugging
	deviceInformation["type"] = "IPhoneDevice";
	deviceInformation["model"] = "iPhone5,1";

	JsonDataAccess jsonAccess;
	QString deviceHeader;

	jsonAccess.saveToBuffer(deviceInformation, &deviceHeader);
	setDefaultHeaderValue("DEVICE", deviceHeader.simplified());

	// setDefaultHeaderValue("DEVICE_SIGNATURE", "Unavailable for now");

	// TODO: set previous authorization
	/*
	if (self.authorizationUsername && self.authorizationPassword) {
		[self setAuthorizationHeaderWithUsername:self.authorizationUsername password:self.authorizationPassword];
	}
	 */
}

HTTPClient::~HTTPClient()
{

}

HTTPClient *HTTPClient::SharedInstance()
   {
      static HTTPClient *instance = new HTTPClient();
      return instance;
   }


void HTTPClient::setDefaultHeaderValue(const QString &headerField, const QString &value)
{
	_defaultHeaders[headerField] = value;
}

void HTTPClient::setAuthorizationHeader(const QString &username, const QString &password)
{
	QString headerField = "Authorization";
	QString value = "Basic " + QByteArray(QString("%1:%2").arg(username).arg(password).toAscii()).toBase64();
	setDefaultHeaderValue(headerField, value);
}

QNetworkRequest HTTPClient::requestToPath(const QString &path)
{
	QString URL = _baseURL.append(path);
	QNetworkRequest request(URL);

	// apply default header values
	QMap<QString, QString>::const_iterator i = _defaultHeaders.constBegin();
	while (i != _defaultHeaders.constEnd()) {
		request.setRawHeader(i.key().toUtf8(), i.value().toUtf8());
		++i;
	}

	return request;
}

QNetworkAccessManager *HTTPClient::accessManager(void) const
{
	return _accessManager;
}

void HTTPClient::log(void)
{
	qDebug() << _defaultHeaders;
}

} /* namespace SL */
