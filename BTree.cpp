#include"Btree.h"
#include<stdio.h>
#include<stdlib.h>

//****B树的创建///
void CreatBTree(BTree &BT, int n,int min,int max){
	KeyType k;
	Result result;
	int i;
	for (i = 0; i <n; i++){//根据用户输入的关键字个数n创建结点
		k =(unsigned)rand() % (max-min+1) + min;//随机产生min - max之间的数字
		SearchBTree(BT, k, result);
		if (!result.tag)  InsertBTree(BT, k, result.pt, result.index);//插入关键字结点，若有相同关键字则不插入
	}
}
//****B树的创建

//****B树的查找
void SearchBTree(BTree BT, KeyType target, Result &result){
	//在m阶B树上查找关键码target,返回（pt,i,tag）
	//若查找成功，则特征值tag = 1,指针pt所指结点中第 i 个关键码等于target，否则
	//查找失败，特征值tag=0，等于target的关键码记录应插在指针pt所指结点中第i个和第i+1个关键码间
	int i = 0, found = 0;//i用于记录对应结点中关键码的位置，found标识是否找到关键码
	BTree temp = BT,tmp=NULL;//初始，temp指向待查结点，tmp指向temp的双亲
	while (temp != NULL && 0 == found){//当待查结点非空且未找到，继续查找
		i = Search(temp,target);
		if (i <= temp->keynum && temp->key[i] == target)
			found = 1;//找到待查关键字
		else{
			tmp = temp;//tmp存储temp
			temp = temp->ptr[i - 1];//待查关键字大于temp结点中第i个关键字，故在temp的第i-1个孩子结点中查找。
		}
	}
	if (1 == found){//查找成功
		result.pt=temp,result.index = i,result.tag=1;
	}
	else{//查找不成功，返回可插入位置 i
		result.pt = tmp, result.index = i, result.tag = 0;
	}
}

int Search(BTree temp, KeyType target){
	int i = 1;
	while (i <= temp->keynum && target > temp->key[i])
		i++;//当结点关键字个数满足要求且待查关键字大于关键字时，继续查找
	return i;
}
//***B树的查找


//***B树的插入
void InsertBTree(BTree &BT, KeyType target, BTree pt, int index){
	//利用查找函数查找关键字target的插入位置。若找到target,则该关键字存在，不插入直接返回。
	//否则查找操作必失败于某个最底层的非终端结点即结点pt上，在B树BT上结点*pt的key[index]和key[index+1]之间插入关键字target.
	//若引起结点过大，则沿双亲链进行必要的结点分裂调整，使T仍是m阶的B树。
	//其中，pt,index由查找函数SearchBTree返回的result所得
	int  mid;//存储中间位置
	KeyType key;//存储插入的关键字
	BTree ap = NULL ;//存储新结点
	int finished = 0, needNewRoot = 0;//标识符，标识是否插入完毕或者是否需要新根
	if (NULL == pt) {//若插入时，BT为空树则需要建立新根存储插入的关键字
		if (OVREFLOW == newRoot(BT, pt, target, ap)){
			printf("内存分配不足");
			exit(1);
		}
	 }
	else{
		key = target;//存储需要插入的关键字
		while (0 == needNewRoot && 0 == finished){//当完成插入或者需要新根时，停止插入操作
			Insert(pt, index, key, ap);//结点*pt的key[index]和key[index+1]之间插入关键字，并将ap插入到pt->ptr[index+1]
			if (pt->keynum < m) finished = 1;//若插入结点关键字数目要求，则插入完毕
			else{//否则需要分裂结点pt
				mid = (m + 1) / 2;//存储中间位置
				if (OVREFLOW == spilt(pt, mid, ap)){//分裂结点pt
					printf("内存分配不足");
					exit(1);
				}
				key = pt->key[mid];//存储中间位置的关键字
				if (pt->parent != NULL){//若不是根结点分裂
					pt = pt->parent;//将中间位置的关键字连同新结点的存储位置插入到父结点中。
					index = Search(pt, key);
				}
				else needNewRoot = 1;//根结点分裂则需要建立新根结点
			}
		}
		if (1 == needNewRoot){
			if (OVREFLOW == newRoot(BT, pt, key, ap)){//建立新根结点BT
				printf("内存分配不足");
				exit(1);
			}
		}
	}
}

Status newRoot(BTree &t, BTree &p, KeyType key, BTree &ap){
	//若插入关键字时，当前为空树或者引起根结点分裂时，需要新的根结点，用到该方法。
	//生成新的根结点t,其中p,ap为t的两棵子树，插入或分裂得到的关键字作为根结点的关键字
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
	//插入关键字后引起结点数目过大时，用到该方法
	//分裂的方法：生成一个新结点ap，从中间位置把结点q（不包括中间位置的关键字）分成两部分。
	//前半部分留在旧结点，后半部分复制到新结点ap中，中间位置的关键字连同新结点的存储位置插入到父结点中。
	//如果插入后父结点的关键字个数也超过m-1,则继续向上分裂。如果根结点仍需分裂，则产生新的根结点
	int i, j, n = q->keynum;
	ap = (BTNode *)malloc(sizeof(BTNode));//新建新结点ap
	if (NULL == ap) return OVREFLOW;
	for (i = 0; i <= m; i++)
		ap->ptr[i] = NULL;
	ap->ptr[0] = q->ptr[mid];//中间位置指向的子树作为新结点ap的第一个子树
	for (i = mid + 1, j = 1; i <= n; i++, j++){//q结点后一半复制到ap
		ap->key[j] = q->key[i];
		ap->ptr[j] = q->ptr[i];
		q->ptr[i] = NULL;
	}
	ap->keynum = n - mid;//新结点关键字个数为n-mid
	ap->parent = q->parent;//旧结点和新结点指向同一个双亲结点
	for (i = 0; i <= n - mid; i++){
		if (ap->ptr[i])
			ap->ptr[i]->parent = ap;//更新ap的子结点即复制后原来旧结点后一半结点的父结点
	}
	q->ptr[mid] = NULL;
	q->keynum = mid - 1;//更新旧结点的关键字个数
	return OK;
}

void Insert(BTree &q, int index, KeyType key, BTree &ap){
	//当插入关键字或者分裂后中间位置关键字加入父结点时，用到该方法。
	//key插入到q->key[index+1],ap插入到q->ptr[index+1]
	int j, n = q->keynum;//n存储q当前关键字个数
	for (j = n; j >= index; j--){//若需要从中间插入，则比key大的关键字需要往后移一位，同时对于的孩子结点指针也得移动
		q->key[j + 1] = q->key[j];
		q->ptr[j + 1] = q->ptr[j];
	}
	q->key[index] = key;//在对应位置插入关键字key
	q->ptr[index] = ap;//在对应位置插入孩子结点指针。若关键字刚插入，则孩子结点指针为NULL，若是分裂后插入，则孩子结点指针为新结点的存储位置
	if (ap != NULL)
		ap->parent = q;//更新孩子结点指针的父结点
	q->keynum++;//插入后q的关键字数目加一。
}
//****B树的插入


//****B树的删除
Status Delete(BTree &BT, KeyType target){
	Result result;
	SearchBTree(BT, target, result);
	if (!result.tag) return FALSE;
	DeleteBTree(BT, result.pt, result.index);
	return OK;
}

void DeleteBTree(BTree &BT,BTree pt, int index){
	//删除B树BT上结点为p的关键字key
	if (pt->ptr[index-1] != NULL){	//若不是最下层非终端结点
		Successor(pt, index);        //由后继最下层非终端结点的最小关键字代替它
		DeleteBTree(BT,pt, 1);	  //变成删除最下层非终端结点的最小关键字
	}
	else{							//若是最下层非终端结点
		Remove(pt, index);		//从结点p中删除key[index]
		if (NULL == pt->parent && 0 == pt->keynum){//若是删除根结点且删除后根结点关键字为0，则将根结点free掉，且根结点赋为NULL。
			free(BT);
			BT = NULL;
			return;
		}
		if (pt->keynum < (m - 1) / 2 && pt->parent!=NULL)//若当前结点不为根结点且删除后关键字个数小于关键字个数最小值（m-1）/2
			Restore(BT,pt);	   //调整B树
	}
}

void Successor(BTree &p, int index){
	//寻找结点p后继最下层非终端结点的最小关键字,并代替要删除的关键字
	BTree temp = p->ptr[index];
	while (temp->ptr[0] != NULL)//寻找后继最小关键字所在的结点
		temp = temp->ptr[0];
	p->key[index] = temp->key[1];//找到结点后以最小关键字代替要删除的关键字
	p = temp;//同时存储当前所在结点传给接下来的函数，转为在该结点中删除最小关键字
}

void Remove(BTree p, int index){
	//从结点p中删除key[index]
	int i;
	for (i = index; i < p->keynum; i++)   
		p->key[i] = p->key[i + 1];//由于是在最下层终端结点删除结点，只需将结点向前移代替删除结点就可以，无需转移子树结点。
	p->keynum--;//删除结点后，结点p关键字个数减一
}

void Restore(BTree &BT,BTree &pt){
	/**被删关键字所在结点的关键字个数n等于关键字个数最小值(m-1)/2，则删关键字后需要调整B树
	/**case 1->若该结点相邻的右（左）兄弟结点中的关键字数目大于(m-1)/2，
	/*		             则可将右（左）结点中 最小（大）关键字上移到双亲结点
	/*				     后将双亲结点中小（大）于该上移关键字的关键字下移至被删关键字的结点中
	/* case  2->若相邻兄弟结点中数目均等于最小值(m-1)/2，则需把要删除关键字的结点及其左（或右）兄弟结点
						 以及双亲结点中分隔两者的关键字合并成一个结点，即在删除关键字后，该结点中剩余的关键字加指针
						，加上双亲结点中的关键字Ki一起，合并到Ai（即双亲结点指向该删除关键字结点的左（右）兄弟结点的指针）
						所指的兄弟结点中去，如果因此使双亲结点中关键字个数小于（m-1）/2,则对此双亲结点做同样处理，
						以致于可能直到对根结点做这样的处理而使整个树减少一层。
						情况2中若对根结点做调整时，需要改变根结点
	***/
	int i,j,pos;
	BTree tp=pt->parent,lp,rp;//tp表示当前结点pt的双亲结点，lp表示当前结点pt的左兄弟结点，rp表示当前结点pt的右兄弟结点

	if (tp == NULL){//此时为向上调整B树时，已经处理到根结点。则有两种情况。
								//case 1:若根结点仍有关键字，则直接跳出函数，不做处理。
								//case 2:若根结点没有关键字，则需要改变根结点，即找到之前合并的结点，然后将该结点作为新的根结点。
		if (pt->keynum == 0)//根结点没有关键字
			for (i = 0; i <= (m - 1) / 2; i++)//遍历子树
			if (pt->ptr[i] != NULL){//找到之前合并的结点
			     	BT = pt->ptr[i];//将该结点作为新的根结点
			    	BT->parent = NULL;//跟新该结点的双亲结点，很重要。
					free(pt);//然后将原来的根结点free掉，防止内存泄露
				break;//此时已经完成了整棵B树的调整
			}
	  return;		 
	}

	for (i = 0; tp!=NULL && i <= tp->keynum;i++)//计算结点pt在其双亲结点tp中的所在位置pos
	  if (pt == tp->ptr[i]){
		  pos = i;
		  break;
	}

	  if (pos != 0 && tp->ptr[pos - 1]->keynum > (m - 1) / 2){
		  //若当前结点不在其双亲结点最左 且 其左兄弟结点的关键字个数大于最小值，则优先“借”其左兄弟结点的最大关键字
		  lp = tp->ptr[pos - 1];
		  for (i = pt->keynum; i>0; i--){//由于要插入到当前结点的最小关键字处，故需要给该关键字腾出位置
			  pt->key[i + 1] = pt->key[i];
			  pt->ptr[ i + 1] = pt->ptr[i];
		  }
		  pt->ptr[1] = pt->ptr[0];

		  pt->key[1] = tp->key[pos];//将其双亲结点中分隔两兄弟结点的关键字插入到当前结点的最小关键字处
		  pt->ptr[0] = lp->ptr[lp->keynum];//同时将左兄弟结点中要转移的关键字指向的子树移到当前结点第0处子树。
		  if (pt->ptr[0] != NULL) pt->ptr[0]->parent = pt;//若该转移的子树不为空，更新其双亲结点。很重要~
		  pt->keynum++;//“借”完关键字后，当前结点关键字个数加一

		  tp->key[pos] = lp->key[lp->keynum];//将其左兄弟结点中最大的关键字代替其双亲结点中转移的关键字

		  lp->ptr[lp->keynum] = NULL;//将转移的子树置为空。此处可忽略，不过为了规范使用，防止溢出使用到该处内存中数据，最好加上。
		  lp->keynum--;//其左兄弟结点lp关键字个数减一
		  return;
	  }
	   else if (pos != tp->keynum && tp->ptr[pos + 1]->keynum > (m - 1) / 2){
		   //若当前结点不在其双亲结点最右 且 其右兄弟结点的关键字个数大于最小值，则"借"其右兄弟结点的最小关键字
		   rp = tp->ptr[pos + 1];
		   pt->keynum++;//当前结点关键字个数加一，给“借”来的关键字腾出位置
		   pt->key[pt->keynum] = tp->key[pos + 1];//将其双亲结点中分隔两兄弟结点的关键字插入到当前结点的最大关键字处
		   pt->ptr[pt->keynum] = rp->ptr[0];//同时将左兄弟结点中要转移的关键字指向的子树移到当前结点最大关键字所指的子树。
		   if (pt->ptr[pt->keynum] != NULL) pt->ptr[pt->keynum]->parent = pt;//若该转移的子树不为空，更新其双亲结点，很重要~

		   tp->key[pos + 1] = rp->key[1];//将其右兄弟结点中最小的关键字代替其双亲结点中转移的关键字

		   rp->ptr[0] = rp->ptr[1];//更新其右兄弟结点rp的关键字以及所指向的子树
		   for (i = 1; i < rp->keynum; i++){
			   rp->key[i] = rp->key[i + 1];
			   rp->ptr[i] = rp->ptr[i + 1];
		   }
		   rp->ptr[rp->keynum] = NULL; //将更新后原来最大关键字处所指向的子树置为空。此处可忽略，不过为了规范使用，防止溢出使用到该处内存中数据，最好加上。
		   rp->keynum--;//其右兄弟结点rp关键字个数减一
		   return;
	   }
	  else {
		  //若其左右兄弟结点的关键字个数都为最小值，则需要合并兄弟结点
		  if (pos != 0){
			  //若当前结点不在其双亲结点最左 则优先合并到左兄弟结点
			  MergeIntoLeft(tp, pt, pos);
			  if (tp->keynum <(m - 1) / 2)//若调整后双亲结点的个数小于最小值，则需要继续向上调整B树。注意特殊情况。若此时的双亲结点是根结点，则只要根结点的关键字不为0，则不需要调整B树。
				  Restore(BT, tp);
			  return;
		  }
		  else if (pos != tp->keynum){
			  //若当前结点不在其双亲结点最右 则合并到右兄弟结点
			  MergeIntoRight(tp, pt, pos);
			  if (tp->keynum < (m - 1) / 2)//若调整后双亲结点的个数小于最小值，则需要继续向上调整B树。注意特殊情况。若此时的双亲结点是根结点，则只要根结点的关键字不为0，则不需要调整B树。
				  Restore(BT, tp);
			  return;
		  }
	  }
}

void MergeIntoLeft(BTree &tp, BTree &pt, int pos){
	BTree lp;
	int i;
	lp = tp->ptr[pos - 1];
	lp->keynum++;//当前结点关键字个数加一，给“借”来的关键字腾出位置
	lp->key[lp->keynum] = tp->key[pos];//先将其双亲结点中分隔两兄弟结点的关键字插入到左兄弟结点关键字后面
	if (pt->keynum == 0){//当此为3阶B树的情况时
		//当向上调整时，由于不能确定是之前是左合并或者右合并。故需要遍历子树找到合并的结点然后转移到左兄弟结点，插入为双亲结点转移的关键字指向的子树。
		lp->ptr[lp->keynum] = pt->ptr[0] != NULL ? pt->ptr[0] : pt->ptr[1];
		//若转移的子树不为空即是当向上调整时之前合并的结点，则需要更新其双亲结点。很重要。
		if (lp->ptr[lp->keynum] != NULL)   lp->ptr[lp->keynum]->parent = lp;
	}
	for (i = 1; i <= pt->keynum; i++) {  //若删除后当前结点仍有关键字，则需将pt结点内容移至lp
		if (i == 1) {
			lp->ptr[lp->keynum] = pt->ptr[0];
			if (lp->ptr[lp->keynum] != NULL)  lp->ptr[lp->keynum]->parent = lp;
		}
		lp->keynum++;
		lp->key[lp->keynum] = pt->key[i];
		lp->ptr[lp->keynum] = pt->ptr[i];
		if (lp->ptr[lp->keynum] != NULL)   lp->ptr[lp->keynum]->parent = lp;
	}
	free(pt);//转移完当前结点后，需要将当前结点free掉，防止内存泄露。

	for (i = pos; i < tp->keynum; i++){//由于转移了双亲结点的关键字，故需要更新双亲结点
		tp->key[i] = tp->key[i + 1];
		tp->ptr[i] = tp->ptr[i + 1];
	}
	tp->ptr[tp->keynum] = NULL;//将更新后原来最大关键字处所指向的子树置为空。此处可忽略，不过为了规范使用，防止溢出使用到该处内存中数据，故加上。
	tp->keynum--;//双亲结点关键字减一
}

void MergeIntoRight(BTree &tp, BTree &pt, int pos){
	BTree rp;
	int i;
	rp = tp->ptr[pos + 1];
	for (i = rp->keynum + pt->keynum; i > pt->keynum; i--){//由于需要插入双亲结点的一个关键字和当前结点删除关键字后剩余的关键字，故需要向后移关键字腾出位置
		rp->key[i + 1] = rp->key[i - pt->keynum];
		rp->ptr[i + 1] = rp->ptr[i - pt->keynum];
	}
	rp->ptr[pt->keynum + 1] = rp->ptr[0];

	rp->key[pt->keynum + 1] = tp->key[pos + 1];//先将其双亲结点中分隔两兄弟结点的关键字插入到右兄弟结点
	rp->keynum++;//右兄弟结点关键字个数加一

	if (pt->keynum == 0){//当此为3阶B树的情况时
		//当向上调整时，由于不能确定是之前是左合并或者右合并。故需要遍历子树找到合并的结点然后转移到右兄弟结点，插入为双亲结点转移的关键字指向的子树。
		rp->ptr[0] = pt->ptr[0] != NULL ? pt->ptr[0] : pt->ptr[1];
		//若转移的子树不为空即是当向上调整时之前合并的结点，则需要更新其双亲结点。很重要。
		if (rp->ptr[0] != NULL)  rp->ptr[0]->parent = rp;
	}
	for (i = 1; i <= pt->keynum; i++) {  //若删除后当前结点仍有关键字，将pt结点内容移至rp
		if (i == 1) {
			rp->ptr[0] = pt->ptr[0];
			if (rp->ptr[0] != NULL)   rp->ptr[0]->parent = rp;
		}
		rp->keynum++;
		rp->key[i] = pt->key[i];
		rp->ptr[i] = pt->ptr[i];
		if (rp->ptr[i] != NULL)  rp->ptr[i]->parent = rp;
	}
	free(pt);//转移完当前结点后，需要将当前结点free掉，防止内存泄露

	for (i = pos; i < tp->keynum; i++){////由于转移了双亲结点的关键字，故需要更新双亲结点
		if (i + 1 < tp->keynum)
			tp->key[i + 1] = tp->key[i + 2];
		tp->ptr[i] = tp->ptr[i + 1];
	}
	tp->ptr[tp->keynum] = NULL;//将更新后原来最大关键字处所指向的子树置为空。此处可忽略，不过为了规范使用，防止溢出使用到该处内存中数据，故加上。
	tp->keynum--;//双亲结点关键字个数减一
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
//****队列的构造
Status InitQueue(BTQueue &Q){
	//构造一个空队列
	Q.front = Q.rear = (BTQueueNode *)malloc(sizeof(BTQueueNode));
	if (NULL == Q.front) return OVREFLOW;
	Q.front->next = Q.rear->next = NULL;
	return OK;
}

Status DeQueue(BTQueue &Q, BTree &e){ 
	//出队操作	
	//若队列不空，则删除队列Q中的头元素,用e返回其值，并返回OK。否则返回ERROR
	BTQueuePtr temp;
	if (NULL == Q.front) return ERROR;
	temp = Q.front; 
	Q.front = Q.front->next; 
	free(temp);
	return OK;
}

Status EnQueue(BTQueue &Q, BTree e){
	//入队操作
	//在队列的尾元素之后，插入元素e为新的队列尾元素
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
	//销毁队列Q,Q不再存在
	BTQueuePtr temp;
	while (Q.front != NULL){
		temp = Q.front;
		Q.front = Q.front->next;
		free(temp);
	}
}

Status QueueEmpty(BTQueue  Q){
	//判断当前队列是否为空
	if (Q.front == Q.rear) return TRUE;
	else return FALSE;
}
//*****队列的构造

//****B树的遍历
void LevelOrderBTree(BTree BT){
	//利用队列层次遍历B树
	BTQueue Q;
	BTree temp;
	int i, num,count;
	if (NULL == BT) return;
	InitQueue(Q);
	if (OVREFLOW == EnQueue(Q, BT)){
		printf("内存分配不足");
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
	//先序遍历B树，利用凹入表的形式打印B表
	BTree temp, tmp;//temp存储当前结点，tmp存储返回时上一个结点
	int i,level,flag;//存储当前结点的层数
	if (BT == NULL)
		printf("当前为空\n");
	if (BT != NULL){//根结点不为空
		temp = BT;
		level = 1;//当前层数为第一层
		while (temp != NULL){
			visitBTNode(temp, level);//访问当前结点
			for (i = 0; i <= temp->keynum; i++){//判断是否有子树不为空
				if (temp->ptr[i] != NULL){//若有子树不为空
					temp = temp->ptr[i];//遍历该子树
					level++;//层数加一
					break;
				}
			}
			if (i > temp->keynum){//若子树都为空，即当前结点为最底层非终端结点
				do{//向上返回
					tmp = temp;
					temp = temp->parent;
					level--;//返回，层数减一
					if (temp != NULL){//若返回到根结点的双亲结点，则退出
						for (i = 0, flag = 0; i <= temp->keynum; i++){
							//判断是否继续返回。思路：找到上一个访问的结点，判断其右兄弟结点是否为空。若为空，则继续访问。若不为空，则访问其右兄弟结点
							if (i == 0){//若之前访问的为第0个子树
								if (NULL == temp->ptr[1])//判断第一个子树是否为空，若为空，则继续返回。
									flag = 1;
							}
							else if (i == temp->keynum){
								if (tmp == temp->ptr[temp->keynum])
									flag = 1;
							}
							else{
								//找到上一个访问的结点，判断其右兄弟结点是否为空。
								if (tmp == temp->ptr[i] && NULL == temp->ptr[i + 1])
									flag = 1;
							}
							if (flag) break;
						}
					}
				} while (temp != NULL && flag);//flag=1，则继续向上返回
				if (temp != NULL)//若找到可以继续访问的结点
				for (i = 0; i <= temp->keynum; i++){
					//则找到上一个访问的结点，然后访问其右兄弟结点
					if (tmp == temp->ptr[i]){
						temp = temp->ptr[i + 1];
						level++;//层数加一
						break;
					}
				}
			}
		}
	}
}

Status visitBTNode(BTree BT, int level){
	//根据结点层次在不同位置打印结点关键字
	if (NULL == BT) return ERROR;
	int  i,j;
	for (j = 0; j < level; j++)//每多一层，则向右位移一个制表符。所以越靠左的结点，层数越低
		printf("\t");
	for (i = 1; i <= BT->keynum; i++)//打印关键字
	{
		if (i >= 2)//当结点有大于等于两个关键字时，用逗号隔开
			printf(",%d", BT->key[i]);
		else 
			printf("%d", BT->key[i]);
	}
	printf("\n");//一行打印一个结点的关键字
	return OK;
}
//****B树的遍历