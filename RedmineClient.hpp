#ifndef __REDMINE_CLIENT_HPP__
#define __REDMINE_CLIENT_HPP__

class IAuthenticator;
class QNetworkAccessManager;

#include "redmine-qt_global.hpp"

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QHash>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

typedef void (*funct_arg_voidptr_QNetworkReply_ret_void)(void *, QNetworkReply *);

struct callback {
    funct_arg_voidptr_QNetworkReply_ret_void     funct;
    void                                        *arg;
};

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

    /* Creates a non-configured client.
     */
    RedmineClient();

    /* Set authentification parameters in case of non-configured client
     */
    void setAuth(QString apiKey);
    void setAuth(QString login, QString password);

	/* Creates a client using an API key authenticator.
	 */
	RedmineClient(QUrl url, QString apiKey, bool checkSsl = true, QObject* parent = NULL);

	/* Creates a client using a login/password authenticator.
	 */
	RedmineClient(QUrl url, QString login, QString password, bool checkSsl = true, QObject* parent = NULL);
	virtual ~RedmineClient();

	/* Defines the user agent for the client.
	 */
	void setUserAgent(QByteArray ua);

protected:
    /* Sends a request to the Redmine endpoint.
     */
    QNetworkReply *sendRequest(
            QUrl url,
            EMode mode = GET,
            funct_arg_voidptr_QNetworkReply_ret_void callback = NULL,
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

	QUrl					_url;
    IAuthenticator*			_authenticator = NULL;
    QNetworkAccessManager*	_nma           = NULL;
    QHash<QNetworkReply *, struct callback> _callbacks;
	QByteArray				_ua;
};

#endif // __REDMINE_CLIENT_HPP__
