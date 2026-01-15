
void showMenu();//展示主菜单

FILE* creatFile(char* s,char* mode);//创建文件

void getInf(char* s,int n);//帮助读入数据

void freeAssist(Assist* assist);//释放内存

void view(char identity);//展示对应所有信息

Assist* read(char* s,char identity);//用于计算数据总数和读取和提取数据内容

void show(Assist* assist,int i);//展示某个具体信息

void add(char identity);//添加信息，批量或单个

void ageSearch(Assist *s_Ass,Assist *t_Ass,int who,int age);//以年龄查询

void search(int who,int which);//成功的话打印该信息

void stringSearch(Assist *s_Ass,Assist *t_Ass,int who,int which,char* input);//以字符串查询

void genderSearch(Assist *s_Ass,Assist *t_Ass,int who,char gender);//以性别查询

void showSearch(Assist *s_Ass,Assist *t_Ass,int *storS,int *storT,int numS,int numT,int who);//展示查询结果

char getIdentity();//判断是对学生还是老师

void showSearchMenu(int who);//展示查询菜单

void writeFile(Assist* assist,FILE* fp);//将Assist内容写入fp对应文件

void change(char identity);//改变信息，某个元素或多个元素

int idTest(char* id);//判断学号/工号是否字数不够或重复

int ageTest(int age,char identity);//判断添加或修改的年龄是不是不合实际

void delete(char identity);//根据索引删除某条信息，可批量

int sort_getChoice(char identity);//获取排序的标准

void sort(char identity);//对信息排序

void strSort(Assist* assist,int choice,int subj);//字符串排序函数

void numSort(Assist* assist,int choice,int subj);//数字排序函数

int addTeacher(int addNum);//添加老师信息

int addStudent(int addNum);//添加学生信息































