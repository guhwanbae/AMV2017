/*
 * queueADT.c
 *
 *  Created on: 2017. 6. 27.
 *      Author: ghbae
 */

/*******************************************************************************
* Include
*******************************************************************************/
#include "queueADT.h"
#include <stdio.h>
#include <stdlib.h>

/*******************************************************************************
* Constant
*******************************************************************************/


/*******************************************************************************
* Define & MACRO
*******************************************************************************/


/*******************************************************************************
* Type Definition
*******************************************************************************/


/*******************************************************************************
* Exported Global Variables
*******************************************************************************/

/*******************************************************************************
* Memeber Structures
*******************************************************************************/

/*******************************************************************************
* Memeber Functions
*******************************************************************************/
void queueInit(queue* pq, int newSize) {
	pq->front	= 0;
	pq->rear	= 0;
	pq->size	= newSize;
	int index;
	for(index=0;index<pq->size;++index) {
		pq->Q[index].row = 0;
		pq->Q[index].col = 0;
		pq->Q[index].dir = 0;
	}
}

bool queueIsEmpty(queue* pq) {
	if(pq->front == pq->rear) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

bool queueIsFull(queue* pq) {
	if(((pq->rear + 1) % QUEUE_SIZE) == pq->front) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

bool enQueue(queue* pq, int col, int row, edge dir) {
	if(queueIsFull(pq) == TRUE) {
		return FALSE;
	}
	else {
		pq->rear = (pq->rear + 1) % QUEUE_SIZE;
		/*	update cur data	*/
		pq->Q[pq->rear].col = col;
		pq->Q[pq->rear].row = row;
		pq->Q[pq->rear].dir = dir;
		/*	end update	*/
		return TRUE;
	}
}

keyType* deQueue(queue* pq) {
	if(queueIsEmpty(pq) == TRUE) {
		/*	If queue is empty, return null pointer!	*/
		return NULL;
	}
	pq->front = (pq->front + 1) % QUEUE_SIZE;
	return &(pq->Q[pq->front]);
}
