/*
 * This class fetches data from CoinGecko every 3 seconds, parses it into a JSON document,
 * and emits it via a signal. It also reports JSON parsing errors and QNetworkReply errors
 * using signals. The network worker is designed to run in a separate thread; therefore,
 * the constructor does not take a parent argument.
 *
 * The worker thread is started, stopped, and destroyed from outside the thread using
 * the public slots `start()` and `quit()`.
 */

#include "NetworkWorker.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QThread>
// Constant request generator
void NetworkWorker::makeRequest()
{
    QNetworkRequest request(QUrl("https://api.coingecko.com/api/v3/coins/markets?vs_currency=usd"));
//  Uncomment below to trigger error
//    request.setUrl(QUrl("https://api.coingecko.com/api/v3/coins/markets?vs_currency=usderror"));
    request.setRawHeader("accept", "application/json");
    request.setRawHeader("x-cg-demo-api-key", "CG-VTPKLw7jLuRgMqyVCjW3V7GC");
    networkManager->get(request);
    emit requestInProgress(true); // Show the update indicator in the UI.
}

// Slot to start a worker
void NetworkWorker::start()
{
    networkManager = new QNetworkAccessManager(this);
    timer = new QTimer(this);
    makeRequest(); // Initial request upon the first connection.
    connect(timer, &QTimer::timeout, this, [this](){makeRequest();}); // Lambda function is connected to the QTimer::timeout() signal.
    timer->setInterval(3000); // Request every 3s
    timer->start();
    connect(networkManager, &QNetworkAccessManager::finished, this, &NetworkWorker::onFinished);
}

// Deletes itself and all its children upon quitting.
void NetworkWorker::quit()
{
    this->deleteLater();
}

// This slot stops network requests to simulate stale data, as requested in the recruitment task.
void NetworkWorker::workerEnable(bool b)
{
    if(b)
        timer->start();
    else
        timer->stop();
}

void NetworkWorker::onFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {

        // Read all the data from the reply
        QByteArray response_data = reply->readAll();
        QJsonParseError parse_error;
        // Parse the JSON
        QJsonDocument json_doc = QJsonDocument::fromJson(response_data, &parse_error);

        // Check for parsing errors
        if (parse_error.error == QJsonParseError::NoError && json_doc.isArray()) {
            emit newData(json_doc);
        } else {
            // Report JSON parsing error
            emit error(parse_error.errorString());
        }
    }else{
        // Report QNetworkReply Error
        emit error(reply->errorString());
    }
    emit requestInProgress(false); // Hide the update indicator in the UI.
    timer->start();
    reply->deleteLater();
}
