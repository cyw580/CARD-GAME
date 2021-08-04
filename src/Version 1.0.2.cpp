#include<bits/stdc++.h>
#include<windows.h>
#include<conio.h> 
#include<iostream>
using namespace std;

#include"PreInfo.h"

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define SPACE 32
#define ENTER 13
//ASCII 

char input;

int winner;

struct player{
	string name;
	int cost,maxcost,rest,heapn,occ;
	int hp,def,cardcnt,prehp;
	int maxhp,maxdef;
	Card handcard[15],heap[1005];
	int used[15];
	int buff[20];
	//0-职业特性,1-燃烧,2-中毒,3-狂暴,4-虚弱,5-治疗
	int hprate(){
		return 10*hp/maxhp;
	}
	void UpdateBuff(){
		for(int i=1;i<=10;i++){
			buff[i]=max(0,buff[i]-1);
		}
	}
}pl[15];

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
	if(rand()%100<MISS){
		SetPos(0,1);
		printf("操作失误了!");
		if(pl[from].occ==4 && ATK>0) pl[from].buff[0]=1;
		UI();
		return;
	}//失效判定
	int damage=ATK,damage2=ATK,flag;
	if(func) flag=Special(from,to);
	if(pl[from].occ==2 && ATK>0) damage=damage2=ATK+5*pl[from].buff[0];
	if(pl[from].occ==5 && ATK>0) damage=damage2=ATK+8*pl[from].buff[0];
	if(pl[from].occ==4 && pl[from].buff[0]){
		damage*=0.7;damage2*=0.7;
	}
	if(pl[from].buff[3]){
		damage*=2;damage2*=2;
	}
	if(pl[to].def>0 && flag!=1){
		damage=max(0,damage-pl[to].def);
		pl[to].def=max(0,pl[to].def-damage2);
		if(pl[to].def==0){
			Shake(3,1);//击破护盾的动画
		}
	}
	if(damage>0){
		pl[to].hp-=damage;
	}
	if(damage>=150){
		Shake(10,1);
	}
	//进攻
	pl[from].def=min(pl[from].maxdef,pl[from].def+DEF);
	pl[from].hp=min(pl[from].maxhp,pl[from].hp+HEAL);
	//套盾与回血
	if(HEAL<0 && pl[from].occ==2) pl[from].buff[0]++;
	if(pl[from].occ==4 && ATK>0) pl[from].buff[0]=1;
	if(pl[from].occ==5 && flag==2){
		pl[from].buff[0]++;
		pl[from].maxhp+=15;
		pl[from].maxdef+=10;
		pl[from].hp=min(pl[from].maxhp,pl[from].hp+20);
	}
	UI();
	Check(to);Check(from);
}

int Card::Special(int from,int to){
	int damage=ATK;
	if(pl[from].occ==2 && ATK>0) damage=ATK+5*pl[from].buff[0];
	if(pl[from].occ==5 && ATK>0) damage=ATK+8*pl[from].buff[0];
	if(pl[from].occ==4 && pl[from].buff[0]){
		damage*=0.7;
	}
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
		pl[from].maxhp+=40;
	}
	else if(func==10){
		pl[from].maxcost--;
	}
	else if(func==11){
		pl[from].maxhp-=60;
		pl[from].hp=min(pl[from].hp,pl[from].maxhp);
	}
	else if(func==12){
		pl[from].buff[0]+=2;
	}
	else if(func==13){
		pl[to].hp-=pl[to].hp*0.25;
	}
	else if(func==14){
		pl[to].buff[3]=2;
	}
	else if(func==15){
		int da=25+35*pl[from].cost;
		if(pl[from].buff[3]) da=da*2;
		if(pl[from].buff[0]) da=da*0.7;
		pl[from].cost=0;
		if(da<pl[to].def) pl[to].def-=da;
		else {
			if(pl[to].def>0) Shake(3,1);
    		pl[to].hp-=da-pl[to].def,pl[to].def=0;
    		if(da-pl[to].def>=150) Shake(10,1);
		}
		pl[from].buff[0]=1;
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
		return 2;
	}
	else if(func==20){
		return 1;
	}
	else if(func==21){
		pl[to].def=min(pl[to].def+50,pl[to].maxdef);
	}
	else if(func==22){
		pl[from].maxhp-=20;
		pl[from].hp=min(pl[from].hp,pl[from].maxhp);
		pl[from].buff[0]++;
	}
	else if(func==23){
		pl[from].buff[5]+=3;
	}
	else if(func==24){
		pl[from].maxhp-=80;
		pl[from].hp=pl[from].maxhp;
		pl[from].buff[0]=0;
	}
	else if(func==25){
		pl[from].buff[0]=0;
		pl[from].cost+=2;
	}
	return 0;
}

string Card::Intro(){
	if(func==1) return "+4◆";
	else if(func==2)return "+1◆并+1◆上限";
	else if(func==3)return "所有人+2◆";
	else if(func==4)return "对方被点燃3回合（每回合损失40HP）（击破护盾时才生效）";
	else if(func==5)return "自身进入狂暴状态至回合结束（造成的伤害变为原来的2倍）";
	else if(func==6)return "对方中毒3回合（每回合损失现有HP的20%）（职业为地精或击破护盾时才生效）";
	else if(func==7)return "立刻补充你的手牌";
	else if(func==8)return "手牌上限+1";
	else if(func==9)return "HP上限+40";
	else if(func==10)return "-1◆上限";
	else if(func==11)return "HP上限-60";
	else if(func==12)return "下回合额外+2◆";
	else if(func==13)return "对方损失现有HP的25%";
	else if(func==14)return "对方下回合结束前 对方进入狂暴状态（造成的伤害变为原来的2倍）";
	else if(func==15)return "消耗所有费用，造成（35*费用+25）的伤害";
	else if(func==16)return "对方-3◆";
	else if(func==17)return "你在本回合、对方在下回合都进入狂暴状态（造成的伤害变为原来的2倍）";
	else if(func==18)return "+2◆";
	else if(func==19)return "+1<★成长>标记";
	else if(func==20)return "穿透护盾的真实伤害";
	else if(func==21)return "对方获得50护甲";
	else if(func==22)return "HP上限-20，+1<★牺牲>标记";
	else if(func==23)return "接下去3回合在每回合开始时恢复30HP";
	else if(func==24)return "HP上限-80，仅清空<★牺牲>标记，恢复到满血";
	else if(func==25)return "+2◆并清除<★疲惫>标记";
	else if(func==26)return "不要太贪心！";
	return "";
}

void init(int x){
	int tot=libcnt[0];
	for(int i=1;i<=tot;i++){
		pl[x].heap[i]=lib[0][i];
		if(pl[x].occ==3 && pl[x].heap[i].HEAL>=80) i--,tot--;
		if(pl[x].occ==4 && pl[x].heap[i].HEAL>0) i--,tot--;
		if(pl[x].occ==5) i--,tot--;
	}
	pl[x].heapn=tot;
	tot=libcnt[pl[x].occ];
	for(int i=1;i<=tot;i++) pl[x].heap[i+pl[x].heapn]=lib[pl[x].occ][i];
	pl[x].heapn+=tot;
	for(int i=1;i<=pl[x].cardcnt;i++){
		pl[x].handcard[i]=pl[x].heap[(rand()%pl[x].heapn)+1];
	}
	pl[x].maxhp=job[pl[x].occ].maxhp;
	pl[x].cardcnt=job[pl[x].occ].cardcnt;
	pl[x].maxdef=job[pl[x].occ].maxdef;
	pl[x].maxcost=job[pl[x].occ].maxcost;
	pl[x].hp=pl[x].maxhp;
	if(pl[x].occ==3) pl[x].buff[0]=1;
}
void adv(int x){
	if(pl[x].occ==1){
		pl[x].def=40;
	}
	else if(pl[x].occ==2){
		pl[x].buff[5]=2;
	}
	else if(pl[x].occ==3){
		pl[x].hp+=30;
		pl[x].maxhp+=30;
	}
	else if(pl[x].occ==4){
		pl[x].def=60;
	}
	else if(pl[x].occ==5){
		pl[x].hp+=10;
		pl[x].maxhp+=10;
		pl[x].def+=20;
		pl[x].maxdef+=20;
	}
	return;
}

string occ_name(int x){
	if(x==1)return "浪人";
	else if(x==2)return "术士";
	else if(x==3)return "法师";
	else if(x==4)return "战士";
	else if(x==5)return "地精";
	else if(x==6)return "随缘";
	return "";
}

// string occ_file(int x){
// 	if(x==1)return "traveller";
// 	else if(x==2)return "caster";
// 	else if(x==3)return "wizard";
// 	else if(x==4)return "warrior";
// 	else if(x==5)return "goblin";
// 	return "";
// }

void Choose(int now){
	SetPos(0,1);
	printf("          ◇选择P%d ",now);
	printf(pl[now].name);
	printf("的职业◇");
	int curse=1;
	while(1){
		for(int i=1;i<=6;i++){	
			if(curse!=i)
				SetColor(7,0);
			else
				SetColor(0,7);
			SetPos(0,i+1);
			printf("                                                             ");
			SetPos(0,i+1);
			printf("   %d ",i);
			printf(occ_name(i));
			printf(" | ");
			printf(occ_intro(i));
		}
		SetPos(0,12);
		occ_func(curse);
		input=getch();
		if(input==UP || input==LEFT || input=='w' || input=='a')
			curse--;
		if(input==DOWN || input==RIGHT || input=='s' || input=='d')
			curse++;
		if(input>='1' && input<='6'){
			if(input-'0'==curse){
				if(curse<=5){
					pl[now].occ=curse;
					return;
				}
				else {
					pl[now].occ=rand()%5+1;
					return;
				}
			}
			else
				curse=input-'0';
		}
		if(curse>6) curse=1;
		if(curse<1) curse=6;
		if(input==SPACE || input==ENTER || input=='z'){
			if(curse<=5){
				pl[now].occ=curse;
				return;
			}
			else {
				pl[now].occ=rand()%5+1;
				return;
			}
		}
	}
}

int UI(){
	for(int rnk=1;rnk<=2;rnk++){
		int i;
		//编号/名字/职业
		SetColor(15);
		SetPos(5,rnk*4-2);
		printf("#%d ",rnk);
		printf(pl[rnk].name);
		SetPos(5,rnk*4+1-2);
		printf("   ");
		printf(occ_name(pl[rnk].occ));
		
		//buff0
		if(pl[rnk].occ>=2){
			SetColor(13);
			SetPos(8,rnk*4+2-2);
			printf("★%d   ",pl[rnk].buff[0]);
		}

		//费用绘制
		SetColor(11);
		SetPos(17,rnk*4-2);
		printf("%d/%d ",pl[rnk].cost,pl[rnk].maxcost);
		SetPos(26,rnk*4-2);
		for(i=1;i<=pl[rnk].cost;i++)
			printf("◆");
		for(;i<=pl[rnk].maxcost;i++)
			printf("◇");
		printf("                                               ");
		
		//血量绘制
		if(pl[rnk].hprate()>5)SetColor(10);
		else if(pl[rnk].hprate()>2)SetColor(6);
		else SetColor(12);
		SetPos(17,rnk*4+1-2);
		printf("%d/%d            ",pl[rnk].hp,pl[rnk].maxhp);

		//护甲绘制
		SetColor(15);
		SetPos(17,rnk*4+2-2);
		printf("[%d/%d]            ",pl[rnk].def,pl[rnk].maxdef);
		
		//Buff绘制
		SetPos(26,rnk*4+2-2);
		if(pl[rnk].buff[1])
			SetColor(6),printf("燃烧 %d : ",pl[rnk].buff[1]);
		if(pl[rnk].buff[2])
			SetColor(2),printf("中毒 %d : ",pl[rnk].buff[2]);
		if(pl[rnk].buff[3])
			SetColor(4),printf("狂暴 %d : ",pl[rnk].buff[3]);
		if(pl[rnk].buff[4])
			SetColor(4),printf("XX %d : ",pl[rnk].buff[4]);
		if(pl[rnk].buff[5])
			SetColor(10),printf("治疗 %d : ",pl[rnk].buff[5]);//修改颜色
		printf("                                                 ");
		
		//血条绘制
		SetPos(26,rnk*4+1-2);
		if(pl[rnk].hprate()>5)SetColor(10);
		else if(pl[rnk].hprate()>2)SetColor(6);
		else SetColor(12);
		
		for(i=1;i<= pl[rnk].hp/40;i++)
			printf("■");
		SetColor(8);
		for(;i<= pl[rnk].prehp/40;i++)
			printf("■");
		if(pl[rnk].hprate()>5)SetColor(10);
		else if(pl[rnk].hprate()>2)SetColor(6);
		else SetColor(12);
		for(;i<=pl[rnk].maxhp/40;i++)
			printf("□");
		SetColor(7);
		for(i=1;i<=pl[rnk].def/20;i++)
			printf("■");
		for(i=1;i<=30-pl[rnk].maxhp/40-pl[rnk].def/20;i++)
		printf(" ");
	}
	return 0; 
}

int Ask(int now){
	int Row=11,option_use=0,option_giveup=0,option_over=0;
	if(pl[now].occ==3){
		if(pl[now].buff[0]){
			pl[now].cost=min(pl[now].maxcost,pl[now].cost+pl[now].buff[0]);
			pl[now].buff[0]=0;
		}
		if(rand()%100<40) pl[now].buff[0]++;
	}
	if(pl[now].occ==4) {
		if(pl[now].def==0) pl[now].def=40;
		pl[now].buff[0]=0;
	}
	if(pl[now].occ==5) pl[now].cost=3;
	if(pl[now].buff[5])
		pl[now].hp=min(pl[now].hp+30,pl[now].maxhp);
	if(pl[now].buff[1])
		pl[now].hp-=40;
	if(pl[now].buff[2])
		pl[now].hp-=pl[now].hp*0.2;
	pl[now].UpdateBuff();
	UI();
	if(Check(now))
		return 0;
	//buff前判
	for(int i=1;i<=pl[now].cardcnt;i++){
		if(pl[now].used[i]){
			pl[now].handcard[i]=pl[now].heap[(rand()%pl[now].heapn)+1];
			pl[now].used[i]=0;
		}
	}
	//补充手牌
	pl[now].rest=pl[now].cardcnt;
	SetColor(7);
	int curse=1;
	SetPos(0,Row);
	printf("P%d",now); 
	SetPos(5,Row);
	printf("  #"); 
	SetPos(11,Row);SetColor(11);
	printf("◆");
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
			if(curse==i && !option_giveup && !option_use) SetColor(color=15),printf("◎%d",i); 
			else if(curse==i && option_giveup) SetColor(color=15),printf("×%d",i); 
			else if(curse==i && option_use) SetColor(color=15),printf("●%d",i); 
			else SetColor(color=7),printf("  %d",i); 
			if(pl[now].used[i]){
				SetColor(8);
				printf("                                                               ");
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
		}//显示手牌
		SetColor(7);
		SetPos(11,Row+9);
		printf("                                                                         ");
		SetPos(11,Row+9);
		printf(pl[now].handcard[curse].Intro());
		input=getch();
		if(input>='1' && input<='9' ){
			if(!pl[now].used[input-'0'] && input-'0'<=pl[now].cardcnt) curse=input-'0';
		}
		if(input==LEFT || input==UP || input=='w' || input=='a'){
			option_use=option_giveup=option_over=0;
			curse--;
			if(curse<=0) curse=pl[now].cardcnt;
			while(pl[now].used[curse]&&curse>0){
				curse--;
				if(curse<=0) curse=pl[now].cardcnt;
			}
		}
		if(input==RIGHT || input==DOWN || input=='s' || input=='d'){
			option_use=option_giveup=option_over=0;
			curse++;
			if(curse>pl[now].cardcnt) curse=1;
			while(pl[now].used[curse]&&curse<=pl[now].cardcnt){
				curse++;
				if(curse>pl[now].cardcnt) curse=1;
			}
		}
		if(curse<=0)curse=pl[now].cardcnt;
		if(curse>pl[now].cardcnt)curse=1;//移动焦点
		if(input=='z' || input=='+')//使用
		{
			option_giveup=option_over=0;
			if(!option_use){
				option_use=1;
			}
			else {
				if(pl[now].used[curse]){
					SetPos(0,1);
					printf("选中的牌不存在！                 ");
				}
				else if(pl[now].cost<pl[now].handcard[curse].cost){
					SetPos(0,1);
					printf("费用不足无法使用此牌！                 ");
				}
				else{
					pl[now].cost-=pl[now].handcard[curse].cost;
					pl[now].used[curse]=1;
					pl[now].rest--;
					pl[now].handcard[curse].Use(now,3-now);
					if(pl[now].occ==5 && rand()%100<80) {
						pl[now].used[curse]=0;
						pl[now].rest++;
						pl[now].handcard[curse]=pl[now].heap[(rand()%pl[now].heapn)+1];
					}
					while(pl[now].used[curse] && curse<=pl[now].cardcnt+1 && pl[now].rest){
						curse++;
						if(curse>pl[now].cardcnt)curse=1;
					}
				}
				option_use=0;
			}
		}
		if(input=='x' || input=='-'){//弃牌
			option_use=option_over=0;
			if(!option_giveup){
				option_giveup=1;
			}
			else{
				pl[now].used[curse]=1;
				pl[now].rest--;
				while(pl[now].used[curse] && curse<=pl[now].cardcnt+1 && pl[now].rest){
					curse++;
					if(curse>pl[now].cardcnt)curse=1;
				}
				option_giveup=0;
			}
		}
		if(input==SPACE || input==ENTER) {//结束回合
			if(!option_over){
				option_over=1;
				SetPos(11,Row+12);
				printf("确定要结束回合吗？");
			}else{
				option_over=0;
				SetPos(11,Row+12);
				printf("                    ");
				break;
			}
		}
		if(!option_over){
			SetPos(11,Row+12);
			printf("                    ");
		}
		if(pl[now].rest<=0)//结束回合
			break; 
		Check(now);
	}
	return 0;
}

int main(){
	SetConsoleOutputCP(65001);
	srand(time(NULL));
	previous();
	printf("WELCOME!!\n");
	for(int i=1;i<=2;i++){
		printf("请输入P%d的名字:",i);
		cin>>pl[i].name;
	}
	Sleep(300);
	system("cls");
	for(int now=1;now<=2;now++){
		Choose(now);//选择职业
		SetPos(0,19+now);
		printf("P%d的职业是",now);
		printf(occ_name(pl[now].occ));
	}
	for(int i=1;i<=2;i++) pl[i].cost=3;
	Sleep(1500);
	system("cls");
	for(int x=1;x<=2;x++) {
		init(x);
		for(int i=1;i<=pl[x].cardcnt;i++) pl[x].handcard[i]=pl[x].heap[(rand()%pl[x].heapn)+1];
	}
	int now=1;
	adv(3-now);
	while(!Check(now)){
		for(int i=1;i<=2;i++) pl[i].prehp=pl[i].hp;
		if(pl[now].cost==0 && pl[now].occ==1)  pl[now].cost=1;
		pl[now].cost=min(pl[now].cost+1,pl[now].maxcost);//加费 
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
	printf("获得了胜利!!");
	Shake(10,1);
	while(1){
		SetColor(rand()%16);
		SetPos(rand()%100,rand()%30);
		printf("#%d ",winner);
		printf(pl[winner].name);
		printf("获得了胜利!!");
		Sleep(10);
	}
	return 0;
}
