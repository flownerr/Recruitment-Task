#ifndef CRYPTOMODEL_H
#define CRYPTOMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QTimer>
#include <QJsonDocument>

struct Coin
{
    QString image;
    QString name;
    QString symbol; //ticker
    QString last_updated;
    double price;
    double price_change24;
    double market_cap;
};

class CryptoModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    CryptoModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    QHash<int,QByteArray> roleNames() const override;

public slots:
    void fetchData(QJsonDocument json_doc);

private:
    QVector<Coin> coins;
    bool fillModel(int rowsNumber);

    enum CryptoRoles {
        MarketCapRole  = Qt::UserRole,
        NameRole,
        PriceChange24Role,
        CurrentPriceRole,
        ImageRole,
        SymbolRole,
        LastUpdatedRole
    };
};

#endif
