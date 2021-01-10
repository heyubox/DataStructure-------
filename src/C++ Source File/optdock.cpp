#include "optdock.h"
#include <QGridLayout>

OptDock::OptDock(QWidget *parent) : QWidget(parent)
{
    initUI();
    area1c->setChecked(true);
    f5c->setChecked(true);
}


OptDock::~OptDock(){}

void OptDock::initUI(){
    QVBoxLayout *vlayout = new QVBoxLayout;
    QHBoxLayout *hlayout = new QHBoxLayout;
    vlayout->addWidget(groupBox1());
    vlayout->addSpacing(20);
    vlayout->addWidget(groupBox2());
    vlayout->addStretch(1);
    vlayout->addSpacing(5);
    vlayout->addLayout(group3());
    hlayout->addStretch(1);
    hlayout->addLayout(vlayout);
    hlayout->addStretch(1);

    btga = new QButtonGroup(this);
    btga->addButton(area1c, 1);
    btga->addButton(area2c, 2);

    btgf = new QButtonGroup(this);
    btgf->addButton(f5c, 5);
    btgf->addButton(f6c, 6);

    this->setLayout(hlayout);
}

QGroupBox * OptDock::groupBox1(){
    QGroupBox *groupbox = new QGroupBox("Area1");
    QGridLayout *grid = new QGridLayout;
    QLabel *lb_llng = new QLabel("Lowedge lng:");
    QLabel *lb_llat = new QLabel("Lowedge lat:");
    QLabel *lb_hlng = new QLabel("Highedge lng:");
    QLabel *lb_hlat = new QLabel("Highedge lat:");

    lowlng1 = new QLineEdit;
    lowlat1 = new QLineEdit;
    highlng1 = new QLineEdit;
    highlat1 = new QLineEdit;

    lowlng1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    lowlat1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    highlng1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    highlat1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    lowlng1->setEnabled(false);
    lowlat1->setEnabled(false);
    highlng1->setEnabled(false);
    highlat1->setEnabled(false);

    lowlng1->setStyleSheet("background:white;");
    lowlat1->setStyleSheet("background:white;");
    highlng1->setStyleSheet("background:white;");
    highlat1->setStyleSheet("background:white;");

    area1c = new QRadioButton("Area1 selected");

    grid->addWidget(lb_llng, 0, 0, 1, 1);
    grid->addWidget(lowlng1, 0, 1, 1, 1);
    grid->addWidget(lb_llat, 0, 2, 1, 1);
    grid->addWidget(lowlat1, 0, 3, 1, 1);

    grid->addWidget(lb_hlng, 1, 0, 1, 1);
    grid->addWidget(highlng1, 1, 1, 1, 1);
    grid->addWidget(lb_hlat, 1, 2, 1, 1);
    grid->addWidget(highlat1, 1, 3, 1, 1);
    grid->addWidget(area1c, 2, 3, 1, 1);


    groupbox->setLayout(grid);
    return groupbox;
}

QGroupBox * OptDock::groupBox2(){
    QGroupBox *groupbox = new QGroupBox("Area2");
    QGridLayout *grid = new QGridLayout;
    QLabel *lb_llng = new QLabel("Lowedge lng:");
    QLabel *lb_llat = new QLabel("Lowedge lat:");
    QLabel *lb_hlng = new QLabel("Highedge lng:");
    QLabel *lb_hlat = new QLabel("Highedge lat:");

    lowlng2 = new QLineEdit;
    lowlat2 = new QLineEdit;
    highlng2 = new QLineEdit;
    highlat2 = new QLineEdit;

    lowlng2->setEnabled(false);
    lowlat2->setEnabled(false);
    highlng2->setEnabled(false);
    highlat2->setEnabled(false);

    lowlng2->setStyleSheet("background:white;");
    lowlat2->setStyleSheet("background:white;");
    highlng2->setStyleSheet("background:white;");
    highlat2->setStyleSheet("background:white;");

    area2c = new QRadioButton("Area2 selected");

    grid->addWidget(lb_llng, 0, 0, 1, 1);
    grid->addWidget(lowlng2, 0, 1, 1, 1);
    grid->addWidget(lb_llat, 0, 2, 1, 1);
    grid->addWidget(lowlat2, 0, 3, 1, 1);

    grid->addWidget(lb_hlng, 1, 0, 1, 1);
    grid->addWidget(highlng2, 1, 1, 1, 1);
    grid->addWidget(lb_hlat, 1, 2, 1, 1);
    grid->addWidget(highlat2, 1, 3, 1, 1);
    grid->addWidget(area2c, 2, 3, 1, 1);


    groupbox->setLayout(grid);
    return groupbox;
}

QHBoxLayout *OptDock::group3(){
    QPalette palette;
    palette.setBrush(QPalette::WindowText, Qt::red);
    hints = new QLabel("          ");
    hints->setPalette(palette);

    f5c = new QRadioButton("f5");
    f6c = new QRadioButton("f6");

    bt_count = new QPushButton("COUNT");
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(f5c);
    layout->addWidget(f6c);
    layout->addStretch(1);
    layout->addWidget(bt_count);
    layout->addWidget(hints);
    return layout;
}
