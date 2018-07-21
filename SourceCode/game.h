#include <ncurses.h>
#include <time.h>
#include "mydefine.h"


typedef struct character_status CHARACTER;
typedef struct game_status GAMES;
typedef struct finestre FINESTRE;
typedef struct maze_ MAZE;
typedef struct enemy_status ENEMY;


struct maze_{
	int row;
	int col;
	char **phisical_matrix;
};

struct finestre {
	WINDOW *main;
	WINDOW *info;
	WINDOW *desc;
	WINDOW *rules;
	int success;
};

struct game_status {
   
	int key; 				/* the current key pressed */
	int lastkey; 			/* the last key pressed */
	clock_t delay;   		/* delay between game rounds */
	char game_over; 		/* the game over flag*/
	float delta; 			/* time passed since last round */
	clock_t lasttime; 		/* clock ticks of last time check */
	long started; 			/* Contiene il long del time() di quando si inizia a giocare */
	char *time_end;			/* Contiene la stringa del tempo trascorso quando la partita finisce */		
	int current_reliqs; 	/* Contenitore di reliquie parziale - usato in "listner_events" per attivare altre parti del game*/
	int total_reliqs;		/* Totale oggetti reliqus che il player deve accumulare(tranne i bullet) */	
	int total_fragments;	/* Contenitore di frammenti '.' sparsi per la mappa */
	int bottom_up;			/* Guardia che mi permette di far spawnare soltanto una volta il bottone nelle varie iterazioni  */
	int current_bottom;		/* Contenitore dei tic times sul bottone */	
	int times_to_exit;		/* Contenitore di "quante volte è spwanato il bottone segreto, mi serve per poter aprire la porta di uscita una volta raggiunto un tot desiderato */
	int exit_up;			/* Guardia che mi permette di far spawnare soltanto una volta l'uscita nella varie iterazioni*/		
	int x_uscita; 			/* Coordinata della riga dell'uscita */
	int y_uscita; 			/* Coordinata della colonna dell'uscita */
	int x_ingresso; 		/* Coordinata della riga di partenza del main character */
	int y_ingresso; 		/* Coordinata della colonna di partenza del main character */	
	MAZE *maze; int flag;	/* Questa flag serve come guardia nel caso il caricamento da file della mappa sia andato storto */	
	ENEMY *ptr_enemy;		/* Questo è un ptr alla struttura nemico */		
};

struct character_status {
   /* The struct for the status of the moving character */   
	int x_next;  /* The next horizontal position of pacman on the screen */
	int y_next;  /* The next vertical position of pacman on the screen */
	int x_curr;  /* The current horizontal position of pacman on the screen */
	int y_curr;  /* The current vertical position of pacman on the screen */
	int health;	/* The health of player initially */	 
	int total_fragments_gained;  /* Contenitore dei pezzi di frammenti di trappola. Ogni 10 formo una trappola */
	int total_reliqs_gained;	 /* Contenitore delle relics ottenute sul campo- per uso di print a fine game */	
	int total_traps;		 /* Contenitore trappole utilizzabili sul nemico */	
	char CHARACTER; /* The current shape of the character */
};
struct enemy_status{
	int x_next;  	/* The next horizontal position of enemy on the screen */
	int y_next;  	/* The next vertical position of enemy on the screen */
	int x_curr;  	/* The current horizontal position of enemy on the screen */
	int y_curr;  	/* The current vertical position of enemy on the screen */
	int debuff;				/* Flag che mi segnala un debuff sul nemico. */
	int duration_debuff;	/* Tempo di tic che deve restare fermo sulla trappola quando il debuff è attivo */
	//ENEMY* next;	/* Possibile sviluppo futuro su una lista di nemici diversi e con skill differenti.. */
};



int read_maze_from_file(GAMES* , char* ,char* );	
int load_maze(GAMES *, char* );
int init_maze_and_his_objects(GAMES *, char* );
void show_player_move(WINDOW *, CHARACTER *);
void show_game_situation(WINDOW *, GAMES *, CHARACTER* );
void show_maze(WINDOW *area, GAMES* game);
void show_new_state(FINESTRE *, GAMES *, CHARACTER *);
void show_dir_elements(char* );
void show_time(WINDOW *area, GAMES* );
char *make_string_time(GAMES *);
void events_listener(WINDOW *, GAMES* , CHARACTER* );
void compute_next_state(FINESTRE* ,GAMES *, CHARACTER *);
void start_new_turn(GAMES *, CHARACTER *);
void game_over(GAMES *,int );
void reset_delta(GAMES *);
int deltatime(GAMES *);
void keyboard_listener(WINDOW* ,GAMES *);
bool hit_maze(GAMES *, int, int);
void respawn_player_somewhere(GAMES* game, CHARACTER* player);
int hit_door_hor(GAMES *, CHARACTER *);
int hit_door_ver(GAMES *, CHARACTER *);
void stop_move_player(GAMES *, CHARACTER *);
void move_player_vert(GAMES *, CHARACTER *, char );
void move_player_hor(GAMES *, CHARACTER *, char );
void make_player_move(WINDOW *, GAMES *, CHARACTER *);
int fix_circular(int , int );
int cmparray_ofint(void *, void *);
void Dijkstra(MAZE *, int , int , int **, int ***);
int create_next_coordinates(MAZE *, int *, int *, int ***, int *, int *);
void follow_with_dijkstra(GAMES *, ENEMY *, CHARACTER *);
void make_enemy_move(GAMES* , CHARACTER* );
void make_enemy_random_move(GAMES* , CHARACTER* );
void init_game(GAMES *);
void init_game_desc(WINDOW *);
void init_game_info(WINDOW *);
void init_secret_bottom(GAMES *);
void init_exit_door(GAMES* );
void init_player(CHARACTER *, int , int );
void init_object(GAMES* ,int ,char );
ENEMY* init_enemy(GAMES* );
void init_color_curses();
int init_curses();
void close_curses();
WINDOW *create_boxed_newwin(int , int , int , int );
void make_area_game(FINESTRE *);
int game_engine(FINESTRE *);
void free_matrxi( MAZE* tofree);
void intHandler();
