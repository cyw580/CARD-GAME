#include<bits/stdc++.h>
#include<windows.h>
#include<conio.h> 
#include<iostream>
using namespace std;

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define SPACE 32
#define ENTER 13
//ASCII 

void SetPos(int x, int y)
{
        COORD coord;
        coord.X = x;
        coord.Y = y;

        //HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
        //SetConsoleCursorPosition(hout, coord);//������Ĵ���һ������

        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
};

void SetColor(short x)
{
  if(x>=0 && x<=15)
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), x);
  else
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

void SetColor(short x,short y)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), x+16*y);
}

void SetSize(int x,int y){
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SMALL_RECT rc = {1,1,x,y};
	SetConsoleWindowInfo(hOut ,true ,&rc);
}

void printf(string s){
	cout<<s;
}

void Shake(int power,int time){
	int shake_time = 25; //���ߵ�ʱ�䣬Ϊ5����
    int shake_distance = power; //�ƶ���10����
    RECT rect; //RECT��һ�����νṹ�壬�൱�ڱ�����һ�����ε������ߵ�����
    HWND window = NULL, oldwindow = NULL; //�������ھ��
    int x, y, width, height; //�������洰�ں�������Ϳ�ȡ��߶ȵı���
    int i;
    for (i = 0; i < time; i++) {
        window = GetForegroundWindow(); //�õ������
        if (window != oldwindow) {
            //��ȡָ�����ڵ�λ��
            GetWindowRect(window, &rect);
            x = rect.left;
            y = rect.top;
            width = rect.right - x;
            height = rect.bottom - y;
            oldwindow = window;
        }
        MoveWindow(window, x - shake_distance, y, width, height, TRUE); //�ƶ����ڣ������ƶ���10���أ���ͬ
        Sleep(shake_time);  //����time���룬�߳�����
        MoveWindow(window, x - shake_distance, y - shake_distance, width, height, TRUE);
        Sleep(shake_time);
        MoveWindow(window, x, y - shake_distance, width, height, TRUE);
        Sleep(shake_time);
        MoveWindow(window, x, y, width, height, TRUE);
        Sleep(shake_time);
    }
    return;
}

//--------------------------------------------------

char input;

struct Card{
	string name;
	int cost,func;
	int ATK,DEF,HEAL,MISS;
	int tag;
	void Use(int from,int to);
	int Special(int from,int to);
	string Intro();
};
//Card heap[1005];
//int heapn;
int winner;

struct player{
	string name,job;
	int cost,maxcost,rest,heapn,occ;
	int hp,def,cardcnt,prehp;
	int maxhp,maxdef;
	Card handcard[10],heap[105];
	int used[10];
	int buff[20];
	//0-ְҵ����,1-ȼ��,2-�ж�,3-��,4-����
	player(){
		hp=maxhp=400;def=0;cardcnt=4;
		maxdef=100;maxcost=6;
	}
	int hprate(){
		return 10*hp/maxhp;
	}
	void UpdateBuff(){
		for(int i=1;i<=10;i++){
			buff[i]=max(0,buff[i]-1);
		}
	}
}pl[5];

//-------------------------------------------------
int UI();
int Sort(int now);

int Check(int now){
	if(pl[now].hp<1){
		winner=3-now;
		return 1;
	}
	return 0;
}

void Card::Use(int from,int to){
	int damage=ATK,damage2=ATK;
	if(pl[from].occ==2 && ATK>0) damage=damage2=ATK+5*pl[from].buff[0];
	if(pl[from].occ==5 && ATK>0) damage=damage2=ATK+10*pl[from].buff[0];
	if(rand()%100<MISS){
		SetPos(0,1);
		printf("����ʧ����!");
		if(pl[from].occ==4 && ATK>0) pl[from].buff[0]=1;
		UI();
		return;
	}
	//ʧЧ�ж�
	int flag;
	if(func) flag=Special(from,to) ;
	if(pl[from].buff[3]){
		damage*=2;damage2*=2;
	}
	if(pl[from].occ==4&&pl[from].buff[0]) damage*=0.6;damage2*=0.6;
	if(pl[to].def>0 && flag!=1){
		damage=max(0,damage-pl[to].def);
		pl[to].def=max(0,pl[to].def-damage2);
		if(pl[to].def==0){
			Shake(3,1);//���ƻ��ܵĶ���
		}
	}
	if(damage>0){
		pl[to].hp-=damage;
	}
	if(damage>=150){
		Shake(10,1);
	}
	//����
	pl[from].def=min(pl[from].maxdef,pl[from].def+DEF);
	pl[from].hp=min(pl[from].maxhp,pl[from].hp+HEAL);
	//�׶����Ѫ
	if(HEAL<0 && pl[from].occ==2)pl[from].buff[0]++;
	if(pl[from].occ==4 && ATK>0) pl[from].buff[0]=1;
	UI();
	Check(to);Check(from);
}

int Card::Special(int from,int to){
	int damage=ATK;
	if(pl[from].occ==2 && ATK>0) damage=ATK+5*pl[from].buff[0];
	if(pl[from].occ==5 && ATK>0) damage=ATK+10*pl[from].buff[0];
	if(pl[from].buff[3]){
		damage*=2;
	}
	if(func==1){
		pl[from].cost=min(pl[from].maxcost,pl[from].cost+4);
	}
	else if(func==2){
		pl[from].maxcost++;
		pl[from].cost++;
	}
	else if(func==3){
		for(int now=1;now<=2;now++)
			pl[now].cost=min(pl[now].maxcost,pl[now].cost+2);
	}
	else if(func==4){
		if(damage>=pl[to].def)
			pl[to].buff[1]+=3;
	}
	else if(func==5){
		pl[from].buff[3]=1;
	}
	else if(func==6){
		if(damage>=pl[to].def||pl[from].occ==5)
			pl[to].buff[2]+=3;
	}
	else if(func==7){
		for(int i=1;i<=pl[from].cardcnt;i++){
			if(pl[from].used[i]){
				pl[from].used[i]=0;
				pl[from].handcard[i]=pl[from].heap[(rand()%pl[from].heapn)+1];
			}
		}
		pl[from].rest=pl[from].cardcnt;
	}
	else if(func==8){
		pl[from].cardcnt++;
		pl[from].used[pl[from].cardcnt]=1;
	}
	else if(func==9){
		pl[from].maxhp+=80;
	}
	else if(func==10){
		pl[from].maxcost--;
	}
	else if(func==11){
		pl[from].maxhp-=80;
		pl[from].hp=min(pl[from].hp,pl[from].maxhp);
	}
	else if(func==12){
		pl[from].buff[0]+=2;
	}
	else if(func==13){
		pl[to].hp-=pl[to].hp*0.35;
	}
	else if(func==14){
		pl[to].buff[3]=2;
	}
	else if(func==15){
		int da=15+30*pl[from].cost;
		pl[from].cost=0;
		if(da<pl[to].def) pl[to].def-=da;
		else pl[to].hp-=da-pl[to].def,pl[to].def=0;
	}
	else if(func==16){
		pl[to].cost=max(0,pl[to].cost-3);
	}
	else if(func==17){
		pl[from].buff[3]=1;
		pl[to].buff[3]=2;
	}
	else if(func==18){
		pl[from].cost=min(pl[from].maxcost,pl[from].cost+2);
	}
	else if(func==19){
		pl[from].buff[0]++;
		pl[from].maxhp+=10;
		pl[from].maxdef+=10;
		pl[from].hp=min(pl[from].maxhp,pl[from].hp+20);
	}
	else if(func==20){
		return 1;
	}
	else if(func==21){
		
	}
	else if(func==22){
		
	}
	else if(func==23){
		
	}
	else if(func==24){
		
	}
	return 0;
}

string Card::Intro(){
	if(func==1) return "ʹ�ú��÷���4��";
	else if(func==2)return "ʹ�ú���1����ã�������1���������";
	else if(func==3)return "ʹ�ú������˻��2�����";
	else if(func==4)return "ʹ�Է�����ȼ���غϣ�ÿ�غ���ʧ40HP,���ƻ��ף������ƻ���ʱ����Ч��";
	else if(func==5)return "ʹ��������״̬���غϽ�������ɵ��˺���Ϊԭ����2����";
	else if(func==6)return "ʹ�Է��ж����غϣ�ÿ�غ���ʧ����HP��20%,���ƻ��ף���ְҵΪ�ؾ�����ƻ���ʱ����Ч��";
	else if(func==7)return "���̲����������";
	else if(func==8)return "ʹ�ú���������+1";
	else if(func==9)return "ʹ�ú�HP����+80";
	else if(func==10)return "ʹ�ú��������-1";
	else if(func==11)return "ʹ�ú�HP����-80";
	else if(func==12)return "ʹ�ú��»غ϶�����2�����";
	else if(func==13)return "ʹ�Է���ʧ35%����ֵ";
	else if(func==14)return "�ڶԷ��»غϽ���ǰ �Է������״̬����ɵ��˺���Ϊԭ����2����";
	else if(func==15)return "�������з��ã���ɣ�30*����+15�����˺�";
	else if(func==16)return "ʹ�ú�Է�����-3";
	else if(func==17)return "���ڱ��غϡ��Է����»غ϶�������״̬����ɵ��˺���Ϊԭ����2����";
	else if(func==18)return "ʹ�ú��÷���2��";
	else if(func==19)return "�ɳ�+1";
	else if(func==20)return "��͸���ܵ���ʵ�˺�";
	return "";
}

void init(int x){
	string s=pl[x].job+".inf";
	freopen("public.inf","r",stdin);
	int tot;
	scanf("%d",&tot);
	for(int i=1;i<=tot;i++){
		cin>>pl[x].heap[i].name;
		scanf("%d%d%d%d%d%d",&pl[x].heap[i].cost,&pl[x].heap[i].ATK,&pl[x].heap[i].HEAL,&pl[x].heap[i].DEF,&pl[x].heap[i].MISS,&pl[x].heap[i].func);
		pl[x].heap[i].tag=i;
		if(pl[x].occ==4 && pl[x].heap[i].HEAL>0) i--,tot--;
		if(pl[x].occ==2 && pl[x].heap[i].HEAL>=50) i--,tot--;
		if(pl[x].occ==5 && (pl[x].heap[i].DEF>30 || pl[x].heap[i].ATK>20)) i--,tot--;
	}
	pl[x].heapn=tot;
	fclose(stdin);
	ifstream fin;
	fin.open(s);
	fin>>tot;
	for(int i=pl[x].heapn+1;i<=pl[x].heapn+tot;i++){
		fin>>pl[x].heap[i].name>>pl[x].heap[i].cost>>pl[x].heap[i].ATK>>pl[x].heap[i].HEAL>>pl[x].heap[i].DEF>>pl[x].heap[i].MISS>>pl[x].heap[i].func;
		pl[x].heap[i].tag=i;
	}
	pl[x].heapn+=tot;
	for(int i=1;i<=pl[x].cardcnt;i++){
		pl[x].handcard[i]=pl[x].heap[(rand()%pl[x].heapn)+1];
	}
	fin>>pl[x].maxhp>>pl[x].cardcnt>>pl[x].maxdef>>pl[x].maxcost;
	pl[x].hp=pl[x].maxhp;
	fin.close();
	if(pl[x].occ==3) pl[x].buff[0]=1;
}

string occ_name(int x){
	if(x==1)return "����";
	else if(x==2)return "��ʿ";
	else if(x==3)return "��ʦ";
	else if(x==4)return "սʿ";
	else if(x==5)return "�ؾ�";
	return "";
}

string occ_file(int x){
	if(x==1)return "traveller";
	else if(x==2)return "caster";
	else if(x==3)return "wizard";
	else if(x==4)return "fighter";
	else if(x==5)return "goblin";
	return "";
}

string occ_intro(int x){
	if(x==1)return "û�г�����Ҳͬ��û������";
	else if(x==2)return "�����������Ѫ��ȡ��֮����";
	else if(x==3)return "���������壬����������Ҫ";
	else if(x==4)return "��ʵ�ķ�����ǿ�������";
	else if(x==5)return "���������ݣ�ͨ�������������";
	return "";
}
string occ_func(int x){
	if(x==1)return "����: HP420 ���DEF100 ��������4 ��������6\n1.����ְҵ�����ƶ��й��оأ��Ҷ�����Ч";
	else if(x==2)return "����: HP560 ���DEF80 ��������4 ��������5\n1.ְҵ��ǣ�������ÿ����Ϊ�Լ����ƶ��ܵ��˺�ʱ����1����ǣ�ÿ�����ʹ�ù���������5\n2.�����ƿ��У��ظ��� �� 50�����Ʋ���鵽\n3.һЩְҵ�����ƻ�ͨ�������Լ����������";
	else if(x==3)return "����: HP300 ���DEF100 ��������4 ��������7\n1.ְҵ��ǣ�������ÿ�غϿ�ʼ��30%���1����ǣ����»غϻ�õ�ͬ�ڱ�������ķ��ò���������\n2.���˺��߱���";
	else if(x==4)return "����: HP480 ���DEF200 ��������4 ��������6\n1.ְҵ��ǣ�ƣ�����غϿ�ʼʱ�����գ�ÿ�غϵ�һ��ʹ���й������ƺ������Ƿ�miss����ñ�ǣ���Ǵ���ʱ��������Ϊԭ����60%\n2.ÿ�غϿ�ʼʱ��û�л��� �򻤼�+30\n3.���ں�ս�������ƿ�������������Ч�����ƶ�����鵽";
	else if(x==5)return "����: HP280 ���DEF0 ��������4 ��������3\n1.ְҵ��ǣ��ɳ���ÿ�����ʹ���ù�����+10��HP����+10�����DEF+10��ÿ�λ�ñ��ʱ�ָ�20HP\n2.ÿ�غ϶���3�ѣ���ÿʹ��һ���ƣ��������ƣ����Զ���һ����\n3.����������С�������ƿ��У�ATK>20 �� DEF>30�������޷��鵽�����������ݣ��д�͸���ܹ���������";
	return "";
}

void Choose(int now){
	SetPos(0,5);
	printf("��ѡ��P%d��ְҵ��",now);
	int curse=1;
	while(1){
		for(int i=1;i<=5;i++){	
			if(curse!=i)
				SetColor(7,0);
			else
				SetColor(0,7);
			SetPos(0,5+i);
			printf("                                                             ");
			SetPos(0,5+i);
			printf("%d ",i);
			printf(occ_name(i));
			printf(" | ");
			printf(occ_intro(i));
		}
		input=getch();
		if(input==UP || input==LEFT)
			curse--;
		if(input==DOWN || input==RIGHT)
			curse++;
		if(curse>5)curse=1;
		if(curse<1)curse=5;
		if(input==SPACE || input==ENTER || input=='z'){
			pl[now].occ=curse;
			pl[now].job=occ_file(curse);
			return;
		}
	}
}

int UI(){
	for(int rnk=1;rnk<=2;rnk++){
		int i;
		//���/����/ְҵ
		SetColor(15);
		SetPos(5,rnk*4);
		printf("#%d ",rnk);
		printf(pl[rnk].name);
		SetPos(5,rnk*4+1);
		printf("   ");
		printf(occ_name(pl[rnk].occ));
		
		//buff0
		if(pl[rnk].occ>=2){
			SetColor(13);
			SetPos(8,rnk*4+2);
			printf("��%d   ",pl[rnk].buff[0]);
		}

		//���û���
		SetColor(11);
		SetPos(17,rnk*4);
		printf("%d/%d ",pl[rnk].cost,pl[rnk].maxcost);
		SetPos(26,rnk*4);
		for(i=1;i<=pl[rnk].cost;i++)
			printf("��");
		for(;i<=pl[rnk].maxcost;i++)
			printf("��");
		printf("                                               ");
		
		//Ѫ������
		if(pl[rnk].hprate()>5)SetColor(10);
		else if(pl[rnk].hprate()>2)SetColor(6);
		else SetColor(12);
		SetPos(17,rnk*4+1);
		printf("%d/%d            ",pl[rnk].hp,pl[rnk].maxhp);

		//���׻���
		SetColor(15);
		SetPos(17,rnk*4+2);
		printf("[%d/%d]            ",pl[rnk].def,pl[rnk].maxdef);
		
		//Buff����
		SetPos(26,rnk*4+2);
		if(pl[rnk].buff[1])
			SetColor(6),printf("ȼ�� %d : ",pl[rnk].buff[1]);
		if(pl[rnk].buff[2])
			SetColor(2),printf("�ж� %d : ",pl[rnk].buff[2]);
		if(pl[rnk].buff[3])
			SetColor(4),printf("�� %d : ",pl[rnk].buff[3]);
		printf("                                                 ");
		
		//Ѫ������
		SetPos(26,rnk*4+1);
		if(pl[rnk].hprate()>5)SetColor(10);
		else if(pl[rnk].hprate()>2)SetColor(6);
		else SetColor(12);
		
		for(i=1;i<= pl[rnk].hp/40;i++)
			printf("��");
		SetColor(8);
		for(;i<= pl[rnk].prehp/40;i++)
			printf("��");
		if(pl[rnk].hprate()>5)SetColor(10);
		else if(pl[rnk].hprate()>2)SetColor(6);
		else SetColor(12);
		for(;i<=pl[rnk].maxhp/40;i++)
			printf("��");
		SetColor(7);
		for(i=1;i<=pl[rnk].def/20;i++)
			printf("��");
		printf("                                                       ");
	}
	return 0; 
}

int Sort(int now){//��������
	for(int i=1;i<=pl[now].cardcnt;i++){
		if(pl[now].used[i]){
			pl[now].handcard[i]=pl[now].heap[(rand()%pl[now].heapn)+1];
			pl[now].used[i]=0;
		}
	}
	return 0;
}

int Ask(int now){
	int Row=13,option=0;
	if(pl[now].occ==3){
		if(pl[now].buff[0]){
			pl[now].cost=min(pl[now].maxcost,pl[now].cost+pl[now].buff[0]);
			pl[now].buff[0]=0;
		}
		if(rand()%100<30) pl[now].buff[0]++;
	}
	if(pl[now].occ==4) {
		if(pl[now].def==0) pl[now].def=30;
		pl[now].buff[0]=0;
	}
	if(pl[now].occ==5) pl[now].cost=3;
	if(pl[now].buff[1])
		pl[now].hp-=40;
	if(pl[now].buff[2])
		pl[now].hp-=pl[now].hp*0.2;
	pl[now].UpdateBuff();
	UI();
	if(Check(now))
		return 0;
	//buffǰ��
	Sort(now);
	pl[now].rest=pl[now].cardcnt;
	SetColor(7);
	int curse=1;
	SetPos(0,Row);
	printf("P%d",now); 
	SetPos(5,Row);
	printf("  #"); 
	SetPos(11,Row);SetColor(11);
	printf("��");
	SetPos(18+14,Row);SetColor(7);
	printf("ATK");
	SetPos(26+14,Row);
	printf("HEAL");
	SetPos(34+14,Row);
	printf("DEF");
	SetPos(42+14,Row);
	printf("MISS");
	while(!winner){
		for(int i=1;i<=pl[now].cardcnt;i++){
			int color=0;
			// SetPos(1,11+i);
			// printf("%d",pl[now].used[i]);//debug
			SetPos(5,Row+i);
			if(pl[now].used[i])SetColor(8);
			if(curse==i && !option) SetColor(color=15),printf("��%d",i); 
			else if(curse==i && option) SetColor(color=15),printf("��%d",i); 
			else SetColor(color=7),printf("  %d",i); 
			if(pl[now].used[i]){
				SetColor(8);
				printf("                                                              ");
				SetPos(14,Row+i);
				printf("[Used.]");
				continue;
			}
			SetPos(11,Row+i);
			if(pl[now].cost>=pl[now].handcard[i].cost)SetColor(11);
			else SetColor(3);
			printf("%d",pl[now].handcard[i].cost);
			SetColor(color);
			SetPos(14,Row+i);
			printf(pl[now].handcard[i].name);
			printf("               ");
			SetPos(18+14,Row+i);
			printf("%d  ",pl[now].handcard[i].ATK);
			SetPos(26+14,Row+i);
			printf("%d  ",pl[now].handcard[i].HEAL);
			SetPos(34+14,Row+i);
			printf("%d  ",pl[now].handcard[i].DEF);
			SetPos(42+14,Row+i);
			printf("%d%%  ",pl[now].handcard[i].MISS);
		}//��ʾ����
		SetColor(7);
		SetPos(0,0);
		printf("%d",curse);
		SetPos(11,Row+7);
		printf("                                                                    ");
		SetPos(11,Row+7);
		printf(pl[now].handcard[curse].Intro());
		input=getch();
		if(input==LEFT || input==UP){
			option=0;
			curse--;
			while(pl[now].used[curse]&&curse>0){
				curse--;
				if(curse<=0)curse=pl[now].cardcnt;
			}
		}
		if(input==RIGHT || input==DOWN){
			option=0;
			curse++;
			while(pl[now].used[curse]&&curse<=pl[now].cardcnt+1){
				curse++;
				if(curse>pl[now].cardcnt)curse=1;
			}
		}
		if(curse<=0)curse=pl[now].cardcnt;
		if(curse>pl[now].cardcnt)curse=1;//�ƶ�����
		if(input=='z')//ʹ��
		{
			option=0;
			if(pl[now].used[curse]){
				SetPos(0,1);
				printf("ѡ�е��Ʋ����ڣ�                 ");
			}
			else if(pl[now].cost<pl[now].handcard[curse].cost){
				SetPos(0,1);
				printf("���ò����޷�ʹ�ô��ƣ�                 ");
			}
			else{
				pl[now].cost-=pl[now].handcard[curse].cost;
				pl[now].used[curse]=1;
				pl[now].rest--;
				pl[now].handcard[curse].Use(now,3-now);
				if(pl[now].occ==5) {
					pl[now].used[curse]=0;
					pl[now].rest++;
					pl[now].handcard[curse]=pl[now].heap[(rand()%pl[now].heapn)+1];
				}
				while(pl[now].used[curse] && curse<=pl[now].cardcnt+1 && pl[now].rest){
					curse++;
					if(curse>pl[now].cardcnt)curse=1;
				}
			}
		}
		if(input=='x'){//����
			if(!option){
				option=1;
			}
			else{
				pl[now].used[curse]=1;
				pl[now].rest--;
				while(pl[now].used[curse] && curse<=pl[now].cardcnt+1 && pl[now].rest){
					curse++;
					if(curse>pl[now].cardcnt)curse=1;
				}
				option=0;
			}
		}
		if(input==SPACE || input==ENTER || pl[now].rest<=0)//�����غ�
			break; 
		Check(now);
	}
	return 0;
}

int main(){
//	SetConsoleOutputCP(65001);
	srand(time(NULL));
	printf("WELCOME!!\n");
	for(int i=1;i<=2;i++){
		printf("������P%d������:",i);
		cin>>pl[i].name;
	}
	printf("�������ʼ����:");
	int start=0;
	scanf("%d",&start);
	// printf("1.���ˣ�2.��ʿ��3.��ʦ��4.սʿ��5.�ؾ�\n");
	// for(int i=1;i<=2;i++){
	// 	printf("��P%dѡ��ְҵ:",i);
	// 	cin>>pl[i].occ;
	// 	if(pl[i].occ[0]<'0'||pl[i].occ[0]>'5') i--;
	// }
	for(int now=1;now<=2;now++){
		Choose(now);//ѡ��ְҵ
		SetPos(0,10+now);
		printf("P%d��ְҵ��",now);
		printf(occ_name(pl[now].occ));
	}
	for(int i=1;i<=2;i++){
		pl[i].cost=min(start,pl[i].maxcost);
	}
	Sleep(2000);
	system("cls");
	for(int i=1;i<=2;i++) init(i);
	int now=1;
	while(!Check(now)){
		for(int i=1;i<=2;i++){
			pl[i].prehp=pl[i].hp;
		}
		pl[now].cost=min(pl[now].cost+1,pl[now].maxcost);//�ӷ� 
		UI();
		Ask(now);
		//debug
		system("cls");
		now++;
		if(now>2)now=1;
	}
	system("cls");
	printf("#%d ",winner);
	printf(pl[winner].name);
	printf("�����ʤ��!!");
	while(1){
		SetColor(rand()%16);
		SetPos(rand()%100,rand()%30);
		printf("#%d ",winner);
		printf(pl[winner].name);
		printf("�����ʤ��!!");
		Sleep(10);
	}
	return 0;
}
