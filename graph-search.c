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
        (*p+i)->link = NULL; //��ũ�� NULL�� �ʱ�ȭ
        (*p+i)->key = -1; //������ �ʱ�ȭ���� �ʾ����� ��Ÿ���� ���� -1 ����
        visit[i] = 0; 
    }
    
    //�׷����� �ʱ�ȭ�����Ƿ� �׷����� Ȯ���� ��ҵ鵵 �ʱ�ȭ
    rear = 0;
    front = 0;
    checkempty = TRUE;

    return 0;
}

//�׷����� �����Ҵ�� �޸� ������ �Ҵ� ������Ű�� �Լ�
void freeGraph(vertex* h)
{
    
    vertex* n = NULL; // ���� ����Ʈ�� Ž���� ������ n�� �����ϰ� NULL�� �ʱ�ȭ
    vertex* p = NULL; // ���� �Ҵ�� �޸𸮸� ������ ������ p�� �����ϰ� NULL�� �ʱ�ȭ
	
    for(int i = 0; i<MAX_Graph_SIZE; i++) //��� ������ ��������Ʈ Ž��
    {
        n = h[i].link; //���� ������ ���� ����Ʈ�� ù ��° ��带 n�� ����
        while(n != NULL) // n�� NULL�� �� ������ �ݺ��� ���� ����Ʈ�� ��� ��带 Ž��
        {
            p = n; //���� ��带 p�� ����
            n = n->link; //n�� ���� ���� �̵�
            free(p); //p�� ����Ű�� ���� �Ҵ�� �޸� ����
        }
    }
    free(h); //���� �迭�� ����Ű�� ���� �Ҵ�� �޸� ����
}

// ������ �����ϴ� �Լ�
int InsertVertex(vertex* h, int key)
{
    if(h[key].key != key) //�Է��� Ű ���� ������ ���� ���
    {
        h[key].key = key; //Ű ���� ��ġ�ϴ� ������ ����
        return 0;
    } 
    else //�Է��� Ű ���� ������ �̹� ����
    {
        printf("The vertex has already been created\n"); 
        return 0;
    }
    checkempty = FALSE; //������ �����߰ų� �̹� ������ �����ϱ� ������ �׷����� ������� �ʾ����� ��Ÿ��.
}

//�� ������ �����ϴ� �Լ�
int InsertEdge(vertex* h, int vertex1, int vertex2)
{
    if(h[vertex1].key != vertex1 || h[vertex2].key != vertex2) //vertex1�̳� vertex2�� �������� �ʴ� ���
    {
        printf("There is no vertex"); 
        return 0; 
    }

    vertex* n = h[vertex1].link; //vertex1�� ��ũ�� n�� ����
    vertex* p = NULL; //n�� ���� ��带 ������ ������ p ����

    while(n != NULL) //n�� NULL�� �� ������ �ݺ�
    {
        if(n->key == vertex2) //�̹� edge�� �����ϴ� ���
        {
            printf("Edge has already been created\n");
            return 0;
        }
        n = n->link; //���� ���� �̵�
    }

    //vertex1�� vertex2�� �����ϴ� ��� ����
    vertex* node1 = (vertex*)malloc(sizeof(vertex));
    node1->key = vertex1;
    node1->link = NULL;

    vertex* node2 = (vertex*)malloc(sizeof(vertex));
    node2->key = vertex2;
    node2->link = NULL;
    
    if(h[vertex1].link == NULL) //vertex1�� ��ũ�� NULL�� ���
    {
        h[vertex1].link = node2; //vertex1�� ��ũ�� node2�� ����
    }
    else
    {
        n = h[vertex1].link; //vertex1�� ��ũ�� n�� ����
        p = n; //n�� ���� ��带 p�� ����

        while(n != NULL) //n�� NULL�� �� ������ �ݺ�
        {
            if(n->key > vertex2) //���� ����� key���� vertex2���� ū ���
            {
                if(n == h[vertex1].link) //n�� vertex1�� ��ũ�� ���
                {
                    h[vertex1].link = node2; //vertex1�� ��ũ�� node2�� ����
                    node2->link = n; //node2�� ��ũ�� n�� ����
                    break;
                }
                else 
                {
                    node2->link = n; //node2�� ��ũ�� n�� ����
                    p->link = node2; //p�� ��ũ�� node2�� ����
                    break;
                }

            }
            p = n; //p�� ���� ���� �̵�
            n = n->link; //n�� ���� ���� �̵�
        }
        if(n == NULL) //n�� NULL�� ���
            p->link = node2; //p�� ��ũ�� node2�� ����
    }

    if(h[vertex2].link == NULL) //vertex2�� ��ũ�� NULL�� ���
    {
        h[vertex2].link = node1; //vertex2�� ��ũ�� node1�� ����
    }
    else
    {
        n = h[vertex2].link; //vertex2�� ��ũ�� n�� ����
        p = n; //n�� ���� ��带 p�� ����

        while(n != NULL) //n�� NULL�� �� ������ �ݺ�
        {
            if(n->key > vertex1) //���� ����� key���� vertex1���� ū ���
            {
                if(n == h[vertex2].link) //n�� vertex2�� ��ũ�� ���
                {
                    h[vertex2].link = node1; //vertex2�� ��ũ�� node1�� ����
                    node1->link = n; //node1�� ��ũ�� n�� ����
                    break;
                }
                else
                {
                    node1->link = n; //node1�� ��ũ�� n�� ����
                    p->link = node1; //p�� ��ũ�� node1�� ����
                    break;
                }

            }
            p = n; //p�� ���� ���� �̵�
            n = n->link; //n�� ���� ���� �̵�
        }
        if(n == NULL) //n�� NULL�� ���
        {
            p->link = node1; //p�� ��ũ�� node1�� ����
        }
    }
    return 0; 
}

//�׷����� ����ϴ� �Լ�
int PrintGraph(vertex* h)
{
    if(checkempty == TRUE) //�׷����� ����ִ��� Ȯ��
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

                vertex* n = h[i].link; //������ ��ũ�� n�� ����
            
                printf("[%d] ",i); //������ ���
                while(n != NULL) //������ ��ũ�� NULL�� �ƴ� ���
                {
                    printf(" -> [%d]", n->key); //����� ������ ���
                    n = n->link; //���� �������� �̵�
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
    if(h[root].link == NULL) // ������ �������� �ʴ� ���
    {
        printf("There is no vertex\n");
        return 0;
    }

    vertex *n; // ���� ����Ʈ�� Ž���ϱ� ���� ������ n ����
    visit[root] = 1; // ������ �湮������ ǥ��
    printf("[%d] ", root); // ������ ���
    for(n = h[root].link; n; n = n->link) // ���� ����Ʈ�� ��ȸ�ϸ� �湮���� ���� ������ Ž��
        if(!visit[n->key])
            DFS(h, n->key); // ��������� ���� �켱 Ž���� ����
}

// �ʺ� �켱 Ž��, ������ ���� ���� ��� ���� vertex���� Ž��
int BFS(vertex* h, int root)
{
    if(h[root].link == NULL) // ������ �������� �ʴ� ���
    {
        printf("There is no vertex\n");
        return 0;
    }
    visit[root] = 1; // ������ �湮������ ǥ��
    printf("[%d] ", root); // ������ ���
    enqueue(root); // ������ ť�� �߰�

    int p; // ť���� ���� ������ ������ ����
    vertex* n = h[root].link; // ������ ��ũ�� n�� ����
    while(rear != front) // ť�� ������� ���� ������ �ݺ�
    {
        p = dequeue(); // ť���� ������ ����
        n = h[p].link; // ������ ��ũ�� n�� ����
        while(n != NULL) // n�� NULL�� �� ������ �ݺ�
        {
            if(!visit[n->key]) // �湮���� ���� ������ ã��
            {
                visit[n->key] = 1; // ������ �湮������ ǥ��
                printf("[%d] ", n->key); // ������ ���
                enqueue(n->key); // ������ ť�� �߰�
            }
            n = n->link; // ���� �������� �̵�
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