#ifndef CENTRAALBESTURINGSSYSTEEMRPIWEMOS_H
#define CENTRAALBESTURINGSSYSTEEMRPIWEMOS_H

#include <QObject>
#include <QString>

class CentraalBesturingssysteemRPIWEMOS : public QObject {
    Q_OBJECT
public:
    explicit CentraalBesturingssysteemRPIWEMOS(QObject *parent = nullptr);
    void verwerkInkomendeStatus(bool brand, bool overrule, bool ventilator);
public slots:
    void activeerBrandOverrule();
signals:
    void stuurNetwerkCommando(const QString &commando);
    void ventilatorStatusGewijzigd(bool aan);
    void brandAlarmStatusGewijzigd(bool actief);
    void overruleStatusGewijzigd(bool actief);
    void logBerichtGegenereerd(const QString &bericht);
};

#endif // CENTRAALBESTURINGSSYSTEEMRPIWEMOS_H