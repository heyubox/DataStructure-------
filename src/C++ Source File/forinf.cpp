#include "forinf.h"
#include <QNetworkProxyFactory>
#include <QDir>
#include "jscontext.h"
#include <QWebChannel>
#include <QWebEngineView>
#include "timeopt.h"
#include <QDebug>

ForInf::ForInf(QWidget *parent) : QMainWindow(parent)
{
    init();
    connect(m_platform, &TimeOpt::msgUp, this, &ForInf::msgPack);
    connect(this, &ForInf::msgToBridge, m_jsContext, &JsContext::innerMsgHandler);
}

ForInf::~ForInf(){
    delete dir;
}

void ForInf::setBackInf(QWidget *b){
    back = b;
}

void ForInf::init(){
    this->setAttribute(Qt::WA_DeleteOnClose, true);
    QNetworkProxyFactory::setUseSystemConfiguration(false);
    dir = new QDir(QDir::currentPath());
    dir->cd("_html_");

    m_jsContext = new JsContext(this);
    m_webChannel = new QWebChannel(this);
    m_webChannel->registerObject("context", m_jsContext);

    m_webView = new QWebEngineView;
    m_webView->page()->setWebChannel(m_webChannel);
    connect(m_jsContext, &JsContext::bcastMsg, this, &ForInf::msgFromBridge);

    this->setCentralWidget(m_webView);

    m_webView->page()->load(QUrl(dir->absoluteFilePath("index4.html")));

    m_platform = new TimeOpt(this);
    m_platform->move(0,20);



    this->showMaximized();
}

void ForInf::msgFromBridge(const QString &msg){
    if(msg == "ready")
        qDebug() << msg;
    else if(msg == "showed"){
       m_platform->aply->setEnabled(true);
    }
}

void ForInf::closeEvent(QCloseEvent *event){
    Q_UNUSED(event);
    back->show();
}

void ForInf::msgPack(const QString &msg){
    emit msgToBridge(msg, 4, m_webView->page());
}
