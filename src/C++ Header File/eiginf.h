#ifndef EIGINF_H
#define EIGINF_H

#include <QMainWindow>

class JsContext;
class QWebChannel;
class QWebEngineView;
class QWebEnginePage;
class QDir;
class EasyPlt;


class EigInf : public QMainWindow
{
    Q_OBJECT
public:
    explicit EigInf(QWidget *parent = nullptr);
    ~EigInf();
    void setBackInf(QWidget *);

protected:
    void closeEvent(QCloseEvent *) override;

private:
    void init();


signals:
    void msgToBridge(const QString &, int, QWebEnginePage *);
    void kPathUp(double &, double &, double &, double &, double &, double &, double &, double &);

public slots:
    void msgFromBridge(const QString &);
    void on_aply_clicked();
    void kPathDone(int);
    void on_radiobt_clicked();

private:
    JsContext *m_jsContext;
    QWebChannel *m_webChannel;
    QWebEngineView *m_webView;
    QDir *dir;
    QWidget *back;
    EasyPlt *m_platform;

};

#endif // EIGINF_H
