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
    printf("| Enter choice (1-6, any other key to quit):                  |\n");
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


void freeList(Node *head){//释放链表
    Node *tmp;
    while(head){//依次释放
        tmp=head;
        head=head->next;//temp变为该节点的下一个
        free(tmp);
    }
}


Linklist* read(char *s, char identity){//读取文件信息，创建链表
    FILE* fp=creatFile(s,"r");
    Linklist* link=malloc(sizeof(Linklist));
	link->head=link->tail=NULL;
    char buffer[BUFFER_MAX];
    while(fgets(buffer,sizeof(buffer),fp)){
        Node *newNode=malloc(sizeof(Node));
        newNode->identity=identity;
        newNode->next=NULL;
        if(identity=='T'){
            sscanf(buffer,"%c %d %s %s %s %s %s %s",&newNode->data.tea.gender,&newNode->data.tea.age,newNode->data.tea.major,newNode->data.tea.name,
                   newNode->data.tea.t_id,newNode->data.tea.object[FIRST_INDEX],newNode->data.tea.object[SECOND_INDEX],newNode->data.tea.object[THIRD_INDEX]);
        } 
		else{
            sscanf(buffer,"%c %d %s %s %s %s %d %s %d %s %d", &newNode->data.stu.gender,&newNode->data.stu.age,newNode->data.stu.major, newNode->data.stu.name,
                newNode->data.stu.s_id, newNode->data.stu.cl[FIRST_INDEX].object,&newNode->data.stu.cl[FIRST_INDEX].score,newNode->data.stu.cl[SECOND_INDEX].object,
                   &newNode->data.stu.cl[SECOND_INDEX].score,newNode->data.stu.cl[THIRD_INDEX].object,&newNode->data.stu.cl[THIRD_INDEX].score);
        }
        if (!link->head) 
			link->head=link->tail=newNode;
        else{
            link->tail->next=newNode;//一次if过后，tail已不是尾
            link->tail=newNode;//tail变为newNode
        }
    }
    fclose(fp);
    return link;
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


char getIdentity(){//判断对学生还是老师
    char input;
    while(1){
        printf("To teacher or student(enter T/S)?");
        input=getchar();
        while(getchar()!='\n');//清空缓冲区
        input=toupper(input);
        if(input=='T'||input=='S')
            return input;
        printf("Invalid input, Please enter T/S\n");
    }
}


void writeFile(Node* head,FILE* fp){//将链表内容写入文件
    Node* current=head;
    while(current){
        if(current->identity=='T'){
            Teacher *t=&current->data.tea;
            fprintf(fp,"%c %d %s %s %s %s %s %s\n",t->gender,t->age,t->major,t->name,t->t_id,
                    t->object[FIRST_INDEX], t->object[SECOND_INDEX], t->object[THIRD_INDEX]);
        } 
		else{
            Student *s=&current->data.stu;
            fprintf(fp,"%c %d %s %s %s %s %d %s %d %s %d\n",s->gender,s->age,s->major,s->name,s->s_id,s->cl[FIRST_INDEX].object, s->cl[FIRST_INDEX].score,
					s->cl[SECOND_INDEX].object, s->cl[SECOND_INDEX].score,s->cl[THIRD_INDEX].object, s->cl[THIRD_INDEX].score);
        }
        current=current->next;
    }
}


void show(Node *head){//展示节点储存信息
    if(head->identity=='S'){
		Student s=head->data.stu;
		printf("%-5c %-4d %-16s %-16s %-12s",s.gender,s.age,s.major,s.name,s.s_id);
        printf("| Subjects: ");
        for (int j=0;j<OBJECT_NUM;j++)
            printf("%s:%d  ",s.cl[j].object,s.cl[j].score);
        putchar('\n');
	}
	if(head->identity=='T'){
		Teacher t=head->data.tea;
        printf("%-5c %-4d %-16s %-16s %-12s",t.gender,t.age,t.major,t.name,t.t_id);
        printf("| Subjects: ");
        for(int j=0;j<OBJECT_NUM;j++){
            printf("%s ",t.object[j]);
        }
        putchar('\n');
    }
    printf("----------------------------------------------------------------\n");
}


int view(char identity,Node *head){//展示信息并返回信息个数
	int num=0;
	if(head!=NULL){//链表尾不是空，链表尾储存的地址为空
		puts("\n\n");
		printf("================================================================\n");
        printf("%-3s  %-4s %-4s %-16s %-16s %-16s\n", 
               "No.","Gender","Age","Major","Name","ID");
        printf("================================================================\n");
        Node* current=head;//不要直接使用head，以免破坏原链表结构
        while(current){
        	printf("No.%-3d",num+1);//序号
        	show(current);
        	current=current->next;//读取下一个节点
        	num++;
		}
        printf("Total Records: %d\n",num);
    }
	else   printf("No Information\n");
	return num;
}


int idTest(char* id,Node* head){//检验学号或工号是否长度合理并且唯一
	if(strlen(id)!=ID_LENGTH)//先判断数目再判断内容
		return ID_SIZE_WRONG;
	for(Node* current=head;current!=NULL;current=current->next){
		if((current->identity=='T')&&!strcmp(current->data.tea.t_id,id))
			return ID_NOT_UNIQUE;
		else if(current->identity=='S'&&!strcmp(current->data.stu.s_id,id))
				return ID_NOT_UNIQUE;
	}
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


void addToList(Node *newNode,Linklist* link){//添加信息到链表
	newNode->next=NULL;//确保新节点next为NULL
    if(!link->head){//判断首节点是否为空
        link->head=newNode;
        link->tail=newNode;
    } 
	else{
        link->tail->next=newNode;//当前尾节点指向新节点
        link->tail=newNode;//更新尾指针，尾节点发生变化
    }
}


void addTeacher(int addNum,Linklist* link){//添加老师信息
	printf("e.g.M 20 Computer 王宁 2220241385 Math Chinese English\n");
	for(int i=0;i<addNum;i++){
		Node *newNode=malloc(sizeof(Node));
		newNode->identity='T';
		if(scanf(" %c %d %s %s %s %s %s %s",&newNode->data.tea.gender,&newNode->data.tea.age,newNode->data.tea.major,newNode->data.tea.name,newNode->data.tea.t_id,
			newNode->data.tea.object[FIRST_INDEX],newNode->data.tea.object[SECOND_INDEX],newNode->data.tea.object[THIRD_INDEX])!=8){
			printf("\nNo.%-3d: Invalid Input Format. Record Skipped.\n");
			while(getchar()!='\n');
       		free(newNode);
           	continue;
		}//跳过此次
		getchar();//清除换行符
		int testI=idTest(newNode->data.tea.t_id,link->head);
		if(testI!=ID_GOOD){
			char *tip=(testI==ID_SIZE_WRONG)?"Invalid ID Length. Record Skipped.\n":
				"The Id Already Exist,Record Skipped.\n";
			printf("\nNo.%-3d: %s",i+1,tip);
			free(newNode);
			continue;
		}
		int testA=ageTest(newNode->data.tea.age,'T');
		if(testA!=AGE_GOOD){
			char* tip=(testA==AGE_NEG)?"Age Can't Be Negative. Record Skipped.\n":(testA==AGE_YOUNG)?"\tToo Young. Record Skipped.\n":
				"Too Old.  Record Skipped.\n";
			printf("\nNo.%-3d: %s",i+1,tip);
			free(newNode);
			continue;
		}
		addToList(newNode,link);
	}
}


void addStudent(int addNum,Linklist* link){//添加学生信息
	printf("e.g.M 20 Computer 王宁 2220241385 Math 59 Chinese 59 English 59\n"); 
	for(int i=0;i<addNum;i++){
		Node *newNode=malloc(sizeof(Node));
		newNode->identity='S';
		if(scanf(" %c %d %s %s %s %s %d %s %d %s %d",&newNode->data.stu.gender,&newNode->data.stu.age,newNode->data.stu.major,newNode->data.stu.name,
			newNode->data.stu.s_id,newNode->data.stu.cl[FIRST_INDEX].object,&newNode->data.stu.cl[FIRST_INDEX].score,newNode->data.stu.cl[SECOND_INDEX].object,
				&newNode->data.stu.cl[SECOND_INDEX].score,newNode->data.stu.cl[THIRD_INDEX].object,&newNode->data.stu.cl[THIRD_INDEX].score)!=11) {
			printf("\nNo.%-3d: Invalid Input Format. Record Skipped.\n",i+1);
			while(getchar()!='\n');
		    free(newNode);
		    continue;
		}
		getchar();//清除换行符
		int testI=idTest(newNode->data.stu.s_id,link->head);
		if(testI!=ID_GOOD){
			char *tip=(testI==ID_SIZE_WRONG)?"Invalid ID Length. Record Skipped.\n":
				"The Id Already Exist,Record Skipped.\n";
			printf("\nNo.%-3d: %s",i+1,tip);
			free(newNode);
			continue;
		}
		int testA=ageTest(newNode->data.stu.age,'S');
		if(testA!=AGE_GOOD){
			char* tip=(testA==AGE_NEG)?"Age Can't Be Negative. Record Skipped.\n":(testA==AGE_YOUNG)?"\tToo Young. Record Skipped.\n":
				"Too Old.  Record Skipped.\n";
			printf("\nNo.%-3d: %s",i+1,tip);
			free(newNode);
			continue;
		}
		addToList(newNode,link);
	}
}


void add(char identity,Linklist* link){//添加信息
	printf("How Many Pieces Of Information To Add?");
	int addNum;
	if(scanf("%d",&addNum)==1&&addNum>0&&getchar()=='\n'){
		printf("Please Enter %d Pieces Of Information:\n",addNum);
		if(addNum>1)
			printf("Use [ENTER] To Divide\n");
		if(identity=='T')
			addTeacher(addNum,link);
		if(identity=='S')
			addStudent(addNum,link);
    }
	else{
		printf("The Count Of Information Can't Be Negative\n");
		while(getchar()!='\n');
		return  ;
	}
}


void delete(char identity,Linklist* link){//根据索引删除某条信息，可批量
	if(!link->head){
        printf("No Data To Delete\n");
        return  ;
    }
	printf("\nCurrent Records:");
    int num=view(identity,link->head);
	//获取批量删除索引
    printf("Enter Indexes To Delete (e.g. 1 3 5, max %d ): ",num);
    char input[BUFFER_MAX];
    getInf(input,sizeof(input));
    //解析索引
    int indexes[DELETE_MAX]={0};
    int count=0;
    char* token=strtok(input," ");
    while(token!=NULL&&count<DELETE_MAX){
        int idx=atoi(token)-1;//转换索引
        if(idx>=0&&idx<num){
            indexes[count++]=idx;
        }
        token=strtok(NULL," ");
    }
    if(count==0){
        printf("No Valid Indexes Entered\n");
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
    int deleted;
    for(deleted=0;deleted<count;deleted++){//执行删除
    	deleteNodeByIndex(link,indexes[deleted]);
	}
	printf("Successfully Deleted %d Records\n",deleted);
}


void deleteNodeByIndex(Linklist* link,int index){//借助索引删除节点
    Node *current=link->head;
    Node *assist=NULL;
    int currentIndex=0;
    while(current&&currentIndex<index){//遍历链表找到要删除的节点
        assist=current;
        current=current->next;
        currentIndex++;
    }
    if(current){//找到了要删除的节点
        if(!assist)//处理头节点
			link->head=current->next;
        else//处理中间节点
            assist->next=current->next;
        if(current==link->tail)//处理尾节点
            link->tail=assist;
        free(current);//释放删除节点内存
    }
}


void change(char identity,Linklist* link){//改变信息，某个元素或多个元素
	char id[ID_MAX];
	printf("Input The ID To Select Which You Want To Change:");
	getInf(id,sizeof(id));
	if(strlen(id)!=ID_LENGTH){
		printf("The Id Must Be Ten Digit\n");
		return ;
	}
	Node* current=link->head;
	if(current->identity=='T'){
		while(current){
			if(!strcmp(current->data.tea.t_id,id))
				break;
			current=current->next;
		}
	}
	else{
		while(current){
			if(!strcmp(current->data.stu.s_id,id))
				break;
			current=current->next;
		}
	}
	if(!current){
        printf("No Such Information\n");
        return  ;
    }
    printf("\nCurrent Information:\n");
    show(current);
    int choices[CHANGE_MAX]={0};//存储选择的字段
    int choiceCount=0;//存储改变元素个数
    int choice;//改变元素
    printf("\nSelect Fields To Change (enter 0 to finish):\n");
    if (current->identity=='T'){
        printf("1.Age  2.Major  3.Name  4.Object\n");
        printf("Enter Choice (1-4, 0 to finish): ");
        while(choiceCount<CHANGE_MAX){
            if(scanf("%d",&choice)!=1) {
                printf("Invalid Input. Please Enter A Number.\n");
                while(getchar()!='\n');//清除无效输入
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
        while(choiceCount<CHANGE_MAX){
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
 	                	if(current->identity=='T'){
 	                		test=ageTest(newAge,'T');
 	                		if(test==AGE_NEG||test==AGE_YOUNG||test==AGE_OLD){
								tip=(test==AGE_NEG)?"Age Can't Be Negative.\n":(test==AGE_YOUNG)?"Too Young,Change Ignored.\n":
									"Too Old, Change Ignored.\n";
	                			printf("\nNo.%-3d: %s",i+1,tip);
	                			continue;
							}
							current->data.tea.age=newAge;
							break;
						}
						else{
							test=ageTest(newAge,'S');
							if(test==AGE_NEG||test==AGE_YOUNG||test==AGE_OLD){
								tip=(test==AGE_NEG)?"Age Can't Be Negative.\n":(test==AGE_YOUNG)?"Too Young,Change Ignored.\n":
									"Too Old, Change Ignored.\n";
	                			printf("\nNo.%-3d: %s",i+1,tip);
	                			continue;
							}
							current->data.stu.age=newAge;
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
                if (current->identity=='T')
                    getInf(current->data.tea.major,sizeof(current->data.tea.major));
				else
                    getInf(current->data.stu.major,sizeof(current->data.stu.major));
                break;
            }
            case NAME:{
                printf("Enter New Name: ");
                if(current->identity=='T')
                    getInf(current->data.tea.name,sizeof(current->data.tea.name));
				else
                    getInf(current->data.stu.name,sizeof(current->data.stu.name));
                break;
            }
            case OBJECT:{//修改课程
                printf("Select Subject To Change (1-3): ");
                int subj;
                if(scanf("%d",&subj)==1&&subj>=FIRST&&subj<=THIRD&&getchar()=='\n'){
                	printf("Enter New Subject: ");
	                if(current->identity=='T')
	                    getInf(current->data.tea.object[subj-1],sizeof(current->data.tea.object[subj-1]));
					else
	                    getInf(current->data.stu.cl[subj-1].object,
	                           sizeof(current->data.stu.cl[subj-1].object));
                }
                else{
                	printf("Invalid Subject Number\n");
                    return  ;
				}
                break;
            }
            case SCORE:{//只有学生修改分数
                printf("Select Subject To Change (1-3): ");
                int subj;
                int newScore;
                if(scanf("%d",&subj)==1&&subj>FIRST&&subj<=THIRD&&getchar()=='\n'){
                    printf("Enter New Score: ");
                    while(1){
                    	if(scanf("%d",&newScore)==1&&getchar()=='\n'){
                    		if(newScore<0||newScore>100){
    							printf("Score Must be 0-100, Please Input The Right Score: ");
    							continue;
							}
                    		current->data.stu.cl[subj-1].score=newScore;
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
                    return  ;
				}
                break;
            }
    	}
	}
    printf("Changes Saved Successfully\n");
    printf("\nUpdated Information:\n");
	show(current);
}


void search(char who,int which,Linklist* link){//成功的话打印该信息
    printf("\nEnter Search Term");
	switch(which){
		case AGE:{
			int age;
			printf("\tAge: ");
			if(scanf("%d",&age)==1){
				getchar();
				numSearch(link->head,age,who,which);
			}
			else{
				printf("Please Enter The Right Age\n");
				return  ;
			}
			break;
		}
		case GENDER:{
			printf("\tGender (M/F): ");
            char gender=toupper(getchar());
            while(getchar()!='\n');
        	if(gender!='F'&&gender!='M'){
            	printf("The Gender Must Be F/M\n");
                return  ;
        	}
            numSearch(link->head,(int)gender,who,which);
            break;
		}
		case MAJOR:
		case NAME:
		case ID:
		case OBJECT:{
			char* fieldName[]={"","Age","Major","Name","Object","Gender","Id","Score"};
            printf("\t%s: ",fieldName[which]);
            char input[BUFFER_MAX]; //使用足够大的缓冲区
            getInf(input,sizeof(input));
            stringSearch(link->head,input,who,which);
            break;
        }
        default:{
            printf("Unsupported Search Type\n");
            break;
        }		
	}
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
    printf("%-6d %-30s %-15s\n",GENDER,"Gender","'M' or 'F'");
    printf("%-6d %-30s %-15s\n",ID,"ID","10-digit String");
    printf("================================================================\n");
    printf("Enter search option: ");
}


void numSearch(Node* head,int num,char who,int which){//以数字查询
	Node* current=head;
	int numT,numS;
	numS=numT=0;
	while(current){
		switch (which){
			case AGE:
				if((current->identity=='T')&&(who=='T'||who=='A')&&current->data.tea.age==num){
					if(numT==0)
							printf("---------------------Teacher----------------------\n");
			        printf("No.%-5d",numT+1);
					show(current);
					numT++;
				}
				if((current->identity=='S')&&(who=='S'||who=='A')&&current->data.stu.age==num){
					if(numS==0)
							printf("---------------------Student----------------------\n");
			        printf("No.%-5d",numS+1);
					show(current);
					numS++;
				}
				break;
			case GENDER:
				if((current->identity=='T')&&(who=='T'||who=='A')&&(int)current->data.tea.gender==num){
					if(numT==0)
							printf("---------------------Teacher----------------------\n");
			        printf("No.%-5d",numT+1);
					show(current);
					numT++;
				}
				if((current->identity=='S')&&(who=='S'||who=='A')&&(int)current->data.stu.gender==num){
					if(numS==0)
							printf("---------------------Student----------------------\n");
			        printf("No.%-5d",numS+1);
					show(current);
					numS++;
				}
				break;
		}
		current=current->next;
	}
	if(who=='S'||who=='A')
        printf("Students' Total: %d\n",numS);
    if(who=='T'||who=='A')
        printf("Teachers' Total: %d\n",numT);
    if(who=='A')
        printf("All Total: %d\n",numS+numT);
}


void stringSearch(Node *head,char* input,char who,int which){//以字符串查询
	Node* current=head;
	int numS,numT;
	numT=numS=0;
	while(current){
		char *fieldValue="";
		if((current->identity=='S')&&(who=='S'||who=='A')){
			switch (which){
                case MAJOR:{
                	fieldValue=current->data.stu.major;
					break;
				}
                case NAME:{
                	fieldValue=current->data.stu.name;
					break;
				}
                case ID:{
                	fieldValue=current->data.stu.s_id;
					break;
				}
				case OBJECT:{
					 for(int j=0;j<OBJECT_NUM;j++){
                        if(!strcasecmp(current->data.stu.cl[j].object,input)){
                            fieldValue=input;
                            break;
                        }
                    }
					break;
				}
			}
			if(!strcasecmp(fieldValue,input)){
				if(numS==0)
					printf("----------------------Student-----------------------\n");
				printf("No.%-5d",numS+1);
				show(current);
				numS++;
			}
		}
	    if((current->identity=='T')&&(who=='T'||who=='A')){
	        switch (which){
	            case MAJOR:{
	                fieldValue=current->data.tea.major;
					break;
				}
	            case NAME:{
	            	fieldValue=current->data.tea.name;
					break;
				}
	            case ID:{
	                fieldValue=current->data.tea.t_id;
					break;
				}
	            case OBJECT:{//检查所有课程
	                for(int j=0;j<OBJECT_NUM;j++){
	                    if(!strcasecmp(current->data.tea.object[j],input)) {
	                        fieldValue=input;//标记为匹配
	                        break;
	                    }
	                }
	                break;
	            }
	        }
	        if(!strcasecmp(fieldValue,input)){
	        	if(numT==0)
					printf("---------------------Teacher----------------------\n");
	        	printf("No.%-5d",numT+1);
	        	show(current);
	        	numT++;
			}
		}
	   	current=current->next;
	}
	if(who=='S'||who=='A')
        printf("Students' Total: %d\n",numS);
    if(who=='T'||who=='A')
        printf("Teachers' Total: %d\n",numT);
    if(who=='A')
        printf("All Total: %d\n",numS+numT);
}


int sort_getChoice(char identity){//获取排序的标准
	printf("\nSort %s by:\n",(identity == 'T')?"teachers":"students");
    printf("------------------------\n");
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
    printf("-------------------------\n");
	printf("Enter sort option: ");
	int choice;
	if(scanf("%d",&choice)==1){
		getchar();
        if(identity=='S'&&(choice<SORT_AGE||choice>SORT_SCORE)){
            printf("Invalid choice! Please select 1-7.\n");
            return -1;
        } 
		else if(identity=='T'&&(choice<SORT_AGE||choice>SORT_ID)){
            	printf("Invalid choice for teachers!Please select 1-6.\n");
            	return -1;
        	}
        return choice;
    }
	printf("Invalid Input,Please Enter Digit\n");
	while(getchar()!='\n');
	return -1;
}


void sort(char identity,Linklist* link){//对信息排序
	if(!link->head){
		printf("No Data To Sort\n");
		while(getchar()!='\n');
		return  ;
	}
	int choice=sort_getChoice(identity);
	if(choice==-1)
		return  ;
	//assist已读取文件内容
	//成绩与年龄和性别均为整形，设计利用同一个numSort函数
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
            return  ;
        }
    }
	if(choice==SORT_AGE||choice==SORT_SCORE||choice==SORT_GENDER){
        numSort(link->head,choice, subj);
    } 
	else{
        strSort(link->head,choice, subj);
    }
	printf("Sorting Completed Successfully!\n");
}


void strSort(Node* head,int choice,int subj){
    int swapped;
    Node* current;
    Node* lptr=NULL;
    do{
        swapped=0;
        current=head;
        while(current->next!=lptr){
            char* str1=getStringField(current,choice,subj);
            char* str2=getStringField(current->next,choice,subj);
            if(strcasecmp(str1,str2)>0){//升序排序
                Data temp=current->data;//交换节点数据
                current->data=current->next->data;
                current->next->data=temp;
                swapped=1;
            }
            current=current->next;
        }
        lptr=current;
    } while(swapped);
}



void numSort(Node* head,int choice,int subj){//数字排序函数
    int swapped;
    Node* current;
    Node* lptr=NULL;
    do{
        swapped=0;
        current=head;
        while(current->next!=lptr){
            int val1=getNumField(current,choice,subj);
            int val2=getNumField(current->next,choice,subj);
            if (val1>val2) {//升序排序
                Data temp=current->data;//交换节点数据
                current->data=current->next->data;
                current->next->data=temp;
                swapped=1;
            }
            current=current->next;
        }
        lptr=current;
    } while(swapped);
}


char* getStringField(Node* node,int choice,int subj){//获取字符串字段
    if (node->identity=='S'){
        Student* stu=&(node->data.stu);
        switch (choice){
            case SORT_MAJOR: return stu->major;
            case SORT_NAME: return stu->name;
            case SORT_OBJECT: return stu->cl[subj].object;
            case SORT_ID: return stu->s_id;
            default: return "";
        }
    } 
	else{
        Teacher* tea=&(node->data.tea);
        switch (choice){
            case SORT_MAJOR: return tea->major;
            case SORT_NAME: return tea->name;
            case SORT_OBJECT: return tea->object[subj];
            case SORT_ID: return tea->t_id;
            default: return "";
        }
    }
}


int getNumField(Node* node,int choice,int subj){//获取数值字段
    if(node->identity=='S'){
        Student* stu=&(node->data.stu);
        switch(choice){
            case SORT_AGE: return stu->age;
            case SORT_SCORE: return stu->cl[subj].score;
            case SORT_GENDER: return stu->gender;
            default: return 0;
        }
    } 
	else{
        Teacher* tea=&(node->data.tea);
        switch (choice){
            case SORT_AGE: return tea->age;
            case SORT_GENDER: return tea->gender;
            default: return 0;
        }
    }
}





























































































































