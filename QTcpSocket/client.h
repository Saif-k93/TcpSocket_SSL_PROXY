#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QDebug>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QMetaEnum>
#include <QNetworkProxy>
#include <QSslSocket>
#include <QSslPreSharedKeyAuthenticator>


class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);

signals:

public:
    void connectToHost(QString host,quint16 port);
    void disconnect();

private slots:
    void connected();
    void disconnected();
    void errorOccurred(QAbstractSocket::SocketError socketError);
    void stateChanged(QAbstractSocket::SocketState socketState);
    void readyread();

    // SSL SLOTS FUNCTIONS
    void encrypted();
    void encryptedBytesWritten(qint64 written);
    void modeChanged(QSslSocket::SslMode mode);
    void peerVerifyError(const QSslError &error);
    void preSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator *authenticator);
    void sslErrors(const QList<QSslError> &errors);


private:
    // QTcpSocket socket; // if we not goin to use ssl

    QSslSocket socket;

};

#endif // CLIENT_H
