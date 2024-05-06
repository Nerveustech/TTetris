#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>


void menu( int *a, int *My, int *Mx){

	int i= 1;
	int choice= 0;
	int arrow;

	move( (*My)/4, (*Mx)/4);
	wattron( stdscr, A_BOLD);
	printw("Tetris");
	wattroff( stdscr, A_BOLD);

	keypad( stdscr, true);

	while( i== 1){

		if(choice== 0) wattron( stdscr, A_REVERSE);
		mvprintw( ((*My)/4)+2, ((*Mx)/4)+1, "Play");
		wattroff( stdscr, A_REVERSE);

		if(choice== 1) wattron( stdscr, A_REVERSE);
		mvprintw( ((*My)/4)+3, ((*Mx)/4)+1, "LeaderBoard");
		wattroff( stdscr, A_REVERSE);

		if(choice== 2) wattron( stdscr, A_REVERSE);
		mvprintw( ((*My)/4)+4, ((*Mx)/4)+1, "Calibrate");
		wattroff( stdscr, A_REVERSE);

		if(choice== 3) wattron( stdscr, A_REVERSE);
		mvprintw( ((*My)/4)+5, ((*Mx)/4)+1, "Exit");
		wattroff( stdscr, A_REVERSE);

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
			break;

		case 1:
			*a= 2;
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

}


void leadbd( int *a, int My, int Mx){

}



int main(){

	int block= 0;
	int My, Mx;

	initscr();
	noecho();
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
