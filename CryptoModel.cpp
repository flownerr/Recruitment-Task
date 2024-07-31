/*
 * This is a minimal implementation of a subclass of QAbstractTableModel
 *
 * According to Qt documentation, when subclassing QAbstractTableModel, the following
 * methods must be implemented:
 * - rowCount()
 * - columnCount()
 * - data()
 *
 * In addition to these, the functions setData() and roleNames() were implemented to
 * create a functional model subclass. The model receives data in JSON format and
 * converts it into a table structure where each entry is represented by a struct Coin.
 */

#include "CryptoModel.h"
#include <QJsonArray>
#include <QJsonObject>

CryptoModel::CryptoModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int CryptoModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : coins.size();
}

// Treat the table model as a list, so there is only 1 column.
int CryptoModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant CryptoModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid()){
        if (index.row() >= coins.size() || index.row() < 0)
            return QVariant();

        const auto &coin = coins.at(index.row());
        // Access data from QML using role names specified in QHash<int, QByteArray> CryptoModel::roleNames().
        switch (role) {
        case NameRole:
            return coin.name;
        case ImageRole:
            return coin.image;
        case SymbolRole:
            return coin.symbol;
        case MarketCapRole:
            return coin.market_cap;
        case LastUpdatedRole:
            return coin.last_updated;
        case CurrentPriceRole:
            return coin.price;
        case PriceChange24Role:
            return coin.price_change24;
        default:
            return QVariant();
        }
    }
    return QVariant();
}

// Fill the model with the specified row count using default data.
bool CryptoModel::fillModel(int rowsNumber)
{
    beginInsertRows(QModelIndex(), 0, rowsNumber - 1);

    for (int row = 0; row < rowsNumber; ++row)
        coins.append({ "", "", "", "", 0.0, 0.0, 0.0 });

    endInsertRows();
    return true;
}

// Set custom role names.
QHash<int, QByteArray> CryptoModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] =           "name";
    roles[ImageRole] =          "image";
    roles[SymbolRole] =         "symbol";
    roles[MarketCapRole] =      "market_cap";
    roles[LastUpdatedRole] =    "last_updated";
    roles[CurrentPriceRole] =   "current_price";
    roles[PriceChange24Role] =  "price_change_percentage_24h";
    return roles;
}

// Update the model's data.
void CryptoModel::fetchData(QJsonDocument json_doc)
{
    int arraySize = json_doc.array().size();
    // Create data rows if the model is empty (upon first connection).
    if(!rowCount(QModelIndex()))
        fillModel(arraySize);

    QJsonArray json_array = json_doc.array();

    for (int i = 0; i < arraySize; ++i) {
        QJsonValue value = json_array.at(i);
        // Set data for existing items.

        if (value.isObject()) {
            QModelIndex index = this->index(i, 0, QModelIndex());
            QJsonObject obj = value.toObject();
            setData(index, obj["name"].toString(),                        NameRole);
            setData(index, obj["image"].toString(),                       ImageRole);
            setData(index, obj["symbol"].toString(),                      SymbolRole);
            setData(index, obj["market_cap"].toDouble(),                  MarketCapRole);
            setData(index, obj["last_updated"].toString(),                LastUpdatedRole);
            setData(index, obj["current_price"].toDouble(),               CurrentPriceRole);
            setData(index, obj["price_change_percentage_24h"].toDouble(), PriceChange24Role);
        }
    }
}

bool CryptoModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.isValid()){
        const int row = index.row();
        auto coin = coins.value(row);

        switch (role) {
        case NameRole:
            if(coin.name == value) // Do nothing if the data hasn't changed.
                return false;
            coin.name = value.toString(); // Set the new data value.
            break;
        case ImageRole:
            if(coin.image == value)
                return false;
            coin.image = value.toString();
            break;
        case SymbolRole:
            if(coin.symbol == value)
                return false;
            coin.symbol = value.toString();
            break;
        case MarketCapRole:
            if(coin.market_cap == value)
                return false;
            coin.market_cap = value.toDouble();
            break;
        case LastUpdatedRole:
            if(coin.last_updated == value)
                return false;
            coin.last_updated = value.toString();
            break;
        case CurrentPriceRole:
            if(coin.price == value)
                return false;
            coin.price = value.toDouble();
            break;
        case PriceChange24Role:
            if(coin.price_change24 == value)
                return false;
            coin.price_change24 = value.toDouble();
            break;
        default:
            break;
        }
        coins.replace(row, coin);
        emit dataChanged(index, index, {role}); // Emit a signal to inform all recipients that data with the specified role has changed.
        // This signal is also used by a slot to highlight the item in the UI for 0.5 seconds.
        return true;
    }
    return false;
}
