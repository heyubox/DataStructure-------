#ifndef OURTEAM_H
#define OURTEAM_H

#include <QDialog>

class QMediaPlayer;

class OurTeam : public QDialog
{
    Q_OBJECT
public:
    explicit OurTeam(QWidget *parent = nullptr);
    ~OurTeam();
    void setBackInf(QWidget *);

protected:
    void closeEvent(QCloseEvent *) override;

public:
    QMediaPlayer *player;
    QWidget *back;

};

#endif // OURTEAM_H
