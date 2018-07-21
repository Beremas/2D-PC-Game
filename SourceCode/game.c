#include <ncurses.h>
#include "game.h"
#include "support.h"
#include "heap.h"
#include "mydefine.h"


/* Dimensione della mappa. Cartella dalla quale leggere le mappe. Caratteri permessi in lettura.*/
#define MAX_SIZE 30
//#define DIRECTORY_FILE_MAZE "./MazeFileTXT/"
#define MAX_ELEM_ALLOW 9
char allowedCharachters[10] = "qead|_0-#";	


/* ================ LOAD/SAVE MAZE FILE(s) ================ */
/**
 * Leggo la mappa da file Viene creata inizialmente una matrice 30x30, 
 * dopodichè viene letto carattere per carattere dal file e se è un carattere accettato allora
 * finisce in matrice, altrimenti la funzione ritorna con un errore.
 * <<Le regole di creazione mappa sono presenti in un file nella cartella contenente appunto le mappe.>>
 * @param 'game'	-> Struttura di gioco avente un attributo 'phisical_matrix' 
 * 					   contenente appunto la mappa di gioco.
 * @param 'path'	-> Percorso della directory contenente la mappa txt da leggere.
 * @param 'allwd'	-> Array di caratteri accettata dalla lettura [di personale scelta,
 * 					   in base a cosa si voglia inserire nella mappa].
 * @return 'flag'	-> 0 se la mappa è stata caricata correttamente, 
 * 					   -1 mappa non caricata problema con gli allowed caratteri, 
 * 					   -2 se la mappa non rispetta le specifiche di creazione 
 * 							[vedi file txt nella apposita cartelle dei file di creazione di gioco ]
 */
int read_maze_from_file(GAMES* game, char*path,char* allwd){
	FILE *fd;
	int i, max_righe, max_colonne, j,jprec, k,err,per_riga,ok,flag;
	char **new_maze,c;
	
	
	fd = fopen(path, "r");
	if(!fd) {
		return 0;
	}
	i = 0;
	j = 0,jprec=j;
	max_righe = MAX_SIZE;
	max_colonne = MAX_SIZE;
	err = 0;
	per_riga = 0;
	flag=0; 	
	
	
	/* Creating relative matrix to host new maze */
	new_maze = malloc(sizeof(char*)*max_righe);
	for(k=0;k<max_righe;k++)
		new_maze[k] = malloc(sizeof(char)*max_colonne);
	
	/* Reading loop */	
	do {
		c = fgetc(fd); /* Prendo il carattere */
		if(feof(fd)) {
			if(j != per_riga) {
				err = 1;
			}
			break;
		}
		if(i > 0 && j > per_riga) { /* La riga attuale non aveva 'per_riga' colonne */
			err = 1;
			break;
		}
		if(i > max_righe || per_riga > max_colonne) {
			err = 1;
			break;
		}
		if(c == '\n' || c == '\r') { /* Se e' un 'a capo', allora */
			 /* Se ancora non ho scritto nessuna linea,
			 ripeti il ciclo senza considerare
			 questo 'a capo' iniziale */
			if(j == 0) {
				continue;
			}
			if(i == 0) { /* Ho terminato la prima riga, ogni riga deve avere 'per_riga' colonne */
				per_riga = j;
			}
			if(j < per_riga) { /* La riga attuale non aveva 'per_riga' colonne */
				err = 1;
				break;
			}
			i++;
			jprec=j;
			j = 0;
		} else { /* Altrimenti se non e' un 'a capo' */
			ok=0;
			for(int k=0;k<MAX_ELEM_ALLOW;k++){
				if( allwd[k] == c ) {
					ok=1;
					break;
				}
			}		
			if(ok==1){
				new_maze[i][j] = c; /* Inserisco il carattere nella matrice */
			}else{
				err=1;
				break;
			}
			j++; /* Incremento la colonna e continuo */
		}
	} while(!err);
	fclose(fd);
	
	
	// Start questioning about the error 
	
	/* Il maze non rispetta le specifiche di costruzione */
	if( (i < max_righe-1) || (jprec<max_colonne-1) ){
		//free new_maze
		for(j=0;j<max_righe;j++){
			free(new_maze[j]);
		}
		free(new_maze);		
		flag =-2;
	}
	/* Il file non ha all'interno i caratteri consentiti */
	else if((ok == 0) ){
		//free new_maze
		for(j=0;j<max_righe;j++){
			free(new_maze[j]);
		}
		free(new_maze);		
		flag=-1;		
	}else{	
		/* il maze è pulito e sistemato. E' stato letto correttamente  */
		game->maze->phisical_matrix=new_maze;
		game->maze->row = i;
		game->maze->col = jprec;
	}	
	return flag;
	
}
/**
 * @param 'game'	-> Nome del file scritto dall'utente da tastiera.
 * @param 'path'	-> Percorso della directory contenente le mappe txt di gioco.
 * @return 			-> 1 se tutto è stato caricato correttamente , -1 altrimenti.
 */
int load_maze(GAMES *game, char* path) {	
	int flag = read_maze_from_file(game, path,allowedCharachters);
	if ( ( flag == -1) || (flag == -2) ) return -1;	
	return 1;
}


/* ====================== SHOW FUNCTIONS ================== */
/**
 * Funzione che si attiva a superamento livello. Mostra il resoconto della partita, 
 * Oggetti presi, il tempo speso,..
 * @param 'area'	-> Nome della finestra all'interno della verra poi stampato il resoconto.
 * @param 'game'	-> Struttura del gioco avente tra i vari suoi attributi quelli utili per il resoconto.
 * @param 'player'	-> Struttura del giocatore contenente attributi utili per il resoconto.
 * @return 			
 */
void show_result_of_game(WINDOW *area,GAMES* game,CHARACTER* player){
	werase(area);
	wattron(area,A_BLINK | A_BOLD| COLOR_PAIR(15));
	mvwprintw(area,0,0,"Result:");
	wattroff(area,A_BLINK | A_BOLD| COLOR_PAIR(15));
	
	if(game->game_over == WIN ){
		mvwprintw(area,1,0,"You win. You ran away!\n");
		wattron(area,A_BLINK | A_BOLD| COLOR_PAIR(15));
		mvwprintw(area,3,0,"Details:");
		wattroff(area,A_BLINK | A_BOLD| COLOR_PAIR(15));
			mvwprintw(area,4,0,"Time: %s",	game->time_end);
			mvwprintw(area,5,0,"Life: %d",	player->health);
		mvwprintw(area,10,0,"<< Press any key >>");
	}
	else if(game->game_over == ESC_PRESSED){
		 mvwprintw(area,1,0,"You lose. Quit is not\nconsidered an escape way.\n ");
		 mvwprintw(area,5,0,"<< Press any key >>");
	}
	else if(game->game_over == LOSE){
				mvwprintw(area,1,0,"You lose. Death is not\nconsidered an escape way.\n");
				mvwprintw(area,5,0,"<< Press any key >>");
	}
	// else if() 							Altre modalità di uscità gioco- sviluppi futuri 
	
	
	wrefresh(area);
	getch();
	
	
}
/**
 * Funzione che gestisce lo spostamente grafico del nemico all'interno della mappa.
 * Rimuovendolo dalla vecchia posizione ( quindi sostituendolo con un CORRIDOIO ) 
 * ed inserendolo nella posizione successiva.
 * @param 'area'	-> Nome del riquadro (uno tra i 4 presenti) all'interno del quale 
 * 					   verra poi stampato il nemico.
 * @param 'game'	-> Struttura del gioco avente tra i vari suoi attributi anche un puntatore 
 * 					   alla struttura del nemico, che ha tra i suoi attributi le coordinate 
 * 					   attuali e successive.
 * @return 			
 */
void show_enemy_move(WINDOW* area, GAMES* game){
	int ene_x_curr=game->ptr_enemy->x_curr;
	int ene_y_curr=game->ptr_enemy->y_curr;
	
	int ene_x_next=game->ptr_enemy->x_next;
	int ene_y_next=game->ptr_enemy->y_next;
	
	/*corridoio*/
	wmove(area, ene_x_curr, ene_y_curr);
	wattron(area,COLOR_PAIR(1)); 
	waddch(area, CORRIDOR);		 
	wattroff(area,COLOR_PAIR(1));
	
	/*nemico*/
	wmove(area, ene_x_next, ene_y_next);
	wattron(area,COLOR_PAIR(10)); 
	waddch(area,ENE);
	wattroff(area,COLOR_PAIR(10)); 
}
/**
 * Funzione che gestisce lo spostamente grafico del giocatore all'interno della mappa.
 * Rimuovendolo dalla vecchia posizione ( quindi sostituendolo con un CORRIDOIO ) 
 * ed inserendolo nella posizione successiva.
 * @param 'area'	-> Nome del riquadro (uno tra i 4 presenti) all'interno del quale 
 * 					   verra poi stampato il giocatore.
 * @param 'player'	-> Struttura del giocatore aventi tra gli attributi le coordinate 
 * 					   quelle attuali e le successive.
 * @return 			
 */
void show_player_move(WINDOW *area, CHARACTER *player) {

	/*corridoio*/
	wmove(area, player->x_curr, player->y_curr);
	wattron(area,COLOR_PAIR(1)); 
	waddch(area, CORRIDOR);
	wattroff(area,COLOR_PAIR(1));

	/*giocatore*/
	wmove(area, player->x_next, player->y_next);
	wattron(area,COLOR_PAIR(22)); 
	waddch(area, player->CHARACTER);
	wattroff(area,COLOR_PAIR(22)); 

}
/**
 * Funzione che viene chiamata ad ogni iterazione (ad ogni spostamento del giocatore).
 * Formatta opportunamente gli 'oggetti' per essere presentati a video.
 * @param 'area'	-> Nome del riquadro (uno tra i 4 presenti) all'interno del quale 
 * 					   verra poi stampato il nemico.
 * @param 'game'	-> Struttura del gioco avente tra i suoi attributi, la mappa di gioco.
 * @param 'player'	-> Struttura del giocatore avente tra i suoi attributi gli 'oggetti' che accumula
 * 					   durante la fase di gioco
 * @return 			
 */
void show_game_situation(WINDOW *area, GAMES *game, CHARACTER* player) {
	char buff_reliqs[5];
	char buff_traps[5];
	char buff_health[5];
	char buff_fragm[5];
	char *time_in_game;
	
	
	/* Questa parte viene mostra soltanto se il bottone segreto è stato attivato */
	if (game->bottom_up == 1){
		char buff_bottom[5];
		sprintf(buff_bottom, "%d", game->current_bottom);	
		
		wattron(area,A_BLINK | A_BOLD| COLOR_PAIR(15));
		mvwprintw(area,game->maze->row-7,0,"Secret Bottom: ");
		wattroff(area,A_BLINK | A_BOLD| COLOR_PAIR(15));	
			
		wattron(area, COLOR_PAIR(1));
		mvwprintw(area,game->maze->row-7,MAX_SIZE-strlen(buff_bottom)-5," %s",buff_bottom);
		wattroff(area, COLOR_PAIR(1));
		
	}else{
		//modo barbaro di cancellare la linea. Da fixare
		mvwprintw(area,game->maze->row-7,0,"												");
	}	
	
	
	/* Converte l'intero contente l'informazione del numero dei vari item*/		
	/* ALERT: RIGA VUOTA PER LO SPAWN DEL BOTTONE, QUI NN VA PRINTANTO NIENT ALTRO */
	sprintf(buff_health, "%d", player->health);	
	sprintf(buff_traps,  "%d", player->total_traps);	
	sprintf(buff_fragm,  "%d", player->total_fragments_gained);	
	sprintf(buff_reliqs, "%d", player->total_reliqs_gained);	
	
	
	
	//Secret Botton
	
	//Life
	wattron(area,A_BLINK | A_BOLD| COLOR_PAIR(15));
	mvwprintw(area,game->maze->row-6,0,"Life: ");
	wattroff(area,A_BLINK | A_BOLD| COLOR_PAIR(15));	
			
	wattron(area, COLOR_PAIR(1));
	mvwprintw(area,game->maze->row-6,MAX_SIZE-strlen(buff_health)-5,"%s/%d",buff_health,LIFE);
	wattroff(area, COLOR_PAIR(1));

	//Traps
	wattron(area,A_BLINK | A_BOLD| COLOR_PAIR(15));
	mvwprintw(area,game->maze->row-5,0,"Traps: ");
	wattroff(area,A_BLINK | A_BOLD| COLOR_PAIR(15));	
			
	wattron(area, COLOR_PAIR(1));
	mvwprintw(area,game->maze->row-5,MAX_SIZE-strlen(buff_traps)-5,"%s",buff_traps);
	wattroff(area, COLOR_PAIR(1));
	
	//Fragments
	wattron(area,A_BLINK | A_BOLD| COLOR_PAIR(15));
	mvwprintw(area,game->maze->row-4,0,"Piece of trap: ");
	wattroff(area,A_BLINK | A_BOLD| COLOR_PAIR(15));	
			
	wattron(area, COLOR_PAIR(1));
	mvwprintw(area,game->maze->row-4,MAX_SIZE-strlen(buff_fragm)-5,"%s/%d",buff_fragm,N_FRAGM_TO_CREATE_TRAP);
	wattroff(area, COLOR_PAIR(1));
	
	//Relics
	wattron(area,A_BLINK | A_BOLD| COLOR_PAIR(15));
	mvwprintw(area,game->maze->row-3,0,"Relics: ");
	wattroff(area,A_BLINK | A_BOLD| COLOR_PAIR(15));	
			
	wattron(area, COLOR_PAIR(1));
	mvwprintw(area,game->maze->row-3,MAX_SIZE-strlen(buff_reliqs)-5,"%s/%d",buff_reliqs,N_RELIC);
	wattroff(area, COLOR_PAIR(1));
	
	
	//Time
	time_in_game = make_string_time(game); 
	wattron(area, A_BLINK | A_BOLD | COLOR_PAIR(15));
	mvwprintw(area,game->maze->row-2,0,"Time : ");
	wattroff(area, A_BLINK | A_BOLD | COLOR_PAIR(15));	
	mvwprintw(area,game->maze->row-2,MAX_SIZE-strlen(time_in_game)-2,time_in_game);		
		strcpy(game->time_end,time_in_game);
	free(time_in_game); 
	
	
}
/**
 * Questa funzione effettua il parsing della matrice letta da file, ed applica una struttura
 * grafica seguendo la regola che ad ogni elemento della matrice corrisponde un preciso oggetto grafico.
 * @param 'area'	-> Nome del riquadro (uno tra i 4 presenti) all'interno del quale 
 * 					   verra poi stampato il nemico.
 * @param 'game'	-> Struttura del gioco avente tra i suoi attributi, la mappa di gioco.
 * @return 			
 */
void show_maze(WINDOW *area, GAMES* game){
	int x,y;	
	for (x=0; x < game->maze->row; x++) {
		for (y=0; y < game->maze->col; y++) {
			wmove(area, x, y);
			switch(game->maze->phisical_matrix[x][y]) {
				case 'q':	
					waddch(area,ACS_ULCORNER);
					break;
				case 'e':
					waddch(area,ACS_URCORNER);
					break;
				case 'a':
					waddch(area,ACS_LLCORNER);
					break;
				case 'd':
					waddch(area,ACS_LRCORNER);
					break;
				case '|':
					waddch(area,ACS_VLINE);
					break;
				case '_':
					waddch(area,ACS_HLINE);
					break;
				case '-':
					waddch(area, CORRIDOR);
					break;
				case '#':
					wattron(area,COLOR_PAIR(20));
					waddch(area, DOOR);
					wattroff(area,COLOR_PAIR(20));
					break;
				case '0':					
					wattron(area,COLOR_PAIR(5));
					waddch(area, '0');
					wattroff(area,COLOR_PAIR(5));					
					break;				
				case 'x':	
					wattron(area,COLOR_PAIR(15));				
					waddch(area, RELIC);					
					wattroff(area,COLOR_PAIR(15));
					break;
				case 'b':	
					wattron(area,COLOR_PAIR(4));				
					waddch(area, BULLET);					
					wattroff(area,COLOR_PAIR(4));
					break;
				case 'B':	
					wattron(area,COLOR_PAIR(3));				
					waddch(area, BOTTON_TO_PRESS);					
					wattroff(area,COLOR_PAIR(3));
					break;				
				case '.':	
					wattron(area,COLOR_PAIR(13));				
					waddch(area, PIECE_OF_TRAP);					
					wattroff(area,COLOR_PAIR(13));
					break;	
		
				case 'E':	
					wattron(area,COLOR_PAIR(3));				
					waddch(area, EXIT_DOOR);					
					wattroff(area,COLOR_PAIR(3));
					break;					
			    case 't':
					wattron(area,COLOR_PAIR(16)); 
					waddch(area, TRAP);
					wattroff(area,COLOR_PAIR(16)); 
					break;	
				default:
					wattron(area,COLOR_PAIR(0));				
					waddch(area, ' ' );					
					wattroff(area,COLOR_PAIR(0));
					
			}
		}
	}
	wrefresh(area);		/* Show that box 		*/
}
/**
 * Questa funzione fa parte del 'cuore del gioco'.
 * Viene chiamata ad ogni iterazione per mostrare le modifiche i cambiamenti e le impostazioni legate 
 * a cio che è successo nella mappa.
 * @param 'area'	-> Nome del riquadro (uno tra i 4 presenti) all'interno del quale 
 * 					   verra poi stampato il nemico.
 * @param 'game'	-> Struttura del gioco avente tra i suoi attributi, la mappa di gioco.
 * @param 'player'	-> Struttura del giocatore.
 * @return 			
 */
void show_new_state(FINESTRE *area, GAMES *game, CHARACTER *player) {

	/* Stampa il labirinto aggiornato nell'area di gioco*/
	show_maze(area->main, game);

	/* Stampa il player nell'area di gioco */
	show_player_move(area->main, player);

	/* Stampa il nemico nell'area di gioco */
	show_enemy_move(area->main, game);

	/* Stampa la situazione del game con tutte le caratteristiche  */
	show_game_situation(area->info, game,player);	
	
	
	/* Aggiorna a video l'area info */
	wrefresh(area->info);
	/* Aggiorna a video l'area di gioco */
 	wrefresh(area->main);

}
/**
 * Stampa nella finestra passata come parametro rispettivamente: Autore,Oggetto,Docente.
 * @param 'area'	-> Nome del riquadro (uno tra i 4 presenti) all'interno del quale 
 * 					   verra poi stampato il nemico.
 * @return 			
 */
void show_game_desc(WINDOW *area){	
	wattron(area,A_BLINK | A_BOLD| COLOR_PAIR(14));
	mvwprintw(area,0,0,"Author: Marco Conti N86001613\nSubject: LASD 2017\nProf.: Benerecetti Massimo");
	wattroff(area,A_BLINK | A_BOLD| COLOR_PAIR(14));
	wrefresh(area);
}
/**
 * Stampa nella finestra passata come parametro il messaggio passato come parametro.
 * La funzione termina premendo INVIO.
 * Mostra di volta in volta il livello corrente. 
 * @param 'area'				-> Nome di una delle finestre di gioco.
 * @param 'curr_level_index'	-> Indice numerico del livello corrente 
 * @return 			
 */
void show_pre_game_msg(WINDOW* area,int  curr_level_index){
	
	wattron(area,A_BLINK | A_BOLD| COLOR_PAIR(15));
	mvwprintw(area,5,10,"Level: %d",curr_level_index);
	mvwprintw(area,6,2,"<< Press ENTER to play >>");
	wattroff(area,A_BLINK | A_BOLD| COLOR_PAIR(15));
	wrefresh(area);
	
	while ( getch() != '\n');
	
}
/**
 * Stampa nella finestra passata come parametro il messaggio passato come parametro.
 * La funzione termina premendo INVIO.
 * @param 'area'		-> Nome di una delle finestre di gioco.
 * @param 'msg'			-> Messaggio da presentare a video.
 * @return 			
 */
void show_end_game_msg(WINDOW* area,char* msg){
	
	wattron(area,A_BLINK | A_BOLD| COLOR_PAIR(15));
	mvwprintw(area,5,0,msg);
	mvwprintw(area,10,0,"<< Press ENTER to continue >>"); 
	wattroff(area,A_BLINK | A_BOLD| COLOR_PAIR(15));
	wrefresh(area);
		
	while ( getch() != '\n' );
	
}
/**
 * Stampa nella finestra passata come parametro dei suggerimenti per ricordare all'utente
 * quali sono i tasti per poter interagire col gioco.
 * @param 'area'		-> Nome del riquadro (uno tra i 4 presenti) all'interno del quale 
 * 					   	   verra poi stampato gli hints.
 * @return 			
 */
void show_game_info(WINDOW *area, int curr_level) {
	wattron(area,A_BLINK | A_BOLD| COLOR_PAIR(15));
	mvwprintw(area,0,0,"Hints:");
	wattroff(area,A_BLINK | A_BOLD| COLOR_PAIR(15));
	mvwprintw(area,1,0,"Use keyboard arrow to move\n-Press"
	" 't' to place a TRAP\n-Press SPACE to stop\n-Press ESC to close game\n");
	wattron(area,A_BLINK | A_BOLD| COLOR_PAIR(15));
	mvwprintw(area,7,0,"Current level: %d",curr_level);
	wattroff(area,A_BLINK | A_BOLD| COLOR_PAIR(15));
	
	wrefresh(area);
}
/**
 * Questa funzione mostra nel riquadro scelto semplicemente le regole del gioco. 
 * Qual è lo scopo del gioco e i suggerimenti per poter vincere una partita.
 * @param 'area'		-> Nome del riquadro (uno tra i 4 presenti) all'interno del quale 
 * 					   	   verranno stampate le regole di gioco.
 * @return 			
 */
void show_game_rules(WINDOW *area){
	wattron(area,A_BLINK | A_BOLD| COLOR_PAIR(15));
	mvwprintw(area,0,0,"Game Rules:");
	wattroff(area,A_BLINK | A_BOLD| COLOR_PAIR(15));
	mvwprintw(area,1,0,"Greeting, traveler.\nWelcome into this mystic secret land.\n\n"
	"This adventure will be not easy, but  I'll explain you how to escape on one condition.\n"
	"If you wanna live, follow my\nistructions and everything will be ok.\n"
	"You have to collect all the relics on the ground to let a secret botton\nspawn up, somewhere.\n"
	"Stay on this botton for a while to \ncreate some other relics.\nRepeat these steps until "
	"an escape\ndoor will appare.\n\n"
	"I gave you 1 trap, so you can stop fora while whatever thing is down there."
	" You can create one by youself, picking10 piece of trap spread on the ground.\n"
	"I suggest you run.. you are not alone down there.\n\n");
	 wattron(area,A_BLINK | A_BOLD| COLOR_PAIR(15));		
	 mvwprintw(area,25,0,"Legend:");
	 wattroff(area,A_BLINK | A_BOLD| COLOR_PAIR(15));
	 mvwprintw(area,26,0,"B = Secret Bottom.\n. = Piece of trap.\nx = Relics.\nt = Trap\n\n");
	
	wrefresh(area);
}

/* ====================== TIME CALCULATING ================= */
/**
 * Questa funzione crea sotto forma di stringa, il tempo di gioco trascorso da inizio game.
 * Il tempo è calcolato ogni volta utilizzando la funzione difftime(tempo attuale-tempo iniziale) della 
 * libreria time.h per poter calcolarmi il tempo attuale per poterlo poi approssimare, e formattare nella
 * stringa desiderata.
 * @param 'game'		-> Struttura del gioco contenete tra i vari attributi, uno per il salvataggio
 * 						   del tempo trascorso.
 * @return 'ret'		-> Stringa contenente il tempo in ore:minuti:secondi
 */
char *make_string_time(GAMES *game) {
	long approssimazione;
	char ore[3], minuti[3], secondi[3], stringa[16], *ret;
	double diff_time;

	/* Esegue la sottrazione tra il time() attuale e quello di inizio gioco */
		diff_time = difftime(time(NULL), game->started);
		approssimazione = (long) diff_time; 		

		sprintf(secondi, "%ld", approssimazione%60); approssimazione /= 60;
		sprintf(minuti, "%ld", approssimazione%60); approssimazione /= 60;
		sprintf(ore, "%ld", approssimazione%24); approssimazione /= 24;

		/* Aggiunge uno zero iniziale alle ore se le ore di gioco sono ad una cifra */
		if(strlen(ore) == 1) {
			strcpy(stringa, "0");
			strcat(stringa, ore);
		} else 
			strcpy(stringa, ore);

		strcat(stringa, ":");
		/* Aggiunge uno zero iniziale ai minuti se i minuti di gioco sono ad una cifra */
		if(strlen(minuti) == 1)
			strcat(stringa, "0");

		strcat(stringa, minuti);
		strcat(stringa, ":");
		/* Aggiunge uno zero iniziale ai secondi se i secondi di gioco sono ad una cifra */
		if(strlen(secondi) == 1)
			strcat(stringa, "0");

		strcat(stringa, secondi);
	
	ret = (char *) malloc(sizeof(char)*(strlen(stringa)+1)); /* Alloca la stringa da restituire */
	strcpy(ret, stringa); 		/* Copia il contenuto composto sino ad ora nella stringa appena allocata */
	return ret; 			
}

/* ====================== EVENT LISTNER ==================== */
/**
 * Questa funzione è il "direttore d'orchestra" per così dire, del gioco.
 * E' presente ad ogni iterazione, e quindi ad ogni cambiamento della mappa.
 * Permette di potersi spostare nelle varie fasi di gioco a seconda della situazione
 * accaduta e di modificare la struttura 'game' in base all'accaduto. 
 * 		[Per una chiara spiegazione dell'implementazione e dettagli, guardare la documentazione].
 * @param 'area'		-> Nome del riquadro (uno tra i 4 presenti) all'interno del quale 
 * 					   	   verra poi stampato il nemico.
 * @param 'game'		-> Struttura di gioco avente tra gli attributi quelli modificabili 
 * 						   da questo listner.
 * @param 'player'		-> Struttura del giocatore avente tra gli attributi le 
 * 						   coordinate correnti e successie.
 * @return 			
 */
void events_listener(WINDOW *area, GAMES* game, CHARACTER* player){
	
	switch(game->maze->phisical_matrix[player->x_curr][player->y_curr]) {
		case RELIC:
			player->total_reliqs_gained+=1;
			//Cancello dalla mappa la relic, essendo stato appena catturato dal giocatore.
			game->maze->phisical_matrix[player->x_curr][player->y_curr] = CORRIDOR;			
			break;
		case PIECE_OF_TRAP:
			player->total_fragments_gained+=1;
			//Cancello dalla mappa il fragm, essendo stato appena catturato dal giocatore.
			game->maze->phisical_matrix[player->x_curr][player->y_curr] = CORRIDOR;			
			break;
		case BOTTON_TO_PRESS:
			//Accumulo i tic ottenuti dal player 
			game->current_bottom-=1;
			break;
		case EXIT_DOOR:
			game->game_over=WIN;
			break;
		case ESC:
			game->game_over=ESC_PRESSED;
			break;
	}
	
	/* DIRETTORE D'ORCHESTRA DEL GIOCO  AD OGNI EVENTO CORRISPONDE UNA CONSEGUENZA */	
	
	/* Controllo la vita del player, nel caso sia 0, la partita finisce.*/
	if (player->health <= 0)	game->game_over = LOSE;
	
	/* Controllo se il nemico sta o meno su una trappola.
	 * Il concetto è:
	 * Se il nemico si trova su una trappola e il tempo che deve consumare sulla trappola è finito, allora può muoversi
	 * altrimenti aspetta (quindi debuff del nemico a 1 ) finche il tempo non è finito (duration_debuff == 0 ) quindi 
	 * togli il debuff ( debuff a 0 ) e può nuovamente muoversi.
	 */
	if( game->maze->phisical_matrix[game->ptr_enemy->x_next][game->ptr_enemy->y_next] == TRAP ){		
			/* In questo if entro se il nemico si trova sulla trappola e ho finito il tempo da consumare su quest'ultima, quindi 
			 * resetto tutto togliendo il debuff e ripristinando i tic del debuff e riprendo a inseguire il player
			 * altrimenti se mi trovo sulla trappola e sto ancora con i tic da consumare, lascio il debuff 
			 * impedendo il movimento del nemico.
			 */
			if( game->ptr_enemy->duration_debuff == 0 ){	
					game->ptr_enemy->debuff= 0;	
					// Resetto i tic che il nemico dovra consumare stando sulla trappola
					game->ptr_enemy->duration_debuff=TIC_ON_TRAP;
					// Tolto la trappola ormai consumata dalla mappa
					game->maze->phisical_matrix[game->ptr_enemy->x_next][game->ptr_enemy->y_next] = CORRIDOR;
			}else game->ptr_enemy->debuff = 1;
	}
		
	/* Controllo se il nemico ti ha acciuffato. In caso positivi si attiva il modulo di respawn random per il player */
	if(	game->ptr_enemy->x_next == player->x_next && game->ptr_enemy->y_next == player->y_next ){
			player->health-=1;			
			respawn_player_somewhere(game,player);
	}
	
	/* Se i frammenti raccolti sono uguali a quella costante allora avrò creato una trappola */
	if( player->total_fragments_gained == N_FRAGM_TO_CREATE_TRAP ){
		player->total_traps++;
		player->total_fragments_gained = 0;		
	}
	

	/* Controllo di avanzamento stato partita. 
	 * Ogni volta che prendo le relics sul campo spwana il bottone. Dopo il tot tic consumato sul bottone ricompaiono 
	 * le relics che andranno riprese per far spawnare nuovamente il bottone.
	 * Ripeto tutto ciò finchè nn spawna la porta di uscita
	 */
	int stop_spawn_Time_to_exit=-1;	
	if(game->times_to_exit == HOW_MANY_CICLE_B4_EXIT){		
		stop_spawn_Time_to_exit=1;
	}else{
		//  entro se le relics raccolte sono = al totale di relics richieste e non è ancora tempo di far spawnare la exitdoor
		if((game->total_reliqs  ==  player->total_reliqs_gained) && (stop_spawn_Time_to_exit==-1)){
			if(game->bottom_up == 0){
				init_secret_bottom(game);				
			}
			//  entro se il tempo sul bottone è finito e non è ancora tempo di far spawnare la exitdoor
			if((game->current_bottom == 0) && (stop_spawn_Time_to_exit==-1)){	
				player->total_reliqs_gained=0;
				game->current_bottom = TIC_ON_BOTTON;			
				game->bottom_up = 0;				

				game->maze->phisical_matrix[player->x_curr][player->y_curr] = CORRIDOR; 
				game->times_to_exit+=1;		
				if(game->times_to_exit != HOW_MANY_CICLE_B4_EXIT ) init_object(game,N_RELIC,RELIC);	
			}
			
		}
	}
	// qui entro se la exit door deve spawnare 
	if (stop_spawn_Time_to_exit == 1 ){
		if(game->exit_up != 1){
			init_exit_door(game);
			game->exit_up = 1;
		}
	}
	
	
		
	
	
}
/**
 * Questa funzione permette di catturare l'input da tastiera dell'utente.
 * Permette quindi di catturare i seguenti tasti:
 * 	- tutte e 4 frecce direzionali
 * 	- il tasto ESC
 * 	- il tasto SPACE
 * 	- il tasto TRAP ('t')
 * wgetch è non bloccate, ritorna se non è premuto niente, permettendo così ad eventuali
 * AI presenti nel gioco di poter computare senza dover attendere l'utente.
 * @param 'area'	-> Nome del riquadro (uno tra i 4 presenti) all'interno del quale 
 * 					   catturare l'input.
 * @param 'game'	-> Struttura del gioco avente tra i suoi attributi l'attributo key e lastkey
 * @return 			
 */
void keyboard_listener(WINDOW* area,GAMES *game) {     
	int key;
	key = wgetch(area); /* wgetch ritorna se niente è immesso, rendendolo non-block */
	flushinp();  /* pulisce il buffer di tutto il contenuto presente */
	if (   key != KEY_LEFT 
		&& key != KEY_RIGHT 
		&& key != KEY_UP 
		&& key != KEY_DOWN  
		&& key != ESC 
		&& key != SPACE 
		&& key != TRAP) {
			game->key = game->lastkey;
	}
	else { // Aggiorna il tasto premuto
		game->key=key; 
		if (key != TRAP) // Se il tasto premuto è la trappola, "piazzala e continua a camminare"
			game->lastkey=key; 
		
	}	
}


/* ====================== GAME FUNCTIONS =================== */

/* GAME TURN MANAGEMENT */
/**
 * Questa funzione fa parte del 'cuore del gioco'.
 * Viene chiamata ad ogni iterazione per computare gli spostamenti del player e del nemico.
 * Questa funzione è "sorvegliata" dal listner.
 * @param 'area'	-> Nome del riquadro (uno tra i 4 presenti) all'interno del quale 
 * 					   verra poi stampato il nemico.
 * @param 'game'	-> Struttura del gioco avente tra i suoi attributi, la mappa di gioco.
 * @param 'player'	-> Struttura del giocatore.
 * @return 			
 */
void compute_next_state(FINESTRE* area,GAMES *game, CHARACTER *player) {
	
	/* Sposta il giocatore nella mappa */	
	make_player_move(area->main,game,player);	
	
	/* Sposta il nemico se questo non si trova sulla trappola, riduci il debuff di tempo altrimenti  */
	if (game->ptr_enemy->debuff == 0 )	make_enemy_move(game,player);
	else{ 
		game->ptr_enemy->duration_debuff-=1;
	}
	
	/* Listner degli eventi del gioco */
	events_listener(area->main, game, player);
	
}
/**
 * Questa funzione fa parte del 'cuore del gioco'.
 * Rendere la posizione corrente per il turno successivo uguale alla posizione successiva di
 * il turno corrente per tutti gli oggetti in movimento
 * @param 'area'	-> Nome del riquadro (uno tra i 4 presenti) all'interno del quale 
 * 					   verra poi stampato il nemico.
 * @param 'game'	-> Struttura del gioco avente tra i suoi attributi, la mappa di gioco.
 * @param 'player'	-> Struttura del giocatore.
 * @return 			
 */
void start_new_turn(GAMES *game, CHARACTER *pac) { 
 	reset_delta(game); 				/* Reset the time passed  from beginning of current round */
	
	pac->x_curr = pac->x_next; 								/* Update x position of player */
	pac->y_curr = pac->y_next; 								/* Update y position of player */

	game->ptr_enemy->x_curr = game->ptr_enemy->x_next;		/* Update x position of the enemy */
	game->ptr_enemy->y_curr = game->ptr_enemy->y_next;		/* Update x position of the enemy */

}
/**
 * Questa funzione server per poter settare l'attributo della struttura game e cioè 'game_over'.
 * @param 'game'	-> Struttura del gioco avente tra i suoi attributi, l'attributo game_over.
 * @param 'why'		-> Intero che determina la modalità di uscità dal gioco.
 * @return 			
 */
void game_over(GAMES *game,int why){
	game->game_over = why; 
}

/* MANAGE GAME DELAYS AND INPUTS */
/**
 * Questa funzione resetta il delta del delay per la prossima mossa.
 * @param 'game'	-> Struttura del gioco avente tra i suoi attributi, l'attributo delta.
 * @return 			
 */
void reset_delta(GAMES *game){ /* reset the delta of delay for the next move */
	game->delta = 0; 
}
/**
 * Questa funzione computa il tempo passato dall'ultimo reset di delta. 
 * Resetta delta ogni volta che diventa piu grande di game->delay.
 * @param 'game'	-> Struttura del gioco avente tra i suoi attributi il delta e il lasttime
 * 					   attributi usati per la gestione del "ritardo di gioco"
 * @return 			-> 0 se il tempo passato dall'ultimo reset è più piccolo di game->dealy.
 * 					   1 altrimenti.	
 */
int deltatime(GAMES *game) {
  clock_t now;
  now = clock(); 		/* Number of clock ticks passed since the beginning of the program */
  game->delta += now - game->lasttime; /* update the number of clock ticks passed since last check */
  game->lasttime = now; /* Record the current ticks count */
  if ((100*game->delta)/CLOCKS_PER_SEC > game->delay) /* check if at least game->delay centiseconds have passed */
		return 1;
  return 0;
}


/* CHARACTER MOVE MANAGEMENT */
/**
 * 
 * 
 */
void respawn_player_somewhere(GAMES* game, CHARACTER* player){
	int x = rand()%30;
	int y = rand()%30;
	
	while(hit_maze(game,x,y) == TRUE){
				x=rand()%game->maze->row;
				y=rand()%game->maze->col;
	}
	player->x_next= x;
	player->y_next= y;
	
}
/**
 * 
 *
 */
bool hit_maze(GAMES *game, int x, int y) {
	
	if( 	 (x<0) || ( x > game->maze->row-1) ) return TRUE;
	else if( (y<0) || ( y > game->maze->col-1) ) return TRUE;
	else if ( (game->maze->phisical_matrix[x][y] == '0')
			|| game->maze->phisical_matrix[x][y] == 'a' 
			|| game->maze->phisical_matrix[x][y] == 'e' 
			|| game->maze->phisical_matrix[x][y] == 'q'
			|| game->maze->phisical_matrix[x][y] == 'd' 
			|| game->maze->phisical_matrix[x][y] == '|' 
			|| game->maze->phisical_matrix[x][y] == '_'
			|| game->maze->phisical_matrix[x][y] == '#') return TRUE;
			//|| game->maze->phisical_matrix[x][y] == '.'	
			//|| game->maze->phisical_matrix[x][y] == 'x') return TRUE;
	return FALSE;
	
}

/**
 * Questa funzione verifica se il player, muovendosi orizzontalmente, ha urtato la porta.
 * Se la porta è stata colpita in uno dei suoi lati, verra cambiata l'orientamento; ovvero si aprirà/chiudera.
 * @param	'game'	-> Struttura del gioco contenente la mappa fisica con porte annesse.
 * @param	'player'-> Struttura del giocatore contenente le coordinate correnti e successive.
 * @return 			-> 0 se è stata toccata la porta (parte sinistra o destra del perno = ### )
 * 					   1 se è stata toccato il perno della porta (parte centrale della porta = ### )
 */
int hit_door_hor(GAMES *game, CHARACTER *player) {
	int x=player->x_next;
	int y=player->y_next;

	if (game->maze->phisical_matrix[x][y] == DOOR) { /* Door is hit */
		/* Hit on the upper edge, then switch orinetation */			
		if (game->maze->phisical_matrix[x+1][y] != DOOR ) { 
			game->maze->phisical_matrix[x][y] = CORRIDOR;
			game->maze->phisical_matrix[x-2][y] = CORRIDOR;			
			game->maze->phisical_matrix[x-1][y-1] = DOOR;
			game->maze->phisical_matrix[x-1][y+1] = DOOR;
			return 0;
		}
		else
			/* Hit on the lower edge, then switch orinetation */			
			if (game->maze->phisical_matrix[x-1][y] != DOOR ) { 
			game->maze->phisical_matrix[x][y] = CORRIDOR;
			game->maze->phisical_matrix[x+2][y] = CORRIDOR;			
			game->maze->phisical_matrix[x+1][y-1] = DOOR;
			game->maze->phisical_matrix[x+1][y+1] = DOOR;
			return 0;
		}
		else /* Hit door in the middle, in this case character stops */
			return 1;
	}
	return 0;
}
/**
 * Questa funzione verifica se il player, muovendosi verticalmente, ha urtato la porta.
 * Se la porta è stata colpita in uno dei suoi lati, verra cambiata l'orientamento; ovvero si aprirà/chiudera.
 * @param	'game'	-> Struttura del gioco contenente la mappa fisica con porte annesse.
 * @param	'player'-> Struttura del giocatore contenente le coordinate correnti e successive.
 * @return 			-> 0 se è stata toccata la porta (parte sinistra o destra del perno = ### )
 * 					   1 se è stata toccato il perno della porta (parte centrale della porta = ### )
 */
int hit_door_ver(GAMES *game, CHARACTER *player) {
	int x=player->x_next;
	int y=player->y_next;

	if (game->maze->phisical_matrix[x][y] == DOOR) { /* Door is hit */
		/* Hit on the left edge, then switch orinetation */
		if (game->maze->phisical_matrix[x][y-1] != DOOR ) { 
			game->maze->phisical_matrix[x][y] = CORRIDOR;
			game->maze->phisical_matrix[x][y+2] = CORRIDOR;
			game->maze->phisical_matrix[x-1][y+1] = DOOR;
			game->maze->phisical_matrix[x+1][y+1] = DOOR;
			return 0;
		}
		else
			/* Hit on the right edge, then switch orinetation */
			if (game->maze->phisical_matrix[x][y+1] != DOOR ) { 
			
			game->maze->phisical_matrix[x][y] = CORRIDOR;
			game->maze->phisical_matrix[x][y-2] = CORRIDOR;
			game->maze->phisical_matrix[x-1][y-1] = DOOR;
			game->maze->phisical_matrix[x+1][y-1] = DOOR;
			return 0;
		}
		else /* Hit door in the middle, in this case character stops */
			return 1;
	}
	return 0;
}
/**
 * Questa funzione permette al giocatore di fermarsi durante il cammine nella mappa, 
 * semplicemente aggiornando di volta in volta le cordinate successive con le attuali.
 * @param 'game'	-> Struttura del gioco contenente la mappa ed altri attributi
 * @param 'ene'		-> Struttura contenente le coordinate del nemico correnti e successive.
 * @return 
 */
void stop_move_player(GAMES *game, CHARACTER *palyer) {
		palyer->x_next = palyer->x_curr;
		palyer->y_next = palyer->y_curr;
}
/**
 * Questa funzione aggiorna le coordinate verticali del giocatore in base alla direzione passata.
 * Ovvimente la posizione verrà aggioranta alla posizione finale , soltanto se nella posizione finale
 * non è presente un ostacolo, altrimenti la posizione successiva sarà uguale alla corrente.
 * La mappa è considerata (solo per il giocatore toroidale, ovvero tutti e 4 i lati come se fossero uniti.
 * Una versione 2D di una mappa sferica 3D) .
 * @param 'game'	-> Struttura del gioco contenente la mappa ed altri attributi
 * @param 'ene'		-> Struttura contenente le coordinate del nemico correnti e successive.
 * @param 'dir'		-> intero (1,2,3,4) che identifica la direzione da assumere. Per le direzione si vada il 
 * 					   #define specifico
 * @return 
 */
void move_player_vert(GAMES *game, CHARACTER *player, char dir) {
	player->y_next = player->y_curr;
	player->x_next = ((player->x_next + (game->maze->row)) + dir)%(game->maze->row); 
	
	if (hit_door_ver(game,player) || hit_maze(game,player->x_next,player->y_next)) 
		player->x_next = player->x_curr;
	switch(dir) {
	case DOWN: player->CHARACTER = 'v';
		break;
	case UP: player->CHARACTER = '^';
		break;
	}
}
/**
 * Questa funzione aggiorna le coordinate orizzontali del giocatore in base alla direzione passata.
 * Ovvimente la posizione verrà aggioranta alla posizione finale , soltanto se nella posizione finale
 * non è presente un ostacolo, altrimenti la posizione successiva sarà uguale alla corrente.
 * La mappa è considerata (solo per il giocatore toroidale, ovvero tutti e 4 i lati come se fossero uniti.
 * Una versione 2D di una mappa sferica 3D).
 * @param 'game'	-> Struttura del gioco contenente la mappa ed altri attributi
 * @param 'ene'		-> Struttura contenente le coordinate del nemico correnti e successive.
 * @param 'dir'		-> intero (1,2,3,4) che identifica la direzione da assumere. Per le direzione si vada il 
 * 					   #define specifico
 * @return 
 */
void move_player_hor(GAMES *game, CHARACTER *pac, char dir) {
	pac->x_next = pac->x_curr;
	pac->y_next = ((pac->y_curr + (game->maze->col)) + dir)%(game->maze->col);
	
	if (hit_door_hor(game,pac) || hit_maze(game,pac->x_next,pac->y_next)) 
		pac->y_next = pac->y_curr;
	switch(dir) {
	case RIGHT: 	pac->CHARACTER = '>';
		break;
	case LEFT: pac->CHARACTER = '<';
		break;
	}
}
/**
 * Questa funzione aggiorna le coordinate verticali del nemico in base alla direzione passata.
 * Ovvimente la posizione verrà aggioranta alla posizione finale , soltanto se nella posizione finale
 * non è presente un ostacolo, altrimenti la posizione successiva sarà uguale alla corrente.
 * @param 'game'	-> Struttura del gioco contenente la mappa ed altri attributi
 * @param 'ene'		-> Struttura contenente le coordinate del nemico correnti e successive.
 * @param 'dir'		-> intero (1,2,3,4) che identifica la direzione da assumere. Per le direzione si vada il 
 * 					   #define specifico
 * @return 
 */
void move_ene_vert(GAMES *game, ENEMY *ene, int dir) {
	ene->x_next= ((ene->x_curr + dir));
	ene->y_next= ene->y_curr;
	
	if (hit_maze(game,ene->x_next,ene->y_next))
			ene->x_next = ene->x_curr;
	else{
				ene->x_next= ((ene->x_curr + dir));
				ene->y_next = ene->y_curr;
	}
			
}
/**
 * Questa funzione aggiorna le coordinate orizzontali del nemico in base alla direzione passata.
 * Ovvimente la posizione verrà aggioranta alla posizione finale , soltanto se nella posizione finale
 * non è presente un ostacolo, altrimenti la posizione successiva sarà uguale alla corrente.
 * @param 'game'	-> Struttura del gioco contenente la mappa ed altri attributi
 * @param 'ene'		-> Struttura contenente le coordinate del nemico correnti e successive.
 * @param 'dir'		-> intero (1,2,3,4) che identifica la direzione da assumere. Per le direzione si vada il 
 * 					   #define specifico
 * @return 
 */
void move_ene_hor(GAMES *game, ENEMY *ene, int dir) {
	ene->x_next = ene->x_curr;
	ene->y_next = ((ene->y_curr + dir));	
	
	if (hit_maze(game,ene->x_next,ene->y_next) )
		ene->y_next = ene->y_curr;
	else {
		ene->x_next = ene->x_curr;
		ene->y_next = ((ene->y_curr + dir));
	}
		
}
/**
 * Questa funzione gestisce il movimento casuale del nemico.
 * Genera un random int da uno a quattro ovvero le varie direzione su, giu, destra e sinistra.
 * In base a questo random int verrà poi richiamata la funzione di movimento.
 * @param 'game'	->	Struttura del game contenente l'attributo nemico con i suoi relativi altri attributi
 * @param 'palyer'	->  Struttura del giocatore 
 * @return 
 */
void make_enemy_random_move(GAMES* game, CHARACTER* player){	
	/* Chose the next move for the enemy */
	int move;	
	move = rand()%4; // Chose a random direction for enemy
	switch(move) { // Process random move 
		case 0 : // Move enen up 
			move_ene_vert(game,game->ptr_enemy, UP);
			break;
		case 1 : // Move ene down 
			move_ene_vert(game,game->ptr_enemy, DOWN);
			break;
		case 2 : // Move ene right
			move_ene_hor(game,game->ptr_enemy, RIGHT);
			break;
		case 3 : // Move ene left 
			move_ene_hor(game,game->ptr_enemy, LEFT);
			break;
		
		}
	
}
/**
 * Questa funzione permette di "lasciare" una trappola nella coordinate del giocatore.
 * Fisicamente inserisce la trappola nella struttura del maze.
 * @param	game	->	Struttura del game contenente, tra i vari attributi, il maze
 * @param	player	->	Struttura contenente tutti gli attributi del giocatore, compreso la quantità di trappole a dispo
 * @return	
 */
void leave_trap_behind( GAMES *game, CHARACTER *player){
	int x = player->x_curr;
	int y = player->y_curr;	
	
	/* Se ne ho ancora a disposizione e non esiste gia una trappola
	 * in quel punto allora ne metto 1 e decremento il totale a disposizione
	 */
	if(	player->total_traps > 0 && game->maze->phisical_matrix[x][y] != TRAP){	
		game->maze->phisical_matrix[x][y]= TRAP;
		player->total_traps--;
	}
	
}
/**
 * Questa funzione gestisce il movimento (e non solo ) del giocatore.
 * E' in grado di catturare i 4 tasti direzionali + eventuali altri bottoni come 
 * lo SPACE l'ESC e TRAP ('t').
 * @param 'area'	->	Nome del riquadro (uno tra i 4 presenti) all'interno del quale 
 * 					   	verra poi stampato il nemico
 * @param 'game'	->	Struttura del game contenente l'attributo nemico con i suoi relativi altri attributi
 * @param 'palyer'	->  Struttura del giocatore 
 * @return 
 */
void make_player_move(WINDOW *area, GAMES *game, CHARACTER *player) {
		keyboard_listener(area,game);
		switch(game->key) { 
			
			case SPACE : 
				stop_move_player(game,player); 	
				break;
			case ESC : 
				game_over(game,ESC_PRESSED); 	
				break;
			case TRAP: 
				leave_trap_behind(game,player);
				break;
			
			case KEY_UP : 
				move_player_vert(game,player, UP);
				break;
			case KEY_DOWN : 
				move_player_vert(game,player,DOWN);
				break;
			case KEY_RIGHT :
				move_player_hor(game,player, RIGHT);
				break;
			case KEY_LEFT : 
				move_player_hor(game,player,LEFT);
				break;
		}
}
/**
 * Questa funzione gestisce il movimento del AI nemico, chiamando 'follow_with_dijkstra' 
 * @param 'game'	->	Struttura del game contenente l'attributo nemico con i suoi relativi altri attributi
 * @param 'palyer'	->  Struttura del giocatore 
 * @return 
 */
void make_enemy_move(GAMES* game, CHARACTER* player){	
	follow_with_dijkstra(game, game->ptr_enemy, player);	
}


/* =================== BEST PATH ALGO ======================== */
int fix_circular(int i, int max) {
	if(i < 0) {
		while(i<0) {
			i += max;
		}
	} else if(i >= max) {
		i -= max;
	}
	return i;
}
int cmparray_ofint(void *a, void *b) {
	int a1, a2, b1, b2;
	if(a == NULL || b == NULL)	return -1;
	a1 = ((int*)a)[0];
	a2 = ((int*)a)[1];
	b1 = ((int*)b)[0];
	b2 = ((int*)b)[1]; 
	if(a1 == b1) {
		if(a2 == b2) return 0;
		else if(a2 > b2) return 1;
			 else        return -1;
	}else if(a1 > b1) return 1;
		  else        return -1;
}
void Dijkstra(MAZE *graph, int x, int y, int **dist, int ***pred) {
	int i, j, k, u_i, u_j;
	int alt, dist_tra;
	int *appoggio;
	int dir;
	Heap_t *Q;
	Pair_t *u;
	// inizializzo il minheap
	Q = NULL;
	if(x < 0 || x >= graph->row || y < 0 || y >= graph->col) {
		return;
	}
	Q = mallocHeap(Q);
	setHeap(Q, 4, 1, 1); 
	Q = initHeap(Q, 0);
	Q->compare = &cmparray_ofint;
	for(i=0;i<graph->row;i++) {
		for(j=0;j<graph->col;j++) {
			if(i == x && j == y) {
				dist[x][y] = 0;
				pred[x][y][0] = x;
				pred[x][y][1] = y;
			} else {
				dist[i][j] = 8888;	// infinito
				pred[i][j][0] = -1;
				pred[i][j][1] = -1;
			}
			appoggio = malloc(sizeof(int)*2);
			appoggio[0] = i;
			appoggio[1] = j;
			insertKey(Q, appoggio, dist[i][j]);
		}
	}

	while(Q->heapsize > 0) {
		u = ((Pair_t *)extractHeapItem(Q));
		if(!u)
			break;
		u_i = ((int*) (u->content))[0];
		u_j = ((int*) (u->content))[1];
		if(dist[u_i][u_j] == 8888) {
			Q->free(u);
			break;
		}
		dir = rand() % 4;
		dir--;
		for(k=0;k<4;k++) {
			i = u_i;
			j = u_j;
			dir = fix_circular(dir+1, 4);
			switch(dir) {
				default:
				case 0:
					i--;
				break;
				case 1:
					j++;
				break;
				case 2:
					i++;
				break;
				case 3:
					j--;
				break;
			}
			if(i < 0 || j < 0 || i > graph->row-1 || j > graph->col-1) {
				continue;
			}
			// Collisioni con la struttura della mappa
			if(  graph->phisical_matrix[i][j] == '#'
				||graph->phisical_matrix[i][j] == '_'
				||graph->phisical_matrix[i][j] == '|' 
				||graph->phisical_matrix[i][j] == '0'
				||graph->phisical_matrix[i][j] == 'a' 
				||graph->phisical_matrix[i][j] == 'e' 
				||graph->phisical_matrix[i][j] == 'q'
				||graph->phisical_matrix[i][j] == 'd' 
			   ){ 
				dist_tra = 8888;
			} else {
				dist_tra = 1;
			}
			alt = dist[u_i][u_j] + dist_tra;
			if(alt < dist[i][j]) {
				dist[i][j] = alt;
				pred[i][j][0] = u_i;
				pred[i][j][1] = u_j;
				appoggio = malloc(sizeof(int)*2);
				appoggio[0] = i;
				appoggio[1] = j;
				decreasePriorityItem(Q, heapgetnumfromel(Q,appoggio), alt);
				free(appoggio);
			}
		}
		Q->free(u);
	}
	if(Q)	freeHeap(Q);
}
int create_next_coordinates(MAZE *lab, int *s, int *v, int ***pred, int *vai_in_x, int *vai_in_y) {
	int appoggio[1];
	int t[1];
	int done;
	*vai_in_x = -1;
	*vai_in_y = -1;
	done = 0;
	if(s[0] == v[0] && s[1] == v[1]) { // uguali.. resta fermo
		*vai_in_x = s[0];
		*vai_in_y = s[1];
	}
	t[0] = v[0];
	t[1] = v[1];
	while(!done) {
		if(pred[v[0]][v[1]][0] == -1) { //Non eiste il percorso
			*vai_in_x = -1;
			*vai_in_y = -1;
			done = 1;
		} else if(s[0] == v[0] && s[1] == v[1]) { // ora sono uguali
			done = 1;
		} else {
			*vai_in_x = v[0];
			*vai_in_y = v[1];
			appoggio[0] = pred[v[0]][v[1]][0];
			appoggio[1] = pred[v[0]][v[1]][1];
			v[0] = appoggio[0];
			v[1] = appoggio[1];
		}
	}
	v[0] = t[0];
	v[1] = t[1];
	return 0;
}
void follow_with_dijkstra(GAMES *game, ENEMY *enemy, CHARACTER *player) {

	int dadove[1], adove[1];
	int **dist;
	int ***pred;
	int i, j;
	int vai_in_x, vai_in_y;
	
	if(!enemy)
		return;
	if(!player)
		return;

	vai_in_x = -1;
	vai_in_y = -1;

	// Inizializza dist, col, pred per Dijkstra 
	dist = malloc(sizeof(int*)*game->maze->row);
	pred = malloc(sizeof(int**)*game->maze->row); 
	for(i=0;i<game->maze->row;i++) {
		dist[i] = malloc(sizeof(int)*game->maze->col);
		pred[i] = malloc(sizeof(int*)*game->maze->col);
		for(j=0;j<game->maze->col;j++)
			pred[i][j] = malloc(sizeof(int)*2); 
	}
	// Sorgente 
	dadove[0] = enemy->x_curr;
	dadove[1] = enemy->y_curr;

	// Destinazione 
	adove[0] = player->x_curr;
	adove[1] = player->y_curr;

	// Esegui Dijkstra sul labirinto 
	Dijkstra(game->maze, dadove[0], dadove[1], dist, pred);
	
	// Calcola delle coordinate dove andare 
	create_next_coordinates(game->maze, dadove, adove, pred, &vai_in_x, &vai_in_y);


	if( ((vai_in_x > -1 && vai_in_y > -1) && (vai_in_x < game->maze->row-1) && (vai_in_y < game->maze->col-1))) {
		enemy->x_next = vai_in_x;
		enemy->y_next = vai_in_y;
	} else {
		make_enemy_random_move(game,player);
	}
	
	// Dealloca tutto il materiale precedentemente allocato
	for(i=0;i<game->maze->row;i++) {
		free(dist[i]);
		for(j=0;j<game->maze->col;j++)
			free(pred[i][j]);
		free(pred[i]);
	}
	free(dist);
	free(pred);


}


/* ============== INIT GAME / TEXT FUNCTIONS ================ */
/**
 * Questa funzione inizializza la struttura di gioco inizializzando tutti gli attributi che 
 * verranno poi usati e modificati durante il corso della partita.
 * @param	'game' -> Struttura di gioco inizialmente allocata e passata "vuota". Qui verrà inizializzata.
 * @return
 */
void init_game(GAMES *game) {
	game->lastkey = 0;     
	game->delay = DELAY;   
	game->game_over = 0;   
	game->delta=0;         
	game->lasttime=0;      
	game->started =time(NULL); 
	game->time_end = malloc(sizeof(char)*10);	  	
	game->current_reliqs=0;		   
	game->total_reliqs=0;	   		
	game->bottom_up=0;	
	game->current_bottom  =TIC_ON_BOTTON;	
	game->times_to_exit=0;
	game->exit_up=0;	
	game->total_fragments=0;		
	game->x_uscita =-1; 
	game->y_uscita =-1; 
	game->x_ingresso =1; 
	game->y_ingresso =1; 	
	game->maze=NULL;
	game->flag=0;	
	game->ptr_enemy=NULL;
		
}	
/**
 * Questa funzione inizializza il bottone segreto nella mappa. 
 * Lo inserisce fisicamente nella matrice, laddove possibile evitando tutta la struttura del maze.
 * @param	'game'	-> Struttura di gioco contenente tra i vari attributi quello del bottone segreto
 */
void init_secret_bottom(GAMES *game){
	int x,y;
	
	x=rand()%game->maze->row;
	y=rand()%game->maze->col;
	while( hit_maze(game,x,y)==TRUE ){
				x=rand()%game->maze->row;
				y=rand()%game->maze->col;
	}
	game->maze->phisical_matrix[x][y]=BOTTON_TO_PRESS;
	game->bottom_up =1;
}
/**
 * Questa funzione inizializza la porta di uscita game nelal mappa.
 * La inserisce fisicamente nella matrice, laddove possibile evitando tutta la struttura del maze.
 * @param	'game'	-> Struttura di gioco.
 * @return
 */
void init_exit_door(GAMES* game){
	int x,y;
	
	x=rand()%game->maze->row;
	y=rand()%game->maze->col;
	
	while(hit_maze(game,x,y)==TRUE){
		x=rand()%game->maze->row;
		y=rand()%game->maze->col;
	}
	game->x_uscita=x;
	game->y_uscita=y;
	game->maze->phisical_matrix[game->x_uscita][game->y_uscita] = EXIT_DOOR;
}
/**
 * Questa funzione si occupa dell'inserimento (di qualsiasi oggetto si tratti) 
 * nella mappa di gioco di un qualche oggetto.
 * @param	'game'		->	Struttura di gioco contenente la mappa
 * @param	'how_many'	->  Quantità degli elementi che dovranno essere inseriti
 * @param	'object'	->  Tipo di oggetto che verra inseritò in mappa.
 * @return
 */
void init_object(GAMES* game,int how_many,char object){
	
	int x,y;	
	for(int index=0; index < how_many; index++){
	
		x=rand()%game->maze->row;
		y=rand()%game->maze->col;
		while(  game->maze->phisical_matrix[x][y] == '0'
			|| game->maze->phisical_matrix[x][y] == 'a' 
			|| game->maze->phisical_matrix[x][y] == 'e' 
			|| game->maze->phisical_matrix[x][y] == 'q'
			|| game->maze->phisical_matrix[x][y] == 'd' 
			|| game->maze->phisical_matrix[x][y] == '|' 
			|| game->maze->phisical_matrix[x][y] == '_'
			|| game->maze->phisical_matrix[x][y] == '#'
			|| game->maze->phisical_matrix[x][y] == '.'	
			|| game->maze->phisical_matrix[x][y] == 'x'){		
			x=rand()%game->maze->row;
			y=rand()%game->maze->col;			
		}
		game->maze->phisical_matrix[x][y]=object;
	}
	
}
/**
 * Questa funzione inizializza, quindi richiama a sua volta la funzione per leggere la mappa 
 * da file, ed una volta inizializzata senza errore, passa ad inizilizzare le varie componenti
 * da inserire nella mappa quali le relics i frammenti ed eventuali altri oggetti..
 * @param 'game'		->	Struttura del gioco contenente i vari attributi di gioco inizializzati passpasso
 * @param 'path_file'	->	Percorso della direcotry contenente il file che la funzione deve caricare in matrice
 * @return 'game->flag'	-> Questa flag serve a segnalare al chiamante se cè stato un problema con la lettura da file
 */
int init_maze_and_his_objects(GAMES *game, char* path_file){
	srand( time( NULL ) );
	
	/*creating phical 2D Array*/
	game->maze=malloc(sizeof(MAZE));	
	game->maze->phisical_matrix=NULL;	
	game->flag=load_maze(game,path_file);
	
	if(game->flag!= -1){	/* Se il file è stato letto correttamente e quindi la matrice è stata allocata, posso inserire gli oggetti. */
		
		/* sezione aggiunta oggettistica al maze..	
		 * ..
		 */
		 game->ptr_enemy=init_enemy(game);
		 init_object(game,N_RELIC,RELIC);		/* Inserisco nel maze un numero prefissato di reliqs in random x,y */
			game->total_reliqs=N_RELIC;			/* e quindi a questo punto aggiorno il totale delle reliqs, inizialmente 0*/		
		 init_object(game,N_PIECE_OF_TRAP,PIECE_OF_TRAP);
			game->total_fragments=N_PIECE_OF_TRAP;		
		
		//init_object(game,2,BULLET);			/* per sviluppi futuri */
		//init_object(game,1,MEDIKIT);			/* per sviluppi futuri */
		//init_object(game,2,BOT_ALLY);			/* per sviluppi futuri */		
		
	}
	
	
	return game->flag;
	
}

/* ============== INIT PLAYER / ENE / ITEM ======================= */

/**
 * Questa funzione inizializza la struttura del giocatore precedetemente allocata e passata "vuota"
 * Qui verrà inizializzata per poter usufruire di questi attributi durante le varie fasi di gioco.
 * @param	'player'-> Struttura del giocatore.
 * @param 'x','y'	->
 * @return 
 */
void init_player(CHARACTER *player, int x, int y) {
	player->health=LIFE;				/* Vita iniziale del giocatore */
	player->total_traps = N_TRAP;
	player->total_fragments_gained=0;
	player->total_reliqs_gained=0;
	
	/* Imposta le coordinate di partenza */
	player->x_curr = x;
	player->y_curr = y;

	/* Imposta le coordinate del prossimo turno */
	player->x_next = x;
	player->y_next = y;
	
	/* Imposto gli oggetti che è possibile accumulare durante il game a 0*/
	player->total_reliqs_gained=0;
	player->total_fragments_gained=0;
	
	
	player->CHARACTER = 'v'; 		/* Imposta la shape iniziale */	

}
/**
 * Questa funzione inizializza il nemico scegliendo per ogni partita, uno spawn random.
 * 	[Per i dettagli e scelte implementative, guardare la documentazione]
 * @param 'game' -> Struttura di gioco contenente un attributo di tipo del nemico per poterne usufruire.
 * @return 'en'	 -> Struttura del nemico.
 */
ENEMY* init_enemy(GAMES* game){
	ENEMY *en = malloc(sizeof(ENEMY));	
	en->debuff = 0;
	en->duration_debuff=TIC_ON_TRAP;
	
	int x=rand()%game->maze->row;
	int y=rand()%game->maze->col;
	
	while( game->maze->phisical_matrix[x][y] == '0'
				|| game->maze->phisical_matrix[x][y] == 'a' 
				|| game->maze->phisical_matrix[x][y] == 'e' 
				|| game->maze->phisical_matrix[x][y] == 'q'
				|| game->maze->phisical_matrix[x][y] == 'd' 
				|| game->maze->phisical_matrix[x][y] == '|' 
				|| game->maze->phisical_matrix[x][y] == '_'
				|| game->maze->phisical_matrix[x][y] == '#'){
				x=rand()%game->maze->row;
				y=rand()%game->maze->col;
	}
	
	/* Imposta le coordinate di partenza */	
	en->x_curr =x ;
	en->y_curr =y;

	/* Imposta le coordinate del prossimo turno */
	en->x_next =x ;
	en->y_next =y ;
	
	return en;
	
	
}

/* ============ GRAPHICAL SYSTEM FUNCTIONS ==============*/
/**
 * Questa funzione inizializza un set di "coppia" di colori impostando <background, colore>
 * @return
 */
void init_color_curses() {
	start_color(); /* Inizializza i colori */
	init_pair(1, COLOR_WHITE, COLOR_BLACK); 	/* Bianco su nero */
	init_pair(2, COLOR_BLACK, COLOR_BLACK); 	/* Nero su nero */
	init_pair(3, COLOR_RED, COLOR_WHITE);   	/* Rosso su bianco */
	init_pair(4, COLOR_RED, COLOR_BLACK);   	/* Rosso su nero */
	init_pair(5, COLOR_WHITE, COLOR_WHITE);   	/* Bianco su bianco */
	init_pair(6, COLOR_BLACK, COLOR_WHITE); 	/* Nero su bianco */
	init_pair(7, COLOR_WHITE, COLOR_RED);   	/* Bianco su rosso */
	init_pair(8, COLOR_BLACK, COLOR_RED);   	/* Nero su rosso */
	init_pair(9, COLOR_YELLOW, COLOR_YELLOW);   /* Giallo su giallo */
	init_pair(10, COLOR_BLACK, COLOR_YELLOW);   /* Nero su Giallo */
	init_pair(11, COLOR_BLACK, COLOR_GREEN);   	/* Nero su Verde */
	init_pair(12, COLOR_BLACK, COLOR_MAGENTA);  /* Nero su Viola */
	init_pair(13, COLOR_YELLOW, COLOR_BLACK);   /* Giallo su Nero */
	init_pair(14, COLOR_RED, COLOR_BLACK);   	/* Rosso su Nero */
	init_pair(15, COLOR_GREEN, COLOR_BLACK);   	/* Verde su Nero */
	init_pair(16, COLOR_CYAN, COLOR_BLACK);   	/* Ciano su Nero */
	init_pair(17, COLOR_BLACK, COLOR_CYAN);   	/* Nero su Ciano */
	init_pair(18, COLOR_GREEN, COLOR_YELLOW);  	/* Verde su Giallo */
	init_pair(19, COLOR_MAGENTA, COLOR_BLACK);  /* Magenta su Nero */	
	init_pair(20, COLOR_BLUE, COLOR_BLACK); 	/* Blue su Bianco */
	init_pair(21, COLOR_BLACK, COLOR_BLACK);	/* Nero su Nero */
	init_pair(22, COLOR_GREEN, COLOR_WHITE);	/* Verde su Bianco */
	
}
/**
 * Questa funzione raggruppa un set di istruzioni che servono per inizializziare la 
 * parte grafica del gioco. In dettaglio la libreria grafica utilizzata è quella di 
 * <ncurses.h>. 
 * @return 		-> 1 se l'inizializzazione è andata a buon fine.
 * 				   0 se il terminale non supporta i colori quindi non è possibile inizializzare la parte grafica.
 */
int init_curses() {
	/* Init screen & keyboard input */
	initscr();					/* Start the curses mode */
	clear();    				/* Clear the screen */  
	keypad(stdscr, TRUE); 		/* Allow control keys to be read (i.e., function and arrow keys) */
	curs_set(0);  				/* Hide cursor */
	noecho();    				/* Do not echo pressed key on the screen */
	cbreak();   				/* Line buffering disabled, pass on everything */
 	if(!has_colors()) {	
		endwin();
		printf("Your terminal does not support colors\n");
		return 0;
	}
	init_color_curses();
	refresh(); 
	ESCDELAY = 20;  			/* Set delay for ESC reaction (default delay is too long) [Linux only] */
	return 1;
}
/**
 * Questa funzione permette di chiudere correttamente la parte grafica di ncurses.
 * @return
 */
void close_curses() {
		attroff(COLOR_PAIR(1)); 		/* Restore screen colors */
		endwin();   					/* Close the window */
}
/**
 * Questa funzione crea fisicamente e graficamente la finestra (contenitore, solo i bordi) settando
 * la dimensione e la posizione in base ai parametri ottenuti in input
 * @param 'height'		-> Altezza della finestra.
 * @param 'width'		-> Larghezza della finestra.
 * @param 'starty'		-> Posizione x (nel terminale 0,0 corrisponde al pixel in alto a sinistra del terminale)
 * @param 'startx'		-> Posizione y ( "" "" "" .. )
 * @return 'local_win'	-> Finestra creata correttamente.
 * 
 */
WINDOW *create_boxed_newwin(int height, int width, int starty, int startx) {
	/* Create a boxed windows with "height" lines, "width" coulumns. The upper
		 left-hand corner of the window is at line starty and column startx, with
		 respect to the current window */
	WINDOW *local_win;

	/* Create a window for the bounding box */
	local_win = newwin(height, width, starty, startx);
	/* Create the bounding box */
	box(local_win, 0 , 0); // the two 0's correspond to the default characters for the vertical and horizontal lines of the box 
	wrefresh(local_win);		/* Show all in window	*/
	
	//Create a new window inside the bounding box 
	local_win = newwin(height-2, width-2, starty+1, startx+1); 
	wrefresh(local_win);		 // Show that box
	nodelay(local_win,TRUE); // Keyboard reads will be non-blocking in window 
	keypad(local_win, TRUE); // Allow control keys to be read 
	
	return local_win;
}
/**
 * Questa funzione mi permette di creare le finestre nelle aree scelte, di dimensione
 * e coordinate x,y a scelta. La dimensione della finestra di gioco e delle coordinate di disposizione 
 * sono state fissate dal programmatore. 
 * 	[per maggiori dettagli e spiegazioni su questa scelta, si guardi la documentazione]
 * @param	'area'		->	Struttura della finestra contenente le varie WINDOW :desc,rules,info,main.
 * @return 
 */
void make_area_game(FINESTRE *area) {
	
	/* Crea la finestra per la descrizione degli eventi accaduti */
	area->desc = create_boxed_newwin(5,MAX_SIZE+2+MAX_SIZE+2,0,40);
	
	/* Crea la finestra per le regole di gioco */
	area->rules = create_boxed_newwin(MAX_SIZE+2,MAX_SIZE+10,6,5);
	
	/* Crea la finestra per il gioco */	
	area->main = create_boxed_newwin(MAX_SIZE+2,MAX_SIZE+2,6,50);
	
	/* Crea la finestra per le informazioni */
	area->info = create_boxed_newwin(MAX_SIZE+2,MAX_SIZE+2,6,50+35);
	
	

	/* Verifica che le finestre siano state creata correttamente */
	if(area->main && area->info && area->desc && area->rules) 
		area->success = 1;
	else
		area->success = 0;
}

/* =================== GAME ENGINE ================= */
/**Dealloca il 2D array.
 * @param	tofree 	-> Puntatore al 2D array.
 * @return
 */
void free_maze( MAZE* tofree){
	for(int i=0;i<tofree->row;i++)
				free(tofree->phisical_matrix[i]);
	free(tofree->phisical_matrix);
}

/**
 * Questo è effettivamente il cuore pulsante del gioco. Da qui partono tutte le funzioni di
 * gestione di gioco.
 * Il game_engine dopo l'inizializzazione, iterativamente inizializza turno per turno, legge
 * da tastiera, computa la mossa successiva, aggiorna a video il risultato.
 *		[Per una chiara spiegazione dell'implementazione e dettagli, guardare la documentazione].
 * @param 'area'		-> Nome della struttura all'interno della quale sono presente le finestre 
 * 						   utilizzate per presentare il gioco. Per mia scelta sono state create 4 finestre.
 * @return 				->  0 se tutto è andato bene ed il gioco è terminato correttamente
 * 						   -1 se ci sono stati problemi con la lettura della mappa da file
 * 						   -2 se ci sono stati problemi con la creazione delle finestre.
 */
int game_engine(FINESTRE *area) {
	
	int curr_level_index;
	char *curr_level=NULL;	
	char *lose_messag=NULL;
	char *win_messag=NULL;
	char *esc_messag=NULL;		
	GAMES *game=NULL;
	CHARACTER *player=NULL;
	srand( time( NULL ) );	
	
	
	/* Ritorno errore se ci sono stati problemi con la creazione delle finestre */
	if(!area) return ERR_WITH_WINDOW_NCURSES;
	
	
	/* Controllo se la direcotry esiste in caso contrario ritorno errore */
	const char* folderr = 	PATH_FILE_GAME;
    struct stat sb;
    if (!(stat(folderr, &sb) == 0 && S_ISDIR(sb.st_mode))){
		return ERR_WITH_FOULDER_FILE;
	}
	
	game = (GAMES *) malloc(sizeof(GAMES));
	player = (CHARACTER *) malloc(sizeof(CHARACTER));	
	bool can_quit = FALSE;
	
	/* setto i parametri per i primi livelli */
	curr_level_index=1;		
	curr_level="./MazeFileTXT/level_1.txt";
   
	
	//ciclo di gioco 
	while(can_quit != TRUE){
		//ciclo dei livelli
		while(curr_level_index <= NUMBER_OF_LEVELS && can_quit != TRUE){			
			
			/* Inizializzo la struttura game e tutti i suoi attributi e nemici */
			init_game(game);								
			/* Lettura del maze da file e inizializzazione successiva degli elementi presenti nel maze libero 
			 * tutto nel caso di errore in lettura. */
			if (init_maze_and_his_objects(game,curr_level) == -1 ){	
				if((game->ptr_enemy))
					free(game->ptr_enemy);
				free(game);
				free(player);			
				return ERR_WITH_FILE; 
			}		
			
			show_game_info(area->info,curr_level_index);
			show_game_desc(area->desc);		
			show_game_rules(area->rules);	
			show_pre_game_msg(area->main,curr_level_index);
				
			//A questo punto posso inizializzare il player.
			init_player(player,game->x_ingresso,game->y_ingresso);	
			
			/* loop di gioco */
			while(game->game_over == 0) { 				
				if(deltatime(game)) { 					
					start_new_turn(game, player);		
					compute_next_state(area, game, player);
					show_new_state(area, game, player);
				}
			}	
			
			/* Smetto di avanzare i livelli se:
			 * 	-il giocatore ha perso
			 * 	-il giocatore ha premuto ESC
			 * Quindi mi setto questa flag per poter uscire dal ciclo dei livelli
			 */
			switch(game->game_over){
				case ESC_PRESSED:
					can_quit = TRUE;
					break;
				case LOSE:
					can_quit = TRUE;
					break;
			}		
			
			/* pulisco i riquadri preparandoli eventualmente per il prossimo livello */
			werase(area->info);
			werase(area->main);	
								
			/* dealloco la vecchia mappa per fare spazio alla lettura della nuova */
			free_maze(game->maze);		
			
			
			/* avanzo dell index del livello corrente con aggiornamento mappa da caricare */
			curr_level_index++;			
			switch(curr_level_index){
				case 2:
					 curr_level="./MazeFileTXT/level_2.txt";
					 break;
				case 3:
					 curr_level="./MazeFileTXT/level_3.txt";
					 break;
			  //case 4: ... altri futuri livelli.
			}			
			
		}//fine ciclo dei livelli
		
		
		//Mi chiedo se il giocatore ha premuto ESC quindi chiudere tutto
		if(game->game_over == ESC_PRESSED){
			esc_messag="Really? you have quitted the  game! Unbelievable, byebye!";
			show_end_game_msg(area->main,esc_messag);
			can_quit=TRUE;
		}
		//oppure ha perso/vinto quindi restartare tutto e farlo giocare nuovamente.
		else{
			lose_messag="I'm sorry you lose. Is this   game too hard?\nTry again!";
			 win_messag="If you can read this, you havewon all levels.\nCongratz! You are free to go..";
			switch(game->game_over){
					case WIN:
						show_end_game_msg(area->main,win_messag);
						break;
					case LOSE:
						show_end_game_msg(area->main,lose_messag);
						break;
						
			}
			// resetto delle impsotazioni di gioco.
			curr_level_index=1;
			curr_level="./MazeFileTXT/level_1.txt";
			can_quit=FALSE;
			werase(area->main);
		}		
	}//fine ciclo di game
	
	
	
	/* A questo punto tutto è finito, e posso liberare la memoria */
	if(game) {
		if(game->ptr_enemy){
			free(game->ptr_enemy);
		}
		free(game);
	}
	if(player)	free(player);		
	return GAME_CLOSE_CORRECTLY;
}







