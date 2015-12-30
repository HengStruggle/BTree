#ifndef BTREE_H
#define BTREE_H
/* B���Ķ��壺
*
*һ��m��B������Ϊ��������Ϊ�����������Ե�m������
*��1������ÿ�������ຬ��m������
*��2��������㲻��Ҷ�ӽ�㣬��������2������
*��3���������֮������з��ն˽��������m/2(��ȡ��)������
*��4��ÿ�����ն˽���а�����Ϣ����n,A0,K1,A1,K1,A2,...,Kn,An��
*     ���У�1��Ki(1<=i<=n)Ϊ�ؼ��֣��ҹؼ��ְ���������
*           2��ָ��Ai(0<=i<=n)ָ�������ĸ���㣬Ai-1ָ��������
*              ���н��Ĺؼ��־�С��Ki,�Ҵ���Ki-1;
*           3) �ؼ��ֵĸ���n�������㣺m/2(��ȡ��)-1<=n<=m-1
* (5) ����Ҷ�ӽ�㶼������ͬһ�㣬Ҷ�ӽ�㲻�����κ���Ϣ������
*     �������ⲿ�������ʧ�ܵĽ�㣬ʵ������Щ��㲻���ڣ�ָ��
*     ��Щ����ָ��Ϊ�ա���
* ʵ���ϣ���B����ÿ������ϻ�Ӧ����n��ָ��ÿ���ؼ�����Ӧ��¼��ָ�롣
*/
#define m 5              // B���Ľף��ݴ�Ϊ3
#define KeyType int    // ����ؼ�������
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

typedef struct BTNode{   // B��������Ͷ���
	int keynum;             // ��㵱ǰӵ�еĹؼ��ֵĸ���
	KeyType key[m + 1];       // �ؼ��֣�key[0]δ��
	struct BTNode *parent;  // ˫�׽��ָ��
	struct BTNode *ptr[m + 1];// ���ӽ��ָ������,ptr[m]δ��
	Record *recptr[m + 1];   // ��¼ָ��������0�ŵ�Ԫδ�ã�һ����Ϊ���̵�ַ
}BTNode, *BTree;

typedef struct {
	BTree pt;      //ָ���ҵ��Ľ��
	int index;         // 1<=i<=m,�ڽ���еĹؼ���λ��
	int tag;       // 1�� ���ҳɹ���0������ʧ��
} Result;       // B���Ĳ��ҽ������

typedef struct BTQNode{
	BTree data;
	struct BTQNode *next;
} BTQueueNode,*BTQueuePtr;
typedef struct {
	BTQueuePtr front;
	BTQueuePtr rear;
} BTQueue;

//B���Ĵ���
void CreatBTree(BTree &BT, int n,int min,int max);
//B���Ĳ���
void SearchBTree(BTree BT, KeyType target, Result &result);//��m��B���ϲ��ҹؼ���target,���أ�pt,i,tag��
int Search(BTree temp, KeyType target);
//B���Ĳ���
void InsertBTree(BTree &BT, KeyType target, BTree pt, int index);
Status spilt(BTree &q, int mid, BTree &ap);
Status newRoot(BTree &t, BTree &p, KeyType key, BTree &ap);
void Insert(BTree &q, int index, KeyType key, BTree &ap);
//B����ɾ��
Status Delete(BTree &BT, KeyType target);
void DeleteBTree(BTree &BT,BTree pt, int index);
void Successor(BTree &pt, int index);
void Remove(BTree pt, int index);
void Restore(BTree &BT,BTree &pt);
void MergeIntoLeft(BTree &tp, BTree &pt, int pos);
void MergeIntoRight(BTree &tp, BTree &pt, int pos);
void DestroyBTree(BTree &BT);
//B���ı���
void LevelOrderBTree(BTree BT);
void PreOrderBTree(BTree BT);
Status visitBTNode(BTree BT,int level);

//���в���
Status InitQueue(BTQueue &Q); //����һ����������
Status DeQueue(BTQueue &Q, BTNode &e);  //���Ӳ���
Status EnQueue(BTQueue &Q, BTNode e);//��Ӳ���
void DestroyQueue(BTQueue &Q);//���ٶ���Q,Q���ٴ���
Status QueueEmpty(BTQueue Q);

#endif BTREE_H 
