/* define di err */
#define ERR_WITH_FILE 5000
#define ERR_WITH_FOULDER_FILE 5001
#define ERR_WITH_WINDOW_NCURSES 5003
#define GAME_CLOSE_CORRECTLY 5004


/* define della cartella delle mappe */
#define PATH_FILE_GAME "./MazeFileTXT/"

/* Define di logica di gioco */
#define NUMBER_OF_LEVELS 3
#define LIFE 2
#define N_RELIC 3

#define TIC_ON_TRAP 40
#define N_TRAP 1
#define N_PIECE_OF_TRAP 60
#define N_FRAGM_TO_CREATE_TRAP 10

#define HOW_MANY_CICLE_B4_EXIT 2
#define TIC_ON_BOTTON 30

#define DELAY 10 /* Time delay in centiseconds */
#define ESC 27
#define SPACE 32
#define TRAP 't'

/* Define di "uscita" */
#define WIN 5
#define LOSE -5
#define ESC_PRESSED 1


/* Define utilizzati per lo spostamento del giocatore. "Macro" comode da usare. */ 
#define VERTICAL 1
#define HORIZONTAL 0
#define UP -1
#define DOWN 1
#define LEFT -1
#define RIGHT 1

/* Define utilizzati per il parser in lettura sui file.txt delle mappe */
#define DOOR '#'
#define EXIT_DOOR 'E'
#define BOTTON_TO_PRESS 'B'
#define ENE '@'
#define BULLET 'b'
#define CORRIDOR ' '
#define RELIC 'x'
#define PIECE_OF_TRAP '.'
