#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <math.h>
#include "Tengine_core.h"
#include "Tengine_scoreboard.h"

void menu( int *a, int *My, int *Mx){

	int i= 1;
	int choice= 0;
	int arrow;

	move( (*My)/4, (*Mx)/4);

	printw("Tetris");
	mvchgat( (*My)/4, (*Mx)/4, 1, A_BOLD, 1, NULL);
	mvchgat( (*My)/4, ((*Mx)/4)+1, 1, A_BOLD, 2, NULL);
	mvchgat( (*My)/4, ((*Mx)/4)+2, 1, A_BOLD, 3, NULL);
	mvchgat( (*My)/4, ((*Mx)/4)+3, 1, A_BOLD, 6, NULL);
	mvchgat( (*My)/4, ((*Mx)/4)+4, 1, A_BOLD, 4, NULL);
	mvchgat( (*My)/4, ((*Mx)/4)+5, 1, A_BOLD, 1, NULL);
	mvchgat( (*My)/4, ((*Mx)/4)+6, 1, A_BOLD, 5, NULL);




	keypad( stdscr, true);

	while( i== 1){

		mvprintw( ((*My)/4)+2, ((*Mx)/4)+1, " Play            ");
		if(choice== 0) mvchgat( ((*My)/4)+2, ((*Mx)/4)+1, 17, A_REVERSE, 3, NULL);

		mvprintw( ((*My)/4)+3, ((*Mx)/4)+1, " Continue        ");
		if(choice== 1) mvchgat( ((*My)/4)+3, ((*Mx)/4)+1, 17, A_REVERSE, 6, NULL);

		mvprintw( ((*My)/4)+5, ((*Mx)/4)+1, " LeaderBoard     ");
		if(choice== 2) mvchgat( ((*My)/4)+5, ((*Mx)/4)+1, 17, A_REVERSE, 1, NULL);

		mvprintw( ((*My)/4)+7, ((*Mx)/4)+1, " Calibrate       ");
		if(choice== 3) mvchgat( ((*My)/4)+7, ((*Mx)/4)+1, 17, A_REVERSE, 4, NULL);

		mvprintw( ((*My)/4)+8, ((*Mx)/4)+1, " Exit            ");
		if(choice== 4) mvchgat( ((*My)/4)+8, ((*Mx)/4)+1, 17, A_REVERSE, 2, NULL);


		mvprintw( (*My)-((*My)/4), ((*Mx)/3)-((*Mx)/4), "In questa versione del tetris, la difficolta aumenta o diminuisce");
		mvprintw( (*My)-((*My)/4)+1, ((*Mx)/3)-((*Mx)/4), "in base alla grandezza del terminale");
		mvprintw( (*My)-((*My)/4)+2, ((*Mx)/3)-((*Mx)/4), "Calibrate serve a dare al programma le dimensioni della finestra, nel caso vengano cambiate tramite CTRL + rotellina del mouse");
		mvprintw( (*My)-((*My)/4)-5, ((*Mx)/3)-((*Mx)/4), "I comandi per controllare il tetramino sono: 'X' per ruotare e (DOWN RIGT LEFT)ARROW per spostare");
		mvprintw( (*My)-((*My)/4)-3, ((*Mx)/3)-((*Mx)/4), "Per chiamare il menu durante la partita, premere ESC");

		mvprintw( (*My)-((*My)/4)+3, ((*Mx)/3)-((*Mx)/4), "Per l'esperienza standard del tetris mettere il terminale in grandezza 145x42 in 16:10, o in  157x38 in 16:9");
		mvprintw( (*My)-2, (*Mx)-4, ":D");
		refresh();

		arrow= wgetch(stdscr);
		switch(arrow){

			case KEY_UP:
				choice--;
				if(choice< 0) choice= 0;
				break;

			case KEY_DOWN:
				choice++;
				if(choice> 4) choice= 4;
				break;

			case 10:
				i= 0;
				break;

			default:
		}
	}

	switch(choice){

		case 0:
			*a= 1;
			clear();
			break;

		case 1:
			*a= 2;
			clear();
			break;

		case 2:
			*a= 3;
			clear();
			break;

		case 3:
			getmaxyx( stdscr, (*My), (*Mx));
			clear();
			break;

		case 4:
			*a= -1;
			break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

nplayer game( int *a, int My, int Mx, nplayer last_player, int *last_score, plist *leadboard){

	int i= 1;
	int error_check= 0;
	int error_check2= 0;
	int load= *a;
	*a= 1;

	int gameMyload= 0;
	int gameMxload= 0;


	int gameMy= My-(My/3);
	gameMy= ceill( gameMy/4)*4;

	int gameMx= (Mx/2)-(Mx/3);
	gameMx= ceill( gameMx/4)*4;

	if( (Mx<= 70) || (My<= 30)){

		mvprintw( My/2, (Mx/2)-(Mx/3), "Schermo troppo piccolo, ricalibrare per continuare");
		refresh();
		wgetch( stdscr);
		clear();
		*a= 0;
	}

	if( (*a)== 1){

//------------------------------------------------------------------------------------------------------------------

		// Inizializazione variabili strutture dati


		int game_cycle= 0;
		int tetramino_state= 0;
		int randcolor;

		int new_level= 0;
		int game_level= 0;

		int score= 0;
		int score_copy= 0;

		int move;
		int spam_counter= 0;
		int delay= 400;

		int line_check= 0;
		int check_ciclo= 0;

		int pause_menu_call= 0;
		int pause_choice= 0;
		int pMy, pMx;

		int replay= 0;
		int replay_choice= 0;

		int conferma= 0;
		int conferma_re= 0;
		int conferma_choice= 0;

		nplayer player= last_player;
		char nickname[20];

//------------------------------------------------------------------------------------------------------------------

		// Inizializazione strutture dati

		vwin gamevwin= NULL;
		tqueue coda= NULL;
		types tetra_type;

		tetramino one= NULL;
		tetramino two= NULL;

		if( load != 2){
			error_check= new_virtual_window( gameMy-2, (gameMx/2)-1, &gamevwin, 0, NULL, NULL);
		}
		else{
			error_check= load_virtual_window( &gamevwin, &gameMyload, &gameMxload, &one);

			gameMy= gameMyload+2;
			gameMx= (gameMxload*2)+2;
			tetramino_state= 2;
			loadscore( &score);
		}

		if( error_check == 1){
			*a= 0;
			return NULL;
		}
		error_check2= new_tetramino_queue( &coda);
		if( error_check2 == 2){
			*a= 0;
			return NULL;
		}


//------------------------------------------------------------------------------------------------------------------

		// Inizializazione finestre e musica

		refresh();

		WINDOW* gamewin= newwin( gameMy, gameMx, (My/3)-(My/4), (Mx/2));
		box( gamewin, 0, 0);
		keypad( gamewin, true);
		mvwchgat( gamewin, 1, 1, 4, A_BOLD, 2, NULL);


		WINDOW* scorewin= newwin( 3, gameMx, (My/3)-(My/4)+(gameMy+1), (Mx/2));
		box( scorewin, 0, 0);
		mvwprintw( scorewin, 1, 1, "SCORE: %d", score);
		wrefresh( scorewin);


		WINDOW* blockprevwin= newwin( (My/2)-(My/3)+1, 20, (My/3)-(My/4)+3, (Mx/2)-25);


		WINDOW* pausemenu= newwin( 7, 16, (My/3)+2,(Mx/2)-22);
		keypad( pausemenu, true);
		getmaxyx( pausemenu, pMy, pMx);

		Mix_Music *music = Mix_LoadMUS("Tetris.mp3");

//------------------------------------------------------------------------------------------------------------------

		// Game cycle

		Mix_PlayMusic(music, -1); // MUSICA

		while( game_cycle != -1 && game_cycle != -2){

			mvwprintw( gamewin, 0, 2, "LEVEL %d", game_level);

			if( tetramino_state == -1) tetramino_state= 0;

			get_tetramino_info_from_queue( &coda, &two);
			if( tetramino_state != 2){

				if( one != NULL) delete_tetramino( &one);
				dequeue_tetramino( &coda, &one);
				tetra_type= (types)(rand() % 7);
				add_in_tetramino_queue( &coda, tetra_type);
				game_cycle= create_block( &one, gameMy-2, (gameMx/2), &gamevwin);
				//mvwprintw( gamewin, 0, gameMx-2, "%d", game_cycle); print di debbug
			}
			else{
				tetramino_state= 0;
			}
			randcolor= (int)(rand() % 4)+1;
			nprint_virtual_window( gamevwin, gamewin, randcolor);
			nprint_tetramino_in_prevscreen( blockprevwin, two);
			box( blockprevwin, 0, 0);
			mvwprintw( blockprevwin, 0, 2, "Block preview");
			wrefresh( blockprevwin);
			wrefresh( gamewin);

			while( tetramino_state != -1){

				if( score <= 7000) wtimeout( gamewin ,delay- (score/20));
				else wtimeout( gamewin, delay- 350);
				move= wgetch( gamewin);

				//mvwprintw( gamewin, 0, 10, "%d", move);   // print di debbug

				if( move == (int)'x' && spam_counter < 5){
					rotate_block_clockwise( &one, &gamevwin);
					spam_counter++;
				}
				else if( move == 258 || move == 261 || move == 260){
					if( spam_counter < 5){
					tetramino_state= shift_block( &one, move, &gamevwin);
					spam_counter++;
					}
					else{
						tetramino_state= shift_block( &one, 258, &gamevwin);
						spam_counter= 0;
					}
				}
				else if( move == 27){

					//Pause menu call

					Mix_PauseMusic();
					pause_menu_call= 1;
					while( pause_menu_call== 1){

						box( pausemenu, 0, 0);
						mvwprintw( pausemenu, 1, (pMx/2)-5, "Pause Menu");
						mvwchgat( pausemenu, 1, (pMx/2)-5, 10, A_BOLD, 1, NULL);

						mvwprintw( pausemenu, 3, (pMx/2)-(pMx/3), "Resume  ");
						if(pause_choice== 0) mvwchgat( pausemenu, 3, (pMx/2)-(pMx/3), 8, A_REVERSE, 3, NULL);

						mvwprintw( pausemenu, 4, (pMx/2)-(pMx/3), "Exit    ");
						if(pause_choice== 1) mvwchgat( pausemenu, 4, (pMx/2)-(pMx/3), 8, A_REVERSE, 2, NULL);

						wrefresh( pausemenu);
						move= wgetch( pausemenu);
						switch( move){

							case 259:
								pause_choice--;
								if( pause_choice< 0) pause_choice= 0;
								break;

							case 258:
								pause_choice++;
								if( pause_choice> 1) pause_choice= 1;
								break;

							case 10:
								if( pause_choice == 0){
									tetramino_state= 0;
									pause_menu_call= 0;
									Mix_ResumeMusic();
								}
								if( pause_choice == 1){
									save_virtual_window( gamevwin, gameMy-2, (gameMx/2)-1, one);
									savescore( score);
									tetramino_state= -1;
									pause_menu_call= 0;
									game_cycle= -2;
								}
								break;

							default: break;
						}
					}
					wclear( pausemenu);
					wrefresh( pausemenu);
				}

				//Pause menu call end

				else{
					tetramino_state= shift_block( &one, 258, &gamevwin);
					spam_counter= 0;
				}
				nprint_virtual_window( gamevwin, gamewin, randcolor);
				wrefresh( gamewin);
				wrefresh( blockprevwin);
			}

			//Score calculator

			check_line( &line_check, (gameMx/2)-1, &gamevwin);
			if( line_check != 0) check_ciclo= 1;

			while( check_ciclo == 1){

				check_line( &line_check, (gameMx/2)-1, &gamevwin);
				if( line_check != 0){
					while( line_check != 0){
						if( line_check != 1) score+= 100 + (line_check*100);
						else score+= 100;
						line_check-= 1;
					}
					check_ciclo= 1;
				}
				else check_ciclo= 0;
			}
			mvwprintw( scorewin, 1, 1, "SCORE: %d", score);

			//Level counter

			new_level= 0;
			for( score_copy= score; score_copy > 0; score_copy-= 1000) new_level++;
			game_level= new_level;
			mvwprintw( gamewin, 0, 2, "LEVEL %d", game_level);
			wrefresh( scorewin);


			//End of game

			if( game_cycle == -2){
				*a= 0;
				wtimeout( gamewin, -1);
				end_virtual_window( &gamevwin);
				end_tetramino_queue( &coda);
				delete_tetramino( &one);
				Mix_FreeMusic(music);
				clear();
				wrefresh( gamewin);
				wrefresh( scorewin);
				wrefresh( blockprevwin);
			}
			if( game_cycle == -1){
				wtimeout( gamewin, -1);
				mvwprintw( gamewin, gameMy/2, (gameMx/2)-6 , " GAME OVER");
				wrefresh( gamewin);
				remove( "vwinscreen.txt");
				remove( "vwin_pointers.txt");
				remove( "tetramino_info.txt");
				remove( "scoresave.txt");
				Mix_PauseMusic();
				wgetch( gamewin);

				if( player == NULL){

					while( conferma == 0){
						clear();
						curs_set(1);
						echo();
						keypad( stdscr, FALSE);
						mvprintw( My/2, (Mx/2)-27, "Inserisci il tuo nickname: ");
						ngetcharArray( nickname);
						keypad( stdscr, TRUE);
						clear();
						refresh();

						conferma_re= 0;
						while( conferma_re != 1){

							mvprintw( (My/2)-2, (Mx/2)-5, "Confermi ?");

							mvprintw( My/2, (Mx/2)- 6, "NO");
							if( conferma_choice== 0) mvchgat(  My/2, (Mx/2)- 6, 2, A_REVERSE, 6, NULL);


							mvprintw( My/2, (Mx/2)+ 4, "SI");
							if( conferma_choice== 1) mvchgat(  My/2, (Mx/2)+ 4, 2, A_REVERSE, 3, NULL);

							curs_set(0);
							refresh();
							conferma= getch();
							switch( conferma){

								case 260:
									conferma_choice--;
									if( conferma_choice< 0) conferma_choice= 0;
									break;

								case 261:
									conferma_choice++;
									if( conferma_choice> 1) conferma_choice= 1;
									break;

								case 10:
									conferma= conferma_choice;
									conferma_re= 1;
							}
							keypad( stdscr, TRUE);
							noecho();
							clear();
						}
					}
					player= add_player_to_list( &(*leadboard), nickname, score);
					*last_score= score;
				}
				else{
					add_score_to_player_in_list( &(*leadboard), nickname, score);
				}

				mvprintw( (My/2)-2, (Mx/2)-4, "REPLAY ?");

				while( replay != 1){

					mvprintw( My/2, (Mx/2)- 6, "NO");
					if(replay_choice== 0) mvchgat(  My/2, (Mx/2)- 6, 2, A_REVERSE, 4, NULL);


					mvprintw( My/2, (Mx/2)+ 4, "SI");
					if(replay_choice== 1) mvchgat(  My/2, (Mx/2)+ 4, 2, A_REVERSE, 1, NULL);

					refresh();
					replay= getch();
					switch( replay){

						case 260:
							replay_choice--;
							if( replay_choice< 0) replay_choice= 0;
							break;

						case 261:
							replay_choice++;
							if( replay_choice> 1) replay_choice= 1;
							break;

						case 10:
							end_virtual_window( &gamevwin);
							end_tetramino_queue( &coda);
							delete_tetramino( &one);
							Mix_FreeMusic(music);
							clear();
							wrefresh( gamewin);
							wrefresh( scorewin);
							wrefresh( blockprevwin);

							if( replay_choice == 0){
								*a= 0;

							}
							if( replay_choice == 1);

							replay= 1;
							break;

						default: break;
					}
				}

			}
		}
		*last_score= score;
		return player;
	}
}



//--------------------------------------------------------------------------------------------------------------------------------------------

void leadbd( int *a, int My, int Mx, nplayer *last_player, int last_score, plist *leadboard, WINDOW *leaderboard_win, WINDOW *yourscores){


// Inizializazione

	int ciclo= 0;
	char pressed_key= 0;

//------------------------------------------------------------------------------------------------------------------

// LeaderBoard

	wclear( leaderboard_win);

	int Maxy_leadb, Maxx_leadb;

	mvprintw( 2, (Mx/2)-(Mx/10), "LEDEARBOARD");

	getmaxyx( leaderboard_win, Maxy_leadb, Maxx_leadb);

	if( (*leadboard)-> head != NULL){
		nprint_players_list( *leadboard, leaderboard_win, Maxy_leadb, Maxx_leadb);
	}
	box( leaderboard_win, 0, 0);

//------------------------------------------------------------------------------------------------------------------

// PersonalBoard

	wclear( yourscores);

	int score_count= 0;
	int Maxy_yourb, Maxx_yourb;

	mvprintw( 2, 7, "LAST SCORE: %d", last_score);

	getmaxyx( yourscores, Maxy_yourb, Maxx_yourb);

	if( last_player != NULL) score_count= nprint_player( *last_player, yourscores, Maxy_yourb, Maxx_yourb);
	if( score_count >= 10) remove_score_from_player_list( &(*leadboard), *last_player);

	box( yourscores, 0, 0);

//------------------------------------------------------------------------------------------------------------------

// General setups;

	mvprintw( My-3, Mx-52, "Per rimuovere il tuo nome dalla lista premi 'r'");
	mvprintw( My-2, Mx-33, "Per tornarnare al menu premi ESC");

	refresh();
	wrefresh( yourscores);
	wrefresh( leaderboard_win);

	while( (int)(pressed_key) != 27 && pressed_key != 'r'){
		pressed_key= getchar();
		if( pressed_key == 'r'){
			*a= 4;
		}
		if( (int)(pressed_key) == 27){
			*a= 0;
		}
	}
	clear();
	refresh();
	wrefresh( yourscores);
	wrefresh( leaderboard_win);
}

//------------------------------------------------------------------------------------------------------------------


int main(){

	int error_check= 0;

// LeaderBoard Inizializazione

	int last_score= 0;
	plist leadboard= NULL;
	nplayer last_player_save= NULL;
	nplayer last_player= NULL;
	error_check= new_player_list( &leadboard);
	if( error_check == 1) exit(1);
	error_check= load_player_list( &leadboard);

// Prgram control and scale values

	int block= 0;
	int My, Mx;

// Starting musiclib

	SDL_Init(SDL_INIT_AUDIO);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

// Starting ncurses

	initscr();
	start_color();
	noecho();
	curs_set(0);

        init_pair(1, COLOR_YELLOW, 0);
        init_pair(2, COLOR_GREEN, 0);
        init_pair(3, COLOR_RED, 0);
        init_pair(4, COLOR_BLUE, 0);
		init_pair(5, COLOR_WHITE, 0);
		init_pair(6, COLOR_MAGENTA, 0);


	getmaxyx( stdscr, My, Mx);

//----------------------------------------------------------------------------------------------------------------

// Start

	move( My/2, (Mx/2)- 5);
	printw("Benvenuto!");
	refresh();
	getch();
	clear();

	while( block!= -1){

		switch( block){

			case 0:
				menu( &block, &My, &Mx);
				last_player_save= NULL;
				break;

			case 1:
				last_player_save= game( &block, My, Mx, last_player_save, &last_score, &leadboard);
				if( last_player_save != NULL) last_player= last_player_save;
				break;

			case 2:
				last_player_save= game( &block, My, Mx, last_player_save, &last_score, &leadboard);
				if( last_player_save != NULL) last_player= last_player_save;
				break;

			case 3:
				WINDOW* leaderboard_win= newwin( My-(My/4), Mx-(Mx/3)-(Mx/12), (My/3)-(My/4), (Mx/2)-(Mx/9));
				WINDOW* yourscores= newwin( My-(My/4), (Mx/3), (My/3)-(My/4), 5);

				leadbd( &block, My, Mx, &last_player, last_score, &leadboard,leaderboard_win, yourscores);
				if( block == 4){
					remove_player_from_list( &leadboard, &last_player);
					block= 3;
				}
				break;
		}
	}

// Game exit screen

	clear();
	mvprintw( My/2, Mx/2-10, "Alla prossima! :D");
	refresh();
	getchar();

// Frees functions
	save_player_list( leadboard);
	SDL_Quit();
	endwin();
	end_player_list( &leadboard);

	return 0;
}
