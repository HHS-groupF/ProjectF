#ifndef TAFELMODULE_H
#define TAFELMODULE_H

#include <QObject>
#include <QString>

#include "IRuneVerwerker.h"

// ============================================================================
//  TafelModule — tafel-oproepsysteem.
// ----------------------------------------------------------------------------
//  Verwerkt het Bifrost-topic "tafel/<id>/status" (HELP / OK) en kan een
//  reset naar een specifieke tafel sturen via "tafel/<id>/reset".
// ============================================================================
class TafelModule : public QObject, public IRuneVerwerker {
    Q_OBJECT
public:
    explicit TafelModule(QObject *parent = nullptr);

    // IRuneVerwerker
    bool kanVerwerken(const QString &topic) const override;
    void verwerkRune(const QString &topic, const QString &payload) override;

public slots:
    void resetTafel(int id);

signals:
    void stuurBifrostBericht(const QString &topic, const QString &payload);
    void tafelStatusGewijzigd(int id, bool hulpNodig);
};

#endif // TAFELMODULE_H
