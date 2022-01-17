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
#include "frontend.h" 
#include <time.h>
#include "librtlfmevent.h"
#include "getopt/getopt.h"

/* =======================================
 * Prototypes
 * =======================================*/
char *build_rtl_command (char *buffer, ReceiverPanel *rp);
void knob_turner(ReceiverPanel *rp);
void increase_frequency(ReceiverPanel *rp);
void decrease_frequency(ReceiverPanel *rp);
void init_panel(ReceiverPanel *rp);
void call_rtl_fm(ReceiverPanel *rp);
void increase_volume(ReceiverPanel *rp);
void increase_clarifier(ReceiverPanel *rp);
void decrease_volume(ReceiverPanel *rp);
void decrease_clarifier(ReceiverPanel *rp);
void reset_clarifier(ReceiverPanel *rp);
void switch_terminal();
int get_preamp_vol_value(ReceiverPanel *rp);
void call_shutdown();


/* =======================================
 * Globals
 * =======================================*/
#define RTL_FM_CMD "rtl_fm "
#define MOD_AM     " am "
#define MOD_AM_BW  11000
#define MOD_AM_SR  11000
#define MOD_AM_PA_DS  150
#define MOD_AM_PA_QS  100
#define MOD_FM     " wbfm "
#define MOD_FM_BW  240000
#define MOD_FM_SR  32000
#define MOD_FM_PA_DS  50
#define MOD_FM_PA_QS  0
#define MOD_NFM    " fm "
#define MOD_NFM_BW 24000
#define MOD_NFM_SR 24000
#define MOD_NFM_PA_DS 0
#define MOD_NFM_PA_QS 0
#define MOD_USB    " usb "
#define MOD_USB_BW 10000
#define MOD_USB_SR 10000
#define MOD_USB_PA_QS 150
#define MOD_USB_PA_DS 200
#define MOD_LSB    " lsb "
#define MOD_LSB_BW 10120
#define MOD_LSB_SR 10120
#define MOD_LSB_PA_QS 150
#define MOD_LSB_PA_DS 200
#define DIRECT_SMP " -E direct2 "
#define DIRECT_SMP_MAX_FREQ 24.0
#define FREQ_DEFAULT_STEP_WBFM .1
#define FREQ_DEFAULT_STEP_FM   .025
#define FREQ_DEFAULT_STEP_AM   .010
#define FREQ_DEFAULT_STEP_USB  .005
#define FREQ_DEFAULT_STEP_LSB  .005
#define FREQ_MAX	   2400.000
#define FREQ_DEFAULT	   4.625
#define MODE_DEFAULT       RECEIVE_MODE_USB
#define MODE_ID_DEFAULT    4
#define LODASH_WAIT_TIME_MS 1000
int	is_rtl_fm_x_enabled=0;

/* =======================================
 * Main 
 * =======================================*/
int main(int argc, char **argv)
{
  int c = 0,
      opt = 0,
      lodash_time = 0,
      is_call_rtl_fm = 0,
      idmode = MODE_ID_DEFAULT,
      rmodes[] = {RECEIVE_MODE_AM,
      		  RECEIVE_MODE_FM,
      		  RECEIVE_MODE_WBFM,
      		  RECEIVE_MODE_LSB,
      		  RECEIVE_MODE_USB};
  clock_t tstart_time,
	  tend_time;
  
  WINDOW *mainwin=initscr();
  cbreak();
  noecho();

  ReceiverPanel *panel = create_receiver_panel(mainwin, 0, 0);
  last_rtl_fm_event = NULL;

  keypad(mainwin, TRUE);
  nodelay(mainwin, TRUE);

  tstart_time = clock();
  lodash_time = LODASH_WAIT_TIME_MS * (CLOCKS_PER_SEC/1000);


  /**
   * User options
   **/
  while ((opt = getopt(argc, argv, "e:hT")) != -1)   {
    switch (opt) 
    {
      case 'e':
	strcpy(rtl_fm_evt_file_path, optarg);
	last_rtl_fm_event = new_rtl_fm_event(NULL);
	/**
   	 * Only enabled with my rtl_fm log event
   	 * patch. Normal rtl_fm works with this app
   	 * Qut s-meter and scan will not work
   	 **/
  	start_rtl_fm_event_listener(rtl_fm_evt_file_path);
	is_rtl_fm_x_enabled = 1;
	break;

      default:
	break;
    }
  }

  init_panel(panel);
  call_rtl_fm(panel);
 
  while ((c = getch()) != KEY_F(1))
  {
    switch(c)
    {
      case KEY_UP:
 	  tstart_time = clock();
	  increase_frequency(panel);
      	  is_call_rtl_fm = 1;
	  reset_clarifier(panel);
	  tunning_status_on(panel);
	  break;

      case KEY_DOWN:
	  tstart_time = clock();
	  decrease_frequency(panel);
      	  is_call_rtl_fm = 1;
	  reset_clarifier(panel);
 	  tunning_status_on(panel);
	  break;

      case 'A':
      case 'a':
      case KEY_LEFT:
	  tstart_time = clock();
	  idmode--;
	  if(idmode < 0) idmode=4;
	  panel->receive_mode=rmodes[idmode];
	  is_call_rtl_fm = 1;
	  knob_turner(panel);
	  break;

      case 'D':
      case 'd':
      case KEY_RIGHT:
	  tstart_time = clock();
	  idmode++;
	  if(idmode > ((int)sizeof(rmodes)/(int)sizeof(int))-1) idmode=0;
	  panel->receive_mode=rmodes[idmode];
	  is_call_rtl_fm = 1;
	  knob_turner(panel);
	  break;

      case KEY_END:
	  call_shutdown();
	  break;

      case 'f':
      case 'F':
	  tstart_time = clock();
	  nodelay(mainwin, FALSE);
	  goto_frequency(panel);
	  nodelay(mainwin, TRUE);
      	  is_call_rtl_fm = 1;
	  reset_clarifier(panel);
	  knob_turner(panel);
	  break;

      case 'l':
	  tstart_time = clock();
	  nodelay(mainwin, FALSE);
	  set_squelch_level(panel);
	  nodelay(mainwin, TRUE);
	  is_call_rtl_fm = 1;
	  knob_turner(panel);
	  show_squelch_level(panel);
	  break;

      case 'L':
	  tstart_time = clock();
	  panel->sqlevel=0;
	  is_call_rtl_fm = 1;
	  break;
      case 'R':
      case 'r':
 	  tstart_time = clock();
	  is_call_rtl_fm = 1;
	  break;

      case '7':
	  panel->current_freq_step = 1000.0;
	  panel->custom_freq_step = 1;
	  break;
      case '6':
	  panel->current_freq_step = 100.0;
	  panel->custom_freq_step = 1;
	  break;
      case '5':
	  panel->current_freq_step = 10.0;
	  panel->custom_freq_step = 1;
	  break;
      case '4':
	  panel->current_freq_step = 1.0;
	  panel->custom_freq_step = 1;
          break;
      case '3':
	  panel->current_freq_step = 0.1;
	  panel->custom_freq_step = 1;
          break;
      case '2':
	  panel->current_freq_step = 0.01;
	  panel->custom_freq_step = 1;
	  break;
      case '1':
	  panel->current_freq_step = 0.001;
	  panel->custom_freq_step = 1;
	  break;
      case '0':
	  panel->custom_freq_step = 0;
	  break;
      case 'n':
      case 'N':
	  panel->volume = 50;
	  break;
      case 'm':
      case 'M':
	  panel->volume = -1;
	  decrease_volume(panel);
	  break;
      case 'p':
      case 'P':
	  if (panel->preamp_mode==0)
	  {
	    panel->preamp_mode = 1;
	    preamp_mode_on(panel);
	  }
	  else
	  {
            panel->preamp_mode = 0;
	    preamp_mode_off(panel);
	  }
	  panel->volume = 0;
	  increase_volume(panel);
	  break;
      case '=':
      case '+':
	  increase_volume(panel);
	  break;
      case '-':
	  decrease_volume(panel);
	  break;
      /* I really hate to do this, since
       * my mechanical keyboard is not
       * supported in linux and I don't
       * have the fn keys supported */
      case 'K':
	  switch_terminal();
	  break;

      case 'w':
      case 'W':
	  tstart_time = clock();
	  increase_clarifier(panel);
      	  is_call_rtl_fm = 1;
	  break;

      case 's':
      case 'S':
	  tstart_time = clock();
	  decrease_clarifier(panel);
      	  is_call_rtl_fm = 1;
	  break;
      
      case 'x':
      case 'X':
	  tstart_time = clock();
	  reset_clarifier(panel);
      	  is_call_rtl_fm = 1;
	  break;
      
  
    }

    tend_time = clock();


    if((((int)tend_time - (int)tstart_time) > lodash_time)) 
    {
       if (is_call_rtl_fm == 1) 
       {
         call_rtl_fm(panel);
         tstart_time = tend_time = clock();
         is_call_rtl_fm = 0;

	 tunning_status_off(panel);
	 volume_off(panel);
	 clarifier_off(panel);
	 redrawwin(mainwin);
       }
       else
       {
         tstart_time = tend_time = clock();
	 
	 volume_off(panel);
	 show_freq_step_scale(panel);

	 if (is_rtl_fm_x_enabled == 1)
	 {
	   panel->lcd_smeter->rms = last_rtl_fm_event->event_value;
	   update_smeter(panel->lcd_smeter);
	 }

       }      
    }
    
  }

  exit(0); 
}

/*===========================================
 * Functions
 *==========================================*/
char* build_rtl_command (char *buffer, ReceiverPanel *rp)
{
  char sub_buffer[16];
  float vfo = rp->vfo;

  strcat(buffer,"( ");
  strcat(buffer,RTL_FM_CMD);
  strcat(buffer,"-M");
  strcat(buffer,rp->rtl_mod);

  if(rp->current_clarifier != 0) 
    vfo += (float)rp->current_clarifier / 1000000.0f;

  snprintf(sub_buffer,16, "-f %.6fM ", vfo);
  strcat(buffer, sub_buffer);
  
  /* Direct sample not needed for 24Mhz or higher */
  if(rp->vfo < DIRECT_SMP_MAX_FREQ )
    strcat(buffer, DIRECT_SMP);

  snprintf(sub_buffer, 16, "-l %d ",rp->sqlevel);
  strcat(buffer, sub_buffer);
  snprintf(sub_buffer, 16,"-s %d ",rp->rtl_bw);
  strcat(buffer, sub_buffer);
  if(rp->vfo < DIRECT_SMP_MAX_FREQ )
    strcat(buffer, "-g 50 -A fast -F 9");
  else
    strcat(buffer, "-g 50 -A std ");

  if(is_rtl_fm_x_enabled == 1)
  {
    strcat(buffer, " -e ");
    strcpy(sub_buffer,rtl_fm_evt_file_path);
    strcat(buffer, sub_buffer);
  }
    
  strcat(buffer, "| aplay ");
  strcat(buffer, " -D pulse ");
  snprintf(sub_buffer, 16,"-r %d ",rp->rtl_sr);
  strcat(buffer, sub_buffer);
  strcat(buffer, "-f S16_LE -t raw ");
  strcat(buffer, ") > /dev/null 2>&1 &");
  return buffer;
}
 

void knob_turner(ReceiverPanel *rp)
{
  char mod[10] = {0};

  switch(rp->receive_mode)
  {
     case (RECEIVE_MODE_WBFM):
      strcpy(mod, MOD_FM);
      rp->rtl_mod = strdup(mod);
      rp->rtl_bw = MOD_FM_BW;
      rp->rtl_sr = MOD_FM_SR;
      rp->receive_mode = RECEIVE_MODE_WBFM;
      break;

     case (RECEIVE_MODE_FM):
      strcpy(mod, MOD_NFM);
      rp->rtl_mod = strdup(mod);
      rp->rtl_bw = MOD_NFM_BW;
      rp->rtl_sr = MOD_NFM_SR;
      rp->receive_mode = RECEIVE_MODE_FM;
      break;

     case (RECEIVE_MODE_USB):
      strcpy(mod,MOD_USB);
      rp->rtl_mod = strdup(mod);
      rp->rtl_bw = MOD_USB_BW;
      rp->rtl_sr = MOD_USB_SR;
      rp->receive_mode = RECEIVE_MODE_USB;
      break;

     case (RECEIVE_MODE_LSB):
      strcpy(mod, MOD_LSB);
      rp->rtl_mod = strdup(mod);
      rp->rtl_bw = MOD_LSB_BW;
      rp->rtl_sr = MOD_LSB_SR;
      rp->receive_mode = RECEIVE_MODE_LSB;
      break;

     default:
      strcpy(mod, MOD_AM);
      rp->rtl_mod = strdup(mod);
      rp->rtl_bw = MOD_AM_BW;
      rp->rtl_sr = MOD_AM_SR;
      rp->receive_mode = RECEIVE_MODE_AM;
  }

  change_receive_mode(rp, rp->receive_mode);
  
  change_frequency(rp, rp->vfo);
  
}

void increase_frequency(ReceiverPanel *rp)
{
 if(!rp->custom_freq_step)
   switch(rp->receive_mode)
   {
     case(RECEIVE_MODE_WBFM):
	rp->vfo += FREQ_DEFAULT_STEP_WBFM;
        break;
     case(RECEIVE_MODE_FM):
	rp->vfo += FREQ_DEFAULT_STEP_FM;
	break;
     case(RECEIVE_MODE_USB):
	rp->vfo += FREQ_DEFAULT_STEP_USB;
	break;
     case(RECEIVE_MODE_LSB):
	rp->vfo += FREQ_DEFAULT_STEP_LSB;
	break;
     default:
	rp->vfo += FREQ_DEFAULT_STEP_AM;	
   }
 else
   rp->vfo += rp->current_freq_step;

 if (rp->vfo <= 0.0) rp->vfo = 0.0;
 if (rp->vfo > FREQ_MAX) rp->vfo = FREQ_MAX;
 
 knob_turner(rp);
}

void decrease_frequency(ReceiverPanel *rp)
{


  if(!rp->custom_freq_step)
   switch(rp->receive_mode)
   {
     case(RECEIVE_MODE_WBFM):
	rp->vfo -= FREQ_DEFAULT_STEP_WBFM;
        break;
     case(RECEIVE_MODE_FM):
	rp->vfo -= FREQ_DEFAULT_STEP_FM;
	break;
     case(RECEIVE_MODE_USB):
	rp->vfo -= FREQ_DEFAULT_STEP_USB;
	break;
     case(RECEIVE_MODE_LSB):
	rp->vfo -= FREQ_DEFAULT_STEP_LSB;
	break;
     default:
	rp->vfo -= FREQ_DEFAULT_STEP_AM;	
   }
   else
     rp->vfo -= rp->current_freq_step;

   if (rp->vfo <= 0.0) rp->vfo = 0.0;
   if (rp->vfo > FREQ_MAX) rp->vfo = FREQ_MAX;

  knob_turner(rp);
}

void init_panel(ReceiverPanel *rp)
{
  rp->vfo = FREQ_DEFAULT;
  rp->receive_mode = MODE_DEFAULT;
  rp->current_freq_step= FREQ_DEFAULT_STEP_USB;
  rp->custom_freq_step=  0;
  rp->volume = 50;
  rp->preamp_mode = 0;
  rp->current_clarifier = 0;

  knob_turner(rp);

}

void call_rtl_fm(ReceiverPanel *rp) 
{
  
  char cmd[255] = { 0 }, 
       cmd3[255] = { 0 };

  /* PTBR: Evita o dedinho nervoso */
  strcpy(cmd3, cmd);
  system("killall -INT rtl_fm  > /dev/null 2>&1");
  clock_t start_time = clock();
  int milli = 1000 * (CLOCKS_PER_SEC/1000);
  while(clock() < start_time + milli)
	    ;
  /* knob_turner(rp);*/
  system("killall -INT rtl_fm > /dev/null 2>&1");
  build_rtl_command(cmd3, rp);
  system(cmd3);
  
}

void increase_volume(ReceiverPanel *rp) {
  
   char cmd[255] = {0};
   char sub_buffer[16] = {0};

   rp->volume += 10;
   if (rp->volume > 100) rp->volume=100;

   snprintf(sub_buffer, 16,"%d%%", rp->volume+get_preamp_vol_value(rp));
 
   /* strcat(cmd, "amixer sset 'Master' ");*/
   strcat(cmd, "pactl -- set-sink-volume $(pacmd list-sinks | grep \"*\" | grep \"index\" | awk '{ print $3 }') ");
   strcat(cmd, sub_buffer);
   strcat(cmd, " > /dev/null 2>&1");
   system(cmd);

   volume_on(rp);
}


void decrease_volume(ReceiverPanel *rp)
{
  char cmd[255] = {0};
   char sub_buffer[16] = {0};
    
   rp->volume -= 10;
   if (rp->volume < 0) rp->volume=0;

   if (rp->volume > 0)
      snprintf(sub_buffer, 16,"%d%%", rp->volume+get_preamp_vol_value(rp));
   else
      snprintf(sub_buffer, 16,"%d%%", rp->volume);
  /* strcat(cmd, "amixer sset 'Master' ");*/
   strcat(cmd, "pactl -- set-sink-volume $(pacmd list-sinks | grep \"*\" | grep \"index\" | awk '{ print $3 }') "); 
   strcat(cmd, sub_buffer);
   strcat(cmd, " > /dev/null 2>&1");

   system(cmd);

   volume_on(rp);
}

void switch_terminal() 
{
  system("chvt 2");

}

void call_shutdown() 
{
   /* Ok, I know, this is a not an elegant way to shutdown the system, but for RPi works great! */
   system("sudo shutdown -h now");

}

int get_preamp_vol_value(ReceiverPanel *rp)
{
 
  if(rp->preamp_mode == 0)
    return 0;

  /* Direct sample not needed for 24Mhz or higher */
  if(rp->vfo < DIRECT_SMP_MAX_FREQ )
   switch(rp->receive_mode)
   {
     case(RECEIVE_MODE_WBFM):
	return MOD_FM_PA_DS;
     case(RECEIVE_MODE_FM):
	return MOD_NFM_PA_DS;
     case(RECEIVE_MODE_USB):
	return MOD_USB_PA_DS;
     case(RECEIVE_MODE_LSB):
	return MOD_LSB_PA_DS;
     case(RECEIVE_MODE_AM):
	return MOD_AM_PA_DS;
     default:
	return 0;	
   }
  else
   switch(rp->receive_mode)
   {
     case(RECEIVE_MODE_WBFM):
	return MOD_FM_PA_QS;
     case(RECEIVE_MODE_FM):
	return MOD_NFM_PA_QS;
     case(RECEIVE_MODE_USB):
	return MOD_USB_PA_QS;
     case(RECEIVE_MODE_LSB):
	return MOD_LSB_PA_QS;
     case(RECEIVE_MODE_AM):
	return MOD_AM_PA_QS;
     default:
	return 0;	
   }
 
}


void increase_clarifier(ReceiverPanel *rp) 
{
   rp->current_clarifier += 10;

   if (rp->current_clarifier > 1000)
     rp->current_clarifier = 990;

   clarifier_on(rp);

}

void decrease_clarifier(ReceiverPanel *rp)
{
   rp->current_clarifier -= 10;
   if (rp->current_clarifier < -1000)
     rp->current_clarifier = -990;

   clarifier_on(rp);

}

void reset_clarifier(ReceiverPanel *rp)
{
   rp->current_clarifier = 0;

   clarifier_off(rp);

}
