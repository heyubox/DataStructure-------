/* the connection Object between webViewPage and javascript*/

#ifndef JSCONTEXT_H
#define JSCONTEXT_H

#include <QObject>
class QWebEnginePage;

class JsContext : public QObject
{
    Q_OBJECT
public:
    explicit JsContext(QObject *parent = nullptr);
    typedef enum{
        F1_SHOWLOCUS,
        F3_SHOWLOCUS,
        F3_CHANGEEDITMODE,
        F5_CHANGEEDITMODE,
        F4_SHOWGRID,
        F8_SHOWPATH,
        F8_SW
    } CODE;

private:
    void sendMsg(QWebEnginePage *, const QString &, const QString &);    //send msg to js

signals:
    void bcastMsg(const QString &);     //broadcast msg of js

public slots:
    void onMsg(const QString &);        //receive the msg from js
    void innerMsgHandler(const QString &, int, QWebEnginePage *);   //deal the qt order to js
};

#endif // JSCONTEXT_H
