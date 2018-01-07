#include "qqtapplication.h"
#ifdef __PLUGINWATCHER__
#include "qqtpluginwatcher.h"
#endif
#include <qqtmsgbox.h>
#include <qqtcore.h>
#include <qqtframe.h>
#include <QFile>
#include <QTextCodec>
#include <QSettings>
#include <QTranslator>
#include <QFontDatabase>
#ifdef __PROCESSMODULE__
#include <QProcess>
#endif
#ifdef __EMBEDDED_LINUX__
#include <qqtinput.h>
#endif

QQtApplication::QQtApplication ( int& argc, char** argv ) :
    QApplication ( argc, argv ),
    bUPanAutoRun ( false )
{
    /*设置Qt框架内部文本编码系统，基础编码。*/
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    QTextCodec::setCodecForTr ( QTextCodec::codecForName ( "UTF-8" ) );
    QTextCodec::setCodecForCStrings ( QTextCodec::codecForName ( "UTF-8" ) );
#endif
    QTextCodec::setCodecForLocale ( QTextCodec::codecForName ( "UTF-8" ) );

    /*嵌入式，应用名称必须指定*/
    QCoreApplication::setOrganizationName ( "QQt" );
    QCoreApplication::setOrganizationDomain ( "www.qqt.com" ); // 专为Mac OS X 准备的
    QCoreApplication::setApplicationName ( "QQt" );

    /*设置配置文件所在路径*/
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings::setPath ( QSettings::IniFormat, QSettings::UserScope, CONFIG_PATH );
    QSettings::setPath ( QSettings::IniFormat, QSettings::SystemScope, CONFIG_PATH );

    /*设置日志系统*/
#ifdef __QQTLOGFILESUPPORT__
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    qInstallMsgHandler ( QQt4FrameMsgHandler );
#else
    qInstallMessageHandler ( QQt5FrameMsgHandler );
#endif
#endif

    /*解决，嵌入式板子上，串口关闭后有时无法打开的问题*/
#ifdef __EMBEDDED_LINUX__
    system ( "rm -f /tmp/LCK..ttyS*" );
#endif

    pline() << qApp->applicationDirPath();
    /*设置语言翻译器，用户需要自己设置语言，（默认为页面上的词汇，设置后为翻译的中文或者英文）*/
    language = new QTranslator ( this );

    /*设置随机数种子*/
    qsrand ( QTime ( 0, 0, 0 ).secsTo ( QTime::currentTime() ) );

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    /*
     * 打印失真与否与此处无关
     */
    QApplication::setGraphicsSystem ( "raster" );
#endif

    /*设置鼠标隐藏*/
#ifdef __EMBEDDED_LINUX__
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    //QApplication::setOverrideCursor(Qt::ArrowCursor);
    QWSServer::setCursorVisible ( false );
#else
    //TODO:
#endif
#endif

    /*嵌入式板子上，初始化输入法*/
#ifdef __EMBEDDED_LINUX__
    QQtInput::Instance()->Init ( "min", "control", "QQT", 14, 14 );
#endif

    /*设置USB热插拔检测，支持U盘，键盘，鼠标检测*/
#ifdef __PLUGINWATCHER__
    QObject::connect ( QQtPluginWatcher::Instance(), SIGNAL ( storageChanged ( int ) ),
                       this, SLOT ( slotUPanAutoRun ( int ) ) );
#endif

}

QQtApplication::~QQtApplication() {
    QFontDatabase::removeAllApplicationFonts();
}


void QQtApplication::setQSSStyle ( QString qssfile )
{
    /*
     * Could not parse application stylesheet 告警 可以忽略
     * 关于选中项的颜色，暂且按照默认，后来更改整体UI颜色和效果
     * 可以实现橙色一行选中
     * 肯定也能实现表头透明和QQT效果。
     */
    QFile styleFile ( qssfile );
    styleFile.open ( QIODevice::ReadOnly );
    QString styleString ( styleFile.readAll() );;
    styleFile.close();
    setStyleSheet ( styleString );
    /*
     * 设置所有默认颜色
     */
    //setPalette(QPalette(QColor("#F0F0F0")));
}


void QQtApplication::setUPanAutorun ( bool run )
{
    bUPanAutoRun = run;
}

void QQtApplication::slotUPanAutoRun ( int status )
{
    if ( !bUPanAutoRun )
        return;

#ifdef __PLUGINWATCHER__

    if ( QQtPluginWatcher::E_ADD == status )
    {
        QString mP = QQtPluginWatcher::Instance()->upanMountPath();
        QString app = QString ( "%1/autorun.sh" ).arg ( mP );
        QFile file ( app );

        if ( file.exists() )
        {
            if ( QDialog::Rejected == QQtMsgBox::question ( 0, tr ( "Some app want to run in u disk!accepted?" ) ) )
            {
                return;
            }
        }
        else
        {
            return;
        }

#ifdef __PROCESSMODULE__
        QProcess* p = new QProcess ( this );
        p->setWorkingDirectory ( mP );
        p->start ( app );
#else
        //TODO:
#endif
    }

#else
    Q_UNUSED ( status )
#endif
}


void QQtApplication::setTextFont ( QString fontfile, int fontsize )
{
    //ignored
    QFontDatabase db;

    int fontID = QFontDatabase::addApplicationFont ( fontfile );
    pline() << "font file:" << fontfile;
    pline() << "font id:" << fontID;
    pline() << "font families:" << QFontDatabase::applicationFontFamilies ( fontID );
    QString ziti = QFontDatabase::applicationFontFamilies ( fontID ).at ( 0 );
    pline() << "font name:" << ziti;

    QFont font ( ziti, fontsize );
    QApplication::setFont ( font );
}


void QQtApplication::setLanguage ( QString qmfile )
{
    language->load ( qmfile );
    pline() << "currentLanguage" << qmfile;
    installTranslator ( language );
}
