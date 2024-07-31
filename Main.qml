import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Controls.Basic

Window {
    id: root
    width: 330
    height: 600
    visible: true

    // Simple popup to display an error message reported from ViewProxy
    ErrorPopup {}

    ColumnLayout {
        anchors.fill: parent

        // Top of the list.
        RowLayout {
            Layout.preferredHeight: 30
            Layout.fillWidth: true

            Label{
                text: "Sort by:"
            }

            // ComboBox for sorting by role.
            ComboBox {
                id: sortRoleCombo
                Layout.preferredHeight: parent.height
                currentIndex: 0
                model: ["Ranking", "Name", "24h Change", "Price"]
                onCurrentIndexChanged: {
                    ProxyModel.sortRole = Qt.UserRole + currentIndex
                    ProxyModel.sort(0, sortOrderButton.checked ? Qt.AscendingOrder : Qt.DescendingOrder)
                }
            }

            // Customized button that maintains a constant appearance even when checked.
            Button {
                id: sortOrderButton
                Layout.preferredWidth: parent.height
                Layout.preferredHeight: parent.height
                checkable: true
                background: Rectangle {
                    color: "lightgray"
                }
                contentItem: Image {
                    source: sortOrderButton.checked ? "qrc:/Resources/sort_asc.png" : "qrc:/Resources/sort_desc.png"
                    fillMode: Image.PreserveAspectFit
                    anchors.margins: 7
                    anchors.fill: parent
                }
                onCheckedChanged: ProxyModel.sort(0, checked ? Qt.AscendingOrder : Qt.DescendingOrder)
            }

            // Control button at the top of the list to trigger random data changes in the model.
            // When checked, it disables the NetworkWorker, marking the data as stale,
            // and sets random data for two random coins visible in the ListView at that time.
            Button {
                id: controlButton
                Layout.preferredHeight: parent.height
                Layout.fillWidth: true
                checkable: true

                //'control' property enables or disables the NetworkWorker.
                Binding {
                    target: ViewProxy
                    property: "control"
                    value: controlButton.checked
                }

                text: "Control"
                onCheckedChanged: {
                    if (!checked) return;

                    function getRandomInt(min, max) {
                        min = Math.ceil(min);
                        max = Math.floor(max);
                        return Math.floor(Math.random() * (max - min + 1)) + min;
                    }

                    // Get indices for the first and last visible items using listView.indexAt().
                    // Account for the possible view displacement after scrolling.
                    let firstVisibleItemIndex = listView.indexAt(1, listView.contentY + listView.delegateHeight / 2);
                    let lastVisibleItemIndex = listView.indexAt(1, listView.height + listView.contentY - listView.delegateHeight / 2);
                    lastVisibleItemIndex = (lastVisibleItemIndex === -1) ? ProxyModel.rowCount() - 1 : lastVisibleItemIndex;

                    // Select two distinct random row indices within the visible range
                    let randomRowNum1 = getRandomInt(firstVisibleItemIndex, lastVisibleItemIndex);
                    let randomRowNum2;

                    // Prevent duplication of random numbers.
                    do {
                        randomRowNum2 = getRandomInt(firstVisibleItemIndex, lastVisibleItemIndex);
                    } while (randomRowNum2 === randomRowNum1 && lastVisibleItemIndex > 0);

                    // Get the corresponding indexes and current prices
                    const firstIndex = ProxyModel.index(randomRowNum1, 0);
                    const secondIndex = ProxyModel.index(randomRowNum2, 0);

                    var firstCoinPrice = ProxyModel.data(firstIndex, 259); // 259 corresponds to CurrentPriceRole. Creating an enum would be overkill for a single function.
                    var secondCoinPrice = ProxyModel.data(secondIndex, 259);

                    // Apply a random percentage change to the coin prices, ranging from -2% to +2%.
                    const firstRandomPercent = Math.random() * 0.04 - 0.02;
                    const secondRandomPercent = Math.random() * 0.04 - 0.02;

                    // Set the generated data.
                    ProxyModel.setData(firstIndex, (firstCoinPrice + (firstCoinPrice * firstRandomPercent)).toFixed(2), 259); // 259 = CurrentPriceRole
                    ProxyModel.setData(secondIndex, (secondCoinPrice + (secondCoinPrice * secondRandomPercent)).toFixed(2), 259);
                }
            }
        }

        ListView {
            id: listView
            Layout.fillWidth: true
            Layout.fillHeight: true
            property int delegateHeight: 20
            clip: true
            model: ProxyModel
            delegate: CoinDelegate {}
        }

        // The bottom of a ListView.
        Row {
            Layout.preferredHeight: 20
            // Stale data indicator
            Image {
                source: "qrc:/Resources/stale_data.png"
                fillMode: Image.PreserveAspectFit
                height: parent.height
                visible: ViewProxy.staleData
            }
            // Update indicator
            Image {
                source: "qrc:/Resources/data_transfer.png"
                fillMode: Image.PreserveAspectFit
                height: parent.height
                visible: ViewProxy.requestInProgress
            }
        }
    }
}
