#ifndef QQTBODYMOVER_P_H
#define QQTBODYMOVER_P_H

#include <QtCore/qglobal.h>

#include <QWidget>
#include <QMouseEvent>

class QQtBodyMover;
class QQtBodyMoverPrivate
{
public:
    QQtBodyMoverPrivate ( QQtBodyMover* q );
    ~QQtBodyMoverPrivate();

    QMargins& margins();
    const QMargins& margins() const;

public:
    virtual void mousePressEvent ( QMouseEvent* event, QWidget* target = 0 );
    virtual void mouseReleaseEvent ( QMouseEvent* event, QWidget* target = 0 );
    virtual void mouseMoveEvent ( QMouseEvent* event, QWidget* target = 0 );

private:
    QQtBodyMover* q_ptr;
    Q_DECLARE_PUBLIC ( QQtBodyMover )

    bool bMousePressed;
    QPoint pressedPoint;

    QMargins m_margins;
};

#endif //QQTBODYMOVER_P_H