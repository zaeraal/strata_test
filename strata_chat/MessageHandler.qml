// MessageHandler.qml
import QtQuick 2.12

Item {
    property QtObject message_view;
    property string username;

    signal registerClient(src_username: string, src_object: QtObject);
    signal unRegisterClient(src_username: string);
    signal sendMessage(src_username: string, dst_username: string, message: string);

    function updateClient(messages: string) {
        //console.log("Receiving: ", messages);
        message_view.text = messages;
    }

    function deliverMessage(dst_username: string, message: string) {
        sendMessage(username, dst_username, message);
    }

    Component.onCompleted: {
        registerClient.connect(chat_server.registerClient);
        unRegisterClient.connect(chat_server.unRegisterClient);
        sendMessage.connect(chat_server.sendMessage);

        registerClient(username, this);
    }

    Component.onDestruction: {
        unRegisterClient(username);
    }
}
