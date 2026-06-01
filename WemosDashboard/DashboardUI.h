#ifndef DASHBOARDUI_H
#define DASHBOARDUI_H

#include <iostream>

class DashboardUI {
public:
    void toonMenu();
    void toonPrompt();
    void toonFoutmelding(const std::string& fout);
};

#endif