#include "qqtosdform.h"
#include "ui_qqtosdform.h"
#include <QPainter>
#include <QBitmap>
#include <qqtbodymover.h>
#include <QMouseEvent>
#include <qqtcore.h>
#include <qqtframe.h>

QQtOsdForm::QQtOsdForm ( QWidget* parent ) :
    QQtWidget ( parent ),
    ui ( new Ui::QQtOsdForm )
{
    ui->setupUi ( this );
    //setStyleSheet ( "QQtOsdForm{ background-color: rgb(222, 222, 222, 0);}" );
    setWindowFlag ( Qt::FramelessWindowHint, true );

    setWindowFlag ( Qt::WindowStaysOnTopHint, true );
    //setWindowFlag ( Qt::Tool, true );

    setAttribute ( Qt::WA_TranslucentBackground, true );
    //setAttribute ( Qt::WA_TransparentForMouseEvents, true );
    setAttribute ( Qt::WA_OpaquePaintEvent, true );

    QPalette pal = palette();
    pal.setColor ( QPalette::Background, QColor ( 255, 255, 255, 255 ) );
    //ignore
    //setPalette ( pal );

//    QQtFramelessHelper* frameless = new QQtFramelessHelper ( this );
//    frameless->addDragWidget ( this );
//    frameless->setCloseButton ( ui->pushButtonClose );
//    frameless->setMaximizedButton ( ui->pushButtonMax );
//    frameless->setRestoreButton ( ui->pushButtonRestore );
//    frameless->setMinimizedButton ( ui->pushButtonMin, false );

    QQtBodyMover* frameless = new QQtBodyMover ( this );
    this->installEventFilter ( frameless );

    //setWindowFlag ( Qt::FramelessWindowHint, true );
    //setWindowFlag ( Qt::CustomizeWindowHint, true );
    //setWindowFlag ( Qt::SubWindow, true );
    //setWindowFlag ( Qt::WindowSystemMenuHint, true );
    //setAttribute ( Qt::WA_DeleteOnClose, true );

    setPixmap ( conf_root ( "rose.png" ) );
}

QQtOsdForm::~QQtOsdForm()
{
    delete ui;
}


void QQtOsdForm::paintEvent ( QPaintEvent* event )
{
    return QQtWidget::paintEvent ( event );

    QPainter p ( this );

    QPen pen = p.pen();
    pen.setWidth ( 12 );
    p.setPen ( pen );

    QRect r0 ( 0, 0, width(), height() );
    p.drawRect ( r0 );

    return;

    return QWidget::paintEvent ( event );

    //ignore
    // Use a bitmap as a mask. A bitmap only has two kind of colors: white(value is 0)
    // or black(other values). When we use it to set mask, we can see the window at the position
    // where the color value is 0, and other place will be transparent.
    QBitmap bitMap ( width(), height() ); // A bit map has the same size with current widget
    QPainter painter ( &bitMap );
    painter.setPen ( QColor ( 255, 255, 255 ) ); // Any color that is not QRgb(0,0,0) is right
    painter.drawRect ( 0, 0, width(), height() );
    setMask ( bitMap );
}


void QQtOsdForm::mousePressEvent ( QMouseEvent* event )
{
    pline() << event->type() << event->pos();
}
