#include "DashboardUI.h"

void DashboardUI::toonMenu() {
    std::cout << "\n=== RESTAURANT COMMANDO'S ===\n";
    std::cout << "- [1, 2, 3...] : Typ een tafelnummer in om te resetten.\n";
    std::cout << "- menu [tekst] : Stel de oneindige loop in op de lichtkrant.\n";
    std::cout << "- msg [tekst]  : Snelle announcement op de lichtkrant.\n";
    std::cout << "- q            : Stoppen.\n";
    toonPrompt();
}

void DashboardUI::toonPrompt() {
    std::cout << "> " << std::flush;
}

void DashboardUI::toonFoutmelding(const std::string& fout) {
    std::cout << "\r\033[2K" << fout << "\n";
    toonPrompt();
}