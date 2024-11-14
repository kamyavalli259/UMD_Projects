/*
  Name: Kamyavalli Mopidevi
  UID: 118131870
  TerpConnect ID: kmopidev
  Honor Pledge: I pleadge on my honor that I have not used
  any unauthorized assistance on this project.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "event.h"
#include "calendar.h"

/*The function initializes the calendar structure based on parameters provided.
  The function allocates memory for Calendar structure, memory necessary to
  store the name of the calendar parameter, and Events field necessary to
  represent an array of pointers to Event structure.*/
int init_calendar(const char *name, int days,
                  int (*comp_func)(const void *ptr1, const void *ptr2),
                  void (*free_info_func)(void *ptr), Calendar **calendar)
{
  int length, return_value = SUCCESS;
  if (calendar == NULL || name == NULL || days < 1)
    {
      return FAILURE;
    }

  /*allocate memory for the calendar struct*/
  *calendar = malloc(sizeof(Calendar));
  if (*calendar == NULL)
    {
      return FAILURE;
    }
  length = strlen(name);
  (*calendar)->name = malloc(length + 1);
  if ((*calendar)->name == NULL)
    {
      return FAILURE;
    }
  strcpy((*calendar)->name, name);
  (*calendar)->events = calloc(days, sizeof(Event *));
  (*calendar)->days = days;
  (*calendar)->total_events = 0;
  (*calendar)->comp_func = comp_func;
  (*calendar)->free_info_func = free_info_func;
  return return_value;
}

/*This function prints, to the designated output stream, the calendar’s name,
  days, and total number of events if print all is true; otherwise this
  information will not be printed.*/
int print_calendar(Calendar *calendar, FILE *output_stream, int print_all)
{
  int i;
  Event *current;
  if (output_stream == NULL || calendar == NULL)
    {
      return FAILURE;
    }
  if (print_all == 1)
    {
      fprintf(output_stream, "Calendar's Name: \"%s\"\n", calendar->name);
      fprintf(output_stream, "Days: %d\n", calendar->days);
      fprintf(output_stream, "Total Events: %d\n", calendar->total_events);
    }
  fprintf(output_stream, "\n**** Events ****\n");
  for (i = 0; i < calendar->days; i++)
    {
      current = calendar->events[i]; /*begining of the linked list*/
      if (current != NULL || calendar->total_events != 0)
	fprintf(output_stream, "Day %d\n", i + 1);

      while (current != NULL)
        {
   fprintf(output_stream, "Event's Name: \"%s\", Start_time: %d, Duration: %d\n",
           current->name, current->start_time, current->duration_minutes);
	  current = current->next;
        }
    }
  return SUCCESS;
}

/*Function adds the specified event to the list associated with the day
  parameter. The event must be added in increasing sorted order using the 
  comparison function (comp_fun) that allows comparison of two events.
  The function will return FAILURE if calendar, name is 
  NULL, start time in invalid, duration minutes less than or equal to zero, 
  day is less than 1 or greater than the number of calendar days, 
  if the event already exists, or if any 
  memory allocation fails. */
int add_event(Calendar *calendar, const char *name, int start_time,
              int duration_minutes, void *info, int day)
{
  Event *event, *current;
  int i;

  if (calendar == NULL || name == NULL || start_time < 0 || start_time >2400
      || duration_minutes <= 0 || day < 1 || day > calendar->days)
    {
      return FAILURE;
    }
  event = malloc(sizeof(Event));
  if (event == NULL)
    {
      return FAILURE;
    }
  for(i =0; i< calendar->days; i++){
    current = calendar->events[i];
    while(current!=NULL){
      if(strcmp(current->name, name)==0){
	return FAILURE;
      }
      current = current->next;
    }
  }
  event->start_time = start_time;
  event->duration_minutes = duration_minutes;
  event->name = malloc(strlen(name) + 1);
  event->info = info;
  event->next = NULL;
  strcpy(event->name, name);
  current = calendar->events[day - 1];
  if (current == NULL)
    {
      calendar->events[day - 1] = event;
      calendar->total_events++;
      return SUCCESS;
    }
  if (calendar->comp_func(event, current) < 0)
    {
      event->next = current;
      calendar->events[day - 1] = event;
    }
  else
    {
  while (current->next != NULL && 
         calendar->comp_func(event, current->next) > 0)
        {
	  current = current->next;
        }
      event->next = current->next;
      current->next = event;
    }
  calendar->total_events++;
  return SUCCESS;
}
/*This function will return a pointer (via the out parameter event) to the 
  event with the specified name(if any). If the event parameter is NULL, 
  no pointer will be returned. The function will fail and return FAILURE 
  if calendar is NULL or name is NULL. The function will return SUCCESS 
  if the event was found and FAILURE otherwise.*/
int find_event(Calendar *calendar, const char *name, Event **event)
{
  int i;
  Event *current;
  if (calendar == NULL || name == NULL)
    {
      return FAILURE;
    }
  for (i = 0; i < calendar->days; i++)
    {
      current = calendar->events[i];
      /*to traverse the linked list of particular day*/
      while (current != NULL)
        {
	  if (strcmp(current->name, name) == 0)
            {
	      *event = current;
	      return SUCCESS;
            }
	  current = current->next;
        }
    }
  return FAILURE;
}

/*This function will return a pointer (via the out parameter event) to the 
  event with the specified name(if any). If the event parameter is NULL, 
  no pointer will be returned. The function will fail and return FAILURE if 
  calendar is NULL or name is NULL, or if
  the day parameter is less than 1 or greater than the number of calendar days. 
  The function will return SUCCESS if the event was found and FAILURE 
  otherwise.*/
int find_event_in_day(Calendar *calendar, const char *name, int day,
                      Event **event)
{
  Event *current;
  if (calendar == NULL || name == NULL || day < 1 || day > calendar->days ||
      event == NULL)
    {
      return FAILURE;
    }
  current = calendar->events[day - 1];
  while (current != NULL)
    {
      if (strcmp(current->name, name) == 0)
        {
	  *event = current;
	  return SUCCESS;
        }
      current = current->next;
    }
  return FAILURE;
}

/*This function will remove the specified event from the calendar returning 
  any memory allocated for the event. If the event has an info field other
  than NULL and a free_info_func exists, the function will be called 
  on the info field. The number of calendar events
  must be adjusted accordingly. The function will fail and return FAILURE 
  if calendar is NULL or name is NULL or if the event cannot be found; 
  otherwise the function will return SUCCESS*/
int remove_event(Calendar *calendar, const char *name)
{
  Event *current, *temp;
  int i;
  if (calendar == NULL || name == NULL)
    {
      return FAILURE;
    }
  for (i = 0; i < calendar->days; i++)
    {
      current = calendar->events[i];
      if (current != NULL && strcmp(current->name, name) == 0)
        {
	  if (current->info != NULL && calendar->free_info_func != NULL)
            {
	      calendar->free_info_func(current->info);
            }
	  free(current->name);
	  calendar->events[i] = current->next;
	  free(current);
	  calendar->total_events--;
	  return SUCCESS;
        }
      while (current != NULL && current->next != NULL)
        {
	  if (strcmp(current->next->name, name) == 0)
            {
	      if (current->next->info != NULL && 
                  calendar->free_info_func != NULL)
                {
		  calendar->free_info_func(current->next->info);
                }
	      free(current->next->name);
	      temp = current->next;
	      current->next = current->next->next;
	      free(temp);
	      calendar->total_events--;
	      return SUCCESS;
            }
	  current = current->next;
        }
    }
  return FAILURE;
}

/*This function returns the info pointer associated with the specified event. 
  The function returns NULL if the event is not found. */
void *get_event_info(Calendar *calendar, const char *name)
{
  Event *event;
  int number;
  number = find_event(calendar, name, &event);
  if (number == FAILURE)
    {
      return NULL;
    }
  /*return info pointer*/
  return event->info;
}

/*This function will remove all the event lists associated with the caledar 
  and set them to empty lists. The array holding the events will not be 
  removed. The total number of events will be set to 0.If the event has an 
  info field other than NULL and a free_info_func exists, the function 
  will be called on the info field. This function will
  fail and return FAILURE if calendar is NULL; 
  otherwise the function will return SUCCESS*/
int clear_calendar(Calendar *calendar)
{
  int i;
  Event *current, *next;
  if (calendar == NULL)
    {
      return FAILURE;
    }
  /*go through each of the day*/
  for (i = 0; i < calendar->days; i++)
    {
      current = calendar->events[i];
      while (current != NULL)
        {
	  next = current->next;
	  if (current->info != NULL && calendar->free_info_func != NULL)
            {
	      calendar->free_info_func(current->info);
            }
	  free(current->name);
	  free(current);
	  current = next;
        }
      calendar->events[i] = NULL;
    }
  calendar->total_events = 0;
  return SUCCESS;
}

/*This function will remove all the events for the specified day setting 
  the event list to empty. The total number of events will be adjusted
  accordingly.If the event has an info field other than NULL and a 
  free_info_func exists, the function will be called on the info field.
  The function will fail and return FAILURE if the calendar is NULL or day 
  is less than 1 or day is less than the days in calendar; 
  otherwise return SUCCESS*/
int clear_day(Calendar *calendar, int day)
{
  Event *current, *next;
  if (calendar == NULL || day < 1 || day > calendar->days)
    {
      return FAILURE;
    }
  current = calendar->events[day - 1];
  while (current != NULL)
    {
      next = current->next;
      if (current->info != NULL && calendar->free_info_func != NULL)
        {
	  calendar->free_info_func(current->info);
        }
      calendar->total_events--;
      free(current->name);
      free(current);
      current = next;
    }
  calendar->events[day - 1] = NULL;
  return SUCCESS;
}
/*This function will return memory that was dynamically-allocated for the 
  calendar.
  If the event has an info field other than NULL and a free_info_func
  exists, the function will be called on the info field. This function will
  fail and return FAILURE if calendar is NULL; 
  otherwise the function will return SUCCESS.*/
int destroy_calendar(Calendar *calendar)
{
  if (calendar == NULL)
    {
      return FAILURE;
    }
  clear_calendar(calendar);
  free(calendar->events);
  free(calendar->name);
  free(calendar);
  return SUCCESS;
}
