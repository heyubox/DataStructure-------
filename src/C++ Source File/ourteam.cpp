#include "ourteam.h"
#include <QMediaPlayer>
#include <QDir>
#include <QLabel>

OurTeam::OurTeam(QWidget *parent)
    : QDialog(parent)
{
    this->setAttribute(Qt::WA_DeleteOnClose, true);
    QDir dir(QDir::currentPath());
    this->resize(641,481);

    QLabel *content = new QLabel(this);
    content->setPixmap(QPixmap(":/copyright.png"));

    player = new QMediaPlayer(this);
    player->setMedia(QUrl(dir.absoluteFilePath("bgm.mp3")));
    player->play();
}

OurTeam::~OurTeam()
{
}

void OurTeam::setBackInf(QWidget *backInf){
    back = backInf;
}

void OurTeam::closeEvent(QCloseEvent *event){
    Q_UNUSED(event);
    player->stop();
    back->show();
}
