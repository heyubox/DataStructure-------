#ifndef FSTFINF_H
#define FSTFINF_H

#include <QMainWindow>
#include <QEvent>

class QWebEngineView;
class QWebEnginePage;
class JsContext;
class QWebChannel;
class QDir;
class DockBar;


class FstFInf : public QMainWindow
{
    Q_OBJECT
public:
    explicit FstFInf(QWidget *parent = nullptr);
    ~FstFInf();

    void setBackInf(QWidget *);

protected:
    void closeEvent(QCloseEvent *) override;

private:
    void Init();

signals:
    void msgToBridge(const QString &, int, QWebEnginePage *);
    void Jsloaded();
    void locusShowed();

private slots:
    void msgFromBridge(const QString &);
    void showLocus(const QString &url);

private:
    QWebEngineView *m_webView;
    JsContext *m_jsContext;
    DockBar *m_platform;
    QWebChannel *m_webChannel;
    QDir *dir;
    QWidget *back;
};

#endif // FSTFINF_H
