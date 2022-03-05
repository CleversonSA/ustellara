#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>
#include<string.h>
#include"frontend.h"
#include"libreceivemode.h"
#include"libdigit.h"
#include"libvoice.h"

/************************************** 
 * MAIN 
 * ************************************/

/**************************************
 * FUNCTIONS
 * ************************************/

void speak(VoiceSettings *vc, char *msg)
{

  char cmd_buffer[300] = {0},
       cmd_kill[300] = {0},
       sbuff[10] = {0};


  strcat(cmd_kill, "killall -9 espeak ");
  strcat(cmd_kill, "> /dev/null 2>&1 ");
  

  strcat(cmd_buffer, "(espeak \"");
  strcat(cmd_buffer, msg);
  strcat(cmd_buffer, "\" ");
  strcat(cmd_buffer, "-g 10 ");

  snprintf(sbuff, 5, "%d", vc->speech_amplitude);
  strcat(cmd_buffer, "-a ");
  strcat(cmd_buffer, sbuff);
  
  strcat(cmd_buffer, ") > /dev/null 2>&1 &");

  system(cmd_kill);
  system(cmd_buffer);

}

char* build_speak_frequency(ReceiverPanel *rp)
{
  char *msg = malloc(sizeof(char) * 128);
  char sdig[4] = {0},
       sdig_clean[4] = {0};

  char digits[MAX_DIGITS+1] = { 0 };
  int i = 0;

  /* Why reeinvent the wheel? Thanks stack
   * overflow! :) */
  sprintf(digits, "%.3f", rp->vfo);
 
  while (digits[i] != '\0')
  {
     if(digits[i] != '.')
     {
       strcpy(sdig, sdig_clean);
       snprintf(sdig,4, " %c ", digits[i]);
       strcat(msg, sdig);
     }
     i++;
  }
  return msg; 
}


void speak_frequency(VoiceSettings *vc, ReceiverPanel *rp) 
{
  speak(vc, build_speak_frequency(rp));
}

char* build_speak_rec_mode(ReceiverPanel *rp)
{
  char *msg = malloc(sizeof(char) * 128);

  switch(rp->receive_mode) {
    case(RECEIVE_MODE_AM):   strcat(msg, SPEECH_AM); break;
    case(RECEIVE_MODE_WBFM): strcat(msg, SPEECH_WBFM); break;
    case(RECEIVE_MODE_FM):   strcat(msg, SPEECH_FM); break;
    case(RECEIVE_MODE_USB):  strcat(msg, SPEECH_USB); break;
    case(RECEIVE_MODE_LSB):  strcat(msg, SPEECH_LSB); break;
    default: strcat(msg, SPEECH_USB);
  }  
 
  return msg;
 
}


void speak_rec_mode(VoiceSettings *vc, ReceiverPanel *rp)
{
 
  speak(vc, build_speak_rec_mode(rp));
}


void speak_clarifier(VoiceSettings *vc, ReceiverPanel *rp)
{
  char msg[128] = {0},
       sdig[4] = {0},
       digits[5] = {0};
  int i=0;

  strcat(msg, SPEECH_CLARIFY);

  /* Why reeinvent the wheel? Thanks stack
   * overflow! :) */
  sprintf(digits, "%d", rp->current_clarifier);
 
  
  while (digits[i] != '\0')
  {
     if(digits[i] == '-')
        strcat(msg, SPEECH_MINUS);
     else if (digits[i] == '1') 
     {
	snprintf(sdig, 4, " %c ", digits[i]);
	strcat(msg, sdig);
     }
     else if (digits[i] == '2') 
     {
	snprintf(sdig, 4, " %c ", digits[i]);
	strcat(msg, sdig);
     }

    else if (digits[i] == '3') 
     {
	snprintf(sdig, 4, " %c ", digits[i]);
	strcat(msg, sdig);
     }

    else if (digits[i] == '4') 
     {
	snprintf(sdig, 4, " %c ", digits[i]);
	strcat(msg, sdig);
     }

    else if (digits[i] == '5') 
     {
	snprintf(sdig, 4, " %c ", digits[i]);
	strcat(msg, sdig);
     }

    else if (digits[i] == '6') 
     {
	snprintf(sdig, 4, " %c ", digits[i]);
	strcat(msg, sdig);
     }

    else if (digits[i] == '7') 
     {
	snprintf(sdig, 4, " %c ", digits[i]);
	strcat(msg, sdig);
     }

    else if (digits[i] == '8') 
     {
	snprintf(sdig, 4, " %c ", digits[i]);
	strcat(msg, sdig);
     }

    else if (digits[i] == '9') 
     {
	snprintf(sdig, 4, " %c ", digits[i]);
	strcat(msg, sdig);
     }

    else if (digits[i] == '0') 
     {
	snprintf(sdig, 4, " %c ", digits[i]);
	strcat(msg, sdig);
     }

     i++;
  }

    
  speak(vc, msg);

}

void speak_scan(VoiceSettings *vc)
{
   speak(vc, SPEECH_SCAN);
}


void speak_preamp(VoiceSettings *vc, ReceiverPanel *rp)
{
   char msg[125] = {0};

   if(rp->preamp_mode <= 0) 
      strcat(msg, SPEECH_DEACT); 
   else
   { 
      strcat(msg, SPEECH_PRE_AMP);
      strcat(msg, SPEECH_ACTIVE);
   }

   speak(vc, msg);
}	

void speak_cancelled(VoiceSettings *vc)
{
   speak(vc, SPEECH_DEACT);
}


void speak_frequency_status(VoiceSettings *vc, ReceiverPanel *rp)
{
   char msg[128] = {0};

   strcat(msg, build_speak_frequency(rp));
   strcat(msg, build_speak_rec_mode(rp));

   speak(vc, msg);
}
