/**************************************************************
@Author: Tyler Zars                                           *
@Program: battleship.c                                        *
@Version: 1.00                                                *
@Date: 2/1/2020                                               *
@Description: Player v.s. Computer Battleship Game            *
***************************************************************/

#include <stdio.h>
#include <stdlib.h>

/* Global Var For Functions */
int g_map_cols = 0;
int g_map_rows = 0;
int g_shot_counter = 1;
int g_num_ships_hit = 0;

/* Fuction Prototypes */
void read_map(FILE *fp, char master[][g_map_cols]);
void print_map(char arr[][g_map_cols]);
void play_game(char master[][g_map_cols], char player[][g_map_cols], int max_shots);
int input_check(char master[][g_map_cols], char player[][g_map_cols], int in_rows, int in_cols);
int num_ships(char master[][g_map_cols]);
int map_checker(char master[][g_map_cols]);

int main(int argc, char const *argv[]) {
    /* Set Max Number of Shots */
    int shot_max = atoi(argv[1]);

    /* Input File From Command Line */
    FILE *fp1 = fopen(argv[2], "r");

    /* File is Valid Check */
    if(fp1 == NULL){
        printf("File Could Not Be Read\n");
        exit(-1);
    }

    /* Read Rows / Cols from File */
    fscanf(fp1, "%d %d", &g_map_rows, &g_map_cols);

    char player_map[g_map_rows][g_map_cols];
    char master_map[g_map_rows][g_map_cols];

    /* Fill Player Map to be Blank */
    for (int rows = 0; rows < g_map_rows; rows++) {
      for (int cols = 0; cols < g_map_cols; cols++) {
        player_map[rows][cols]='~';
      }
    }

    /* Fill Master Map */
    read_map(fp1, master_map);

    /* Close File Pointer */
    fclose(fp1);

    /* Find Max Ships */
    int number_ships = num_ships(master_map);

    /* Make sure map only contains '~' and 'B' */
    if (map_checker(master_map)==-1) {
      printf("Map contains errors, please try with a new map\n");
      exit(-1);
    }

    /* Make sure more shots than ships */
    if (number_ships>shot_max) {
      printf("Not enough shots to win, please try again!\n");
      exit(-1);
    }


    while(g_shot_counter <= shot_max+1){
      /* Lose Condition */
      if(g_shot_counter==shot_max+1){
        printf("Map\n\n");
        print_map(player_map);
        printf("Captain, we ran out of ammo. You lose!\n");
        exit(0);
      }
      play_game(master_map, player_map, shot_max);
      /* Win Condition */
      if(g_num_ships_hit==number_ships){
        printf("Map\n\n");
        print_map(player_map);
        printf("Captain, we have sunk all the battleships. You win!\n");
        exit(0);
      }
    }


    printf("\n");
    return 0;
}

void read_map(FILE *fp, char master[][g_map_cols]){
  for (int rows = 0; rows < g_map_rows; rows++){
		for (int cols = 0; cols < g_map_cols; cols++){
			fscanf(fp, "\n%c \n\t", &master[rows][cols]);
      /* (\n & \t) swallows the newline/tab character not taking up space in array */
		}
	}
}

int num_ships(char master[][g_map_cols]){
  int ship_count = 0;
  for (int i = 0; i < g_map_rows; i++) {
    for (int j = 0; j < g_map_cols; j++) {
      if (master[i][j]=='B') {
        ship_count++;
      }
    }
  }
  return ship_count;
}

int map_checker(char master[][g_map_cols]){
  for (int rows = 0; rows < g_map_rows; rows++) {
    for (int cols = 0; cols < g_map_cols; cols++) {
      if ((master[rows][cols]!='B')&&(master[rows][cols]!='~')) {
        return -1;
      }
    }
  }
  return 1;
}

void print_map(char arr[][g_map_cols]){
  for (int rows = 0; rows < g_map_rows; rows++){
		for (int cols = 0; cols < g_map_cols; cols++){
			printf("%c", arr[rows][cols]);
		}
  printf("\n");
	}
}

void play_game(char master[][g_map_cols], char player[][g_map_cols], int max_shots){
  /* Print Map & Shots */
  printf("Map\n\n");
  print_map(player);
  printf("%d shots remaining.\n", max_shots-g_shot_counter+1);

  /* Get User Input */
  int user_in_row = 0, user_in_col = 0;
  printf("Captain, please enter coordinates: ");
  scanf("%d %d", &user_in_row, &user_in_col);

  /* Check User Input */
  while (input_check(master, player, user_in_row, user_in_col)!=1) {
    if(input_check(master, player, user_in_row, user_in_col)==-1){
      printf("Out of Range\n");
      printf("Captain, please enter coordinates: ");
      scanf("%d %d", &user_in_row, &user_in_col);
    }
    if (input_check(master, player, user_in_row, user_in_col)==-2) {
      printf("Captian, you've already shot in that position!\n");
      printf("Captain, please enter coordinates: ");
      scanf("%d %d", &user_in_row, &user_in_col);
    }
  }
  /* See if it is a Hit or Miss */
  if(input_check(master, player, user_in_row, user_in_col)==1){
    if (master[user_in_row][user_in_col]=='B') {
      printf("HIT!\n\n");
      player[user_in_row][user_in_col]='H';
      g_num_ships_hit++;
    } else {
      printf("MISS!\n\n");
      player[user_in_row][user_in_col]='M';
    }
  }
  g_shot_counter++;

}

int input_check(char master[][g_map_cols], char player[][g_map_cols], int in_rows, int in_cols){
  if ((in_rows>=g_map_rows)||(in_cols>=g_map_cols)) {
    return -1;
  }
  if ((in_rows<0)||(in_cols<0)) {
    return -1;
  }
  if ((player[in_rows][in_cols]=='H')||(player[in_rows][in_cols]=='M')) {
    return -2;
  }
  return 1;
}
