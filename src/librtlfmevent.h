#ifndef LIBRTLEVENT_H_     /* Include guard */
#define LIBRTLEVENT_H_


/****************************************
 * Structures
 * *************************************/
typedef struct RtlFMEventSt
{
   char  *event_type;
   int   event_value;
   float frequency;
   int   commited;
   int   last_millis;

} RtlFMEvent;


/****************************************
 * Globals
 * *************************************/
#define RTL_FM_EVT_WAIT_MS	    200
#define RTL_FM_CLEAN_EVENT_WATCHDOG_MS 10000
#define RTL_FM_EVT_PIPE_BUFFER_SIZE 32

RtlFMEvent *last_rtl_fm_event;
char       rtl_fm_evt_file_path[255];
pthread_t  rtl_fm_evt_listener;


/***************************************
 * Prototypes
 * *************************************/
void parse_rtl_fm_event(RtlFMEvent *event, char *event_data)
;
RtlFMEvent *new_rtl_fm_event(RtlFMEvent *event);
void start_rtl_fm_event_listener(char *re_path);
void *update_rtl_fm_status(void *args);
void sleep_ms(int millis);
int clean_events_watchdog(RtlFMEvent *e);


#endif /* LIBRTLEVENT_H_ */
