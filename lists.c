/*****************************************************************************
* Author: Tyler Zars
* Program: lists.c
* Date: 4/28/2020
* Description: Doubly & Singly Linked List Calendar with Queue
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Unknown Size of Queue Given */
#define MAX 5000

/* Structures */
typedef struct event_date {
  int month;
  int day;
} event_date_t;

typedef struct event {
  char event_title[20];
  event_date_t event_date;
  struct event *next;
} event_t;

typedef struct event_double {
  char event_title[20];
  event_date_t event_date;
  struct event_double *next;
  struct event_double *prev;
} event_double_t;

/* Queue Global Vars */
event_t g_queue[MAX];
int queue_rear = -1;
int queue_front = -1;

/* Function Prototypes */
void insert_events_linked_list(event_t **head, event_t *new_event);
void print_linked_list(event_t *head);
void insert_events_queue(event_t in_event);
void print_queue();
void merge_lists(event_t **head, int num_positions);
void insert_doubly_list(event_double_t **head, event_double_t *new, event_double_t **tail);
void print_double_linked(event_double_t *head);
void add_left_elements_queue(event_double_t **head, event_double_t **tail);

int main(void) {
  event_t *head_node = NULL;

  /* Open File */
  FILE *fp = fopen("input_1.txt", "r");
  if(fp == NULL){
      printf("File Could Not Be Read\n");
      exit(-1);
  }

  printf("Part 1:\n\n");

  /* Fill Linked List */
  int num_linked_list = 0;
  fscanf(fp, "%d\n", &num_linked_list);
  char buffer[50];
  int date_check = 0;

  for (int i = 0; i < num_linked_list; i++) {
    event_t *new = (event_t*)malloc(sizeof(event_t));
    new->next = NULL;
    fscanf(fp, "\n%s\n", buffer);
    fscanf(fp, "\n%d %d\n", &new->event_date.month, &new->event_date.day);
    if ((new->event_date.month > 0) && (new->event_date.month < 13)) {
      if ((new->event_date.day > 0) && (new->event_date.day < 32)) {
        date_check = 1;
      }
    }
    if((strlen(buffer) < 21) && (date_check == 1)) {
      strcpy(new->event_title, buffer);
      insert_events_linked_list(&head_node, new);
    } else if(strlen(buffer) < 21) {
      printf("Title '%s' is too long. Ignored!\n", buffer);
    } else {
      printf("Date '%d/%d' is not real. Ignored!\n", new->event_date.month,
        new->event_date.day);
    }
    date_check = 0;
  }
  /* Print Filled List */
  printf("Linked List: \n");
  print_linked_list(head_node);

  /* Fill Queue */
  int num_queue = 0;
  fscanf(fp, "%d\n", &num_queue);

  for (int i = 0; i < num_queue; i++) {
    event_t new;
    fscanf(fp, "\n%s\n", buffer);
    fscanf(fp, "\n%d %d\n", &new.event_date.month, &new.event_date.day);
    if ((new.event_date.month > 0) && (new.event_date.month < 13)) {
      if ((new.event_date.day > 0) && (new.event_date.day < 32)) {
        date_check = 1;
      }
    }
    if((strlen(buffer) < 21) && (date_check == 1)) {
      strcpy(new.event_title, buffer);
      insert_events_queue(new);
    }
    date_check = 0;
  }
  /* Print Queue */
  printf("Queue:\n");
  print_queue();

  /* Merge and Reprint the Linked Lsit */
  merge_lists(&head_node, num_linked_list);
  printf("\nAfter Merging:\nList + Queue:\n");
  print_linked_list(head_node);
  printf("Queue:\n");
  print_queue();


  /* PART 2 */
  printf("\n--------------------------------\nPart 2\n\n");

  /* Head & Tail Node */
  event_double_t *head_double_node = NULL;
  event_double_t *tail_double_node = NULL;

  /* Filling Double Linked List */
  event_t *scanner = head_node;
  while(scanner){
    event_double_t *new_double = (event_double_t*)malloc(sizeof(event_double_t));
    new_double->next = NULL;
    new_double->event_date.month = scanner->event_date.month;
    new_double->event_date.day = scanner->event_date.day;
    strcpy(new_double->event_title, scanner->event_title);
    insert_doubly_list(&head_double_node, new_double, &tail_double_node);
    scanner = scanner->next;
  }
  /* Print Double Linked List */
  printf("Doubly Linked List: \n");
  print_double_linked(head_double_node);

  if (!((queue_front == -1) || (queue_front > queue_rear))) {
    printf("\nQueue: \n");
    print_queue();
    add_left_elements_queue(&head_double_node, &tail_double_node);
    printf("\nFinal List: \n");
    print_double_linked(head_double_node);
  }


  return 0;
}

/* Singly Linked List Methods */
void print_linked_list(event_t *head){
  event_t *temp = head;
  while (temp) { //Print Each Element
    if (temp->next != NULL) {
      printf("%s->", temp->event_title);
    } else {
      printf("%s\n", temp->event_title);
    }
    temp = temp->next;
  }
}

void insert_events_linked_list(event_t **head, event_t *new_event){
  event_t *temp = NULL;
  if(*head == NULL) {
    *head = new_event;
  } else {
    temp = *head;
    while(temp->next != NULL) {
      temp = temp->next;
    }
    temp->next = new_event;
  }
}

/* Queue Methods */
void insert_events_queue(event_t in_event){
  if (queue_rear == MAX - 1) {
    printf("Queue is full!\n");
  } else {
    if (queue_front == -1){
      queue_front = 0;
      queue_rear += 1;
      g_queue[queue_rear] = in_event;
    } else {
      queue_rear += 1;
      g_queue[queue_rear] = in_event;
    }
  }
}

void print_queue(){
  if ((queue_front == -1) || (queue_front > queue_rear)) {
    printf("Empty");
  } else {
    for (int i = queue_front; i <= queue_rear; i++) {
      if (i != queue_rear) {
        printf("%s->", g_queue[i].event_title);
      } else {
        printf("%s", g_queue[i].event_title);
      }
    }
  }
  printf("\n");
}

/* Merge Queue and Linked List */
void merge_lists(event_t **head, int num_positions){
  event_t *temp = *head;
  event_t *nexter = NULL;
  for (int i = 0; i < num_positions; i++) {
    event_t *new = (event_t*)malloc(sizeof(event_t));
    if (queue_front > queue_rear || queue_front == -1) {
      printf("Empty\n");
      break;
    } else {
      strcpy(new->event_title, g_queue[queue_front].event_title);
      new->event_date.month = g_queue[queue_front].event_date.month;
      new->event_date.day = g_queue[queue_front].event_date.day;
      new->next = NULL;
      nexter = temp->next;
      temp->next = new;
      new->next = nexter;
      queue_front += 1;
      temp = new->next;
    }
  }
}

/* Double List Insertion */
void insert_doubly_list(event_double_t **head, event_double_t *new, event_double_t **tail){
  event_double_t *temp = *head;

  /* Insert head node */
  if ((*head) == NULL) {
      (*head) = new;
      (*tail) = new;
      (*head)->prev = NULL;
      return;
  }

  /* Insert at front */
  if ((new->event_date.month) < ((*head)->event_date.month) ||
    (((new->event_date.month) == ((*head)->event_date.month)) &&
    (new->event_date.day) < ((*head)->event_date.day)) ||
    (((new->event_date.month) == ((*head)->event_date.month)) &&
    (new->event_date.day) == ((*head)->event_date.day) &&
    (strcmp(new->event_title, (*head)->event_title) < 0))) {
      new->prev = NULL;
      (*head)->prev = new;
      new->next = (*head);
      (*head) = new;
      return;
  }

  /* Insert at end */
  if ((new->event_date.month) > ((*tail)->event_date.month) ||
    (((new->event_date.month) == ((*tail)->event_date.month)) &&
    (new->event_date.day) > ((*tail)->event_date.day)) ||
    (((new->event_date.month) == ((*tail)->event_date.month)) &&
    (new->event_date.day) == ((*tail)->event_date.day) &&
    (strcmp(new->event_title, (*tail)->event_title) > 0))) {
      new->prev = (*tail);
      (*tail)->next = new;
      (*tail) = new;
      return;
  }

  /* Insert in middle */
  temp = (*head)->next;
  while (((temp->event_date.month) < (new->event_date.month)) ||
    (((new->event_date.month) == (temp->event_date.month)) &&
    (new->event_date.day) > (temp->event_date.day)) ||
    (((new->event_date.month) == (temp->event_date.month)) &&
    (new->event_date.day) == (temp->event_date.day) &&
    (strcmp(new->event_title, temp->event_title) > 0))) {
      temp = temp->next;
  }

  (temp->prev)->next = new;
  new->prev = temp->prev;
  temp->prev = new;
  new->next = temp;
}

void add_left_elements_queue(event_double_t **head, event_double_t **tail){
  for (int i = queue_front; i <= (queue_rear); i++) {
    event_double_t *new = (event_double_t*)malloc(sizeof(event_double_t));
    event_double_t *temp = NULL;

    strcpy(new->event_title, g_queue[queue_front].event_title);
    new->event_date.month = g_queue[queue_front].event_date.month;
    new->event_date.day = g_queue[queue_front].event_date.day;
    new->next = NULL;

    /* Insert head node */
    if ((*head) == NULL) {
        (*head) = new;
        (*tail) = new;
        (*head)->prev = NULL;
        return;
    }

    /* Insert at front */
    if ((new->event_date.month) < ((*head)->event_date.month) ||
      (((new->event_date.month) == ((*head)->event_date.month)) &&
      (new->event_date.day) < ((*head)->event_date.day)) ||
      (((new->event_date.month) == ((*head)->event_date.month)) &&
      (new->event_date.day) == ((*head)->event_date.day) &&
      (strcmp(new->event_title, (*head)->event_title) < 0))) {
        new->prev = NULL;
        (*head)->prev = new;
        new->next = (*head);
        (*head) = new;
        return;
    }

    /* Insert at end */
    if ((new->event_date.month) > ((*tail)->event_date.month) ||
      (((new->event_date.month) == ((*tail)->event_date.month)) &&
      (new->event_date.day) > ((*tail)->event_date.day)) ||
      (((new->event_date.month) == ((*tail)->event_date.month)) &&
      (new->event_date.day) == ((*tail)->event_date.day) &&
      (strcmp(new->event_title, (*tail)->event_title) > 0))) {
        new->prev = (*tail);
        (*tail)->next = new;
        (*tail) = new;
        return;
    }

    /* Insert in middle */
    temp = (*head)->next;
    while (((temp->event_date.month) < (new->event_date.month)) ||
      (((new->event_date.month) == (temp->event_date.month)) &&
      (new->event_date.day) > (temp->event_date.day)) ||
      (((new->event_date.month) == (temp->event_date.month)) &&
      (new->event_date.day) == (temp->event_date.day) &&
      (strcmp(new->event_title, temp->event_title) > 0))) {
        temp = temp->next;
    }

    (temp->prev)->next = new;
    new->prev = temp->prev;
    temp->prev = new;
    new->next = temp;

    /* Reset & Increment */
    queue_front += 1;
    temp = NULL;
    new = NULL;
  }
}

/* Print Double Linked List */
void print_double_linked(event_double_t *head){
  event_double_t *temp = head;
  while (temp) {
    printf("%s %d/%d\n", temp->event_title, temp->event_date.month,
      temp->event_date.day);
    temp = temp->next;
  }
}
