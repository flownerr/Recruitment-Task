#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QThread>
#include <QSortFilterProxyModel>
#include "CryptoModel.h"
#include "NetworkWorker.h"
#include "ViewProxy.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    QThread *workerThread             = new QThread();
    ViewProxy *viewProxy              = new ViewProxy(&app);
    CryptoModel* cryptoModel          = new CryptoModel(&app);
    NetworkWorker *networkWorker      = new NetworkWorker();
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(&app);

    networkWorker->moveToThread(workerThread);
    workerThread->start();
    // Start worker in different thread
    QObject::connect(workerThread, &QThread::started, networkWorker, &NetworkWorker::start);
    // Reports errors to ViewProxy.
    QObject::connect(networkWorker, &NetworkWorker::error, viewProxy, &ViewProxy::setTheError);
    // Check for stale data in ViewProxy.
    QObject::connect(networkWorker, &NetworkWorker::newData, viewProxy, &ViewProxy::fetchData);
    // Update the data model.
    QObject::connect(networkWorker, &NetworkWorker::newData, cryptoModel, &CryptoModel::fetchData);
    // Show or hide the data request icon.
    QObject::connect(networkWorker, &NetworkWorker::requestInProgress, viewProxy, &ViewProxy::setRequestInProgress);
    // Enable worker
    QObject::connect(viewProxy, &ViewProxy::enableWorker, networkWorker, &NetworkWorker::workerEnable);
    // Terminate the thread and delete any objects living in it when the main application quits.
    QObject::connect(&app, &QGuiApplication::aboutToQuit, [workerThread, networkWorker](){
        networkWorker->quit();
        workerThread->quit();
        workerThread->wait();
    });

    proxyModel->setSourceModel(cryptoModel);
    // Set default sorting by the 'Ranking' role.
    proxyModel->setSortRole(Qt::UserRole+0);
    // Register classes with QML.
    engine.rootContext()->setContextProperty("ProxyModel",  proxyModel);
    engine.rootContext()->setContextProperty("CryptoModel", cryptoModel);
    engine.rootContext()->setContextProperty("ViewProxy",   viewProxy);

    const QUrl url(u"qrc:/RecruitmentTask/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
