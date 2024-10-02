#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include "Tengine_scoreboard.h"

void savescore( int score){

    FILE *file = fopen("scoresave.txt", "w");
    if( file == NULL) return;

    fprintf( file, "%d", score);

    fclose(file);
}

void loadscore( int *score){

    FILE *file = fopen("scoresave.txt", "r");
    if( file == NULL) return;

    fscanf( file, "%d", score);

    fclose(file);

}

void getcharArray( char nickname[20]){

    int ciclo;

    for(ciclo= 0; ciclo< 20; ciclo++){
        nickname[ciclo]= getchar();
        if( nickname[ciclo] == ' ') nickname[ciclo]= '_';
        if( nickname[ciclo] == '\n'){
            for(ciclo; ciclo< 20; ciclo++) nickname[ciclo]= ' ';
            nickname[20]= '\0';
            return;
        }
    }

    nickname[19]= ' ';
    nickname[20]= '\0';
    while( getchar() != '\n');

}

void ngetcharArray( char nickname[20]){

    int ciclo;

    for(ciclo= 0; ciclo< 20; ciclo++){
        nickname[ciclo]= getch();
        if( nickname[ciclo] == ' ') nickname[ciclo]= '_';
        if( nickname[ciclo] == '\n'){
            for(ciclo; ciclo< 20; ciclo++) nickname[ciclo]= ' ';
            nickname[20]= '\0';
            return;
        }
    }

    nickname[19]= ' ';
    nickname[20]= '\0';
    while( getch() != '\n');

}

//--------------------------------------------------------------------------------------------------------------------------------------------

// Score_info ADT interface

int create_score_info( fscore *info, int score){

    (*info)= (fscore)malloc(sizeof( struct score_info));
    if( (*info) == NULL) return 1;
    (*info)-> score= score;
    (*info)-> date= time(0);

    return 0;
}

int get_score( fscore info){

    return info-> score;
}

int set_date( fscore *info, time_t new_date){

    if( *info == NULL) return 1;
    (*info)-> date= new_date;
    return 0;
}

int delete_score_info( fscore *info){

    if( *info == NULL) return 1;
    free( *info);
    return 0;
}

void print_score_info( fscore info){

    if( info == NULL) printf("\nNon ci sono info\n");
    printf("Score: %d. Scored %s \n", info-> score, asctime(gmtime( &(info-> date))));
}

void nprint_score_info( fscore info, WINDOW *win, int cord_y, int cord_x){

    if( info == NULL) mvwprintw( win, cord_y, cord_x, "Non ci sono info");
    mvwprintw( win, cord_y, cord_x, "Score: %d", info-> score);
    mvwprintw( win, cord_y+1, cord_x+1, "Scored %s", asctime(gmtime( &(info-> date))));
}

//--------------------------------------------------------------------------------------------------------------------------------------------

// Player_scores ADT interface

int add_score( pscores *node, int score){

    pscores new_node= (pscores)malloc(sizeof(struct player_scores));
    if( new_node == NULL) return 1;

    if( *node == NULL){
        *node= new_node;
        new_node-> under= NULL;
    }
    else{
        new_node-> under= (*node);
        *node= new_node;
    }
    create_score_info( &(new_node-> pscore), score);
    return 0;
}

int del_score( pscores *node){

    if( *node == NULL) return 1;

    pscores del= (*node);
    pscores del_shadow= del;
    pscores worst_score= del;
    pscores worst_score_shadow= worst_score;

    int score_comp= 0;
    int score_comp_prev= 0;

    while( del != NULL){
        score_comp= (del-> pscore)-> score;
        score_comp_prev= get_score( (del-> pscore));
        if( score_comp < score_comp_prev){
            worst_score= del;
            worst_score_shadow= del_shadow;

        }
        del_shadow= del;
        del= del-> under;
    }
    delete_score_info( &(worst_score-> pscore));
    if( worst_score == (*node)){
        if( worst_score-> under != NULL) (*node)= worst_score-> under;
        else (*node)= NULL;
        free( worst_score);

    }
    else{
        if( worst_score-> under == NULL) worst_score_shadow-> under= NULL;
        else worst_score_shadow-> under= worst_score-> under;
        free( worst_score);
    }
    return 0;
}

void print_score( pscores node){

    print_score_info( node-> pscore);
}

void nprint_score( pscores node, WINDOW *win, int cord_y, int cord_x){

    nprint_score_info( node-> pscore, &(*win), cord_y, cord_x);
}

//--------------------------------------------------------------------------------------------------------------------------------------------

// Node_player ADT interface

int add_player( nplayer *node, char nick[20], int score){

    int ciclo= 0;

    (*node)= (nplayer)malloc(sizeof(struct node_player));
    if( *node == NULL) return 1;

    for(ciclo; ciclo< 20; ciclo++){

        (*node)-> nickname[ciclo]= nick[ciclo];
    }
    (*node)-> nickname[20]= '\0';

    (*node)-> under= NULL;
    add_score( &((*node)-> under), score);
    (*node)-> next= NULL;
    (*node)-> previews= NULL;

    return 0;
}

int get_best_score_from_player( nplayer player){

    int best_score= 0;
    pscores node= player-> under;

    while( node != NULL){

        if( best_score < get_score( node-> pscore)) best_score= get_score( node-> pscore);
        node= node-> under;
    }
    return best_score;
}

int delete_player( nplayer *node){

    if( *node == NULL) return 1;
    while( (*node)-> under != NULL) del_score( &((*node)-> under));
    free( (*node)-> under);
    free( *node);
    *node= NULL;
    return 0;

}

void print_player_name( nplayer node){

    if( node == NULL) return;

    int ciclo= 0;

    printf("Player: ");
    for( ciclo; node-> nickname[ciclo] != ' '; ciclo++)
        printf("%c", node-> nickname[ciclo]);
}

void nprint_player_name( nplayer node, WINDOW *win){

    if( node == NULL) return;

    int ciclo= 0;

    for( ciclo; node-> nickname[ciclo] != ' '; ciclo++)
        wprintw( win, "%c", node-> nickname[ciclo]);
}

void print_player( nplayer node){

    if( node == NULL) return;

    pscores print= node-> under;

    print_player_name( node);

    printf("\nPlayer scores:\n\n");
    while( print != NULL){

        print_score( print);
        print= print-> under;

    }
}

int nprint_player( nplayer node, WINDOW *win, int Max_y, int Max_x){

    if( node == NULL) return 0;

    int ciclo= 0;
    int cord_y= (Max_y/2)-(Max_y/4)+1;

    pscores nprint= node-> under;

    mvwprintw( win, 2, 2, "Player: ");
    nprint_player_name( node, &(*win));

    mvwprintw( win, cord_y-2, 3, "SCORES:");
    while (nprint != NULL){

        nprint_score( nprint, &(*win), cord_y, 4);
        nprint= nprint-> under;
        cord_y+= 3;
        ciclo++;

    }
    return ciclo;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

// Player_list ADT interface

int new_player_list( plist *lista){

    (*lista)= (plist)malloc(sizeof(struct player_list));
    if( *lista == NULL) return 1;

    (*lista)-> head= NULL;
    (*lista)-> tail= NULL;
    return 0;
}

int order_player_in_list( plist *lista, nplayer *new_node, int score){

    int ciclo= 1;
    int comp_node_score= 0;
    nplayer node= (*lista)-> head;
    nplayer node_shadow= node;
    nplayer point_saver= NULL;

    while( node != NULL && ciclo != 0){ // cerco dove mettere il nodo

        if( node == *new_node) return 0;
        comp_node_score= get_best_score_from_player( node);
        //printf( "\nbest score so far: %d\n", comp_node_score);
        if( score <= comp_node_score){
            node_shadow= node;
            node= node-> next;
        }
        else ciclo= 0;
    }

    point_saver= (*new_node)-> previews;        // serve ad evitare che spostando il nodo possano rimanere collegamenti i nodi precedenti
    if( point_saver != NULL){
        point_saver-> next= (*new_node)-> next;

        if( point_saver-> next == NULL){
            (*lista)-> tail= point_saver;
        }
        else ((*new_node)-> next)-> previews= point_saver;
    }

    if( node == (*lista)-> head){               // controllo se il nodo è rimasto sulla posizione dellà head
        (*new_node)-> next= node_shadow;
        (*new_node)-> previews= NULL;
        node_shadow-> previews= *new_node;
        (*lista)-> head= *new_node;
    }
    else{                                       // se arriva qui, è in un punto casuale della lista
        if( node_shadow == *new_node) return 0;

        (*new_node)-> next= node_shadow-> next;
        (*new_node)-> previews= node_shadow;
        node_shadow-> next= *new_node;
        if( node != NULL) node-> previews= *new_node;
    }
    return 0;
}

nplayer search_player_in_list( plist lista, char nickname[20]){

    nplayer search= lista-> head;
    int ciclo= 0;
    int comp= 0;

    while( search != NULL){

        comp= 0;
        for(ciclo= 0; ciclo< 20; ciclo++){
            if( nickname[ciclo] != ' ' && (search-> nickname[ciclo]) != ' '){
                if( nickname[ciclo] != (search-> nickname[ciclo])) comp= 1;
            }
        }
        if( comp == 0) return search;
        else search= search-> next;
    }
    return NULL;
}

int add_score_to_player_in_list( plist *lista, char nickname[20], int score){

    int best_score= 0;
    nplayer ascore= search_player_in_list( *lista, nickname);
    if( ascore == NULL) return 1;

    best_score= get_best_score_from_player( ascore);
    add_score( &(ascore-> under), score);

    if( score > best_score){
        order_player_in_list( &(*lista), &ascore, score);
    }
    return 0;
}

nplayer add_player_to_list( plist *lista, char nick[20], int score){

    if( *lista == NULL) return NULL;

    nplayer check= search_player_in_list( *lista, nick);
    if( check != NULL){
        add_score_to_player_in_list( lista, nick, score);
        return check;
    }

    nplayer copy= NULL;
    nplayer newp= NULL;
    add_player( &newp, nick, score);
    if( newp == NULL) return NULL;

    if( (*lista)-> head == NULL){
        (*lista)-> head= newp;
    }
    else{
        order_player_in_list( &(*lista), &newp, score);
    }
    copy= newp;
    while( copy-> next != NULL) copy= copy-> next;
    (*lista)-> tail= newp;

    return newp;
}

void print_best_player_score_in_list( plist lista, char nickname[20]){

    int best_score= 0;
    nplayer print= search_player_in_list( lista, nickname);
    if( print == NULL){
        printf(" No player found");
        return;
    }

    best_score= get_best_score_from_player( print);
    print_player_name( print);
    printf("\nBest score: %d\n", best_score);
    return;
}

int remove_player_from_list( plist *lista, nplayer *player){

    if( *player == NULL) return 1;
    nplayer search= (*lista)-> head;
    nplayer prev= NULL;

    while( search != *player){
        prev= search;
        search= search-> next;
    }

    if( search != (*lista)-> head){
        prev-> next = search-> next;
        if( search-> next != NULL){
            search-> next-> previews = prev;
        }
        if( search == (*lista)-> tail) (*lista)-> tail= prev;
    }
    else{
        if( search-> next != NULL){
            (*lista)-> head= search-> next;
            search->next->previews = NULL;
        }
        else{
            (*lista)-> head= NULL;
            (*lista)-> tail= NULL;
        }
    }
    delete_player( &search);
    *player= NULL;

    return 0;
}

int remove_score_from_player_list( plist *lista, nplayer player){

    nplayer rscore= player;
    if( rscore == NULL) return 1;
    del_score( &(rscore-> under));
    return 0;
}

int end_player_list( plist *lista){

    if( *lista == NULL) return 1;
    if( (*lista)-> head == NULL){
        free( *lista);
        return 0;
    }
    nplayer del_shadow= NULL;
    nplayer del= NULL;

    while( *lista != NULL){
        del= (*lista)-> head;
        del_shadow= del;

        if( del == NULL){
            free( *lista);
            return 0;
        }
        while( del != NULL){
            del_shadow= del;
            del= del-> next;
        }
        del= del_shadow;
        if( del == (*lista)-> head){
            delete_player( &del);
            (*lista)-> head= NULL;
        }
        else{
            del_shadow= del_shadow-> previews;
            delete_player( &del);
            if( del_shadow != NULL) del_shadow-> next= NULL;
        }
    }
    free( *lista);
    return 0;
}

void print_players_list( plist lista){

    if( lista == NULL) printf("\n\nNon esiste una player_list\n\n");

    nplayer copy= lista-> head;
    while( copy != NULL){
        print_player( copy);
        copy= copy-> next;
    }
}

void print_player_from_list( plist lista, char nickname[20]){

    nplayer print= search_player_in_list( lista, nickname);
    if( print == NULL) printf("\n\nNo player found\n\n");
    else print_player( print);
}

int nprint_player_from_list( plist lista, char nickname[20], WINDOW *win, int Max_y, int Max_x){

    int ciclo_save= 0;

    nplayer nprint= search_player_in_list( lista, nickname);
    if( nprint == NULL) mvwprintw( win, Max_y/2, (Max_x/2)-7 , "No player found");
    else ciclo_save= nprint_player( nprint, &(*win), Max_y, Max_x);
    return ciclo_save;
}

void nprint_players_list( plist lista, WINDOW *win, int Max_y, int Max_x){

    if( lista == NULL || lista-> head == NULL){
        mvwprintw( win, Max_y/2, (Max_x/2)-14, "Non esistena una player_list");
        return;
    }

    int list_pos= 1;
    int cord_y= 2;
    int cord_x= 3;

    nplayer copy= lista-> head;
    int score_to_print= 0;

    while( copy != NULL){
        mvwprintw( win, cord_y, cord_x, "%d / ", list_pos);
        nprint_player_name( copy, &(*win));
        score_to_print= get_best_score_from_player( copy);
        wprintw( win, "  Score: %d", score_to_print);

        cord_y+= 2;
        if( cord_y >= Max_y-1){
            cord_y= 2;
            cord_x+= 39;
            if( cord_x >= Max_x) return;
        }

        list_pos+= 1;
        copy= copy-> next;
    }
}

int save_player_list(plist lista){

    FILE *file = fopen("listsave.txt", "w");
    if (file == NULL) return 1;

    if( lista == NULL || lista-> head == NULL){
        remove( "listsave.txt");
        return 1;
    }
    int ciclo= 0;
    int end_score_marker= -1;
    int end_of_list= -2;
    nplayer node = lista->head;
    while(node != NULL){

        for( ciclo= 0; ciclo < 20; ciclo++ ){
            fprintf( file, "%c", node-> nickname[ciclo]);
        }
        node-> nickname[20]= '\0';
        fprintf( file, "\n");

        pscores pscore_node = node->under;
        while(pscore_node != NULL){
            fscore scoreinfo= pscore_node-> pscore;
            fprintf( file, "%d %ld\n", scoreinfo-> score, scoreinfo-> date);
            pscore_node = pscore_node->under;
        }

        if( node-> next == NULL) fprintf( file, "%d", end_of_list);
        else fprintf( file, "%d", end_score_marker);

        node = node->next;
    }

    fclose(file);
    return 0;
}

int load_player_list(plist *lista){

    FILE *file = fopen("listsave.txt", "r");
    if (file == NULL) return 1;

    if( *lista == NULL) new_player_list( &(*lista));

    if( *lista == NULL){
        fclose(file);
        return 1;
    }

    int ciclo;
    char nick_copy[20];
    int score_copy= 0;
    time_t date_copy;
    nplayer player= NULL;
    pscores scores= NULL;

    while(!feof(file)){

        for( ciclo= 0; ciclo < 20; ciclo++ ){
            fscanf( file, "%c", &nick_copy[ciclo]);
        }
        nick_copy[20]= '\0';

        fscanf( file, "%d %ld\n", &score_copy, &date_copy);

        player= add_player_to_list( &(*lista), nick_copy, score_copy);

        scores= player-> under;
        set_date( &(scores-> pscore), date_copy);

        while(1){

            fscanf( file, "%d", &score_copy);
            if( score_copy != -1 && score_copy != -2){
                add_score_to_player_in_list( &(*lista), nick_copy, score_copy);

                scores= player-> under;
                fscanf( file, " %ld\n", &date_copy);
                set_date( &(scores-> pscore), date_copy);
            }
            else break;
        }
        if( score_copy == -2) break;
    }

    fclose(file);
    return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------
