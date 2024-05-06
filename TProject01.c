#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>


void menu( int *a, int *My, int *Mx){

	int i= 1;
	int choice= 0;
	int arrow;

	move( (*My)/4, (*Mx)/4);

	printw("Tetris");
	mvchgat( (*My)/4, (*Mx)/4, 1, A_BOLD, 1, NULL);
	mvchgat( (*My)/4, ((*Mx)/4)+1, 1, A_BOLD, 2, NULL);
	mvchgat( (*My)/4, ((*Mx)/4)+2, 1, A_BOLD, 3, NULL);
	mvchgat( (*My)/4, ((*Mx)/4)+3, 1, A_BOLD, 4, NULL);
	mvchgat( (*My)/4, ((*Mx)/4)+4, 1, A_BOLD, 1, NULL);
	mvchgat( (*My)/4, ((*Mx)/4)+5, 1, A_BOLD, 2, NULL);
	mvchgat( (*My)/4, ((*Mx)/4)+6, 1, A_BOLD, 3, NULL);




	keypad( stdscr, true);

	while( i== 1){

		mvprintw( ((*My)/4)+2, ((*Mx)/4)+1, " Play          ");
		if(choice== 0) mvchgat( ((*My)/4)+2, ((*Mx)/4)+1, 15, A_REVERSE, 3, NULL);

		mvprintw( ((*My)/4)+3, ((*Mx)/4)+1, " LeaderBoard   ");
		if(choice== 1) mvchgat( ((*My)/4)+3, ((*Mx)/4)+1, 15, A_REVERSE, 1, NULL);

		mvprintw( ((*My)/4)+4, ((*Mx)/4)+1, " Calibrate     ");
		if(choice== 2) mvchgat( ((*My)/4)+4, ((*Mx)/4)+1, 15, A_REVERSE, 4, NULL);

		mvprintw( ((*My)/4)+5, ((*Mx)/4)+1, " Exit          ");
		if(choice== 3) mvchgat( ((*My)/4)+5, ((*Mx)/4)+1, 15, A_REVERSE, 2, NULL);


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
				if(choice> 3) choice= 3;
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
			getmaxyx( stdscr, (*My), (*Mx));
			clear();
			break;

		case 3:
			*a= -1;
			break;
	}
}


void game( int *a, int My, int Mx){

	int i= 1;
	int score= 0;

	if( (Mx<= 70) || (My<= 25)){

		mvprintw( My/2, (Mx/2)-(Mx/3), "Schermo troppo piccolo, ricalibrare per continuare");
		refresh();
		wgetch( stdscr);
		clear();
		*a= 0;
	}

	if( (*a)== 1){
        	WINDOW* gamewin= newwin( My-(My/4), Mx/3, (My/3)-(My/4), (Mx/2)-(Mx/12));

	        box( gamewin, 0, 0);

//		mvwprintw( gamewin, 1, 1, "ciao");
		mvwchgat( gamewin, 1, 1, 4, A_BOLD, 2, NULL);
		mvwprintw( gamewin, 0, 2, "LEVEL 1");
		wrefresh( gamewin);


        	WINDOW* scorewin= newwin( 3, (Mx/2)-(Mx/3), (My/3)+(My/2)+1, Mx/2);

        	box( scorewin, 0, 0);

        	mvwprintw( scorewin, 1, 1, "SCORE: %d", score);
		wrefresh( scorewin);


		WINDOW* blockprevwin= newwin( (My/2)-(My/3), (Mx/2)-(Mx/3)-5, (My/3)-(My/4)+3, (Mx/2)-(Mx/4));

		box( blockprevwin, 0, 0);

		mvprintw( (My/3)-(My/4)+2, (Mx/2)-(Mx/4), "Block preview" );
		wrefresh( blockprevwin);


		WINDOW* pausemenu= newwin( My/4, Mx/4, (My/3), (Mx/2)-(Mx/3)-4);

		int pMy, pMx;
		box( pausemenu, 0, 0);
		getmaxyx( pausemenu, pMy, pMx);

		mvwprintw( pausemenu, 2, (pMx/2)-5, "Pause Menu");
		mvwchgat( pausemenu, 2, (pMx/2)-5, 10, A_BOLD, 1, NULL);

		mvwprintw( pausemenu, 4, (pMx/2)-(pMx/3), "Resume  ");
		mvwchgat( pausemenu, 4, (pMx/2)-(pMx/3), 9, A_REVERSE, 3, NULL);

		mvwprintw( pausemenu, 5, (pMx/2)-(pMx/3), "Exit    ");
//		mvwchgat( pausemenu, 5, (pMx/2)-15, 9, A_BOLD, 2, NULL);

		wrefresh( pausemenu);

        	mvprintw( My-2, Mx-4, ":O");
		refresh();

		getchar();
	}
}


void leadbd( int *a, int My, int Mx){

	mvprintw( 2, (Mx/2)-(Mx/10), "LEDEARBOARD");
	WINDOW* leaderboard_win= newwin( My-(My/4), Mx-(Mx/3)-(Mx/10), (My/3)-(My/4), (Mx/2)-(Mx/9));
	box( leaderboard_win, 0, 0);

	mvprintw( (My/3)-(My/4)-1, 7, "LAST SCORE: 0");
	WINDOW* yourscores= newwin( My-(My/3), (Mx/3), (My/3)-(My/4), 5);
	box( yourscores, 0, 0);

	mvprintw( My-2, Mx-30, "To return to menu click ESC");

	wrefresh( yourscores);
	wrefresh( leaderboard_win);
	refresh();
	getchar();
}



int main(){

	int block= 0;
	int My, Mx;

	initscr();
	start_color();
	noecho();

        init_pair(1, COLOR_YELLOW, 0);
        init_pair(2, COLOR_GREEN, 0);
        init_pair(3, COLOR_RED, 0);
        init_pair(4, COLOR_BLUE, 0);


	getmaxyx( stdscr, My, Mx);

	move( My/2, (Mx/2)- 5);
	printw("Benvenuto!");
	refresh();
	getchar();

	clear();

	while( block!= -1){

		switch( block){

			case 0:
				menu( &block, &My, &Mx);
				break;

			case 1:
				game( &block, My, Mx);
				break;

			case 2:
				leadbd( &block, My, Mx);
				break;
		}
	}

	clear();
	mvprintw( My/2, Mx/2-10, "Alla prossima! :D");
	refresh();
	getchar();

	endwin();

	return 0;
}
