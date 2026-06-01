#include "Tafel.h"

Tafel::Tafel(int bPin, int lPin, int id, NetwerkManager& netwerkManager) 
  : _netwerk(netwerkManager) { 
  _buttonPin = bPin;
  _ledPin = lPin;
  _id = id;
  _helpNodig = false;
  _laatsteKnopStatus = HIGH;
  _laatsteDebounceTijd = 0;
}

void Tafel::setup() {
  pinMode(_buttonPin, INPUT_PULLUP);
  pinMode(_ledPin, OUTPUT);
  digitalWrite(_ledPin, LOW);
}

void Tafel::update() {
  int lezing = digitalRead(_buttonPin);

  // 1. Check of de fysieke knopstatus is veranderd (debounce logica)
  if (lezing != _laatsteKnopStatus) {
    _laatsteDebounceTijd = millis();
  }

  // 2. Als de status al even stabiel is (50ms), kijken we wat er moet gebeuren
  if ((millis() - _laatsteDebounceTijd) > _debounceDelay) {
    
    // Als de knop wordt ingedrukt (LOW) en er was nog geen hulp nodig
    if (lezing == LOW && !_helpNodig) {
      zetLampAan();
    }
  }

  _laatsteKnopStatus = lezing;
}

void Tafel::zetLampAan() {
  _helpNodig = true;
  digitalWrite(_ledPin, HIGH);
  
  // We maken een uniek bericht voor deze tafel
  String topic = "tafel/" + String(_id) + "/status";
  
  // De tafel vraagt aan de manager: "stuur dit bericht voor mij"
  _netwerk.stuurBericht(topic.c_str(), "HELP");
  
  Serial.print("Tafel "); Serial.print(_id); Serial.println(": Roept hulp!");
}

void Tafel::zetLampUit() {
  // Alleen iets doen als de lamp ook echt aan stond
  if (_helpNodig) {
    _helpNodig = false;
    digitalWrite(_ledPin, LOW);
    
    String topic = "tafel/" + String(_id) + "/status";
    _netwerk.stuurBericht(topic.c_str(), "OK");
    
    Serial.print("Tafel "); Serial.print(_id); Serial.println(": Geholpen, lamp uit.");
  }
}

int Tafel::getId() {
  return _id;
}