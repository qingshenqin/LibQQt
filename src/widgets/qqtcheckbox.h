#ifndef QQTCHECKBOX_H
#define QQTCHECKBOX_H

#include <QCheckBox>
#include "qqtcore.h"
#include "qqtwidgets.h"
#include "qqt-local.h"
#include <qqtwidgets.h>

/**
 * @brief The QQtCheckBox class
 * 为了图片占据全部全部控件而编写。
 */
class QQTSHARED_EXPORT QQtCheckBox : public QCheckBox
{
    Q_OBJECT

public:
    explicit QQtCheckBox ( QWidget* parent = 0 );
    virtual ~QQtCheckBox();

    QImage stateImage ( int index );
    void setStateImage ( int index, const QImage& image );

    //normal, press; uncheck, check; [0,1];
    void setNormalImage ( const QImage& normal, const QImage& press );
    //hover; [2];
    void setHoverImage ( const QImage& hover );
    //disable; [4];
    void setDisableImage ( const QImage& disable );

    void setEnabled ( bool );
    void setDisabled ( bool );

    const TBtnImageTable& imageTable() const;
    TBtnImageTable& imageTable();
    //使用ImageTable必须手动translateImage()一次，update不管用。
    virtual void translateImage();

    int workState() const;

protected:
    void setWorkState ( int index );
    virtual void setImage ( const QImage& image );
protected:

private:
    EBtnStatus mWorkState;
    TBtnImageTable mImageTable;
    QImage mImage;

    // QWidget interface
protected:
    virtual void mousePressEvent ( QMouseEvent* event ) override;
    virtual void mouseReleaseEvent ( QMouseEvent* event ) override;
    virtual void enterEvent ( QEvent* event ) override;
    virtual void leaveEvent ( QEvent* event ) override;

    // QWidget interface
protected:
    virtual void paintEvent ( QPaintEvent* event ) override;

    // QAbstractButton interface
protected:
    bool hitButton ( const QPoint& pos ) const;
};

#endif // QQTCHECKBOX_H
