#include <stdio.h> // Don't think I need this
#include <stdlib.h>
#include <stdint.h>
#include "Header.h"

void clearHeap(struct heapMemoryNode* heapBank);

struct heapMemoryNode* startBank()
{
    struct heapMemoryNode* tmpNode = malloc(sizeof(struct heapMemoryNode));
    if(tmpNode == NULL)
    {
        perror("Node not allocated memory\n");
        //exit(1);
        return NULL;
    }
    tmpNode->contents = malloc(64*sizeof(char));
    if (tmpNode->contents == NULL)
    {
        perror("Memory not allocated for heap bank node contents\n");
        free(tmpNode);
        return NULL;
        //exit(1);
    }
    tmpNode->linked = 0;
    tmpNode->next = NULL;
    tmpNode->index = 0;
    tmpNode->allocated = 0;
    return tmpNode;
}

struct heapMemoryNode * getNextBank(struct heapMemoryNode * n)
{
    return n->next;
}

unsigned char addBanks(struct heapMemoryNode** head, unsigned char pos)
{
    struct heapMemoryNode * nodeToAdd = malloc(sizeof(struct heapMemoryNode));
    if(nodeToAdd == NULL){
        clearHeap(*head);
        return 1;
        //exit(1);
    }
    nodeToAdd->contents = calloc(64, sizeof(char));
    nodeToAdd->next = NULL;
    nodeToAdd->linked = 0;
    nodeToAdd->index = pos;
    nodeToAdd->allocated = 0;
    if(*head == NULL)
    {
        *head = nodeToAdd;
    }
    else
    {
        struct heapMemoryNode * cursor = *head;
        while (getNextBank(cursor) != NULL)
        {
            cursor = getNextBank(cursor);
        } 
        cursor->next = nodeToAdd;
    }
    return 0;
}

void clearNode(struct heapMemoryNode * n)
{
    free(n->contents);
}

void clearHeap(struct heapMemoryNode* heapBank)
{
    struct heapMemoryNode * cursor;
    while(heapBank != NULL)
    {
        cursor = heapBank;
        heapBank = getNextBank(heapBank);
        clearNode(cursor);
        free(cursor);
    }
}

/*
malloc(uint32_t size);
Must find a contiguous block of memory of size long. 
*/

struct heapMemoryNode * toNextBank(struct heapMemoryNode * heapHead)
{
    heapHead = heapHead->next;
    return heapHead;
}

uint32_t malloc_OP(uint32_t size, struct heapMemoryNode* heapHead)
{
    struct heapMemoryNode * toRet = NULL;
    // Size too large
    if(size > 8192)
    {
        // assign 0 to the 20th register
        return 0;
    }
    uint32_t length = 0;

    /*
    Start at head
    * Assess blocks linked 
    ? If linked != 0: move to the head + offset block and run again 
    */
    struct heapMemoryNode * start = heapHead;
    struct heapMemoryNode * cursor = heapHead; 
    uint32_t startLink = 0;
    while(length < size)
    {
        if(cursor == NULL || start == NULL)
        {
            return 0;
        }
        if(start->linked != 0)
        {
            //printf("Here1\n");
            startLink = 0;
            while(start->linked != 0)
            {
                uint32_t saved = start->linked; // ! Check this
                //printf("Saved: %u\n", saved);
                for (size_t i = 0; i < saved; i++) // ! FIX THIS ! //
                {
                    //printf("Jumps forward %zu", i);
                    start = start->next;
                    cursor = start;
                }
                if(start == NULL)
                {
                    return 0;
                }
            }
            
        }
        length += 64; 
        if(length < size && cursor->linked != 0)
        {
            start = cursor->next;
            cursor = start; 
            startLink = 0;
            continue;
        }
        cursor = cursor->next;
        startLink ++;
    }
    toRet = start;
    start->linked = startLink;
    while (start != cursor)
    {
        start->allocated = 1;
        start = start->next;
    }
    
    //printf("ToRet Index: %d\n", toRet->index);
    //printf("Number of banks linked: %d\n", toRet->linked);
    return ((toRet->index)*64) + 0xb700;
}

/*
Free is given an address and you must find the node associated with it
Address will correspond to one of the nodes in the heap bank LL
*/
int32_t free_OP(struct heapMemoryNode * toFree)
{
    if(toFree == NULL || toFree->linked == 0)
    {
        // Illegal operation
        return 1;
    }
    uint32_t numLinked = toFree->linked;
    struct heapMemoryNode * cursor = toFree;
    for (size_t i = 0; i < numLinked; i++)
    {
        free(cursor->contents);
        cursor->contents = calloc(64, sizeof(char));
        cursor->linked = 0;
        cursor->allocated = 0;
    }
    toFree->linked = 0;  
    return 0; 
}