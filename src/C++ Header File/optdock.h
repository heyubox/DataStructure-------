#ifndef OPTDOCK_H
#define OPTDOCK_H

#include <QWidget>
#include <QButtonGroup>
#include <QRadioButton>
#include <QLabel>
#include <QLineEdit>
#include <QGroupBox>
#include <QPushButton>

class QGroupBox;
class QHBoxLayout;



class OptDock : public QWidget
{
    Q_OBJECT
public:
    explicit OptDock(QWidget *parent = nullptr);
    ~OptDock();



private:
    void initUI();


public:
    QLineEdit *lowlng1, *lowlat1, *highlng1, *highlat1;
    QLineEdit *lowlng2, *lowlat2, *highlng2, *highlat2;
    QRadioButton *area1c, *area2c, *f5c, *f6c;
    QPushButton *bt_count;
    QButtonGroup *btgf, *btga;
    QLabel *hints;

private:
    QGroupBox *groupBox1();
    QGroupBox *groupBox2();
    QHBoxLayout *group3();

};

#endif // OPTDOCK_H
