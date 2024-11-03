/*
 * List.h
 *
 *  Created on: 2023Äê11ÔÂ17ÈÕ
 *      Author: howlo
 */

#ifndef LIST_H
#define LIST_H
extern char Direction_X[4];
extern char Direction_Y[4];
extern unsigned short VisitFlag[60][94];

typedef struct Node
{
    int x;
    int y;
    struct Node* next;
}TypeNode, * ptrNode;

typedef struct ListHead
{

    ptrNode Head;
    ptrNode Tie;
    int Length;

}TypeListHead, * ptrHead;
ptrHead ListCreate();

void AddNode(ptrHead list, ptrNode node);

ptrNode CreatNode(int x,int y);

//void BFS(int x, int y, int (*rule)(int,int));

#endif /* CODE_LIST_H_ */
