#ifndef TAFEL_CONFIG_H
#define TAFEL_CONFIG_H

// Unit 2 - Tafel 2
// Tafel(knopPin, ledPin, tafelId, n)
#define TAFEL_CONFIG(n) Tafel(D1, D2, 2, n) // D1 = knop, D2 = led

// Voorbeeld met meerdere tafels:
// #define TAFEL_CONFIG(n) Tafel(D1, D2, 2, n), Tafel(D5, D6, 3, n)

#endif
