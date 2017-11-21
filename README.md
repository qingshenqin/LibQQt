
# QQt Foundation Class  

==============================================  
1. 支持绘制图表  
2. 支持检测USB设备热插拔  
3. 自定义了一部分特殊需要的控件，用来解决Qt的嵌入式bug。
    - 包括完全使用图片的checkbox（switchbutton）
    - QQtTabWidget，解决QtTab不能竖排横显文字的问题
    - QQt进度条，解决Qt进度条在Dialog中导致系统死机的问题
1. 添加精美控件
    - 包括复杂的圆盘进度条, SpeedMeter    
    - 包括有多种效果的Widget、Dialog
    - 二维码、校验码生成器
4. 有线和无线网络自动连接管理类  
6. 解决Qt4.8.7在嵌入式屏幕上QGraphics系统不正常绘画的问题 
7. QQt输入法
    - 从网络资源改编而来
8. Qt对象工厂，允许注册和查找和删除所有来自QObject的类。
9. 嵌入式视频播放器，模拟端口实时预览器
0. QQt打印机，支持打印pdf
2. QQtWord，支持doc文档编写，输出pdf格式。
4. QQt TreeView，添加Qt4内部没有TreeView
    - 包括QQtXmlTreeModel/QQtJsonTreeModel/QQtFileSystemTreeModel/QQtSqlTreeModel
5. 支持工程的版本变更
    - 添加config.h.in支持
6. QQt通讯套件。
     - 通讯口类   
        - QQtSerialPort 兼容QSerialPort and QextSerialPort
        - QQtTcpClient/QQtTcpServer/QQtUdpClient/QQtUdpServer
        - QQtBluetooth +QQtBluetoothManager
        - QQtWebWork
        - QQtWebSocket
     - 协议虚类（接口类） QQtProtocol
     - 报文虚类（接口类） QQtMessage
7. 添加应用中常用的form
8. 支持多页TableWidget
9. 添加QQtApplication，支持入门级、通用级、专用级嵌入式App所必须的初始化内容
5. 支持Qt5.9.2   
0. 跨平台支持macOS、iOS、Android、Windows、Linux、MIPS、ARM等。  
1. 添加svg support widgets  
     - 比如 QQtSvgProgressbar  

# Usage  

==============================================  
include (link_qqt_library.pri)  to your app.  
*MOD: QQT_SOURCE_ROOT and QQT_BUILD_ROOT in your computer.*


# Contact Me  

==============================================  
Mail me: tianduanrui@163.com  
My qq: 2657635903  
Make issues: [new issue link](https://gitee.com/drabel/LibQt/issues/new?issue%5Bassignee_id%5D=0&issue%5Bmilestone_id%5D=0)  



