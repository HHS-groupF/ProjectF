#include <QCoreApplication>
#include "SocketCommunicatieRPIBUS.h"
#include "CentraalBesturingssysteemRPIBUS.h"
#include "CanBusCommunicatieRPIBUS.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Maak de netwerkmodule en de logica module aan
    SocketCommunicatieRPIBUS socket;
    CentraalBesturingssysteemRPIBUS systeem;
    CanBusCommunicatieRPIBUS canBus;

    // 1. Koppel inkomende netwerkcommando's (vanaf WEMOS) aan het systeem
    QObject::connect(&socket, &SocketCommunicatieRPIBUS::inkomendCommando,
                     &systeem, &CentraalBesturingssysteemRPIBUS::verwerkCommando);

    // 2. Koppel uitgaande data en statussen (vanuit het systeem) aan het netwerk
    QObject::connect(&systeem, &CentraalBesturingssysteemRPIBUS::stuurNetwerkData,
                     &socket, &SocketCommunicatieRPIBUS::verzendData);
    
    // NIEUW: CAN -> Systeem (Binnenkomende STM32 Sensor Data)
    QObject::connect(&canBus, &CanBusCommunicatieRPIBUS::inkomendeSensorData,
                     &systeem, &CentraalBesturingssysteemRPIBUS::verwerkCanSensorData);

    // NIEUW: Systeem -> CAN (Uitgaande STM32 Commando's)
    QObject::connect(&systeem, &CentraalBesturingssysteemRPIBUS::stuurCanCommando,
                     &canBus, &CanBusCommunicatieRPIBUS::verstuurCommandoNaarSTM);

    QObject::connect(&canBus, &CanBusCommunicatieRPIBUS::noodstopVrijgegeven, &systeem, &CentraalBesturingssysteemRPIBUS::verwerkStmNoodstopReset);

    // Start de servers/clients
    socket.start();
    canBus.start("can0");
    
    return a.exec();
}
