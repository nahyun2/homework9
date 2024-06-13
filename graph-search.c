#include <stdio.h>
#include <stdlib.h>

#define MAX_Graph_SIZE 10 //최대 Vertex의 수를 10개로 지정
#define MAX_QUEUE_SIZE 100
#define TRUE 1
#define FALSE 0

// 인접리스트를 구현하기 위한 구조체
typedef struct adjLists
{
	int key; // vertex의 key값
	struct adjLists* link; // 다음 vertex를 가리키는 포인터
}vertex;

int visit[MAX_Graph_SIZE] = {0}; //방문한 vertex를 표시하기 위한 배열
int checkempty = TRUE; //TRUE일 경우 그래프가 비어있음을 나타냄
int Queue[MAX_QUEUE_SIZE] = {0}; //BFS를 위한 큐를 생성
int front, rear = 0; //큐의 front와 rear를 나타내는 변수


int InitializeGraph(vertex** p);
void freeGraph(vertex* h);
int InsertVertex(vertex* h, int key);
int InsertEdge(vertex* h, int vertex1, int vertex2);
int PrintGraph(vertex* h);
int DFS(vertex* h, int root);
int BFS(vertex* h, int root);
int enqueue(int i);
int dequeue();
void visitReset();

int main()
{
	char command;
    int key;
	int vertex1, vertex2;
	vertex* Graph = NULL; 

	
	do{
		printf("\n\n");
        printf("-------------[-----[최나현]    [2023041039]-----]---------------\n");
		printf("----------------------------------------------------------------\n");
		printf("                      Graph Searches                           \n");
		printf("----------------------------------------------------------------\n");
		printf(" Initialize Graph       = z                                     \n");
		printf(" Insert Vertex          = v      Insert Edge             = e \n");
		printf(" Depth First Search     = d      Breath First Search     = b \n");
		printf(" Print Graph            = p      Quit                    = q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'z': case 'Z':
			InitializeGraph(&Graph);
			break;

		case 'q': case 'Q':
			freeGraph(Graph);
			break;

		case 'v': case 'V':
            printf("key : ");
            scanf("%d",&key);
			InsertVertex(Graph, key);
			break;

		case 'd': case 'D':
            printf("select vertex :");
            scanf("%d",&key);
            visitReset();
			DFS(Graph,key);
			break;

		case 'e': case 'E':
			printf("choose two vertex : ");
			scanf("%d %d",&vertex1, &vertex2);
            InsertEdge(Graph,vertex1,vertex2);
			break;

		case 'b': case 'B':
            printf("select vertex : ");
            scanf("%d",&key);
            visitReset();
			BFS(Graph,key);
			break;

		case 'p': case 'P':
			PrintGraph(Graph);
			break;

		default:
			printf("\n       Please enter again     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}

//인접리스트를 초기화하는 함수
int InitializeGraph(vertex** p)
{
    if(*p != NULL) //이미 그래프가 존재한다면
    {
        freeGraph(*p); //그래프를 해제
    }

	*p = (vertex*)malloc(sizeof(vertex)*MAX_Graph_SIZE); //vertex의 크기만큼 메모리 할당

    for(int i = 0; i<MAX_Graph_SIZE; i++) 
    {
        (*p+i)->link = NULL;
        (*p+i)->key = -1; //정점이 초기화되지 않았음을 나타내기 위해 -1 대입
        visit[i] = 0;
    }
    
    
    rear = 0;
    front = 0;
    checkempty = TRUE;

    return 0;
}

//그래프의 동적할당된 메모리 영역을 할당 해제시키는 함수
void freeGraph(vertex* h)
{
    
    vertex* n = NULL;
    vertex* p = NULL;
	
    for(int i = 0; i<MAX_Graph_SIZE; i++) //10번 반복
    {
        n = h[i].link; 
        while(n != NULL)
        {
            p = n;
            n = n->link;
            free(p); 
        }
    }
    free(h); 
}

// 정점을 생성하는 함수
int InsertVertex(vertex* h, int key)
{
    if(h[key].key != key) 
    {
        h[key].key = key; 
        return 0;
    } 
    else 
    {
        printf("The vertex has already been created\n"); 
        return 0;
    }
    checkempty = FALSE; //그래프가 비어있지 않음을 나타냄
}

//두 정점을 연결하는 함수
int InsertEdge(vertex* h, int vertex1, int vertex2)
{
    if(h[vertex1].key != vertex1 || h[vertex2].key != vertex2) //vertex1이나 vertex2가 존재하지 않는 경우
    {
        printf("There is no vertex"); 
        return 0; 
    }

    vertex* n = h[vertex1].link;
    vertex* p = NULL;

    while(n != NULL)
    {
        if(n->key == vertex2) 
        {
            printf("Edge has already been created\n");
            return 0;
        }
        n = n->link;
    }

    
    vertex* node1 = (vertex*)malloc(sizeof(vertex));
    node1->key = vertex1;
    node1->link = NULL;

    vertex* node2 = (vertex*)malloc(sizeof(vertex));
    node2->key = vertex2;
    node2->link = NULL;
    
    if(h[vertex1].link == NULL) 
    {
        h[vertex1].link = node2;
    }
    else
    {
        n = h[vertex1].link;
        p = n;

        while(n != NULL)
        {
            if(n->key > vertex2) 
            {
                if(n == h[vertex1].link) 
                {
                    h[vertex1].link = node2; 
                    node2->link = n;
                    break;
                }
                else 
                {
                    node2->link = n;
                    p->link = node2;
                    break;
                }

            }
            p = n;
            n = n->link;
        }
        if(n == NULL) 
            p->link = node2; 
    }

    if(h[vertex2].link == NULL)
    {
        h[vertex2].link = node1;
    }
    else
    {
        n = h[vertex2].link;
        p = n;

        while(n != NULL)
        {
            if(n->key > vertex1)
            {
                if(n == h[vertex2].link)
                {
                    h[vertex2].link = node1;
                    node1->link = n;
                    break;
                }
                else
                {
                    node1->link = n;
                    p->link = node1;
                    break;
                }

            }
            p = n;
            n = n->link;
        }
        if(n == NULL)
        {
            p->link = node1;
        }
    }
    return 0; 
}

int PrintGraph(vertex* h)
{
    if(checkempty == TRUE) 
    {
        printf("Graph is empty\n");
        return 0;
    }
    else
    {
        for(int i = 0; i<MAX_Graph_SIZE; i++) //모든 정점을 확인
        {
            if(h[i].key == i) //정점이 존재하는 경우 
            {

                vertex* n = h[i].link;
            
                printf("[%d] ",i); 
                while(n != NULL)
                {
                    printf(" -> [%d]",n->key); 
                    n = n->link;
                }
                printf("\n");
             
            }

        }
    }
    
    return 0;
}

//깊이 우선 탐색, 엣지가 여러개일 경우 작은 vertex부터 탐색
int DFS(vertex* h, int root) 
{
    if(h[root].link == NULL) 
    {
        printf("There is no vertex\n"); 
        return 0;
    }

    vertex *n; 
    visit[root] = 1; 
    printf("[%d] ",root); 
    for(n = h[root].link; n; n = n->link)
        if(!visit[n->key])
            DFS(h,n->key); 
}

//너비 우선 탐색, 엣지가 여러개일 경우 작은 vertex부터 탐색
int BFS(vertex* h, int root)
{

    if(h[root].link == NULL) //vertex가 존재하지 않는 경우
    {
        printf("There is no vertex\n");
        return 0;
    }
    visit[root] = 1; 
    printf("[%d] ",root); 
    enqueue(root); 

    int p;
    vertex* n = h[root].link; 
    while(rear != front)
    {
        p = dequeue();
        n = h[p].link; 
        while(n != NULL) 
        {
            if(!visit[n->key]) 
            {
                visit[n->key] = 1; 
                printf("[%d] ",n->key); 
                enqueue(n->key); 
            }
            n = n->link;
        }
    } 
}

//큐에 값을 넣는 함수
int enqueue(int i) 
{
    if((rear+1)%MAX_QUEUE_SIZE == front) //큐가 가득 찼는지 확인
    {
        printf("Queue is FULL\n");
        return 0;
    }

    rear = (rear+1)%MAX_QUEUE_SIZE; //rear를 1 증가시킴
    Queue[rear] = i; //rear가 가리키는 위치에 i값을 저장
    return 0;
}

//큐에서 값을 꺼내는 함수
int dequeue() 
{
    if(front == rear) //큐가 비어있는지 확인
    {
        printf("Queue is empty\n");
        return 0;
    }

    front = (front+1)%MAX_QUEUE_SIZE; //front를 1 증가시킴
    return Queue[front]; //front가 가리키는 값을 반환
}

//visit배열을 0으로 초기화하는 함수
void visitReset() 
{
    for(int i = 0; i<MAX_Graph_SIZE; i++)
        visit[i] = 0; 
}