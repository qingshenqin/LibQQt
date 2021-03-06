#include "qqtbluetoothclient.h"
#include <qqtcore.h>

QQtBluetoothClient::QQtBluetoothClient ( QObject* parent ) : QBluetoothSocket ( parent )
{
    initSocket();
}

QQtBluetoothClient::QQtBluetoothClient ( QBluetoothServiceInfo::Protocol socketType, QObject* parent ) :
    QBluetoothSocket ( socketType, parent )
{
    initSocket();
}


void QQtBluetoothClient::initSocket()
{
    connect ( this, SIGNAL ( stateChanged ( QBluetoothSocket::SocketState ) ),
              this, SLOT ( socketStateChanged ( QBluetoothSocket::SocketState ) ) );
    // connected
    connect ( this, SIGNAL ( connected() ),
              this, SLOT ( socketConnected() ) );
    // disconnected
    connect ( this, SIGNAL ( disconnected() ),
              this, SLOT ( socketDisconnect() ) );
    // error
    connect ( this, SIGNAL ( error ( QBluetoothSocket::SocketError ) ),
              this, SLOT ( socketErrorOccured ( QBluetoothSocket::SocketError ) ) );

    connect ( this, SIGNAL ( readyRead() ),
              this, SLOT ( readyReadData() ) );

    connect ( this, SIGNAL ( bytesWritten ( qint64 ) ),
              this, SLOT ( updateProgress ( qint64 ) ) );

    connect ( this, SIGNAL ( bytesWritten ( qint64 ) ),
              this, SIGNAL ( signalUpdateProgress ( qint64 ) ) );

    m_PORT = 0;
    m_protocol = nullptr;
}

void QQtBluetoothClient::installProtocol ( QQtProtocol* stack )
{
    if ( m_protocol )
        return;
    m_protocol = stack;

    connect ( m_protocol, SIGNAL ( write ( const QByteArray& ) ),
              this, SLOT ( slotWriteData ( const QByteArray& ) ) );
    m_protocol->setClientHandler ( this );
    m_protocol->attach();
    m_protocol->initializer();
}

void QQtBluetoothClient::uninstallProtocol ( QQtProtocol* stack )
{
    Q_UNUSED ( stack )

    if ( !m_protocol )
        return;

    disconnect ( m_protocol, SIGNAL ( write ( const QByteArray& ) ),
                 this, SLOT ( slotWriteData ( const QByteArray& ) ) );
    m_protocol->detach();
    m_protocol = NULL;
}

QQtProtocol* QQtBluetoothClient::installedProtocol()
{
    return m_protocol;
}

void QQtBluetoothClient::sendConnectMessage()
{
    pline() << isOpen() << state();

    if ( !isOpen() )
    {
        connectToSingelHost();
        return;
    }

    if ( state() == ServiceLookupState ||
         state() == ConnectingState )
    {
        emit signalConnecting();
        return;
    }

    if ( state() == ConnectedState )
        emit signalConnectSucc();

    return;
}


int QQtBluetoothClient::sendDisConnectFromHost()
{
    pline() << isOpen() << state();

    if ( isOpen() )
    {
#if defined(__WIN__)
        ;
#else
        shutdown ( this->socketDescriptor(), SHUT_RDWR );
#endif
        disconnectFromService();
        close();
        emit signalDisConnectSucc();
    }

    return true;
}

/**
 * @brief QQtBluetoothClient::socketStateChanged
 * @param eSocketState
 * ????????????
 */
void QQtBluetoothClient::socketStateChanged ( QBluetoothSocket::SocketState eSocketState )
{
    pline() << eSocketState;

    switch ( eSocketState )
    {
        case ServiceLookupState:
        case ConnectingState:
            break;

        case ConnectedState:
            break;

        case ClosingState:
            break;

        case UnconnectedState:
            break;

        default:
            break;
    }
}

/**
 * @brief QQtBluetoothClient::socketErrorOccured
 * @param e
 * ????????????
 */
void QQtBluetoothClient::socketErrorOccured ( QBluetoothSocket::SocketError e )
{
    /*
     * ??????????????????????????????????????????????????????????????????????????????????????????
     */
    pline() << e << errorString();

    switch ( e )
    {
        case HostNotFoundError:
        default:
            emit signalConnectFail();
            break;
    }
}

/**
 * @brief QQtBluetoothClient::socketConnected
 * ????????????
 */
void QQtBluetoothClient::socketConnected()
{
    pline() << peerName() << peerAddress().toString() << peerPort();
    /*
     * ???????????????socket???????????????????????????
     */
    emit signalConnectSucc();
}

/**
 * @brief QQtBluetoothClient::socketDisconnect
 * ????????????
 */
void QQtBluetoothClient::socketDisconnect()
{
    pline();
}

void QQtBluetoothClient::updateProgress ( qint64 bytes )
{
    Q_UNUSED ( bytes )
    //pline() << bytes;
}

void QQtBluetoothClient::connectToSingelHost()
{
    if ( !m_uuid.isNull() )
    {
        connectToService ( m_serverIP, m_uuid );
        pline() << peerName() << m_uuid;
    }
    else if ( m_serviceInfo.isValid() )
    {
        connectToService ( m_serviceInfo );
        pline() << m_serviceInfo;
    }
    else if ( m_PORT != 0 )
    {
        connectToService ( m_serverIP, m_PORT );
        pline() << peerName() << m_PORT;
    }
}



void QQtBluetoothClient::readyReadData()
{
    QByteArray bytes;
    bytes = readAll();
    translator ( bytes );
}

void QQtBluetoothClient::slotWriteData ( const QByteArray& bytes )
{
    write ( bytes );
}

void QQtBluetoothClient::translator ( const QByteArray& bytes )
{
    // queued conn and queued package;
    // direct conn and direct package;
    /**
     * ????????????????????????????????????????????????
     * ????????????????????????????????????
     * ????????????????????????????????????????????????????????????
     * ???????????????protocol???????????????????????????????????????????????????
     * ???????????????????????????????????????????????????
     *
     * ???????????????????????????
     * ????????????????????????????????????static????????????????????????????????????buffer???
     * ??????????????????????????????????????????????????????????????????????????????????????????
     */
    static QByteArray sqbaBlockOnNet;
    sqbaBlockOnNet += bytes;
    //qint64 aaa = bytesAvailable();
    //pline() << aaa;

    do
    {
        quint16 nBlockLen = m_protocol->splitter ( sqbaBlockOnNet );

        pmeta ( this ) << sqbaBlockOnNet.size() << "..." << nBlockLen;

        if ( sqbaBlockOnNet.length() < nBlockLen || nBlockLen < m_protocol->minlength() )
        {
            /*
             * ??????????????????????????????????????????????????????
             */
            return;
        }
        else if ( nBlockLen > m_protocol->maxlength() )
        {
            /*
             * ?????????????????????????????????
             */
            sqbaBlockOnNet.clear();
            pmeta ( this ) << "forbidden package" << sqbaBlockOnNet.length() << nBlockLen;
            return;
        }
        else if ( sqbaBlockOnNet.length() > nBlockLen )
        {
            /*
             * ??????
             * ????????????????????????????????????????????????????????????????????????????????????
             * ??????:?????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????
             */
            pmeta ( this ) << "stick package" << sqbaBlockOnNet.length() << nBlockLen;
            QByteArray netData;
            netData.resize ( nBlockLen );
            sqbaBlockOnNet >> netData;
            m_protocol->dispatcher ( netData );
            continue;
        }

        /*
         * ????????????
         */
        m_protocol->dispatcher ( sqbaBlockOnNet );
        break;
    } while ( 1 );

    sqbaBlockOnNet.clear();
}
