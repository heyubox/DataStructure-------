#ifndef MIF_H
#define MIF_H

#include <QDialog>
#include <QEvent>

class LnkButton;
class QLabel;
class FstFInf;
class TrdFInf;
class FixInf;
class ForInf;
class EigInf;
class OurTeam;

class Mif : public QDialog
{
    Q_OBJECT

public:
    Mif(QWidget *parent = nullptr);
    ~Mif();

public:
    LnkButton *btf1, *btf3, *btf56, *btfx, *btf4, *btf8;


protected:
    void closeEvent(QCloseEvent *) override;

private:
    void initUI();

signals:
    void amountUp(double, double, double, double, int, int);
    void amountDoneDown(int &);

    void volume5Up(double, double,double, double,double, double,double, double);
    void volume6Up(double, double,double, double);
    void volumeDoneDown(int *);

    void kPathUp(double, double, double, double, double, double, double, double);
    void kPathDoneDown(int);

    void mifonQuit();

public slots:
    void treeDone();

    void amount(double &, double&, double&, double&, int&, int&);
    void amountDone(int);

    void volume5(double &, double&, double&, double&, double &, double&, double&, double&);
    void volume6(double &, double&, double&, double&);
    void volumeDone(int *);

    void kPath(double &, double &, double &, double &, double &, double &, double &, double &);
    void kPathDone(int);

private slots:
    void onf1();
    void onf3();
    void onf56();
    void onf4();
    void onf8();
    void onfx();

private:
    bool hasTree;
    FstFInf *f1;
    TrdFInf *f3;
    FixInf *f56;
    ForInf *f4;
    EigInf *f8;
    OurTeam *fx;
};
#endif // MIF_H
