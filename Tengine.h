struct block{

	int block;
	int x, y;
};
typedef struct block block;

struct node_block_y{

	block info;
	struct node_block_y *down;
	struct node_block_y *up;
};
typedef struct node_block_y nblock_y;

struct node_block_x{

	block info;
	struct node_block_x *right;
	struct node_block_x *left;
	nblock_y *down;
};
typedef struct node_block_x nblock_x;

struct virtual_window{

	nblock_x *head_x;
	nblock_x *tail_x;
	nblock_x *search_x;

	nblock_y *tail_y;
	nblock_y *search_y;
};
typedef struct virtual_window* vwin;


int new_virtual_window( int max_y, int max_x, vwin *schermo);

int size_increase_virtual_window( int max_y, int max_x, vwin *schermo);

int size_decrease_virtual_window( int max_y, int max_x, vwin *schermo);

int check_block( int wanted_y, int wanted_x, int max_y, int max_x, vwin *schermo);

int mod_block( int new_block, int wanted_y, int wanted_x, int max_y, int max_x, vwin *schermo);

int end_virtual_window( vwin *schermo);



struct block_piece{

	int piece_x[4];
	int piece_y[4];
};
typedef struct block_piece bpiece;

struct block_body{

	bpiece[3];
	int center_y;
	int center_x;
}
typedef struct block_body bbody;

struct block_type{

	char block_type_name;
	bbody cord;
};

struct block_queue_node{

	block_type n;
	block_queue_node* next;
};

struct block_queue{

	block_queue_node *head;
	block_queue_node *tail;
};
typedef struct block_queue* bqueue;


int new_block_queue( bqueue *coda);

int add_in_block_queue( bqueue *coda, block_type n);

int dequeue_block( bqueue *coda, vwin);

int end_block_queue( bqueue *coda);




struct player_score{

	int score;
	int date[5];
};

struct player_scores{

	player_score pscore;
	struct player_scores *next;
};
typedef struct player_scores pscores;

struct node_player{

	char nickname[20];
	pscores *next;
	struct node_player *next;
};
typedef struct node_player nplayer;

struct player_list{

	nplayer *head;
	nplayer *tail;
	nplayer *search;
	player_scores *pssearch;
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

