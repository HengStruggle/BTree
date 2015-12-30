#include"Btree.h"
#include<stdio.h>
#include<stdlib.h>

//****B���Ĵ���///
void CreatBTree(BTree &BT, int n,int min,int max){
	KeyType k;
	Result result;
	int i;
	for (i = 0; i <n; i++){//�����û�����Ĺؼ��ָ���n�������
		k =(unsigned)rand() % (max-min+1) + min;//�������min - max֮�������
		SearchBTree(BT, k, result);
		if (!result.tag)  InsertBTree(BT, k, result.pt, result.index);//����ؼ��ֽ�㣬������ͬ�ؼ����򲻲���
	}
}
//****B���Ĵ���

//****B���Ĳ���
void SearchBTree(BTree BT, KeyType target, Result &result){
	//��m��B���ϲ��ҹؼ���target,���أ�pt,i,tag��
	//�����ҳɹ���������ֵtag = 1,ָ��pt��ָ����е� i ���ؼ������target������
	//����ʧ�ܣ�����ֵtag=0������target�Ĺؼ����¼Ӧ����ָ��pt��ָ����е�i���͵�i+1���ؼ����
	int i = 0, found = 0;//i���ڼ�¼��Ӧ����йؼ����λ�ã�found��ʶ�Ƿ��ҵ��ؼ���
	BTree temp = BT,tmp=NULL;//��ʼ��tempָ������㣬tmpָ��temp��˫��
	while (temp != NULL && 0 == found){//��������ǿ���δ�ҵ�����������
		i = Search(temp,target);
		if (i <= temp->keynum && temp->key[i] == target)
			found = 1;//�ҵ�����ؼ���
		else{
			tmp = temp;//tmp�洢temp
			temp = temp->ptr[i - 1];//����ؼ��ִ���temp����е�i���ؼ��֣�����temp�ĵ�i-1�����ӽ���в��ҡ�
		}
	}
	if (1 == found){//���ҳɹ�
		result.pt=temp,result.index = i,result.tag=1;
	}
	else{//���Ҳ��ɹ������ؿɲ���λ�� i
		result.pt = tmp, result.index = i, result.tag = 0;
	}
}

int Search(BTree temp, KeyType target){
	int i = 1;
	while (i <= temp->keynum && target > temp->key[i])
		i++;//�����ؼ��ָ�������Ҫ���Ҵ���ؼ��ִ��ڹؼ���ʱ����������
	return i;
}
//***B���Ĳ���


//***B���Ĳ���
void InsertBTree(BTree &BT, KeyType target, BTree pt, int index){
	//���ò��Һ������ҹؼ���target�Ĳ���λ�á����ҵ�target,��ùؼ��ִ��ڣ�������ֱ�ӷ��ء�
	//������Ҳ�����ʧ����ĳ����ײ�ķ��ն˽�㼴���pt�ϣ���B��BT�Ͻ��*pt��key[index]��key[index+1]֮�����ؼ���target.
	//���������������˫�������б�Ҫ�Ľ����ѵ�����ʹT����m�׵�B����
	//���У�pt,index�ɲ��Һ���SearchBTree���ص�result����
	int  mid;//�洢�м�λ��
	KeyType key;//�洢����Ĺؼ���
	BTree ap = NULL ;//�洢�½��
	int finished = 0, needNewRoot = 0;//��ʶ������ʶ�Ƿ������ϻ����Ƿ���Ҫ�¸�
	if (NULL == pt) {//������ʱ��BTΪ��������Ҫ�����¸��洢����Ĺؼ���
		if (OVREFLOW == newRoot(BT, pt, target, ap)){
			printf("�ڴ���䲻��");
			exit(1);
		}
	 }
	else{
		key = target;//�洢��Ҫ����Ĺؼ���
		while (0 == needNewRoot && 0 == finished){//����ɲ��������Ҫ�¸�ʱ��ֹͣ�������
			Insert(pt, index, key, ap);//���*pt��key[index]��key[index+1]֮�����ؼ��֣�����ap���뵽pt->ptr[index+1]
			if (pt->keynum < m) finished = 1;//��������ؼ�����ĿҪ����������
			else{//������Ҫ���ѽ��pt
				mid = (m + 1) / 2;//�洢�м�λ��
				if (OVREFLOW == spilt(pt, mid, ap)){//���ѽ��pt
					printf("�ڴ���䲻��");
					exit(1);
				}
				key = pt->key[mid];//�洢�м�λ�õĹؼ���
				if (pt->parent != NULL){//�����Ǹ�������
					pt = pt->parent;//���м�λ�õĹؼ�����ͬ�½��Ĵ洢λ�ò��뵽������С�
					index = Search(pt, key);
				}
				else needNewRoot = 1;//������������Ҫ�����¸����
			}
		}
		if (1 == needNewRoot){
			if (OVREFLOW == newRoot(BT, pt, key, ap)){//�����¸����BT
				printf("�ڴ���䲻��");
				exit(1);
			}
		}
	}
}

Status newRoot(BTree &t, BTree &p, KeyType key, BTree &ap){
	//������ؼ���ʱ����ǰΪ�������������������ʱ����Ҫ�µĸ���㣬�õ��÷�����
	//�����µĸ����t,����p,apΪt�������������������ѵõ��Ĺؼ�����Ϊ�����Ĺؼ���
	int i;
	t = (BTNode *)malloc(sizeof(BTNode));
	if (NULL == t) return OVREFLOW;

	t->keynum = 1;
	t->key[1] = key;
	for (i = 0; i <= m; i++)
		t->ptr[i] = NULL;
	t->ptr[0] = p; t->ptr[1] = ap;
	if (p != NULL) p->parent = t;
	if (ap != NULL)ap->parent = t;
	t->parent = NULL;
	return OK;
}

Status spilt(BTree &q, int mid, BTree &ap){
	//����ؼ��ֺ���������Ŀ����ʱ���õ��÷���
	//���ѵķ���������һ���½��ap�����м�λ�ðѽ��q���������м�λ�õĹؼ��֣��ֳ������֡�
	//ǰ�벿�����ھɽ�㣬��벿�ָ��Ƶ��½��ap�У��м�λ�õĹؼ�����ͬ�½��Ĵ洢λ�ò��뵽������С�
	//�������󸸽��Ĺؼ��ָ���Ҳ����m-1,��������Ϸ��ѡ���������������ѣ�������µĸ����
	int i, j, n = q->keynum;
	ap = (BTNode *)malloc(sizeof(BTNode));//�½��½��ap
	if (NULL == ap) return OVREFLOW;
	for (i = 0; i <= m; i++)
		ap->ptr[i] = NULL;
	ap->ptr[0] = q->ptr[mid];//�м�λ��ָ���������Ϊ�½��ap�ĵ�һ������
	for (i = mid + 1, j = 1; i <= n; i++, j++){//q����һ�븴�Ƶ�ap
		ap->key[j] = q->key[i];
		ap->ptr[j] = q->ptr[i];
		q->ptr[i] = NULL;
	}
	ap->keynum = n - mid;//�½��ؼ��ָ���Ϊn-mid
	ap->parent = q->parent;//�ɽ����½��ָ��ͬһ��˫�׽��
	for (i = 0; i <= n - mid; i++){
		if (ap->ptr[i])
			ap->ptr[i]->parent = ap;//����ap���ӽ�㼴���ƺ�ԭ���ɽ���һ����ĸ����
	}
	q->ptr[mid] = NULL;
	q->keynum = mid - 1;//���¾ɽ��Ĺؼ��ָ���
	return OK;
}

void Insert(BTree &q, int index, KeyType key, BTree &ap){
	//������ؼ��ֻ��߷��Ѻ��м�λ�ùؼ��ּ��븸���ʱ���õ��÷�����
	//key���뵽q->key[index+1],ap���뵽q->ptr[index+1]
	int j, n = q->keynum;//n�洢q��ǰ�ؼ��ָ���
	for (j = n; j >= index; j--){//����Ҫ���м���룬���key��Ĺؼ�����Ҫ������һλ��ͬʱ���ڵĺ��ӽ��ָ��Ҳ���ƶ�
		q->key[j + 1] = q->key[j];
		q->ptr[j + 1] = q->ptr[j];
	}
	q->key[index] = key;//�ڶ�Ӧλ�ò���ؼ���key
	q->ptr[index] = ap;//�ڶ�Ӧλ�ò��뺢�ӽ��ָ�롣���ؼ��ָղ��룬���ӽ��ָ��ΪNULL�����Ƿ��Ѻ���룬���ӽ��ָ��Ϊ�½��Ĵ洢λ��
	if (ap != NULL)
		ap->parent = q;//���º��ӽ��ָ��ĸ����
	q->keynum++;//�����q�Ĺؼ�����Ŀ��һ��
}
//****B���Ĳ���


//****B����ɾ��
Status Delete(BTree &BT, KeyType target){
	Result result;
	SearchBTree(BT, target, result);
	if (!result.tag) return FALSE;
	DeleteBTree(BT, result.pt, result.index);
	return OK;
}

void DeleteBTree(BTree &BT,BTree pt, int index){
	//ɾ��B��BT�Ͻ��Ϊp�Ĺؼ���key
	if (pt->ptr[index-1] != NULL){	//���������²���ն˽��
		Successor(pt, index);        //�ɺ�����²���ն˽�����С�ؼ��ִ�����
		DeleteBTree(BT,pt, 1);	  //���ɾ�����²���ն˽�����С�ؼ���
	}
	else{							//�������²���ն˽��
		Remove(pt, index);		//�ӽ��p��ɾ��key[index]
		if (NULL == pt->parent && 0 == pt->keynum){//����ɾ���������ɾ��������ؼ���Ϊ0���򽫸����free�����Ҹ���㸳ΪNULL��
			free(BT);
			BT = NULL;
			return;
		}
		if (pt->keynum < (m - 1) / 2 && pt->parent!=NULL)//����ǰ��㲻Ϊ�������ɾ����ؼ��ָ���С�ڹؼ��ָ�����Сֵ��m-1��/2
			Restore(BT,pt);	   //����B��
	}
}

void Successor(BTree &p, int index){
	//Ѱ�ҽ��p������²���ն˽�����С�ؼ���,������Ҫɾ���Ĺؼ���
	BTree temp = p->ptr[index];
	while (temp->ptr[0] != NULL)//Ѱ�Һ����С�ؼ������ڵĽ��
		temp = temp->ptr[0];
	p->key[index] = temp->key[1];//�ҵ���������С�ؼ��ִ���Ҫɾ���Ĺؼ���
	p = temp;//ͬʱ�洢��ǰ���ڽ�㴫���������ĺ�����תΪ�ڸý����ɾ����С�ؼ���
}

void Remove(BTree p, int index){
	//�ӽ��p��ɾ��key[index]
	int i;
	for (i = index; i < p->keynum; i++)   
		p->key[i] = p->key[i + 1];//�����������²��ն˽��ɾ����㣬ֻ�轫�����ǰ�ƴ���ɾ�����Ϳ��ԣ�����ת��������㡣
	p->keynum--;//ɾ�����󣬽��p�ؼ��ָ�����һ
}

void Restore(BTree &BT,BTree &pt){
	/**��ɾ�ؼ������ڽ��Ĺؼ��ָ���n���ڹؼ��ָ�����Сֵ(m-1)/2����ɾ�ؼ��ֺ���Ҫ����B��
	/**case 1->���ý�����ڵ��ң����ֵܽ���еĹؼ�����Ŀ����(m-1)/2��
	/*		             ��ɽ��ң��󣩽���� ��С���󣩹ؼ������Ƶ�˫�׽��
	/*				     ��˫�׽����С�����ڸ����ƹؼ��ֵĹؼ�����������ɾ�ؼ��ֵĽ����
	/* case  2->�������ֵܽ������Ŀ��������Сֵ(m-1)/2�������Ҫɾ���ؼ��ֵĽ�㼰���󣨻��ң��ֵܽ��
						 �Լ�˫�׽���зָ����ߵĹؼ��ֺϲ���һ����㣬����ɾ���ؼ��ֺ󣬸ý����ʣ��Ĺؼ��ּ�ָ��
						������˫�׽���еĹؼ���Kiһ�𣬺ϲ���Ai����˫�׽��ָ���ɾ���ؼ��ֽ������ң��ֵܽ���ָ�룩
						��ָ���ֵܽ����ȥ��������ʹ˫�׽���йؼ��ָ���С�ڣ�m-1��/2,��Դ�˫�׽����ͬ������
						�����ڿ���ֱ���Ը�����������Ĵ����ʹ����������һ�㡣
						���2�����Ը����������ʱ����Ҫ�ı�����
	***/
	int i,j,pos;
	BTree tp=pt->parent,lp,rp;//tp��ʾ��ǰ���pt��˫�׽�㣬lp��ʾ��ǰ���pt�����ֵܽ�㣬rp��ʾ��ǰ���pt�����ֵܽ��

	if (tp == NULL){//��ʱΪ���ϵ���B��ʱ���Ѿ���������㡣�������������
								//case 1:����������йؼ��֣���ֱ��������������������
								//case 2:�������û�йؼ��֣�����Ҫ�ı����㣬���ҵ�֮ǰ�ϲ��Ľ�㣬Ȼ�󽫸ý����Ϊ�µĸ���㡣
		if (pt->keynum == 0)//�����û�йؼ���
			for (i = 0; i <= (m - 1) / 2; i++)//��������
			if (pt->ptr[i] != NULL){//�ҵ�֮ǰ�ϲ��Ľ��
			     	BT = pt->ptr[i];//���ý����Ϊ�µĸ����
			    	BT->parent = NULL;//���¸ý���˫�׽�㣬����Ҫ��
					free(pt);//Ȼ��ԭ���ĸ����free������ֹ�ڴ�й¶
				break;//��ʱ�Ѿ����������B���ĵ���
			}
	  return;		 
	}

	for (i = 0; tp!=NULL && i <= tp->keynum;i++)//������pt����˫�׽��tp�е�����λ��pos
	  if (pt == tp->ptr[i]){
		  pos = i;
		  break;
	}

	  if (pos != 0 && tp->ptr[pos - 1]->keynum > (m - 1) / 2){
		  //����ǰ��㲻����˫�׽������ �� �����ֵܽ��Ĺؼ��ָ���������Сֵ�������ȡ��衱�����ֵܽ������ؼ���
		  lp = tp->ptr[pos - 1];
		  for (i = pt->keynum; i>0; i--){//����Ҫ���뵽��ǰ������С�ؼ��ִ�������Ҫ���ùؼ����ڳ�λ��
			  pt->key[i + 1] = pt->key[i];
			  pt->ptr[ i + 1] = pt->ptr[i];
		  }
		  pt->ptr[1] = pt->ptr[0];

		  pt->key[1] = tp->key[pos];//����˫�׽���зָ����ֵܽ��Ĺؼ��ֲ��뵽��ǰ������С�ؼ��ִ�
		  pt->ptr[0] = lp->ptr[lp->keynum];//ͬʱ�����ֵܽ����Ҫת�ƵĹؼ���ָ��������Ƶ���ǰ����0��������
		  if (pt->ptr[0] != NULL) pt->ptr[0]->parent = pt;//����ת�Ƶ�������Ϊ�գ�������˫�׽�㡣����Ҫ~
		  pt->keynum++;//���衱��ؼ��ֺ󣬵�ǰ���ؼ��ָ�����һ

		  tp->key[pos] = lp->key[lp->keynum];//�������ֵܽ�������Ĺؼ��ִ�����˫�׽����ת�ƵĹؼ���

		  lp->ptr[lp->keynum] = NULL;//��ת�Ƶ�������Ϊ�ա��˴��ɺ��ԣ�����Ϊ�˹淶ʹ�ã���ֹ���ʹ�õ��ô��ڴ������ݣ���ü��ϡ�
		  lp->keynum--;//�����ֵܽ��lp�ؼ��ָ�����һ
		  return;
	  }
	   else if (pos != tp->keynum && tp->ptr[pos + 1]->keynum > (m - 1) / 2){
		   //����ǰ��㲻����˫�׽������ �� �����ֵܽ��Ĺؼ��ָ���������Сֵ����"��"�����ֵܽ�����С�ؼ���
		   rp = tp->ptr[pos + 1];
		   pt->keynum++;//��ǰ���ؼ��ָ�����һ�������衱���Ĺؼ����ڳ�λ��
		   pt->key[pt->keynum] = tp->key[pos + 1];//����˫�׽���зָ����ֵܽ��Ĺؼ��ֲ��뵽��ǰ�������ؼ��ִ�
		   pt->ptr[pt->keynum] = rp->ptr[0];//ͬʱ�����ֵܽ����Ҫת�ƵĹؼ���ָ��������Ƶ���ǰ������ؼ�����ָ��������
		   if (pt->ptr[pt->keynum] != NULL) pt->ptr[pt->keynum]->parent = pt;//����ת�Ƶ�������Ϊ�գ�������˫�׽�㣬����Ҫ~

		   tp->key[pos + 1] = rp->key[1];//�������ֵܽ������С�Ĺؼ��ִ�����˫�׽����ת�ƵĹؼ���

		   rp->ptr[0] = rp->ptr[1];//���������ֵܽ��rp�Ĺؼ����Լ���ָ�������
		   for (i = 1; i < rp->keynum; i++){
			   rp->key[i] = rp->key[i + 1];
			   rp->ptr[i] = rp->ptr[i + 1];
		   }
		   rp->ptr[rp->keynum] = NULL; //�����º�ԭ�����ؼ��ִ���ָ���������Ϊ�ա��˴��ɺ��ԣ�����Ϊ�˹淶ʹ�ã���ֹ���ʹ�õ��ô��ڴ������ݣ���ü��ϡ�
		   rp->keynum--;//�����ֵܽ��rp�ؼ��ָ�����һ
		   return;
	   }
	  else {
		  //���������ֵܽ��Ĺؼ��ָ�����Ϊ��Сֵ������Ҫ�ϲ��ֵܽ��
		  if (pos != 0){
			  //����ǰ��㲻����˫�׽������ �����Ⱥϲ������ֵܽ��
			  MergeIntoLeft(tp, pt, pos);
			  if (tp->keynum <(m - 1) / 2)//��������˫�׽��ĸ���С����Сֵ������Ҫ�������ϵ���B����ע���������������ʱ��˫�׽���Ǹ���㣬��ֻҪ�����Ĺؼ��ֲ�Ϊ0������Ҫ����B����
				  Restore(BT, tp);
			  return;
		  }
		  else if (pos != tp->keynum){
			  //����ǰ��㲻����˫�׽������ ��ϲ������ֵܽ��
			  MergeIntoRight(tp, pt, pos);
			  if (tp->keynum < (m - 1) / 2)//��������˫�׽��ĸ���С����Сֵ������Ҫ�������ϵ���B����ע���������������ʱ��˫�׽���Ǹ���㣬��ֻҪ�����Ĺؼ��ֲ�Ϊ0������Ҫ����B����
				  Restore(BT, tp);
			  return;
		  }
	  }
}

void MergeIntoLeft(BTree &tp, BTree &pt, int pos){
	BTree lp;
	int i;
	lp = tp->ptr[pos - 1];
	lp->keynum++;//��ǰ���ؼ��ָ�����һ�������衱���Ĺؼ����ڳ�λ��
	lp->key[lp->keynum] = tp->key[pos];//�Ƚ���˫�׽���зָ����ֵܽ��Ĺؼ��ֲ��뵽���ֵܽ��ؼ��ֺ���
	if (pt->keynum == 0){//����Ϊ3��B�������ʱ
		//�����ϵ���ʱ�����ڲ���ȷ����֮ǰ����ϲ������Һϲ�������Ҫ���������ҵ��ϲ��Ľ��Ȼ��ת�Ƶ����ֵܽ�㣬����Ϊ˫�׽��ת�ƵĹؼ���ָ���������
		lp->ptr[lp->keynum] = pt->ptr[0] != NULL ? pt->ptr[0] : pt->ptr[1];
		//��ת�Ƶ�������Ϊ�ռ��ǵ����ϵ���ʱ֮ǰ�ϲ��Ľ�㣬����Ҫ������˫�׽�㡣����Ҫ��
		if (lp->ptr[lp->keynum] != NULL)   lp->ptr[lp->keynum]->parent = lp;
	}
	for (i = 1; i <= pt->keynum; i++) {  //��ɾ����ǰ������йؼ��֣����轫pt�����������lp
		if (i == 1) {
			lp->ptr[lp->keynum] = pt->ptr[0];
			if (lp->ptr[lp->keynum] != NULL)  lp->ptr[lp->keynum]->parent = lp;
		}
		lp->keynum++;
		lp->key[lp->keynum] = pt->key[i];
		lp->ptr[lp->keynum] = pt->ptr[i];
		if (lp->ptr[lp->keynum] != NULL)   lp->ptr[lp->keynum]->parent = lp;
	}
	free(pt);//ת���굱ǰ������Ҫ����ǰ���free������ֹ�ڴ�й¶��

	for (i = pos; i < tp->keynum; i++){//����ת����˫�׽��Ĺؼ��֣�����Ҫ����˫�׽��
		tp->key[i] = tp->key[i + 1];
		tp->ptr[i] = tp->ptr[i + 1];
	}
	tp->ptr[tp->keynum] = NULL;//�����º�ԭ�����ؼ��ִ���ָ���������Ϊ�ա��˴��ɺ��ԣ�����Ϊ�˹淶ʹ�ã���ֹ���ʹ�õ��ô��ڴ������ݣ��ʼ��ϡ�
	tp->keynum--;//˫�׽��ؼ��ּ�һ
}

void MergeIntoRight(BTree &tp, BTree &pt, int pos){
	BTree rp;
	int i;
	rp = tp->ptr[pos + 1];
	for (i = rp->keynum + pt->keynum; i > pt->keynum; i--){//������Ҫ����˫�׽���һ���ؼ��ֺ͵�ǰ���ɾ���ؼ��ֺ�ʣ��Ĺؼ��֣�����Ҫ����ƹؼ����ڳ�λ��
		rp->key[i + 1] = rp->key[i - pt->keynum];
		rp->ptr[i + 1] = rp->ptr[i - pt->keynum];
	}
	rp->ptr[pt->keynum + 1] = rp->ptr[0];

	rp->key[pt->keynum + 1] = tp->key[pos + 1];//�Ƚ���˫�׽���зָ����ֵܽ��Ĺؼ��ֲ��뵽���ֵܽ��
	rp->keynum++;//���ֵܽ��ؼ��ָ�����һ

	if (pt->keynum == 0){//����Ϊ3��B�������ʱ
		//�����ϵ���ʱ�����ڲ���ȷ����֮ǰ����ϲ������Һϲ�������Ҫ���������ҵ��ϲ��Ľ��Ȼ��ת�Ƶ����ֵܽ�㣬����Ϊ˫�׽��ת�ƵĹؼ���ָ���������
		rp->ptr[0] = pt->ptr[0] != NULL ? pt->ptr[0] : pt->ptr[1];
		//��ת�Ƶ�������Ϊ�ռ��ǵ����ϵ���ʱ֮ǰ�ϲ��Ľ�㣬����Ҫ������˫�׽�㡣����Ҫ��
		if (rp->ptr[0] != NULL)  rp->ptr[0]->parent = rp;
	}
	for (i = 1; i <= pt->keynum; i++) {  //��ɾ����ǰ������йؼ��֣���pt�����������rp
		if (i == 1) {
			rp->ptr[0] = pt->ptr[0];
			if (rp->ptr[0] != NULL)   rp->ptr[0]->parent = rp;
		}
		rp->keynum++;
		rp->key[i] = pt->key[i];
		rp->ptr[i] = pt->ptr[i];
		if (rp->ptr[i] != NULL)  rp->ptr[i]->parent = rp;
	}
	free(pt);//ת���굱ǰ������Ҫ����ǰ���free������ֹ�ڴ�й¶

	for (i = pos; i < tp->keynum; i++){////����ת����˫�׽��Ĺؼ��֣�����Ҫ����˫�׽��
		if (i + 1 < tp->keynum)
			tp->key[i + 1] = tp->key[i + 2];
		tp->ptr[i] = tp->ptr[i + 1];
	}
	tp->ptr[tp->keynum] = NULL;//�����º�ԭ�����ؼ��ִ���ָ���������Ϊ�ա��˴��ɺ��ԣ�����Ϊ�˹淶ʹ�ã���ֹ���ʹ�õ��ô��ڴ������ݣ��ʼ��ϡ�
	tp->keynum--;//˫�׽��ؼ��ָ�����һ
}

void DestroyBTree(BTree &BT){
	int i;
	if (NULL == BT) return;
	i = 0;
	while (i <= BT->keynum){
		DestroyBTree(BT->ptr[i]);
		i++;
	}
	free(BT);
	BT = NULL;
}
//****���еĹ���
Status InitQueue(BTQueue &Q){
	//����һ���ն���
	Q.front = Q.rear = (BTQueueNode *)malloc(sizeof(BTQueueNode));
	if (NULL == Q.front) return OVREFLOW;
	Q.front->next = Q.rear->next = NULL;
	return OK;
}

Status DeQueue(BTQueue &Q, BTree &e){ 
	//���Ӳ���	
	//�����в��գ���ɾ������Q�е�ͷԪ��,��e������ֵ��������OK�����򷵻�ERROR
	BTQueuePtr temp;
	if (NULL == Q.front) return ERROR;
	temp = Q.front; 
	Q.front = Q.front->next; 
	free(temp);
	return OK;
}

Status EnQueue(BTQueue &Q, BTree e){
	//��Ӳ���
	//�ڶ��е�βԪ��֮�󣬲���Ԫ��eΪ�µĶ���βԪ��
	BTQueuePtr newNode;
	newNode = (BTQueueNode *)malloc(sizeof(BTQueueNode));
	if (NULL == newNode) return OVREFLOW;
	newNode->data = e;
	newNode->next = NULL;
	if (NULL == Q.front)   Q.front = newNode;
	else		Q.rear->next = newNode;
	Q.rear = newNode;
	return OK;
}

void DestroyQueue(BTQueue &Q){
	//���ٶ���Q,Q���ٴ���
	BTQueuePtr temp;
	while (Q.front != NULL){
		temp = Q.front;
		Q.front = Q.front->next;
		free(temp);
	}
}

Status QueueEmpty(BTQueue  Q){
	//�жϵ�ǰ�����Ƿ�Ϊ��
	if (Q.front == Q.rear) return TRUE;
	else return FALSE;
}
//*****���еĹ���

//****B���ı���
void LevelOrderBTree(BTree BT){
	//���ö��в�α���B��
	BTQueue Q;
	BTree temp;
	int i, num,count;
	if (NULL == BT) return;
	InitQueue(Q);
	if (OVREFLOW == EnQueue(Q, BT)){
		printf("�ڴ���䲻��");
		exit(1);
	}
	while (!QueueEmpty(Q))
	{
		DeQueue(Q, temp);
		for (i = 0; i <= temp->keynum; i++){
			if(temp->ptr[i]!=NULL ) EnQueue(Q, temp->ptr[i]);
			if (i != 0) printf("%d    ", temp->key[i]);
		}
		printf("\n");
	}
	DestroyQueue(Q);
}

void PreOrderBTree(BTree BT){
	//�������B�������ð�������ʽ��ӡB��
	BTree temp, tmp;//temp�洢��ǰ��㣬tmp�洢����ʱ��һ�����
	int i,level,flag;//�洢��ǰ���Ĳ���
	if (BT == NULL)
		printf("��ǰΪ��\n");
	if (BT != NULL){//����㲻Ϊ��
		temp = BT;
		level = 1;//��ǰ����Ϊ��һ��
		while (temp != NULL){
			visitBTNode(temp, level);//���ʵ�ǰ���
			for (i = 0; i <= temp->keynum; i++){//�ж��Ƿ���������Ϊ��
				if (temp->ptr[i] != NULL){//����������Ϊ��
					temp = temp->ptr[i];//����������
					level++;//������һ
					break;
				}
			}
			if (i > temp->keynum){//��������Ϊ�գ�����ǰ���Ϊ��ײ���ն˽��
				do{//���Ϸ���
					tmp = temp;
					temp = temp->parent;
					level--;//���أ�������һ
					if (temp != NULL){//�����ص�������˫�׽�㣬���˳�
						for (i = 0, flag = 0; i <= temp->keynum; i++){
							//�ж��Ƿ�������ء�˼·���ҵ���һ�����ʵĽ�㣬�ж������ֵܽ���Ƿ�Ϊ�ա���Ϊ�գ���������ʡ�����Ϊ�գ�����������ֵܽ��
							if (i == 0){//��֮ǰ���ʵ�Ϊ��0������
								if (NULL == temp->ptr[1])//�жϵ�һ�������Ƿ�Ϊ�գ���Ϊ�գ���������ء�
									flag = 1;
							}
							else if (i == temp->keynum){
								if (tmp == temp->ptr[temp->keynum])
									flag = 1;
							}
							else{
								//�ҵ���һ�����ʵĽ�㣬�ж������ֵܽ���Ƿ�Ϊ�ա�
								if (tmp == temp->ptr[i] && NULL == temp->ptr[i + 1])
									flag = 1;
							}
							if (flag) break;
						}
					}
				} while (temp != NULL && flag);//flag=1����������Ϸ���
				if (temp != NULL)//���ҵ����Լ������ʵĽ��
				for (i = 0; i <= temp->keynum; i++){
					//���ҵ���һ�����ʵĽ�㣬Ȼ����������ֵܽ��
					if (tmp == temp->ptr[i]){
						temp = temp->ptr[i + 1];
						level++;//������һ
						break;
					}
				}
			}
		}
	}
}

Status visitBTNode(BTree BT, int level){
	//���ݽ�����ڲ�ͬλ�ô�ӡ���ؼ���
	if (NULL == BT) return ERROR;
	int  i,j;
	for (j = 0; j < level; j++)//ÿ��һ�㣬������λ��һ���Ʊ��������Խ����Ľ�㣬����Խ��
		printf("\t");
	for (i = 1; i <= BT->keynum; i++)//��ӡ�ؼ���
	{
		if (i >= 2)//������д��ڵ��������ؼ���ʱ���ö��Ÿ���
			printf(",%d", BT->key[i]);
		else 
			printf("%d", BT->key[i]);
	}
	printf("\n");//һ�д�ӡһ�����Ĺؼ���
	return OK;
}
//****B���ı���