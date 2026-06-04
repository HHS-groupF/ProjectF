#ifndef CENTRAALBESTURINGSSYSTEEMRPIBUS_H
#define CENTRAALBESTURINGSSYSTEEMRPIBUS_H

#include <QObject>
#include <QString>
#include <QTimer>

class CentraalBesturingssysteemRPIBUS : public QObject {
    Q_OBJECT
public:
    explicit CentraalBesturingssysteemRPIBUS(QObject *parent = nullptr);

public slots:
    // Slot voor inkomende netwerkcommando's (bijv. vanaf een andere Pi of dashboard)
    void verwerkCommando(const QString &commando);

    // Slot voor inkomende fysieke sensorwaardes vanaf de CAN-bus
    void verwerkCanSensorData(uint32_t stmId, const QString &sensorType, double waarde);

signals:
    // Signaal om data via de TCP-socket te verzenden
    void stuurNetwerkData(const QString &data);

    // Signaal om een fysiek commando over de CAN-bus naar een STM32 te sturen
    void stuurCanCommando(uint32_t stmId, const QByteArray &data);

private:
    double huidigTemp = 0.0;
    double huidigCO2 = 0.0;
    double huidigLucht = 0.0;

    bool isVentilatorAan = false;
    bool isBrandAlarmActief = false;
    bool isOverruleActief = false;

    // Constante voor het CAN ID van de STM32 waar de ventilator (LED) fysiek aan gekoppeld zit
    const uint32_t STM32_VENTILATOR_ID = 0x200;

    void controleerLimieten();
    void verzendSysteemStatus();
};

#endif // CENTRAALBESTURINGSSYSTEEMRPIBUS_H
