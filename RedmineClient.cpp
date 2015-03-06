#include "RedmineClient.hpp"

#include "KeyAuthenticator.hpp"
#include "PasswordAuthenticator.hpp"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>

RedmineClient::RedmineClient() {
    return;
}

RedmineClient::RedmineClient(QUrl url, QString apiKey, bool checkSsl, QObject* parent) : QObject(parent), _url(url) {
    setAuth(apiKey);
}

RedmineClient::RedmineClient(QUrl url, QString login, QString password, bool checkSsl, QObject* parent) : QObject(parent), _url(url) {
    setAuth(login, password);
}

void RedmineClient::setAuth(QString apiKey) {
    _authenticator = new KeyAuthenticator(apiKey.toLatin1());
    init();
}

void RedmineClient::setAuth(QString login, QString password) {
    _authenticator = new PasswordAuthenticator(login, password);
    init();
}

RedmineClient::~RedmineClient() {
    if (_nma != NULL)
        delete _nma;

    if (_authenticator != NULL)
        delete _authenticator;
}

void RedmineClient::init() {
	_nma = new QNetworkAccessManager;
	_ua = "redmine-qt";
}

void RedmineClient::setUserAgent(QByteArray ua) {
	_ua = ua;
}

void RedmineClient::sendRequest(QUrl url, EMode mode, const QByteArray& requestData) {
	QByteArray postDataSize = QByteArray::number(requestData.size());

	QNetworkRequest request(url);
	request.setRawHeader("User-Agent", _ua);
	request.setRawHeader("X-Custom-User-Agent", _ua);
	request.setRawHeader("Content-Type", "text/xml");
	request.setRawHeader("Content-Length", postDataSize);

	_authenticator->addAuthentication(&request);

	QNetworkReply* reply = NULL;
	switch (mode) {
	case GET:
		reply = _nma->get(request);
		break;

	case POST:
		reply = _nma->post(request, requestData);
		break;

	case PUT:
		reply = _nma->put(request, requestData);
		break;

	case DELETE:
		reply = _nma->deleteResource(request);
		break;
	}
}
