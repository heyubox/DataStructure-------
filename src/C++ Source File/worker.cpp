#include "worker.h"
#include "rstartree.h"
#include <QDebug>
#include "fastreader.h"
#include <QDir>

Worker::Worker(QObject *parent) : QObject(parent)
{
    dir = new QDir(QDir::currentPath());
    //dir->cdUp();
    //dir->cd("BigHomework");


    volumeRe = new int[336];    //336 is the half hours amount from 08/2/2-08/2/8
    timeList = new int[337];
    timeList[0] = 2000000;
    for(int i = 1; i <= 336; i++){
        if(!(i % 48)){timeList[i] = timeList[i-48] + 1000000;}
        else if(i % 2){timeList[i] = timeList[i-1] + 3000;}
        else if(!(i % 2)){timeList[i] = timeList[i-2] + 10000;}
        else assert(0);
    }
}

Worker::~Worker(){
    delete dir;
    delete tree;
    delete reader;
    delete[] volumeRe;
    delete[] timeList;
}

//public slots
void Worker::buildTree(){

    QString path1 = dir->absoluteFilePath("dataSet.dat");
    QByteArray tmp1 = path1.toLatin1();
    const char *p1 = tmp1.data();

    QString path2 = dir->absoluteFilePath("dataIndex.dat");
    QByteArray tmp2 = path2.toLatin1();
    const char *p2 = tmp2.data();

    tree = new RStarTree;
    tree->setPath(p1, p2);
    reader = new FastReader(p1);
    double x[2], y[2];
    int time[2], ID[2];
    qDebug() << "start build";
    while (reader->constructBox(x, y, ID, time)) {
        tree->Insert(ID, time, boundingbox(x[0], y[0], x[1], y[1]), x[0], y[0], x[1], y[1]);
    }
    emit treeDone();
}

void Worker::amount(double lowlng, double lowlat, double highlng, double highlat, int tbegin, int tend){
    double lowedge[2];
    double highedge[2];
    lowedge[0] = lowlng;
    lowedge[1] = lowlat;
    highedge[0] = highlng;
    highedge[1] = highlat;
    qDebug("find entry");
    tree->FindEntry(tree->m_root, lowedge, highedge, tbegin, tend);
    int re = tree->found.size();

    if(re < 100000 && re > 0){
        QString path = dir->absoluteFilePath("tempFile.json");
        QByteArray tmp = path.toLatin1();
        const char *p = tmp.data();
        tree->foundToJsonp(p);
    }
    tree->found.clear();
    tree->found.shrink_to_fit();
    emit amountDoneUp(re);
}

void Worker::volume5(double llng1, double llat1, double highlng1,
                     double highlat1, double llng2, double llat2, double highlng2, double highlat2){

    qDebug()<<llng1<<","<<llat1<<","<<highlng1<<","<<highlat1<<llng2<<","<<llat2<<","<<highlng2<<","<<highlat2;

    double p1_low[2] = {llng1, llat1};
    double p1_high[2] = {highlng1, highlat1};
    double p2_low[2] = {llng2, llat2};
    double p2_high[2] = {highlng2, highlat2};
    tree->trafficVolume5(tree->m_root, p1_low, p1_high, p2_low, p2_high);
    int sizes = tree->volumeTime.size();
    for(int i = 0, count = 0, j = 0; i < 336; i++){
        while(j < sizes && tree->volumeTime[j] >= timeList[i]  && tree->volumeTime[j] < timeList[i+1])
        {++count;   ++j;}
        volumeRe[i] = count;
        count = 0;
    }
    tree->volumeTime.clear();
    tree->volumeTime.shrink_to_fit();
    emit volumeDoneUp(volumeRe);
}

void Worker::volume6(double llng, double llat, double hlng, double hlat){
    qDebug() << llng << "," << llat << "," << hlng << "," << hlat;

    double low[2] = {llng, llat};
    double high[2] = {hlng, hlat};
    tree->trafficVolume6(tree->m_root, low, high);

    int sizes = tree->volumeTime.size();
    for(int i = 0, count = 0, j = 0; i < 336; i++){
        while(j < sizes && tree->volumeTime[j] >= timeList[i]  && tree->volumeTime[j] < timeList[i+1])
        {++count;   ++j;}
        volumeRe[i] = count;
        count = 0;
    }
    tree->volumeTime.clear();
    tree->volumeTime.shrink_to_fit();
    emit volumeDoneUp(volumeRe);
}

void Worker::kPath(double llng1, double llat1, double hlng1, double hlat1,
                   double llng2, double llat2, double hlng2, double hlat2)
{
    QString path = dir->absoluteFilePath("tempFile.json");
    QByteArray tmp = path.toLatin1();
    const char *p = tmp.data();

    qDebug() << llng1 << " " << llat1 << " " << hlng1 << " " << hlat1 << " "
             << llng2 << " " << llat2 << " " << hlng2 << " " << hlat2;

    double p1_low[2] = {llng1, llat1};
    double p1_high[2] = {hlng1, hlat1};
    double p2_low[2] = {llng2, llat2};
    double p2_high[2] = {hlng2, hlat2};
    int tol = tree->Route(tree->m_root, p1_low, p1_high, p2_low, p2_high, p);
    emit kPathDoneUp(tol);
}
