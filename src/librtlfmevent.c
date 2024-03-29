#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>
#include<string.h>
#include"librtlfmevent.h"

/************************************** 
 * MAIN 
 * ************************************/
/*void main(void)
{
  
  last_rtl_fm_event = new_rtl_fm_event(NULL);
  
  start_rtl_fm_event_listener("./elogs.txt");


  while(1)
  {
    if(last_rtl_fm_event->commited) {
      fprintf(stderr, "Evt Type = [%s] ", last_rtl_fm_event->event_type);
      fprintf(stderr, "Evt value = [%d] ", last_rtl_fm_event->event_value);
      fprintf(stderr, "Evt freq = [%.6f] \n", last_rtl_fm_event->frequency);

      sleep_ms(RTL_FM_EVT_WAIT_MS);
    }
  }

   * WWWWWWW   It almost drove me mad! When I
   * d  O O b  use pthread, the printf stopped
   *  \  O/    working! So I lost 1 hour to
   *           notice that you have to use 
   *           fprintf(stderr) to show logs...

   exit(0);     
}*/

/**************************************
 * FUNCTIONS
 * ************************************/

void parse_rtl_fm_event(RtlFMEvent *rtl_fm_evt, char *event_data)
{


    rtl_fm_evt->commited = 0;
    char delimiter[] = "|";
    char *buffer = 0;

    rtl_fm_evt->event_type = "";
    rtl_fm_evt->event_value = 0;

    if (event_data == 0) {

      rtl_fm_evt->event_type = "";
      rtl_fm_evt->event_value = 0;
      rtl_fm_evt->frequency = 0.0;
      rtl_fm_evt->commited = 1;
      return;

    }

    // Get the first token
    char *token = strtok(event_data, delimiter);
 
    // Event type
    if (token != 0) {

      buffer = malloc(sizeof(char) * 10);
      *buffer = '\0';
      strcpy(buffer, token);
      rtl_fm_evt->event_type = buffer;
      free(buffer);
      token = strtok(NULL, delimiter);

    }
    
    /* Parse event value */
    if (token != 0) {

      buffer = malloc(sizeof(char)*10);
      *buffer = '\0';
      strcpy(buffer, token);
      rtl_fm_evt->event_value = atoi(buffer);
      free(buffer);
      token = strtok(NULL, delimiter);

    } else {

      rtl_fm_evt->event_value = 0;

    }

    /* Parse event freq */
    if (token != 0) {

      buffer = malloc(sizeof(char)*10);
      *buffer = '\0';
      strcpy(buffer, token);
      rtl_fm_evt->frequency = atoi(buffer);
      free(buffer); 

    } else {

      rtl_fm_evt->frequency = 0.00;

    }
    
    rtl_fm_evt->commited = 1;
}


void *update_rtl_fm_status(void *args)
{
  char cmd_buffer[300] = {0},
       buffer_cleaned[RTL_FM_EVT_PIPE_BUFFER_SIZE] = {0},
       buffer[RTL_FM_EVT_PIPE_BUFFER_SIZE] = {0};
  char *clean_buffer;
  int  i=0,
       default_exit=-1;


  strcat(cmd_buffer, "tail -n 1 ");
  strcat(cmd_buffer, rtl_fm_evt_file_path);
  
  while(1)
  {
    /* Simple check if logf file exists */
    FILE *fp = fopen(rtl_fm_evt_file_path, "r");
    if (fp == NULL)
      continue;
    else
      fclose(fp);

    last_rtl_fm_event = new_rtl_fm_event(last_rtl_fm_event);
   
    FILE *pipe = popen(cmd_buffer, "r");
    
    strcpy(buffer, buffer_cleaned);

    if(fread(buffer, RTL_FM_EVT_PIPE_BUFFER_SIZE, 1, pipe) == 0)
    {
       i = 0;
       clean_buffer = buffer;

       for (i=0; i<RTL_FM_EVT_PIPE_BUFFER_SIZE; i++){
          if(buffer[i] == '\n')
	        break;
          *clean_buffer=buffer[i]; 
          clean_buffer++;
        }

        *clean_buffer='\0';
        clean_buffer=buffer;

  	    parse_rtl_fm_event(last_rtl_fm_event, clean_buffer);
    } 
    pclose(pipe);
  

    sleep_ms(RTL_FM_EVT_WAIT_MS);
  }
  
  pthread_exit(&default_exit);
}



void start_rtl_fm_event_listener(char *re_file_path)
{
   int t_c_status = 0;

   strcpy(rtl_fm_evt_file_path, re_file_path);

   /* Simple check if logf file exists 
   FILE *fp = fopen(rtl_fm_evt_file_path, "r");
   if (fp == NULL)
   {
      last_rtl_fm_event = NULL;
      return;
   }
   else
      fclose(fp);*/

   t_c_status = pthread_create (&rtl_fm_evt_listener, NULL, update_rtl_fm_status,(void *)(NULL));
   if(t_c_status != 0)
   {
     fprintf(stderr, "Could'n possible to start thread\n");
   }
   else
   {
      /* Try avoid blocking */
      if(pthread_detach(rtl_fm_evt_listener)) {
	      fprintf(stderr, "ERROR: Could not deatch thread\n");
      }
   }

}


RtlFMEvent *new_rtl_fm_event(RtlFMEvent *event)
{
 
  /**
   *  ....   When you create the code that
   * d oO b  runs fine in your mind but
   *  \o /   get a annoying "Segmentation fault" 
   *         The answer? Never use a pointer before 
   *         initialize it, wherever operation
   *         you plan to do! It took me two ours
   *         and a sleep time to realize that.
   */
  if (event == NULL) 
  {
     event = malloc(sizeof(RtlFMEvent));
     event->last_millis = 0;
  }
  

  /**
   * mmmmmmm  In C, unlike Java, Pyhton, NodeJS
   * d - - b  you have to initialize the pointer
   *  \ + /   s by yourself method
   */
  event->event_type = NULL;
  event->event_value= -1;
  event->frequency  = -1;
  event->commited   = 0;

  return event;
}

void sleep_ms(int millis)
{
  clock_t start_time = clock();
  int milli = millis * (CLOCKS_PER_SEC/1000);
  while(clock() < (int)start_time + milli)
	  ;

}


int clean_events_watchdog(RtlFMEvent *e)
{
	clock_t current_millis = clock();
	int millis_size = RTL_FM_CLEAN_EVENT_WATCHDOG_MS * (CLOCKS_PER_SEC / 1000);


	if (e->last_millis == 0) {
		e->last_millis = (int)current_millis;
		return 0;
	}

	if ((int)current_millis > (e->last_millis + millis_size)) {
		e->last_millis = (int)current_millis;
		return 1;
	}

	return 0;
	
}


