#include "StaticValue.h"
#include "MyFunction.h"

//对文件的读取和写入改为主函数中
int main(){
	int option;//保存读入的选择
	showMenu();//展示选择的菜单
	while(scanf("%d",&option)==1){//对选择做出回应
		while(getchar()!='\n');//除去选择后缓冲区留存的回车
		char identity;//表征身份
		Linklist* link;//链表储存数据
		char* fileName;//文件名称
		FILE* fp;
		if(option<ADD||option>SORT){
			printf("Wrong Input,Try Again Or Other Character To Quit\n");
			if(scanf("%d",&option)==1){
				while(getchar()!='\n');
				if(option<ADD||option>SORT){
					printf("Wrong Input Again,The Application Exits Automatically\n");
					break;
				}
			}
			else break;
		}
		if(option!=SEARCH){//search有自己的查找选项
			identity=getIdentity();
			fileName=(identity=='T')?teaFile:stuFile;
			link=read(fileName,identity);//链表读取文件要在修改模式开启文件前
			if(option!=VIEW)
				fp=creatFile(fileName,"w");
			else fp=creatFile(fileName,"r");
		}
		switch(option){
			case ADD:{
				add(identity,link);
				break;
			}
			case SEARCH:{
				Linklist* linkS=read(stuFile,'S');
				Linklist* linkT=read(teaFile,'T');
				printf("\nWho To Search(teachers/students/all)?\n");
				printf("Enter T/S/A: ");
				char who;
				if((who=toupper(getchar()))=='T'||who=='S'||who=='A'){
					getchar();
					showSearchMenu(who);
					int which;
					if(scanf("%d",&which)==1&&which>=AGE&&which<=ID){
						getchar();
						Linklist combined={NULL,NULL};//合并链表
						if(who=='T'){
							Linklist* linkT=read(teaFile,'T');
    						search(who,which,linkT);
							freeList(linkT->head);
					        free(linkT);
						}
						if(who=='S'){
							Linklist* linkS=read(stuFile,'S');
    						search(who,which,linkS);
    						freeList(linkS->head);
					        free(linkS);    						
						}
						if(who=='A'){
							//连接两个链表
							if(linkT->head){
                    			combined.head=linkT->head;
                    		if(linkS->head){
                        		linkT->tail->next=linkS->head;
                        		combined.tail=linkS->tail;
                        		combined.tail->next=NULL;
		                    }
							else
		                        combined.tail=linkT->tail;
		                    } 
							else{
		                    	combined.head=linkS->head;
		                    	combined.tail=linkS->tail;
		               	 	}
		               	 	search(who,which,&combined);
					       	freeList(combined.head);//释放整个连接链
					      	free(linkT);
					    	free(linkS);          
					    } 
						if(who!='A'){//单独释放两个链表
					        freeList(linkT->head);
					        free(linkT);
					        freeList(linkS->head);
					        free(linkS);
					    }
					}
					else{
						printf("Invalid Search Option Selection\n");
						while(getchar()!='\n');
						freeList(linkT->head);
			            free(linkT);
			            freeList(linkS->head);
			            free(linkS);
					}
				}
				else{
					printf("\nInvalid Search Target Selection\n");
					while(getchar()!='\n');
					freeList(linkT->head);
		            free(linkT);
		            freeList(linkS->head);
		            free(linkS);
				}
				break;
			}
			case CHANGE:{
				change(identity,link);
				break;
			}
			case DELETE:{
				delete(identity,link);
				break;
			}
			case VIEW:{
				view(identity,link->head);
				break;
			}
			case SORT:{
				sort(identity,link);
				break;
			}
		}
		printf("\n\t\t Operation Complete\n");
		if(option!=VIEW&&option!=SEARCH)
			writeFile(link->head,fp);
		if(option!=SEARCH){
			fclose(fp);
			freeList(link->head);
		}
		puts("\n\n");
		showMenu();
	}
	printf("Bye\n");
	return 0;
}












































































































































































































































