/*
 * HTTPJSONRequest.h
 *
 *  Created on: 24.03.2013
 *      Author: oliver
 */

#ifndef HTTPJSONREQUEST_H_
#define HTTPJSONREQUEST_H_

#include "HTTPRequest.h"



namespace SL {

class HTTPJSONRequest : public HTTPRequest {
	Q_OBJECT

public:
	HTTPJSONRequest(const QNetworkRequest &request, QObject *parent = NULL);
	HTTPJSONRequest(const QNetworkRequest &request, const QByteArray &data, QObject *parent = NULL, HTTPRequestType type = HTTPRequestTypeGET);

	HTTPJSONRequest(const HTTPJSONRequest &other);

	void start(void);
	QVariant responseObject();

private:
	QVariant _reponseObject;

Q_SIGNALS:
	void receivedJSONObject(const QVariant &responseObject);

private Q_SLOTS:
    void successCallback(const QByteArray &response);
};

} /* namespace SL */
#endif /* HTTPJSONREQUEST_H_ */
