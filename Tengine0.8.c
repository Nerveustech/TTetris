
#include <stdio.h>
#include <stdlib.h>

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
        struct node_block *up;
};
typedef struct node_block nblock;

struct virtual_window{

        nblock *head_x;
        nblock *tail_x;

        nblock *tail_y;
	nblock *tail_y_max;

        nblock *search;
};
typedef struct virtual_window* vwin;

//-------------------------------------------------------------------//

int new_virtual_window( int max_y, int max_x, vwin *schermo){

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

				node-> info= stdb;

				(*schermo)-> head_x= node;
				(*schermo)-> search= node;
				mirror_x= node;
				mirror_y= node;
			}
			else{
				if( l== 0){
					node= (nblock*)malloc(sizeof(nblock));
					if( node== NULL) return 1;

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
					node-> left= nodel;

					if( h== 0 && l== (max_x-1)) (*schermo)-> tail_x= node;

					if( h != 0){
						node-> up= mirror_x;
						mirror_x-> down= node;

						mirror_x= mirror_x-> right;

						if( h== (max_y-1) && l== (max_x-1)) (*schermo)-> tail_y= node;
					}
				}
			}
		}
	}
}



int check_block( int wanted_y, int wanted_x, int max_y, int max_x, vwin *schermo){

	nblock* node= NULL;

	block b_info;
	block b_info_comp;

	node= (*schermo)-> search;
	b_info= node-> info;

	if( b_info.x <= wanted_x && b_info.y <= wanted_y){

	}

	if( b_info.x >= wanted_x && b_info.y >= wanted_y){

	}

	if( b_info.x >= wanted_x && b_info.y <= wanted_y){

	}

	if( b_info.x <= wanted_x && b_info.y >= wanted_y){

	}

	if( ( b_info.y != wanted_y) && ( b_info.x != wanted_x)) b_info.block= -12;
	else{
		if(b_info.y != wanted_y) b_info.block= -11;
		if(b_info.x != wanted_x) b_info.block= -10;
	}
}



int main(){

	vwin schermo;
	int max_y= 3;
	int max_x= 3;

	int error_check;
	block copy;

	error_check= new_virtual_window( max_y, max_x, &schermo);
	if(error_check == 1){

		printf("Errore nella malloc");
		getchar();
		exit(1);
	}

	copy= check_block( 2, 3, max_y, max_x, &schermo);
	printf(" stai checkando il blocco di valore: %d \n", copy.block);
}
