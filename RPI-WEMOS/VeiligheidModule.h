#ifndef VEILIGHEIDMODULE_H
#define VEILIGHEIDMODULE_H

#include <QObject>
#include <QString>

// ============================================================================
//  VeiligheidModule — brand / ventilator / brand-overrule.
// ----------------------------------------------------------------------------
//  Krijgt de gebouwstatus binnen via de RPI-BUS-socket (verwerkInkomendeStatus)
//  en kan een handmatige overrule terugsturen naar de BUS. Bevat geen RGB- of
//  tafellogica meer; dat zijn aparte modules.
// ============================================================================
class VeiligheidModule : public QObject {
    Q_OBJECT
public:
    explicit VeiligheidModule(QObject *parent = nullptr);

    void verwerkInkomendeStatus(bool brand, bool overrule, bool ventilator);

public slots:
    void activeerBrandOverrule();

signals:
    void ventilatorStatusGewijzigd(bool aan);
    void brandAlarmStatusGewijzigd(bool actief);
    void overruleStatusGewijzigd(bool actief);
    void stuurNetwerkCommando(const QString &commando); // naar de RPI-BUS-socket
    void logBerichtGegenereerd(const QString &bericht);
};

#endif // VEILIGHEIDMODULE_H
