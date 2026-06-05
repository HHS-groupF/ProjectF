#include "CentraalBesturingssysteemRPIBUS.h"
#include "SysteemConfig.h"
#include <QByteArray>
#include <QTimer>

CentraalBesturingssysteemRPIBUS::CentraalBesturingssysteemRPIBUS(QObject *parent)
    : QObject(parent)
{
    huidigTemp = 20.0;
    huidigCO2 = 400.0;
    huidigLucht = 45.0;
    isVentilatorAan = false;
    isBrandAlarmActief = false;
    isOverruleActief = false;
}

void CentraalBesturingssysteemRPIBUS::verwerkCanSensorData(uint32_t stmId, const QString &sensorType, double waarde) {
    if (sensorType == "TEMP") huidigTemp = waarde;
    else if (sensorType == "CO2") huidigCO2 = waarde;
    else if (sensorType == "HUM") huidigLucht = waarde;

    QString bericht = QString("SENSOR %1 %2 %3\n").arg(static_cast<int>(stmId)).arg(sensorType).arg(waarde);
    emit stuurNetwerkData(bericht);
    controleerLimieten();
}

void CentraalBesturingssysteemRPIBUS::verwerkCommando(const QString &commando) {
    if (commando.contains("ALARM_OVERRULED")) {
        if (isBrandAlarmActief) {
            isBrandAlarmActief = false;
            isOverruleActief = true;

            // Schakel na 5 seconden (5000 milliseconden) de overrule automatisch weer uit
            QTimer::singleShot(5000, this, [this]() {
                isOverruleActief = false;
                controleerLimieten(); // Herbereken de status om de normale monitoring te hervatten
            });

            controleerLimieten(); 
        }
    }
}

void CentraalBesturingssysteemRPIBUS::verwerkStmNoodstopReset() {
    // Dit wordt getriggerd door de fysieke STM32 knop (ID 0x300)
    isBrandAlarmActief = false;
    isOverruleActief = true;
    
    // Stuur een melding naar de WEMOS zodat de actie in het logboek verschijnt
    emit stuurNetwerkData("LOG STM32 Fysieke brandoverrule-knop ingedrukt!\n");
    
    // Start exact dezelfde timer van 5 seconden voor de fysieke knop
    QTimer::singleShot(5000, this, [this]() {
        isOverruleActief = false;
        controleerLimieten(); // Herbereken de status om de normale monitoring te hervatten
    });

    controleerLimieten();
}

void CentraalBesturingssysteemRPIBUS::controleerLimieten() {
    bool brandGevaar = (huidigTemp > Config::TEMP_BRAND || huidigCO2 > Config::CO2_BRAND);
    bool vorigeBrandAlarmStaat = isBrandAlarmActief; 

    if (brandGevaar) {
        if (!isOverruleActief) {
            isBrandAlarmActief = true;
        }
    }

    // Controleer of de status is VERANDERD (dus ook als hij UIT gaat door een overrule)
    if (isBrandAlarmActief != vorigeBrandAlarmStaat) {
        QByteArray alarmPayload;
        // Stuur 0x01 als het alarm afgaat, stuur 0x00 als het overruled/uit is
        alarmPayload.append(static_cast<char>(isBrandAlarmActief ? 0x01 : 0x00));
        // Gebruik het nieuwe, hoge-prioriteit CAN ID
        emit stuurCanCommando(Config::STM32_BRANDALARM_ID, alarmPayload); 
    }

    bool teWarm = (huidigTemp > Config::TEMP_WAARSCHUWING);
    bool teVeelCO2 = (huidigCO2 > Config::CO2_WAARSCHUWING);
    bool teVochtig = (huidigLucht > Config::HUM_WAARSCHUWING);

    bool vorigeVentilatorStaat = isVentilatorAan;

    if (isBrandAlarmActief) {
        isVentilatorAan = false; 
    } else {
        isVentilatorAan = (teWarm || teVeelCO2 || teVochtig);
    }

    if (isVentilatorAan != vorigeVentilatorStaat) {
        QByteArray canPayload;
        canPayload.append(static_cast<char>(isVentilatorAan ? 0x01 : 0x00)); 
        emit stuurCanCommando(Config::STM32_VENTILATOR_ID, canPayload);
    }

    verzendSysteemStatus();
}

void CentraalBesturingssysteemRPIBUS::verzendSysteemStatus() {
    QString bericht = QString("STATUS %1 %2 %3\n")
                          .arg(isBrandAlarmActief ? 1 : 0)
                          .arg(isOverruleActief ? 1 : 0)
                          .arg(isVentilatorAan ? 1 : 0);
    emit stuurNetwerkData(bericht);
}
