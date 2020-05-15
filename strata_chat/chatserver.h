#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QObject>
#include <QDebug>

struct client {
    int ii;
};

class ChatServer : public QObject
{
    Q_OBJECT
public:
    explicit ChatServer(QObject *parent = nullptr);

    Q_INVOKABLE void registerClient();
    Q_INVOKABLE void sendMessage();

signals:
    void update(struct client, std::string from, std::string to);

private:
    std::map<std::string, struct client> m_clients;
};

#endif // CHATSERVER_H
