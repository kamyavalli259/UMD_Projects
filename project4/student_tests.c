/*
  Name: Kamyavalli Mopidevi
  UID: 118131870
  TerpConnect ID: kmopidev
  Honor Pledge: I pledge on my honor that I have not used any unauthorized
  assistance on this project or assignment.
*/

#include <stdio.h>
#include <stdlib.h>
#include "event.h"
#include "calendar.h"

/*****************************************************/
/* In this file you will provide tests for your      */
/* calendar application.  Each test should be named  */
/* test1(), test2(), etc. Each test must have a      */
/* brief description of what it is testing (this     */
/* description is important).                        */
/*                                                   */
/* You can tell whether any test failed if after     */
/* executing the students tests, you executed        */
/* "echo $?" on the command line and you get a value */
/* other than 0.  "echo $?" prints the status of     */
/* the last command executed by the shell.           */ 
/*                                                   */
/* Notice that main just calls test1(), test2(), etc.*/
/* and once one fails, the program eventually        */
/* return EXIT_FAILURE; otherwise EXIT_SUCCESS will  */
/* be returned.                                      */
/*****************************************************/

static int comp_minutes(const void *ptr1, const void *ptr2) {
  return ((Event *)ptr1)->duration_minutes - ((Event *)ptr2)->duration_minutes;
}

static int comp_name(const void *ptr1, const void *ptr2) {
  return strcmp(((Event *)ptr1)->name, ((Event *)ptr2)->name);
}

/* The test1 function is trying to check if the init calendar
   is working or not. If it is not working as expected then it will not
   print the calendar and return FAILURE.  */
static int test1() {
  int days = 7;
  Calendar *calendar;

  if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {
    if (print_calendar(calendar, stdout, 1) == SUCCESS) {
      return destroy_calendar(calendar);
    }
  }
    
  return FAILURE;
}

/*Testing if the calendar failed if the name is NULL, calendar is NULL and 
  days is less than 1. */
static int test2() {
  int days = 1;
  Calendar *calendar;
  init_calendar(NULL, days, comp_minutes, NULL, NULL);
  printf("Init calendar failed\n");
  return SUCCESS;
}


/*Trying to see if the add_event function works with invalid numbers*/
static int test4(){
  Calendar *calendar;
  void *info = NULL;
  printf("In the TEST4\n");
  if (init_calendar("Test Calendar", 30, comp_minutes, NULL, &calendar) != 
      SUCCESS) {
    printf("Failed to initialize calendar\n");
    return FAILURE;
  }
  if(add_event(calendar, "DJ Movie", 2600, -24, 
	       NULL, 1)== SUCCESS){
    printf("Add event worked with invalid numbers\n");
    return FAILURE;
  }else{
    printf("add event failed with invalid numbers\n");
    return SUCCESS;
  }
}

/*trying to test find_event IF it is finding the event or not. If cannot find 
  event return FAILURE*/
static int test5(){ 
  Calendar *calendar;
  Event *event = NULL;
  printf("In the test 5\n");
  init_calendar("Find Event", 10, comp_name, NULL, &calendar);
  add_event(calendar, "Rambo", 1000, 30, NULL, 2);
  if(find_event(calendar, "Hello", &event)== SUCCESS){
    printf("Found WRONG EVENT\n");
    return FAILURE;
  }
  else{
    printf("Did not find the event EXPECTED\n");
    return SUCCESS;
  }
}
/*testing find_event_in_a_day*/
static int test6(){
  Calendar *calendar;
  Event *event = NULL;
  printf("In the test 6\n");
  init_calendar("Test Calendar", 30, comp_minutes, NULL, &calendar);
  add_event(calendar, "Event1", 1000, 60, NULL, 1);
  /*find event on Invalid day*/
  if(find_event_in_day(calendar, "Event1", 0, &event)!= FAILURE || event!=NULL){
    printf("Event found on WRONG DAY\n");
    destroy_calendar(calendar);
    return FAILURE;
  }
  /*find event on Invalid day with number of days greater than the calendar*/
  if(find_event_in_day(calendar, "Event1", 32, &event)!=FAILURE || event!=NULL){
    printf("event found with number of days greater than calendar\n");
    destroy_calendar(calendar);
    return FAILURE;
  }
  destroy_calendar(calendar);
  return SUCCESS;
}

/*function is testing the remove_event function. Trying to see if it can
  remove an existing event which is expected. If it removes a event
  that is not added then the function returns FAILURE because it not 
  expected.  */
static int test7(){
  Calendar *calendar;
  printf("In test 7\n");
  
  if(init_calendar("flower", 60, comp_minutes, NULL, &calendar)!= SUCCESS){
    printf("Initilaization failed\n");
    return FAILURE;  
  }
  if(add_event(calendar, "Event1", 1000, 60, NULL, 1)!= SUCCESS){
    printf("unable to add-event\n");
    destroy_calendar(calendar);
    return FAILURE;
  }
  if(remove_event(calendar, "Event1")!= SUCCESS){
    printf("unanle to remove existing event\n");
    destroy_calendar(calendar);
    return FAILURE;
  }
  printf("tEST 7\n");
  if (find_event_in_day(calendar, "Event1", 1, NULL) != FAILURE) {
    printf("Failed: Event still exists in calendar.\n");
    destroy_calendar(calendar);
    return FAILURE;
  }
  printf("Test 1 passed: Existing event successfully removed.\n");

  /*Now trying to remove non-existing event*/
  if(remove_event(calendar, "None")!=FAILURE){
    printf("FAILED, remove none event\n");
    destroy_calendar(calendar);
    return FAILURE;
  }
  printf("Non existing event not removed successful\n");

  /*Now trying to remove an event with NULL name*/
  if(remove_event(NULL, "Event1")!=FAILURE){
    printf("FAILED, removed calendar with NULL\n");
    destroy_calendar(calendar);
    return FAILURE;
  }
  printf("3. NULL calendar NOT removed successful\n");

  printf("Test 4: Removing an event with a NULL name...\n");
  if (remove_event(calendar, NULL) != FAILURE) {
    printf("Fail: Removed event with NULL name.\n");
    destroy_calendar(calendar);
    return FAILURE;
  }
  printf("NULL event not removed.\n");
  destroy_calendar(calendar);
  return SUCCESS;
}
 
/*get_event_info being tested. The function is trying to verify that 
 infomation for an existing event can be correctly retrieved. Second, 
 trying to ensure that the asking for info on an non-existing event 
 correctly returns NULL*/
static int test8(){
  Calendar *calendar;
  void *info;
  init_calendar("Lily Tillu", 30, comp_minutes, NULL, &calendar);
  add_event(calendar, "Tillu", 1000, 60, (void *)"Info1", 1);
  info = get_event_info(calendar, "Tillu");
  printf("In Test8\n");
  if (info == NULL || strcmp((char *) info, "Info1") != 0) {
    printf(" failed: Incorrect or missing event info.\n");
    destroy_calendar(calendar);
    return FAILURE;
  }
  printf("passed correct info received from file.\n");

  /*Get info of a non-existing event*/
  info = get_event_info(calendar, "NonExistingEvent");
  if (info != NULL) {
    printf("failed: Retrieved info for non-existing event.\n");
    destroy_calendar(calendar);
    return FAILURE;
  }
  printf("Test 2 passed: NULL returned for non-existing event.\n");
  destroy_calendar(calendar);
  return SUCCESS;
}
/*testing clear calendar*/
static int test9(){
  Calendar *calendar;
  int i;
  init_calendar("Lily Tillu", 30, comp_minutes, NULL, &calendar);
  add_event(calendar, "Rose", 1000, 60, (void *)"Info1", 1);
  add_event(calendar, "Jasmine", 1010, 80, (void *)"Info2", 2);

  /*clearing a non-empty calender*/
  if(clear_calendar(calendar)!= SUCCESS){
    printf("Unable to clear FAILED\n");
    destroy_calendar(calendar);
    return FAILURE;

  }
  printf("I am in Test9\n");
  printf("clear empty calendar passed\n");
  /*check if the calendar cleared is empty*/
  for(i=0; i< calendar->days; i++){
    if(calendar->events[i]!=NULL){
      printf("fail: calendar not cleared\n");
      return FAILURE;
    }
    printf("calendar clear\n");
    destroy_calendar(calendar);
    return SUCCESS;
  }
}

/*testing the clear day function*/
static int test10(){
  Calendar *calendar;
  init_calendar("test calendar", 30, comp_minutes, NULL, &calendar);
  add_event(calendar, "Rose", 1000, 60, (void *)"Info1", 1);
  add_event(calendar, "Jasmine", 1010, 80, (void *)"Info2", 2);
  printf("in test10\n");
  /*clearing events for specific day*/
  if(clear_day(calendar, 1) != SUCCESS){
    printf("failed: unable to clear events of day1\n");
    destroy_calendar(calendar);
    return FAILURE;
  }

  /*CLEARING EVNTS IF CALENDAR IS NULL*/
  if(clear_day(NULL, 1)!= FAILURE){
    printf("failed cleared events for NULL calendar");
    return FAILURE;
  }
  printf("success: NULL CALENDAR not cleared\n");
  destroy_calendar(calendar);
  return SUCCESS;
}

/*testing destroy_calendar*/
static int test11(){
  Calendar *calendar;
  init_calendar("test calendar", 30, comp_minutes, NULL, &calendar);
  add_event(calendar, "Rose", 1000, 60, (void *)"Info1", 1);
  add_event(calendar, "Jasmine", 1010, 80, (void *)"Info2", 2);
  printf("In test 11 of destroy_calendar\n");
  if(destroy_calendar(calendar)!= SUCCESS){
    printf("failed, not cleared calender\n");
    return FAILURE;
  }
  printf("calendar destroyed\n");

  /*destroying a NULL calendar*/
  if(destroy_calendar(NULL)!= FAILURE){
    printf("fail: destroyed a NULL calendar\n");
    return FAILURE;
  }
  printf("success:not cleared a null calendar\n");
  return SUCCESS;
}

/*trying to see if the add_events adds the duplicate event by name or 
  return NULL when it encounters an event added by the same name previously
  and need to add it again. Expected should be return NULL. */
static int test12()
{
  Calendar *calendar;
  printf("In the TEST12\n");
  if (init_calendar("Test Calendar", 5, comp_name, NULL, &calendar) !=
      SUCCESS)
    {
      return FAILURE;
    }
  printf("In test12\n");
  add_event(calendar, "DJ Movie", 2300, 21, NULL, 1);
  if(add_event(calendar, "DJ Movie", 2100, 20, NULL, 1)== SUCCESS){
    printf("ERROR: added duplicate\n");
    return FAILURE;
}
  add_event(calendar, "lily", 2000, 60, NULL, 2);
  print_calendar(calendar, stdout, 1);
  destroy_calendar(calendar);
  printf("success, no duplicate added\n");
  return SUCCESS;
}

int main() {
  int result = SUCCESS;

  if (test1() == FAILURE) result = FAILURE;
  if (test2() == FAILURE) result = FAILURE;
  if(test4()== FAILURE) result = FAILURE;
  if(test5()== FAILURE) result = FAILURE;
  if(test6()== FAILURE) result = FAILURE;
  if(test7()== FAILURE) result = FAILURE;
  if(test8() == FAILURE) result = FAILURE;
  if(test9() == FAILURE) result = FAILURE;
  if(test10() == FAILURE) result = FAILURE;
  if(test11() == FAILURE) result = FAILURE;
  if(test12()== FAILURE) result = FAILURE;
  if (result == FAILURE) {
    exit(EXIT_FAILURE);
  }
  return EXIT_SUCCESS;
}
