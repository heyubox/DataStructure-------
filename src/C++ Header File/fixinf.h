#ifndef FIXINF_H
#define FIXINF_H

#include <QMainWindow>
#include <QEvent>

class OSCgraph;
class QWebEngineView;
class QWebEnginePage;
class JsContext;
class QWebChannel;
class QDir;
class OSCgraph;
class OSCplatform;
class OptDock;


class FixInf : public QMainWindow
{
    Q_OBJECT
public:
    explicit FixInf(QWidget *parent = nullptr);
    ~FixInf();

    void setBackInf(QWidget *);



protected:
    void closeEvent(QCloseEvent *) override;



signals:
    void msgToBridge(const QString &, int, QWebEnginePage *);

    void Volume5Up(double &, double &, double &, double &, double &, double &, double &, double &);
    void Volume6Up(double &, double &, double &, double &);


public slots:
    void msgFromBridge(const QString &msg);

    void volumeDone(int *);



private:
    void init();


private slots:
    void editModeC();
    void on_count_clicked();


private:
    QWebEngineView *m_webView;
    JsContext *m_jsContext;
    QWebChannel *m_webChannel;
    QDir *dir;
    QWidget *back, *centralW, *mask, *OptW;
    OSCgraph *m_graph;
    OSCplatform *m_bar;
    OptDock *m_platform;

public:
    int **ary;
    volatile bool data;
};

#endif // FIXINF_H
