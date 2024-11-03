/*
 * List.c
 *
 *  Created on: 2023年11月17日
 *      Author: howlo
 */
#include"List.h"
#include<stdlib.h>
 TypeNode Mem_Poll[500];//内存池
 TypeListHead head;
static int NofPoll=0;//下一个内存地址
ptrHead ListCreate(){
    ptrHead p = &head;
    p->Head = NULL;
    p->Tie = NULL;
    p->Length = 0;
    return p;

}


ptrNode CreatNode(int x, int y)
{
    ptrNode p = Mem_Poll+NofPoll++;
    p->next = 0;
    p->x = x;
    p->y = y;
    if(NofPoll>=500) NofPoll=0;
    return p;
}

void AddNode(ptrHead list, ptrNode node)
{

    if (list->Length == 0)
    {
        list->Length++;
        list->Head = node;
        list->Tie = node;


    }
    else
    {

        list->Length++;
        (list->Tie)->next = node;
        list->Tie = node;

    }


}

void ListPop(ptrHead list)
{

    ptrNode p;
    if (list->Length > 1)
    {
        list->Length--;
        p = list->Head;
        list->Head = p->next;
        //free(p);
    }
    else if (list->Length == 1)
    {

        list->Length--;
        //free(list->Head);
        list->Head = list->Tie = 0;
        NofPoll=0;
    }


}



void ClearList(ptrHead head)
{
    while (head->Length > 0)
        ListPop(head);

}



