//Name : Parth Patel
//Roll No : 111501019
//cycle detection in directed graph

#include <stdio.h>
#include <stdlib.h>

short flag = 0 ; //to track whether cycle is detected or not
short cycleEnd = 0 ;
short cycleStart = 0 ;
int clock = 0 ;

struct member //structure for adjacencylist
{
    short m;
    struct member* next;
};
typedef struct member Member;

struct point  //structure to store details about Point
{
    short visit; //flag for visit
    short predecessor; //predecessor in DFS starting from
    int preorderTime;
    int postorderTime;
    Member* head; //head of adjacency list
};
typedef struct point Point;

void dfs(Point*, short, short);
void push(Member**, short);
short isComplete(Point*, int);
void deleteStack(Member*);
void printCycle(Point*);
void printTiming(Point*, int);

int main(void)
{   int nPoints; //number of points
    int nEdges; // number of edges

    printf("Enter the numbert of pointes : ");
    scanf("%d",&nPoints);

    printf("Enter the number of edges : ");
    scanf("%d",&nEdges);

    // array of Point structures
    Point* graph= (Point*) calloc(nPoints, sizeof(Point));
    printf("Enter edges :\nExample : 1 2 will add 2 in 1's adjacency list \n\n");
    short mem1, mem2; // variables to store inputs during Edge input

    while( nEdges-- ) //taking input for edges
    {
        printf("Edge -> ");
        scanf("%hd%hd",&mem1, &mem2);
        push( &(graph[mem1-1].head), mem2); //adding mem2 in mem1 adjacency list
    }

    printf("\nGraph is ready!");

    dfs(graph,1, 0); //starting DFS

    short startPoint; // to store point from where next DFS will start

    while ( (startPoint = isComplete(graph, nPoints)) != -1 ) //if unvisited point is there it will excecute
    {
        dfs(graph, startPoint,0);
    }

    printf("\n\n");

    if(flag==1) //if cycle is found
        printCycle(graph);
    else
        printf("\n********************\nNo Cycle is there.\n********************\n\n");

    printTiming(graph, nPoints);

    //clearing all memory allocated dynamically

    int i=0 ;
    while( i<nPoints)
    {
        deleteStack(graph[i].head);
        i++;
    }

    free(graph);
    return 0;
}

void dfs(Point* graph, short startPoint, short predecessor)
{
    graph[startPoint-1].visit = 1; //making visited
    graph[startPoint-1].predecessor = predecessor; //storing predecessor
    graph[startPoint-1].preorderTime = ++clock;

    Member *tmpPtr = NULL; //pointer for visiting membes of adjacencylist
    tmpPtr = graph[startPoint-1].head;

    while( tmpPtr != NULL )
    {
        if( graph[(tmpPtr->m)-1].visit == 0) //if not visited
        {
            dfs(graph, tmpPtr->m, startPoint);
        }
        else
        {
            if(flag==0) //cycle is not detecetd
            {
                if(  graph[(tmpPtr->m)-1].postorderTime == 0 &&\
                     ( graph[(tmpPtr->m)-1].preorderTime < graph[startPoint-1].preorderTime))  //if it is not a predecessor
                {
                    flag = 1; //flag 1 means cycle is found
                    cycleEnd = startPoint;
                    cycleStart = tmpPtr->m;
                }
            }
        }

        tmpPtr = tmpPtr->next;
    }

    graph[startPoint-1].postorderTime = ++clock;
}
// return -1 if all points are visited
// else return first unvisited point
short isComplete(Point* graph, int nPoints)
{
    int i=0;
    while( i < nPoints)
    {
        if( graph[i].visit == 0)
            return i+1;
        i++;
    }

    return -1; //if all are visited
}

void printCycle(Point* graph)
{
    printf("\n\nCycle Members : %hd ",cycleEnd);
    short tmp = cycleEnd;
    do
    {
        tmp = graph[tmp-1].predecessor;
        printf("<- %hd ",tmp);
    } while (tmp != cycleStart);
}

void push( Member** headptr, short add)
{
    Member* tmpptr = (Member*) calloc(1, sizeof(Member));
    tmpptr->m = add;

    if(*headptr == NULL)
    {
        *headptr = tmpptr;
    }
    else
    {
        tmpptr->next = *headptr;
        *headptr = tmpptr;
    }
}

void deleteStack(Member* head)
{
    if(head==NULL)
        return;
    else
    {
        Member* tmpptr;
        do
        {
            tmpptr = head;
            head = head->next;
            free(tmpptr);
        } while (head != NULL);
    }

}

void printTiming(Point* graph, int nPoints)
{
    printf("\n\n=============================================\nStarting time is 1\n\nPoint | preorderTime | PostorderTime\n");

    int i=0 ;
    while(i<nPoints)
    {
        printf("%5d | %12hd | %12hd\n",i+1, graph[i].preorderTime, graph[i].postorderTime);
        i++;
    }
    printf("=============================================\n");
}
