#ifndef IRUNEVERWERKER_H
#define IRUNEVERWERKER_H

#include <QString>

// ============================================================================
//  IRuneVerwerker — interface voor een module die Bifrost-runes verwerkt.
// ----------------------------------------------------------------------------
//  Een Rune is "topic payload" (bv. "sensor/beweging JA"). Elke device-module
//  (RGB, tafels, ...) beslist zelf of hij een topic aankan (kanVerwerken) en
//  hoe hij het afhandelt (verwerkRune). De coordinator hoeft de topics niet te
//  kennen: nieuwe modules toevoegen = registreren, niets in de dispatch wijzigen
//  (open/closed-principe).
// ============================================================================
class IRuneVerwerker {
public:
    virtual ~IRuneVerwerker() = default;

    // True als deze module het gegeven topic afhandelt.
    virtual bool kanVerwerken(const QString &topic) const = 0;

    // Handel één binnengekomen Rune af (alleen aangeroepen als kanVerwerken()).
    virtual void verwerkRune(const QString &topic, const QString &payload) = 0;
};

#endif // IRUNEVERWERKER_H
