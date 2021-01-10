#include "fstfinf.h"
#include "jscontext.h"
#include <QWebEngineView>
#include <QWebChannel>
#include <QDebug>
#include <QDir>
#include <QNetworkProxyFactory>
#include "dockbar.h"
#include <QHBoxLayout>

//public:
FstFInf::FstFInf(QWidget *parent) : QMainWindow(parent), back(NULL)
{
    Init();
}

FstFInf::~FstFInf(){
    delete dir;
}

void FstFInf::setBackInf(QWidget *backInf){
    back = backInf;
}

//protected:
void FstFInf::closeEvent(QCloseEvent *e){
    Q_UNUSED(e);
    back->show();
}


//private:
void FstFInf::Init()
{
    this->setAttribute(Qt::WA_DeleteOnClose, true);
    QNetworkProxyFactory::setUseSystemConfiguration(false);
    dir = new QDir(QDir::currentPath());
    dir->cd("_html_");

    QWidget *centralW = new QWidget;
    centralW->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    this->setCentralWidget(centralW);

    m_platform = new DockBar;
    m_platform->setMinimumHeight(749);
    m_platform->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);

    m_jsContext = new JsContext(this);
    m_webChannel = new QWebChannel(this);
    m_webChannel->registerObject("context", m_jsContext);

    m_webView = new QWebEngineView;
    m_webView->setMinimumWidth(968);
    m_webView->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    m_webView->page()->setWebChannel(m_webChannel);

    connect(m_platform, &DockBar::showLocusUp, this, &FstFInf::showLocus);
    connect(m_jsContext, &JsContext::bcastMsg, this, &FstFInf::msgFromBridge);
    connect(this, &FstFInf::msgToBridge, m_jsContext, &JsContext::innerMsgHandler);
    connect(this, &FstFInf::locusShowed, m_platform, &DockBar::enablebtShow);
    connect(this, &FstFInf::Jsloaded, m_platform, &DockBar::enablebtShow);

    QHBoxLayout *mw_layout = new QHBoxLayout();
    mw_layout->addWidget(m_platform);
    mw_layout->addWidget(m_webView);
    this->centralWidget()->setLayout(mw_layout);

    m_webView->page()->load(QUrl(dir->absoluteFilePath("index1.html")));
    this->showMaximized();
}


//private slots:
void FstFInf::showLocus(const QString &url){
    emit msgToBridge(url, 0, m_webView->page());
}

void FstFInf::msgFromBridge(const QString &msg){
    if(msg == "ready"){
        emit Jsloaded();
        qDebug("ready");
    }
    else if(msg == "showed"){
        emit locusShowed();
        qDebug("showed");
    }
}

