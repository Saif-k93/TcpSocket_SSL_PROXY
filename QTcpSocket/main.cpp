

#include <QCoreApplication>
#include "client.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Client client;

    std::string host;quint16 port;

    std::cout << "Enter Host: ";
    std::cin >> host;
    std::cout << "Enter port: ";
    std::cin >> port;

    system("cls");

    client.connectToHost(QString::fromStdString(host),port); // u have to use 443 port for ssl



    return a.exec();
}
