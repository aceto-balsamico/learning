#include "common.h"

LIST* Create_NewLIST()
{
	LIST* ptr_new = (LIST*)malloc(sizeof(LIST));

	ptr_new->val1 = -1;
	ptr_new->val2 = -1;
	ptr_new->val3 = -1;

	ptr_new->top  = NULL;
	ptr_new->next = NULL;
	ptr_new->prev = NULL;

	return ptr_new;
}
LIST* Search_TopLIST(LIST* ptr_target)
{
	LIST* ptr_tmp = ptr_target;
	while(ptr_tmp->prev != NULL)
	{
		ptr_tmp = ptr_tmp->prev;
	}
	return ptr_tmp;
}
LIST* Search_EndLIST(LIST* ptr_target)
{
	LIST* ptr_tmp = ptr_target;
	while(ptr_tmp->next != NULL)
	{
		ptr_tmp = ptr_tmp->next;
	}
	return ptr_tmp;
}
void Free_AllLIST(LIST* ptr_target)
{
	LIST* ptr_NextCheck = Search_TopLIST(ptr_target);
	LIST* ptr_Delete	= NULL;
	
	while(ptr_NextCheck != NULL)
	{
		ptr_Delete		= ptr_NextCheck;
		ptr_NextCheck	= ptr_NextCheck->next;
		if(ptr_Delete != NULL)
		{
			free(ptr_Delete);
			ptr_Delete = NULL;
		}
	}
}
void Add_NodeLIST(LIST* ptr_target, int val1, int val2, int val3)
{
	if(ptr_target->top == NULL)
	{
		ptr_target->val1 = val1;
		ptr_target->val2 = val2;
		ptr_target->val3 = val3;

		ptr_target->top = ptr_target;
	}
	else
	{
		LIST* ptr_new = (LIST*)malloc(sizeof(LIST));
		ptr_new->val1 = val1;
		ptr_new->val2 = val2;
		ptr_new->val3 = val3;
	
		ptr_new->top = Search_TopLIST(ptr_target);
		ptr_new->prev = ptr_target;
		ptr_new->next = NULL;
		ptr_target->next = ptr_new;
	}
}
void Access_from0_to1000()
{
	//リスト構造を利用して100までの数字で1から1000を表示する
	LIST* ChainResult = Create_NewLIST();
	for(int vartical = 0; vartical < 100; vartical++)
	{
		for(int horizontal = 1; horizontal <= 10; horizontal++)
		{
			Add_NodeLIST(ChainResult, vartical, horizontal, 0);
			ChainResult = Search_EndLIST(ChainResult);
		}
	}

	// for(LIST* Judge = ChainResult->top; Judge != NULL; Judge = Judge->next)
	// {
	// 	if(Judge->val1 < 3 || Judge->val1 > 96) 
	// 	{
	// 		printf(" %4d,", Judge->val1 * 10 + Judge->val2);
	// 		if(Judge->val2 == 10) printf("\n");
	// 	}
	// }
	Free_AllLIST(ChainResult);	
}

int main()
{
	begin_time();

	//リスト構造を利用して100までの数字で1から1000を表示する
	LIST* ChainResult = Create_NewLIST();
	for(int vartical = 0; vartical < 100; vartical++)
	{
		for(int horizontal = 1; horizontal <= 10; horizontal++)
		{
			Add_NodeLIST(ChainResult, vartical, horizontal, 0);
			ChainResult = Search_EndLIST(ChainResult);
		}
	}

	for(LIST* Judge = ChainResult->top; Judge != NULL; Judge = Judge->next)
	{
		if(Judge->val1 < 3 || Judge->val1 > 96) 
		{
			printf(" %4d,", Judge->val1 * 10 + Judge->val2);
			if(Judge->val2 == 10) printf("\n");
		}
	}
	Free_AllLIST(ChainResult);	//Create_NewLISTした階層で行う

	end_time();
}