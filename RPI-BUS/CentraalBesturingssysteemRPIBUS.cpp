#include "CentraalBesturingssysteemRPIBUS.h"
#include "SysteemConfig.h"
#include <QRandomGenerator>
#include <QJsonObject>
#include <QJsonDocument>

CentraalBesturingssysteemRPIBUS::CentraalBesturingssysteemRPIBUS(QObject *parent)
    : QObject(parent)
{
    sensorTimer = new QTimer(this);
    connect(sensorTimer, &QTimer::timeout, this, &CentraalBesturingssysteemRPIBUS::genereerEnVerwerkSensorData);
    sensorTimer->start(Config::SENSOR_TIMER_INTERVAL);
}

void CentraalBesturingssysteemRPIBUS::genereerEnVerwerkSensorData() {
    QJsonObject json;
    json["type"] = "sensor";

    int randomKeuze = QRandomGenerator::global()->bounded(3);
    if(randomKeuze == 0) {
        huidigCO2 = QRandomGenerator::global()->bounded(200, 1550);
        json["sensorId"] = "CO2";
        json["waarde"] = huidigCO2;
    } else if (randomKeuze == 1) {
        huidigTemp = QRandomGenerator::global()->bounded(15, 50);
        json["sensorId"] = "TEMP";
        json["waarde"] = huidigTemp;
    } else {
        huidigLucht = QRandomGenerator::global()->bounded(30, 70);
        json["sensorId"] = "HUM";
        json["waarde"] = huidigLucht;
    }
    emit stuurNetwerkData(QJsonDocument(json).toJson(QJsonDocument::Compact) + "\n");
    controleerLimieten();
}

void CentraalBesturingssysteemRPIBUS::verwerkCommando(const QString &commando) {
    if (commando.contains("ALARM_OVERRULED") && isBrandAlarmActief) {
        isBrandAlarmActief = false;
        isOverruleActief = true;
        verzendSysteemStatus();
        controleerLimieten();
    }
}

void CentraalBesturingssysteemRPIBUS::controleerLimieten() {
    // 1. Controleer op brandgevaar
    bool brandGevaar = (huidigTemp > Config::TEMP_BRAND || huidigCO2 > Config::CO2_BRAND);

    if (brandGevaar && !isBrandAlarmActief && !isOverruleActief) {
        isBrandAlarmActief = true;
    } else if (!brandGevaar && isOverruleActief) {
        isOverruleActief = false;
    }

    // 2. Controleer normale limieten
    bool teWarm = (huidigTemp > Config::TEMP_WAARSCHUWING);
    bool teVeelCO2 = (huidigCO2 > Config::CO2_WAARSCHUWING);
    bool teVochtig = (huidigLucht > Config::HUM_WAARSCHUWING);

    // 3. Ventilator logica (bij brand moet deze UIT om zuurstof toevoer te stoppen)
    isVentilatorAan = !isBrandAlarmActief && (teWarm || teVeelCO2 || teVochtig);
    verzendSysteemStatus();
}

void CentraalBesturingssysteemRPIBUS::verzendSysteemStatus() {
    // Verpakt de UI-status in JSON zodat WEMOS alleen nog maar hoeft te renderen
    QJsonObject status;
    status["type"] = "status";
    status["brandAlarm"] = isBrandAlarmActief;
    status["overrule"] = isOverruleActief;
    status["ventilator"] = isVentilatorAan;

    emit stuurNetwerkData(QJsonDocument(status).toJson(QJsonDocument::Compact) + "\n");
}