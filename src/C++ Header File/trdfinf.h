#ifndef TrdFInf_H
#define TrdFInf_H

#include <QMainWindow>
#include <QEvent>

class OptPlatform;
class JsContext;
class QWebChannel;
class QWebEngineView;
class QWebEnginePage;
class QDir;



class TrdFInf : public QMainWindow
{
    Q_OBJECT
public:
    explicit TrdFInf(QWidget *parent = nullptr);
    ~TrdFInf();
    void setBackInf(QWidget *);

protected:
    void closeEvent(QCloseEvent *) override;


private:
    void init();

signals:
    void amountUp(double &, double&, double&, double&, int&, int&);
    void amountDoneDown(int &);
    void msgToBridge(const QString &, int, QWebEnginePage *);
    void Jsloaded();
    void editDoneDown(const QString &msg);

public slots:
    void amountDone(int &);

    void changeEditMode(const QString &);

    void msgFromBridge(const QString &);


private slots:
    void amount(double &, double&, double&, double&, int&, int&);




private:
    JsContext *m_jsContext;
    OptPlatform *m_platform;
    QWebChannel *m_webChannel;
    QWebEngineView *m_webView;
    QDir *dir;
    QWidget *back;
};

#endif // TrdFInf_H
