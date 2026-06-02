#ifndef CONFIG_H
#define CONFIG_H

#include <string>

// Centrale netwerkconfiguratie voor de socketverbinding tussen RPI-BUS en RPI-WEMOS.
// Pas hier de IP-adressen en poort aan; de socketklassen lezen deze waarden uit.
namespace Config {
    // IP-adressen van de twee machines
    const std::string IP_RPIBUS   = "10.0.0.1"; // adres van de BUS-machine
    const std::string IP_RPIWEMOS = "10.0.0.2"; // adres van de WEMOS-machine

    // Gedeelde poort voor zenden en ontvangen
    constexpr int POORT = 50001;
}

#endif // CONFIG_H
