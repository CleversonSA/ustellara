
#ifndef LIBRECEIVE_MODE_H_     /* Include guard */
#define LIBRECEIVE_MODE_H_

/****************************************
 * Structures
 * *************************************/
typedef struct LCDReceiveModeSt
{
  WINDOW *am;
  WINDOW *wbfm;
  WINDOW *fm;
  WINDOW *usb;
  WINDOW *lsb;
} LCDReceiveMode;


/****************************************
 * Prototypes
 * *************************************/
void display_mode(LCDReceiveMode *lmode, int mode);
LCDReceiveMode * create_lcd_receive(WINDOW *main, int y, int x);
void display_lcd_receive_off(LCDReceiveMode *lmode);
WINDOW * get_receive_mode(LCDReceiveMode *lmode, int mode);
void set_receive_mode(LCDReceiveMode *lmode, int mode, WINDOW *wmode);

/***************************************
 * Globals
 * *************************************/
#define RECEIVE_MODE_AM   1000
#define RECEIVE_MODE_WBFM 1001
#define RECEIVE_MODE_FM   1002
#define RECEIVE_MODE_USB  1003
#define RECEIVE_MODE_LSB  1004
#define RECEIVE_COLOR_ON  3
#define RECEIVE_COLOR_OFF 4

#endif /* LIBRECEIVEMODE_H_ */
