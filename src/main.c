/* ======================================== 
 *
 * uStellara - Simple SDR Tunner in nCurses
 *
 * @author: Cleverson S A
 * date: 12/2021
 *
 * =======================================*/

/* =======================================
 * Libraries
 * =======================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* =======================================
 * Prototypes
 * =======================================*/
char *build_rtl_command (char *buffer, float freq, char *mod);

/* =======================================
 * Globals
 * =======================================*/
#define RTL_FM_CMD "rtl_fm "
#define MOD_AM     " am "
#define MOD_AM_BW  16000
#define MOD_FM     " wbfm "
#define MOD_FM_BW  32000
#define MOD_NFM    " fm "
#define MOD_NFM_BW 24000
#define MOD_USB    " usb "
#define MOD_USB_BW 8000
#define MOD_LSB    " lsb "
#define MOD_LSB_BW 8000
#define DIRECT_SMP " -E direct2 "


/* =======================================
 * Main 
 * =======================================*/
int main(char args[])
{
  int tecla = 0;
  float freq = 0.0;
  char cmd[255] = "";
  char cmd3[255] = "";

	  
  while (1)
  {
    strcpy(cmd3, cmd);

    system("clear");
    printf("Entre com a frequencia: ");
    scanf("%f", &freq);


    /* printf("%s\n\n", cmd3); */ 
    build_rtl_command(cmd3, freq, "am");

    system("killall -9 rtl_fm  > /dev/null 2>&1");
    system(cmd3);

    printf("\n\nPressione uma tecla para sair");
    while((getchar()) != '\n');
    tecla = getchar();
  }

  exit(0); 
}

/*===========================================
 * Functions
 *==========================================*/
char* build_rtl_command (char *buffer, float freq, char *mod)
{
  char sub_buffer[16];

  strcat(buffer,"( ");
  strcat(buffer,RTL_FM_CMD);
  strcat(buffer,"-M");
  strcat(buffer,MOD_AM);
  snprintf(sub_buffer,16, "-f %.3fM ", freq);
  strcat(buffer, sub_buffer);
  strcat(buffer, DIRECT_SMP);
  strcat(buffer, "-l0 ");
  snprintf(sub_buffer, 16,"-s %d ",MOD_AM_BW);
  strcat(buffer, sub_buffer);
  strcat(buffer, "-g 50 | aplay ");
  snprintf(sub_buffer, 16,"-r %d ",MOD_AM_BW);
  strcat(buffer, sub_buffer);
  strcat(buffer, "-f S16_LE -t raw ");
  strcat(buffer, ") > /dev/null 2>&1 &");

  return buffer;
}
 

