#include <wiringPi.h>

int main()
{
  const int tpin = 7;
  wiringPiSetup();
  pinMode(tpin, OUTPUT);
  
  while(1)
  {
    digitalWrite(tpin, HIGH);
    delay(500);
    digitalWrite(tpin, LOW);
    delay(500);
  }

  return 0;
}
