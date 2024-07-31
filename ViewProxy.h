#ifndef VIEWPROXY_H
#define VIEWPROXY_H

#include <QObject>
#include <QTimer>
#include <QAbstractItemModel>

class ViewProxy : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool staleData READ getStaleData WRITE setStaleData RESET resetStaleData NOTIFY staleDataChanged FINAL)
    Q_PROPERTY(bool requestInProgress READ getRequestInProgress WRITE setRequestInProgress NOTIFY requestInProgressChanged FINAL)
    Q_PROPERTY(QString theError READ getTheError WRITE setTheError RESET resetTheError NOTIFY theErrorChanged FINAL)
    Q_PROPERTY(bool control READ getControl WRITE setControl NOTIFY controlChanged FINAL)

public:
    explicit ViewProxy(QObject *parent = nullptr);
    bool getRequestInProgress() const;
    QString getTheError() const;
    void resetTheError();
    bool getStaleData() const;
    void setStaleData(bool newStaleData);
    void resetStaleData();
    bool getControl() const;
    void setControl(bool newControl);

signals:
    void requestInProgressChanged();
    void theErrorChanged();
    void staleDataChanged();
    void controlChanged();
    void enableWorker(bool b);

public slots:
    void setRequestInProgress(bool newRequestInProgress);
    void setTheError(const QString &newTheError);
    void fetchData();

private:
    bool requestInProgress = false;
    bool staleData = true;
    bool control = false;
    QTimer *staleDataTimer;
    QString theError = ""; // The name 'error' is ambiguous, which is why 'theError' is used.
};

#endif // VIEWPROXY_H
