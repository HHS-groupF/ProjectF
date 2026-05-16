#ifndef TAFEL_CONFIG_H
#define TAFEL_CONFIG_H

// Unit 1 - Tafel 1
// Tafel(knopPin, ledPin, tafelId, n)
#define TAFEL_CONFIG(n) Tafel(D1, D2, 1, n) // D1 = knop, D2 = led

// Voorbeeld met meerdere tafels:
// #define TAFEL_CONFIG(n) Tafel(D1, D2, 1, n), Tafel(D5, D6, 2, n)

#endif
