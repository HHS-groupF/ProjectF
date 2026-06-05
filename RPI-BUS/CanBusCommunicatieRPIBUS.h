#ifndef CANBUSCOMMUNICATIERPIBUS_H
#define CANBUSCOMMUNICATIERPIBUS_H

#include <QObject>
#include <QCanBus>
#include <QCanBusDevice>
#include <QCanBusFrame>
#include <QDebug>

class CanBusCommunicatieRPIBUS : public QObject {
    Q_OBJECT
public:
    explicit CanBusCommunicatieRPIBUS(QObject *parent = nullptr);
    ~CanBusCommunicatieRPIBUS();
    void start(const QString &interfaceName = "can0");

public slots:
    // Slot om commando's (bijv. ventilator of LED aan/uit) naar een specifieke STM32 te sturen
    void verstuurCommandoNaarSTM(uint32_t stmId, const QByteArray &data);

signals:
    // Signaal dat wordt afgevuurd als we data ontvangen van de STM32
    void inkomendeSensorData(uint32_t stmId, const QString &sensorType, double waarde);
    void noodstopVrijgegeven();

private slots:
    void ontvangCanBerichten();

private:
    QCanBusDevice *canDevice = nullptr;
};

#endif // CANBUSCOMMUNICATIERPIBUS_H
