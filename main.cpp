// BTree.cpp : 定义控制台应用程序的入口点。

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
		printf("开始创建B树\n");
		printf("输入关键字的个数（正整数！）\n");
		scanf("%d", &input);
		do{
			printf("输入关键字数值的区间（格式为 x,x）\n");
			scanf("%d,%d", &min, &max);
			if (min > max) printf("格式不对，请重新输入！\n");
		} while (min > max);
		CreatBTree(BT, input, min, max);
		printf("创建成功!");			
		system("CLS");
		do{
			PreOrderBTree(BT);
			if (BT == NULL) break;
			printf("输入 1: 插入关键字\n");
			printf("输入 2: 删除关键字\n");
			printf("输入 3: 销毁B树\n");
			printf("输入 0: 退出系统\n");
			scanf("%d", &select);
			switch (select)
			{
			case 1:
				do{
					printf("输入你想插入的关键字（数字）\n");
					scanf("%d", &input);
					SearchBTree(BT, input, result);
					if (result.tag)
						printf("该关键字已存在，请重新输入！\n");
					else{
						InsertBTree(BT, input, result.pt, result.index);
						printf("插入成功!按回车键返回菜单");
						getchar();
					}
				} while (result.tag);
				getchar();
				system("CLS");
				break;
			case 2:
				do{
					printf("输入你想删除的关键字（数字）\n");
					scanf("%d", &input);
					SearchBTree(BT, input, result);
					if (NULL == result.pt){
						printf("当前值为空，不能删除!\n");
						printf("按回车键返回菜单!");
						getchar();
						getchar();
						break;
					}
					else if (!result.tag)
						printf("该关键字不存在，请重新输入！");
					else{
						DeleteBTree(BT, result.pt, result.index);
						printf("删除成功！按回车键返回菜单！");
						getchar();
					}
				} while (!result.tag);
				getchar();
				system("CLS");
				break;
			case 3:
				printf("是否真的确定销毁B树？\n");
				printf("确定请输入1，输入其他数字返回菜单！");
				scanf("%d", &input);
				if (input == 1)
					DestroyBTree(BT);
				system("CLS");
				break;
			case 0:
				exit(1);
				break;
			default:
				printf("输入无效，请重新输入\n");
				system("CLS");
				break;
			}
		} while (1);
	} while (1);
    return 0;
}  

