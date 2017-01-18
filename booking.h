
struct ADMIN
{
	char name[30];
	char username[30];
	char pass[30];	
};

struct user
{
	char name[30];
	char username[30];
	char pass[30];
};

struct BOOK
{
	int product_key;
	char title[30];
	int source_key; /* every city has a key asociated */
	int cost;
	char seller_name[30];
	int no_of_copies; /* stores the no. of copies left*/
};

struct BOOKING
{
	struct BOOK book;
	int status; /*stores the current city code*/
	int total_charge;
	int shipping_city;
};
struct BOOK_LIST
{
	int n;
	struct BOOK list[30];
};
struct BOOKING_list
{
	int n;
	struct BOOKING list[30];
};

struct GRAPH /*stores as a 2d matrix */
{
	int n; /* stores the no. of cities */
	char  **city; /*array of cities*/
	int **cost; /*0 based array*/
};

int minDistance(int dist[], int sptSet[], int v);
int printSolution(int dist[], int n);
int minDistance(int dist[], int sptSet[], int V);
int dijkstra(int **graph, int V, int src, int dest);
void display_detail_book(struct BOOK *book);
struct GRAPH *generate_graph(struct GRAPH *g, int base_cost);
int find_index(int code);
int find_optimum_ship_cost(struct GRAPH *g, int source_index, int dest_code);
struct BOOKING do_booking(struct BOOK *book, int delivery_code, int cost);
void amend_booking_list(struct BOOKING booking);
void init();