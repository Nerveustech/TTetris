
#include <stdio.h>
#include <stdlib.h>

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

//-------------------------------------------------------------------//

int new_virtual_window( int max_y, int max_x, vwin *schermo){

    	int l= 0;
	int h= 0;
	nblock_x *node= NULL;
	nblock_y *nodey= NULL;


	block stdb;
	stdb.block= 0;

    	*schermo= (vwin)malloc(sizeof(struct virtual_window));
   	if( *schermo== NULL) return 1;

   	for( l; l< max_x; l++){

		stdb.x= l;
		stdb.y= 0;

   	     	if( l== 0){
   	        	node= (nblock_x*)malloc(sizeof(nblock_x));
			if( node== NULL) return 1;

			(*schermo)-> head_x= node;
			(*schermo)-> search_x= node;
		}
        	else{
        		node-> right= (nblock_x*)malloc(sizeof(nblock_x));
			if( (node-> right)== NULL) return 1;

       			(*schermo)-> tail_x= node-> right;
			node= node-> right;
        	    	node-> left= (*schermo)-> search_x;
			(*schermo)-> search_x= node;
        	}
		node-> info= stdb;

		for( h= 1; h< max_y; h++){

			stdb.y= h;

			if( h== 1){
				nodey= (nblock_y*)malloc(sizeof(nblock_y));
				if( nodey== NULL) return 1;
				node-> down= nodey;
				(*schermo)-> search_y= nodey;
			}
			else{
				nodey-> down= (nblock_y*)malloc(sizeof(nblock_y));
				if( nodey== NULL) return 1;
				(*schermo)-> tail_y= nodey-> down;
				nodey= nodey-> down;
				nodey-> up= (*schermo)-> search_y;

			}
			nodey-> info= stdb;
		}
    	}
	return 0;
}



int check_block( int wanted_y, int wanted_x, int max_y, int max_x, vwin *schermo){

	nblock_x* node= NULL;
	nblock_y* nodey= NULL;
	block b_info;
	block b_info_comp;

	node= (*schermo)-> search_x;
	b_info= node-> info;

	if( wanted_x <= max_x/2 && b_info.x <= wanted_x){

		if( b_info.x - wanted_x <= wanted_x - 0);
		else node= (*schermo)-> head_x;

                while( b_info.x != wanted_x && b_info.x != (max_x-1)){

                        node= node-> right;
                        b_info= node-> info;
                }
	}
	else{
		if( b_info.x - wanted_x <= wanted_x - (max_x - 1));
		else node= (*schermo)-> tail_x;

		if(wanted_x == max_x);
		else{
			while( b_info.x != wanted_x && b_info.x != 0){

				node= node-> left;
				b_info= node-> info;

			}
		}
	}

	if( wanted_y != 0){

		nodey= (*schermo)-> search_y;
		b_info_comp= nodey-> info;

		if( b_info_comp.x != b_info.x) nodey= NULL;

		if ( nodey == NULL){
			nodey= node-> down;
			b_info= nodey-> info;
		}
		else{
			b_info= nodey-> info;
		}

	        if( b_info.y <= wanted_y){
	                if( b_info.y - wanted_y <= wanted_y - 0);
	                else nodey= node-> down;

	                while( b_info.y != wanted_y && b_info.y != (max_y-1)){

	                        nodey= nodey-> down;
	                        b_info= nodey-> info;
	                }
	        }
		else{

			while( b_info.y != wanted_y && b_info.y != 1){

				nodey= nodey-> up;
				b_info= nodey-> info;
			}
		}
	}

	if( ( b_info.y != wanted_y) && ( b_info.x != wanted_x)) return -12;
	else{
		if(b_info.y != wanted_y) return -11;
		if(b_info.x != wanted_x) return -10;

		else{
			printf(" Cord blocco x:%d, y:%d\n", b_info.x, b_info.y);
			return b_info.block;
		}
	}
}


int main(){

	vwin schermo;
	int max_y= 13;
	int max_x= 15;
	int error_check;
	int i= 0;
	int y= 0;
	int x= 0;

	error_check= new_virtual_window( max_y, max_x, &schermo);
	if(error_check == -1){

		printf("Errore nella malloc");
		getchar();
		exit(1);
	}
//	while( i <= max_x){
		error_check= check_block( 14, 13, max_y, max_x, &schermo);
	/*	if(error_check == -10){

			printf("Blocco inesistente");
			getchar();
			exit(1);
		}
	*/
	i++;
	y++;
//	}

	printf(" stai checkando il blocco di valore: %d \n", error_check);
}
