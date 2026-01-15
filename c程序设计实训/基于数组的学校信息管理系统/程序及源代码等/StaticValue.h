#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>

//用作判断ID是否合理
#define ID_LENGTH 10//ID有效长度
#define ID_GOOD 0//ID合理且唯一
#define ID_SIZE_WRONG 1//ID长度不合理
#define ID_NOT_UNIQUE 2//ID不唯一

//用作判断年龄是否合理
#define AGE_GOOD 0//年龄合理
#define AGE_NEG 1//年龄为负数
#define AGE_YOUNG 2//年龄过小
#define AGE_OLD 3//年龄过大
#define AGE_T_MIN 18//老师的最小年龄
#define AGE_S_MIN 9//学生的最小年龄
#define AGE_MAX 65//最大年龄

//改变信息所对应的常量
#define CHANGE_MAX 10//单次最多改变元素个数
#define DELETE_MAX 50//单次最多删除个数
#define BUFFER_MAX 256//规定储存索引的字符数组大小

#define OBJECT_NUM 3//课程个数
#define OBJECT_MAX 16//课程名称长度

//改变课程或者成绩对应索引
#define FIRST_INDEX 0//第一门课程对应索引
#define SECOND_INDEX 1
#define THIRD_INDEX 2

//结构体中数组对应大小
#define OBJECT_MAX 16//课程名称最大长度
#define MAJOR_MAX 32//专业名称最大长度
#define NAME_MAX 32//姓名最大长度
#define ID_MAX 16//ID最大储存长度

//文件名常量
#define stuFile "Student.dat"//学生对应文件
#define teaFile "Teacher.dat"//老师对应文件


typedef struct class{//课程的结构体
	char object[OBJECT_MAX];//科目
	unsigned int score;//分数
} Class;


typedef struct student{//学生的结构体
	int age;//年龄
	char gender;//性别
	char major[MAJOR_MAX];//专业
	char name[NAME_MAX];//姓名
	char s_id[ID_MAX];//学号
	Class cl[OBJECT_NUM];//课程
} Student;

typedef struct teacher{//老师的结构体
	int age;//年龄
	char gender;//性别
	char major[MAJOR_MAX];//专业
	char name[NAME_MAX];//姓名
	char t_id[ID_MAX];//工号
	char object[OBJECT_NUM][OBJECT_MAX];//课程
} Teacher;

typedef struct assist{//将老师与学生联系在一起的辅助结构体
    char identity;//身份
    int n;//数目
    union{//联合体，学生和老师只能为其中之一
        Student *stu;
        Teacher *tea;
    }data;
} Assist;

enum{//主函数中删减等功能对应的枚举常量
	ADD=1,SEARCH,CHANGE,DELETE,VIEW,SORT
};

enum{//改变对应索引
	AGE=1,MAJOR,NAME,OBJECT,SCORE,GENDER,ID
};

enum{//写入或改变课程和成绩时对应选择
	FIRST=1,SECOND,THIRD
};

enum{//排序对应索引
	SORT_AGE=1,SORT_MAJOR,SORT_NAME,SORT_OBJECT,SORT_GENDER,SORT_ID,SORT_SCORE
};



















