typedef struct node_ Node;


struct node_{
	int x, y; //Node position - little waste of memory, but it allows faster generation
	void *parent; //Pointer to parent node
	char c; //Character to be displayed
	char dirs; //Directions that stil haven't been explored
};


int init();
Node *linkk( Node *n ,int ,int );
void fill_maze(char** maze, int, int);
void generate_maze(int ,int ,char**);
void show_random_maze(int ,int ,char**);
char **alloc_maze(int rows, int cols);
