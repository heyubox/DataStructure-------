#include "mif.h"

#include <QApplication>
#include"lnkbutton.h"
#include <QThread>
#include "worker.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Mif mif;
    Worker m_worker;
    QThread m_thread;
    m_worker.moveToThread(&m_thread);

    QObject::connect(&m_thread, &QThread::started, &m_worker, &Worker::buildTree);
    QObject::connect(&m_worker, &Worker::treeDone, &mif, &Mif::treeDone);

    QObject::connect(&mif, &Mif::amountUp, &m_worker, &Worker::amount);
    QObject::connect(&m_worker, &Worker::amountDoneUp, &mif, &Mif::amountDone);

    QObject::connect(&mif, &Mif::volume5Up, &m_worker, &Worker::volume5);
    QObject::connect(&mif, &Mif::volume6Up, &m_worker, &Worker::volume6);

    QObject::connect(&m_worker, &Worker::volumeDoneUp, &mif, &Mif::volumeDone);

    QObject::connect(&mif, &Mif::kPathUp, &m_worker, &Worker::kPath);
    QObject::connect(&m_worker, &Worker::kPathDoneUp, &mif, &Mif::kPathDone);

    QObject::connect(&mif, &Mif::mifonQuit, &m_thread, &QThread::quit);
    QObject::connect(&m_thread, &QThread::finished, &app, &QApplication::quit);


    mif.show();

    m_thread.start();

    return app.exec();
}
