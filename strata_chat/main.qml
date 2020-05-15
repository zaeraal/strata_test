import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.13

Window {
    id: chatWindow
    visible: true
    width: 640
    height: 480
    title: qsTr("Strata Chat Application")

    Rectangle {
        id: userOneChatContainer
        height: parent.height * 0.5
        width: parent.width
        color: "#ffffff"
        border.width: 2
        border.color: "#000000"
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.right: parent.right
        anchors.leftMargin: 0
        anchors.left: parent.left
        anchors.rightMargin: 0

        Text {
            id: chatWindowOneTitle
            width: parent.width
            height: 45
            color: "#ff0000"
            text: qsTr("User 1")
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            padding: 10
            font.family: "Helvetica"
            font.pointSize: 18
        }

        TextField {
            id: userOneSendMessageInput
            anchors.top: chatWindowOneTitle.bottom
            anchors.topMargin: 4
            width: parent.width *0.8
            height: 30
            anchors.left: parent.left
            anchors.leftMargin: parent.border.width
            placeholderText: qsTr("Enter Message ...")
            onAccepted: {
                if (userOneSendMessageInput.text.length != 0) {
                    console.log("Sending: ", userOneSendMessageInput.text);
                    userOneMessageView.text += "<b><font color=\"red\">" + chatWindowOneTitle.text + ": </b></font><font color=\"black\">" + userOneSendMessageInput.text + "</font></n>";
                    userTwoMessageView.text += "<b><font color=\"red\">" + chatWindowOneTitle.text + ": </b></font><font color=\"black\">" + userOneSendMessageInput.text + "</font></n>";
                    userOneSendMessageInput.clear();
                } else {
                    userOneSendMessageInput.clear();
                }
            }
        }

        Button {
            id: userOneSendMessageButton
            anchors.top: chatWindowOneTitle.bottom
            anchors.topMargin: 4
            anchors.left: userOneSendMessageInput.right
            anchors.leftMargin: 2
            width: parent.width *0.1
            height: 30
            text: qsTr("Send")
            onClicked: {
                if (userOneSendMessageInput.text.length != 0) {
                    console.log("Sending: ", userOneSendMessageInput.text);
                    userOneMessageView.text += "<b><font color=\"red\">" + chatWindowOneTitle.text + ": </b></font><font color=\"black\">" + userOneSendMessageInput.text + "</font></n>";
                    userTwoMessageView.text += "<b><font color=\"red\">" + chatWindowOneTitle.text + ": </b></font><font color=\"black\">" + userOneSendMessageInput.text + "</font></n>";
                    userOneSendMessageInput.clear();
                } else {
                    userOneSendMessageInput.clear();
                }
                chat_server.registerClient();
            }
        }

        MessageHandler {
            id: userOneObject
        }

        ScrollView {
            id: userOneMessageScrollView
            anchors.top: userOneSendMessageInput.bottom
            anchors.topMargin: 4
            anchors.right: parent.right
            anchors.rightMargin: parent.width *0.1
            anchors.left: parent.left
            anchors.leftMargin: parent.border.width
            anchors.bottom: parent.bottom
            anchors.bottomMargin: parent.border.width
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

            TextArea {
                id: userOneMessageView
                text: ""
                enabled: false
                textFormat: Text.AutoText
                wrapMode: Text.WordWrap
                placeholderText: qsTr("")
            }
        }
    }

    Rectangle {
        id: userTwoChatContainer
        color: "#ffffff"
        border.width: 2
        border.color: "#000000"
        height: parent.height * 0.5
        width: parent.width
        anchors.top: userOneChatContainer.bottom
        anchors.topMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.rightMargin: 0
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.left: parent.left

        Text {
            id: chatWindowTwoTitle
            width: parent.width
            height: 45
            color: "#0000ff"
            text: qsTr("User 2")
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            padding: 10
            font.family: "Helvetica"
            font.pointSize: 18
        }

        TextField {
            id: userTwoSendMessageInput
            anchors.top: chatWindowTwoTitle.bottom
            anchors.topMargin: 4
            width: parent.width *0.8
            height: 30
            anchors.left: parent.left
            anchors.leftMargin: parent.border.width
            placeholderText: qsTr("Enter Message ...")
            onAccepted: {
                if (userTwoSendMessageInput.text.length != 0) {
                    console.log("Sending: ", userTwoSendMessageInput.text);
                    userOneMessageView.text += "<b><font color=\"blue\">" + chatWindowTwoTitle.text + ": </b></font><font color=\"black\">" + userTwoSendMessageInput.text + "</font></n>";
                    userTwoMessageView.text += "<b><font color=\"blue\">" + chatWindowTwoTitle.text + ": </b></font><font color=\"black\">" + userTwoSendMessageInput.text + "</font></n>";
                    userTwoSendMessageInput.clear();
                } else {
                    userTwoSendMessageInput.clear();
                }
            }
        }

        Button {
            id: userTwoSendMessageButton
            anchors.top: chatWindowTwoTitle.bottom
            anchors.topMargin: 4
            anchors.left: userTwoSendMessageInput.right
            anchors.leftMargin: 2
            width: parent.width *0.1
            height: 30
            text: qsTr("Send")
            onClicked: {
                if (userTwoSendMessageInput.text.length != 0) {
                    console.log("Sending: ", userTwoSendMessageInput.text);
                    userOneMessageView.text += "<b><font color=\"blue\">" + chatWindowTwoTitle.text + ": </b></font><font color=\"black\">" + userTwoSendMessageInput.text + "</font></n>";
                    userTwoMessageView.text += "<b><font color=\"blue\">" + chatWindowTwoTitle.text + ": </b></font><font color=\"black\">" + userTwoSendMessageInput.text + "</font></n>";
                    userTwoSendMessageInput.clear();
                } else {
                    userTwoSendMessageInput.clear();
                }
            }
        }

        MessageHandler {
            id: userTwoObject
        }

        ScrollView {
            id: userTwoMessageScrollView
            anchors.top: userTwoSendMessageInput.bottom
            anchors.topMargin: 4
            anchors.right: parent.right
            anchors.rightMargin: parent.width *0.1
            anchors.left: parent.left
            anchors.leftMargin: parent.border.width
            anchors.bottom: parent.bottom
            anchors.bottomMargin: parent.border.width
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

            TextArea {
                id: userTwoMessageView
                text: ""
                enabled: false
                textFormat: Text.AutoText
                wrapMode: Text.WordWrap
                placeholderText: qsTr("")
            }
        }

    }

}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
