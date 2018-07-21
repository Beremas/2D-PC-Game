#include "support.h"

//INPUT DA TASTIERA
char* reallocString_utility(char *s, size_t dim){
	return ((char*) realloc(s, dim));
}

void readInt_utility(int *a){	
	int x;
	char c = getchar();
	while((c < '0' || c > '9') && c != '\n' && c != EOF) c = getchar();
	if(c == '\n' || c == EOF){
		//Nessun numero riconoscibile
		return;
	}
	while(c < '0' || c > '9') c = getchar(); //Stavolta cerchiamo davvero solo cifre
	x = c - '0'; //Inizializzo x a c - '0', che equivale alla rappresentazione numerica in codice ascii del carattere c meno la rappresentazione del carattere 0. Se c è una cifra, come sappiamo che è, allora x diventerà uguale a quella cifra
	c = getchar();
	while(c >= '0' && c <= '9'){ //Continuo a leggere caratteri fino a che sono cifre
		x = (x << 1) + (x << 3) + (c - '0'); //Questa istruzione opera una moltiplicazione di x per dieci, sommando x shiftato di un bit (*2) e x shiftato di tre bit (*8), poichè (x*2)+(x*8)=(x*10). A questo valore somma quello della nuova cifra trovata, ad ottenere il numero finora letto
		c = getchar();
	}
	while(c != '\n' && c != EOF) c = getchar(); //Svuoto il buffer fino al prossimo carattere di ritorno a capo, che in un input correttamente formattato indica la fine dell'input
	*a = x;
}
int readStr_utility(char **s){
	char c = getchar();
	size_t dim = 20;
	*s = (char*) calloc(dim, sizeof(char));
	if(*s == NULL){
		printf("Errore nell'allocazione, ABORT\n");
		exit(-112);
	}
	size_t counter = 0;
	while(c != '\n' && c != '\0'){
		if(counter == dim){
			dim = (dim << 1);
			*s = reallocString_utility(*s, dim);
			if(*s == NULL){
				printf("Errore nell'allocazione, ABORT\n");
				exit(-112);
			}
		}
		(*s)[counter++] = c;
		c = getchar();
	}
	if(counter == dim){
		dim = (dim << 1);
		*s = reallocString_utility(*s, dim);
		if(*s == NULL){
			printf("Errore nell'allocazione, ABORT\n");
			exit(-112);
		}
	}
	(*s)[counter] = '\0';
	return counter;
}
void readDouble_utility(double *d){
	int i = 0;
	char *a = (char *) malloc(sizeof(char) * 1024);
	while (1) {
		scanf("%c", &a[i]);
		if (a[i] == '\n') {
			break;
		}else {
			i++;
		}
	}
	a[i] = '\0';
	
	double provvisiorio=atof(a);
	*d=provvisiorio;
}

void* readInt(){
	int *intero;
	intero = malloc(sizeof(int));
	readInt_utility(intero);
	return (void*)intero;
}
void* readStr(){
	char *stringa;
	readStr_utility(&stringa);
	return (void*)stringa;
}
void* readDouble() {
	double *decimale;
	decimale = malloc(sizeof(double));
	readDouble_utility(decimale);
	return (void*)decimale;
}



//OUTPUT TIPO DATI
void printDouble(void* elem){
	printf("%lf ",*((double*)(elem)));
}
void printInt(void* elem){
	printf("%d ",*(int *)elem);
}
void printStr(void* elem){
	printf("%s ",(char*)(elem));
}


void clearLine(int x,int y){
	move(x, y);          // move to begining of line
	clrtoeol();          // clear line
}
void pressEnter(){
	char userCheck;
		printf(GREEN_COLOR_TEXT"Press ENTER to continue.."RESET_COLOR_TEXT);
	do
		userCheck=getchar();
	while(userCheck != '\n');	
	system("clear");
}
