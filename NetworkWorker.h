#ifndef NETWORKWORKER_H
#define NETWORKWORKER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTimer>
#include <QJsonDocument>

class NetworkWorker : public QObject
{
    Q_OBJECT

public:
    explicit NetworkWorker(){};

signals:
    void requestInProgress(bool b);
    void newData(QJsonDocument json_doc);
    void error(QString error);

public slots:
    void start();
    void quit();
    void workerEnable(bool b);

private slots:
    void onFinished(QNetworkReply *reply);

private:
    void makeRequest();
    QNetworkAccessManager *networkManager;
    QTimer *timer;
};

#endif // NETWORKWORKER_H
