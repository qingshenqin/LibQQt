#include <qqtprotocolmanager.h>


QQtProtocolManager::QQtProtocolManager ( QObject* parent ) : QObject ( parent )
{
}

QQtProtocolManager::~QQtProtocolManager()
{

}

QQtProtocol* QQtProtocolManager::createProtocol()
{
    if ( m_protocol_list.isEmpty() )
        return NULL;

    //无论如何，使用对象工厂也一样，都不能正确生成。对象工厂崩溃退出。
    //QQtProtocol* p0 = ( QQtProtocol* ) mProtocol->metaObject()->newInstance ( Q_ARG(QQtProtocolManager*, this) );
    QQtProtocol* p0 = findDetachedInstance();
    if ( p0 == 0 )
        return NULL;
    pmeta ( p0 ) << p0;

    //帮助Protocol给用户发数据。
    connect ( p0, SIGNAL ( notifyToProtocolManager ( const QQtProtocol*, const QQtMessage* ) ),
              this, SIGNAL ( notifyToBusinessLevel ( const QQtProtocol*, const QQtMessage* ) ) );
    return p0;
}

void QQtProtocolManager::deleteProtocol ( QQtProtocol* stack )
{
    //决定不删除句柄
    return;

    if ( m_protocol_list.isEmpty() )
        return;

    QQtProtocol* p0 = stack;

    if ( !p0 )
        return;

    disconnect ( p0, SIGNAL ( notifyToProtocolManager ( const QQtProtocol*, const QQtMessage* ) ),
                 this, SIGNAL ( notifyToBusinessLevel ( const QQtProtocol*, const QQtMessage* ) ) );

    m_protocol_list.removeOne ( p0 );
    p0->deleteLater();

    return;
}

QQtProtocol* QQtProtocolManager::findDetachedInstance()
{
    //但凡attached就是在使用的。createProtocol,attach.
    //但凡deattached的都是准备删除的。deattach,deleteProtocol.
    //初始deattached的呢？不会删除。删除操作发生在deattached阶段。
    QListIterator<QQtProtocol*> itor0 ( m_protocol_list );
    int index = 0;
    while ( itor0.hasNext() )
    {
        QQtProtocol* p = itor0.next();
        if ( p->detached() )
            index++;
    }
    pline() << "total protocol handler:" << m_protocol_list.size();
    pline() << "remaining protocol handler:" << index;
    pline() << "using protocol handler:" << m_protocol_list.size() - index;
    emit remanentProtocolChanged ( index );
    emit remainingProtocolChanged ( index );

    QListIterator<QQtProtocol*> itor ( m_protocol_list );
    while ( itor.hasNext() )
    {
        QQtProtocol* p = itor.next();
        if ( p->detached() )
            return p;
    }
    return NULL;
}
