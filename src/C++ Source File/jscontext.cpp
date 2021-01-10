#include "jscontext.h"
#include <QWebChannel>
#include <QWebEngineView>


//public:
JsContext::JsContext(QObject *parent) : QObject(parent)
{
}


//private:
void JsContext::sendMsg(QWebEnginePage *page, const QString &msg, const QString &cod)
{
    page->runJavaScript(QString("recvMessage('%1','%2');").arg(msg).arg(cod));
}




//public slots:
void JsContext::innerMsgHandler(const QString &msg, int code, QWebEnginePage *page){
    switch(code){
    case F1_SHOWLOCUS: sendMsg(page, msg, "f1sl"); break;
    case F3_SHOWLOCUS: sendMsg(page, msg, "f3sl"); break;
    case F3_CHANGEEDITMODE:sendMsg(page, msg, "f3ced"); break;
    case F5_CHANGEEDITMODE:sendMsg(page, msg, "f5ced");break;
    case F4_SHOWGRID:sendMsg(page, msg, "f4sg"); break;
    case F8_SHOWPATH:sendMsg(page, msg, "f8sp");break;
    case F8_SW:sendMsg(page, msg, "f8sw");break;
    default: break;
    }
}

void JsContext::onMsg(const QString &msg)
{
    emit bcastMsg(msg);
}




