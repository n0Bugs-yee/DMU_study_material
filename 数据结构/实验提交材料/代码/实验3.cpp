#include<iostream>
#include<stdlib.h>
#include<vector>

#define MAXSIZE 100  // 队列最大
#define OVERFLOW -1 // 申请空间失败
#define ERROR -2 // 对应空队列出队
#define GETOUT 0 // 对应退出主菜单

enum{ BUILD=1, DLR, LDR, LRD, BFS, PATH, DEPTH };

using namespace std;

 // 二叉树的定义
typedef struct  node{
	char data;	//数据域
	struct node *lchild ,  *rchild;  //左右孩子指针
}BinTNode, *BinTree;   //树中结点类型

 // 广度优先所借助的队列
typedef struct{
    BinTree *base; // 储存的是指针（指针的指针/指针的数组）
    int front;
    int rear;
}SqQueue;


void show_menu(){
    cout << endl;
    cout << "1.建立二叉树存储结构\n";
    cout << "2.求二叉树的先序遍历\n";
    cout << "3.求二叉树的中序遍历\n";
    cout << "4.求二叉树的后序遍历\n";
    cout << "5.求二叉树的层次遍历\n";
    cout << "6.求给定结点的路径\n";
    cout << "7.求树的深度\n";
    cout << "0.退出系统\n";
    cout << "请选择0—7:";
}

 // 按照DLR创建树
void build_tree(BinTree &T){
    char tem;
    std::cin >> tem;
    if(tem == '#') {
        T = nullptr;
        return ;
    }
    else{
        T = new BinTNode;
        T->data = tem;
        build_tree(T->lchild);
        build_tree(T->rchild);
    }
}

 // 前中后序遍历
void dlr(BinTree T){
    if(T){
        cout << T->data;
        dlr(T->lchild);
        dlr(T->rchild);
    }
}

void ldr(BinTree T){
    if(T){
        ldr(T->lchild);
        cout << T->data;
        ldr(T->rchild);
    }
}

void lrd(BinTree T){
    if(T){
        lrd(T->lchild);
        lrd(T->rchild);
        cout << T->data;
    }
}


 // 初始化广度优先搜索对应的队列
void init_queue(SqQueue &Q){
    Q.base = new BinTree[MAXSIZE]; 
    if( !Q.base){
        cout << "申请空间创建队列失败\n";
        exit(OVERFLOW);
    }
    Q.front = Q.rear = 0;
}


 // 入队函数 
void en_queue(SqQueue &Q, BinTree node){
    if((Q.rear+1)%MAXSIZE == Q.front){
        cout << "队列已满" << endl; 
        exit(OVERFLOW);
    }
    else{
        Q.base[Q.rear] = node;
        Q.rear = (Q.rear+1)%MAXSIZE;
    }
}


 // 判断队列是否为空
bool is_empty_queue(SqQueue Q){
    if(Q.front == Q.rear) return true;
    else return false;
}


 // 出队函数
void de_queue(SqQueue &Q, BinTree &univ){
    if( !is_empty_queue(Q)){
        univ = Q.base[Q.front];
        Q.front = (Q.front+1)%MAXSIZE;
    }
    else{
        cout << "队列为空，无法出队" << endl;
        univ = nullptr; 
    }
}


 // 广度优先搜索/层次遍历（树不需要visited数组）
void bfs_traverse(BinTree T){
    if( !T){
        cout << "树为空，退出广度遍历" << endl;
        return ;
    }
    
    SqQueue Q;
    init_queue(Q); // 初始化队列
    en_queue(Q, T); // 入队

    BinTree univ; // 出队结点对应的通用变量

    while( !is_empty_queue(Q)){
        de_queue(Q, univ);

        if(univ){
            cout << univ->data << " ";
            if(univ->lchild)
                en_queue(Q, univ->lchild);
            if(univ->rchild)
                en_queue(Q, univ->rchild);
        }
    }
    cout << endl;
}


 // 找到根到某节点间的路径
bool root_bitree_path(BinTree path[] ,BinTree T, char target, int &index){
    if( !T)
        return  false;

    path[index++] = T; 

    if(T->data == target)
        return true;

    if(root_bitree_path(path, T->lchild, target, index)) // 左子树
        return true;
    
    if(root_bitree_path(path,T->rchild, target, index)) // 右子树
        return true;
    

    index--; // 未查找到则返回覆盖已记录的地址
    return false;
}


 // 返回树的深度
int bitree_depth(BinTree T){
    if( !T) return 0;
    else{
        int m = bitree_depth(T->lchild); // 递归左子树深度
        int n = bitree_depth(T->rchild); // 递归右子树深度
        return  m>n? m+1:n+1; 
    }
}


void destroy_tree(BinTree &T){
    if(T){
        destroy_tree(T->lchild);
        destroy_tree(T->rchild);
        delete T;
        T = nullptr;
    }
}

 // ABC##DE#G##F###

int main(){
    int choice;
    show_menu();
    cin >> choice;
    BinTree T = nullptr;
    while(choice != GETOUT){
        if(choice>DEPTH || choice<BUILD){
            cout << "\n请选择正确的功能\n\n";
            show_menu();
            cin >> choice;
            continue;
        }
        else if(choice!=BUILD && T==nullptr)
            cout << "\n请先建立二叉树(选择 1)\n\n";
        else{
            switch(choice){
                case BUILD:{
                    cout << "请输入二叉树的先序序列(用#表示空节点): ";
                    build_tree(T);
                    break;
                }
                case DLR:{
                    dlr(T);
                    cout << endl;
                    break;
                }
                case LDR:{
                    ldr(T);
                    cout << endl;
                    break;
                }
                case LRD:{
                    lrd(T);
                    cout << endl;
                    break;
                }
                case BFS:{
                    bfs_traverse(T);
                    break;
                }
                case PATH:{
                    BinTree path[MAXSIZE];
                    int index = 0;
                    char target;
                    cout << "请输入查找的结点对应的字符：";
                    cin >> target;
                    if(root_bitree_path(path, T, target, index)){
                        cout << "根到所找节点路径为：";
                        for(int i=0; i<index; i++){
                            cout << path[i]->data;
                            if(i < index-1) 
                                cout << " -> ";
                        }
                        cout << '\n';
                    } 
                    else
                        cout << "未找到该节点" << endl;
                    break;
                }
                case DEPTH:{
                    cout << "数的深度为:" << bitree_depth(T) << endl;
                    break;
                }
            }
        }
    show_menu();
    cin >> choice;
    }
    destroy_tree(T);
    return 0;
}
