/*
 * This class serves as a proxy between the user interface and the backend.
 * It collects data from the NetworkWorker and exposes it to the UI using QProperties.
 * Additionally, it enables interaction with the NetworkWorker through signals and
 * reports any errors that occur in the NetworkWorker.
 */

#include "ViewProxy.h"
#include  <QDebug>

ViewProxy::ViewProxy(QObject *parent)
    : QObject{parent}
{
    // Create and configure staleDataTimer. This timer checks if a signal has been received from the NetworkWorker.
    // If no signal is received within the specified time, it sets the property `bool staleData` to true,
    // which notifies the view.
    staleDataTimer = new QTimer(this);
    staleDataTimer->setInterval(3500); // NetworkWorker requests data every 3 seconds. Mark data as stale if 3.5 seconds have passed.
    connect(staleDataTimer, &QTimer::timeout, this, [this]() {setStaleData(true);});  // On timeout, mark the data as stale.
    connect(this, &ViewProxy::controlChanged, this, [this]() {enableWorker(!control);}); // Disable the worker; the property controlling this is bound to a button in the UI.
}

bool ViewProxy::getRequestInProgress() const
{
    return requestInProgress;
}

void ViewProxy::setRequestInProgress(bool newRequestInProgress)
{
    if (requestInProgress == newRequestInProgress)
        return;
    requestInProgress = newRequestInProgress;
    emit requestInProgressChanged();
}

QString ViewProxy::getTheError() const
{
    return theError;
}

void ViewProxy::setTheError(const QString &newTheError)
{
    if (theError == newTheError)
        return;
    theError = newTheError;
    emit theErrorChanged();
}

void ViewProxy::resetTheError()
{
    setTheError("");
}

void ViewProxy::fetchData()
{
    // Check if a signal from the worker was emitted.
    // If so, reset the timer and clear any previously reported stale data.
    staleDataTimer->start(); // restart timer
    resetStaleData();
}

bool ViewProxy::getControl() const
{
    return control;
}

void ViewProxy::setControl(bool newControl)
{
    if (control == newControl)
        return;
    control = newControl;
    emit controlChanged();
}

bool ViewProxy::getStaleData() const
{
    return staleData;
}

void ViewProxy::setStaleData(bool newStaleData)
{
    if (staleData == newStaleData)
        return;
    staleData = newStaleData;
    emit staleDataChanged();
}

void ViewProxy::resetStaleData()
{
    setStaleData(false);
}

