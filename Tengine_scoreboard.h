#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

// Utility

void savescore( int score);

void loadscore( int *score);

void getcharArray( char nickname[20]);

void ngetcharArray( char nickname[20]);

//--------------------------------------------------------------------------------------------------------------------------------------------

struct score_info{

	int score;
	time_t date;
};
typedef struct score_info* fscore;


int create_score_info( fscore *info, int score);

int get_score( fscore node);

int set_date( fscore *node, time_t new_date);

int delete_score_info( fscore *info);

void print_score_info( fscore info);

void nprint_score_info( fscore info, WINDOW *win, int cord_y, int cord_x);

//--------------------------------------------------------------------------------------------------------------------------------------------

struct player_scores{

	fscore pscore;
	struct player_scores *under;
};
typedef struct player_scores* pscores;


int add_score( pscores *node, int score);

int del_score( pscores *node);

void print_score( pscores node);

void nprint_score( pscores node, WINDOW *win, int cord_y, int cord_x);

//--------------------------------------------------------------------------------------------------------------------------------------------

struct node_player{

	char nickname[20];
	pscores under;
	struct node_player *next;
    struct node_player *previews;
};
typedef struct node_player* nplayer;

int add_player( nplayer *node, char nick[20], int score);

int delete_player( nplayer *node);

void print_player_name( nplayer node);

void nprint_player_name( nplayer node, WINDOW *win);

void print_player( nplayer node);

int nprint_player( nplayer node, WINDOW *win, int Max_y, int Max_x);

//--------------------------------------------------------------------------------------------------------------------------------------------

struct player_list{

	nplayer head;
    nplayer tail;
};
typedef struct player_list* plist;

int new_player_list( plist *lista);

int get_best_score_from_player_in_list( nplayer *player);

int order_player_in_list( plist *lista, nplayer *new_node, int score);

nplayer add_player_to_list( plist *lista, char nick[20], int score);

nplayer search_player_in_list( plist lista, char nickname[20]);

int add_score_to_player_in_list( plist *lista, char nickname[20], int score);

void print_best_player_score_in_list( plist lista, char nickname[20]);

int remove_player_from_list( plist *lista, nplayer *player);

int remove_score_from_player_list( plist *lista, nplayer player);

int end_player_list( plist *lista);

void print_players_list( plist lista);

void print_player_from_list( plist lista, char nickname[20]);

int nprint_player_from_list( plist lista, char nickname[20], WINDOW *win, int Max_y, int Max_x);

void nprint_players_list( plist lista, WINDOW *win, int Max_y, int Max_x);

int save_player_list(plist lista);

int load_player_list(plist *lista);
