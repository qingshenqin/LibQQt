#include <qqtwebloader.h>

QQtNetworkReply::QQtNetworkReply ( QObject* parent ) : QNetworkReply ( parent )
{
    open ( QIODevice::ReadWrite );
}

QQtNetworkReply::~QQtNetworkReply()
{
    close();
}

QQtNetworkReply::QQtNetworkReply ( const QQtNetworkReply& other )
{
    *this = ( const QNetworkReply& ) other;
}

QQtNetworkReply& QQtNetworkReply::operator = ( const QNetworkReply& other )
{
    setOperation ( other.operation() );
    setRequest ( other.request() );
    setError ( other.error(), other.errorString() );
    setFinished ( other.isFinished() );
    setUrl ( other.url() );

    for ( int i = ( int ) QNetworkRequest::ContentTypeHeader;
          i <= ( int ) QNetworkRequest::ServerHeader; i++ )
    {
        QNetworkRequest::KnownHeaders ii = ( QNetworkRequest::KnownHeaders ) i;
        setHeader ( ii, other.header ( ii ) );
    }

    QListIterator<QByteArray> itor ( other.rawHeaderList() );
    while ( itor.hasNext() )
    {
        const QByteArray& key = itor.next();
        setRawHeader ( key, other.rawHeader ( key ) );
    }

#if QT_VERSION > QT_VERSION_CHECK(5, 9, 0)
    for ( int i = ( int ) QNetworkRequest::HttpStatusCodeAttribute;
          i <= ( int ) QNetworkRequest::RedirectPolicyAttribute; i++ )
#elif QT_VERSION > QT_VERSION_CHECK(5, 6, 3)
    for ( int i = ( int ) QNetworkRequest::HttpStatusCodeAttribute;
          i <= ( int ) QNetworkRequest::HTTP2WasUsedAttribute; i++ )
#elif QT_VERSION > QT_VERSION_CHECK(5, 5, 1)
    for ( int i = ( int ) QNetworkRequest::HttpStatusCodeAttribute;
          i <= ( int ) QNetworkRequest::FollowRedirectsAttribute; i++ )
#else
    for ( int i = ( int ) QNetworkRequest::HttpStatusCodeAttribute;
          i <= ( int ) QNetworkRequest::EmitAllUploadProgressSignalsAttribute; i++ )
#endif
    {
        QNetworkRequest::Attribute ii = ( QNetworkRequest::Attribute ) i;
        setAttribute ( ii, other.attribute ( ii ) );
    }
    setAttribute ( QNetworkRequest::User, other.attribute ( QNetworkRequest::User ) );

#ifndef QT_NO_SSL
    setSslConfiguration ( other.sslConfiguration() );
#endif
    return *this;
}

qint64 QQtNetworkReply::readData ( char* data, qint64 maxlen )
{
    int size1  = mBytes.size();
    if ( maxlen < size1 )
        size1 = maxlen;
    memcpy ( data, mBytes.constData(), size1 );
    mBytes.remove ( 0, size1 );
    return size1;
}

qint64 QQtNetworkReply::writeData ( const char* data, qint64 len )
{
    mBytes.push_back ( QByteArray ( data, len ) );
    return len;
}

void QQtNetworkReply::abort()
{
    //
}


QQtWebLoader::QQtWebLoader ( QObject* parent )
    : QObject ( parent )
{
    mWebAccessManager = new QQtWebAccessManager ( this );

    connect ( mWebAccessManager, SIGNAL ( replyFinished ( QQtWebAccessSession* ) ),
              this, SLOT ( localReplyFinished ( QQtWebAccessSession* ) ) );
    connect ( mWebAccessManager, SIGNAL ( replyTimeOut ( QQtWebAccessSession* ) ),
              this, SLOT ( localReplyTimeout ( QQtWebAccessSession* ) ) );

    setTimerInterval();
}

QQtWebLoader::~QQtWebLoader() {}

void QQtWebLoader::setTimerInterval ( int millsecond )
{
    mWebAccessManager->setTimerInterval ( millsecond );
}

int QQtWebLoader::timerInterval()
{
    return mWebAccessManager->timerInterval();
}

QQtWebAccessSession* QQtWebLoader::get ( const QString& url )
{
    QNetworkRequest netRequest;
    netRequest.setUrl ( QUrl ( url ) ); //????????????
    return get ( netRequest );
}

QQtWebAccessSession* QQtWebLoader::get ( const QNetworkRequest& req )
{
    QQtWebAccessSession* session = mWebAccessManager->sendGetRequest ( req );

    int millsecond = timerInterval();
    //manager?????????????????????????????????session???
    //???????????????????????????????????????session?????????????????????????????????????????????
    //session->getTimer()->setInterval ( millsecond );
    //session->getTimer()->start();
    millsecond += 1000;
    //+1s ????????????????????????????????????????????????????????????????????????????????????timeout???????????????

    block1.lock ( millsecond );

    return &session1;
}

QQtWebAccessSession* QQtWebLoader::post ( const QString& url, const QByteArray& data )
{
    QNetworkRequest netRequest;
    netRequest.setUrl ( QUrl ( url ) ); //????????????
    return post ( netRequest, data );
}

QQtWebAccessSession* QQtWebLoader::post ( const QNetworkRequest& req, const QByteArray& data )
{
    QQtWebAccessSession* session = mWebAccessManager->sendPostRequest ( req, data );

    int millsecond = timerInterval();
    //manager?????????????????????????????????session???
    //???????????????????????????????????????session?????????????????????????????????????????????
    //session->getTimer()->setInterval ( millsecond );
    //session->getTimer()->start();
    millsecond += 1000;
    //+1s ????????????????????????????????????????????????????????????????????????????????????timeout???????????????

    block1.lock ( millsecond );
    return &session1;
}

void QQtWebLoader::localReplyFinished ( QQtWebAccessSession* session )
{
    //???session?????????????????????????????????????????????
    reply1 = * ( session->getWebAccessReply() );
    reply1.write ( session->getWebAccessReply()->readAll() );

    session1.setWebAccessSessionName ( session->getWebAccessSessionName() );
    session1.setWebAccessUrl ( session->getWebAccessUrl() );
    session1.getTimer()->setInterval ( session->getTimer()->interval() );
    session1.setWebAccessReply ( &reply1 );
    session1.webAccessRequest() = session->webAccessRequest();

    block1.unlock();
}

void QQtWebLoader::localReplyTimeout ( QQtWebAccessSession* session )
{
    reply1 = * ( session->getWebAccessReply() );
    //reply1.write ( session->getWebAccessReply()->readAll() );//???

    session1.setWebAccessSessionName ( session->getWebAccessSessionName() );
    session1.setWebAccessUrl ( session->getWebAccessUrl() );
    session1.getTimer()->setInterval ( session->getTimer()->interval() );
    session1.setWebAccessReply ( &reply1 );
    session1.webAccessRequest() = session->webAccessRequest();
    block1.unlock();
}
