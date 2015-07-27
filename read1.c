#include <wiringPi.h>
#include <stdio.h>


int main()
{
  const int tpin = 0;
  wiringPiSetup();
  pinMode(tpin, INPUT);
  pullUpDnControl(tpin, PUD_UP);
  char last = 0;
  char tval;

  while(1)
  {
    tval = !digitalRead(tpin);
    if(tval && last == 0)
    {
      printf("It's on!\n");
      last = 1;
    }
    if(!tval && last == 1)
    {
      printf("It's off!\n");
      last = 0;
    }
    delay(100);
  }


  return 0;
}
