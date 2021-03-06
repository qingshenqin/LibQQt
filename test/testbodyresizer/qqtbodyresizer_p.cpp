#include "qqtbodyresizer_p.h"
#include "qqtbodyresizer.h"

#include <QObject>
#include <QWidget>
#include <QMouseEvent>
#ifdef Q_OS_WIN
//#pragma comment(lib, "user32.lib")
#include <qt_windows.h>
#endif
#include <qqtcore.h>

QQtBodyResizerPrivate::QQtBodyResizerPrivate ( QQtBodyResizer* q ) :
    q_ptr ( q )
{
    bMousePressed = false;
    margins = QMargins ( 10, 10, 10, 10 );
}

QQtBodyResizerPrivate::~QQtBodyResizerPrivate()
{
}

void QQtBodyResizerPrivate::mousePressEvent ( QMouseEvent* event, QWidget* target )
{
    Q_ASSERT ( target );

    if ( event->button() == Qt::LeftButton )
    {
        bMousePressed = true;
        pressedPoint = event->globalPos();
    }

    checkDirection ( event, target );

    event->accept();
}

void QQtBodyResizerPrivate::mouseReleaseEvent ( QMouseEvent* event, QWidget* target )
{
    Q_ASSERT ( target );
    bMousePressed = false;
    direction = None;
    event->accept();
}

void QQtBodyResizerPrivate::mouseMoveEvent ( QMouseEvent* event, QWidget* target )
{
    Q_ASSERT ( target );

    QWidget* win = target;


    //如果鼠标没有press，遭遇到窗体边缘，变换鼠标样式。
    if ( !bMousePressed && !win->isMaximized() )
    {
        checkDirection ( event, target );
    }

    //如果鼠标pressed，那么移动。
    if ( bMousePressed && !win->isMaximized() )
    {
        QRect oldGeometry = target->geometry();
        QPoint widgetPos = oldGeometry.topLeft();

        int rx, ry;
        int x, y, width = 0, height = 0;

        QRect newGeometry = oldGeometry;

        switch ( direction )
        {
            case Left: // left
                //cursor
                rx = event->globalPos().x() - pressedPoint.x();
                ry = 0;

                //widget
                x = widgetPos.x() + rx;
                y = widgetPos.y();
                width = oldGeometry.width() - rx;
                height = oldGeometry.height();

                newGeometry = QRect ( x, y, width, height );

                //pline() << event->globalPos().x() << pressedPoint.x() << rx << ry;
                //pline() << oldGeometry << newGeometry;

                break;

            case Right: //right
                //cursor
                rx = event->globalPos().x() - pressedPoint.x();
                ry = 0;

                //widget
                x = widgetPos.x();
                y = widgetPos.y();
                width = oldGeometry.width() + rx;
                height = oldGeometry.height();

                newGeometry = QRect ( x, y, width, height );
                break;

            case LeftTop: //left top
                //cursor
                rx = event->globalPos().x() - pressedPoint.x();
                ry = event->globalPos().y() - pressedPoint.y();

                //widget
                x = widgetPos.x() + rx;
                y = widgetPos.y() + ry;
                width = oldGeometry.width() - rx;
                height = oldGeometry.height() - ry;

                newGeometry = QRect ( x, y, width, height );
                break;

            case RightTop: //right top
                //cursor
                rx = event->globalPos().x() - pressedPoint.x();
                ry = event->globalPos().y() - pressedPoint.y();

                //widget
                x = widgetPos.x();
                y = widgetPos.y() + ry;
                width = oldGeometry.width() + rx;
                height = oldGeometry.height() - ry;

                newGeometry = QRect ( x, y, width, height );

                break;

            case LeftBottom: //left bottom
                //cursor
                rx = event->globalPos().x() - pressedPoint.x();
                ry = event->globalPos().y() - pressedPoint.y();

                //widget
                x = widgetPos.x() + rx;
                y = widgetPos.y();
                width = oldGeometry.width() - rx;
                height = oldGeometry.height() + ry;

                newGeometry = QRect ( x, y, width, height );

                break;

            case RightBottom: //right bottom
                //cursor
                rx = event->globalPos().x() - pressedPoint.x();
                ry = event->globalPos().y() - pressedPoint.y();

                //widget
                x = widgetPos.x();
                y = widgetPos.y();
                width = oldGeometry.width() + rx;
                height = oldGeometry.height() + ry;

                newGeometry = QRect ( x, y, width, height );

                break;

            case Top: //top
                //cursor
                rx = 0;
                ry = event->globalPos().y() - pressedPoint.y();

                //widget
                x = widgetPos.x();
                y = widgetPos.y() + ry;
                width = oldGeometry.width();
                height = oldGeometry.height() - ry;

                newGeometry = QRect ( x, y, width, height );

                break;

            case Bottom: //bottom
                //cursor
                rx = 0;
                ry = event->globalPos().y() - pressedPoint.y();

                //widget
                x = widgetPos.x();
                y = widgetPos.y();
                width = oldGeometry.width();
                height = oldGeometry.height() + ry;

                newGeometry = QRect ( x, y, width, height );
                break;

            default:
                break;
        }

        pressedPoint = event->globalPos();

        if ( width > ( margins.left() + margins.right() ) &&
             height > ( margins.top() + margins.bottom() ) )
        {
            //target->move ( x, y );
            //target->resize ( width, height );
            target->setGeometry ( newGeometry );
        }

    }

    event->accept();
}

void QQtBodyResizerPrivate::checkDirection ( QMouseEvent* event, QWidget* target )
{
    QRect rectMustIn = target->geometry();//target->frameGeometry();
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    QRect rectMustNotIn = rectMustIn.marginsRemoved ( margins );
#else
    QRect rectMustNotIn = rectMustIn.adjusted ( margins.left(), margins.top(), margins.right(), margins.bottom() );
#endif
    QPoint cursorPos = event->globalPos();//QCursor::pos();

    if ( target->isMaximized() ||
         !target->isActiveWindow() ||
         !rectMustIn.contains ( cursorPos ) ||
         rectMustNotIn.contains ( cursorPos ) )
    {
        direction = None;
        target->unsetCursor();
        event->ignore();
        return;
    }

    int baseLeftCoord, baseTopCoord, baseRightCoord, baseBottomCoord;
    rectMustNotIn.getCoords ( &baseLeftCoord, &baseTopCoord, &baseRightCoord, &baseBottomCoord );

    int x, y;
    x = cursorPos.x();
    y = cursorPos.y();

    direction = None;

    if ( x < baseLeftCoord )
    {
        if ( y < baseTopCoord )
        {
            direction = LeftTop;
        }
        else if ( y > baseBottomCoord )
        {
            direction = LeftBottom;
        }
        else
        {
            direction = Left;
        }
    }
    else if ( x > baseRightCoord )
    {
        if ( y < baseTopCoord )
        {
            direction = RightTop;
        }
        else if ( y > baseBottomCoord )
        {
            direction = RightBottom;
        }
        else
        {
            direction = Right;
        }
    }

    if ( direction == None )
    {
        if ( y < baseTopCoord )
        {
            direction = Top;
        }
        else
        {
            direction = Bottom;
        }
    }

    switch ( direction )
    {
        case Left:
        case Right:
            target->setCursor ( Qt::SizeHorCursor );
            break;

        case Top:
        case Bottom:
            target->setCursor ( Qt::SizeVerCursor );
            break;

        case LeftTop:
        case RightBottom:
            target->setCursor ( Qt::SizeFDiagCursor );
            break;

        case LeftBottom:
        case RightTop:
            target->setCursor ( Qt::SizeBDiagCursor );
            break;

        default:
            break;
    }

}
