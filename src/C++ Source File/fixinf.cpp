#include "fixinf.h"
#include "jscontext.h"
#include "oscgraph.h"
#include "oscplatform.h"
#include "optdock.h"
#include <QWebEngineView>
#include <QWebChannel>
#include <QNetworkProxyFactory>
#include <QDir>
#include <QVBoxLayout>

FixInf::FixInf(QWidget *parent) : QMainWindow(parent)
{
    init();

    connect(m_bar, &OSCplatform::RangeC, m_graph, &OSCgraph::setCycLen);
    connect(m_bar->slider, SIGNAL(valueChanged(int)), m_graph, SLOT(setCycle(int)));

    connect(this, &FixInf::msgToBridge, m_jsContext, &JsContext::innerMsgHandler);

    connect(m_platform->btga, SIGNAL(buttonClicked(int)), this, SLOT(editModeC()));
    connect(m_platform->btgf, SIGNAL(buttonClicked(int)), this, SLOT(editModeC()));

    connect(m_platform->bt_count, &QPushButton::clicked, this, &FixInf::on_count_clicked);

}

FixInf::~FixInf(){
    delete dir;
}

void FixInf::setBackInf(QWidget *bac){
    back = bac;
}

void FixInf::closeEvent(QCloseEvent *){
    qDebug() << this->height();
    back->show();
}

void FixInf::init(){

    data = false;

    ary = new int*[6];
    ary[0] = new int[7];
    ary[1] = new int[14];
    ary[2] = new int[28];
    ary[3] = new int[56];
    ary[4] = new int[168];
    ary[5] = new int[336];

    this->setAttribute(Qt::WA_DeleteOnClose, true);
    QNetworkProxyFactory::setUseSystemConfiguration(false);
    dir = new QDir(QDir::currentPath());
    dir->cd("_html_");

    m_jsContext = new JsContext(this);
    m_webChannel = new QWebChannel(this);
    m_webChannel->registerObject("context", m_jsContext);

    m_webView = new QWebEngineView;
    m_webView->page()->setWebChannel(m_webChannel);
    connect(m_jsContext, &JsContext::bcastMsg, this, &FixInf::msgFromBridge);


    centralW = new QWidget;
    centralW->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);


    m_graph = new OSCgraph(&data, ary);
    m_graph->setMinimumSize(660, 368);
    m_graph->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    m_bar = new OSCplatform(&data);
    m_bar->setMinimumWidth(660);
    m_bar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    m_platform = new OptDock;
    m_platform->setMinimumWidth(660);
    m_platform->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);


    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addWidget(m_graph);
    vlayout->addWidget(m_bar);
    vlayout->addWidget(m_platform);
    vlayout->addStretch(1);

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addLayout(vlayout);
    hlayout->addWidget(m_webView);
    centralW->setLayout(hlayout);
    this->setCentralWidget(centralW);

    m_webView->page()->load(QUrl(dir->absoluteFilePath("index56.html")));
    this->showMaximized();

}

void FixInf::editModeC(){

    if(m_platform->btgf->checkedId() == 6){
        m_platform->area1c->setChecked(true);
        m_platform->area2c->setEnabled(false);
        m_platform->lowlng2->clear();
        m_platform->lowlat2->clear();
        m_platform->highlng2->clear();
        m_platform->highlat2->clear();

        emit msgToBridge("f6a1", 3, m_webView->page());
    }else if(m_platform->btga->checkedId() == 1){
        m_platform->area2c->setEnabled(true);
        emit msgToBridge("f5a1", 3, m_webView->page());
    }else{
        m_platform->area2c->setEnabled(true);
        emit msgToBridge("f5a2", 3, m_webView->page());
    }
}

void FixInf::msgFromBridge(const QString& msg){
    if(msg == "ready"){
        qDebug() << msg;
    }else{
        QStringList tmp = msg.split(',');
        if(m_platform->btga->checkedId() == 1){
            m_platform->lowlng1->setText(tmp.at(2));
            m_platform->lowlat1->setText(tmp.at(3));
            m_platform->highlng1->setText(tmp.at(0));
            m_platform->highlat1->setText(tmp.at(1));
        }else{
            m_platform->lowlng2->setText(tmp.at(2));
            m_platform->lowlat2->setText(tmp.at(3));
            m_platform->highlng2->setText(tmp.at(0));
            m_platform->highlat2->setText(tmp.at(1));
        }
    }
}

void FixInf::on_count_clicked(){
    if(m_platform->btgf->checkedId() == 5){

        if(m_platform->lowlng1->text() == "" || m_platform->lowlng2->text() == ""){
            m_platform->hints->setText("Input error");
            return ;
        }else{
            m_platform->hints->setText("           ");
            double llng1 = m_platform->lowlng1->text().toDouble();
            double llat1 = m_platform->lowlat1->text().toDouble();
            double hlng1 = m_platform->highlng1->text().toDouble();
            double hlat1 = m_platform->highlat1->text().toDouble();
            double llng2 = m_platform->lowlng2->text().toDouble();
            double llat2 = m_platform->lowlat2->text().toDouble();
            double hlng2 = m_platform->highlng2->text().toDouble();
            double hlat2 = m_platform->highlat2->text().toDouble();
            qDebug()<<llng1<<","<<llat1<<","<<hlng1<<","<<hlat1<<","<<llng2<<","<<llat2<<","<<hlng2<<","<<hlat2;
            data = false;
            emit Volume5Up(llng1, llat1, hlng1, hlat1, llng2, llat2, hlng2, hlat2);
        }
    }
    else{
        if(m_platform->lowlng1->text() == ""){
            m_platform->hints->setText("lack");
            return ;
        }else{
            m_platform->hints->setText("           ");
            double llng1 = m_platform->lowlng1->text().toDouble();
            double llat1 = m_platform->lowlat1->text().toDouble();
            double hlng1 = m_platform->highlng1->text().toDouble();
            double hlat1 = m_platform->highlat1->text().toDouble();
            qDebug()<<llng1<<","<<llat1<<","<<hlng1<<","<<hlat1;
            data = false;
            emit Volume6Up(llng1, llat1, hlng1, hlat1);
        }
    }
}

void FixInf::volumeDone(int *ptr){

    int total = 0;
    for(int i = 0; i < 336; i++){
        ary[5][i] = ptr[i];
        total += ary[5][i];
    }
    for(int i = 0, j = 0; i < 168; i++, j = 2 * i)
        ary[4][i] = ary[5][j] + ary[5][j+1];
    for(int i = 0, j = 0; i < 56; i++, j = 3 * i)
        ary[3][i] = ary[4][j] + ary[4][j+1] + ary[4][j+2];
    for(int i = 0,j = 0; i < 28; i++, j = 2 * i)
        ary[2][i] = ary[3][j] + ary[3][j+1];
    for(int i = 0, j = 0; i < 14; i++, j = 2 * i)
        ary[1][i] = ary[2][j] + ary[2][j+1];
    for(int i = 0, j = 0; i < 7; i++, j = 2 * i)
        ary[0][i] = ary[1][j] + ary[1][j+1];


    m_graph->setTotal(total);
    data = true;
    m_bar->slider->setValue(0);
    m_graph->setCycle(0);

    qDebug() << "count done";
}
