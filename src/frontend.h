#ifndef FRONTEND_H_     /* Include guard */
#define FRONTEND_H_

#include "libdigit.h"
#include "libreceivemode.h"

/****************************************
 * Structures
 * *************************************/
typedef struct ReceiverPanelSt
{
  LCDDisplay     *lcd_vfo;
  LCDReceiveMode *lcd_mode;
  WINDOW         *wreceive_mode;
  WINDOW         *wvolume;
  WINDOW	 *wsquelch;
  WINDOW         *tunning_status;
  WINDOW         *wpreamp_mode;
  WINDOW	 *wfreq_step;
  WINDOW	 *wclarifier;
  float		 vfo;
  int		 receive_mode;
  char		 *rtl_mod;
  int            rtl_bw;
  int		 rtl_sr;
  int		 xinput;
  int            yinput;
  int            maxx;
  int            maxy;
  int		 sqlevel;
  float 	 current_freq_step;
  int		 custom_freq_step;
  int   	 volume;
  int		 preamp_mode;
  int		 current_clarifier;
} ReceiverPanel;


/****************************************
 * Globals
 * *************************************/
#define APP_VERSION "0.7.5"

/***************************************
 * Prototypes
 * *************************************/
ReceiverPanel *create_receiver_panel(WINDOW *main, int y, int x); 
void change_receive_mode(ReceiverPanel *panel, int mode);
void change_frequency(ReceiverPanel *panel, float freq);
void goto_frequency(ReceiverPanel *panel);
void set_squelch_level(ReceiverPanel *panel);
void tunning_status_on(ReceiverPanel *panel);
void tunning_status_off(ReceiverPanel *panel);
void volume_on(ReceiverPanel *panel);
void volume_off(ReceiverPanel *panel);
void show_squelch_level(ReceiverPanel *panel);
void preamp_mode_on(ReceiverPanel *panel);
void preamp_mode_off(ReceiverPanel *panel);
void show_freq_step_scale(ReceiverPanel *panel);
void clarifier_on(ReceiverPanel *panel);
void clarifier_off(ReceiverPanel *panel);

#endif /* FRONTEND_H_ */
