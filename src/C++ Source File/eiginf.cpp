#include "eiginf.h"
#include "easyplt.h"
#include <QWebEngineView>
#include <QWebChannel>
#include "jscontext.h"
#include <QNetworkProxyFactory>
#include <QDir>



EigInf::EigInf(QWidget *parent) : QMainWindow(parent)
{
    init();
}

EigInf::~EigInf(){
    delete dir;
}


void EigInf::setBackInf(QWidget *backInf){
    back = backInf;
}

void EigInf::closeEvent(QCloseEvent *e){
    Q_UNUSED(e);
    back->show();
}

void EigInf::init(){

    this->setAttribute(Qt::WA_DeleteOnClose, true);
    QNetworkProxyFactory::setUseSystemConfiguration(false);
    dir = new QDir(QDir::currentPath());
    dir->cd("_html_");

    m_jsContext = new JsContext(this);
    m_webChannel = new QWebChannel(this);
    m_webChannel->registerObject("context", m_jsContext);

    m_webView = new QWebEngineView;
    m_webView->page()->setWebChannel(m_webChannel);
    connect(m_jsContext, &JsContext::bcastMsg, this, &EigInf::msgFromBridge);
    connect(this, &EigInf::msgToBridge, m_jsContext, &JsContext::innerMsgHandler);

    this->setCentralWidget(m_webView);

    m_webView->page()->load(QUrl(dir->absoluteFilePath("index8.html")));

    m_platform = new EasyPlt(this);
    m_platform->resize(300,300);
    m_platform->move(0,20);

    connect(m_platform->aply, &QPushButton::clicked, this, &EigInf::on_aply_clicked);
    connect(m_platform->ar, &QRadioButton::clicked, this, &EigInf::on_radiobt_clicked);
    connect(m_platform->br, &QRadioButton::clicked, this, &EigInf::on_radiobt_clicked);

    this->showMaximized();


}

void EigInf::msgFromBridge(const QString &msg){
        if(msg == "ready")
            qDebug() << msg;
        else if(msg == "showed"){
            m_platform->aply->setEnabled(true);
        }
        else{
            QStringList tmp = msg.split(",");
            if(m_platform->ar->isChecked()){
                m_platform->llng1->setText(tmp[2]);
                m_platform->llat1->setText(tmp[3]);
                m_platform->hlng1->setText(tmp[0]);
                m_platform->hlat1->setText(tmp[1]);
            }else{
                m_platform->llng2->setText(tmp[2]);
                m_platform->llat2->setText(tmp[3]);
                m_platform->hlng2->setText(tmp[0]);
                m_platform->hlat2->setText(tmp[1]);
            }
        }

}

void EigInf::on_aply_clicked(){
    m_platform->hints->setText("     ");

    if(m_platform->hlat1->text() == "" || m_platform->hlat2->text() == ""){
        qDebug("error data");
        m_platform->hints->setText("error");
        return ;
    }

    double lowlng1 = m_platform->llng1->text().toDouble();
    double lowlat1 = m_platform->llat1->text().toDouble();
    double highlng1 = m_platform->hlng1->text().toDouble();
    double highlat1 = m_platform->hlat1->text().toDouble();

    double lowlng2 = m_platform->llng2->text().toDouble();
    double lowlat2 = m_platform->llat2->text().toDouble();
    double highlng2 = m_platform->hlng2->text().toDouble();
    double highlat2 = m_platform->hlat2->text().toDouble();

    qDebug() << lowlng1 << " " << lowlat1 << " " << highlng1 << " " << highlat1 << " "
             << lowlng2 << " " << lowlat2 << " " << highlng2 << " " << highlat2;

    if((lowlat1 >= lowlat2 && lowlat1 <= highlat2 && highlng1 >= lowlng2 && highlng1 <= highlng2) ||
       (highlat1 >= lowlat2 && highlat1 <= highlat2 && lowlng1 >= lowlng2 && lowlng1 <= highlng2) ||
        (lowlat1 >= lowlat2 && lowlat1 <= highlat2 && lowlng1 >= lowlng2 && lowlng1 <= highlng2) ||
        (highlat1 >= lowlat2 && highlat1 <= highlat2 && highlng1 >= lowlng2 && highlng1 <= highlng2)){
        qDebug("error data");
        m_platform->hints->setText("error");
        return ;
    }

    m_platform->aply->setEnabled(false);
    emit kPathUp(lowlng1, lowlat1, highlng1, highlat1, lowlng2, lowlat2, highlng2, highlat2);
}

void EigInf::kPathDone(int tol){
    if(tol > 700000){
        emit msgToBridge("false", 5, m_webView->page());
        m_platform->aply->setEnabled(true);
    }
    else
        emit msgToBridge("../tempFile.json?callback=callback", 5, m_webView->page());
}

void EigInf::on_radiobt_clicked(){
    emit msgToBridge("", 6, m_webView->page());
}
