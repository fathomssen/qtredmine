#include "RedmineClient.hpp"

#include "KeyAuthenticator.hpp"
#include "PasswordAuthenticator.hpp"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>

RedmineClient::RedmineClient() {
    return;
}

RedmineClient::RedmineClient(QUrl url, QString apiKey, bool checkSsl, QObject* parent) : QObject(parent), _url(url) {
    (void)checkSsl;
    this->setAuth(apiKey);
}

RedmineClient::RedmineClient(QUrl url, QString login, QString password, bool checkSsl, QObject* parent) : QObject(parent), _url(url) {
    (void)checkSsl;
    this->setAuth(login, password);
}

void RedmineClient::setAuth(QString apiKey) {
    this->_authenticator = new KeyAuthenticator(apiKey.toLatin1());
    init();
}

void RedmineClient::setAuth(QString login, QString password) {
    this->_authenticator = new PasswordAuthenticator(login, password);
    init();
}

RedmineClient::~RedmineClient() {
    if (this->_nma != NULL)
        delete _nma;

    if (this->_authenticator != NULL)
        delete this->_authenticator;
}

void RedmineClient::requestFinished(QNetworkReply *reply) {
    if (this->_callbacks.contains(reply)) {
        struct callback *cb;

        cb = &this->_callbacks[reply];
        cb->funct(cb->arg, reply);

        this->_callbacks.remove(reply);
    }

    return;
}

void RedmineClient::init() {
    this->_nma       = new QNetworkAccessManager;
    this->_ua        = "redmine-qt";

    connect(this->_nma, SIGNAL(finished(QNetworkReply *)), this, SLOT(requestFinished(QNetworkReply *)));

    return;
}

void RedmineClient::setUserAgent(QByteArray ua) {
    this->_ua = ua;

    return;
}

bool RedmineClient::checkUrl(const QUrl &url) {
    if (!url.isValid()) {
        qDebug(QString("Invalid URL: %1").arg(url.toString()).toLatin1());
        return false;
    }

    return true;
}

QNetworkReply *RedmineClient::sendRequest(QUrl url, EMode mode, funct_arg_voidptr_QNetworkReply_ret_void callback, void *callback_arg, const QByteArray& requestData) {
	QByteArray postDataSize = QByteArray::number(requestData.size());

    if (!this->checkUrl(url))
        return NULL;

    QNetworkRequest request(url);
    request.setRawHeader("User-Agent",          _ua);
	request.setRawHeader("X-Custom-User-Agent", _ua);
    request.setRawHeader("Content-Type",        "text/xml");
    request.setRawHeader("Content-Length",      postDataSize);

    this->_authenticator->addAuthentication(&request);

	QNetworkReply* reply = NULL;
	switch (mode) {
	case GET:
        reply = this->_nma->get(request);
		break;

	case POST:
        reply = this->_nma->post(request, requestData);
		break;

	case PUT:
        reply = this->_nma->put(request, requestData);
		break;

	case DELETE:
        reply = this->_nma->deleteResource(request);
        break;
	}

    if ((reply != NULL) && (callback != NULL)) {
        this->_callbacks[reply].arg   = callback_arg;
        this->_callbacks[reply].funct = callback;
    }

    return reply;
}
