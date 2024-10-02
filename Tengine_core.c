#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include "Tengine_core.h"


// Funzione di debbug, per convertire char in types
types gettype(){

	char key= getchar();
	switch(key){

		case 'i':
			return I;

		case 'o':
			return O;

		case 't':
			return T;

		case 's':
			return S;

		case 'z':
			return Z;

		case 'j':
			return J;

		case 'l':
			return L;

		default:
			return I;
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------

// Funzione per creare un tetramino
int create_tetramino( tetramino *newt, int new_y, int new_x, types name){

        *newt= (tetramino)malloc(sizeof(struct tetramino));
        if( *newt == NULL) return 1;

        (*newt)-> tetramino_type= name;
        (*newt)-> cord.core_x= new_x;
        (*newt)-> cord.core_y= new_y;
        (*newt)-> cord.block_orientation= 1;
        return 0;

}

// Funzione che permette la modifica delle cordinate e l'orientamento del tetramino'
int mod_tetramino( tetramino *one, block core_info, int orientation){

	if( (*one) == NULL) return 1;

	(*one)-> cord.core_x= core_info.x;
	(*one)-> cord.core_y= core_info.y;
	(*one)-> cord.block_orientation+= orientation;
	if( (*one)-> cord.block_orientation == 5) (*one)-> cord.block_orientation= 1;

	return 0;

}

// Funzione che dealloca dalla heap il tetramino
int delete_tetramino( tetramino *delt){

	if( *delt == NULL) return 1;

	free(*delt);
	return 0;
}

// Funzione di debbug per controllare le info del tetramino
void print_tetramino_info( tetramino *info){

	if( *info == NULL) printf("No info sul Tetramino");
	else{

		tbody print_cord= (*info)-> cord;
		printf("Tetramino_type:%d, X:%d, Y:%d, Block_orientation:%d\n", (*info)-> tetramino_type, print_cord.core_x, print_cord.core_y, print_cord.block_orientation);
	}
}

// Serve a stampare il tetramino in una finestra di ncurses
void nprint_tetramino_in_prevscreen( WINDOW *win, tetramino two){

	int Maxy= 0;
	int Maxx= 0;

	getmaxyx( win, Maxy, Maxx);
	if( two == NULL) mvwprintw( win, Maxy/2, (Maxx/2)-4, "No info");
	else{

		types tetratype= two->tetramino_type;

		Maxy-= 2;
		Maxx-= 2;
		wclear( win);
		if( tetratype == I || tetratype == T || tetratype == L || tetratype == J){
			mvwprintw( win, (Maxy/2)+1, (Maxx/2)-2, "      ");
			mvwchgat( win, (Maxy/2)+1, (Maxx/2)-2, 6, A_REVERSE, 5, NULL);
			if( tetratype == I){
				mvwprintw( win, (Maxy/2)+1, (Maxx/2)+4, "  ");
				mvwchgat( win, (Maxy/2)+1, (Maxx/2)+4, 2, A_REVERSE, 5, NULL);
			}
			if( tetratype == T){
				mvwprintw( win, (Maxy/2), (Maxx/2), "  ");
				mvwchgat( win, (Maxy/2), (Maxx/2), 2, A_REVERSE, 5, NULL);
			}
			if( tetratype == L){
				mvwprintw( win, (Maxy/2), (Maxx/2)+2, "  ");
				mvwchgat( win, (Maxy/2), (Maxx/2)+2, 2, A_REVERSE, 5, NULL);
			}
			if( tetratype == J){
				mvwprintw( win, (Maxy/2), (Maxx/2)-2, "  ");
				mvwchgat( win, (Maxy/2), (Maxx/2)-2, 2, A_REVERSE, 5, NULL);
			}
		}
		if( tetratype == S || tetratype == Z || tetratype == O){
			mvwprintw( win, (Maxy/2)+1, (Maxx/2)-2, "    ");
			mvwchgat( win, (Maxy/2)+1, (Maxx/2)-2, 4, A_REVERSE, 5, NULL);
			if( tetratype == O){
			mvwprintw( win, (Maxy/2), (Maxx/2)-2, "    ");
			mvwchgat( win, (Maxy/2), (Maxx/2)-2, 4, A_REVERSE, 5, NULL);
			}
			if( tetratype == S){
			mvwprintw( win, (Maxy/2), (Maxx/2), "    ");
			mvwchgat( win, (Maxy/2), (Maxx/2), 4, A_REVERSE, 5, NULL);
			}
			if( tetratype == Z){
			mvwprintw( win, (Maxy/2)+2, (Maxx/2), "    ");
			mvwchgat( win, (Maxy/2)+2, (Maxx/2), 4, A_REVERSE, 5, NULL);
			}
		}
	}
}

//Salva il tetramino in un file aperto in binario
void save_tetramino(tetramino t){

	FILE *file = fopen("tetramino_info.txt", "w");

	int copy_int= (int)( t-> tetramino_type);
	tbody cord_copy= t-> cord;
    fprintf( file, "%d %d", copy_int, cord_copy.block_orientation);
	fclose(file);
}

//Carica il tetramino da un file aperto in binario
void load_tetramino(tetramino *t){

	FILE *file = fopen("tetramino_info.txt", "r");

	int copy_int, block_orientation;
	types copy;

	fscanf( file, "%d %d", &copy_int, &block_orientation);

	copy= (types)(copy_int);
	create_tetramino( &(*t), 0, 0, copy);

	(*t)-> cord.block_orientation= block_orientation;
	fclose(file);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------

// Crea un nodo da aggiungere alla coda di tetramini
int create_tqueue_node( tqueue_node *node, types block_type){

	tetramino copy;

	*node= ( tqueue_node)malloc(sizeof( struct tetramino_queue_node));
	if( *node == NULL) return 1;
	create_tetramino( &(*node)-> one, 0, 0, block_type);
	return 0;
}

// Dealloca un nodo della coda di tetramini
int delete_tqueue_node( tqueue_node *node){

	if( *node == NULL) return 1;

	free( *node);               // Non dealloco il tetramino perchè in una funzione più avanti, questo viene dato ad una variabile esterna alla coda
	return 0;
}

// Funzione di debbug per controllare lo stato dei nodi
void print_tqueue_node( tqueue_node *node){

	if( *node == NULL){
		printf( "Non esiste il nodo");
	}
	print_tetramino_info( &(*node)-> one);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------

// Aggiunge un nodo alla coda dei tetramini
int add_in_tetramino_queue( tqueue *coda, types name){

	int error_check;
	tqueue_node newq;

	error_check= create_tqueue_node( &newq, name);
	if( error_check == 1) return 1;

	if( (*coda)-> head == NULL){
		newq-> next= NULL;
		(*coda)-> head= newq;
		(*coda)-> tail= newq;
	}
	else{
		newq-> next= (*coda)->head;
		(*coda)-> head= newq;

	}
	return 0;
}

// Crea la struttura che ospita la coda dei tetramini
int new_tetramino_queue( tqueue *coda){

	int ciclo= 0;
	int error_check= 0;
	types name;
	int copy= 0;

	*coda= ( tqueue)malloc(sizeof( struct tetramino_queue));
	if( *coda == NULL) return 1;

	(*coda)-> head= NULL;
	(*coda)-> tail= NULL;

	srand(time(NULL));

	for(ciclo; ciclo< 2; ciclo++){

		do{
			name= (types)(rand() % 7);
		}while( name> 6);

		if( name == (types)copy){
			if( name == L) name-= 1;
			else name+= 1;
			copy= name;
		}
		error_check= add_in_tetramino_queue( &(*coda), name);
		if( error_check == 1) return 1;
	}
	return 0;
}

// Prende l'info del blocco head'
void get_tetramino_info_from_queue( tqueue *coda, tetramino *two){

	if( *coda == NULL) return;

	tqueue_node toget= (*coda)-> head;
	tqueue_node shadow;

	while( toget-> next != NULL){

		shadow= toget;
		toget= toget-> next;
	}
	*two= shadow-> one;
}

// Fa uscire la variabile tetramino dalla coda e libera il nodo in cui si trovava
int dequeue_tetramino( tqueue *coda, tetramino *two){

	if( *coda == NULL) return 1;
	tqueue_node del= (*coda)-> tail;
	tqueue_node shadow= (*coda)-> head;

	*two= del-> one;

	del= shadow-> next;
	while( del != (*coda)-> tail){
		shadow= shadow-> next;
		del= del-> next;
	}
	shadow-> next= NULL;
	(*coda)-> tail= shadow;
	delete_tqueue_node( &del);
	return 0;
}

// Libera conpletamente la coda di tetramini
int end_tetramino_queue( tqueue *coda){

	if( *coda == NULL) return 1;
	tetramino tdel= NULL;
	tqueue_node del= (*coda)-> head;

	while( (*coda)-> head != (*coda)-> tail){
		dequeue_tetramino( &(*coda), &tdel);
		delete_tetramino( &tdel);
	}
	tdel= del-> one;
	delete_tetramino( &tdel);
	delete_tqueue_node( &del);
	free( *coda);
	return 0;
}

// Funzione di debbug per controllare i valori della coda
void print_tetramino_queue( tqueue *coda){

	tqueue_node copy= (*coda)-> head;
	while( copy != NULL){
		print_tqueue_node( &copy);
		copy= copy-> next;
	}
	printf("\n");
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------

int new_virtual_window( int max_y, int max_x, vwin *schermo, int mode, int *my, int *mx){

	FILE *file;
	if( mode == 1){

		file = fopen("vwinscreen.txt", "r");
		if( file == NULL) return 1;

		fscanf( file, "%2d", &max_y);
		fscanf( file, " %2d\n", &max_x);
		*my= max_y;
		*mx= max_x;
	}

	int l= 0;
	int h= 0;

	nblock *mirror_x= NULL;
	nblock *mirror_y= NULL;

	nblock *node= NULL;
	nblock *nodel= NULL;

	block stdb;
	stdb.block= 0;

	*schermo= (vwin)malloc(sizeof(struct virtual_window));
   	if( *schermo== NULL) return 1;

	for( h; h< max_y; h++){
		l= 0;

		for( l; l< max_x; l++){

			stdb.x= l;
			stdb.y= h;

			if( l== 0 && h== 0){
				node= (nblock*)malloc(sizeof(nblock));
				if( node== NULL) return 1;


				if( mode == 1) fscanf( file, "%d", &(stdb.block));
				node-> info= stdb;
				node-> up= NULL;
				node-> left= NULL;

				(*schermo)-> head_x= node;
				mirror_x= node;
				mirror_y= node;
			}
			else{
				if( l== 0){
					node= (nblock*)malloc(sizeof(nblock));
					if( node== NULL) return 1;

					if( mode == 1) fscanf( file, "%d", &(stdb.block));
					node-> info= stdb;
					node-> left= NULL;
					node-> down= NULL;

					mirror_x= mirror_y;

					node-> up= mirror_x;
					mirror_x-> down= node;

					mirror_x= mirror_x-> right;
					mirror_y= node;

					if( h== (max_y-1)) (*schermo)-> tail_y_max= node;
				}
				else{
					node-> right= (nblock*)malloc(sizeof(nblock));
					if( node== NULL)return 1;

					nodel= node;
					node= node-> right;

					if( mode == 1) fscanf( file, "%d", &(stdb.block));
					node-> info= stdb;
					node-> left= nodel;

					node-> up= NULL;
					if( l== (max_x-1)) node-> right= NULL;
					if( h== 0 && l== (max_x-1)) (*schermo)-> tail_x= node;

					if( h != 0){
						node-> up= mirror_x;
						mirror_x-> down= node;

						mirror_x= mirror_x-> right;

						if( h== (max_y-1)) node-> down= NULL;
						if( h== (max_y-1) && l== (max_x-1)) (*schermo)-> tail_y= node;
					}
				}
			}
		}
	}
	if( mode == 1) fclose( file);
	return 0;
}

nblock* search_block( nblock *core, int cord_y, int cord_x){

	nblock *new_cord_xy= core;

	while( new_cord_xy-> info.x != cord_x){
		if( new_cord_xy-> info.x > cord_x) new_cord_xy= new_cord_xy-> left;
		else if( new_cord_xy-> info.x < cord_x) new_cord_xy= new_cord_xy-> right;
		if( new_cord_xy == NULL) return NULL;
	}
	while( new_cord_xy-> info.y != cord_y){
		if( new_cord_xy-> info.y > cord_y) new_cord_xy= new_cord_xy-> up;
		else if( new_cord_xy-> info.y < cord_y) new_cord_xy= new_cord_xy-> down;
		if( new_cord_xy == NULL) return NULL;
	}
	return new_cord_xy;
}

int create_block( tetramino *type, int max_y, int max_x, vwin *schermo){

	int error_check;
	nblock *tcreate= (*schermo)-> head_x;
	if( tcreate == NULL) return 1;

	nblock *tcreate_piece;
	nblock *tcreate_piece2;

	block binfo= tcreate-> info;
	tetramino tinfo= *type;
	tbody new_cord;

	tcreate= search_block( tcreate, 2, (max_x/2)-1);
	if( tcreate-> info.block == 2) return -1;

	tcreate-> info.block= 1;
	(*schermo)-> core= tcreate;

	for( int i= 1; i<= 3; i++){
		switch(i){

			case 1:
				if( tinfo-> tetramino_type== S || tinfo-> tetramino_type== O) tcreate_piece= tcreate-> left;
				else tcreate_piece= tcreate-> right;
				(*schermo)-> piece1= tcreate_piece;
				tcreate_piece2= tcreate_piece;
				break;

			case 2:
				if( tinfo-> tetramino_type== S || tinfo-> tetramino_type== Z || tinfo-> tetramino_type== O) tcreate_piece2= tcreate-> up;
				else tcreate_piece2= tcreate-> left;
				(*schermo)-> piece2= tcreate_piece2;
				break;

			case 3:
				if( tinfo-> tetramino_type== T) tcreate_piece2= tcreate-> up;
				if( tinfo-> tetramino_type== I) tcreate_piece2= tcreate_piece-> right;
				if( tinfo-> tetramino_type== L) tcreate_piece2= tcreate_piece-> up;
				if( tinfo-> tetramino_type== J) tcreate_piece2= tcreate_piece2-> up;
				if( tinfo-> tetramino_type== S) tcreate_piece2= tcreate_piece2-> right;
				if( tinfo-> tetramino_type== Z || tinfo-> tetramino_type== O) tcreate_piece2= tcreate_piece2-> left;
				(*schermo)-> piece3= tcreate_piece2;
				break;
		}
		tcreate_piece2-> info.block= 1;
	}
	mod_tetramino( &(*type), tcreate->info, 0);
	return 0;
}

int shift_block( tetramino *type, int shift_type, vwin *schermo){

	nblock **piece_shadow[4]={ &((*schermo)-> core), &((*schermo)-> piece1), &((*schermo)-> piece2), &((*schermo)-> piece3)};
	nblock *check= NULL;
	block binfo;

	int proximity_check;
	int ciclo;

	for( ciclo= 0; ciclo< 4; ciclo++){
		if( shift_type == 261){
			check= (*piece_shadow[ciclo])-> right;
			if( (*piece_shadow[ciclo])-> right == NULL || check-> info.block == 2) return 1;
		}

		if( shift_type == 260){
			check= (*piece_shadow[ciclo])-> left;
			if( (*piece_shadow[ciclo])-> left == NULL || check-> info.block == 2) return 1;
		}

		if( shift_type == 258){
			check= (*piece_shadow[ciclo])-> down;
			if( (*piece_shadow[ciclo])-> down == NULL || check-> info.block == 2){
				for( ciclo= 0; ciclo< 4; ciclo++) (*piece_shadow[ciclo])-> info.block= 2;
				return -1;
			}
		}
	}
	for( ciclo= 0; ciclo< 4; ciclo++) (*piece_shadow[ciclo])-> info.block= 0;

	for( ciclo= 0; ciclo< 4; ciclo++){
		if( shift_type == 261) (*piece_shadow[ciclo])= (*piece_shadow[ciclo])-> right;

		if( shift_type == 260) (*piece_shadow[ciclo])= (*piece_shadow[ciclo])-> left;

		if( shift_type == 258) (*piece_shadow[ciclo])= (*piece_shadow[ciclo])-> down;

		(*piece_shadow[ciclo])-> info.block= 1;
	}
	mod_tetramino( &(*type), (*piece_shadow[0])-> info, 0);
	return 0;
}

void rotate_block_clockwise( tetramino *one, vwin *schermo){

	nblock **tetramino_pieces[4]= { &((*schermo)-> core), &((*schermo)-> piece1), &((*schermo)-> piece2), &((*schermo)-> piece3)};
	nblock *check[4]= { NULL, NULL, NULL, NULL};

	int ciclo;
	int core_dist;
	int core_dist2;

	if( (*tetramino_pieces[0]) == NULL || (*tetramino_pieces[0])-> info.block == 2) return;
	if( (*one)-> tetramino_type == O) return;

	for( ciclo = 1; ciclo < 4; ciclo++){
		core_dist= (*tetramino_pieces[ciclo])-> info.x - (*tetramino_pieces[0])-> info.x;
		core_dist2= (*tetramino_pieces[0])-> info.y - (*tetramino_pieces[ciclo])-> info.y;

		if( ciclo != 3 && (*one)-> tetramino_type != S && (*one)-> tetramino_type != Z){
			if( (*one)-> cord.block_orientation == 1 || (*one)-> cord.block_orientation == 3)
				check[ciclo]= search_block( (*tetramino_pieces[0]), (*tetramino_pieces[0])-> info.y + core_dist, (*tetramino_pieces[0])-> info.x );

			if( (*one)-> cord.block_orientation == 2 || (*one)-> cord.block_orientation == 4)
				check[ciclo]= search_block( (*tetramino_pieces[0]), (*tetramino_pieces[0])-> info.y, (*tetramino_pieces[0])-> info.x + core_dist2);
		}
		else{
			if( ( ciclo == 1 && ((*one)-> cord.block_orientation == 1 || (*one)-> cord.block_orientation == 3)) || ( ciclo == 2 && ((*one)-> cord.block_orientation == 2 || (*one)-> cord.block_orientation == 4)))
				check[ciclo]= search_block( (*tetramino_pieces[0]), (*tetramino_pieces[0])-> info.y + core_dist, (*tetramino_pieces[0])-> info.x );

			if( ( ciclo == 2 && ((*one)-> cord.block_orientation == 1 || (*one)-> cord.block_orientation == 3)) || ( ciclo == 1 && ((*one)-> cord.block_orientation == 2 || (*one)-> cord.block_orientation == 4)))
				check[ciclo]= search_block( (*tetramino_pieces[0]), (*tetramino_pieces[0])-> info.y, (*tetramino_pieces[0])-> info.x + core_dist2 );
		}
		if( ciclo == 3){
			if( ((*one)-> tetramino_type == I && ((*one)-> cord.block_orientation == 1 || (*one)-> cord.block_orientation == 3)) || ( (*one)-> tetramino_type == T && ((*one)-> cord.block_orientation == 2 || (*one)-> cord.block_orientation == 4)))
				check[ciclo]= search_block( (*tetramino_pieces[0]), (*tetramino_pieces[0])-> info.y + core_dist, (*tetramino_pieces[0])-> info.x );

			if( ((*one)-> tetramino_type == T && ((*one)-> cord.block_orientation == 1 || (*one)-> cord.block_orientation == 3)) || ( (*one)-> tetramino_type == I && ((*one)-> cord.block_orientation == 2 || (*one)-> cord.block_orientation == 4)))
				check[ciclo]= search_block( (*tetramino_pieces[0]), (*tetramino_pieces[0])-> info.y, (*tetramino_pieces[0])-> info.x + core_dist2);

			if( (((*one)-> tetramino_type == J || (*one)-> tetramino_type == Z) && ((*one)-> cord.block_orientation == 1 || (*one)-> cord.block_orientation == 3)) || (((*one)-> tetramino_type == L || (*one)-> tetramino_type == S) && ((*one)-> cord.block_orientation == 2 || (*one)-> cord.block_orientation == 4)))
				check[ciclo]= search_block( (*tetramino_pieces[0]), (*tetramino_pieces[0])-> info.y - core_dist2, (*tetramino_pieces[0])-> info.x - core_dist );

			if( (((*one)-> tetramino_type == L || (*one)-> tetramino_type == S) && ((*one)-> cord.block_orientation == 1 || (*one)-> cord.block_orientation == 3)) || (((*one)-> tetramino_type == J || (*one)-> tetramino_type == Z) && ((*one)-> cord.block_orientation == 2 || (*one)-> cord.block_orientation == 4)))
				check[ciclo]= search_block( (*tetramino_pieces[0]), (*tetramino_pieces[0])-> info.y + core_dist, (*tetramino_pieces[0])-> info.x + core_dist2 );
		}
		if( check[ciclo] == NULL || check[ciclo]-> info.block == 2) return;
	}

	for( ciclo = 1; ciclo < 4; ciclo++) (*tetramino_pieces[ciclo])-> info.block= 0;

	for( ciclo = 1; ciclo < 4; ciclo++){
		(*tetramino_pieces[ciclo])= check[ciclo];
		(*tetramino_pieces[ciclo])-> info.block= 1;
	}
	mod_tetramino( &(*one), (*tetramino_pieces[0])-> info, 1);
	return;
}

int check_line( int *line_counter, int max_x, vwin *schermo){

	nblock *check_line= (*schermo)-> tail_y;
	nblock *mirror_y= check_line-> up;
	int line= 0;

	int full_line_counter= 0;

	nblock *check_down_block;
	int down_empty_line_counter= 0;

	while( check_line != NULL){

		if( check_line-> down != NULL){
			check_down_block= check_line-> down;
			if( check_down_block-> info.block == 0) down_empty_line_counter++;
		}
		if( check_line-> info.block == 2) full_line_counter++;

		if( check_line-> left == NULL){
			line= check_line-> info.y;

			if( full_line_counter == max_x){
				*line_counter+= delete_line( line, &(*schermo));
			}
			if( down_empty_line_counter == max_x){
				shift_line( line, &(*schermo));
			}
			check_line= mirror_y;
			full_line_counter= 0;
			down_empty_line_counter= 0;

			if( mirror_y != NULL) mirror_y= mirror_y-> up;
		}
		else check_line= check_line-> left;
	}
	return 0;
}

int delete_line( int wanted_line_y, vwin *schermo){

	nblock *del_line= (*schermo)-> tail_y;

	while( del_line-> info.y != wanted_line_y) del_line= del_line-> up;

	while( del_line != NULL){

		if( del_line-> info.block= 0) return 0;

		del_line-> info.block= 0;
		del_line= del_line-> left;
	}
	return 1;
}

int shift_line( int wanted_line_y, vwin *schermo){

	nblock *shift= (*schermo)-> tail_y;
	nblock *shift_shadow= NULL;
	int ciclo= 0;

	while( shift-> info.y != wanted_line_y) shift= shift-> up;

	while( shift != NULL){

		if( shift-> info.block == 2){
			shift_shadow= shift;

			while( ciclo != 1){

				shift_shadow= shift_shadow-> down;
				if( shift_shadow-> down == NULL) ciclo= 1;
				else if( shift_shadow-> info.block == 2) ciclo= 1;
			}
			if( shift_shadow-> info.block == 2) shift_shadow= shift_shadow-> up;

			shift_shadow-> info.block= 2;
			shift-> info.block= 0;
			shift= shift-> left;
		}
		else
		shift= shift-> left;
		ciclo= 0;
	}
	return 0;
}

int end_virtual_window( vwin *schermo){

	if( *schermo == NULL) return 1;

	nblock *delete_block= (*schermo)-> tail_y;
	nblock *del_shadow= NULL;
	nblock *mirror_y= delete_block-> up;

	while( delete_block != NULL){
		del_shadow= delete_block;
		if( delete_block-> left== NULL){
			delete_block= mirror_y;
			if( mirror_y != NULL) mirror_y= mirror_y-> up;
		}
		else delete_block= delete_block-> left;
		free( del_shadow);
	}
	free( *schermo);
	return 0;
}

void print_virtual_window( vwin schermo){

	nblock *print= schermo-> head_x;
	nblock *mirror_y= print-> down;

	while( print != NULL){

		if( print-> info.block != 0){
			if( print-> info.block == 1) printf("1");
			if( print-> info.block == 2) printf("2");
		}
		else printf("0");

		if( print-> right== NULL){
			printf("\n");
			print= mirror_y;
			if( mirror_y!= NULL) mirror_y= mirror_y-> down;
		}
		else print= print->right;
	}
}

void nprint_virtual_window( vwin schermo, WINDOW *win, int color){

	nblock *nprint= schermo-> head_x;
	nblock *mirror_y= nprint-> down;

	int ciclo;
	int ciclo2;
	int cord_x= 1;
	int cord_y= 1;

	while( nprint != NULL){


		for( ciclo= 0; ciclo< 2; ciclo++){

			if( nprint-> info.block == 0)
				mvwprintw( win, cord_y, cord_x, " ");

			if( nprint-> info.block == 1){
				mvwprintw( win, cord_y, cord_x, " ");
				mvwchgat( win, cord_y, cord_x, 1, A_REVERSE, color, NULL);
			}

			if( nprint-> info.block == 2){
				mvwprintw( win, cord_y, cord_x, " ");
				mvwchgat( win, cord_y, cord_x, 1, A_REVERSE, 5, NULL);
			}
			cord_x++;

		}

		if( nprint-> right== NULL){
			cord_y+= 1;
			cord_x= 1;
			nprint= mirror_y;
			if( mirror_y!= NULL) mirror_y= mirror_y-> down;
		}
		else{
			nprint= nprint->right;
		}
	}
}

void save_virtual_window( vwin schermo, int maxy, int maxx, tetramino one){

	FILE *file = fopen("vwinscreen.txt", "w");
	FILE *file2 = fopen("vwin_pointers.txt", "w");
	if( file == NULL || file2 == NULL) return;

	block info_copy;
	fprintf( file, "%d %d\n", maxy, maxx);

	nblock *savey= schermo-> head_x;
	while( savey != NULL){

		nblock *savex= savey;
		savey= savey-> down;
		while( savex != NULL){
			info_copy= savex-> info;
			fprintf( file, "%d ", info_copy.block);
			savex= savex-> right;
		}
	}
	fclose(file);


	info_copy= schermo-> core-> info;
	fprintf( file2, "%d %d\n", info_copy.y, info_copy.x );

	info_copy= schermo-> piece1-> info;
	fprintf( file2, "%d %d\n", info_copy.y, info_copy.x );

	info_copy= schermo-> piece2-> info;
	fprintf( file2, "%d %d\n", info_copy.y, info_copy.x );

	info_copy= schermo-> piece3-> info;
	fprintf( file2, "%d %d\n", info_copy.y, info_copy.x );

	save_tetramino( one);
	fclose(file2);
}

int load_virtual_window( vwin *schermo, int *maxy, int *maxx, tetramino *one){

	int error_check= 0;

	error_check= new_virtual_window( 0, 0, &(*schermo), 1, maxy, maxx);
	if( error_check == 1) return 1;


	FILE *file = fopen("vwin_pointers.txt", "r");
	if( file == NULL) return 1;

	for( int ciclo= 0; ciclo < 4; ciclo++){

		int y, x;
		fscanf( file, "%d %d\n", &y, &x);
		if( ciclo == 0) (*schermo)-> core= search_block( (*schermo)-> head_x, y, x);
		if( ciclo == 1) (*schermo)-> piece1= search_block( (*schermo)-> core, y, x);
		if( ciclo == 2) (*schermo)-> piece2= search_block( (*schermo)-> core, y, x);
		if( ciclo == 3) (*schermo)-> piece3= search_block( (*schermo)-> core, y, x);
	}
	load_tetramino( &(*one));
	fclose(file);
	return 0;
}
