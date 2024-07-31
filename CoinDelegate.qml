import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item{
    width: listView.width;
    //height: 100
    height: listView.width * 0.3  // Make delegates resizable along with the view.

    Rectangle {
        anchors.fill: parent
        anchors.margins: 2
        border.width: 2

        id: cryptoDelegate
        clip: true // Ensure nothing renders outside of the view.

        // Cut the left corner of the parent rectangle.
        Rectangle{
            id: leftCorner
            x: -width/2; y: parent.height - height/2
            height: parent.height * 0.5
            width: height
            z:2
            border.width: 2
            border.color: "black"
            transform: Rotation{
                origin.x: leftCorner.width /2 // Rotate around the center of the rectangle
                origin.y: leftCorner.height/2 // Rotate around the center of the rectangle
                angle: 45
            }
        }

        // Cut the right corner of the parent rectangle.
        Rectangle{
            id: rightCorner
            x: parent.width - width/2; y:- height/2
            height: parent.height * 0.5
            width: height
            border.width: 2
            border.color: "black"
            transform: Rotation{
                origin.x: rightCorner.width  /2 // Rotate around the center of the rectangle
                origin.y: rightCorner.height /2 // Rotate around the center of the rectangle
                angle: 45
            }
        }

        // Delegate bindings to the model data using role names.
        RowLayout{
            anchors.fill: parent
            Item{
                Layout.fillHeight: true
                Layout.preferredWidth: parent.width * 2/7
                Image {
                    id: bitcoinImage
                    source: image
                    anchors.fill: parent
                    anchors.margins: 15
                    fillMode: Image.PreserveAspectFit
                }
            }

            ColumnLayout{
                Layout.fillHeight: true
                Layout.fillWidth: true
                spacing: 0

                Label {
                    text: "<b>" + name + " " + symbol + "</b>"
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    minimumPixelSize: 5
                    font.pixelSize: 100
                    fontSizeMode: Text.Fit
                    Layout.topMargin: 5
                }

                Label {
                    text: "Current price: " + current_price + " USD"
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    minimumPixelSize: 5
                    font.pixelSize: 100
                    fontSizeMode: Text.Fit
                }

                Label {
                    text: "<font color=\"#000000\">24h: </font>" + price_change_percentage_24h + "%" + "</font>"
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    minimumPixelSize: 5
                    font.pixelSize: 100
                    fontSizeMode: Text.Fit
                    color: {
                        let priceChange = Number(price_change_percentage_24h)
                        if(priceChange > 0) return "green"
                        else if(priceChange < 0) return "red"
                        else return "black"
                    }
                }

                Label {
                    text: "Market cap: " + market_cap
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    minimumPixelSize: 5
                    font.pixelSize: 100
                    fontSizeMode: Text.Fit
                }

                Label {
                    text: "Last updated: " +last_updated
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    minimumPixelSize: 5
                    font.pixelSize: 100
                    fontSizeMode: Text.Fit
                    Layout.bottomMargin: 5
                }
            }
        }

        // Connects to the model's dataChanged signal and highlights an item if the data changes.
        Connections{
            target: CryptoModel
            function onDataChanged(topLeft) {
                if(topLeft.row === index){
                    highlightTimer.start()
                    cryptoDelegate.color = "lightblue"
                }
            }
        }

        // A timer that resets the highlight, triggered by the onDataChanged(topLeft) function.
        Timer {
            id: highlightTimer
            interval: 500
            onTriggered: {
                cryptoDelegate.color = "white"
            }
        }
    }
}
