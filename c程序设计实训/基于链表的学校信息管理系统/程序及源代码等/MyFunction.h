FILE* creatFile(char* s,char* mode);//创建文件

void freeList(Node *head);//释放链表

void showMenu();//展示主菜单

void getInf(char* s,int n);//帮助读入数据

Linklist* read(char *s, char identity);//读取文件信息，创建链表

char getIdentity();//判断对学生还是老师

void writeFile(Node* head,FILE* fp);

int view(char identity,Node *head);//展示对应所有信息

void show(Node *node);//展示单个信息

int idTest(char* id,Node* head);//检验学号或工号是否长度合理并且唯一

void add(char identity,Linklist* link);//添加信息

int idTest(char* id,Node* head);//判断学号/工号是否字数不够或重复

int ageTest(int age,char identity);//判断添加或修改的年龄是不是不合实际

void addToList(Node *head,Linklist* link);//添加信息到链表

void delete(char identity,Linklist* link);//根据索引删除某条信息，可批量

void deleteNodeByIndex(Linklist* link,int index);//借助索引删除节点

void search(char who,int which,Linklist* link);//成功的话打印该信息

void numSearch(Node* head,int num,char who,int which);//以数字查询

void stringSearch(Node *head,char* input,char who,int which);//以字符串查询

void showSearchMenu(int who);//展示查询菜单

void change(char identity,Linklist* link);//改变信息，某个元素或多个元素

int sort_getChoice(char identity);//获取排序的标准

void sort(char identity,Linklist* link);//对信息排序

void strSort(Node* head, int choice, int subj);//字符串排序函数

void numSort(Node* head, int choice, int subj);//数字排序函数

char* getStringField(Node* node,int choice,int subj);//获取字符串字段

int getNumField(Node* node,int choice,int subj);//获取数值字段



























