/*an image button widget*/


#ifndef LNKBUTTON_H
#define LNKBUTTON_H
#include<QLabel>

class LnkButton : public QLabel
{
    Q_OBJECT
public:
    LnkButton(QString objName, QString fileName, QRect rect, QWidget *parent = nullptr);

protected:
    void enterEvent(QEvent *)override;
    void leaveEvent(QEvent *)override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;

signals:
    void buttonClicked();

private:
    QPoint pos;
};

#endif // LNKBUTTON_H
