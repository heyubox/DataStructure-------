#ifndef FORINF_H
#define FORINF_H

#include <QMainWindow>

class QWebEngineView;
class QWebEnginePage;
class JsContext;
class QWebChannel;
class QDir;
class TimeOpt;

class ForInf : public QMainWindow
{
    Q_OBJECT
public:
    explicit ForInf(QWidget *parent = nullptr);
    ~ForInf();
    void setBackInf(QWidget *);
    void init();

protected:
    void closeEvent(QCloseEvent *) override;

signals:
    void msgToBridge(const QString &, int code, QWebEnginePage *page);

public slots:
    void msgFromBridge(const QString &);
    void msgPack(const QString &);

public:
    QWidget *back;
    QWebEngineView *m_webView;
    JsContext *m_jsContext;
    QWebChannel *m_webChannel;
    QDir *dir;
    TimeOpt *m_platform;

};

#endif // FORINF_H
