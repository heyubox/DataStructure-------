#include "trdfinf.h"
#include <QHBoxLayout>
#include <QWebEngineView>
#include <QWebChannel>
#include "jscontext.h"
#include "optplatform.h"
#include <QNetworkProxyFactory>
#include <QDir>

//public:
TrdFInf::TrdFInf(QWidget *parent) : QMainWindow(parent), back(NULL)
{
    init();
}

TrdFInf::~TrdFInf(){
    delete dir;
}


void TrdFInf::setBackInf(QWidget *backInf){
    back = backInf;
}


//protected:
void TrdFInf::closeEvent(QCloseEvent *e){
    Q_UNUSED(e);
    back->show();
}

//private:
void TrdFInf::init(){
        this->setAttribute(Qt::WA_DeleteOnClose, true);
        QNetworkProxyFactory::setUseSystemConfiguration(false);
        dir = new QDir(QDir::currentPath());
        dir->cd("_html_");

        QWidget *centralW = new QWidget;
        centralW->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        this->setCentralWidget(centralW);

        m_platform = new OptPlatform;
        m_platform->setMinimumHeight(749);
        m_platform->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);

        m_jsContext = new JsContext(this);
        m_webChannel = new QWebChannel(this);
        m_webChannel->registerObject("context", m_jsContext);

        m_webView = new QWebEngineView;
        m_webView->setMinimumWidth(940);
        m_webView->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        m_webView->page()->setWebChannel(m_webChannel);

        connect(m_jsContext, &JsContext::bcastMsg, this, &TrdFInf::msgFromBridge);
        connect(this, &TrdFInf::msgToBridge, m_jsContext, &JsContext::innerMsgHandler);
        connect(this, &TrdFInf::Jsloaded, m_platform, &OptPlatform::enableButton);
        connect(this, &TrdFInf::editDoneDown, m_platform, &OptPlatform::editDone);
        connect(this, &TrdFInf::amountDoneDown, m_platform, &OptPlatform::amountDone);
        connect(m_platform, &OptPlatform::changeEditModeUp, this, &TrdFInf::changeEditMode);
        connect(m_platform, &OptPlatform::amountUp, this, &TrdFInf::amount);

        QHBoxLayout *mw_layout = new QHBoxLayout;
        mw_layout->addWidget(m_platform);
        mw_layout->addWidget(m_webView);
        this->centralWidget()->setLayout(mw_layout);

        m_webView->page()->load(QUrl(dir->absoluteFilePath("index3.html")));
        this->showMaximized();
}

//public slots:
void TrdFInf::amountDone(int &result){
    if(result > 0 && result < 100000){
        emit msgToBridge("../tempFile.json?callback=callback", 1, m_webView->page());
    }else{
        emit msgToBridge("false", 1, m_webView->page());
    }
    emit amountDoneDown(result);
}

//private slots:
void TrdFInf::amount(double &lowlng, double &lowlat, double &highlng, double &highlat, int &tbegin, int &tend){
    emit amountUp(lowlng, lowlat, highlng, highlat, tbegin, tend);
}

void TrdFInf::changeEditMode(const QString &msg){
    emit msgToBridge(msg, 2, m_webView->page());
}

void TrdFInf::msgFromBridge(const QString &msg){
    if(msg == "ready")
        emit Jsloaded();
    else{
        emit editDoneDown(msg);
    }
}
