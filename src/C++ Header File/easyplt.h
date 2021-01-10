#ifndef EASYPLT_H
#define EASYPLT_H

#include <QWidget>

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>
#include <QPushButton>
#include <QLabel>


class EasyPlt : public QWidget
{
    Q_OBJECT

public:
    EasyPlt(QWidget *parent = nullptr);
    ~EasyPlt();


public slots:
    void on_hidbt_clicked();


public:
    void initUI();

    QLineEdit *llng1, *llat1, *hlng1, *hlat1;
    QLineEdit *llng2, *llat2, *hlng2, *hlat2;
    QPushButton *bt;
    QWidget *wg;
    QRadioButton *ar, *br;
    QButtonGroup *bg;
    QPushButton *aply, *hidbt;
    QLabel *hints;
    bool hid;


};

#endif // EASYPLT_H
