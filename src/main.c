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
char *build_rtl_command (char *buffer, float freq, char *mod, int bw, int sr);
void knob_turner(float *freq, char *mod, int *bw, int *sr);

/* =======================================
 * Globals
 * =======================================*/
#define RTL_FM_CMD "rtl_fm "
#define MOD_AM     " am "
#define MOD_AM_BW  16000
#define MOD_AM_SR  16000
#define MOD_FM     " wbfm "
#define MOD_FM_BW  240000
#define MOD_FM_SR  32000
#define MOD_NFM    " fm "
#define MOD_NFM_BW 24000
#define MOD_NFM_SR 24000
#define MOD_USB    " usb "
#define MOD_USB_BW 8000
#define MOD_USB_SR 8000
#define MOD_LSB    " lsb "
#define MOD_LSB_BW 8000
#define MOD_LSB_SR 8000
#define DIRECT_SMP " -E direct2 "
#define DIRECT_SMP_MAX_FREQ 24.0


/* =======================================
 * Main 
 * =======================================*/
int main(char args[])
{
  float freq = 0.0;
  int bw = 0;
  int sr = 0;
  char mod[6] = "",
       cmd[255] = "",
       cmd3[255] = "";

	  
  while (1)
  {
    strcpy(cmd3, cmd);

    knob_turner(&freq, mod, &bw, &sr);

    build_rtl_command(cmd3, freq, mod, bw, sr);
    /* printf("%s", cmd3); */

    system("killall -9 rtl_fm  > /dev/null 2>&1");
    system(cmd3);

    while((getchar()) != '\n');
  }

  exit(0); 
}

/*===========================================
 * Functions
 *==========================================*/
char* build_rtl_command (char *buffer, float freq, char *mod, int bw, int sr)
{
  char sub_buffer[16];

  strcat(buffer,"( ");
  strcat(buffer,RTL_FM_CMD);
  strcat(buffer,"-M");
  strcat(buffer,mod);
  snprintf(sub_buffer,16, "-f %.3fM ", freq);
  strcat(buffer, sub_buffer);
  
  /* Direct sample not needed for 24Mhz or higher */
  if(freq < DIRECT_SMP_MAX_FREQ )
    strcat(buffer, DIRECT_SMP);

  strcat(buffer, "-l0 ");
  snprintf(sub_buffer, 16,"-s %d ",bw);
  strcat(buffer, sub_buffer);
  strcat(buffer, "-g 50 | aplay ");
  snprintf(sub_buffer, 16,"-r %d ",sr);
  strcat(buffer, sub_buffer);
  strcat(buffer, "-f S16_LE -t raw ");
  strcat(buffer, ") > /dev/null 2>&1 &");

  return buffer;
}
 

void knob_turner(float *freq, char *mod, int *bw, int *sr)
{
  int mode = 1;
  int bwaux = *bw;
  int sraux = *sr;
  float freqaux = *freq;
  char modaux[10];

  system("clear");
  printf("Entre com a frequencia (MHz): ");
  scanf("%f",&freqaux);
  while((getchar()) != '\n');

  do
  {
    printf("\nModulacao:\n");
    printf("===================\n");
    printf(" 1 - AM (Default) \n");
    printf(" 2 - FM (Comercial) \n");
    printf(" 3 - FM (Radio Amador) \n");
    printf(" 4 - Banda lateral superior (USB) \n");
    printf(" 5 - Banda lateral inferior (LSB) \n");
    printf("====================\n");
    printf("Modo: ");
    scanf("%d", &mode);

    if(mode < 1 && mode >5)
       printf("\n***** MODO INVALIDO *****");
  
  }
  while(mode < 1 && mode > 5);
  

  switch(mode)
  {
     case (2):
      strcpy(mod, MOD_FM);
      bwaux = MOD_FM_BW;
      sraux = MOD_FM_SR;
      break;

     case (3):
      strcpy(mod, MOD_NFM);
      bwaux = MOD_NFM_BW;
      sraux = MOD_NFM_SR;
      break;

     case (4):
      strcpy(mod,MOD_USB);
      bwaux = MOD_USB_BW;
      sraux = MOD_USB_SR;
      break;

     case (5):
      strcpy(mod, MOD_LSB);
      bwaux = MOD_LSB_BW;
      sraux = MOD_LSB_SR;
      break;

     default:
      strcpy(mod, MOD_AM);
      bwaux = MOD_AM_BW;
      sraux = MOD_AM_SR;
  }

  *bw = bwaux;
  *freq = freqaux;
  *sr = sraux;
}


