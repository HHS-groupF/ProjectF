#ifndef CENTRAALBESTURINGSSYSTEEMRPIWEMOS_H
#define CENTRAALBESTURINGSSYSTEEMRPIWEMOS_H

#include <QObject>
#include <QString>
#include <QTimer>

class CentraalBesturingssysteemRPIWEMOS : public QObject {
    Q_OBJECT
public:
    explicit CentraalBesturingssysteemRPIWEMOS(QObject *parent = nullptr);
    void verwerkInkomendeStatus(bool brand, bool overrule, bool ventilator);

public slots:
    void activeerBrandOverrule();

    // --- Tafel- en RGB-logica ---
    void verwerkBifrostRune(const QString &topic, const QString &payload); // tafel/status + sensor/beweging (van Heimdall)
    void resetTafel(int id);
    void zetRgbKleur(const QString &kleurNaam);
    void zetRgbUit();
    void zetRgbAutoModus(bool aan);                 // checkbox: RGB volgt bewegingssensor
    void zetRgbKleurKeuze(const QString &kleurNaam); // onthoudt de combobox-keuze

signals:
    void stuurNetwerkCommando(const QString &commando);                     // naar de BUS-socket
    void stuurBifrostBericht(const QString &topic, const QString &payload); // naar Heimdall (Wemos)
    void ventilatorStatusGewijzigd(bool aan);
    void brandAlarmStatusGewijzigd(bool actief);
    void overruleStatusGewijzigd(bool actief);
    void logBerichtGegenereerd(const QString &bericht);

    // --- Tafel + RGB UI-signalen ---
    void tafelStatusGewijzigd(int id, bool hulpNodig);
    void bewegingStatusGewijzigd(bool beweging);
    void rgbStatusGewijzigd(const QString &rgbWaarde);  // "r,g,b" of "UIT"

private:
    bool rgbAutoModus = true;        // standaard: RGB volgt de bewegingssensor
    bool laatsteBeweging = false;    // huidige bewegingsstatus (voor "RGB volgt beweging")
    QString rgbKleurKeuze = "Wit";   // laatst gekozen kleur in de combobox
    QTimer *rgbUitTimer = nullptr;   // vertraging voor het uitschakelen van de RGB na "geen beweging"
    bool kleurNaarWaarde(const QString &naam, QString &rgbWaarde) const;
};

#endif // CENTRAALBESTURINGSSYSTEEMRPIWEMOS_H
