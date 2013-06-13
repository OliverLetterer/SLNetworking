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

	template <class T>
	void start(void);

	virtual void willStart(void);

protected Q_SLOTS:
    void receivedResponse();
    void _downloadProgress(qint64 recieved, qint64 total);

Q_SIGNALS:
	void success(const QByteArray &response);
	void failure(QNetworkReply *reply);
	void downloadProgress(qint64 recieved, qint64 total);

private:
    QNetworkRequest _request;
	HTTPRequestType _type;
	QByteArray _data;
};



template <class T>
void HTTPRequest::start()
{
	_request.setRawHeader("DEVICE", _request.rawHeader("DEVICE"));

	this->willStart();

	T *client = T::SharedInstance();

	switch (_type) {
		case HTTPRequestTypeGET: {
			QNetworkReply *reply = client->accessManager()->get(_request);
			connect(reply, SIGNAL(finished()), this, SLOT(receivedResponse()));
			connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(_downloadProgress(qint64, qint64)));
			break;
		} case HTTPRequestTypePOST: {
			QNetworkReply *reply = client->accessManager()->post(_request, _data);
			connect(reply, SIGNAL(finished()), this, SLOT(receivedResponse()));
			connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(_downloadProgress(qint64, qint64)));
			break;
		} case HTTPRequestTypePUT: {
			QNetworkReply *reply = client->accessManager()->put(_request, _data);
			connect(reply, SIGNAL(finished()), this, SLOT(receivedResponse()));
			connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(_downloadProgress(qint64, qint64)));
			break;
		} case HTTPRequestTypeDELETE: {
			QNetworkReply *reply = client->accessManager()->deleteResource(_request);
			connect(reply, SIGNAL(finished()), this, SLOT(receivedResponse()));
			connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(_downloadProgress(qint64, qint64)));
			break;
		}
	}
}

} /* namespace SL */
#endif /* HTTPREQUEST_H_ */
