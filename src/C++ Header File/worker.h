/*the Object work on the subThread to load data and build tree*/


#ifndef WORKER_H
#define WORKER_H

#include <QObject>

class RStarTree;
class FastReader;
class QDir;

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = nullptr);
    ~Worker();

signals:
    void treeDone();
    void amountDoneUp(int);
    void volumeDoneUp(int *);
    void kPathDoneUp(int);

public slots:
    void buildTree();   //start build Rstar tree
    void amount(double, double, double, double, int, int);
    void volume5(double, double, double, double, double, double, double, double);
    void volume6(double, double, double, double);
    void kPath(double, double, double, double, double, double, double, double);

private:
    RStarTree *tree;
    FastReader *reader;
    int *volumeRe;
    int *timeList;
    QDir *dir;
};

#endif // WORKER_H
