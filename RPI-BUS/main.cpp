#include <QCoreApplication>
#include "SocketCommunicatieRPIBUS.h"
#include "CentraalBesturingssysteemRPIBUS.h"
#include "CanBusCommunicatieRPIBUS.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    SocketCommunicatieRPIBUS socket;
    CentraalBesturingssysteemRPIBUS systeem;
    CanBusCommunicatieRPIBUS canBus;

    QObject::connect(&socket, &SocketCommunicatieRPIBUS::inkomendCommando,
                     &systeem, &CentraalBesturingssysteemRPIBUS::verwerkCommando);

    QObject::connect(&systeem, &CentraalBesturingssysteemRPIBUS::stuurNetwerkData,
                     &socket, &SocketCommunicatieRPIBUS::verzendData);
    
    
    QObject::connect(&canBus, &CanBusCommunicatieRPIBUS::inkomendeSensorData,
                     &systeem, &CentraalBesturingssysteemRPIBUS::verwerkCanSensorData);

   
    QObject::connect(&systeem, &CentraalBesturingssysteemRPIBUS::stuurCanCommando,
                     &canBus, &CanBusCommunicatieRPIBUS::verstuurCommandoNaarSTM);

    QObject::connect(&canBus, &CanBusCommunicatieRPIBUS::noodstopVrijgegeven, &systeem, &CentraalBesturingssysteemRPIBUS::verwerkStmNoodstopReset);

    socket.start();
    canBus.start("can0");
    
    return a.exec();
}
