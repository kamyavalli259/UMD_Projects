#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "event.h"
#include "calendar.h"

static int comp_duration_minutes(const void *ptr1, const void *ptr2) {
   return ((Event *)ptr1)->duration_minutes - ((Event *)ptr2)->duration_minutes;
}

int main(void) {
   int days = 2;
   Calendar *calendar;

   init_calendar("Summer Calendar", days, comp_duration_minutes, NULL, &calendar);
   print_calendar(calendar, stdout, 1);
   destroy_calendar(calendar);

   exit(EXIT_SUCCESS);
}

