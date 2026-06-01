#include <QCoreApplication>
#include "SocketCommunicatieRPIBUS.h"
#include "CentraalBesturingssysteemRPIBUS.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Maak de netwerkmodule en de logica module aan
    SocketCommunicatieRPIBUS socket;
    CentraalBesturingssysteemRPIBUS systeem;

    // 1. Koppel inkomende netwerkcommando's (vanaf WEMOS) aan het systeem
    QObject::connect(&socket, &SocketCommunicatieRPIBUS::inkomendCommando,
                     &systeem, &CentraalBesturingssysteemRPIBUS::verwerkCommando);

    // 2. Koppel uitgaande data en statussen (vanuit het systeem) aan het netwerk
    QObject::connect(&systeem, &CentraalBesturingssysteemRPIBUS::stuurNetwerkData,
                     &socket, &SocketCommunicatieRPIBUS::verzendData);

    // Start de servers/clients
    socket.start();
    return a.exec();
}