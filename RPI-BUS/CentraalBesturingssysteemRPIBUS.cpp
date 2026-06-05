#include "CentraalBesturingssysteemRPIBUS.h"
#include "SysteemConfig.h" // Zorg dat STM32_VENTILATOR_ID en limieten hierin staan
#include <QByteArray>

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
            controleerLimieten(); 
        }
    }
}

void CentraalBesturingssysteemRPIBUS::verwerkStmNoodstopReset() {
    if (isBrandAlarmActief) {
        isBrandAlarmActief = false;
        isOverruleActief = false;
        controleerLimieten();
    }
}

void CentraalBesturingssysteemRPIBUS::controleerLimieten() {
    bool brandGevaar = (huidigTemp > Config::TEMP_BRAND || huidigCO2 > Config::CO2_BRAND);
    bool vorigeBrandAlarmStaat = isBrandAlarmActief; 

    if (brandGevaar) {
        if (!isOverruleActief) {
            isBrandAlarmActief = true;
        }
    }

    if (isBrandAlarmActief && !vorigeBrandAlarmStaat) {
        QByteArray alarmPayload;
        alarmPayload.append(static_cast<char>(0x01));
        emit stuurCanCommando(0x400, alarmPayload); 
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
        if (isVentilatorAan) canPayload.append(static_cast<char>(0x01)); 
        else canPayload.append(static_cast<char>(0x00)); 
        
        emit stuurCanCommando(STM32_VENTILATOR_ID, canPayload);
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
