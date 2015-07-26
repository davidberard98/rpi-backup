#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
  const int poweroffpin = 7;
  wiringPiSetup();
  pinMode(poweroffpin, INPUT);
  pullUpDnControl(poweroffpin, PUD_UP);

  while(1)
  {
    if(!digitalRead(poweroffpin))
    {
      system("poweroff");
    }
    delay(25);
  }


  return 0;
}
