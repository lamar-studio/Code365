
#include "list.h"





int main()
{
    ListNode* list = NULL;
    ElemType  elem = 10;
    int       i    = 0;

    for( i = 0; i < 5; i++)
    {
        addToTail(&list, elem);
        elem += 5;
    }

    for (i = 0; i < 5; i++ )
    {
        printf("the list[%d] val:%d \n", i, list->m_eVal);
        list = list->m_pNext;
    }

    return OK;
}






