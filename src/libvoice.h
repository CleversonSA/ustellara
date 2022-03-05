#ifndef LIBVOICE_H_     /* Include guard */
#define LIBVOICE_H_

#include "frontend.h"
#include "libreceivemode.h"

/****************************************
 * Structures
 * *************************************/
typedef struct VoiceSettingsSt
{
   
  int speech_amplitude;

} VoiceSettings;

/****************************************
 * Globals
****************************************/
#define SPEECH_LSB     " L S B "
#define SPEECH_USB     " U S B "
#define SPEECH_AM      " A M "
#define SPEECH_WBFM    " BROADCAST "
#define SPEECH_FM      " NARROW F M "
#define SPEECH_PRE_AMP " PRE AMPLIFIER "
#define SPEECH_CLARIFY " CLARIFIER "
#define SPEECH_ACTIVE  " ON "
#define SPEECH_DEACT   " CANCELLED "
#define SPEECH_SCAN    " SCANNING BEGIN "
#define SPEECH_MINUS   " MINUS "

/***************************************
 * Prototypes
 * *************************************/
void speak(VoiceSettings *vc, char *msg);
void speak_frequency(VoiceSettings *v, ReceiverPanel *rp);
void speak_rec_mode(VoiceSettings *vc, ReceiverPanel *rp);
void speak_clarifier(VoiceSettings *vc, ReceiverPanel *rp);
void speak_scan(VoiceSettings *vc);
void speak_preamp(VoiceSettings *vc, ReceiverPanel *rp);
void speak_cancelled(VoiceSettings *vc);
char* build_speak_rec_mode(ReceiverPanel *rp);
char* build_speak_frequency(ReceiverPanel *rp);
void speak_frequency_status(VoiceSettings *vc, ReceiverPanel *rp);

#endif /* LIBVOICE_H_ */
