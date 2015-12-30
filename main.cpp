// BTree.cpp : �������̨Ӧ�ó������ڵ㡣

#include<stdio.h>
#include<stdlib.h>
#include"Btree.h"

int main()
{
	BTree BT = NULL;
	Result result;
	int min,max, select, input;
	//KeyType key[18] = { 35, 16, 18, 70, 5, 50, 22, 60, 13, 17, 12, 45, 25, 42, 15, 90, 30, 7 };
	do{
		printf("��ʼ����B��\n");
		printf("����ؼ��ֵĸ���������������\n");
		scanf("%d", &input);
		do{
			printf("����ؼ�����ֵ�����䣨��ʽΪ x,x��\n");
			scanf("%d,%d", &min, &max);
			if (min > max) printf("��ʽ���ԣ����������룡\n");
		} while (min > max);
		CreatBTree(BT, input, min, max);
		printf("�����ɹ�!");			
		system("CLS");
		do{
			PreOrderBTree(BT);
			if (BT == NULL) break;
			printf("���� 1: ����ؼ���\n");
			printf("���� 2: ɾ���ؼ���\n");
			printf("���� 3: ����B��\n");
			printf("���� 0: �˳�ϵͳ\n");
			scanf("%d", &select);
			switch (select)
			{
			case 1:
				do{
					printf("�����������Ĺؼ��֣����֣�\n");
					scanf("%d", &input);
					SearchBTree(BT, input, result);
					if (result.tag)
						printf("�ùؼ����Ѵ��ڣ����������룡\n");
					else{
						InsertBTree(BT, input, result.pt, result.index);
						printf("����ɹ�!���س������ز˵�");
						getchar();
					}
				} while (result.tag);
				getchar();
				system("CLS");
				break;
			case 2:
				do{
					printf("��������ɾ���Ĺؼ��֣����֣�\n");
					scanf("%d", &input);
					SearchBTree(BT, input, result);
					if (NULL == result.pt){
						printf("��ǰֵΪ�գ�����ɾ��!\n");
						printf("���س������ز˵�!");
						getchar();
						getchar();
						break;
					}
					else if (!result.tag)
						printf("�ùؼ��ֲ����ڣ����������룡");
					else{
						DeleteBTree(BT, result.pt, result.index);
						printf("ɾ���ɹ������س������ز˵���");
						getchar();
					}
				} while (!result.tag);
				getchar();
				system("CLS");
				break;
			case 3:
				printf("�Ƿ����ȷ������B����\n");
				printf("ȷ��������1�������������ַ��ز˵���");
				scanf("%d", &input);
				if (input == 1)
					DestroyBTree(BT);
				system("CLS");
				break;
			case 0:
				exit(1);
				break;
			default:
				printf("������Ч������������\n");
				system("CLS");
				break;
			}
		} while (1);
	} while (1);
    return 0;
}  

