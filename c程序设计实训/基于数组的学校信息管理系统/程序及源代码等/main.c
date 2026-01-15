#include "StaticValue.h"
#include "MyFunction.h"


int main(){
	int option;//保存读入的选择
	showMenu();//展示选择的菜单
	while(scanf("%d",&option)==1){//对选择做出回应
		while(getchar()!='\n');//除去选择后缓冲区留存的回车
		if(option<ADD||option>SORT){
			printf("Wrong Input,Try Again Or Other Character To Quit\n");
			if(scanf("%d",&option)==1){
				while(getchar()!='\n');//清空缓冲区
				if(option<ADD||option>SORT){
					printf("Wrong Input Again,The Application Exits Automatically\n");
					break;
				}
			}
			else break;
		}
		switch(option){
			case ADD:{
					add(getIdentity());
				break;
				}
			case SEARCH:{
				printf("\n\nWho To Search(teachers/students/all)?\n");
				printf("Enter T/S/A: ");
				char who;
				if((who=toupper(getchar()))=='T'||who=='S'||who=='A'){
					getchar();
					showSearchMenu(who);
					int which;
					if(scanf("%d",&which)==1&&which>=AGE&&which<=ID){
						getchar();
						search(who,which);
					}
					else
						printf("\nInvalid Search Option Selection\n");
				}
				else 
					printf("\nInvalid Search Target Selection\n");
				break;
			}
			case CHANGE:{
				change(getIdentity());
				break;
			}
			case DELETE:{
				delete(getIdentity());
				break;
			}
			case VIEW:{
				view(getIdentity());
				break;
			}
			case SORT:{
				sort(getIdentity());
				break;
			}
		}
	printf("\t\t Operation Complete\n");
	puts("\n\n");
	showMenu();
	}
	printf("Bye\n");
	return 0;
}













































































































































































































































