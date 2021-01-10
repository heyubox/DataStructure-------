#include "oscplatform.h"
#include <QSlider>
#include <QDebug>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>

OSCplatform::OSCplatform(volatile bool *dat, QWidget *parent)
    : QWidget(parent)
{
   data = dat;

   this->setAutoFillBackground(true);
   QPalette palette(QPalette::Background, QColor("#969696"));
   this->setPalette(palette);

   slider = new QSlider(Qt::Horizontal);
   slider->setMinimumSize(202,32);
   slider->setMinimum(0);  // 最小值
   slider->setMaximum(1);  // 最大值
   slider->setValue(0);
   slider->setEnabled(false);
   slider->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

   add = new QPushButton;
   add->setObjectName("add");
   add->setEnabled(false);
   add->setMinimumSize(32,34);
   add->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
   sub = new QPushButton;
   sub->setObjectName("sub");
   sub->setEnabled(false);
   sub->setMinimumSize(32,34);
   sub->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

   QFont font("宋体", 10, 1);
   QLabel *lb_cycle = new QLabel;
   lb_cycle->setFont(font);
   lb_cycle->setText("Cycle:");

   cycleBox = new QComboBox;
   cycleBox->addItem("24h");
   cycleBox->addItem("12h");
   cycleBox->addItem("6h");
   cycleBox->addItem("3h");
   cycleBox->addItem("1h");
   cycleBox->addItem("30min");

   this->setStyleSheet("\
   QSlider:horizontal{background-color:#4B4B4B;padding: 0;margin: 0;width:400px;height:30px;border: 1px solid #000000;}\
   QSlider::groove:horizontal{height:6px;width:380px;padding:0;background-color:transparent;}\
   QSlider::add-page:horizontal{background:#dddddd;}\
   QSlider::sub-page:horizontal{background:#00ccff;}\
   QSlider::handle:horizontal{border-image:url(:/handle.png);margin:-5px -8px;}\
   QPushButton#add:enabled{border-image:url(:/add.png);}\
   QPushButton#add:pressed, QPushButton#add:disabled{border-image:url(:/add1.png);}\
   QPushButton#sub:enabled{border-image:url(:/sub.png);}\
   QPushButton#sub:pressed, QPushButton#sub:disabled{border-image:url(:/sub1.png);}\
   ");


   QHBoxLayout *layout = new QHBoxLayout;
   layout->addStretch(1);
   layout->addWidget(sub);
   layout->addWidget(slider);
   layout->addWidget(add);
   layout->addSpacing(10);
   layout->addWidget(lb_cycle);
   layout->addWidget(cycleBox);
   layout->addStretch(1);
   this->setLayout(layout);

   connect(add, &QPushButton::clicked, this, &OSCplatform::incSliderVal);
   connect(sub, &QPushButton::clicked, this, &OSCplatform::decSliderVal);
   connect(cycleBox, SIGNAL(currentIndexChanged(int)),  this, SLOT(sliderRangeC(int)));

}

void OSCplatform::incSliderVal(){
    if(slider->value() == slider->maximum())
        return ;
    slider->setValue(slider->value()+1);
}

void OSCplatform::decSliderVal(){
    if(!slider->value()) return;
    slider->setValue(slider->value()-1);
}

void OSCplatform::sliderRangeC(int now){
    qDebug() << now;
    if(now == 0){
        slider->setMinimum(0);
        slider->setMaximum(1);
    }else if(now == 1){
        slider->setMinimum(0);
        slider->setMaximum(7);
    }else if(now == 2){
        slider->setMinimum(0);
        slider->setMaximum(21);
    }else if(now == 3){
        slider->setMinimum(0);
        slider->setMaximum(49);
    }else if(now == 4){
        slider->setMinimum(0);
        slider->setMaximum(161);
    }else if(now == 5){
        slider->setMinimum(0);
        slider->setMaximum(329);
    }else{
        assert(0);
    }

    if(*data && now){
        slider->setEnabled(true);
        add->setEnabled(true);
        sub->setEnabled(true);
    }else{
        slider->setEnabled(false);
        add->setEnabled(false);
        sub->setEnabled(false);
    }

    slider->setValue(0);
    emit RangeC(now);
}

OSCplatform::~OSCplatform()
{
}
