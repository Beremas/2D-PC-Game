#include <signal.h>
#include "support.h"
#include "game.h"
#include "mydefine.h"


 
int main() {
	
	
	int exit_status;
	FINESTRE *area = NULL; 
	
	/* Verifica che l'inizializzazione di curses sia andata a buon fine */
	if (init_curses()) { 										
		area = (FINESTRE *) malloc(sizeof(FINESTRE));
		/* Crea le aree di gioco/descrizione/informazione */
		make_area_game(area); 			
		/* Se tutte le aree sono state create con successo, allora inizia il gioco */
		if (area->success) {  								
				/* Avvia il cuore del gioco */
				exit_status=game_engine(area); 
				close_curses();	
				switch (exit_status){
					case ERR_WITH_FILE:
						close_curses();
						system("clear");
						LOG_ERR("\nThere was a problem with the file.txt."
						"\nCheck the rules into documents about how create a correctly maze file."
						 );
						pressEnter();
						break;
					case ERR_WITH_WINDOW_NCURSES:
						close_curses();
						system("clear");
						LOG_ERR("\nThere was a WINDOW problem with ncurses. Please check restart you terminal.");						
						pressEnter();
						break;
					case ERR_WITH_FOULDER_FILE:
						close_curses();
						system("clear");
						LOG_ERR("\nThe file game must be into a dir: %s\nCheck into game foulder if there is that direcotry. All file game must be in there.",PATH_FILE_GAME);						
						pressEnter();
						break;	
				}		
				/* Elimina le finestre e libera la memoria */
				delwin(area->info);
				delwin(area->main);
				delwin(area->desc);
				delwin(area->rules);
				free(area);
		}
	}	
	return 1;
}


