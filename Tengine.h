struct tetramino_body{

        int block_orientation[4];
        int core_y;
        int core_x;
}
typedef struct tetramino_body tbody;

struct tetramino{

        char tetramino_type;
        tbody cord;
};

struct tetramino_queue_node{

	tetramino one;
	tetramino_queue_node* next;
};
typedef struct tetramino_queue_node tqueue_node;

struct tetramino_queue{

	tqueue_node *head;
	tqueue_node *tail;
};
typedef struct tetramino_queue* tqueue;


int new_tetramino_queue( tqueue *coda);

int add_in_tetramino_queue( tqueue *coda, char block_type_name);

int dequeue_tetramino( tqueue *coda, vwin schermo);

int end_tetramino_queue( tqueue *coda);




struct block{

	int block;
	int x, y;
};
typedef struct block block;

struct node_block{

	block info;
	struct node_block *right;
	struct node_block *left;
	struct node_block *down;
	struct node_up *up;
};
typedef struct node_block nblock;

struct virtual_window{

	nblock *head_x;
	nblock *tail_x;

	nblock *tail_y;
	nblock *tail_y_max;

	nblock *search;

	nblock *block_center;
	nblock *block_piece_1;
	nblock *block_piece_2;
	nblock *block_piece_3;
};
typedef struct virtual_window* vwin;


int new_virtual_window( int max_y, int max_x, vwin *schermo);

int size_increase_virtual_window( int max_y, int max_x, vwin *schermo);

int size_decrease_virtual_window( int max_y, int max_x, vwin *schermo);

int check_line( int max_y, int max_x, vwin *schermo);

int delete_line( int wanted_line_y, vwin schermo);

int shift_line( int wanted_line_y, vwin schermo);

block create_block( binfo type, int max_y, int max_x, vwin *schermo);

block shift_block( binfo type, int shift_type, int max_y, int max_x, vwin *schermo);

block rotate_block( binfo type, int rotation_type, int max_y, int max_x, vwin *schermo);

int end_virtual_window( vwin *schermo);




struct player_score{

	int score;
	int date[5];
};

struct player_scores{

	player_score pscore;
	struct player_scores *under;
};
typedef struct player_scores pscores;

struct node_player{

	char nickname[20];
	pscores *under;
	struct node_player *next;
};
typedef struct node_player nplayer;

struct player_list{

	nplayer *head;
	nplayer *tail;
};
typedef struct player_list plist;

int new_player_list( plist *lista);

int add_player_to_list( plist *lista, char nickname[20]);

int search_player_in_list( plist *lista, char nickname[20]);

int add_score_to_player_list( plist *lista, char nickname[20], int score);

int search_score_in_list( plist *lista, char nickname[20], int score);

int remove_score_from_player_list( plist *lista, char nickname[20]);

int end_player_list( plist *lista);

int savelist_on_file( plist *lista, FILE* save);

