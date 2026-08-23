#include "Keyboard.h"

class StreamDeck
{
public:
  StreamDeck(uint8_t clockPin, uint8_t resetPin, uint8_t readPin, uint8_t
    pulseTime = 5, bool pulseVal = 1)
  {
    _clockPin = clockPin;
    _resetPin = resetPin;
    _readPin = readPin;
    _pulseTime = pulseTime;
    _pulseVal = pulseVal;
  }

  void begin()
  {
    pinMode(_clockPin, OUTPUT);
    pinMode(_resetPin, OUTPUT);
    pinMode(_readPin, INPUT);
    Reset();
  }

  void Reset()
  {
    _SendPulse(_resetPin);
  }

  void SendPulse()
  {
    _SendPulse(_clockPin);
  }

  void SkipPulses(uint8_t count)
  {
    for(int i = 0; i < count; i++)
      SendPulse();
  }

  bool ReadPin()
  {
    return digitalRead(_readPin);
  }

  // Замість рядків з цифрами можна впсати будь-які бажані символи
  String dictionary[9] = {
    "1", "4", "7",
    "2", "5", "8",
    "3", "6", "9",
  };

private:
  uint8_t _clockPin;
  uint8_t _resetPin;
  uint8_t _readPin;
  uint8_t _pulseTime;
  bool _pulseVal;

  void _SendPulse(uint8_t pin)
  {
    digitalWrite(pin, _pulseVal);
    delay(_pulseTime);
    digitalWrite(pin, !_pulseVal);
  }
};

uint8_t row = 0;
uint8_t col = 0;
uint8_t keyNum = 0;
volatile uint16_t bitArr = 0;
StreamDeck strDeck(7, 5, 2);

void setup()
{
  Serial.begin(9600);
  strDeck.begin(); // Скидаємо лічильник
  Keyboard.begin();
}

void loop()
{
  if(strDeck.ReadPin()) // Зчитуємо значення з урахуванням брязкіту
  {
    delay(1);
    if(strDeck.ReadPin())
    {
      Keyboard.print(strDeck.dictionary[keyNum]);
      delay(45);
    }
  }

  keyNum++; // Рахуємо наступний імпульс пропускаючи 4 ряд, 4 стовп
  if(!(keyNum % 3))
  {
    strDeck.SkipPulses(1);
  }

  if(keyNum == 9)
  {
    keyNum = 0;
    strDeck.SkipPulses(4);
  }

  strDeck.SendPulse();
  delay(1);
}
