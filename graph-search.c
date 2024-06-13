#include <stdio.h>
#include <stdlib.h>

#define MAX_Graph_SIZE 10 //�ִ� Vertex�� ���� 10���� ����
#define MAX_QUEUE_SIZE 100
#define TRUE 1
#define FALSE 0

// ��������Ʈ�� �����ϱ� ���� ����ü
typedef struct adjLists
{
	int key; // vertex�� key��
	struct adjLists* link; // ���� vertex�� ����Ű�� ������
}vertex;

int visit[MAX_Graph_SIZE] = {0}; //�湮�� vertex�� ǥ���ϱ� ���� �迭
int checkempty = TRUE; //TRUE�� ��� �׷����� ��������� ��Ÿ��
int Queue[MAX_QUEUE_SIZE] = {0}; //BFS�� ���� ť�� ����
int front, rear = 0; //ť�� front�� rear�� ��Ÿ���� ����


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
        printf("-------------[-----[�ֳ���]    [2023041039]-----]---------------\n");
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

//��������Ʈ�� �ʱ�ȭ�ϴ� �Լ�
int InitializeGraph(vertex** p)
{
    if(*p != NULL) //�̹� �׷����� �����Ѵٸ�
    {
        freeGraph(*p); //�׷����� ����
    }

	*p = (vertex*)malloc(sizeof(vertex)*MAX_Graph_SIZE); //vertex�� ũ�⸸ŭ �޸� �Ҵ�

    for(int i = 0; i<MAX_Graph_SIZE; i++) 
    {
        (*p+i)->link = NULL;
        (*p+i)->key = -1; //������ �ʱ�ȭ���� �ʾ����� ��Ÿ���� ���� -1 ����
        visit[i] = 0;
    }
    
    
    rear = 0;
    front = 0;
    checkempty = TRUE;

    return 0;
}

//�׷����� �����Ҵ�� �޸� ������ �Ҵ� ������Ű�� �Լ�
void freeGraph(vertex* h)
{
    
    vertex* n = NULL;
    vertex* p = NULL;
	
    for(int i = 0; i<MAX_Graph_SIZE; i++) //10�� �ݺ�
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

// ������ �����ϴ� �Լ�
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
    checkempty = FALSE; //�׷����� ������� ������ ��Ÿ��
}

//�� ������ �����ϴ� �Լ�
int InsertEdge(vertex* h, int vertex1, int vertex2)
{
    if(h[vertex1].key != vertex1 || h[vertex2].key != vertex2) //vertex1�̳� vertex2�� �������� �ʴ� ���
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
        for(int i = 0; i<MAX_Graph_SIZE; i++) //��� ������ Ȯ��
        {
            if(h[i].key == i) //������ �����ϴ� ��� 
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

//���� �켱 Ž��, ������ �������� ��� ���� vertex���� Ž��
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

//�ʺ� �켱 Ž��, ������ �������� ��� ���� vertex���� Ž��
int BFS(vertex* h, int root)
{

    if(h[root].link == NULL) //vertex�� �������� �ʴ� ���
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

//ť�� ���� �ִ� �Լ�
int enqueue(int i) 
{
    if((rear+1)%MAX_QUEUE_SIZE == front) //ť�� ���� á���� Ȯ��
    {
        printf("Queue is FULL\n");
        return 0;
    }

    rear = (rear+1)%MAX_QUEUE_SIZE; //rear�� 1 ������Ŵ
    Queue[rear] = i; //rear�� ����Ű�� ��ġ�� i���� ����
    return 0;
}

//ť���� ���� ������ �Լ�
int dequeue() 
{
    if(front == rear) //ť�� ����ִ��� Ȯ��
    {
        printf("Queue is empty\n");
        return 0;
    }

    front = (front+1)%MAX_QUEUE_SIZE; //front�� 1 ������Ŵ
    return Queue[front]; //front�� ����Ű�� ���� ��ȯ
}

//visit�迭�� 0���� �ʱ�ȭ�ϴ� �Լ�
void visitReset() 
{
    for(int i = 0; i<MAX_Graph_SIZE; i++)
        visit[i] = 0; 
}