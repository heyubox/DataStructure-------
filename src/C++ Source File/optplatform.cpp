#include "optplatform.h"
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QDebug>


//public:
OptPlatform::OptPlatform(QWidget *parent)
    : QWidget(parent)
{
    editMode = false;
    initUI();
    connect(bt_apply, &QPushButton::clicked, this, &OptPlatform::on_bt_apply_clicked);
    connect(bt_sw, &QPushButton::clicked, this, &OptPlatform::on_bt_sw_clicked);
}

OptPlatform::~OptPlatform()
{
    
}


//private:
QGroupBox* OptPlatform::llowgroup(){

    QGroupBox *grid = new QGroupBox("Lowedge Input");
    QLabel *lb_lowlng = new QLabel("longitude: ");
    QLabel *lb_lowlat = new QLabel("latitude: ");
    edit_lowlng = new QLineEdit;
    edit_lowlat = new QLineEdit;

    QHBoxLayout *layout_1 = new QHBoxLayout;
    QHBoxLayout *layout_2 = new QHBoxLayout;
    QVBoxLayout *layout_3 = new QVBoxLayout;

    layout_1->addWidget(lb_lowlng);
    layout_1->addWidget(edit_lowlng);
    layout_2->addWidget(lb_lowlat);
    layout_2->addWidget(edit_lowlat);
    layout_3->addLayout(layout_1);
    layout_3->addLayout(layout_2);
    grid->setLayout(layout_3);
    return grid;
}

QGroupBox* OptPlatform::lhighgroup(){
    QGroupBox *grid = new QGroupBox("Highedge Input");
    QLabel *lb_lowlng = new QLabel("longitude: ");
    QLabel *lb_lowlat = new QLabel("latitude: ");
    edit_highlng = new QLineEdit;
    edit_highlat = new QLineEdit;

    QHBoxLayout *layout_1 = new QHBoxLayout;
    QHBoxLayout *layout_2 = new QHBoxLayout;
    QVBoxLayout *layout_3 = new QVBoxLayout;

    layout_1->addWidget(lb_lowlng);
    layout_1->addWidget(edit_highlng);
    layout_2->addWidget(lb_lowlat);
    layout_2->addWidget(edit_highlat);
    layout_3->addLayout(layout_1);
    layout_3->addLayout(layout_2);
    grid->setLayout(layout_3);
    return grid;
}

QGroupBox* OptPlatform::tbeggroup(){

    QGroupBox *grid = new QGroupBox("begin Time:");

    QLabel *lb_d = new QLabel("day:");
    QLabel *lb_h = new QLabel("hour:");
    QLabel *lb_m = new QLabel("min:");
    QLabel *lb_s = new QLabel("sec:");

    bdbox = new QSpinBox;
    bhbox = new QSpinBox;
    bmbox = new QSpinBox;
    bsbox = new QSpinBox;

    QHBoxLayout *layout_1 = new QHBoxLayout;
    QHBoxLayout *layout_2 = new QHBoxLayout;
    QHBoxLayout *layout_3 = new QHBoxLayout;
    QHBoxLayout *layout_4 = new QHBoxLayout;

    layout_1->addWidget(lb_d);
    layout_1->addWidget(bdbox);
    layout_2->addWidget(lb_h);
    layout_2->addWidget(bhbox);
    layout_3->addWidget(lb_m);
    layout_3->addWidget(bmbox);
    layout_4->addWidget(lb_s);
    layout_4->addWidget(bsbox);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addLayout(layout_1);
    vlayout->addLayout(layout_2);
    vlayout->addLayout(layout_3);
    vlayout->addLayout(layout_4);

    grid->setLayout(vlayout);
    return grid;
}

QGroupBox* OptPlatform::tendroup(){
    QGroupBox *grid = new QGroupBox("end Time:");

    QLabel *lb_d = new QLabel("day:");
    QLabel *lb_h = new QLabel("hour:");
    QLabel *lb_m = new QLabel("min:");
    QLabel *lb_s = new QLabel("sec:");

    edbox = new QSpinBox;
    ehbox = new QSpinBox;
    embox = new QSpinBox;
    esbox = new QSpinBox;

    QHBoxLayout *layout_1 = new QHBoxLayout;
    QHBoxLayout *layout_2 = new QHBoxLayout;
    QHBoxLayout *layout_3 = new QHBoxLayout;
    QHBoxLayout *layout_4 = new QHBoxLayout;

    layout_1->addWidget(lb_d);
    layout_1->addWidget(edbox);
    layout_2->addWidget(lb_h);
    layout_2->addWidget(ehbox);
    layout_3->addWidget(lb_m);
    layout_3->addWidget(embox);
    layout_4->addWidget(lb_s);
    layout_4->addWidget(esbox);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addLayout(layout_1);
    vlayout->addLayout(layout_2);
    vlayout->addLayout(layout_3);
    vlayout->addLayout(layout_4);

    grid->setLayout(vlayout);
    return grid;
}

void OptPlatform::initUI(){
    QFont font;
    font.setBold(true);
    font.setPixelSize(20);
    QLabel *title = new QLabel;
    title->setFont(font);
    title->setText("Search Amount");

    QGroupBox *llgroup = llowgroup();
    llgroup->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    llgroup->setParent(this);

    QGroupBox *lhgroup = lhighgroup();
    lhgroup->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    lhgroup->setParent(this);

    QGroupBox *tbgroup = tbeggroup();
    tbgroup->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    tbgroup->setParent(this);

    QGroupBox *tegroup = tendroup();
    tegroup->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    tegroup->setParent(this);

    bt_sw = new QPushButton("Switch");
    bt_sw->setEnabled(false);
    bt_apply = new QPushButton("Apply");
    bt_apply->setEnabled(false);
    bt_sw->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    bt_apply->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QHBoxLayout * hlayout_1 = new QHBoxLayout;
    hlayout_1->addSpacing(50);
    hlayout_1->addWidget(bt_sw);
    hlayout_1->addWidget(bt_apply);
    hlayout_1->addStretch(1);

    QHBoxLayout *hlayout_2 = new QHBoxLayout;
    QPalette palette;
    palette.setColor(QPalette::WindowText, Qt::red);
    QLabel *sup = new QLabel(" ");
    hints = new QLabel;
    hints->setPalette(palette);
    hints->setText("Input error");
    hlayout_2->addWidget(hints);
    hlayout_2->addWidget(sup);
    hints->hide();

    QHBoxLayout *hlayout_3 = new QHBoxLayout;
    result = new QLineEdit;
    result->setReadOnly(true);
    QLabel *re = new QLabel("Result: ");
    hlayout_3->addWidget(re);
    hlayout_3->addWidget(result);

    QVBoxLayout *layout_1 = new QVBoxLayout;
    layout_1->addWidget(title);
    layout_1->addSpacing(10);
    layout_1->addWidget(llgroup);
    layout_1->addSpacing(10);
    layout_1->addWidget(lhgroup);
    layout_1->addSpacing(10);
    layout_1->addWidget(tbgroup);
    layout_1->addSpacing(10);
    layout_1->addWidget(tegroup);
    layout_1->addLayout(hlayout_2);
    layout_1->addLayout(hlayout_1);
    layout_1->addSpacing(40);
    layout_1->addLayout(hlayout_3);
    layout_1->addStretch(1);
    this->setLayout(layout_1);

    QRegExp rx("^(\\d{1,3}(\\.\\d{1,6}))|(\\d{1,3})$");
    QRegExpValidator *validator = new QRegExpValidator(rx,this);
    edit_lowlng->setValidator(validator);
    edit_lowlat->setValidator(validator);
    edit_highlng->setValidator(validator);
    edit_highlat->setValidator(validator);

    bdbox->setRange(2,9);   bdbox->setWrapping(true);
    edbox->setRange(2,9);   edbox->setWrapping(true);
    bhbox->setRange(0,23);  bhbox->setWrapping(true);
    ehbox->setRange(0,23);  ehbox->setWrapping(true);
    bmbox->setRange(0,59);  bmbox->setWrapping(true);
    embox->setRange(0,59);  embox->setWrapping(true);
    bsbox->setRange(0,59);  bsbox->setWrapping(true);
    esbox->setRange(0,59);  esbox->setWrapping(true);
}


//public slots:
void OptPlatform::amountDone(int& amount){
    result->setText(QString::number(amount));
    bt_sw->setEnabled(true);
    bt_apply->setEnabled(true);
}

void OptPlatform::editDone(const QString &msg){
    QStringList numlist = msg.split(',');
    edit_highlng->setText(numlist.at(0));
    edit_highlat->setText(numlist.at(1));
    edit_lowlng->setText(numlist.at(2));
    edit_lowlat->setText(numlist.at(3));
}

void OptPlatform::enableButton(){
    bt_sw->setEnabled(true);
    bt_apply->setEnabled(true);
    qDebug("ready");
}


//private slots:
void OptPlatform::on_bt_apply_clicked(){

    double lowlng = edit_lowlng->text().toDouble();
    double lowlat = edit_lowlat->text().toDouble();
    double highlng = edit_highlng->text().toDouble();
    double highlat = edit_highlat->text().toDouble();

    if(highlng <= lowlng || highlat <= lowlat){
        hints->show();
        return ;
    }
    int tbegin = bdbox->value() * 1000000 + bhbox->value() * 10000 + bmbox->value() * 100 + bsbox->value();
    int tend =   edbox->value() * 1000000 + ehbox->value() * 10000 + embox->value() * 100 + esbox->value();
    if(tbegin >= tend){
        hints->show();
        return ;
    }
    hints->hide();
    bt_sw->setEnabled(false);
    bt_apply->setEnabled(false);
    emit amountUp(lowlng, lowlat, highlng, highlat, tbegin, tend);
}

void OptPlatform::on_bt_sw_clicked(){
    if(!editMode){
        edit_lowlng->setReadOnly(true);
        edit_lowlat->setReadOnly(true);
        edit_highlng->setReadOnly(true);
        edit_highlat->setReadOnly(true);

        edit_lowlng->clear();
        edit_lowlat->clear();
        edit_highlat->clear();
        edit_highlng->clear();

        editMode = true;
        qDebug("open");
        emit changeEditModeUp("open");
    }else{
        edit_lowlng->setReadOnly(false);
        edit_lowlat->setReadOnly(false);
        edit_highlng->setReadOnly(false);
        edit_highlat->setReadOnly(false);

        edit_lowlng->clear();
        edit_lowlat->clear();
        edit_highlat->clear();
        edit_highlng->clear();

        editMode = false;
        qDebug("close");
        emit changeEditModeUp("close");
    }
}

