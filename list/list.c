/***********************************************
 * file       : list.c
 * description: the implement to LinkList
 * author	  : lamar
 * date		  : 2018/04/07
 *************/

#include"list.h"


int insert( ListNode **ppHead, ElemType eVal, int location )
{
	int i;
	ListNode *p, *buf;

	i = 1;
	p = *ppHead;

	/* 查找要插入的位置 */
	while( p && (i < location) )
	{
		p = p->m_pNext;
		i++;
	}

	/* 元素不存在 */
	if( !p || (i > location) )
	{
		printf("the element is inexistence \n");

		return ERROR;
	}

	/* 插入操作 */
	buf = (ListNode*)malloc( sizeof(ListNode) );

	buf->m_eVal     = eVal;
	buf->m_pNext    = p->m_pNext;
	p->m_pNext		= buf;

	return OK;
}

int addToTail( ListNode **ppHead, ElemType eVal )
{
    ListNode *new = (ListNode*)malloc( sizeof(ListNode) );
    new->m_eVal  = eVal;
    new->m_pNext = NULL;

    if ( *ppHead == NULL )
    {
        *ppHead = new;
		printf("it's the head of list \n");

        return OK;
    }else
    {
        ListNode* buf =  *ppHead;
 
        while( buf->m_pNext != NULL )
            buf = buf->m_pNext;

        buf->m_pNext = new;
    }

    return OK;
}















