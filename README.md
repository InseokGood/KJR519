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

### 허프만 압축 알고리즘이란?
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
    char ch_list[] = { 'T', 'C', 'C', 'A' };
    int freq[] = { 90,270,120,450 };
    huffman_tree(freq, ch_list, 4);
    return 0;
}
```
