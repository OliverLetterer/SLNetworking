/*
 * HTTPRequest.cpp
 *
 *  Created on: 24.03.2013
 *      Author: oliver
 */

#include "HTTPRequest.h"
#include "HTTPClient.h"

namespace SL {

HTTPRequest::HTTPRequest(const QNetworkRequest &request, QObject* parent) : QObject(parent), _request(request), _type(HTTPRequestTypeGET) {

}

HTTPRequest::HTTPRequest(const QNetworkRequest &request, const QByteArray &data, QObject *parent, HTTPRequestType type) : QObject(parent), _request(request), _type(type), _data(data)
{

}

HTTPRequest::HTTPRequest(const HTTPRequest &other) : QObject(this), _request(other._request), _type(other._type) {

}

HTTPRequestType HTTPRequest::type() const
{
	return _type;
}

QNetworkRequest HTTPRequest::request(void) const
{
	return _request;
}

void HTTPRequest::receivedResponse()
{
	QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
	QByteArray response;
	bool error = false;

	if (reply) {
		if (reply->error() == QNetworkReply::NoError) {
			const int available = reply->bytesAvailable();
			if (available > 0) {
				response =  QByteArray(reply->readAll());
			}
		} else {
			response = tr("Error: %1 status: %2").arg(reply->errorString(), reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString()).toUtf8();
			error = true;
		}

		reply->deleteLater();
	}

	if (error) {
		qDebug() << response;
		emit failure(QString::fromUtf8(response));
	} else {
		emit success(response);
	}
}

void HTTPRequest::willStart() {

}

HTTPRequest::~HTTPRequest() {

}

} /* namespace SL */
