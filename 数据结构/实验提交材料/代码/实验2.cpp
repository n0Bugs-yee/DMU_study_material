#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<iostream>

using namespace std;

#define MAXSIZE 100  // 最大串长
#define GETOUT 0 // 对应退出主菜单

typedef struct{
    char s[MAXSIZE+1];
    int length;
}SString;


enum{
     GETIF=1, BP, KMP, SUKMP 
};


void get_next(SString S, int next[], bool is_improve){
    next[0] = -1; // 运行过程中有可能变量对应为0
    int i, j;
    i = 1;
    next[1] = 0;
    j = 0;
    while(i < S.length){
        if(j==0 || S.s[i]==S.s[j]){
            i++;
            j++;
            if(is_improve && S.s[i]==S.s[j])
                next[i] = next[j];   
            else     
                next[i] = j;       
        }
        else
            j = next[j];
    }
}


 // 朴素的模式匹配算法（BF算法）
int my_bp(SString s1, SString s2, int pos){
    int i, j;
    i = pos;
    j = 1;
    int cir = 1;        // 比较趟数
    int total_cmp = 0;  // 总比较次数
    int this_round = 0; // 当前趟的比较次数

    cout << "\n    主串:";
    for(int i=1; i<s1.length+1; i++) // 输出主串
        cout << s1.s[i] << " ";  
    cout << "\n";


    while(i<=s1.length && j<=s2.length){

        if(0 == this_round){
            cout << "第" << cir << "趟匹配";
            for(int k=1; k<=(i-j)*2; k++) // 空格对齐
                cout << " ";
            if(cir>=10)
                cout << '\b'; // 防止cir>=10时出现的错位
        }

        this_round++;
        total_cmp++;

        cout << s2.s[j] << " ";
        if(s1.s[i] == s2.s[j]){
            i++;
            j++;
        }
        else{
            i = i-j+2;
            j = 1;
            cir++;
            cout << "本趟比较" << this_round << "次\n" ;
            this_round = 0;
        }
    }

    if(j >= s2.length){
        cout << "本趟比较" << this_round << "次" ;
        cout << "\n\n共 " << cir << " 趟，比较 " << total_cmp << " 次" << endl;
        cout << "匹配位置:" << i-s2.length << endl;
        cout << "------------------------------------------------------------------------------\n";
        return i-s2.length;
    }
    else{
        cout << "\n失配" << endl;
        cout << "------------------------------------------------------------------------------\n";
        return 0;
    }
}



 // KMP算法
int basic_kmp(SString s1, SString s2, int pos){
    int i, j;
    i = pos;
    j = 1;
    int cir = 1;        // 比较趟数
    int this_round = 0;  // 每趟比较次数
    int total_cmp = 0;  // 总比较次数
    int next[s2.length+1];
    get_next(s2, next, false); // 获取next数组

    cout << "\n  Next[]:";
    for(int i=1; i<s2.length+1; i++)
        cout << next[i] << " ";
    cout << "\n";

    cout << "\n    主串:";
    for(int i=1; i<s1.length+1; i++)
        cout << s1.s[i] << " ";
    cout << "\n";

    while(j<=s2.length && i<=s1.length){

        if(0 == this_round){
            cout << "第" << cir << "趟匹配";
            for(int k=1; k<=(i-j)*2; k++) // 空格对齐
                cout << ' ';
            if(cir>=10)
                cout << '\b';

            if(j>1){
                cout << "\b(";
                for(int i=1; i<j; i++)
                    cout << s2.s[i] << ' ';
                cout << "\b)";
            }
        }

        if(j != s2.length){
            this_round++;
            total_cmp++;
        }

        if(j != 0) // s2对应s[0]没意义
            cout << s2.s[j] << " ";

        if(j==0 || s1.s[i]==s2.s[j]){
            i++;
            j++;
        }
        else{
            cout << "本趟比较" << this_round << "次\n" ;
            j = next[j];
            cir++;
            this_round = 0;
        }
    }
    if(j >= s2.length){
        cout << "本趟比较" << this_round << "次" ;
        cout << "\n\n共 " << cir << " 趟，比较 " << total_cmp << " 次" << endl;
        cout << "匹配位置:" << i-s2.length << endl;
        cout << "------------------------------------------------------------------------------\n";
        return i-s2.length;

    }
    else{
        cout << "\n失配" << endl;
        cout << "------------------------------------------------------------------------------\n";
        return 0;
    }
}



 // KMP改进算法(NextVal[ ])
int improved_kmp(SString s1, SString s2, int pos){
    int i, j;
    i = pos;
    j = 1;
    int cir = 1;        // 比较趟数
    int this_round = 0;  // 每趟比较次数
    int total_cmp = 0;  // 总比较次数
    int next_val[s2.length+1];
    get_next(s2, next_val, true); // 获取nextval数组

    cout << "\nNextVal[]:";
    for(int i=1; i<s2.length+1; i++)
        cout << next_val[i] << " ";
    cout << "\n";

    cout << "\n    主串:";
    for(int i=1; i<s1.length+1; i++)
        cout << s1.s[i] << " ";
    cout << "\n";

    while(j<=s2.length && i<=s1.length){
        if(0 == this_round){
            cout << "第" << cir << "趟匹配";
            for(int k=1; k<=(i-j)*2; k++) // 空格对齐
                cout << ' ';
            if(cir>=10)
                cout << '\b';

            if(j>1) {
                cout << "\b(";
                for(int i=1; i<j; i++)
                    cout << s2.s[i] << ' ';
                cout << "\b)";
            }
        }

        if(j != s2.length){
            this_round++;
            total_cmp++;
        }

        if(j != 0) // s2对应s[0]没意义
            cout << s2.s[j] << " ";

        if(j==0 || s1.s[i]==s2.s[j]){
            i++;
            j++;
        }
        else{
            j = next_val[j];
            cir++;
            cout << "本趟比较" << this_round << "次\n" ;
            this_round = 0;
        }
    }
    if(j >= s2.length){
        cout << "本趟比较" << this_round << "次" ;
        cout << "\n\n共 " << cir << " 趟，比较 " << total_cmp << " 次" << endl;
        cout << "匹配位置:" << i-s2.length << endl;
        cout << "--------------------------------------------------------------------------\n";
        return i-s2.length;

    }
    else{
        cout << "\n失配" << endl;
        cout << "--------------------------------------------------------------------------\n";
        return 0;
    }
}




void show_menu(){
    cout << "\n1.输入主串、子串和匹配起始位置\n"
         << "2.朴素的模式匹配算法\n"
         << "3.KMP基础算法(Next[ ])\n"
         << "4.KMP改进算法(NextVal[ ])\n"
         << "0.退出管理系统" << endl;
    cout << "请选择0—4:" ;
}




void get_in(SString &s1, SString &s2, int &pos){
    string tem;
    cout << "请输入主串:";
    cin >> tem;
    s1.length = tem.length();
    for(int i=1; i<=s1.length; i++){
        s1.s[i] = tem[i-1];
    }
    s1.s[s1.length + 1] = '\0';
    cout << "请输入模式:";
    cin >> tem;
    s2.length = tem.length();
    for(int i=1; i<=s2.length; i++){
        s2.s[i] = tem[i-1];
    }
    s2.s[s2.length + 1] = '\0';
    cout << "请输入查找位置:";
    cin >> pos; 
    if(pos < 1){
        cout << "查找位置最小为1" << endl;
        return ; 
    }
}


// 请输入主串:ebababababcaababababcabadaaaa
// 请输入模式:babababcabad

int main(){
    int choice;
    show_menu();
    cin >> choice;
    SString s1, s2;
    int pos; 
    while(choice != GETOUT){
        if(choice<GETIF || choice>SUKMP){
            cout << "请选择正确的功能\n";
            show_menu();
            cin >> choice;
            continue;
        }
        switch(choice){
            case GETIF:{
                get_in(s1, s2, pos);
                break;
            }
            case BP:{
                my_bp(s1, s2, pos);
                break;
            }
            case KMP:{
                basic_kmp(s1, s2, pos);
                break;
            }
            case SUKMP:{
                improved_kmp(s1, s2, pos);
                break;
            }
        }
    show_menu();
    cin >> choice;
    }
    return 0;
}