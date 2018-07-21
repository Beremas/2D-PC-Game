#ifndef W_OS_LINUX
  #include <ncurses.h>
#elif TARGET_OS_X
	#include <curses.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>
#include <termios.h>  //_getch*/
#include <time.h>
#include <unistd.h>
#include <dirent.h> 
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define GREEN_COLOR_TEXT   "\x1b[32m"
#define RED_COLOR_TEXT     "\x1b[31m"
#define RESET_COLOR_TEXT   "\x1b[0m"
#define YELLOW_COLOR_TEXT  "\x1b[33m"



char* reallocString_utility(char *, size_t);
int readStr_utility(char**);
void readInt_utility(int*);
void readDouble_utility(double *d);


void* readDouble();
void* readInt();
void* readStr();

void printDouble(void*);
void printInt(void*);
void printStr(void*);

void clearLine(int x,int y);
void pressEnter();

/**
 * Following Defines are to Make life easy
 */
#define LOG_ERR(f_, ...)   printf("%s:%d ERROR: "f_"\n",__FILE__, __LINE__, ## __VA_ARGS__)
#define LOG_INFO(f_, ...)  printf("%s:%d INFO:  "f_"\n",__FILE__, __LINE__, ## __VA_ARGS__)
#ifdef DEBUG
#define LOG_DEBUG(f_, ...) printf("%s:%d DEBUG: "f_"\n",__FILE__, __LINE__, ## __VA_ARGS__)
#else
#define LOG_DEBUG(f_, ...)
#endif /* DEBUG */
