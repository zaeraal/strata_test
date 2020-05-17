#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QObject>
#include <QDebug>

struct sClientData {
    QObject* m_object;
    QString  m_username;
    QString  m_messages;
};

class ChatServer : public QObject
{
    Q_OBJECT
public:
    explicit ChatServer(QObject *parent = nullptr);

public slots:
    Q_INVOKABLE void registerClient(QString src_username, QObject* src_object);
    Q_INVOKABLE void unRegisterClient(QString src_username);
    Q_INVOKABLE void sendMessage(QString src_username, QString dst_username, QString message);

signals:
    void updateClient(QString messages);

private:
    std::map<QString, sClientData> m_clients;
};

#endif // CHATSERVER_H
