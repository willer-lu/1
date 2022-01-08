#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>
//3��B-��Ӧ��
//������������
//Ӧ��3��B-������ͼ��ݵ�ͼ����м򵥹���
//��ʵ��Ҫ��
//��ƻ���B-����ͼ�����ģ�����
//��1������3��B-���洢�ṹ��
//��2�������Ϊ�ؼ��֣����ͼ�����⡢����Ͳ�ѯ������
//��3���Զ�����ʾ�����������ʽ��
//�ļ���txt��ʽ����,������ţ�ÿ����¼֮���Ի��зָ�
#define MIN_T 2
#define MAX_T 4//B-3�������Լ���ഢ��Ľ��
#define BTREE_NODE_SIZE sizeof(BTreeNodedata)
#define BTREE_SIZE sizeof(BTreedata)
typedef struct BTreeNodedata BTreeNodedata;
typedef struct BTreeNodedata *BTreeNode;
typedef struct BTreedata BTreedata;
typedef struct BTreedata *BTree;
struct BTreeNodedata//B�����ṹ��
{
 int n;	//�ؼ��ָ���
 int leaf;//��¼�Ƿ�ΪҶ�ӽ�㣬1ΪҶ�ӽڵ� 0��֮
 int key[MAX_T - 1];	//�ؼ���
 BTreeNode child[MAX_T];	//�ӽ��
};
struct BTreedata//B���ṹ��
{
 BTreeNode	root;	//B���ĸ����
};
BTreeNode allocate_node()
{
	BTreeNode node = (BTreeNode) malloc (BTREE_NODE_SIZE);
	return node;
}//Ϊ�½�����ռ�
void btree_create(BTree tree)
{
	BTreeNode r_node = allocate_node();
	(r_node)->n = 0;
	(r_node)->leaf = 1;
	(tree)->root = r_node;
}//����һ�ÿ������ؼ��ָ���Ϊ0����ΪҶ�ӽ��
//����nodeΪ���������У�Ѱ�ҹؼ���λ�� ���عؼ������ڽ�㣬�����ؼ���λ�ñ�����location
BTreeNode btree_maximum(BTreeNode node)
{
	BTreeNode newnode = node;
	if(newnode->n < 1)
	{
		printf("this is null tree\n");
		return NULL;
	}
	if(node->leaf)
		return newnode;
	else
		newnode = btree_maximum(node->child[node->n]);
	return newnode;
}//������nodeΪ������������ؼ��ֵĽ�㣬�ؼ��ֵ�λ�ÿ϶�Ϊ�ý���n-1ֵ
BTreeNode btree_minimum(BTreeNode node)
{
	BTreeNode newnode = node;
	if(newnode->n < 1)
	{
		printf("this is null tree\n");
		return NULL;
	}
	if(node->leaf)
		return newnode;
	else
		newnode = btree_minimum(node->child[0]);
	return newnode;
}//������nodeΪ�����������С�ؼ��ֵĽ�㣬�ؼ��ֵ�λ�ÿ϶�Ϊ0

void btree_search(BTreeNode node, int key)
{

	int j = 0;
	/*
     * ������ǰ��㣬Ѱ��ǡ���Ĺؼ��֣�����ҵ���ȵĹؼ��֣����ؽ�㲢���ؼ���λ�ñ�����location
     * ���û�ҵ���Ƚ�㣬�Ҹý��ΪҶ�ӽ�㣬�򱨴�
     * ����ݹ�Ѱ��
     */
	while(j < node->n && key > node->key[j])
		j++;
	if(j < node->n && key == node->key[j])
	{
		printf("��ͼ���ڿ���\n");
	}
	else if(node->leaf)
	{
		printf("���鲻����\n");
	}
	else  btree_search(node->child[j], key);
}
void btree_split_child(BTreeNode node, int location)
{
	/* �����µĿս�� */
	BTreeNode newnode = allocate_node();
	BTreeNode childnode = node->child[location];
	int i = 0;
	/* ��ʼ���ս��newnode�����ӽ��childnode����Ϣ���Ƶ��½��node�� */
	newnode->leaf = childnode->leaf;
	newnode->n = MIN_T - 1;
	/* ���ӽ��childnode��T-1���ؼ��ָ��Ƶ��½���У����ı��ӽ���nֵ */
	for(i = 0;i <= MIN_T - 2;i++)
		newnode->key[i] = childnode->key[i + MIN_T];
	childnode->n = MIN_T - 1;
	/* ����ӽ���Ҷ�ӽ�㣬����Ӧ�Ľ��ӽ��Ľ��㸴�Ƶ��½���� */
	if(!childnode->leaf)
		for(i = 0;i <= MIN_T - 1;i++)
			newnode->child[i] = childnode->child[i + MIN_T];
	/* ��������Ӧ�Ĺؼ����Լ��ӽ��λ������ƶ�һλ */
	for(i = node->n;i > location;i--)
	{
		node->key[i] = node->key[i - 1];
		node->child[i+1] = node->child[i];
	}
	/* Ϊ����������µĹؼ��ֺ��ӽ�㣬���޸�nֵ */
	node->child[location + 1] = newnode;
	node->key[location] = childnode->key[MIN_T - 1];
	node->n = node->n + 1;
}//���Ѹ����node��λ��Ϊlocation���ӽ��������

void btree_insert_nonfull(BTreeNode node, int key)
{
	int i = node->n - 1;
	if(node->leaf)
	{
		/* �ý��ΪҶ�ӽ��ʱ���ҵ���Ӧλ�ã����ؼ��ֲ��룬���Խ��node�����޸� */
		while(i >=0 && key < node->key[i])
		{
			node->key[i+1] = node->key[i];
			i--;
		}
		node->key[i+1] = key;
		node->n = node->n + 1;
	}
	else
	{
		/* ��Ҷ�ӽ��ʱ�����Ҷ�Ӧ�ӽ�㣬�ж����Ƿ�Ϊ����㣬�ǣ�����ѣ���ݹ���� */
		while(i >=0 && key < node->key[i])
			i--;
		i++;
		if(node->child[i]->n == MAX_T - 1)
		{
			btree_split_child(node, i);
			if(key > node->key[i])
				i++;
		}
		btree_insert_nonfull(node->child[i], key);
	}
}//�Է����ڵ���в���ؼ���
void btree_insert(BTree tree, int key)/*
 * �����������в���ؼ���
 * ����Ϊ����ֻ��һ���ؼ��֣�������ʱ����Ҫ�����µĽ����Ϊ���ĸ���㣬
 * ����ԭ���ĸ������Ϊ�½����ӽ�㣬���з��Ѳ���
 * ����ֱ�ӽ��з������������
 */
{
	BTreeNode r_node = tree->root;
	if(r_node->n == MAX_T - 1)
	{
		BTreeNode r_node_new = allocate_node();

		r_node_new->leaf = 0;
		r_node_new->n = 0;
		r_node_new->child[0] = r_node;
		tree->root = r_node_new;
		btree_split_child(r_node_new, 0);
		btree_insert_nonfull(r_node_new, key);
	}
	else btree_insert_nonfull(r_node, key);
}
/*
 * ���½��Ľ��node�Ĺؼ��ָ���ΪT-1ʱ��
 * Ϊ�������½������У������Ľ��Ĺؼ��ָ������ڵ���T��
 * �Խ��parent��node��othernode�������Ĺؼ�����������
 * ��node��other���ʱ����node���ҽ��ʱ�����������ӽ�㣩�� * ��T+1λ�ã�����һ���ؼ��֣���ֵΪ������Ӧ�Ĺؼ���ֵ��
 * ��������Ӧ�ؼ���ֵ��ֵΪ���ӽ���еĵ�һ���ؼ��֡�
 * �����ӽ��Ĺؼ��ֺ��ӽ�㣨����еĻ�����ǰ�ƶ�һλ
 * �޸����ӽ���Լ��ý���nֵ
 */
void btree_left(BTreeNode parent, BTreeNode node, BTreeNode othernode, int location)
{
	int i = 0;
	node->key[node->n] = parent->key[location];
	parent->key[location] = othernode->key[0];
	for(i = 0; i <= othernode->n - 2; i++)
		othernode->key[i] = othernode->key[i + 1];
	if(!othernode->leaf)
	{
		node->child[node->n + 1] = othernode->child[0];
		for(i = 0; i <= othernode->n - 1; i++)
			othernode->child[i] = othernode->child[i + 1];
	}
	node->n = node->n + 1;
	othernode->n = othernode->n - 1;
}
/*
 * ���½��Ľ��node�Ĺؼ��ָ���ΪT-1ʱ��
 * Ϊ�������½������У������Ľ��Ĺؼ��ָ������ڵ���T��
 * �Խ��parent��node��othernode�������Ĺؼ�����������
 * ��node��other�Ҳ�ʱ����node������ʱ�����������ӽ�㣩��
 * node���Ĺؼ��ֺ��ӽ�㣨����еĻ�������ƶ�һλ,
 * �ڵ�һ��λ������һ���ؼ��֣���ֵΪ������Ӧ�Ĺؼ���ֵ��
 * ��������Ӧ�ؼ���ֵ��ֵΪ���ӽ���е����һ���ؼ��֡�
 * �޸����ӽ��͸ý���nֵ
 */
void btree_right(BTreeNode parent, BTreeNode node, BTreeNode othernode, int location)
{
	int i = 0;

	for(i = node->n - 1; i >= 0; i--)
		othernode->key[i+1] = othernode->key[i];
	node->key[0] = parent->key[location];
	parent->key[location] = othernode->key[othernode->n];

	if(!node->leaf)
	{
		node->child[0] = othernode->child[othernode->n + 1];
		for(i = othernode->n; i >= 0; i--)
			othernode->child[i + 1] = othernode->child[i];
	}
	node->n = node->n + 1;
	othernode->n = othernode->n - 1;
}
/*
 * �ϲ������ؼ��ָ���ΪT-1�����Ϊparentλ��Ϊlocation���ӽ��
 * �Ը�����Ӧ�Ĺؼ���Ϊ�м�ֵ���������ӽ��
 * ��������Ҫ�½����ӽ��λ��
 */
int btree_merge_child(BTreeNode parent, int location)
{
	int i;
	BTreeNode	lnode = NULL;
	BTreeNode	rnode = NULL;
	if(location == parent->n)
		location--;
	lnode = parent->child[location];
	rnode = parent->child[location + 1];
	/* ��������Ӧ�Ĺؼ����Լ����ֵ����еĹؼ��ָ��Ƹý�㣬ͬʱ�޸����ӵ�nֵ */
	lnode->key[lnode->n] = parent->key[location];
	for(i = 0; i < rnode->n; i++)
	{
		lnode->key[MIN_T + i] = rnode->key[i];
		lnode->n++;
	}
	/* ������ӽ��ͬ�����Ƶ��ý�� */
	if(!rnode->leaf)
		for(i = 0; i <= rnode->n; i++)
			lnode->child[MIN_T + i] = rnode->child[i];
	rnode->n= 0;
	lnode->n = MAX_T - 1;
	/* �Ը������Ӧ�Ĺؼ��ֺ��ӽ��λ�÷����仯 */
	for(i = location; i < parent->n - 1; i++)
	{
		parent->key[i] = parent->key[i + 1];
		parent->child[i + 1] = parent->child[i + 2];
	}
	/* ����������nֵ */
	parent->n = parent->n - 1;
	rnode = NULL;
	return location;
}
/*��Ҷ�ӽ��nodeλ��Ϊlocation�Ĺؼ���ɾ��
  ֱ�ӽ�λ��location��Ĺؼ�����ǰ�ƶ�һλ*/
void btree_delete_leaf(BTreeNode node, int location)
{
	int i = 0;
	for(i = location; i < node->n - 1; i++)
		node->key[i] = node->key[i + 1];
	node->n = node->n - 1;
}
/*ɾ���ò���������Ϊi�Ĺؼ���*/
int btree_delete_node_in(BTreeNode r_node, int i)
{
	BTreeNode lnode = r_node->child[i];
	BTreeNode rnode = r_node->child[i + 1];
	int temp = 0;
	/* ��ǰ�ڸ�λ�õĹؼ��ֵ����ӽ��ؼ��ָ������ڵ���Tʱ��
	 Ѱ�Ҹ�λ�õĹؼ���ǰ�������ӽ������ؼ��֣�
	*/
	if(lnode->n >= MIN_T)
	{
		BTreeNode newnode = btree_maximum(lnode);
		temp = r_node->key[i];
		r_node->key[i] = newnode->key[newnode->n - 1];
		newnode->key[newnode->n - 1] = temp;
	}
   //�෴�ģ������ӽ���������������Ѱ��̣������ӽ�����С�ؼ��֣�
	else if(rnode->n >= MIN_T)
	{
		BTreeNode newnode = btree_minimum(rnode);
		temp = r_node->key[i];
		r_node->key[i] = newnode->key[0];
		newnode->key[0] = temp;
		i++;
	}
   //�������ӽ�㶼��������������ϲ������ӽ��
	else	i = btree_merge_child(r_node, i);
	return i;
}
// ɾ����r_nodeΪ���������Ĺؼ���key
void btree_delete_node(BTreeNode r_node, int key)
{
	int i = 0;
	/* Ѱ�ҹؼ���λ�ã������½����ӽ��λ�� */
	while(i < r_node->n && key > r_node->key[i])
		i++;
	/* ���ٸò���ΪҶ�ӽ��ɾ����㣬�����½�Ѱ�ҽ��ɾ�� */
	if(i < r_node->n && key == r_node->key[i])
		if(r_node->leaf)
			{btree_delete_leaf(r_node, i);printf("ɾ���ɹ�\n");}
		else
		{
			i = btree_delete_node_in(r_node, i);
			btree_delete_node(r_node->child[i], key);
		}
	else
	{
		if(r_node->leaf)
			printf("���鲻����\n");
		else
		{
			if(r_node->child[i]->n >= MIN_T){
				btree_delete_node(r_node->child[i], key);}
			else
			{
				if(i > 0 && r_node->child[i - 1]->n >= MIN_T)
				{
					btree_right(r_node, r_node->child[i], r_node->child[i - 1], i);}
				else if(i < r_node->n && r_node->child[i + 1]->n >= MIN_T)
					btree_left(r_node, r_node->child[i], r_node->child[i + 1], i);
				else
					i = btree_merge_child(r_node, i);
				btree_delete_node(r_node->child[i], key);
			}
		}
	}
}
/*ɾ�����ڵĹؼ���key����������Ϊ�գ����滻�����*/
void btree_delete(BTree tree, int key)
{
 BTreeNode r_node = tree->root;
 btree_delete_node(r_node, key);
 if(tree->root->n == 0 && tree->root->leaf == 0)
   tree->root = tree->root->child[0];
}
int main()
{
    BTree tree = (BTree) malloc (BTREE_SIZE);
	tree->root	= (BTreeNode) malloc (BTREE_NODE_SIZE);
	btree_create(tree);
	FILE* fp = fopen("library.txt", "r");
	if (fp == NULL) {
		printf("Failed to open file");
	}
	int i = 0,a[100];
	for(i=0;!feof(fp);i++)fscanf(fp,"%d\n",&a[i]);
	a[i]=0;
	fclose(fp);
	for(i = 0; a[i]!=0; i++){
		btree_insert(tree, a[i]);
		}
    system("color F0");
    while(1)
    {
        system("cls");//��ս���
			printf(" ***********************************************\n");
			printf(" ^_^  ^_^  ^_^  ^_^  ^_^  ^_^  ^_^  ^_^  ^_^ ^_^\n");
			printf(" ***********************************************\n");
			printf(" ______________________________________________\n");
			printf("��                                            ��\n");
			printf("��               ͼ�����ϵͳ                 ��\n");
			printf("��____________________________________________��\n");
			printf("***********************************************\n");
			printf("***********************************************\n");
			printf("**              1.�������                   **\n");
			printf("**              2.��ѯͼ��                   **\n");
			printf("**              3.ͼ�����                   **\n");
            printf("**              4.�˳�ϵͳ                   **\n");
			printf("***********************************************\n");
			printf("***********************************************\n");
			printf("^_^  ^_^  ^_^  ^_^  ^_^  ^_^  ^_^  ^_^  ^_^ ^_^\n");
			printf("***********************************************\n");
			printf("               ѡ�������");
            int option,m,j;
            scanf("%d",&option);
           switch (option)
          {
           case 1:  {system("cls");
               printf("������������ӵ����:");scanf("%d",&m);
               btree_insert(tree, m);
               a[i]=m;i++;a[i]=0;
               printf("��ӳɹ�\n");
               system("pause");break;} //����ͼ��
           case 2: {system("cls");
               printf("������������ҵ����\n");scanf("%d",&m);
               btree_search(tree->root,m);
               system("pause");break;} //����ͼ��
           case 3: {system("cls");
               printf("����������ɾ�������\n");scanf("%d",&m);
               btree_delete(tree, m);
               for( j=0;a[j]!=m&&j<100;j++){}
               a[j]=-1;
               system("pause");break;} //ɾ��ͼ��
           case 4:{printf("��л����ʹ��");
           FILE* fp = fopen("library.txt", "w");
           for(i=0;a[i]!=0;i++)if(a[i]!=-1){fprintf(fp, "%d",a[i]);
		   fputc('\n', fp);}fclose(fp);exit(0);
           break;}//�˳�ʱ�����ļ�
           default:break;
          }}

    return 0;
}
