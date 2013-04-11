/*
 * HTTPJSONRequest.cpp
 *
 *  Created on: 24.03.2013
 *      Author: oliver
 */

#include "HTTPJSONRequest.h"
#include <bb/data/JsonDataAccess>

using namespace bb::data;

namespace SL {

HTTPJSONRequest::HTTPJSONRequest(const QNetworkRequest &request, QObject *parent) : HTTPRequest(request, parent)
{

}

HTTPJSONRequest::HTTPJSONRequest(const QNetworkRequest &request, const QByteArray &data, QObject *parent, HTTPRequestType type) : HTTPRequest(request, data, parent, type)
{

}

HTTPJSONRequest::HTTPJSONRequest(const HTTPJSONRequest& other) : HTTPRequest(HTTPRequest::request(), QObject::parent())
{

}

void HTTPJSONRequest::willStart()
{
	HTTPRequest::willStart();

	connect(this, SIGNAL(success(const QByteArray&)), this, SLOT(successCallback(const QByteArray&)));
}

QVariant HTTPJSONRequest::responseObject()
{
	return _reponseObject;
}

void HTTPJSONRequest::successCallback(const QByteArray &response)
{
	JsonDataAccess jsonAccess;
	_reponseObject = jsonAccess.loadFromBuffer(response);

	qDebug() << _reponseObject;

	emit receivedJSONObject(_reponseObject);
}

} /* namespace SL */
