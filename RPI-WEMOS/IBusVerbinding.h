#ifndef IBUSVERBINDING_H
#define IBUSVERBINDING_H

#include <QObject>
#include <QString>

// ============================================================================
//  IBusVerbinding — interface voor de verbinding met de RPI-BUS.
// ----------------------------------------------------------------------------
//  Abstraheert het ontvangen van sensordata en het versturen van commando's
//  richting de RPI-BUS. Concreet geïmplementeerd door SocketCommunicatieRPIWEMOS
//  (rauwe BSD-sockets), maar vervangbaar door bv. een QTcpSocket- of test-/mock-
//  variant. De rest van de applicatie kent alleen deze interface.
// ============================================================================
class IBusVerbinding : public QObject {
    Q_OBJECT
public:
    explicit IBusVerbinding(QObject *parent = nullptr) : QObject(parent) {}
    ~IBusVerbinding() override = default;

    virtual bool verbind() = 0;                            // start luisteren/verbinden
    virtual void verzendData(const QString &bericht) = 0;  // commando naar de BUS
    virtual QString ontvangData() = 0;                     // opgehoopte sensordata ophalen
    virtual bool checkConnectieStatus() = 0;               // recent nog data ontvangen?

signals:
    void nieuwLogBericht(QString bericht);
};

#endif // IBUSVERBINDING_H
