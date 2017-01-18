#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include "booking.h"
#define INT_MAX 12421

struct GRAPH *per_wt_cost_graph;
struct BOOKING_list booking_list;
struct BOOK_LIST book_list;

int minDistance(int dist[], int sptSet[], int V)
{
   // Initialize min value
   int min = INT_MAX, min_index, v;
  
   for (v = 0; v < V; v++)
     if (sptSet[v] == 0 && dist[v] <= min)
         min = dist[v], min_index = v;
  
   return min_index;
}
  
// A utility function to print the constructed distance array
int printSolution(int dist[], int V)
{
   int i;
   printf("Vertex   Distance from Source\n");
   for ( i = 0; i < V; i++)
      printf("%d \t\t %d\n", i, dist[i]);
}
  
// Funtion that implements Dijkstra's single source shortest path algorithm
// for a graph represented using adjacency matrix representation
int dijkstra(int **graph, int V, int src, int dest)
{
     int dist[V];     // The output array.  dist[i] will hold the shortest
                      // distance from src to i
	int count, v, u, i;
     int sptSet[V]; // sptSet[i] will true if vertex i is included in shortest
                     // path tree or shortest distance from src to i is finalized
  
     // Initialize all distances as INFINITE and stpSet[] as false
     for (i = 0; i < V; i++)
        dist[i] = INT_MAX, sptSet[i] = 0;
  
     // Distance of source vertex from itself is always 0
     dist[src] = 0;
  
     // Find shortest path for all vertices
     for (count = 0; count < V-1; count++)
     {
       // Pick the minimum distance vertex from the set of vertices not
       // yet processed. u is always equal to src in first iteration.
        u = minDistance(dist, sptSet, V);
  
       // Mark the picked vertex as processed
       sptSet[u] = 1;
  
       // Update dist value of the adjacent vertices of the picked vertex.
       for (v = 0; v < V; v++)
  
         // Update dist[v] only if is not in sptSet, there is an edge from 
         // u to v, and total weight of path from src to  v through u is 
         // smaller than current value of dist[v]
         if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX 
                                       && dist[u]+graph[u][v] < dist[v])
            dist[v] = dist[u] + graph[u][v];
     }
  
     // print the constructed distance array
     printSolution(dist, V);
     return dist[dest];
}

void amend_booking_list(struct BOOKING booking)
{
	(booking_list.list)[booking_list.n] = booking;
	booking_list.n ++;
}

struct GRAPH *generate_graph(struct GRAPH *g, int base_cost)
{
	int i, j;
	int V = per_wt_cost_graph->n;
	g->n = per_wt_cost_graph->n;
	
	g->cost = (int**) malloc(V * sizeof(int*));
	for(i=0; i<V; i++)
	{
		g->cost[i] = (int*)malloc( V * sizeof(int));
	}
	g->city = per_wt_cost_graph -> city;

	for( i =0; i < V; i++)
	{
		for(j = 0; j<V; j++)
		{
			(g->cost)[i][j] = base_cost + (per_wt_cost_graph -> cost)[i][j] ;
		}
	}
	return g;
}

int find_index(int code) /* city and codes will be from 1 to n not 0*/
{
	return code-1;
}

int find_optimum_ship_cost(struct GRAPH *g, int source_code, int dest_code)
{
	int source_index = find_index(source_code);
	int dest_index = find_index(dest_code);
	int total_cost = dijkstra(g->cost, g->n, source_index, dest_index);
	return total_cost;
}

void display_book_list()
{
	int tmp, i;
	int n = book_list.n;
	printf("Press the product key of the book to know more \n");
	for( i=0; i<n; i++)
	{
		printf("Product key : %d\nTitle=%s\ncost: %d\nNo.Of copies remaining: %d\n\n\n", (book_list.list)[i].product_key, (book_list.list)[i].title, (book_list.list)[i].cost, (book_list.list)[i].no_of_copies);
	}
	scanf("%d",&tmp);
	if(tmp==0) return;
	else display_detail_book(& (book_list.list)[find_index(tmp)]);
}
void display_detail_book(struct BOOK *book)
{
	int dest_code, ship_charge, tmp;
	struct BOOKING new_booking;
	struct GRAPH *g;
	printf("Title: %s", book->title);
	printf("Base_cost: %d\n", book->cost );
	printf("Enter city no. to select yr shippig city, enter 0 to return to previous menu\n");
	scanf("%d", &dest_code);
	if(dest_code == 0) return;
	else  /*consider the case when the entered code is invalid*/
	{	
		g = malloc(sizeof(struct GRAPH));
		g = generate_graph(g, book->cost);
		ship_charge = find_optimum_ship_cost(g, book->source_key, dest_code);
	}
	printf("Optimum cost = %d\n",ship_charge);
	printf("Do you wish to continue booking? 1/0\n");

	while(1)
	{
		scanf("%d",&tmp);
		if(tmp == 0) return;
		else if( tmp==1)
		{
			new_booking = do_booking(book, dest_code, book->cost+ship_charge);
			amend_booking_list(new_booking);
			return ;
	
		}
		else
		{
			printf("Please enter correct no. 1/0 \n");
		}
	}
}

struct BOOKING do_booking(struct BOOK *book, int delivery_code, int cost) 
{
	struct BOOKING b;
	if(book->no_of_copies == 0)
	{
		printf("Out of stock\n");
		return b;
	}

	b.book = *book;
	b.status = book -> source_key;
	book->no_of_copies = book->no_of_copies -1;
	b.shipping_city = delivery_code ;
	b.total_charge = cost;
	return b;
}

void init()
{
	int i, j;
	
	per_wt_cost_graph = malloc(sizeof(struct GRAPH));
	per_wt_cost_graph->n =3;
	per_wt_cost_graph->city = (char **) malloc(3 *sizeof(char *));
	per_wt_cost_graph->city[0] = (char *) malloc(30 *sizeof(char ));
	per_wt_cost_graph->city[1] = (char *) malloc(30 *sizeof(char ));
	per_wt_cost_graph->city[2] = (char *) malloc(30 *sizeof(char ));
	strcpy(per_wt_cost_graph->city[0], "Delhi");
	strcpy(per_wt_cost_graph -> city[1], "Bombay");
	strcpy(per_wt_cost_graph -> city[2], "Kolkata");	
	per_wt_cost_graph->cost = (int **) malloc(3 *sizeof(int *));
	per_wt_cost_graph->cost[0] = (int *) malloc(sizeof(int ));
	per_wt_cost_graph->cost[1] = (int *) malloc(sizeof(int ));
	per_wt_cost_graph->cost[2] = (int *) malloc(sizeof(int));
	for(i=0; i<3; i++)
	{
		for(j=0; j<3; j++)
		{
			per_wt_cost_graph->cost[i][j] =1;
		}
	}
	booking_list.n = 0;
	book_list.n =3;

	(book_list.list)[0] .product_key= 1;
	strcpy((book_list.list)[0].title,"myfirst");
	(book_list.list)[0] .source_key =2; /* every city has a key asociated */
	(book_list.list)[0] .cost = 500;
	strcpy((book_list.list)[0].seller_name,"Agrawal");
	(book_list.list)[0] .no_of_copies = 3;

	(book_list.list)[1] .product_key= 2;
	strcpy((book_list.list)[1].title,"mysecond");
	(book_list.list)[1] .source_key = 2; /* every city has a key asociated */
	(book_list.list)[1] .cost = 700;
	strcpy((book_list.list)[1] .seller_name, "Panwars");
	(book_list.list)[1] .no_of_copies = 2;

	(book_list.list)[2] .product_key= 3;
	strcpy((book_list.list)[2].title,"mythird");
	(book_list.list)[2] .source_key = 3; /* every city has a key asociated */
	(book_list.list)[2] .cost = 900;
	strcpy((book_list.list)[2] .seller_name,"here");
	(book_list.list)[2] .no_of_copies = 8;
}
int main()	
{
	init();
	display_book_list();
	struct ADMIN admin;
	 strcpy(admin.name, "Shikha");
	 printf("%s",admin.name);
}
