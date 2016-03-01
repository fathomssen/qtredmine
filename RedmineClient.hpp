#ifndef __REDMINE_CLIENT_HPP__
#define __REDMINE_CLIENT_HPP__

/* CALLBACK_T */

#define CALLBACK_DISPATCHER(fromClass, toClass, thisptr)     \
	public:                                             \
	typedef void (toClass::*callback_t)(QNetworkReply *reply, QJsonDocument *data, void *);\
	\
	private:                                            \
	fromClass::callback_t operator = (toClass::callback_t & callback) { \
		return reinterpret_cast <fromClass::callback_t> (callback); \
	}                                               \
	\
	Q_SLOT void callback_dispatcher(                \
	        void          *obj_ptr,                 \
	        callback_t     callback,                \
	        QNetworkReply *reply,                   \
	        QJsonDocument *data,                    \
	        void          *argument)                \
	{                                               \
		/*qDebug("callback_dispatcher: %p %p", this, obj_ptr);*/ \
		if (obj_ptr == thisptr)\
			(this->*callback)(reply, data, argument);\
		else                                        \
			callback_call(obj_ptr, callback, reply, data, argument);\
	}                                               \
	\
	Q_SIGNAL void callback_call(                    \
	        void          *obj_ptr,                 \
	        callback_t     callback,                \
	        QNetworkReply *reply,                   \
	        QJsonDocument *data,                    \
	        void          *argument);


/* /CALLBACK_DISPATCHER */

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

typedef void ( *funct_callback_json ) ( void *, QNetworkReply *, QJsonDocument * );
//typedef void (*funct_callback_xml)(void *, QNetworkReply *, QXML *);


/* Base management class.
 *
 * Handles the connexions to a Redmine instance.
 */
class REDMINEQTSHARED_EXPORT RedmineClient : public QObject
{

	Q_OBJECT

public:
	typedef void ( RedmineClient::*callback_t ) ( QNetworkReply *reply, QJsonDocument *data, void * );

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
	RedmineClient ( QString base_url );

	/* Sets base url (to root URI of redmine site)
	 */
	void setBaseUrl ( QString base_url );
	void setBaseUrl ( const char *base_url );

	/* Gets previously set base url (to the root URL of redmine site)
	 */
	QString getBaseUrl();

	/* Set authentification parameters in case of non-configured client
	 */
	void setAuth ( QString apiKey );
	void setAuth ( QString login, QString password );

	/* Creates a client using an API key authenticator.
	 */
	RedmineClient ( QString base_url, QString apiKey, bool checkSsl = true, QObject* parent = NULL );

	/* Creates a client using a login/password authenticator.
	 */
	RedmineClient ( QString base_url, QString login, QString password, bool checkSsl = true, QObject* parent = NULL );
	virtual ~RedmineClient();

	/* Defines the user agent for the client.
	 */
	void setUserAgent ( QByteArray ua );

	void getIssues (
	    QUrl url,
	    void *callback     = NULL,
	    void *callback_arg = NULL,

	    QString filters = "" );

protected:
	/* Sends a request to the Redmine endpoint.
	 */
	QNetworkReply *sendRequest ( QString uri,
	                             EFormat format = JSON,
	                             EMode   mode   = GET,
	                             void *obj_ptr = NULL,
	                             callback_t callback = NULL,
	                             void *callback_arg = NULL,
	                             bool  free_arg     = false,
	                             const QString    &getParams   = "",
	                             const QByteArray &requestData = "" );

private slots:
	void requestFinished_wrapper ( QNetworkReply *reply );

private:
	/* Commont initialization steps.
	 *
	 * Creates the network access manager.
	 */
	void					init();
	bool                    checkUrl ( const QUrl &url );

	struct callback {
		void                   *obj_ptr;
		callback_t              funct;
		void                   *arg;
		bool                    free_arg;
		RedmineClient::EFormat  format;
	};

	QString					_base_url;
	IAuthenticator*			_authenticator = NULL;
	QNetworkAccessManager*	_nma           = NULL;
	QHash<QNetworkReply *, struct callback> _callbacks;
	QByteArray				_ua;

signals:
	void requestFinished ( void *obj_ptr, callback_t callback, QNetworkReply *reply, QJsonDocument *data, void * );

};
#endif // __REDMINE_CLIENT_HPP__
