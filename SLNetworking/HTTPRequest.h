/*
 * HTTPRequest.h
 *
 *  Created on: 24.03.2013
 *      Author: oliver
 */

#ifndef HTTPREQUEST_H_
#define HTTPREQUEST_H_

#include <QtCore/QObject>
#include <QtNetwork>

namespace SL {

typedef enum {
	HTTPRequestTypeGET = 0,
	HTTPRequestTypePOST,
	HTTPRequestTypePUT,
	HTTPRequestTypeDELETE
} HTTPRequestType;

class HTTPRequest : public QObject {
	Q_OBJECT

public:
	HTTPRequest(const QNetworkRequest &request, QObject *parent = NULL); // get request
	HTTPRequest(const QNetworkRequest &request, const QByteArray &data, QObject *parent = NULL, HTTPRequestType type = HTTPRequestTypeGET);
	HTTPRequest(const HTTPRequest& other);
	virtual ~HTTPRequest();

	HTTPRequestType type(void) const;
	QNetworkRequest request(void) const;
	virtual void start(void);

protected Q_SLOTS:
    void receivedResponse();

Q_SIGNALS:
	void success(const QByteArray &response);
	void failure(const QString &error);

private:
    QNetworkRequest _request;
	HTTPRequestType _type;
	QByteArray _data;
};

} /* namespace SL */
#endif /* HTTPREQUEST_H_ */
