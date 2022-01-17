#ifndef LIBSMETER_H_     /* Include guard */
#define LIBSMETER_H_


/****************************************
 * Structures
 * *************************************/
typedef struct SmeterSt
{
  WINDOW *wsmeter_level;
  WINDOW *wsmeter_ldesc;
  WINDOW *wsmeter_rms;
  int	 rms;
  int	 level;

} Smeter;


/****************************************
 * Globals
 * *************************************/
#define SMETER_L_COLOR    5
#define SMETER_L920_COLOR 6
/* 
 * These values has no official scale,
 * it's based on rtl_fm rms return function
 */
#define S1_MIN_VALUE	25
#define S2_MIN_VALUE	50
#define S3_MIN_VALUE	75
#define S4_MIN_VALUE	100
#define S5_MIN_VALUE	125
#define S6_MIN_VALUE	150
#define S7_MIN_VALUE	175
#define S8_MIN_VALUE	225
#define S9_MIN_VALUE    250
#define S9_20_MIN_VALUE 300



/***************************************
 * Prototypes
 * *************************************/
void parse_smeter_level(Smeter *smeter);
Smeter *new_smeter_level(Smeter *smeter);
Smeter *smeter_level(WINDOW *mainwin, int y, int x);
void   update_smeter(Smeter *smeter);



#endif /* LIBSMETER_H_ */
