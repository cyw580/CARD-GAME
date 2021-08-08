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
int Row=11;

int env_now,env_cnt,env_rate;
int env_on;
int cost_bgn=3;
int player_bgn=1;

struct player{
	string name;
	int cost,maxcost,rest,heapn,occ;
	int hp,def,cardcnt,prehp;
	int maxhp,maxdef;
	Card handcard[15],heap[1005];
	int used[15];
	int buff[20];
	//0-职业特性,1-燃烧,2-中毒,3-狂暴,4-虚弱,5-治疗,10-牧师的[神圣意志]记录
	int hprate(){
		return 10*hp/maxhp;
	}
	void UpdateBuff(){
		for(int i=1;i<=9;i++){
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
	if(pl[now].occ==7 && pl[now].buff[10]>=4) {
		winner=now;
		return 1;
	}
	return 0;
}

int Card::cal_atk(int from,int to){
	int damage=ATK;
	if(pl[from].occ==2 && ATK>0) damage=ATK+6*pl[from].buff[0];//术士
	if(pl[from].occ==5 && ATK>0) damage=ATK+7*pl[from].buff[0];//地精
	if(pl[from].occ==4 && pl[from].buff[0])//战士
		damage*=0.7;
	//职业影响
	if(pl[from].buff[3])//狂暴
		damage*=2;
	if(pl[from].buff[4])//虚弱
		damage*=0.5;
	//Buff影响
	if(env_now==1)//血月
		damage*=1.2;
	if(env_now==2)//暴雨
		damage*=0.8;
	//天气影响
	return damage;
}

bool Card::check_atk(int from,int to){
	if(pl[from].occ==2 && ATK>0 && pl[from].buff[0]>0) return 1;
	if(pl[from].occ==5 && ATK>0 && pl[from].buff[0]>0) return 1;
	if(pl[from].occ==4 && (ATK>0 || func==15) && pl[from].buff[0]>0) return 1;
	//职业影响
	if(pl[from].buff[3] || pl[from].buff[4])
		return 1;
	//Buff影响
	if(env_now==1 || env_now==2)
		return 1;
	//天气影响
	return 0;
}

int Card::cal_miss(int from,int to){
	int miss=MISS;
	if(env_now==3)
		miss+=10;
	if(env_now==4)
		miss-=10;
	//天气对失误率的影响
	if(miss>100)miss=100;
	if(miss<0)miss=0;
	return miss;
}
bool Card::check_miss(int from,int to){
	if(env_now==3)
		return 1;
	if(env_now==4)
		return 1;
	//天气对失误率的影响
	return 0;
}

int Card::Use(int from,int to){
	int miss=cal_miss(from,to);
	if(rand()%100<miss){
		SetPos(0,1);
		printf("操作失误了!");
		if(pl[from].occ==4 && ATK>0) pl[from].buff[0]=1;
		UI();
		return 0;
	}//失效判定
	int damage,damage2,flag;
	if(func) flag=Special(from,to);
	damage=damage2=cal_atk(from,to);
	//护盾抵消
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
	if(damage>=150){//重击动画
		system("color 47");
		Shake(10,1);
		system("color 07");
		system("cls");
		UI();
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
	if(pl[from].occ==5 && flag==3){
		pl[from].buff[0]+=2;
		pl[from].maxhp+=30;
		pl[from].maxdef+=20;
		pl[from].hp=min(pl[from].maxhp,pl[from].hp+40);
	}
	UI();
	Check(to);Check(from);
	return 1;
}

int Card::Special(int from,int to){
	int damage=cal_atk(from,to);
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
		pl[from].buff[4]=0;
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
		pl[to].buff[4]=0;
	}
	else if(func==16){
		pl[to].cost=max(0,pl[to].cost-3);
	}
	else if(func==17){
		pl[from].buff[3]=1;
		pl[to].buff[3]=2;
		pl[from].buff[4]=pl[to].buff[4]=0;
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
		pl[from].cost=min(pl[from].cost+2,pl[from].maxcost);
	}
	else if(func==27){
		pl[from].heap[++pl[from].heapn]=lib[6][1];
		pl[from].heap[++pl[from].heapn]=lib[6][1];
	}
	else if(func==28){
		pl[from].heap[++pl[from].heapn]=lib[6][1];
		pl[from].heap[++pl[from].heapn]=lib[6][1];
		pl[from].heap[++pl[from].heapn]=lib[6][1];
		pl[from].cost=min(pl[from].cost+4,pl[from].maxcost);
	}
	else if(func==30){
		pl[from].heap[++pl[from].heapn]=lib[6][1];
	}
	else if(func==31){
		pl[from].heap[++pl[from].heapn]=lib[6][1];
		pl[to].hp=min(pl[to].hp+20,pl[to].maxhp);
		for(int i=1;i<=10;i++) pl[from].buff[i]=0;
	}
	else if(func==32){
		for(int i=1;i<=pl[from].cardcnt;i++){
			if(pl[from].used[i]) continue;
			if(pl[from].handcard[i].name=="[虚空垃圾]"){
				 pl[from].used[i]=1;
				 pl[from].rest--;
				 pl[to].hp-=50;
			}
		}
	}
	else if(func==33){
		pl[from].heap[++pl[from].heapn]=lib[6][2];
	}
	else if(func==34){
		return 3;
	}
	else if(func==35){
		pl[to].buff[5]+=2;
	}
	else if(func==36){
		pl[from].buff[1]+=2;
	}
	else if(func==37){
		pl[from].cost=min(pl[from].cost+pl[from].buff[0],pl[from].maxcost);
		pl[from].buff[0]=0;
	}
	else if(func==38){
		pl[from].heap[++pl[from].heapn]=pl[to].heap[rand()%pl[to].heapn+1];
		pl[from].heap[pl[from].heapn].func=0;
		if(pl[from].heap[pl[from].heapn].name=="[神圣意志]") pl[from].heap[pl[from].heapn].func=42;
	}
	else if(func==39){
		pl[from].buff[5]+=2;
		pl[to].buff[5]+=2;
	}
	else if(func==42){
		pl[from].buff[10]--;
	}
	else if(func==43){
		pl[from].heap[++pl[from].heapn]=lib[7][libcnt[7]+1];
	}
	else if(func==44){
		int x=rand()%pl[from].cardcnt+1;//选取随机槽位
		if(pl[from].used[x]){
			pl[from].rest++;
			pl[from].used[x]=0;
			pl[from].buff[10]++;
		}
		else if(pl[from].handcard[x].func!=42) pl[from].buff[10]++;
		pl[from].handcard[x]=lib[7][libcnt[7]+1];
	}
	else if(func==45){
		pl[from].heap[++pl[from].heapn]=pl[to].heap[rand()%pl[to].heapn+1];
		pl[from].heap[pl[from].heapn].func=1;
	}
	else if(func==46){
		pl[from].cost=pl[from].maxcost;
		int x=rand()%pl[to].cardcnt+1;//选取随机槽位
		if(!pl[to].used[x] && pl[to].handcard[x].func==42) pl[to].buff[10]--;
		pl[to].handcard[x]=lib[7][10];
		if(pl[to].used[x]) pl[to].used[x]=0;
	}
	else if(func==47){
		for(int i=1;i<=pl[to].cardcnt;i++){
			if(pl[to].used[i]){
				pl[to].handcard[i]=pl[to].heap[(rand()%pl[to].heapn)+1];
				pl[to].used[i]=0;
				if(pl[to].handcard[i].func==42) pl[to].buff[0]++;
			}
		}
		int x=rand()%pl[to].cardcnt+1;//选取随机槽位
		pl[to].handcard[x].cost+=1;
	}
	else if(func==48){
		for(int i=1;i<=pl[to].cardcnt;i++){
			if(pl[to].used[i]) continue;
			if(pl[to].handcard[i].name=="[神圣意志]") pl[to].used[i]=1,pl[to].buff[0]++,pl[to].buff[10]--;
		}
	}
	else if(func==49){
		pl[to].buff[4]=2;
		pl[to].buff[3]=0;
	}
	return 0;
}

void init(int x){
	int tot=libcnt[0];
	for(int i=1;i<=tot;i++){
		pl[x].heap[i]=lib[0][i];
		if(pl[x].occ==3 && pl[x].heap[i].HEAL>=75) i--,tot--;
		if(pl[x].occ==4 && pl[x].heap[i].HEAL>0) i--,tot--;
		if(pl[x].occ==5 || pl[x].occ==6) i--,tot--;
		if(pl[x].occ==7 && pl[x].heap[i].ATK>=80) i--,tot--;
	}
	pl[x].heapn=tot;
	tot=libcnt[pl[x].occ];
	for(int i=1;i<=tot;i++) pl[x].heap[i+pl[x].heapn]=lib[pl[x].occ][i];
	pl[x].heapn+=tot;
	if(pl[3-x].occ==7 && pl[x].occ!=7) pl[x].heap[++pl[x].heapn]=lib[7][libcnt[7]+2];//获得[亵渎]
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
	else if(pl[x].occ==2 || pl[x].occ==6){
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
	else if(pl[x].occ==7){
		pl[x].buff[0]+=1;
	}
	return;
}

string occ_name(int x){
	if(x==1)return "浪人";
	else if(x==2)return "术士";
	else if(x==3)return "法师";
	else if(x==4)return "战士";
	else if(x==5)return "地精";
	else if(x==6)return "恶魔";
	else if(x==7)return "牧师";
	else if(x==8)return "随缘";
	return "";
}

void Choose(int now){
	SetPos(0,1);
	printf("          ◇选择P%d ",now);
	printf(pl[now].name);
	printf("的职业◇");
	int cursor=1,sumjob=7;
	while(1){
		for(int i=1;i<=sumjob+1;i++){	
			if(cursor!=i)
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
		occ_func(cursor);
		input=getch();
		if(input==UP || input==LEFT || input=='w' || input=='a')
			cursor--;
		if(input==DOWN || input==RIGHT || input=='s' || input=='d')
			cursor++;
		if(input>='1' && input<='8'){
			if(input-'0'==cursor){
				if(cursor<=sumjob){
					pl[now].occ=cursor;
					return;
				}
				else {
					pl[now].occ=rand()%sumjob+1;
					return;
				}
			}
			else
				cursor=input-'0';
		}
		if(cursor>sumjob+1) cursor=1;
		if(cursor<1) cursor=sumjob+1;
		if(input==SPACE || input==ENTER || input=='z'){
			if(cursor<=sumjob){
				pl[now].occ=cursor;
				return;
			}
			else {
				pl[now].occ=rand()%sumjob+1;
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
			SetColor(11),printf("虚弱 %d : ",pl[rnk].buff[4]);
		if(pl[rnk].buff[5])
			SetColor(10),printf("治疗 %d : ",pl[rnk].buff[5]);
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

	//显示天气
	SetPos(15,0);
	SetColor(env_color(env_now));
	printf("<");
	printf(env_name(env_now));
	printf(">  | ");
	printf(env_brief(env_now));
	// debug
	// SetPos(80,0);
	// printf("rate:%d%%",env_rate);

	return 0; 
}

int Ask(int now){
	int option_use=0,option_giveup=0,option_over=0;
	if(pl[now].occ==3){
		if(pl[now].buff[0]){
			pl[now].cost=min(pl[now].maxcost,pl[now].cost+pl[now].buff[0]);
			pl[now].buff[0]=0;
		}
		if(rand()%100<30) pl[now].buff[0]++;
	}
	if(pl[now].occ==4) {
		if(pl[now].def==0) pl[now].def=40;
		pl[now].buff[0]=0;
	}
	if(pl[now].occ==5) pl[now].cost=3;
	if(pl[now].occ==6){
		pl[now].cost=min(pl[now].cost+1,pl[now].maxcost);
		int da=5*pl[now].buff[0];
		if(da<pl[now].def) pl[now].def-=da;
		else {
			if(pl[now].def>0) Shake(3,1);
			pl[now].hp-=da-pl[now].def,pl[now].def=0;
		}
	}
	if(pl[now].buff[5])
		pl[now].hp=min(pl[now].hp+30,pl[now].maxhp);
	if(pl[now].buff[1])
		pl[now].hp-=40;
	if(pl[now].buff[2])
		pl[now].hp-=pl[now].hp*0.2;
	pl[now].UpdateBuff();
	//buff前判

	if(env_now==5){
		pl[now].hp=min(pl[now].maxhp,(int)(pl[now].hp+pl[now].maxhp*0.05));
	}//彩虹
	if(env_now==6){
		pl[now].hp=(pl[now].hp-pl[now].maxhp*0.05);
	}
	//酸雨
	//环境判断
	
	UI();
	if(Check(now))
		return 0;
	//死亡判断

	for(int i=1;i<=pl[now].cardcnt;i++){
		if(pl[now].used[i]){
			if(pl[now].occ==7 && rand()%100<pl[now].buff[0]*3){
				pl[now].handcard[i]=lib[7][libcnt[7]+1];
				pl[now].used[i]=0;
				if(++pl[now].buff[10]>=4) {
					winner=now;
					return 1;
				}
			}
			else{
				pl[now].handcard[i]=pl[now].heap[(rand()%pl[now].heapn)+1];
				pl[now].used[i]=0;
				if(pl[now].handcard[i].func==42) 
					if(++pl[now].buff[10]>=4) {
						winner=now;
						return 1;
					}
			}
		}
	}
	//补充手牌
	pl[now].rest=pl[now].cardcnt;
	int cursor=1;

	SetColor(7);
	while(!winner){
		SetColor(7);
		SetPos(0,Row);
		printf("P%d",now); 
		SetPos(5,Row);
		printf("  #"); 
		SetPos(11,Row);SetColor(11);
		printf("◆");
		SetPos(20+14,Row);SetColor(7);
		printf("ATK");
		SetPos(30+14,Row);
		printf("HEAL");
		SetPos(40+14,Row);
		printf("DEF");
		SetPos(50+14,Row);
		printf("MISS");
		
		for(int i=1;i<=pl[now].cardcnt;i++){
			int color=0;
			// SetPos(1,11+i);
			// printf("%d",pl[now].used[i]);//debug
			SetPos(5,Row+i);
			if(pl[now].used[i])SetColor(8);
			if(cursor==i && !option_giveup && !option_use) SetColor(color=15),printf("◎%d",i); 
			else if(cursor==i && option_giveup) SetColor(color=15),printf("×%d",i); 
			else if(cursor==i && option_use) SetColor(color=15),printf("●%d",i); 
			else SetColor(color=7),printf("  %d",i); 
			if(pl[now].used[i]){
				SetColor(8);
				printf("                                                                   ");
				SetPos(14,Row+i);
				printf("[Used.]");
				continue;
			}

			SetPos(11,Row+i);
			if(pl[now].handcard[i].func==15) {
				pl[now].handcard[i].ATK=pl[now].cost*35+25;
				pl[now].handcard[i].cost=pl[now].cost;
			}//[背水一战]
			if(pl[now].cost>=pl[now].handcard[i].cost)SetColor(11);
			else SetColor(3);//费用是否足够
			printf("%d",pl[now].handcard[i].cost);
			SetColor(color);
			SetPos(14,Row+i);
			printf(pl[now].handcard[i].name);//牌名
			printf("               ");
			SetPos(20+14,Row+i);
			if(pl[now].handcard[i].func!=32)
				printf("%-3d",pl[now].handcard[i].ATK);
			else {
				int da=0;
				for(int i=1;i<=pl[now].cardcnt;i++){
					if(pl[now].used[i]) continue;
					if(pl[now].handcard[i].name=="[虚空垃圾]") da+=50;
				}
				printf("%-3d",da);
			}
			//ATK
			if(pl[now].handcard[i].ATK>0 && pl[now].handcard[i].check_atk(now,3-now)){
				int atk=pl[now].handcard[i].cal_atk(now,3-now);
				if(pl[now].handcard[i].ATK < atk)SetColor(12);
				else if(pl[now].handcard[i].ATK == atk)SetColor(7);
				else SetColor(8);
				printf("(%d)  ",atk);
				SetColor(7);
			}
			else{
				printf("     ");
			}
			SetPos(30+14,Row+i);
			printf("%-3d",pl[now].handcard[i].HEAL);//HEAL
			SetPos(40+14,Row+i);
			printf("%-3d",pl[now].handcard[i].DEF);//DEF
			SetPos(50+14,Row+i);
			printf("%-3d%%",pl[now].handcard[i].MISS);//MISS
			if(pl[now].handcard[i].check_miss(now,3-now)){
				SetColor(8);
				printf("(%d%%)  ",pl[now].handcard[i].cal_miss(now,3-now));
				SetColor(7);
			}
			else{
				printf("         ");
			}

		}//显示手牌
		SetColor(7);
		SetPos(11,Row+9);
		printf("                                                                         ");
		SetPos(11,Row+9);
		printf(pl[now].handcard[cursor].Intro());
		input=getch();
		if(input>='1' && input<='9' ){
			if(!pl[now].used[input-'0'] && input-'0'<=pl[now].cardcnt) cursor=input-'0';
		}
		if(input==LEFT || input==UP || input=='w' || input=='a'){
			option_use=option_giveup=option_over=0;
			cursor--;
			if(cursor<=0) cursor=pl[now].cardcnt;
			while(pl[now].used[cursor]&&cursor>0){
				cursor--;
				if(cursor<=0) cursor=pl[now].cardcnt;
			}
		}
		if(input==RIGHT || input==DOWN || input=='s' || input=='d'){
			option_use=option_giveup=option_over=0;
			cursor++;
			if(cursor>pl[now].cardcnt) cursor=1;
			while(pl[now].used[cursor]&&cursor<=pl[now].cardcnt){
				cursor++;
				if(cursor>pl[now].cardcnt) cursor=1;
			}
		}
		if(cursor<=0)cursor=pl[now].cardcnt;
		if(cursor>pl[now].cardcnt)cursor=1;//移动焦点
		if(input=='z' || input=='+')//使用
		{
			SetPos(0,1);
			printf("                ");
			option_giveup=option_over=0;
			if(!option_use){
				option_use=1;
			}
			else {
				if(pl[now].used[cursor]){
					SetPos(0,1);
					printf("选中的牌不存在！                 ");
				}
				else if(pl[now].cost<pl[now].handcard[cursor].cost){
					SetPos(0,1);
					printf("费用不足无法使用此牌！                 ");
				}
				else{
					pl[now].cost-=pl[now].handcard[cursor].cost;
					pl[now].used[cursor]=1;
					pl[now].rest--;
					if(pl[now].handcard[cursor].name=="[神圣意志]") pl[now].buff[10]--;
					if(pl[now].handcard[cursor].Use(now,3-now) && pl[now].handcard[cursor].func==40) {
						pl[now].used[cursor]=0;
						pl[now].rest++;
						pl[now].handcard[cursor]=pl[3-now].heap[(rand()%pl[3-now].heapn)+1];
						pl[now].handcard[cursor].MISS=pl[now].handcard[cursor].func=0;
						if(pl[now].handcard[cursor].name=="[神圣意志]") 
							if(++pl[now].buff[10]>=4) {
								winner=now;return 1;
							}
					}//牌的使用 与 [不完全记忆]
					if(pl[now].occ==5 && rand()%100<80) {
						pl[now].used[cursor]=0;
						pl[now].rest++;
						pl[now].handcard[cursor]=pl[now].heap[(rand()%pl[now].heapn)+1];
					}
					if(pl[now].occ==7){
						for(int i=1;i<=pl[now].cardcnt;i++){
							if(pl[now].used[i]) continue;
							if(pl[now].handcard[i].name=="[意识反馈]") pl[now].handcard[i].ATK+=9;
						}
					}//[意识反馈]
					while(pl[now].used[cursor] && cursor<=pl[now].cardcnt+1 && pl[now].rest){
						cursor++;
						if(cursor>pl[now].cardcnt)cursor=1;
					}
				}
				option_use=0;
			}
		}
		if(input=='x' || input=='-'){//弃牌
			SetPos(0,1);
			printf("                ");
			option_use=option_over=0;
			if(!option_giveup){
				option_giveup=1;
			}
			else{
				if(pl[now].occ!=6){
					pl[now].used[cursor]=1;
					pl[now].rest--;
					if(pl[now].handcard[cursor].func==42) --pl[now].buff[10];
					while(pl[now].used[cursor] && cursor<=pl[now].cardcnt+1 && pl[now].rest){
						cursor++;
						if(cursor>pl[now].cardcnt)cursor=1;
					}
					option_giveup=0;
				}
				else{
					if(pl[now].used[cursor]){
						SetPos(0,1);
						printf("选中的牌不存在！                 ");
					}
					else if(pl[now].handcard[cursor].func==29 || pl[now].handcard[cursor].func==28){
						pl[now].used[cursor]=1;
						pl[now].rest--;
						while(pl[now].used[cursor] && cursor<=pl[now].cardcnt+1 && pl[now].rest){
							cursor++;
							if(cursor>pl[now].cardcnt) cursor=1;
						}
						option_giveup=0;
						pl[now].buff[0]++;
						UI();
					}
					else if(pl[now].cost<pl[now].handcard[cursor].cost){
						SetPos(0,1);
						printf("费用不足无法弃置此牌！                 ");
					}
					else{
						pl[now].cost-=pl[now].handcard[cursor].cost;
						pl[now].used[cursor]=1;
						pl[now].rest--;
						swap(pl[now].handcard[cursor].ATK,pl[now].handcard[cursor].HEAL);
						pl[now].handcard[cursor].Use(now,3-now);
						while(pl[now].used[cursor] && cursor<=pl[now].cardcnt+1 && pl[now].rest){
							cursor++;
							if(cursor>pl[now].cardcnt)cursor=1;
						}
						option_giveup=0;
						pl[now].buff[0]++;
						UI();
					}
				}
			}
		}
		if(input==SPACE || input==ENTER) {//结束回合
			SetPos(0,1);
			printf("                ");
			if(!option_over){
				option_over=1;
				SetPos(11,Row+12);
				printf("确定要结束回合吗？");
			}else{
				if(pl[now].occ==7 && pl[now].cost==0) {
					if(++pl[now].buff[0]>=12) {
						winner=now;
						return 1;
					}
				}
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
		if(pl[now].rest<=0){//结束回合
			SetPos(0,1);
			printf("                ");
			if(pl[now].occ==7 && pl[now].cost==0) {
					if(++pl[now].buff[0]>=12) {
						winner=now;
						return 1;
					}
			}
			Sleep(200);
			break;
		} 
		Check(now);
	}
	return 0;
}

void Options(){
	SetPos(0,0);
	printf("※ 设置 Options ※");
	int cursor=1;
	while(1){

		SetPos(1,3);
		if(cursor!=1)SetColor(7,0);
		else SetColor(0,7);
		printf("天气环境变化                          ");
		SetPos(25,3);
		if(env_on==1)printf("■开");
		else printf("关■");//天气设定

		SetPos(1,5);
		if(cursor!=2)SetColor(7,0);
		else SetColor(0,7);
		printf("先手玩家                             ");
		SetPos(25,5);
		if(player_bgn>0)
			printf("P%d",player_bgn);
		else
			printf("随机");//先手玩家设定

		SetPos(1,7);
		
		input=getch();
		if(input==UP || input==LEFT) cursor--;
		if(input==DOWN || input==RIGHT) cursor++;
		if(cursor>2) cursor=1;
		if(cursor<1) cursor=2;
		if(input=='z' || input=='x'){
			if(cursor==1){
				env_on^=1;
			}
			if(cursor==2){
				player_bgn=(player_bgn+1)%3;
			}
		}
		if(input==SPACE || input==ENTER){
			break;
		}
	}
	return;
}

int main(){
	SetConsoleOutputCP(65001);
	SetConsoleTitle("CARD GAME:v1.2.0");
	srand(time(NULL));
	previous();
	printf("WELCOME!!\n");
	for(int i=1;i<=2;i++){
		printf("请输入P%d的名字:",i);
		cin>>pl[i].name;
	}
	Sleep(300);
	system("cls");

	SetConsoleTitle("CARD GAME:Options");
	Options();
	SetColor(0,7);
	system("color 07");
	system("cls");

	SetConsoleTitle("CARD GAME:Choose Your Identity.");
	for(int now=1;now<=2;now++){
		Choose(now);//选择职业
		SetPos(0,19+now);
		printf("P%d的职业是",now);
		printf(occ_name(pl[now].occ));
	}
	for(int i=1;i<=2;i++) if(pl[i].occ!=6) pl[i].cost=3;
	SetConsoleTitle("Here we go...");
	Sleep(1500);
	system("cls");

	for(int x=1;x<=2;x++) {
		init(x);
		for(int i=1;i<=pl[x].cardcnt;i++) {
			pl[x].handcard[i]=pl[x].heap[(rand()%pl[x].heapn)+1];
			if(pl[x].handcard[i].func==42) {
				if(++pl[x].buff[10]>=4) winner=x;
			}
		}
	}//初始发牌

	if(!player_bgn){
		player_bgn=rand()%2+1;
	}//随机先手
	int now=player_bgn;

	adv(3-player_bgn);//后手补偿

	env_now=0;

	char title[]="CARD GAME:Turn of P1";

	while(!Check(now) && !winner){
		title[19]=now+'0';
		SetConsoleTitle(title);
		for(int i=1;i<=2;i++) pl[i].prehp=pl[i].hp;
		if(pl[now].cost==0 && pl[now].occ==1)  pl[now].cost=1;
		pl[now].cost=min(pl[now].cost+1,pl[now].maxcost);//加费 
		if(env_now==7)
			pl[now].cost=min(pl[now].cost+1,pl[now].maxcost);//环境加费 

		UI();
		if(Ask(now)==1) break;

		system("cls");
		now++;
		if(now>2)now=1;

		//环境变动
		if(env_on){
			if(env_cnt>=2){
				env_rate+=25;
				if(rand()%100 < env_rate){
					int _p=0;
					while((_p=rand()%env_num)==env_now);
					env_now=_p;
					env_cnt=-1;
					env_rate=0;
				}
			}
			else{
				env_rate=0;
			}
			env_cnt++;
		}
	}
	UI();
	now=1;
	SetPos(0,Row);
	printf("GAME OVER!!! Result:");
	for(int i=1;i<=pl[1].cardcnt;i++){
			int color=0;
			// SetPos(1,11+i);
			// printf("%d",pl[now].used[i]);//debug
			SetPos(5,Row+i);
			if(pl[1].used[i])SetColor(8);
			SetColor(color=7),printf("  %d ",i); 
			if(pl[1].used[i]){
				SetColor(8);
				printf("                                                                   ");
				SetPos(14,Row+i);
				printf("[Used.]");
				continue;
			}
			printf(" %d ",pl[now].handcard[i].cost);
			SetColor(color);
			SetPos(14,Row+i);
			printf(pl[now].handcard[i].name);//牌名
			printf("               ");
			SetPos(20+14,Row+i);
			printf("%-3d",pl[now].handcard[i].ATK);
			//ATK
			if(pl[now].handcard[i].ATK>0 && pl[now].handcard[i].check_atk(now,3-now)){
				int atk=pl[now].handcard[i].cal_atk(now,3-now);
				if(pl[now].handcard[i].ATK < atk)SetColor(12);
				else if(pl[now].handcard[i].ATK == atk)SetColor(7);
				else SetColor(8);
				printf("(%d)",atk);
				SetColor(7);
			}
			else{
				printf("     ");
			}
			SetPos(30+14,Row+i);
			printf("%-3d",pl[now].handcard[i].HEAL);//HEAL
			SetPos(40+14,Row+i);
			printf("%-3d",pl[now].handcard[i].DEF);//DEF
			SetPos(50+14,Row+i);
			printf("%-3d%%",pl[now].handcard[i].MISS);//MISS
			if(pl[now].handcard[i].check_miss(now,3-now)){
				SetColor(8);
				printf("(%d%%)",pl[now].handcard[i].cal_miss(now,3-now));
				SetColor(7);
			}
			else{
				printf("         ");
			}
	}
	Row+=5;
	now=2;
	for(int i=1;i<=pl[2].cardcnt;i++){
			int color=0;
			// SetPos(1,11+i);
			// printf("%d",pl[now].used[i]);//debug
			SetPos(5,Row+i);
			if(pl[2].used[i])SetColor(8);
			SetColor(color=7),printf("  %d ",i); 
			if(pl[2].used[i]){
				SetColor(8);
				printf("                                                                   ");
				SetPos(14,Row+i);
				printf("[Used.]");
				continue;
			}
			printf(" %d ",pl[now].handcard[i].cost);
			SetColor(color);
			printf("                 ");
			SetPos(14,Row+i);
			printf(pl[now].handcard[i].name);//牌名
			printf("                 ");
			SetPos(20+14,Row+i);
			printf("%-3d",pl[now].handcard[i].ATK);
			printf("                 ");
			//ATK
			if(pl[now].handcard[i].ATK>0 && pl[now].handcard[i].check_atk(now,3-now)){
				int atk=pl[now].handcard[i].cal_atk(now,3-now);
				if(pl[now].handcard[i].ATK < atk)SetColor(12);
				else if(pl[now].handcard[i].ATK == atk)SetColor(7);
				else SetColor(8);
				printf("(%d)",atk);
				SetColor(7);
			}
			else{
				printf("     ");
			}
			SetPos(30+14,Row+i);
			printf("%-3d",pl[now].handcard[i].HEAL);//HEAL
			printf("                 ");
			SetPos(40+14,Row+i);
			printf("%-3d",pl[now].handcard[i].DEF);//DEF
			printf("                 ");
			SetPos(50+14,Row+i);
			printf("%-3d%%",pl[now].handcard[i].MISS);//MISS
			if(pl[now].handcard[i].check_miss(now,3-now)){
				SetColor(8);
				printf("(%d%%)",pl[now].handcard[i].cal_miss(now,3-now));
				SetColor(7);
			}
			else{
				printf("         ");
			}

	}
			//debug
	Sleep(4000);//4s的情况展示
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
