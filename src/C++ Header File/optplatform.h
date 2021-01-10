/*workshop of func2*/

#ifndef OPTPLATFORM_H
#define OPTPLATFORM_H

#include <QWidget>
class QLineEdit;
class QSpinBox;
class QPushButton;
class QGroupBox;
class QLabel;

class OptPlatform : public QWidget
{
    Q_OBJECT

public:
    OptPlatform(QWidget *parent = nullptr);
    ~OptPlatform();

private:
    void initUI();
    QGroupBox *llowgroup();
    QGroupBox *lhighgroup();
    QGroupBox *tbeggroup();
    QGroupBox *tendroup();

signals:
    void amountUp(double &, double&, double&, double&, int&, int&);
    void changeEditModeUp(const QString &);

public slots:
    void amountDone(int &);
    void editDone(const QString &);
    void enableButton();


private slots:
    void on_bt_apply_clicked();
    void on_bt_sw_clicked();

public:
    QLineEdit *edit_lowlng, *edit_lowlat, *edit_highlng, *edit_highlat, *result;
    QSpinBox *bdbox, *bhbox, *bmbox, *bsbox, *edbox, *ehbox, *embox, *esbox;
    QPushButton *bt_sw, *bt_apply;
    QLabel *hints;

private:
    bool editMode;

};
#endif // OPTPLATFORM_H
