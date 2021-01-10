/* workshop of function 1 show locus*/

#ifndef DOCKBAR_H
#define DOCKBAR_H

#include <QWidget>

class QLineEdit;
class QPushButton;
class QLabel;

class DockBar : public QWidget
{
    Q_OBJECT

public:
    DockBar(QWidget *parent = nullptr);
    ~DockBar();

private:
   void initUI();

signals:
    void showLocusUp(const QString &);

public slots:
    void enablebtShow();

private slots:
    void on_btShow_clicked();

private:
    QPushButton *btShow;
    QLineEdit *edit;
    QLabel *hints;
};
#endif // DOCKBAR_H
