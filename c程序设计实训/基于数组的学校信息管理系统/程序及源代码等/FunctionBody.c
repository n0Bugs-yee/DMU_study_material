#include "StaticValue.h"
#include "MyFunction.h"

void showMenu(){//展示主菜单
    printf("\n");
    printf("+-------------------------------------------------------------+\n");
    printf("|                  MANAGEMENT SYSTEM MENU                     |\n");
    printf("+----------------------------+--------------------------------+\n");
    printf("|  1. Add Record             |  2. Search Records             |\n");
    printf("|  3. Change Record          |  4. Delete Record              |\n");
    printf("|  5. View Records           |  6. Sort Records               |\n");
    printf("+----------------------------+--------------------------------+\n");
    printf("| Enter Choice (1-6, any other key to quit):                  |\n");
    printf("+-------------------------------------------------------------+\n");

}


FILE* creatFile(char* s,char* mode){//创建文件
	FILE *fp=fopen(s,mode);
	if(!fp){
		printf("Error On Opening The File\n");
		exit(0);
	}
	return fp;
}


void freeAssist(Assist* assist){//释放申请内存
    if(assist){
    	if(assist->identity=='S') 
			free(assist->data.stu);
       	else if(assist->identity=='T') 
				free(assist->data.tea);
        free(assist);
    }
}


Assist* read(char *s,char identity){//记录数据个数和读出数据
	FILE* fp=creatFile(s,"r");
	int numOfInf=0;//记录数据个数
	char buffer[BUFFER_MAX];
	Assist *assist=malloc(sizeof(Assist));
	while(fgets(buffer,sizeof(buffer),fp)!=NULL)//计数
		numOfInf++;
	rewind(fp);
	assist->n=numOfInf;
	if(identity=='T'){
		assist->data.tea=malloc(numOfInf*sizeof(Teacher));
		assist->identity='T';
		for(int i=0;i<numOfInf;i++){
			fgets(buffer,sizeof(buffer),fp);
			sscanf(buffer,"%c %d %s %s %s %s %s %s",&assist->data.tea[i].gender,&assist->data.tea[i].age,assist->data.tea[i].major,assist->data.tea[i].name,assist->data.tea[i].t_id,assist->data.tea[i].object[FIRST_INDEX],assist->data.tea[i].object[SECOND_INDEX],
										assist->data.tea[i].object[THIRD_INDEX]);
		}
	}
	if(identity=='S'){
		assist->data.stu=malloc(numOfInf*sizeof(Student));
		assist->identity='S';
		for(int i=0;i<numOfInf;i++){
			fgets(buffer,sizeof(buffer),fp);
			sscanf(buffer,"%c %d %s %s %s %s %d %s %d %s %d",&assist->data.stu[i].gender,&assist->data.stu[i].age,assist->data.stu[i].major,assist->data.stu[i].name,assist->data.stu[i].s_id,assist->data.stu[i].cl[FIRST_INDEX].object,&assist->data.stu[i].cl[FIRST_INDEX].score,assist->data.stu[i].cl[SECOND_INDEX].object,&assist->data.stu[i].cl[SECOND_INDEX].score,
										assist->data.stu[i].cl[THIRD_INDEX].object,&assist->data.stu[i].cl[THIRD_INDEX].score);
		}
	}
	fclose(fp);
	return assist;
}


void show(Assist* assist,int i){//展示单个具体信息
	if(assist->identity=='S'){
		Student* s=&assist->data.stu[i];
		printf("No.%-4d %-6c %-4d %-16s %-16s %-16s",i+1,s->gender,s->age,s->major,s->name,s->s_id);
		printf("\n  Subjects: ");
        for(int j=0;j<OBJECT_NUM;j++){
            printf("%-10s:%-4d   ",s->cl[j].object,s->cl[j].score);
        }
        putchar('\n');
	}
	if(assist->identity=='T'){
		 Teacher* t=&assist->data.tea[i];
        printf("No.%-4d %-6c %-4d %-16s %-16s %-16s",i+1,t->gender,t->age,t->major,t->name,t->t_id);
        printf("\n  Teaching: ");
        for(int j=0;j<OBJECT_NUM;j++){
            printf("%-16s  ",t->object[j]);
        }
        putchar('\n');
    }
    printf("----------------------------------------------------------------\n");
}


void view(char identity){//展示信息
	Assist *assist;
	char* fileName=(identity=='T')?teaFile:stuFile;
	assist=read(fileName,identity);
	if(assist->n!=0){
		puts("\n\n");
		printf("================================================================\n");
        printf("%-6s  %-6s %-4s %-16s %-16s %-16s\n", 
               "No.", "Gender", "Age", "Major", "Name", "ID");
        printf("================================================================\n");
        for(int i=0;i<assist->n;i++){
            show(assist,i);
        }
        printf("Total Records: %d\n",assist->n);
    }
	else   printf("No Information\n");
	freeAssist(assist);
}


int idTest(char* id){//检验学号或工号是否长度合理并且唯一
	if(strlen(id)!=ID_LENGTH)//先判断数目再判断内容
		return ID_SIZE_WRONG;
	Assist* assist=read(teaFile,'T');
	for(int i=0;i<assist->n;i++){
		if(!strcmp(assist->data.tea[i].t_id,id)){
			freeAssist(assist);
			return ID_NOT_UNIQUE;
		}
	}
	assist=read(stuFile,'S');
	for(int i=0;i<assist->n;i++){
		if(!strcmp(assist->data.stu[i].s_id,id)){
			freeAssist(assist);
			return ID_NOT_UNIQUE;
		}
	}
	freeAssist(assist);
	return ID_GOOD;
}


int ageTest(int age,char identity){//判断更改和输入的年龄是否合理
	if(age<0) return AGE_NEG;
	if(age<AGE_T_MIN&&identity=='T')
		return AGE_YOUNG;
	if(age<AGE_S_MIN&&identity=='S')
		return AGE_YOUNG;
	if(age>AGE_MAX) return AGE_OLD; 
	return AGE_GOOD;
}


int addTeacher(int addNum){//添加老师信息
	FILE* fp=creatFile(teaFile,"a");
	int success=addNum;
	printf("e.g.M 20 Computer 王宁 2220241385 Math Chinese English\n");
	for(int i=0;i<addNum;i++){
		Teacher t;
		if(scanf(" %c %d %s %s %s %s %s %s",&t.gender,&t.age,t.major,t.name,t.t_id,
			t.object[FIRST_INDEX],t.object[SECOND_INDEX], t.object[THIRD_INDEX])!=8){
			printf("\nNo.%-3d: Invalid Input Format. Record Skipped.\n",i+1);
			success--;
       		while(getchar()!='\n');
           	continue;
		}//跳过此次
		getchar();//清除换行符
		int testI=idTest(t.t_id);
		if(testI!=ID_GOOD){
			char *tip=(testI==ID_SIZE_WRONG)?"Invalid ID Length. Record Skipped.\n":
				"The Id Already Exists,Record Skipped.\n";
			success--;
			printf("\nNo.%-3d: %s",i+1,tip);
			continue;
		}
		int testA=ageTest(t.age,'T');
		if(testA!=AGE_GOOD){
			char* tip=(testA==AGE_NEG)?"Age Can't Be Negative. Record Skipped.\n":((testA==AGE_YOUNG)?"Too Young. Record skipped.\n":
				"Too Old.  Record Skipped.\n");
				success--;
				printf("\nNo.%-3d: %s",i+1,tip);
				continue;
		}
		fprintf(fp,"%c %d %s %s %s %s %s %s\n",toupper(t.gender),t.age,t.major,t.name,t.t_id,
                t.object[FIRST_INDEX], t.object[SECOND_INDEX], t.object[THIRD_INDEX]);
	}
	fclose(fp);
	return success;
}


int addStudent(int addNum){//添加学生信息
	FILE* fp=creatFile(stuFile,"a");
	int success=addNum;
	printf("e.g.M 20 Computer 王宁 2220241385 Math 59 Chinese 59 English 59\n");
	for(int i=0;i<addNum;i++){
		Student s; 
		if(scanf(" %c %d %s %s %s %s %d %s %d %s %d",&s.gender,&s.age,s.major,s.name,s.s_id,s.cl[FIRST_INDEX].object,&s.cl[FIRST_INDEX].score,
             s.cl[SECOND_INDEX].object, &s.cl[SECOND_INDEX].score,s.cl[THIRD_INDEX].object,&s.cl[THIRD_INDEX].score)!=11) {
			printf("\nNo.%-3d: Invalid Input Format. Record Skipped.\n",i+1);
		    success--;
		    while(getchar()!='\n');
		    continue;
		}
		getchar(); //清除换行符
		int testI=idTest(s.s_id);
		if(testI!=ID_GOOD){
			char *tip=(testI==ID_SIZE_WRONG)?"Invalid ID Length. Record Skipped.\n":
				"The Id Already Exists,Record Skipped.\n";
			success--;
			printf("\nNo.%-3d: %s",i+1,tip);
			continue;
		}
		int testA=ageTest(s.age,'S');
		if(testA!=AGE_GOOD){
			char* tip=(testA==AGE_NEG)?"Age Can't Be Negative. Record Skipped.\n":((testA==AGE_YOUNG)?"Too Young. Record Skipped.\n":
				"Too Old.  Record Skipped.\n");
			success--;
			printf("\nNo.%-3d: %s",i+1,tip);
			continue;
		}
		fprintf(fp,"%c %d %s %s %s %s %d %s %d %s %d\n",toupper(s.gender),s.age,s.major,s.name,s.s_id,
		     s.cl[FIRST_INDEX].object,s.cl[FIRST_INDEX].score,s.cl[SECOND_INDEX].object,s.cl[SECOND_INDEX].score,s.cl[THIRD_INDEX].object,s.cl[THIRD_INDEX].score);
	}
	fclose(fp);
	return success;
}


void add(char identity){//添加信息
	printf("How Many Pieces Of Information To Add?");
	int addNum,success;
	if(scanf("%d",&addNum)==1&&addNum>0&&getchar()=='\n'){
		printf("Please Enter %d Pieces Of Information:\n",addNum);
		if(addNum>1)
			printf("Use [ENTER] To Divide\n");
		if(identity=='T')
			success=addTeacher(addNum);
		if(identity=='S')
			success=addStudent(addNum);
    	printf("||Successfully Added %d Records||\n\n",success);
    }
	else{
		printf("The Count Of Information Can't Be Negative\n");
		while(getchar()!='\n');
		return  ;
	}
}


void getInf(char* s,int n){//帮助读入数据
	if(fgets(s,n,stdin)){
		char *re=strchr(s,'\n');
		if(re){
			*re='\0';
		}
	}
	else{
		while(getchar()!='\n');
		*s='\0';
	}
}


void ageSearch(Assist *s_Ass,Assist *t_Ass,int who,int age){//以年龄查询
	int *storS=NULL;
	int *storT=NULL;
	if(who=='S'||who=='A'){//保证不申请无用的内存
        storS=malloc(s_Ass->n*sizeof(int));
    }
    if(who=='T'||who=='A'){
        storT=malloc(t_Ass->n*sizeof(int));
    }
	int numS,numT;
	numS=numT=0;
	if(storS){
		for(int i=0;i<s_Ass->n;i++){
			if(s_Ass->data.stu[i].age==age)
				storS[numS++]=i;
		}
	}
	if(storT){
		for(int i=0;i<t_Ass->n;i++){
			if(t_Ass->data.tea[i].age==age)
				storT[numT++]=i;
		}
	}
	showSearch(s_Ass,t_Ass,storS,storT,numS,numT,who);
	free(storS);
	free(storT);
}


void showSearch(Assist *s_Ass,Assist *t_Ass,int *storS,int *storT,int numS,int numT,int who){//展示查询结果
	if(who=='T'||who=='A'){
		if(numT==0){
			printf("No Teachers' Data\n");
			if(who!='A')
				return  ;
		}
		for(int i=0;i<numT;i++)
			show(t_Ass,storT[i]);
		printf("Teachers' Total Record: %d\n\n",numT);
	}
	if(who=='S'||who=='A'){
		if(numS==0){
			printf("No Students' Data\n");
			if(who!='A')
				return  ;
		}
		for(int i=0;i<numS;i++)
			show(s_Ass,storS[i]);
		printf("Students' Total Record: %d\n\n",numS);
	}
	if(who=='A')
		printf("All Total Record: %d\n",numS+numT);
}


void genderSearch(Assist *s_Ass,Assist *t_Ass,int who,char gender){//以性别查询
	int *storS=NULL;
	int *storT=NULL;
	if(who=='S'||who=='A'){
        storS=malloc(s_Ass->n*sizeof(int));
    }
    if(who=='T'||who=='A'){
        storT=malloc(t_Ass->n*sizeof(int));
    }
	int numS,numT;
	numS=numT=0;
	if(storS){
		for(int i=0;i<s_Ass->n;i++){
			if(s_Ass->data.stu[i].gender==gender)
				storS[numS++]=i;
		}
	}
	if(storT){
		for(int i=0;i<t_Ass->n;i++){
			if(t_Ass->data.tea[i].gender==gender)
				storT[numT++]=i;
		}
	}
	showSearch(s_Ass,t_Ass,storS,storT,numS,numT,who);
	free(storS);
	free(storT);
}


void stringSearch(Assist *s_Ass,Assist *t_Ass,int who,int which,char* input){//以字符串查询
	int *storS=NULL;
	int *storT=NULL;
	if(who=='S'||who=='A'){
        storS=malloc(s_Ass->n*sizeof(int));
    }
    if(who=='T'||who=='A'){
        storT=malloc(t_Ass->n*sizeof(int));
    }
	int numS,numT;
	numS=numT=0;
	if(who=='S'||who=='A'){
        for(int i=0;i<s_Ass->n;i++){
            char *fieldValue=NULL;
            switch (which){
                case MAJOR:{
                	fieldValue=s_Ass->data.stu[i].major;
					break;
				}
                case NAME:{
                	fieldValue=s_Ass->data.stu[i].name;
					break;
				}
                case ID:{
                	fieldValue=s_Ass->data.stu[i].s_id;
					break;
				}
				case OBJECT:{
					 for(int j=0;j<OBJECT_NUM;j++){
                        if(!strcasecmp(s_Ass->data.stu[i].cl[j].object,input)){
                            fieldValue=input;
                            break;
                        }
                    }
					break;
				}
            }
            if(!strcasecmp(fieldValue,input))
                storS[numS++]=i;
        }
    }
     if(who=='T'||who=='A'){
        for(int i=0;i<t_Ass->n;i++){
            char *fieldValue=NULL;
            switch (which){
                case MAJOR:{
                	fieldValue=t_Ass->data.tea[i].major;
					break;
				}
                case NAME:{
                	fieldValue=t_Ass->data.tea[i].name;
					break;
				}
                case ID:{
                	fieldValue=t_Ass->data.tea[i].t_id;
					break;
				}
                case OBJECT:{//检查所有课程
                    for(int j=0;j<OBJECT_NUM;j++){
                        if(!strcasecmp(t_Ass->data.tea[i].object[j],input)) {
                            fieldValue=input;//标记为匹配
                            break;
                        }
                    }
                    break;
            	}
        	}
        if (!strcasecmp(fieldValue,input))
            storT[numT++]=i;	
   		}
	}
	showSearch(s_Ass,t_Ass,storS,storT,numS,numT,who);
	free(storS);
	free(storT);
}


void showSearchMenu(int who){//展示查询菜单
    char* whoStr=(who=='T')?"teachers":(who=='S')?"students":"all records";
    printf("\n\n================================================================\n");
    printf("\t\t   Search %s by:\n", whoStr);
    printf("================================================================\n");
    printf("%-6s %-30s %-15s\n","Option","Field","Input Format");
    printf("----------------------------------------------------------------\n");
    printf("%-6d %-30s %-15s\n",AGE,"Age","Integer (e.g.  25)");
    printf("%-6d %-30s %-15s\n",MAJOR,"Major","String (e.g.  Computer)");
    printf("%-6d %-30s %-15s\n",NAME,"Name","String (e.g.  Wang Ning)");
    printf("%-6d %-30s %-15s\n",OBJECT,"Teaching/Subject","String (e.g.  Math)");
    printf("%-6d %-30s %-15s\n",GENDER,"Gender","'M'or 'F'");
    printf("%-6d %-30s %-15s\n",ID,"ID","10-digit String");
    printf("================================================================\n");
    printf("Enter search option: ");
}


void search(int who,int which){//成功的话打印该信息
	Assist *s_Ass=read(stuFile,'S');//读取信息
	Assist *t_Ass=read(teaFile,'T');//读取信息
    printf("\nEnter Search Term");
	switch(which){
		case AGE:{
			int age;
			printf("\tAge: ");
			if(scanf("%d",&age)==1){
				getchar();
				ageSearch(s_Ass,t_Ass,who,age);
			}
			else{
				printf("\nPlease Enter The Right Age\n");
				freeAssist(s_Ass);
				freeAssist(t_Ass);
				return  ;
			}
			break;
		}
		case GENDER:{
			char gender;
			printf("\tGender (M/F): ");
			if(scanf("%c",&gender)==1){
				gender=toupper(gender);
				getchar();
				genderSearch(s_Ass,t_Ass,who,gender);
			}
			else {
				printf("\nPlease Enter F/M\n");
				freeAssist(s_Ass);
				freeAssist(t_Ass);
				return  ;
			}
			break;
		}
		case MAJOR:
		case NAME:
		case ID:
		case OBJECT:{
			char* fieldName[]={"","Age","Major","Name","Object","Gender","Id","Score"};
            printf("\t\t%s: ",fieldName[which]);
            char input[BUFFER_MAX]; //使用足够大的缓冲区
            getInf(input,sizeof(input));
            stringSearch(s_Ass,t_Ass,who,which,input);
            break;
        }
        default:{
            printf("\nUnsupported Search Type\n");
            break;
        }		
	}
	freeAssist(s_Ass);
    freeAssist(t_Ass);
}


char getIdentity(){//判断对学生还是老师
    char input;
    while(1){
        printf("To Teacher Or Student(enter T/S)?");
        input=getchar();
        while(getchar()!='\n');//清空缓冲区
        input=toupper(input);
        if(input=='T'||input=='S')
            return input;
        printf("Invalid Input, Please Enter T/S\n");
    }
}


void change(char identity){//改变信息，某个元素或多个元素
	FILE *fp=NULL;
	char id[ID_MAX];
	printf("Input The Id To Select Which You Want To Change:");
	getInf(id,sizeof(id));
	if(strlen(id)!=ID_LENGTH){
		printf("The Id Must Be Ten Digit\n");
		return ;
	}
	char* fileName=(identity=='T')?teaFile:stuFile;
	Assist* assist=read(fileName,identity);
	int index=-1;//输入学号对应位置
	if(identity=='T'){
		for(int i=0;i<assist->n;i++){
			if(!strcmp(assist->data.tea[i].t_id,id)){
				index=i;
				break;
			}
		}
	}
	else{
		for(int i=0;i<assist->n;i++) {
            if (!strcmp(assist->data.stu[i].s_id,id)) {
                index=i;
                break;
            }
		}
	}
	if(index==-1){
        printf("No Such Information\n");
        freeAssist(assist);
        return  ;
    }
    printf("\nCurrent Information:\n");
    show(assist,index);
    int choices[CHANGE_MAX]={0};//存储选择的字段
    int choiceCount=0;//存储改变元素个数
    int choice;//改变元素
    printf("\nSelect Fields To Change (enter 0 to finish):\n");
    if (identity=='T'){
        printf("1.Age  2.Major  3.Name  4.Object\n");
        printf("Enter Choice (1-4, 0 to finish): ");
        while(choiceCount<CHANGE_MAX){
            if(scanf("%d",&choice)!=1){
                printf("Invalid Input. Please Enter A Number.\n");
                while(getchar()!='\n'); //清除无效输入
                continue;
            }
            getchar();//清除换行符
            if(choice==0) break;
            if(choice<AGE||choice>OBJECT) {
                printf("Invalid Choice. Please Enter 1-4 or 0.\n");
                continue;
            }
            choices[choiceCount++]=choice;
        }
    }
	else{
        printf("1.Age  2.Major  3.Name  4.Object  5.Score\n");
        printf("Enter Choice (1-5, 0 to finish): ");
        while(choiceCount<10){
            if(scanf("%d",&choice)!=1){
                printf("Invalid Input. Please Enter A Number.\n");
                while(getchar()!='\n');
                continue;
            }
            getchar();
            if(choice==0) break;
            if(choice<AGE||choice>SCORE){
                printf("Invalid Choice. Please Enter 1-5 or 0.\n");
                continue;
            }
            choices[choiceCount++]=choice;
        }
    }
    if (choiceCount==0){
        printf("No Changes Made\n");
        freeAssist(assist);
        return  ;
    }
    for(int i=0;i<choiceCount;i++){//执行修改
        switch (choices[i]){
            case AGE:{
            	int newAge;
            	while(1){
            		printf("Enter New Age: ");
	                if(scanf("%d",&newAge)==1){
	                	getchar();
	                	char* tip;
	                	int test;
 	                	if(identity=='T'){
 	                		test=ageTest(newAge,'T');
 	                		if(test==AGE_NEG||test==AGE_YOUNG||test==AGE_OLD){
								tip=(test==AGE_NEG)?"Age Can't Be Negative.\n":(test==AGE_YOUNG)?"Too Young.\n":
									"Too Old.\n";
	                			printf("\nNo.%-3d: %s",i+1,tip);
	                			continue;
							}
							assist->data.tea[index].age=newAge;
							break;
						}
						else{
							test=ageTest(newAge,'S');
							if(test==AGE_NEG||test==AGE_YOUNG||test==AGE_OLD){
								tip=(test==AGE_NEG)?"Age Can't Be Negative.\n":(test==AGE_YOUNG)?"Too Young.\n":
									"Too Old.\n";
	                			printf("\nNo.%-3d: %s",i+1,tip);
	                			continue;
							}
							assist->data.stu[index].age=newAge;
							break;
						}
                	}
	                else{
	                    printf("Age Must Be Digit\n");
	                    while(getchar()!='\n');
		            }
				}
				break;
			}
            case MAJOR:{
                printf("Enter New Major: ");
                if (identity=='T')
                    getInf(assist->data.tea[index].major, 
                           sizeof(assist->data.tea[index].major));
				else
                    getInf(assist->data.stu[index].major, 
                           sizeof(assist->data.stu[index].major));
                break;
            }
            case NAME:{
                printf("Enter New Name: ");
                if(identity=='T')
                    getInf(assist->data.tea[index].name, 
                        sizeof(assist->data.tea[index].name));
				else
                    getInf(assist->data.stu[index].name, 
                        sizeof(assist->data.stu[index].name));
                break;
            }
            case OBJECT:{//修改课程
                printf("Select Subject To Change (1-3): ");
                int subj;
                if(scanf("%d",&subj)==1&&subj>=FIRST&&subj<=THIRD&&getchar()=='\n'){
                	printf("Enter New Subject: ");
	                if(identity=='T'){
	                    getInf(assist->data.tea[index].object[subj-1], 
	                           sizeof(assist->data.tea[index].object[0]));
	                } 
					else{
	                    getInf(assist->data.stu[index].cl[subj-1].object, 
	                           sizeof(assist->data.stu[index].cl[0].object));
	                }
                }
                else{
                	printf("Invalid Subject Number\n");
                	fclose(fp);
        			freeAssist(assist);
                    return  ;
				}
                break;
            }
            case SCORE:{//学生修改分数
                if(identity=='S'){
                    printf("Select Subject To Change (1-3): ");
                    int subj;
                    int newScore;
                    if(scanf("%d",&subj)==1&&subj>FIRST&&subj<=THIRD&&getchar()=='\n'){
                    	printf("Enter New Score: ");
                    	while(1){
                    		if(scanf("%d",&newScore)==1&&getchar()=='\n'){
                    			if(newScore<0||newScore>100){
    								printf("Score Must Be 0-100");
    								continue;
								}
                    			assist->data.stu[index].cl[subj-1].score=newScore;
                    			break;
							}
							else{
								printf("Invalid Input. Please Enter A Number.");
            					while(getchar()!='\n');
							}
						}
                    }
                    else{
                    	printf("Invalid Subject Number\n");
                    	fclose(fp);
        				freeAssist(assist);
                        return  ;
					}
                }
                break;
            }
    	}
	}
    fp=creatFile(fileName,"w");
    writeFile(assist,fp);
    fclose(fp);
    printf("Changes Saved Successfully\n");
    printf("\nUpdated Information:\n");
	show(assist, index);
    freeAssist(assist);
}


void writeFile(Assist* assist,FILE* fp){//将Assist内容写入fp对应文件
	if(assist->identity=='T'){
		for(int i=0;i<assist->n;i++){
			Teacher *t=&assist->data.tea[i];
        	fprintf(fp,"%c %d %s %s %s %s %s %s\n",t->gender,t->age,t->major,t->name,t->t_id,t->object[0],t->object[1],t->object[2]);//写入文件
   	 	}
	}
    if(assist->identity=='S'){
		for(int i=0;i<assist->n;i++){
			Student *s=&assist->data.stu[i];
        	fprintf(fp,"%c %d %s %s %s %s %d %s %d %s %d\n",s->gender,s->age,s->major,s->name,s->s_id,
					s->cl[0].object,s->cl[0].score,s->cl[1].object,s->cl[1].score,s->cl[2].object,s->cl[2].score);//写入文件
   	 	}
	}
}


void delete(char identity){//根据索引删除某条信息，可批量
	char* fileName=(identity=='T')?teaFile:stuFile;
    Assist* assist=read(fileName,identity);
	if(assist->n==0){
        printf("No Data To Delete\n");
        freeAssist(assist);
        return  ;
    }
	printf("\nCurrent Records:\n");
    view(identity);
	//获取批量删除索引
    printf("Enter Indexes To Delete (e.g. 1 3 5, max %d ): ", assist->n);
    char input[BUFFER_MAX];
    getInf(input, sizeof(input));
    //解析索引
    int indexes[DELETE_MAX]={0};
    int count=0;
    char* token=strtok(input," ");
    while(token!=NULL&&count<DELETE_MAX){
        int idx=atoi(token)-1;//转换索引
        if(idx>=0&&idx<assist->n){
            indexes[count++]=idx;
        }
        token=strtok(NULL," ");
    }
    if(count==0){
        printf("No Valid Indexes Entered\n");
        freeAssist(assist);
        return;
    }
    //降序排序索引（避免删除时位移影响）
    for(int i=0;i<count-1;i++){//冒泡排序
        for(int j=i+1;j<count;j++){
            if(indexes[i]<indexes[j]){
                int temp=indexes[i];
                indexes[i]=indexes[j];
                indexes[j]=temp;
            }
        }
    }
    int deleted=0;//执行删除
    for(int i=0;i<count;i++){
        int idx=indexes[i];
        if(identity=='T') {
            for(int j=idx;j<assist->n-1;j++){
                assist->data.tea[j]=assist->data.tea[j+1];
            }
        }
		else{
            for(int j=idx;j<assist->n-1;j++){
                assist->data.stu[j]=assist->data.stu[j+1];
            }
        }
        assist->n--;
        deleted++;
    }
    FILE* fp=creatFile(fileName,"w");//写入更新后的数据
    writeFile(assist,fp);
    fclose(fp);
    printf("Successfully Deleted %d Records\n", deleted);
    freeAssist(assist);
}


int sort_getChoice(char identity){//获取排序的标准
	printf("\nSort %s By:\n",(identity == 'T')?"Teachers":"Students");
    printf("-----------------------------\n");
    if(identity=='S'){
        printf("1. Age\n");
        printf("2. Major\n");
        printf("3. Name\n");
        printf("4. Subject\n");
        printf("5. Gender\n");
        printf("6. ID\n");
        printf("7. Score\n");
    } 
	else{
        printf("1. Age\n");
        printf("2. Major\n");
        printf("3. Name\n");
        printf("4. Teaching Subject\n");
        printf("5. Gender\n");
        printf("6. ID\n");
    }
    printf("-----------------------------\n");
	printf("Enter Sort Option: ");
	int choice;
	if(scanf("%d",&choice)==1){
		getchar();
        if(identity=='S'&&(choice<SORT_AGE||choice>SORT_SCORE)){
            printf("Invalid Choice! Please Select 1-7.\n");
            return -1;
        } 
		else if(identity=='T'&&(choice<SORT_AGE||choice>SORT_ID)){
            	printf("Invalid Choice For Teachers! Please Select 1-6.\n");
            	return -1;
        	}
        return choice;
    }
	printf("Invalid Input,Please Enter Digit\n");
	while(getchar()!='\n');
	return -1;
}


void sort(char identity){//对信息排序
	int choice=sort_getChoice(identity);
	if(choice==-1)
		return  ;
	char* fileName=(identity=='T')?teaFile:stuFile;
	Assist *assist=read(fileName,identity);//assist已读取文件内容
	//成绩与年龄均为整形，设计利用同一个numSort函数
	//其余字符串通用一个strSort函数
	//处理需要科目选择的排序
	int subj=-1;
    if(choice==SORT_OBJECT||choice==SORT_SCORE){
        printf("Select Subject To Sort (1-3): ");
        if(scanf("%d",&subj)==1&&subj>=FIRST&&subj<=THIRD){
            getchar();
            subj--;//转换索引
        } 
		else{
            printf("Invalid Subject Selection!\n");
            freeAssist(assist);
            return  ;
        }
    }
    FILE* fp=creatFile(fileName,"w");
	switch (choice){
		case SORT_GENDER:
		case SORT_AGE:
        case SORT_SCORE:{
			numSort(assist,choice,subj);
			break;
		}
        case SORT_MAJOR:
        case SORT_NAME:
        case SORT_OBJECT:
        case SORT_ID:{
			strSort(assist,choice,subj);
			break;	
		}
	}
	writeFile(assist,fp);
	freeAssist(assist);
	fclose(fp);
	printf("Sorting Completed Successfully!\n");
}


void strSort(Assist* assist,int choice,int subj){//字符串排序函数
    int n=assist->n;
    for(int i=0;i<n-1;i++){
        for(int j=0;j<n-i-1;j++){
            char *str1,*str2;
            // 获取比较字符串
            if (assist->identity=='S'){
                Student* stu1=&assist->data.stu[j];
                Student* stu2=&assist->data.stu[j + 1];
                switch (choice){
                    case SORT_MAJOR:{
                    	str1=stu1->major;
                        str2=stu2->major;
						break;
					}
                    case SORT_NAME:{
                    	str1=stu1->name;
                        str2=stu2->name;
						break;
					}
                    case SORT_OBJECT:{
                    	str1=stu1->cl[subj].object;
                        str2=stu2->cl[subj].object;
                        break;
					}
                    case SORT_ID:{
                    	str1=stu1->s_id;
                        str2=stu2->s_id;
						break;
					}
                }
            } 
			else{//Teacher
                Teacher* tea1=&assist->data.tea[j];
                Teacher* tea2=&assist->data.tea[j + 1];
                switch (choice){
                    case SORT_MAJOR:{
                    	str1=tea1->major;
                        str2=tea2->major;
						break;
					}
                    case SORT_NAME:{
                    	str1=tea1->name;
                        str2=tea2->name;
						break;
					}
                    case SORT_OBJECT:{
                    	str1=tea1->object[subj];
                        str2=tea2->object[subj];
                        break;
					}
                    case SORT_ID:{
                    	str1=tea1->t_id;
                        str2=tea2->t_id;
                        break;
					}
                }
            }
            if(strcasecmp(str1,str2)>0){
                if(assist->identity=='S'){
                    Student temp=assist->data.stu[j];
                    assist->data.stu[j]=assist->data.stu[j+1];
                    assist->data.stu[j+1]=temp;
                } 
				else{
                    Teacher temp=assist->data.tea[j];
                    assist->data.tea[j]=assist->data.tea[j+1];
                    assist->data.tea[j+1] = temp;
                }
            }
        }
    }
}


void numSort(Assist* assist,int choice,int subj){//数字排序函数
    int n=assist->n;
    for(int i=0;i<n-1;i++){
        for(int j=0;j<n-i-1;j++){
            int value1,value2;
            if(assist->identity=='S'){
                Student* stu1=&assist->data.stu[j];
                Student* stu2=&assist->data.stu[j+1];
                if(choice==SORT_GENDER){
                	value1=stu1->gender;
                    value2=stu2->gender;
				}
                else if(choice==SORT_AGE){
                    	value1=stu1->age;
                    	value2=stu2->age;
                	}
					else{//SORT_SCORE
                    	value1=stu1->cl[subj].score;
                    	value2=stu2->cl[subj].score;
                		}
                if(value1>value2){//冒泡排序,升序排列
                    Student temp=assist->data.stu[j];
                    assist->data.stu[j]=assist->data.stu[j+1];
                    assist->data.stu[j+1]=temp;
				}
            } 
			else{//Teacher
                Teacher* tea1=&assist->data.tea[j];
                Teacher* tea2=&assist->data.tea[j+1];
                if(choice==SORT_GENDER){
                	value1=tea1->gender;
                    value2=tea2->gender;
				}
                else{//老师无SORT_SCORE
                    	value1=tea1->age;
                    	value2=tea2->age;
                }
            	if(value1>value2){//冒泡排序,升序排列
                    Teacher temp=assist->data.tea[j];
                    assist->data.tea[j]=assist->data.tea[j+1];
                    assist->data.tea[j+1]=temp;
            	}
       	 	}
		}
    }
}



































































































































