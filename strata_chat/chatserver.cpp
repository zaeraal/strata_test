#include "chatserver.h"

using namespace std;

ChatServer::ChatServer(QObject *parent) : QObject(parent)
{

}

void ChatServer::registerClient(QString src_username, QObject *src_object)
{
    qDebug() << "called registerClient with src_username: " << src_username << ", src_object: " << src_object;

    if(src_username == "") {
        qDebug() << "called registerClient with invalid username";
        return;
    }

    if(src_object == nullptr) {
        qDebug() << "called registerClient with NULL ptr";
        return;
    }

    auto it = m_clients.find(src_username);
    if(it == m_clients.end()) {
        m_clients.insert(make_pair(src_username, sClientData{src_object, src_username, ""}));
    } else {
        it->second.m_object = src_object;
    }
}

void ChatServer::unRegisterClient(QString src_username)
{
    qDebug() << "called unRegisterClient with src_username: " << src_username;

    if(src_username == "") {
        qDebug() << "called unRegisterClient with invalid username";
        return;
    }

    auto it = m_clients.find(src_username);
    if(it != m_clients.end()) {
        if(it->second.m_object != nullptr) {
            disconnect(it->second.m_object, NULL, this, NULL);
            it->second.m_object = nullptr;
        }
    }
}

void ChatServer::sendMessage(QString src_username, QString dst_username, QString message)
{
    qDebug() << "called sendMessage with src_username: " << src_username << "dst_username: " << dst_username << "message: " << message;

    if(src_username == "") {
        qDebug() << "called sendMessage with invalid src_username";
        return;
    }

    if(dst_username == "") {
        qDebug() << "called sendMessage with invalid dst_username";
        return;
    }

    if(message == "") {
        qDebug() << "called sendMessage with invalid message";
        return;
    }

    auto it_src = m_clients.find(src_username);
    if(it_src == m_clients.end()) {
        qDebug() << "called sendMessage with never registered sender";
        return;
    }

    if(it_src->second.m_object == nullptr) {
        qDebug() << "called sendMessage with unregistered sender";
        return;
    }

    auto it_dst = m_clients.find(dst_username);
    if(it_dst == m_clients.end()) {
        qDebug() << "called sendMessage with never registered receiver";
        return;
    }

    if(it_dst->second.m_object == nullptr) {
        qDebug() << "called sendMessage with unregistered receiver";
        return;
    }

    it_src->second.m_messages += message;
    it_dst->second.m_messages += message;

    QMetaObject::invokeMethod(it_src->second.m_object,                      // pointer to a QObject
                              "updateClient",                               // member name (no parameters here)
                              Qt::QueuedConnection,                         // connection type
                              Q_ARG(QString, it_src->second.m_messages));   // parameters

    QMetaObject::invokeMethod(it_dst->second.m_object,                      // pointer to a QObject
                              "updateClient",                               // member name (no parameters here)
                              Qt::QueuedConnection,                         // connection type
                              Q_ARG(QString, it_dst->second.m_messages));   // parameters

}
