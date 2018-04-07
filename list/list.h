
#ifndef __LIST_H__
#define __LIST_H__

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define   TRUE		(1)
#define	  FALSE		(0)

/* the functions status */
#define   OK		(0)
#define   ERROR     (-1)

typedef int ElemType;   /* define data type for the element */

typedef struct ListNode
{
	ElemType            m_eVal;
	struct ListNode*	m_pNext;
}ListNode;


int insert(ListNode** ppHead, ElemType eVal, int location );

int addToTail( ListNode** ppHead, ElemType eVal );



#endif






