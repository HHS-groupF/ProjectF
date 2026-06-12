#ifndef BUSPROTOCOLPARSER_H
#define BUSPROTOCOLPARSER_H

#include <QObject>
#include <QString>

// ============================================================================
//  BusProtocolParser — decodeert het tekstprotocol van de RPI-BUS.
// ----------------------------------------------------------------------------
//  Krijgt rauwe (mogelijk gefragmenteerde) TCP-data binnen, buffert tot hele
//  regels, en zet die om in getypeerde signalen. Zo kent de UI het draadformaat
//  niet meer; die reageert alleen nog op betekenisvolle gebeurtenissen.
//
//  Regelformaten (ongewijzigd t.o.v. de BUS/STM):
//    HEARTBEAT
//    LOG <tekst>
//    STATUS <brand> <overrule> <ventilator>     (1/0)
//    SENSOR <nodeId> <sensorId> <waarde>
// ============================================================================
class BusProtocolParser : public QObject {
    Q_OBJECT
public:
    explicit BusProtocolParser(QObject *parent = nullptr);

    // Voeg nieuwe rauwe data toe en verwerk alle complete regels die nu in de
    // buffer zitten (TCP stream-safe).
    void verwerk(const QString &ruweData);

signals:
    void heartbeatOntvangen();
    void hardwareLog(const QString &bericht);              // "LOG ..." (bv. STM-meldingen)
    void algemeenBerichtOntvangen(const QString &bericht); // elke overige regel (voor het logboek)
    void gebouwStatus(bool brand, bool overrule, bool ventilator);
    void sensorMeting(const QString &sensorId, double waarde);

private:
    QString buffer;
};

#endif // BUSPROTOCOLPARSER_H
