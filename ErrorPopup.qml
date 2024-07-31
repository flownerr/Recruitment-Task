import QtQuick
import QtQuick.Controls

Popup {
    id: root
    width: parent.width -30
    height: parent.height *0.3
    focus: true
    modal: true
    anchors.centerIn: parent

    // The popup opens when the QProperty 'theError' in 'ViewProxy' changes.
    Connections {
        target: ViewProxy
        function onTheErrorChanged(error) {
            if(error !== "")
                root.open();
        }
    }

    Rectangle {
        width: parent.width
        height: parent.height
        color: "lightgray"
        border.color: "gray"

        Column {
            anchors.centerIn: parent
            spacing: 10
            width: parent.width * 0.9 // Adjust width as needed for wrapping

            Text {
                // Text is bound to the QProperty in 'ViewProxy'.
                text: ViewProxy.theError
                wrapMode: Text.Wrap
                width: parent.width // Ensures text wraps within the Column width
            }

            Button {
                // Close this popup when the button is clicked.
                text: "OK"
                onClicked: root.close()
                width: parent.width -10
            }
        }
    }
}
