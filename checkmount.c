#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
  char tcheck[] = "usb-Garmin_FR10_Flash-0:0";
  char * currentdisk;
  currentdisk = (char*)malloc(200*sizeof(char));
  int i;
  int currentloc = 0;
  for(i=0;i<length;++i)
  {
    if(input[i] == '\0')
      break;
    else if(input[i] == '\n')
    {
      currentdisk[currentloc] = '\0';
      if(strcmp(currentdisk, tcheck) == 0)
      {
        free(currentdisk);
        return 1;
      }
      currentloc = 0;
    }
    else
    {
      currentdisk[currentloc] = input[i];
      ++currentloc;
    }
  }

  currentdisk[currentloc] = '\0';
  if(strcmp(currentdisk, tcheck) == 0)
  {
    free(currentdisk);
    return 1;
  }
  return 0;
}

int read_devices(char ** fresults)
{
  system("ls -1 /dev/disk/by-id/ > lsres.txt");
  char fname[] = "lsres.txt";
  *fresults = NULL;
  *fresults = (char*) malloc(10000*sizeof(char));
  FILE *fobj;
  int cloc = 0;
  fobj = fopen(fname, "r");
  char c;

  while((c=fgetc(fobj)) != EOF && c != 255)
  {
    (*fresults)[cloc] = c;
    ++cloc;
  }
  (*fresults)[cloc] = '\0';
  fclose(fobj);
  return cloc;
}

int main()
{
  char currentval = -1;

  while(1)
  {
    char * fresults;
    int cloc = read_devices(&fresults);
    char tval = is_mounted(fresults, cloc);
    free(fresults);
    if(tval != currentval)
    {
      printf("======tv!=cv\n");
      currentval = tval;
      if(tval == 1)
        printf("Garmin connected!\n");
      else
        printf("Garmin disconnected!\n");
    }
    delay(500);
  }

  
  return 0;
}
