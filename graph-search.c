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
        (*p+i)->link = NULL; //링크를 NULL로 초기화
        (*p+i)->key = -1; //정점이 초기화되지 않았음을 나타내기 위해 -1 대입
        visit[i] = 0; 
    }
    
    //그래프를 초기화했으므로 그래프를 확인할 요소들도 초기화
    rear = 0;
    front = 0;
    checkempty = TRUE;

    return 0;
}

//그래프의 동적할당된 메모리 영역을 할당 해제시키는 함수
void freeGraph(vertex* h)
{
    
    vertex* n = NULL; // 인접 리스트를 탐색할 포인터 n을 선언하고 NULL로 초기화
    vertex* p = NULL; // 동적 할당된 메모리를 해제할 포인터 p를 선언하고 NULL로 초기화
	
    for(int i = 0; i<MAX_Graph_SIZE; i++) //모든 정점의 인접리스트 탐색
    {
        n = h[i].link; //현재 정점의 인접 리스트의 첫 번째 노드를 n에 저장
        while(n != NULL) // n이 NULL이 될 때까지 반복해 인접 리스트의 모든 노드를 탐색
        {
            p = n; //현재 노드를 p에 저장
            n = n->link; //n을 다음 노드로 이동
            free(p); //p가 가리키는 동적 할당된 메모리 해제
        }
    }
    free(h); //정점 배열을 가리키는 동적 할당된 메모리 해제
}

// 정점을 생성하는 함수
int InsertVertex(vertex* h, int key)
{
    if(h[key].key != key) //입력한 키 값의 정점이 없을 경우
    {
        h[key].key = key; //키 값과 일치하는 정점을 생성
        return 0;
    } 
    else //입력한 키 값의 정점이 이미 존재
    {
        printf("The vertex has already been created\n"); 
        return 0;
    }
    checkempty = FALSE; //정점을 생성했거나 이미 정점이 존재하기 때문에 그래프가 비어있지 않았음을 나타냄.
}

//두 정점을 연결하는 함수
int InsertEdge(vertex* h, int vertex1, int vertex2)
{
    if(h[vertex1].key != vertex1 || h[vertex2].key != vertex2) //vertex1이나 vertex2가 존재하지 않는 경우
    {
        printf("There is no vertex"); 
        return 0; 
    }

    vertex* n = h[vertex1].link; //vertex1의 링크를 n에 저장
    vertex* p = NULL; //n의 이전 노드를 저장할 포인터 p 선언

    while(n != NULL) //n이 NULL이 될 때까지 반복
    {
        if(n->key == vertex2) //이미 edge가 존재하는 경우
        {
            printf("Edge has already been created\n");
            return 0;
        }
        n = n->link; //다음 노드로 이동
    }

    //vertex1과 vertex2를 연결하는 노드 생성
    vertex* node1 = (vertex*)malloc(sizeof(vertex));
    node1->key = vertex1;
    node1->link = NULL;

    vertex* node2 = (vertex*)malloc(sizeof(vertex));
    node2->key = vertex2;
    node2->link = NULL;
    
    if(h[vertex1].link == NULL) //vertex1의 링크가 NULL인 경우
    {
        h[vertex1].link = node2; //vertex1의 링크에 node2를 연결
    }
    else
    {
        n = h[vertex1].link; //vertex1의 링크를 n에 저장
        p = n; //n의 이전 노드를 p에 저장

        while(n != NULL) //n이 NULL이 될 때까지 반복
        {
            if(n->key > vertex2) //현재 노드의 key값이 vertex2보다 큰 경우
            {
                if(n == h[vertex1].link) //n이 vertex1의 링크인 경우
                {
                    h[vertex1].link = node2; //vertex1의 링크에 node2를 연결
                    node2->link = n; //node2의 링크에 n을 연결
                    break;
                }
                else 
                {
                    node2->link = n; //node2의 링크에 n을 연결
                    p->link = node2; //p의 링크에 node2를 연결
                    break;
                }

            }
            p = n; //p를 현재 노드로 이동
            n = n->link; //n을 다음 노드로 이동
        }
        if(n == NULL) //n이 NULL인 경우
            p->link = node2; //p의 링크에 node2를 연결
    }

    if(h[vertex2].link == NULL) //vertex2의 링크가 NULL인 경우
    {
        h[vertex2].link = node1; //vertex2의 링크에 node1을 연결
    }
    else
    {
        n = h[vertex2].link; //vertex2의 링크를 n에 저장
        p = n; //n의 이전 노드를 p에 저장

        while(n != NULL) //n이 NULL이 될 때까지 반복
        {
            if(n->key > vertex1) //현재 노드의 key값이 vertex1보다 큰 경우
            {
                if(n == h[vertex2].link) //n이 vertex2의 링크인 경우
                {
                    h[vertex2].link = node1; //vertex2의 링크에 node1을 연결
                    node1->link = n; //node1의 링크에 n을 연결
                    break;
                }
                else
                {
                    node1->link = n; //node1의 링크에 n을 연결
                    p->link = node1; //p의 링크에 node1을 연결
                    break;
                }

            }
            p = n; //p를 현재 노드로 이동
            n = n->link; //n을 다음 노드로 이동
        }
        if(n == NULL) //n이 NULL인 경우
        {
            p->link = node1; //p의 링크에 node1을 연결
        }
    }
    return 0; 
}

//그래프를 출력하는 함수
int PrintGraph(vertex* h)
{
    if(checkempty == TRUE) //그래프가 비어있는지 확인
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

                vertex* n = h[i].link; //정점의 링크를 n에 저장
            
                printf("[%d] ",i); //정점을 출력
                while(n != NULL) //정점의 링크가 NULL이 아닌 경우
                {
                    printf(" -> [%d]", n->key); //연결된 정점을 출력
                    n = n->link; //다음 정점으로 이동
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
    if(h[root].link == NULL) // 정점이 존재하지 않는 경우
    {
        printf("There is no vertex\n");
        return 0;
    }

    vertex *n; // 인접 리스트를 탐색하기 위한 포인터 n 선언
    visit[root] = 1; // 정점을 방문했음을 표시
    printf("[%d] ", root); // 정점을 출력
    for(n = h[root].link; n; n = n->link) // 인접 리스트를 순회하며 방문하지 않은 정점을 탐색
        if(!visit[n->key])
            DFS(h, n->key); // 재귀적으로 깊이 우선 탐색을 수행
}

// 너비 우선 탐색, 엣지가 여러 개일 경우 작은 vertex부터 탐색
int BFS(vertex* h, int root)
{
    if(h[root].link == NULL) // 정점이 존재하지 않는 경우
    {
        printf("There is no vertex\n");
        return 0;
    }
    visit[root] = 1; // 정점을 방문했음을 표시
    printf("[%d] ", root); // 정점을 출력
    enqueue(root); // 정점을 큐에 추가

    int p; // 큐에서 꺼낸 정점을 저장할 변수
    vertex* n = h[root].link; // 정점의 링크를 n에 저장
    while(rear != front) // 큐가 비어있지 않을 때까지 반복
    {
        p = dequeue(); // 큐에서 정점을 꺼냄
        n = h[p].link; // 정점의 링크를 n에 저장
        while(n != NULL) // n이 NULL이 될 때까지 반복
        {
            if(!visit[n->key]) // 방문하지 않은 정점을 찾음
            {
                visit[n->key] = 1; // 정점을 방문했음을 표시
                printf("[%d] ", n->key); // 정점을 출력
                enqueue(n->key); // 정점을 큐에 추가
            }
            n = n->link; // 다음 정점으로 이동
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