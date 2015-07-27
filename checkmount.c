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

void mountcopywow(int number)
{
  char *mkdircmd;
  mkdircmd = (char*)malloc(16*sizeof(char));
  char loc[10];
  char lbegin[] = "mkdir ";
  strcpy(mkdircmd, lbegin);
  loc[0] = 'd';
  loc[1] = 'a';
  loc[2] = 't';
  loc[3] = 'a';
  loc[4] = '/';
  loc[5] = 'b';
  loc[6] = (number/100)%10 + 48;
  loc[7] = (number/10)%10 + 48;
  loc[8] = (number%10) + 48;
  loc[9] = '\0';
  strcpy(mkdircmd+6, loc);
  system(mkdircmd);
  printf("  %s\n", mkdircmd);
  char mtcmd[] = "mount /dev/disk/by-id/usb-Garmin_FR10_Flash-0\\:0 mloc";
  system(mtcmd);
  printf("  %s\n", mtcmd);
  char *cpcmd = (char *) malloc(60);
  char cpbegin[] = "cp -R mloc/* ";
  strcpy(cpcmd, cpbegin);
  strcpy(cpcmd+13, loc);
  system(cpcmd);
  printf("  %s\n", cpcmd);
  char umtcmd[] = "umount mloc";
  system(umtcmd);
  printf("  %s\n", umtcmd);

  free(cpcmd);
  free(mkdircmd);
}

int findBN(char incr)
{
  char fname[] = "data/counter.txt";
  FILE *fobj = fopen(fname, "r");
  
  char c;
  int output = 0;
  while((c=fgetc(fobj)) != EOF && c >=48 && c <= 57)
  {
    output *= 10;
    output += c -48;
  }
  fclose(fobj);
  if(incr)
  {
    fobj = fopen(fname, "w");
    fprintf(fobj, "%d", output+1);
    fclose(fobj);
  }
  return output;
}

int main()
{
  const int tpin = 7;
  wiringPiSetup();
  pinMode(tpin, OUTPUT);

  const int popin = 0;
  wiringPiSetup();
  pinMode(popin, INPUT);
  pullUpDnControl(popin, PUD_UP);

  delay(1500);
  digitalWrite(tpin, HIGH);
  delay(1500);
  int j;
  for(j=0;j<20;++j)
  {
    delay(40);
    digitalWrite(tpin, HIGH);
    delay(40);
    digitalWrite(tpin, LOW);
  }

  char currentval = -1;
  char blinker = 0;
  int counter = 0;

  while(1)
  {
    if(!digitalRead(popin))
      system("poweroff");
    if(blinker)
      digitalWrite(tpin, counter%2);
    char * fresults;
    int cloc = read_devices(&fresults);
    char tval = is_mounted(fresults, cloc);
    free(fresults);
    if(tval != currentval)
    {
      printf("======tv!=cv\n");
      currentval = tval;
      if(tval == 1)
      {
        digitalWrite(tpin, HIGH);
	int backupnumber = findBN(1);
        mountcopywow(backupnumber);
	blinker = 1;
      }
      else
      {
        digitalWrite(tpin, LOW);
	blinker = 0;
      }
    }
    ++counter;
    delay(500);
  }

  
  return 0;
}
