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
  float		 vfo;
  int		 receive_mode;
  char		 *rtl_mod;
  int            rtl_bw;
  int		 rtl_sr;
  int		 xinput;
  int            yinput;
  int            maxx;
  int            maxy;
} ReceiverPanel;


/****************************************
 * Globals
 * *************************************/

/***************************************
 * Prototypes
 * *************************************/
ReceiverPanel *create_receiver_panel(WINDOW *main, int y, int x); 
void change_receive_mode(ReceiverPanel *panel, int mode);
void change_frequency(ReceiverPanel *panel, float freq);
void goto_frequency(ReceiverPanel *panel);

#endif /* FRONTEND_H_ */
