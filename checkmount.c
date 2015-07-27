#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

// Try looking for the watch and copying it if it is found.

int readnum(char * input, int length)
{
  int output = 0;
  int i;
  for(i=0;i<length;++i)
  {
    if(input[i] > 57 || input[i] < 48)
      break;
    output *= 10;
    output += input[i] - 48;
  }
  return output;
}

char is_mounted(char* input, int length)
{
  char ** disksfound;

  
  
}

int main()
{
  system("ls -1 /dev/disk/by-id/");

  usb-Garmin_FR10_Flash-0:0
  
  return 0;
}
