#ifndef BTREE_H
#define BTREE_H
/* B树的定义：
*
*一棵m阶B树，或为空树，或为满足以下特性的m叉树：
*（1）树中每个结点最多含有m棵子树
*（2）若根结点不是叶子结点，则至少有2个子树
*（3）除根结点之外的所有非终端结点至少有m/2(上取整)棵子树
*（4）每个非终端结点中包含信息：（n,A0,K1,A1,K1,A2,...,Kn,An）
*     其中：1）Ki(1<=i<=n)为关键字，且关键字按升序排序
*           2）指针Ai(0<=i<=n)指向子树的根结点，Ai-1指向子树中
*              所有结点的关键字均小于Ki,且大于Ki-1;
*           3) 关键字的个数n必须满足：m/2(上取整)-1<=n<=m-1
* (5) 所有叶子结点都出现在同一层，叶子结点不包含任何信息（可以
*     看做是外部结点或查找失败的结点，实际上这些结点不存在，指向
*     这些结点的指针为空。）
* 实际上，在B树的每个结点上还应包括n个指向每个关键字相应记录的指针。
*/
#define m 5              // B树的阶，暂存为3
#define KeyType int    // 定义关键字类型
typedef enum{
	TRUE = 1,
	OK = 1,
	ERROR = 0,
	FALSE =0,
	OVREFLOW =0
}Status;

typedef struct BorrowInfo{
	char id[20];
	int borrowDate;
	int returnDate;
	struct BorrowInfo *next;
}BorrowInfo, *Borrower;

typedef struct{
	char bookTitle[20];
	char writer[20];
	int	stock;
	int   amount;
	Borrower borrower;
}Record;

typedef struct BTNode{   // B树结点类型定义
	int keynum;             // 结点当前拥有的关键字的个数
	KeyType key[m + 1];       // 关键字，key[0]未用
	struct BTNode *parent;  // 双亲结点指针
	struct BTNode *ptr[m + 1];// 孩子结点指针数组,ptr[m]未用
	Record *recptr[m + 1];   // 记录指针向量，0号单元未用，一般作为磁盘地址
}BTNode, *BTree;

typedef struct {
	BTree pt;      //指向找到的结点
	int index;         // 1<=i<=m,在结点中的关键字位序
	int tag;       // 1： 查找成功，0：查找失败
} Result;       // B树的查找结果类型

typedef struct BTQNode{
	BTree data;
	struct BTQNode *next;
} BTQueueNode,*BTQueuePtr;
typedef struct {
	BTQueuePtr front;
	BTQueuePtr rear;
} BTQueue;

//B树的创建
void CreatBTree(BTree &BT, int n,int min,int max);
//B树的查找
void SearchBTree(BTree BT, KeyType target, Result &result);//在m阶B树上查找关键码target,返回（pt,i,tag）
int Search(BTree temp, KeyType target);
//B树的插入
void InsertBTree(BTree &BT, KeyType target, BTree pt, int index);
Status spilt(BTree &q, int mid, BTree &ap);
Status newRoot(BTree &t, BTree &p, KeyType key, BTree &ap);
void Insert(BTree &q, int index, KeyType key, BTree &ap);
//B树的删除
Status Delete(BTree &BT, KeyType target);
void DeleteBTree(BTree &BT,BTree pt, int index);
void Successor(BTree &pt, int index);
void Remove(BTree pt, int index);
void Restore(BTree &BT,BTree &pt);
void MergeIntoLeft(BTree &tp, BTree &pt, int pos);
void MergeIntoRight(BTree &tp, BTree &pt, int pos);
void DestroyBTree(BTree &BT);
//B树的遍历
void LevelOrderBTree(BTree BT);
void PreOrderBTree(BTree BT);
Status visitBTNode(BTree BT,int level);

//队列操作
Status InitQueue(BTQueue &Q); //构造一个空链队列
Status DeQueue(BTQueue &Q, BTNode &e);  //出队操作
Status EnQueue(BTQueue &Q, BTNode e);//入队操作
void DestroyQueue(BTQueue &Q);//销毁队列Q,Q不再存在
Status QueueEmpty(BTQueue Q);

#endif BTREE_H 
