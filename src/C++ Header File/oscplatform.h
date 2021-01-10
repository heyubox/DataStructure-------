#ifndef OSCPLATFORM_H
#define OSCPLATFORM_H

#include <QWidget>
#include <QComboBox>

class QSlider;
class QPushButton;

class OSCplatform : public QWidget
{
    Q_OBJECT

public:
    OSCplatform(volatile bool *dat, QWidget *parent = nullptr);
    ~OSCplatform();

signals:
    void RangeC(int);

public slots:
    void incSliderVal();
    void decSliderVal();
    void sliderRangeC(int);

public:
    volatile bool *data;
    QSlider *slider;
    QPushButton *add, *sub;
    QComboBox *cycleBox;
};
#endif // OSCPLATFORM_H
