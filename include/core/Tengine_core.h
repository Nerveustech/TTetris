#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>


typedef enum types{ I, O, T, S, Z, J, L} types;

types gettype();

//--------------------------------------------------------------------------------------------------------------------------------------------

struct block{

	int block;
	int x, y;
};
typedef struct block block;

//--------------------------------------------------------------------------------------------------------------------------------------------

struct tetramino_body{

        int block_orientation;
        int core_y;
        int core_x;
};
typedef struct tetramino_body tbody;

struct tetramino{

        types tetramino_type;                                                             // ADT Tetramino
        tbody cord;
};
typedef struct tetramino* tetramino;

int create_tetramino( tetramino *newt, int y, int x, types type);

int mod_tetramino( tetramino *one, block core_info, int orientation);

int delete_tetramino( tetramino *delt);

void print_tetramino_info( tetramino *info);

void nprint_tetramino_in_prevscreen( WINDOW *win, tetramino two);

void save_tetramino(tetramino t);

void load_tetramino(tetramino *t);

//--------------------------------------------------------------------------------------------------------------------------------------------

struct tetramino_queue_node{

	tetramino one;
	struct tetramino_queue_node* next;
};
typedef struct tetramino_queue_node* tqueue_node;                                         // ADT Tqueue_node

int create_tqueue_node( tqueue_node *node, types block_type);

int delete_tqueue_node( tqueue_node *node);

void print_tqueue_node( tqueue_node *node);

//--------------------------------------------------------------------------------------------------------------------------------------------

struct tetramino_queue{

	tqueue_node head;
	tqueue_node tail;
};
typedef struct tetramino_queue* tqueue;

int add_in_tetramino_queue( tqueue *coda, types name);

int new_tetramino_queue( tqueue *coda);                                                   // ADT Tqueue

void get_tetramino_info_from_queue( tqueue *coda, tetramino *two);

int dequeue_tetramino( tqueue *coda, tetramino *two);

int end_tetramino_queue( tqueue *coda);

void print_tetramino_queue( tqueue *coda);

//--------------------------------------------------------------------------------------------------------------------------------------------

struct node_block{

    block info;
    struct node_block *right;
    struct node_block *left;
    struct node_block *down;
    struct node_block *up;
};
typedef struct node_block nblock;

struct virtual_window{

    nblock *head_x;
    nblock *tail_x;

    nblock *tail_y;                                                                       // ADT Virtual window
    nblock *tail_y_max;

    nblock *core;
    nblock *piece1;
    nblock *piece2;
	nblock *piece3;
};
typedef struct virtual_window* vwin;

int new_virtual_window( int max_y, int max_x, vwin *schermo, int mode, int *my, int *mx);

int check_line( int *line, int max_x, vwin *schermo);

int delete_line( int wanted_line_y, vwin *schermo);

int shift_line( int wanted_line_y, vwin *schermo);

nblock* search_block( nblock *core, int cord_y, int cord_x);

int create_block( tetramino *type, int max_y, int max_x, vwin *schermo);

int shift_block( tetramino *type, int shift_type, vwin *schermo);

void rotate_block_clockwise( tetramino *one, vwin *schermo);

int end_virtual_window( vwin *schermo);

void print_virtual_window( vwin schermo);

void nprint_virtual_window( vwin schermo, WINDOW *win, int color);

void save_virtual_window( vwin schermo, int maxy, int maxx, tetramino one);

int load_virtual_window( vwin *schermo, int *maxy, int *maxx, tetramino *one);
