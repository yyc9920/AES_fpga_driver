#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#define MAX_WORD_LENGTH 20
#define MAX_MEAN_LENGTH 200

typedef struct
{
    char transName[MAX_WORD_LENGTH];
    int money;
} transInfo;

typedef struct
{
    char accountNum[MAX_WORD_LENGTH];
    char userName[MAX_WORD_LENGTH];
    transInfo *transinfo[10]; //TODO : Segmantation fault 에러 고치기
    int randNum;
    int otp_Num;
    int money;
    int transCnt;
} element;

typedef struct treeNode
{
    element key;
    struct treeNode *left;
    struct treeNode *right;
} treeNode;

// 포인터 p가 가리키는 노드와 비교하여 항목 key를 삽입하는 연산
treeNode *insertKey(treeNode *p, element key)
{
    treeNode *newNode;
    int compare;

    // 삽입할 자리에 새 노드를 구성하여 연결
    if (p == NULL)
    {
        newNode = (treeNode *)malloc(sizeof(treeNode));
        newNode->key = key;
        newNode->left = NULL;
        newNode->right = NULL;
        return newNode;
    }
    // 이진 트리에서 삽입할 자리 탐색
    else
    {
        compare = strcmp(key.accountNum, p->key.accountNum);
        if (compare < 0)
            p->left = insertKey(p->left, key);
        else if (compare > 0)
            p->right = insertKey(p->right, key);
        else
            printf("\n 이미 같은 번호로 등록된 계좌가 있습니다. \n");

        return p; // 삽입한 자리 반환
    }
}

void insert(treeNode **root, element key)
{
    *root = insertKey(*root, key);
}

// root 노드부터 탐색하여 key와 같은 노드를 찾아 삭제하는 연산
void deleteNode(treeNode *root, element key)
{
    treeNode *parent, *p, *succ, *succ_parent;
    treeNode *child;
    parent = NULL;
    p = root;
    while ((p != NULL) && (strcmp(p->key.accountNum, key.accountNum) != 0))
    {
        parent = p;
        if (strcmp(key.accountNum, p->key.accountNum) < 0)
            p = p->left;
        else
            p = p->right;
    }
    // 삭제할 노드가 없는 경우
    if (p == NULL)
    {
        printf("\n 삭제할 계좌가 등록되어 있지 않습니다. \n");
        return;
    }
    // 삭제할 노드가 단말 노드인 경우
    if ((p->left == NULL) && (p->right == NULL))
    {
        if (parent != NULL)
        {
            if (parent->left == p)
                parent->left = NULL;
            else
                parent->right = NULL;
        }
        else
            root = NULL;
    }
    // 삭제할 노드가 자식 노드를 한 개 가진 경우
    else if ((p->left == NULL) || (p->right == NULL))
    {
        if (p->left != NULL)
            child = p->left;
        else
            child = p->right;
        if (parent != NULL)
        {
            if (parent->left == p)
                parent->left = child;
            else
                parent->right = child;
        }
        else
            root = child;
    }
    // 삭제할 노드가 자식 노드를 두 개 가진 경우
    else
    {
        succ_parent = p;
        succ = p->right;
        while (succ->left != NULL)
        {
            succ_parent = succ;
            succ = succ->left;
        }
        if (succ_parent->left == succ)
            succ_parent->left = succ->right;
        else
            succ_parent->right = succ->right;
        p->key = succ->key;
        p = succ;
    }
    free(p);
}

// 이진 탐색 트리에서 키값이 key인 노드 위치를 탐색하는 연산
treeNode *searchBST(treeNode *root, element key)
{
    treeNode *p;
    int compare;
    p = root;
    while (p != NULL)
    {
        compare = strcmp(key.accountNum, p->key.accountNum);
        if (compare < 0)
            p = p->left;
        else if (compare > 0)
            p = p->right;
        else
        {
            printf("\n찾은 계좌 : %s", p->key.accountNum);
            return p;
        }
    }
    return p;
}

// 이진 탐색 트리를 중위 순회하면서 출력하는 연산
void displayInorder(treeNode *root)
{
    if (root)
    {
        displayInorder(root->left);
        printf("\n%s : %s", root->key.userName, root->key.accountNum);
        displayInorder(root->right);
    }
}

void menu()
{
    printf("\n*---------------------------*");
    printf("\n\t1 : 출력");
    printf("\n\t2 : 입력");
    printf("\n\t3 : 삭제");
    printf("\n\t4 : 검색");
    printf("\n\t5 : 송금");
    printf("\n\t6 : 거래내역조회");
    printf("\n\t7 : 종료");
    printf("\n*---------------------------*\n ");
}

void main()
{
    element e;
    treeNode *root = NULL, *temp = NULL, *temp2 = NULL;
    int tmp;
    char tmp2[10];
    int choice;
    do
    {
        menu();
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            printf("\t[계좌 출력]");
            displayInorder(root);
            printf("\n\t[계좌 끝]\n");
            break;
        case 2:
			if(e.userName == NULL)
				printf("\n NULL");
			else
				printf("\n No NULL");
            printf("\n[이름 입력] 이름을 입력하시오 : ");
            scanf("%s", e.userName);
            tmp = system("./prng");
            sprintf(e.accountNum, "%d", tmp);
            tmp = system("./prng");
            sprintf(tmp2, "%d", tmp);
            strcat(e.accountNum, tmp2);
            e.money = 500000;
            e.transCnt = 0;
            for(int i = 0; i<10; i++){
                e.transinfo[i] = (transInfo *)malloc(sizeof(transInfo));
            }
            insert(&root, e);
            break;
        case 3:
            printf("\n[계좌 삭제] 삭제할 계좌 : ");
            scanf("%s", e.accountNum);
            deleteNode(root, e);
            break;
        case 4:
            printf("\n[계좌 검색] 검색할 계좌 : ");
            scanf("%s", e.accountNum);
            temp = searchBST(root, e);
            if (temp != NULL)
            {
                printf("\n계좌 주인 : %s", temp->key.userName);
                printf("\n잔액 : %d", temp->key.money);
            }
            else
                printf("\n계좌가 없습니다.");
            break;
        case 5:
            printf("\n[계좌 입력] 송금할 계좌 입력 : ");
            scanf("%s", e.accountNum);
            temp = searchBST(root, e);
            printf("\n[계좌 입력] 본인 계좌 입력 : ");
            scanf("%s", e.accountNum);
            temp2 = searchBST(root, e);
            if (temp != NULL)
            {
                printf("\n예금주 명 : %s", temp->key.userName);
                printf("\n송금 금액 : ");
                scanf("%d", &e.money);
                if(e.money > temp2->key.money){
                    printf("\n잔액이 부족합니다!");
                    break;
                }
                printf("\n받는 분에게 표기 : ");
                scanf("%s", tmp2);

                temp->key.money = temp->key.money + e.money;
                strcpy(temp->key.transinfo[temp->key.transCnt]->transName, tmp2);
                temp->key.transinfo[temp->key.transCnt]->money = e.money;
                temp->key.transCnt++;

                temp2->key.money = temp2->key.money - e.money;
                strcpy(temp2->key.transinfo[temp2->key.transCnt]->transName, temp->key.userName);
                temp2->key.transinfo[temp2->key.transCnt]->money = -e.money;
                temp2->key.transCnt++;
            }
            else
                printf("\n계좌가 없습니다.");
            break;
        case 6:
            printf("\n[계좌 입력] 거래 내역을 조회할 계좌를 입력하세요 : ");
            scanf("%s", e.accountNum);
            temp = searchBST(root, e);
            printf("\n/**********거래내역조회**********/\n");
            for(int i = 0; i<10; i++){
                printf("\n%d : 거래금액  %d, 거래자 명  %s\n", i+1, temp->key.transinfo[i]->money, temp->key.transinfo[i]->transName);
            }
            break;
        }
        while (getchar() != '\n')
            ;
    } while (choice != 7);
}
