/**************************************************************
@Author: Tyler Zars                                           *
@Program: events.c                                            *
@Version: 1.00                                                *
@Date: 2/1/2020                                               *
@Description: A personal calendar from a file input           *
***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Structures */
typedef struct {
  int hour;
  int minute;
} event_time_t;

typedef struct {
  int month;
  int day;
  int year;
} event_date_t;

typedef struct {
  char event_name[20];
  event_time_t event_time;
  event_date_t event_date;
} event_t;

/* Function Prototypes */
event_t* make_events(char file_name[100], int num_events);
void print_event(event_t event);
void print_selected_events(event_t events[], event_date_t start_date, event_date_t end_date);
void count_events(event_t events[]);


int main(void) {
  char user_file_input[100];
  int user_num_events = 0;

  printf("Event Database: \n\n");
  printf("Enter the name of the input file: " );
  fgets(user_file_input, 100, stdin);

  /* To Remove the \n so file can be read */
  int len = strlen(user_file_input);
  if(user_file_input[len-1]=='\n'){
    user_file_input[len-1]='\0';
  }

  /* Check for Valid Num of Events */
  do {
    printf("How many events are in the input file?: ");
    scanf("%d", &user_num_events);
  } while((user_num_events < 0)&&(user_num_events >= 99));

  /* Make Event Array for all other functions */
  event_t *events = make_events(user_file_input, user_num_events);

  /* Print All Events Individually */
  printf("\nSchedule of Events: \n\n");
  for (int i = 0; i < user_num_events; i++) {
    print_event(events[i]);
  }

  /* Hardcoded event value for checking (MM/DD/YYYY) */
  event_date_t start_date_search = {01, 01, 2018};
  event_date_t end_date_search = {12, 20, 2019};

  printf("\nDate:\t%02d/%02d/%04d\t%02d/%02d/%04d\n\n", start_date_search.month,
      start_date_search.day, start_date_search.year, end_date_search.month,
      end_date_search.day, end_date_search.year);

  printf("Events: \n\n");
  print_selected_events(events, start_date_search, end_date_search);

  printf("\nEvents:\n");
  count_events(events);
  printf("\n");

  return 0;
}

event_t* make_events(char file_name[100], int num_events){
  /* File Input & Check */
  FILE *fp1 = fopen(file_name, "r");
  if(fp1 == NULL){
      printf("File Could Not Be Read\n");
      exit(-1);
  }

  static event_t events[99]; /* Must be defined during compile, hence 99 hardcoded */

  /* Scan in Each Event */
  for (int i = 0; i < num_events; i++) {
    fscanf(fp1, "\n%s\n", events[i].event_name);
    fscanf(fp1, "\n%d%d\n", &events[i].event_time.hour, &events[i].event_time.minute);
    fscanf(fp1, "\n%d %d %d\n", &events[i].event_date.month,
      &events[i].event_date.day, &events[i].event_date.year);
  }

  fclose(fp1);

  return events;
}

void print_event(event_t event){
  /* If statement to ensure proper print formatting for short strings */
  if(strlen(event.event_name)>3){
    printf("\t%s at:\t\t %02d %02d \ton:\t%02d/%02d/%04d\n", event.event_name,
      event.event_time.hour, event.event_time.minute, event.event_date.month,
      event.event_date.day, event.event_date.year);
  } else {
    printf("\t%s at:\t\t\t %02d %02d \ton:\t%02d/%02d/%04d\n", event.event_name,
      event.event_time.hour, event.event_time.minute, event.event_date.month,
      event.event_date.day, event.event_date.year);
  }
}

void print_selected_events(event_t events[], event_date_t start_date, event_date_t end_date){
  /* Check Each To See if it Falls in Date Range */
  for (int i = 0; i < 99; i++) {
    if((events[i].event_date.year >= start_date.year)&&
      (events[i].event_date.day >= start_date.day)&&
      (events[i].event_date.year >= start_date.year)){
      if((events[i].event_date.year <= end_date.year)&&
        (events[i].event_date.day <= end_date.day)&&
        (events[i].event_date.year <= end_date.year)){
          print_event(events[i]);
      }
    }
  }
}

void count_events(event_t events[]){
  enum event_titles {
    Wedding,
    Anniversary,
    Birthday,
    Seminar,
    Formal,
    Party,
  };
  enum event_titles search;

  /* Initialize Counters */
  int wedding_ctr = 0, anniversary_ctr = 0, birthday_ctr = 0, party_ctr = 0;
  int seminar_ctr = 0, formal_ctr = 0;

  /* Set enum to correct value */
  for (int i = 0; i < 99; i++) {
    if (strcmp(events[i].event_name, "Wedding")==0) {
      search = Wedding;
    } else if (strcmp(events[i].event_name, "Anniversary")==0) {
      search = Anniversary;
    } else if (strcmp(events[i].event_name, "Birthday")==0) {
      search = Birthday;
    } else if (strcmp(events[i].event_name, "Seminar")==0) {
      search = Seminar;
    } else if (strcmp(events[i].event_name, "Formal")==0) {
      search = Formal;
    } else if (strcmp(events[i].event_name, "Party")==0) {
      search = Party;
    } else {
      continue;
    }

    /* Increment counter based off of enum */
    switch (search) {
      case Wedding:
        wedding_ctr++;
        break;
      case Anniversary:
        anniversary_ctr++;
        break;
      case Birthday:
        birthday_ctr++;
        break;
      case Seminar:
        seminar_ctr++;
        break;
      case Formal:
        formal_ctr++;
        break;
      case Party:
        party_ctr++;
        break;
    }
  }

  /* Print All Values */
  printf("\n\tWedding\t\t%d", wedding_ctr);
  printf("\n\tAnniversary\t%d", anniversary_ctr);
  printf("\n\tBirthday\t%d", birthday_ctr);
  printf("\n\tSeminar\t\t%d", seminar_ctr);
  printf("\n\tFormal\t\t%d", formal_ctr);
  printf("\n\tParty\t\t%d", party_ctr);

}
