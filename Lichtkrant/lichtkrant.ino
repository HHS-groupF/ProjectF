/**
 * @file led_matrix_menu.ino
 * @brief Aansturing van een MAX7219 LED-matrix voor het project.
 * @details Dit script fungeert als een Proof of Concept voor het aansturen 
 * van een display. Het gebruikt de MD_Parola bibliotheek om tekst (het dagmenu) 
 * vloeiend over vier 8x8 matrix modules te laten scrollen via een ESP8266.
 */

#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

/** @def HARDWARE_TYPE Type LED-matrix hardware (FC16_HW is de standaard voor deze modules) */
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW 

/** @def MAX_DEVICES Het aantal 8x8 LED-matrix blokjes dat in serie is geschakeld */
#define MAX_DEVICES 4

/** @def CS_PIN De Chip Select (CS) pin van de SPI interface op de ESP8266 */
#define CS_PIN D8

/** * @brief Het hoofdobject dat de LED-matrix aanstuurt via de Parola bibliotheek.
 * @param HARDWARE_TYPE De specifieke hardware configuratie.
 * @param CS_PIN De pin die vertelt dat er data naar het scherm wordt gestuurd.
 * @param MAX_DEVICES Het totale aantal gekoppelde schermpjes.
 */
MD_Parola display = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

/**
 * @brief Initialisatie van de hardware en software (Opstartfase).
 * @details Deze functie wordt eenmalig aangeroepen bij het opstarten. Het start de 
 * SPI-communicatie, beperkt de helderheid om stroompieken te voorkomen, en wist 
 * eventuele oude data uit het geheugen van de schermen.
 */
void setup() {
  display.begin();
  display.setIntensity(5); // Helderheid beperkt tot 5 (schaal 0-15) voor stroomveiligheid
  display.displayClear();
}

/**
 * @brief De hoofdloop die de tekstanimatie continu uitvoert (Uitvoeringsfase).
 * @details Zet de animatieparameters klaar (de string, de richting en de snelheid). 
 * Daarna wordt een while-lus gebruikt om de functie displayAnimate() te pollen. 
 * Dit rendert de animatie frame-voor-frame totdat de tekst volledig uit beeld is.
 */
void loop() {
  // Configureer de tekst, uitlijning, het scroll-effect en de snelheid (pauze in ms tussen frames)
  display.displayScroll("Dagmenu : Groene curry !", PA_LEFT, PA_SCROLL_LEFT, 50);

  // Blijf frames renderen zolang de animatie nog bezig is (non-blocking)
  while (!display.displayAnimate()) {
    // Hier kan eventueel andere non-blocking code draaien
  }
}
