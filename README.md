## 허프만  알고리즘 
#### 허프만 압축이 필요한이유 ?
흔히 우리는 무거운 파일을 전송할 때는 .zip파일로 압축을 해서 보낸다. 압축을 하면 용량을 적게 차지하므로 효율적이기 때문이다.

#### 어떻게 압축할 수 있을까요?
최대한 적은 비트로 문자를 표현한다.

#### a부터 z까지 차례대로 특정한 비트를 부여 하는 것이 합리적일까요?
a = 01100001, b=01100010,...

많이 발생하는 문자에 적은 비트를 할당하는게 좋다.

a = 0; b =1, c=10,...

#### 어떻게 구현할수 있나요?
트리구조를 사용한다.

#### 허프만 압축 알고리즘이란?
1.발생 빈도가 가장 낮은 두문자를 선택해서 하나로 합칩니다.

2.왼쪽은 빈도수가 낮고 오른쪽은 높습니다.

3.빈도수가 같은 것 중에서는 작은것을 우선적으로 연결합니다.

4.트리가 생성되면 차례대로 0과 1을 부여합니다.
![참고 이미지](https://img1.daumcdn.net/thumb/R1280x0/?scode=mtistory2&fname=https%3A%2F%2Fblog.kakaocdn.net%2Fdn%2Fc58ACk%2Fbtq33aZ2ZTw%2FIUWs7EXbb766PJXXVJVMpk%2Fimg.png)

위에 이미지를 문자 A, G, C, T에 대한 빈도수가 각각 450, 120, 270, 90일 경우 다음과 같은 순서를 따른다

결과적으로 A, G, C, T는 각각 허프만 코드로 0, 100, 11, 101이 된다.

이진 트리의 특성에 따라 빈도수가 높을 수록 루트 노드와 가까우므로 짧은 코드에 대응되고, 접두부 특성을 지님을 알 수 있다.

#### 압축비

허프만 코드로 변환하기 전 문자들의 용량은 (450 + 120 + 270 + 90) * 1byte * 8bit/1byte = 7440bit이다.

변환 후의 용량은 (450 * 1) + (120 * 3) + (270 * 2) + (90 * 3) = 1620bit이다.

따라서 압축비 = (압축 전 크기) / (압축 후 크기) = 7440 / 1620 = 4.593 이다.

약 1/4.6의 크기로 압축된 것을 알 수 있다
#### 시간복잡도

최소 값 추출에 logN의 시간을 소모하고, N개의 원소를 이용해 트리를 만들기 때문에 O(NlogN)의 시간복잡도를 가진다.
```c
//최소히프를 사용한 허프만 코드
#include<stdio.h>
#include<stdlib.h>
#define MAX_ELEMENT 200

typedef struct TreeNode {
    int weight;//출현 빈도수 의미
    char ch;//문자하나를 의미
    struct TreeNode* left;//왼쪽노드
    struct TrreeNode* right;//오른쪽노드
} TreeNode;

typedef struct {
    TreeNode* ptree;
    char ch;
    int key;
} element;

typedef struct {
    element heap[MAX_ELEMENT];
    int heap_size;
} HeapType;

//생성 함수
HeapType* create()
{
    return (HeapType*)malloc(sizeof(HeapType));
}

//초기화 함수
void init(HeapType* h)
{
    h->heap_size = 0;
}

//현재 요소의 개수가 heap_size인 히프 h에 item을 삽입한다.
//삽입 함수
void insert_min_heap(HeapType* h, element item)
{
    int i;
    i = ++(h->heap_size);

    //트리를 거슬러 올라가면서 부모 노드와 비교하는 과정
    while ((i != 1) && (item.key < h->heap[i / 2].key)) {
        h->heap[i] = h->heap[i / 2];
        i /= 2;
    }
    h->heap[i] = item; // 새로운 노드 삽입
}

//삭제 함수
element delete_min_heap(HeapType* h)
{
    int parent, child;
    element item, temp;

    item = h->heap[1];
    temp = h->heap[(h->heap_size)--];
    parent = 1;
    child = 2;
    while (child <= h->heap_size) {
        //현재 노드의 자식노드중 더 작은 자식노드를 찾는다.
        if ((child < h->heap_size) && (h->heap[child].key) > h->heap[child + 1].key)
            child++;
        if (temp.key < h->heap[child].key) break;

        //한 단계 아래로 이동
        h->heap[parent] = h->heap[child];
        parent = child;
        child *= 2;
    }
    h->heap[parent] = temp;
    return item;
}

//이진 트리 생성 함수
TreeNode* make_tree(TreeNode* left, TreeNode* right)
{
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->left = left;
    node->right = right;
    return node;
}

//이진 트리 제거 함수
void destroy_tree(TreeNode* root)
{
    if (root == NULL) return;
    destroy_tree(root->left);
    destroy_tree(root->right);
    free(root);
}

int is_leaf(TreeNode* root)
{
    return !(root->left) && !(root->right);
}

void print_array(int codes[], int n)
{
    for (int i = 0; i < n; i++)
        printf("%d", codes[i]);
    printf("\n");
}

void print_codes(TreeNode* root, int codes[], int top)
{
    //1을 저장하고 순환호출한다.
    if (root->left) {
        codes[top] = 1;
        print_codes(root->left, codes, top + 1);
    }
    //0을 저장하고 순환호출한다.
    if (root->left) {
        codes[top] = 0;
        print_codes(root->right, codes, top + 1);
    }
    //단말노드이면 코드를 출력한다.
    if (is_leaf(root)) {
        printf("%c: ", root->ch);
        print_array(codes, top);
    }
}

//허프만 코드 생성 함수
void huffman_tree(int freq[], char ch_list[], int n)
{
    int i;
    TreeNode* node, * x;
    HeapType* heap;
    element e, e1, e2;
    int codes[100];
    int top = 0;

    heap = create();
    init(heap);
    for (i = 0; i < n; i++) {
        node = make_tree(NULL, NULL);
        e.ch = node->ch = ch_list[i];
        e.key = node->weight = freq[i];
        e.ptree = node;
        insert_min_heap(heap, e);
    }

    for (i = 1; i < n; i++) {
        //최소값을 가지는 두 개의 노드를 삭제
        e1 = delete_min_heap(heap);
        e2 = delete_min_heap(heap);

        //두개의 노드를 합친다. 
        x = make_tree(e1.ptree, e2.ptree);
        e.key = x->weight = e1.key + e2.key;
        e.ptree = x;
        printf("%d+%d->%d \n", e1.key, e2.key, e.key);
        insert_min_heap(heap, e);
    } 
    e = delete_min_heap(heap);
    print_codes(e.ptree, codes, top);
    destroy_tree(e.ptree);
    free(heap);
}

int main(void)
{
    char ch_list[] = { 'T', 'C', 'G', 'A' };
    int freq[] = { 90,270,120,450 };
    huffman_tree(freq, ch_list, 4);
    return 0;
}
```
#### 출력화면
![2022-04-12 (4)](https://user-images.githubusercontent.com/101339244/163328258-7b6c4038-effa-4acc-a534-0f1977e4d521.png)


### 성능
허프만 트리를 만드는데 걸리는 시간 + 길이 m인 텍스트의  실제 인코딩 시간

O(nlogn)+O(m)=O(nlogn+m)

logn에서 n은 문자 집합의 크기

#### 참조자료: c언어로 쉽게풀어쓴 자료구조 교재 허프만 코드 부분


#### 대문자 입력후 허프만 코드 출력 
```c
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#pragma warning(disable:4996)
#define MAX 100
#define alph_num 26

//트리 노드
typedef struct node
{
	char alphabet;  //알파벳
	int freq;//빈도수
	struct node* left; //왼쪽 자식 노드
	struct node* right; //오른쪽 자식 노드
}node;

node* make_Huffman_tree(char arr[]);  //허프만 코드 트리 생성(압축하고자 하는 문자열)
node* makenode(char alphabet, int freq, struct node* left, struct node* right); //새 노드 생성
void make_table(node* n, char str[], int len, char* table[]); //알파벳 별 가변길이 코드 배열 생성
void decode(char* str, node* root); //디코드함수
node node_arr[alph_num] = { NULL };
int ind = 0;//문자 갯수

//새 노드 생성(알파벳,빈도수,왼쪽 자식 노드,오른쪽 자식 노드) 
node* makenode(char alphabet, int freq, struct node* left, struct node* right)
{
	node* new_node = (node*)malloc(sizeof(node));
	new_node->alphabet = alphabet;
	new_node->freq = freq;
	new_node->left = left;
	new_node->right = right;
	return new_node;
}

//허프만 코드 트리 생성(압축하고자 하는 문자열)
node* make_Huffman_tree(char arr[])
{
	int i = 0;
	int j;
	int temp_n = 0;
	int min = 0;  //제일 빈도수가 작은 index
	int min2 = 0; //두 번째로 빈도수가 작은 index
	int fre[alph_num] = { 0 };  //알파벳(A~Z) 빈도 수
	int check[alph_num] = { 0 };  //합쳐졌는지 확인(합쳐져서 살펴 볼 필요가 없으면 -1)
	node* tree[alph_num] = { NULL };  //비교할 노드 배열
	node* new_node; //새 노드

	while (arr[i] != NULL)
	{
		//빈도수 구하기
		fre[arr[i] - 'A']++;
		i++;
	}
	for (int i = 0; i < alph_num; i++)
	{
		//알파벳이 존재하면
		if (fre[i] != 0)
		{
			node_arr[ind] = *makenode(i + 'A', fre[i], NULL, NULL);
			tree[ind++] = makenode(i + 'A', fre[i], NULL, NULL); //노드 생성
		}
	}
	for (i = 0; i < ind - 1; i++)
	{
		//가장 작은 수 찾기
		j = 0;
		while (check[j] == -1)	j++; //합쳐진 노드를 제외한 배열 중 가장 앞 index
		min = j; //우선 제일 작다고 가정

		for (j = min; j < ind-1; j++) //모든 배열을 검사
			if (check[j] != -1) //이미 합쳐진 노드가 아니면(검사해볼 필요가 있으면)
				if (tree[min]->freq > tree[j]->freq)
					//min인덱스 빈도수 보다 빈도수가 작은 경우
					min = j;

		//두번째로 작은 수 찾기
		j = 0;
		while (check[j] == -1 || j == min) j++;
		//합쳐진 노드와 최소 노드 제외한 배열 중 가장 앞 index
		min2 = j;  //두번째로 작다고 가정

		for (j = min2; j < ind; j++)
			if (check[j] != -1) //이미 합쳐진 노드가 아니면
				if (tree[min2]->freq > tree[j]->freq)
					//min2인덱스 빈도수 보다 빈도수가 작은 경우
					if (j != min) //가장 작은 index가 아닌 경우
						min2 = j;

		tree[min] = makenode(NULL, tree[min]->freq + tree[min2]->freq, tree[min], tree[min2]);
		//min과 min2인덱스의 빈도수를 합친 빈도수로 새 노드 생성
		//새로 만든 노드를 min인덱스 자리에 넣는다.

		check[min2] = -1;
		//min2인덱스는 min인덱스 자리의 노드에 합쳐졌으므로 check[min2]<-=1
	}
	return tree[min]; //만들어진 트리의 루트 노드 반환
}

//알파벳 별 가변길이 코드 배열 생성
//(트리 루트 노드,가변 길이 코드 문자열,문자열에 들어갈 위치, 저장 될 배열)
void make_table(node* n, char str[], int len, char* table[])
{
	if (n->left == NULL && n->right == NULL) //n이 단노드인 경우
	{
		str[len] = '\0'; //문장의 끝을 str끝에 넣어주고
						 //단 노드의 알파벳을 확인하여 table의 적절한 위치에 str문자열을 넣는다.
		strcpy(table[(n->alphabet)-'A'], str);
	}
	else //단 노드가 아닌 경우
	{
		if (n->left != NULL) //왼쪽 자식이 있는 경우
		{
			str[len] = '0'; //문자열에 0 삽입
			make_table(n->left, str, len + 1, table);
			//재귀호출(문자열에 들어갈 위치에 +1)
		}
		if (n->right != NULL) //오른쪽 자식이 있는 경우
		{
			str[len] = '1'; //문자열에 1 삽입
			make_table(n->right, str, len + 1, table);
			//재귀호출(문자열에 들어갈 위치에 +1)
		}
	}
}

//디코드함수(디코딩 하고 싶은 문자열, 트리 루트 노드)
void decode(char* str, node* root)
{
	int i = 0;
	node* j = root;
	while (str[i] != '\0') //문자의 끝이 아닌 경우
	{
		if (str[i] == '0') //문자가 0인 경우
			j = j->left; //왼쪽 자식으로 이동
		else if (str[i] == '1') //문자가 1인 경우
			j = j->right; //오른쪽 자식으로 이동
		if (j->left == NULL && j->right == NULL) //단 노드인 경우
		{
			printf("%c", j->alphabet); //단 노드의 알파벳 출력
			j = root;
		}
		i++;
	}
	printf("\n");
}

//메인 함수
int main()
{
	char arr[MAX]; //압축하고자 하는 문자열
	char* code[alph_num]; //각 알파벳에 대한 가변길이 코드 배열
	char str[MAX]; //문자열 변수
	char encoding[MAX] = ""; //인코딩해서 나온 이진수 배열
	int i; //반복문 변수
	char answer; //디코딩 원하는가에 대한 대답 변수
	node* root;//트리의 루트

	for (i = 0; i < alph_num; i++)
		code[i] = (char*)malloc(sizeof(char));
	printf("압축하고자 하는 문자열(대문자) : ");
	scanf("%s", arr); //압축하고자 하는 문자열 입력

	root = make_Huffman_tree(arr); //허프만코드를 이용한 트리 생성
	make_table(root, str, 0, code); //트리를 사용한 알파벳 별 가변길이 코드 생성

	i = 0;
	while (arr[i] != NULL) { //입력받은 문자열이 끝날때까지
		strcat(encoding, code[arr[i] - 'A']); //문자별 코드 인코딩 문자열 뒤에 넣기
		strcat(encoding, " ");
		i++;
	}

	for (i = 0; i < ind; i++)
		printf("%c : %s\n", node_arr[i].alphabet, code[node_arr[i].alphabet -'A']);

	printf("압축 결과 : %s\n", encoding); //인코딩 한 이진수 배열 출력
	
	printf("압축 해제 : ");
	decode(encoding, root); //디코딩 함수 호출
	return 0;
}
```
#### 출력 화면
![2022-04-14 (1)](https://user-images.githubusercontent.com/101339244/163326197-5070bce6-22bc-4864-8ffa-707f683536a7.png)
