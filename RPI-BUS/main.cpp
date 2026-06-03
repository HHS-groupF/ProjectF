#include <QCoreApplication>
#include "SocketCommunicatieRPIBUS.h"
#include "CentraalBesturingssysteemRPIBUS.h"
#include "CanBusCommunicatieRPIBUS.h" // NIEUW

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    SocketCommunicatieRPIBUS socket;
    CentraalBesturingssysteemRPIBUS systeem;
    CanBusCommunicatieRPIBUS canBus; // NIEUW

    // Socket -> Systeem (Binnenkomende Netwerk Commando's)
    QObject::connect(&socket, &SocketCommunicatieRPIBUS::inkomendCommando,
                     &systeem, &CentraalBesturingssysteemRPIBUS::verwerkCommando);

    // Systeem -> Socket (Uitgaande Netwerk Data)
    QObject::connect(&systeem, &CentraalBesturingssysteemRPIBUS::stuurNetwerkData,
                     &socket, &SocketCommunicatieRPIBUS::verzendData);

    // NIEUW: CAN -> Systeem (Binnenkomende STM32 Sensor Data)
    QObject::connect(&canBus, &CanBusCommunicatieRPIBUS::inkomendeSensorData,
                     &systeem, &CentraalBesturingssysteemRPIBUS::verwerkCanSensorData);

    // NIEUW: Systeem -> CAN (Uitgaande STM32 Commando's)
    QObject::connect(&systeem, &CentraalBesturingssysteemRPIBUS::stuurCanCommando,
                     &canBus, &CanBusCommunicatieRPIBUS::verstuurCommandoNaarSTM);

    socket.start();
    canBus.start("can0"); // Start de CAN-bus op interface can0

    return a.exec();
}