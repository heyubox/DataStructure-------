#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>

class QLabel;
class QLineEdit;
class QLineEdit;
class QSlider;

class TimeOpt : public QWidget
{
    Q_OBJECT

public:
    TimeOpt(QWidget *parent = nullptr);
    ~TimeOpt();

    void initUI();
    void setTime(int);

signals:
    void msgUp(const QString &);


public slots:
    void on_up_clicked();
    void on_down_clicked();
    void on_aply_clicked();
    void on_shid_clicked();
    void on_slider_moved(int);


public:
    QWidget *wg;
    QLabel *sd, *sh, *sm;
    QLabel *ed, *eh, *em;
    QPushButton *shid;
    QPushButton *up, *down;
    QPushButton *aply;
    QLineEdit *input;
    QSlider *slider;
    int *timeList;
    int ind;
    bool hid;


};
#endif // WIDGET_H
