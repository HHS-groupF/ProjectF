#ifndef IBERICHTKANAAL_H
#define IBERICHTKANAAL_H

#include <QObject>
#include <QString>

// ============================================================================
//  IBerichtKanaal — communicatie-interface (transport) voor Runes.
// ----------------------------------------------------------------------------
//  Abstraheert "hoe" berichten ("topic payload") de deur uit en weer binnen
//  komen. Concreet geïmplementeerd door Heimdall (rauwe BSD-sockets / Bifrost),
//  maar net zo goed vervangbaar door bv. een MQTT-, WebSocket- of QTcpServer-
//  variant. De rest van de applicatie (MainWindow, modules) kent alleen deze
//  interface, niet de concrete implementatie — Heimdall is dus uitwisselbaar.
//
//  Het is een abstracte QObject zodat de signalen hier gedeclareerd kunnen
//  worden; implementaties erven ervan en 'emit'-en runeOntvangen / logBericht.
// ============================================================================
class IBerichtKanaal : public QObject {
    Q_OBJECT
public:
    explicit IBerichtKanaal(QObject *parent = nullptr) : QObject(parent) {}
    ~IBerichtKanaal() override = default;

    // Begin te luisteren / verbinden op de gegeven poort. True bij succes.
    virtual bool start(int poort) = 0;

    // Verstuur één Rune (topic + payload) naar de aangesloten devices.
    virtual void publiceer(const QString &topic, const QString &payload) = 0;

signals:
    void runeOntvangen(QString topic, QString payload); // inkomend bericht
    void logBericht(QString bericht);                   // statusmelding
};

#endif // IBERICHTKANAAL_H
