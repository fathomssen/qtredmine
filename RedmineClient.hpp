#ifndef __REDMINE_CLIENT_HPP__
#define __REDMINE_CLIENT_HPP__

class IAuthenticator;
class QNetworkAccessManager;

#include "redmine-qt_global.hpp"

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QHash>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariant>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

typedef void (*funct_callback_json)(void *, QNetworkReply *, QJsonDocument *);
//typedef void (*funct_callback_xml)(void *, QNetworkReply *, QXML *);


/* Base management class.
 *
 * Handles the connexions to a Redmine instance.
 */
class REDMINEQTSHARED_EXPORT RedmineClient : public QObject {

	Q_OBJECT

public:
	enum EMode {
		GET,
		POST,
		PUT,
		DELETE
	};
    enum EFormat {
        JSON,
        XML
    };

    /* Creates a non-configured client.
     */
    RedmineClient();
    RedmineClient(QString base_url);

    /* Sets base url (to root URI of redmine site)
     */
    void setBaseUrl(QString base_url);
    void setBaseUrl(const char *base_url);

    /* Set authentification parameters in case of non-configured client
     */
    void setAuth(QString apiKey);
    void setAuth(QString login, QString password);

	/* Creates a client using an API key authenticator.
	 */
    RedmineClient(QString base_url, QString apiKey, bool checkSsl = true, QObject* parent = NULL);

	/* Creates a client using a login/password authenticator.
	 */
    RedmineClient(QString base_url, QString login, QString password, bool checkSsl = true, QObject* parent = NULL);
	virtual ~RedmineClient();

	/* Defines the user agent for the client.
	 */
	void setUserAgent(QByteArray ua);

    void getIssues(
            QUrl url,
            void *callback     = NULL,
            void *callback_arg = NULL,
            QString filters = "");

protected:
    /* Sends a request to the Redmine endpoint.
     */
    QNetworkReply *sendRequest(
            QString uri,
            EFormat format = JSON,
            EMode   mode   = GET,
            void *callback     = NULL,
            void *callback_arg = NULL,
            const QByteArray& requestData = "");

private slots:
    void requestFinished(QNetworkReply *reply);

private:
	/* Commont initialization steps.
	 *
	 * Creates the network access manager.
	 */
	void					init();
    bool                    checkUrl(const QUrl &url);

    QString					_base_url;
    IAuthenticator*			_authenticator = NULL;
    QNetworkAccessManager*	_nma           = NULL;
    QHash<QNetworkReply *, struct callback> _callbacks;
	QByteArray				_ua;
};

struct callback {
    void                   *funct;
    void                   *arg;
    RedmineClient::EFormat  format;
};

#endif // __REDMINE_CLIENT_HPP__
