

#include "client.h"

Client::Client(QObject *parent)
    : QObject{parent}
{
    QObject::connect(&socket,&QTcpSocket::connected,this,&Client::connected);
    QObject::connect(&socket,&QTcpSocket::disconnected,this,&Client::disconnected);
    QObject::connect(&socket,&QTcpSocket::readyRead,this,&Client::readyread);
    QObject::connect(&socket,&QTcpSocket::stateChanged,this,&Client::stateChanged);
    QObject::connect(&socket,&QTcpSocket::errorOccurred,this,&Client::errorOccurred);

    // SSL connect
    QObject::connect(&socket,&QSslSocket::encrypted,this,&Client::encrypted);
    QObject::connect(&socket,&QSslSocket::encryptedBytesWritten,this,&Client::encryptedBytesWritten);
    QObject::connect(&socket,&QSslSocket::modeChanged,this,&Client::modeChanged);
    QObject::connect(&socket,&QSslSocket::peerVerifyError,this,&Client::peerVerifyError);
    QObject::connect(&socket,&QSslSocket::preSharedKeyAuthenticationRequired,this,&Client::preSharedKeyAuthenticationRequired);
    QObject::connect(&socket,QOverload<const QList<QSslError>&>::of(&QSslSocket::sslErrors),this,&Client::sslErrors);


    // proxy
    // QNetworkProxy proxy(QNetworkProxy::HttpProxy,"20.111.54.16",8123); // creatring proxy, change proxy its old

    // proxy.setUser("user");  // use if needed
    // proxy.setPassword("pass"); // use if needed

    // QNetworkProxy::setApplicationProxy(proxy); // use to make the intire app uses proxy

    // socket.setProxy(proxy);  // use to make proxy per socket

}

void Client::connectToHost(QString host, quint16 port)
{
    if(socket.isOpen())disconnect();

    qInfo() << "Connecting To :" << host << "On Port :" << port;

    // socket.connectToHost(host,port); // use if no ssl used

    // socket.ignoreSslErrors();  // do not use in real world it will block errors

    socket.setProtocol(QSsl::SecureProtocols);

    socket.connectToHostEncrypted(host,port); // use when using ssl

}

void Client::disconnect()
{
    socket.close();
    socket.waitForDisconnected();
}

void Client::connected()
{
    qInfo() << "\n\t<<<<<<< Connected >>>>>>>\n";
    qInfo() << "Sending:";

    // this is the request we send to the website to get response back
    QByteArray data =
        {
            // we have to add new line for each req
            "GET / HTTP/1.1\n"
            "Host: www.google.com\n"  // change google.com to whatever the host is
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:109.0) Gecko/20100101 Firefox/114.0\n"
            "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8\n"
            "Accept-Language: en-US,en;q=0.5\n"
            "Accept-Encoding: gzip, deflate\n"
            "Connection: Close\n"  // use Keep-Alive if u dont want to disconnect from server
            "Upgrade-Insecure-Requests: 1\n\n"       // we have to add double new line at the end
        };

    socket.write(data);  // write data to server

    // it wont exit the func until all data is written
    socket.waitForBytesWritten(); // optional
}

void Client::disconnected()
{
    qInfo() << "\n\n\t <<<<<< Disconnected >>>>>>";
}

void Client::errorOccurred(QAbstractSocket::SocketError socketError)
{
    qInfo() << "Error :" << socketError << " " << socket.errorString();
}

void Client::stateChanged(QAbstractSocket::SocketState socketState)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<QAbstractSocket::SocketState>();

    qInfo() << "State:" << metaEnum.valueToKey(socketState);

}

void Client::readyread()
{
    qInfo() << "Data From:" << sender() << " bytes:" << socket.bytesAvailable();

    qInfo() << "Data:" << socket.readAll();

}

void Client::encrypted()
{
    qInfo() << "Encrypted";
}

void Client::encryptedBytesWritten(qint64 written)
{
    qInfo() << "Encrypted Bytes Written:" << written;
}

void Client::modeChanged(QSslSocket::SslMode mode)
{
    qInfo() << "Ssl Mode:" << mode;
}

void Client::peerVerifyError(const QSslError &error)
{
    qInfo() << "peerVerifyError:" << error.errorString();
}

void Client::preSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator *authenticator)
{
    qInfo() << "preSharedKeyAuthenticationRequired:";
}

void Client::sslErrors(const QList<QSslError> &errors)
{
    qInfo() << "Ssl Errors:>";

    foreach (QSslError e, errors)
    {
        qInfo() << e.errorString();
    }
}


