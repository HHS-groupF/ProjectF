#ifndef CENTRAALBESTURINGSSYSTEEMRPIWEMOS_H
#define CENTRAALBESTURINGSSYSTEEMRPIWEMOS_H

#include <QObject>
#include <QString>
#include <QVector>

class IRuneVerwerker;
class RgbModule;
class TafelModule;
class VeiligheidModule;

// ============================================================================
//  CentraalBesturingssysteemRPIWEMOS — coordinator.
// ----------------------------------------------------------------------------
//  Bevat zelf GEEN domeinlogica meer (was voorheen een god-class). Hij bezit de
//  losse modules (veiligheid, tafels, RGB), bundelt hun signalen naar buiten, en
//  routeert inkomende Bifrost-runes naar de module die het topic kan verwerken
//  (via de IRuneVerwerker-interface). Nieuwe device-modules toevoegen vereist
//  geen wijziging in de dispatch-logica.
//
//  De publieke API is bewust identiek aan de oude god-class gebleven, zodat de
//  MainWindow-koppelingen ongewijzigd blijven werken.
// ============================================================================
class CentraalBesturingssysteemRPIWEMOS : public QObject {
    Q_OBJECT
public:
    explicit CentraalBesturingssysteemRPIWEMOS(QObject *parent = nullptr);

    void verwerkInkomendeStatus(bool brand, bool overrule, bool ventilator);

    // Directe toegang tot de modules (voor fijnmazige koppeling indien gewenst).
    RgbModule        *rgb() const { return m_rgb; }
    TafelModule      *tafel() const { return m_tafel; }
    VeiligheidModule *veiligheid() const { return m_veiligheid; }

public slots:
    void activeerBrandOverrule();

    // --- Tafel- en RGB-logica (gedelegeerd aan de modules) ---
    void verwerkBifrostRune(const QString &topic, const QString &payload); // van Heimdall
    void resetTafel(int id);
    void zetRgbKleur(const QString &kleurNaam);
    void zetRgbUit();
    void zetRgbAutoModus(bool aan);
    void zetRgbKleurKeuze(const QString &kleurNaam);

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
    RgbModule        *m_rgb = nullptr;
    TafelModule      *m_tafel = nullptr;
    VeiligheidModule *m_veiligheid = nullptr;

    QVector<IRuneVerwerker*> m_verwerkers;  // rune-dispatch (open/closed)
};

#endif // CENTRAALBESTURINGSSYSTEEMRPIWEMOS_H
