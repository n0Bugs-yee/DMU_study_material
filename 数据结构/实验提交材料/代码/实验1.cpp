#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>

#define GETOUT 0 // 对应退出主菜单

using namespace std;

enum{
    BUILD=1, EXP, FIND, UPDATEH, ADD, FIRSTH, MOVEK1, REVERSEN2, SORTPRICE, UPBED, MERGE, 
};

typedef struct HNode{
	char roomN[7];	//客房名称
	float Price;		//标准价格
	float PriceL;	//入住价格(默认值=标准价格*80%)
	int Beds;		//床位数Beds
	char State[5];	//入住状态(值域："空闲"、"入住"、"预订"，默认值为"空闲")
	struct HNode *next;	//指针域
}Hotel, *HLink;



void Build(HLink &H, const char* filename){
    FILE* fp = fopen(filename, "r");
    if( !fp){
        cout << "文件打开错误，程序退出\n" << endl;
        exit(-1);
    }

    H = new Hotel;
    H->next = nullptr;

    Hotel* h = H;
    while(true){
        Hotel* temp = new Hotel;
        temp->next = nullptr;
        //读取文件信息
        if(fscanf(fp, "%s %f %d", temp->roomN, &temp->Price, &temp->Beds) == 3){
            temp->PriceL = temp->Price * 0.8; // 入住价格=标准价格*80%
            strcpy(temp->State, "空闲"); //默认状态
            // 尾插
            h->next = temp;
            h = temp;
        }
        else{
            delete temp;
            break;
        }
    }
    fclose(fp);
}



void Exp(HLink H){
    Hotel* tem = H->next;
    cout<<"客房信息\n"<<endl;
    while(tem){
        cout << "房间名称:" << tem->roomN 
             << ", 标准价格:" << tem->Price 
             << ", 入住价格:" << tem->PriceL
             << ", 床位数:" << tem->Beds 
             << ", 状态:" << tem->State << endl;
        tem = tem->next;
    }
}



int Find(HLink &H, char *roomN){
    Hotel* tem = H->next;
    int position = 1;
    while(tem){
        if( !strcmp(tem->roomN, roomN))
            return position;
        tem = tem->next;
        position++;
    }
    return 0;
}



 // 将床位数为beds的客房入住状态改为state
void updateH(HLink &H, int beds, char* state){
    Hotel* tem = H->next;
    while(tem){
        if(tem->Beds == beds)
            strcpy(tem->State, state);
        tem = tem->next;
    }
}



 // 将该链表中未入住的客房入住价格均加价20%
void Add(HLink &H){
    Hotel* tem = H->next;
    while(tem){
        if( !strcmp(tem->State, "空闲"))
            tem->PriceL *= 1.2f;
        tem = tem->next;
    }
}



 // 该函数内return语句返回入住价格最高的客房结点指针，返回前将该结点在链表中删除
HLink FirstH(HLink &H){
    if(!H->next){// 空链表处理
        cout << "链表为空，返回空指针";
        return nullptr;
    } 
    Hotel* temp = H->next;
    Hotel* max = temp;
    Hotel* prior = H;
    Hotel* max_prior = H;
    while(temp){ // 找到最大入住价格对应结点
        if(temp->PriceL > max->PriceL){
            max = temp;
            max_prior = prior;
        }
        prior = temp;
        temp = temp->next;
    }
    max_prior->next = max->next;
    return max;
} 



// "快慢指针法"

// 将单链表中倒数第k个结点之后的所有结点移到头结点后面
void MoveK1(HLink &H, int k){ 
    HNode* fast = H->next;
    HNode* slow = H->next;
    // 对空链表和不合法k的处理
    if(k <= 0){
        cout << "移动结点个数不能为负";
        return ;
    }
    if( !H->next){
        cout << "空表无法移动";
        return ; 
    } 
       
    for(int i=0; i<k; i++){
        if( !fast){
            cout << "移动个数大于表长，无法操作";
            return ; 
        }
        fast = fast->next;
    }
    // 若判断条件为fast使得slow对应倒数k-1
    while(fast->next){ 
        // 快慢指针一起后移到尾,此时slow指针对应倒数第k个结点
        fast = fast->next; 
        slow = slow->next; 
    }
    if( !slow->next){// 没有移动元素
        cout << "仅一个节点无需移动";
        return ;
    }    
    HNode* tem = slow->next;
    slow->next = nullptr; // 断开节点
    while(tem){ // 依次头插
        HNode* after = tem->next;  
        tem->next = H->next;
        H->next = tem;
        tem = after;
    }
}



// 将单链表的正中间位置结点之后的全部结点倒置 fast移动两个位置对应slow移动1个位置
void ReverseN2(HLink &H){ 
    HNode* fast = H->next;
    HNode* slow = H->next;
    // 空表和单节点的处理
    if( !fast){
        cout << "空表无法倒置";
        return ;
    } 
        
    if( !fast->next){
        cout << "单节点无需操作";
        return ;
    }
    while(fast && fast->next){ // 每次移动两个位置，若仅用fast或fast->next做判断会出现引用空指针
        fast = fast->next->next;
        slow = slow->next; // slow最终对应中间节点
    }
    // 反转slow->next开始的部分，无头结点的头插
    HNode* part = nullptr; 
    HNode* curr = slow->next;
    slow->next = nullptr; // 断开
    while(curr){
        HNode* aft = curr->next;
        curr->next = part;
        part = curr;
        curr = aft;
    }
    // 连接反转后的部分
    slow->next = part;
}


 // 按照客房（入住价格，客房名称）升序排序
void SortPrice(HLink &H){ 
    if( !H->next){
        cout << "空表无法排序";
        return ;
    } 
        
    if( !H->next->next){
        cout << "单节点无需操作";
        return ;
    }
    HNode* p = H;
    HNode* q = H->next->next;
    p->next->next = nullptr; // 拆出对应排序的部分最初p为1个结点，找q插在p后面
    while(q){
        while((p->next && ((p->next->PriceL < q->PriceL) || 
        (p->next->PriceL == q->PriceL && strcmp(p->next->roomN, q->roomN)<0)))){
            p = p->next;  // 定位到尾结点或找到p对应下一结点值大于q对应值（此时该将q插到p后）
        }
        HNode* tem = q->next; // 储存q的下一结点位置，在插入q时其next会被覆盖
        q->next = p->next; // 将q插到p后
        p->next = q;
        q = tem; // q对应后移
        p = H; // 对p进行重定位
    }
} 


// 创建一个新结点，接入特定位置
void upBed(HLink &H, int beds){
    Hotel* h = new Hotel;
    h->next = nullptr;
    h->Beds = beds;
    strcpy(h->State, "空闲");
    cout << "价格:";
    cin >> h->Price;
    h->PriceL = h->Price * 0.8;
    cout << "房间名称:";
    cin >> h->roomN;
    // 接入特定位置
    Hotel* att = H;
    Hotel* cur = H->next;
    if( !cur){ 
        cout << "链表为空，已接入头节点后" << endl;
        H->next = h;
        return ;
    }
    Hotel* dex = h;
    while(cur){
        Hotel *tem = cur->next;
        if(cur->Beds<=beds){
            cur->next = nullptr;
            dex->next = cur;
            dex = cur;
        }
        else{
            att->next = cur;
            att = cur;
        }        
        cur = tem;
    }
    att->next = h;
}


// 将两个按入住价格非递减排序的客房记录单合并为一个按入住价格非递增排序的客房记录
void Merge(HLink &H1, HLink &H2, HLink &Hc){ 
    Hotel* h1 = H1->next;
    Hotel* h2 = H2->next;
    Hc = H1;
    Hc->next = nullptr;
    Hotel* insert = nullptr;
    while(h1 && h2){
        if(h1->PriceL >= h2->PriceL){
            insert = h2;
            h2 = h2->next;
        }
        else{
            insert = h1;
            h1 = h1->next;
        }
        insert->next = Hc->next;
        Hc->next = insert;
    }
    while(h1){
        insert = h1;
        h1 = h1->next;
        insert->next = Hc->next;
        Hc->next = insert;
    }
    while(h2){
        insert = h2;
        h2 = h2->next;
        insert->next = Hc->next;
        Hc->next = insert;
    }
    delete H2;
}



void show_menu(){
    cout << "1.创建链表\n"
         << "2.展示链表\n"
         << "3.按名称查找房间\n"
         << "4.改变特定床位数的客房的入住状态\n"
         << "5.未入住房间加价20%\n"
         << "6.按入住价格查找价格最高房间并删除\n"
         << "7.移动K个到头结点后\n"
         << "8.将中间节点后所有节点移动到中间节点前\n"
         << "9.按入住价格和名称排序\n"
         << "10.创建特定床位数的房间插入特定位置\n"
         << "11.合并两个非递减链表为非递增形式\n"
         << "0.退出\n";
    cout << "请选择0-11:" ;
}


// 释放链表内存的函数
void destroy_hLink(HLink &H){
    if(!H) return;  // 如果已经是空指针，直接返回

    HNode *p = H;
    while(p){
        HNode *temp = p;
        p = p->next;
        delete temp;
    }
    H = nullptr;
}


int main(){
    char* filename_1 = (char*)"experiment_1_dataset_1.txt.txt"; // 指定存储路径
    char* filename_2 = (char*)"experiment_1_dataset_2.txt";
    HLink hotel = nullptr;
    HLink new_hotel = nullptr; // 用作合并链表的表头
    cout << "\n请选择功能" << endl;
    int choice;
    show_menu();
    cin >> choice;
    while(choice != GETOUT){
        if(choice>MERGE || choice<BUILD){
            cout << "请选择正确的功能\n";
            show_menu();
            cin >> choice;
            continue;
        }
        switch(choice){
            case BUILD:{
                Build(hotel, filename_1);
                break;
            }
            case EXP:{
                Exp(hotel);
                break;
            }
            case FIND:{
                cout << "请输入房间名称:";
                char name[7];
                cin >> name;
                int find = Find(hotel, name); 
                if(find){
                    cout << find << endl;
                }   
                break;
            }
            case UPDATEH:{
                cout << "输入修改的房间床位:";
                int index;
                cin >> index;
                cout << "输入对应状态：";
                char str[20];
                cin >> str;
                updateH(hotel, index, str);    
                Exp(hotel);
                break;
            }
            case ADD:{
                Add(hotel);
                Exp(hotel);
                break;
            }
            case FIRSTH:{
                Hotel * t = FirstH(hotel);
                cout << t->roomN << endl;
                Exp(hotel);
                delete t;
                break;
            }
            case MOVEK1:{
                cout << "请输入要移动的房间数目:" ;
                int k;
                cin >> k;
                MoveK1(hotel, k);
                Exp(hotel);
                break;
            }
            case REVERSEN2:{
                ReverseN2(hotel);
                Exp(hotel);
                break;
            }
            case SORTPRICE:{
                SortPrice(hotel);
                Exp(hotel);
                break;
            }
            case UPBED:{
                cout << "\n请按提示输入房间信息" << endl;
                cout << "房间床位数:";
                int beds;
                cin >> beds;
                upBed(hotel, beds);
                break;
            }
            case MERGE:{
                HLink hot; 
                Build(hot, filename_2);
                cout << "\n展示合并前的两组数据\n";
                Exp(hotel);
                Exp(hot);
                Merge(hotel, hot, new_hotel);
                cout << "展示合并后的数据\n";
                Exp(new_hotel);
            }
        }
    cout << "\n请选择功能" << endl;
    show_menu();
    cin >> choice;
    }
    destroy_hLink(hotel);
    destroy_hLink(new_hotel);
    return 0;
}


