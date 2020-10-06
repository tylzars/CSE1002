/**************************************************************
@Author: Tyler Zars                                           *
@Program: list.c                                              *
@Version: 1.00                                                *
@Date: 4/8/2020                                               *
@Description: Personal calendar from file redirection         *
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

typedef struct event {
  char event_title[20];
  event_time_t event_time;
  event_date_t event_date;
  struct event *next;
} event_t;

/* Function Prototypes */
void make_events(event_t **head_ptr);
void print_list(event_t *head);
void print_selected_events(event_t *head, event_date_t date);


int main(void) {
  /* Declare Head of List */
  event_t *head_node = NULL;

  /* Fill list from file */
  make_events(&head_node);

  /* Print All Events */
  print_list(head_node);

  /* Hardcoded event value for checking (MM/DD/YYYY) */
  event_date_t date_search = {06, 15, 2018};

  /* Print Events on this Day */
  printf("\nDate:\t%02d/%02d/%04d\n", date_search.month, date_search.day,
    date_search.year);
  printf("Events:\n");
  print_selected_events(head_node, date_search);

  return EXIT_SUCCESS; //Blast-off
}

void make_events(event_t **head_ptr){
  /* Extra Vars */
  char buffer[20];
  int i = 0;

  /* This while will run until file ends */
  while(fgets(buffer, 20, stdin)!=NULL){
    event_t *new = NULL, *prev = NULL, *curr = *head_ptr;

    /* Malloc for new event */
    new = (event_t*)malloc(sizeof(event_t));
    prev = (event_t*)malloc(sizeof(event_t));

    /* String Editing */
    int str_len = strlen(buffer);
    if(buffer[str_len-1]=='\n'){
      buffer[str_len-1]='\0';
    }
    strcpy(new->event_title, buffer);

    /* Date and Time Setting */
    scanf("\n%d%d\n", &new->event_time.hour, &new->event_time.minute);
    scanf("\n%d %d %d\n", &new->event_date.month, &new->event_date.day,
      &new->event_date.year);
    new->next = NULL;

    /* Insertsion */
    while(curr != NULL && strcmp(new->event_title,curr->event_title)>0){
      prev = curr;   /* Find place to insert */
      curr = curr->next;
    }
    if(prev == NULL || (*head_ptr) == NULL){ /* Insert at beginning */
      new->next = curr;
      (*head_ptr) = new;
    } else if (strcmp((*head_ptr)->event_title,new->event_title)>=0) {
      new->next = *head_ptr; /* Insert before head */
      *head_ptr = new;
    } else { /* Insert between the two */
      prev->next = new;
      new->next = curr;
    }
    i++;
  }
}

void print_list(event_t *head){
  /* Print all events & make sure formatting is correct */
  printf("Schedule of Events: \n");
  while (head) {
    if(strlen(head->event_title)>3){
      printf("\t%s  \tat: %02d:%02d \ton: %02d/%02d/%04d\n", head->event_title,
        head->event_time.hour, head->event_time.minute, head->event_date.month,
        head->event_date.day, head->event_date.year);
    } else {
      printf("\t%s\t\t\tat: %02d:%02d \ton: %02d/%02d/%04d\n",
        head->event_title, head->event_time.hour, head->event_time.minute,
        head->event_date.month, head->event_date.day, head->event_date.year);
    }
    head = head->next;
  }
}

void print_selected_events(event_t *head, event_date_t date){
  /* Scan every event to see if it is on 'date' and print accordingly */
  while (head) {
    if ((head->event_date.month == date.month)&&
      (head->event_date.day == date.day)&&
      (head->event_date.year == date.year)) {
        if(strlen(head->event_title)>3){
          printf("\t%s  \tat: %02d:%02d\n", head->event_title,
            head->event_time.hour, head->event_time.minute);
        } else {
          printf("\t%s\t\tat: %02d:%02d\n",
            head->event_title, head->event_time.hour, head->event_time.minute);
        }
    }
    head = head->next;
  }
}
