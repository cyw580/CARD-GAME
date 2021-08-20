#include<bits/stdc++.h>
#include<windows.h>
#include<conio.h> 
#include<iostream>
#include<cstdio>
using namespace std;

#include"PreInfo.h"
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define SPACE 32
#define ENTER 13
#define MAXAPCARDNUM 55
//ASCII 

char input;
int server_mode;
int winner;
int Row=11;
int now;
int env_now,env_cnt,env_rate;
int env_on;
int cost_bgn=3;
int player_bgn=1;
int mode;
Card appcard[MAXAPCARDNUM];
int appcnt;
struct player{
	string name;
	int cost,maxcost,rest,heapn,occ;
	int hp,def,cardcnt,prehp;
	int maxhp,maxdef;
	Card handcard[11],heap[65];
	int used[11];
	int buff[11];
	//0-职业特性,1-燃烧,2-中毒,3-狂暴,4-虚弱,5-治疗,6-迷惑,10-神圣意志
	int hprate(){
		return 10*hp/maxhp;
	}
	void UpdateBuff(int x){
		if(x==1) {//回合开始更新
			buff[1]=max(0,buff[1]-1);//燃烧
			buff[2]=max(0,buff[2]-1);//中毒
			buff[5]=max(0,buff[5]-1);//治疗
		}
		if(x==2) {//回合结束更新
			buff[3]=max(0,buff[3]-1);//狂暴
			buff[4]=max(0,buff[4]-1);//虚弱
			buff[6]=max(0,buff[6]-1);//迷惑
		}
	}
}pl[15];
Card void_card=(Card){0,0,0,0,0,-2,0};

#include"CARDGAMEsocket.hpp"

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

int Card::cal_atk(int from,int to){//计算实际攻击力
	int damage=ATK;
	if(func==15) return damage;//[背水一战]无视buff
	if(pl[from].occ==2 && ATK>0) damage=ATK+6*pl[from].buff[0];//术士
	if(pl[from].occ==5 && ATK>0) damage=ATK+ATK*pl[from].buff[0]*0.08;//地精
	if(pl[from].occ==4 && pl[from].buff[0])//战士
		damage-=0.04*damage*pl[from].buff[0];
	//职业影响
	if(pl[from].buff[3])//狂暴
		damage*=2;
	if(pl[from].buff[4])//虚弱
		damage*=0.7;
	//Buff影响
	if(env_now==1)//血月
		damage*=1.2;
	if(env_now==2)//暴雨
		damage*=0.8;
	//天气影响
	if(mode==2) //欢乐模式：随机buff
		damage*=0.7;
	return damage;
}

int Card::cal_heal(int from,int to){//计算实际恢复量
	int heal=HEAL;
	if(heal>0 && pl[from].buff[4])//虚弱
		heal*=0.7;
	if(mode==2) //欢乐模式：随机buff
		heal*=0.7;
	return heal;
}

int Card::cal_def(int from,int to){//计算实际叠盾
	int def=DEF;
	if(mode==2) //欢乐模式：随机buff
		def*=0.7;
	return def;
}

int Card::cal_miss(int from,int to){
	int miss=MISS;
	if(env_now==3)
		miss+=10;
	if(env_now==4)
		miss-=10;
	//天气对失误率的影响
	if(pl[from].buff[6])//迷惑
		miss+=25;
	if(miss>100) miss=100;
	if(miss<0) miss=0;
	return miss;
}

Card use_card;

int Card::Use(int from,int to){
	int miss=cal_miss(from,to);
	use_card=(Card){0,0,0,0,0,0,0};
	use_card.id=id;
	use_card.cost=cost;
	if(rand()%100<miss){
		SetPos(0,1);
		printf("操作失误了!");
		if(pl[from].occ==4 && ATK>0) pl[from].buff[0]+=2;
		UI();
		use_card.func=-1;
		return 0;
	}//失效判定
	int damage,damage2,flag=0;
	if(func) use_card.func=func,flag=Special(from,to);
	if(flag==4) return 0;
	damage=damage2=use_card.ATK=cal_atk(from,to);
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
	int heal=cal_heal(from,to);
	use_card.HEAL=heal;
	pl[from].hp=min(pl[from].maxhp,pl[from].hp+heal);
	int def=cal_def(from,to);
	use_card.DEF=def;
	pl[from].def=min(pl[from].maxdef,pl[from].def+def);
	//套盾与回血
	if(HEAL<0 && pl[from].occ==2) pl[from].buff[0]++;
	if(pl[from].occ==4 && ATK>0) pl[from].buff[0]+=2;
	if(pl[from].occ==5 && flag==2){
		pl[from].buff[0]++;
		pl[from].maxhp+=10;
		pl[from].maxdef+=10;
		pl[from].hp=min(pl[from].maxhp,pl[from].hp+10);
	}
	if(pl[from].occ==5 && flag==3){
		pl[from].buff[0]+=2;
		pl[from].maxhp+=20;
		pl[from].maxdef+=20;
		pl[from].hp=min(pl[from].maxhp,pl[from].hp+20);
	}
	UI();
	if(Check(to)){
		return 1;
	}
	Check(from);
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
		pl[from].buff[3]=max(pl[from].buff[3],1);
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
		return 4;
	}
	else if(func==8){
		if(pl[from].cardcnt<6){
			pl[from].cardcnt++;
			pl[from].used[pl[from].cardcnt]=1;
		}
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
		pl[to].buff[3]=max(pl[to].buff[3],1);
	}
	else if(func==16){
		pl[to].cost=max(0,pl[to].cost-3);
	}
	else if(func==17){
		pl[from].buff[3]=max(pl[from].buff[3],1);
		pl[to].buff[3]=max(pl[to].buff[3],1);
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
		pl[from].maxhp-=100;
		pl[from].hp=pl[from].maxhp;
		pl[from].buff[0]=0;
	}
	else if(func==25){
		pl[from].buff[0]=max(pl[from].buff[0]-1,0);
		pl[from].cost=min(pl[from].cost+2,pl[from].maxcost);
	}
	else if(func==27){
		pl[from].heap[++pl[from].heapn]=lib[6][libcnt[6]+1];
	}
	else if(func==28){
		pl[from].heap[++pl[from].heapn]=lib[6][libcnt[6]+1];
		pl[from].heap[++pl[from].heapn]=lib[6][libcnt[6]+1];
		pl[from].heap[++pl[from].heapn]=lib[6][libcnt[6]+1];
		pl[from].cost=min(pl[from].cost+3,pl[from].maxcost);
	}
	else if(func==30){
		pl[from].heap[++pl[from].heapn]=lib[6][libcnt[6]+1];
	}
	else if(func==31){
		pl[from].heap[++pl[from].heapn]=lib[6][libcnt[6]+1];
		pl[to].hp=min(pl[to].hp+20,pl[to].maxhp);
		for(int i=1;i<=10;i++) pl[from].buff[i]=0;
	}
	else if(func==32){
		for(int i=1;i<=pl[from].cardcnt;i++){
			if(pl[from].used[i]) continue;
			if(pl[from].handcard[i].id==98){ //98-->[虚空垃圾]
				 pl[from].used[i]=1;
				 pl[from].rest--;
			}
		}
	}
	else if(func==33){
		pl[from].heap[++pl[from].heapn]=lib[6][libcnt[6]+2];
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
		if(pl[from].hp<=100) pl[from].hp+=min(pl[from].buff[0],(100-pl[from].hp)/20+1)*20;
		pl[from].cost=min(pl[from].cost+pl[from].buff[0],pl[from].maxcost);
		pl[from].buff[0]=0;
	}
	else if(func==38){
		pl[to].buff[6]=max(pl[to].buff[6],1);
	}
	else if(func==39){
		pl[from].buff[5]+=2;
		pl[to].buff[5]+=2;
	}
	else if(func==43){
		pl[from].heap[++pl[from].heapn]=lib[7][libcnt[7]+1];
	}
	else if(func==44){
		int x=rand()%pl[from].cardcnt+1;//选取随机槽位
		if(pl[from].used[x]){
			pl[from].rest++;
			pl[from].used[x]=0;
		}
		pl[from].handcard[x]=lib[7][libcnt[7]+1];
	}
	else if(func==45){
		pl[from].heap[++pl[from].heapn]=pl[to].heap[rand()%pl[to].heapn+1];
		pl[from].heap[pl[from].heapn].func=18;
	}
	else if(func==46){
		pl[from].cost=min(pl[from].maxcost,pl[from].cost+4);
		int x=rand()%pl[to].cardcnt+1;//选取随机槽位
		pl[to].handcard[x]=lib[7][10];//[法力水晶]
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
			if(pl[to].handcard[i].id==114) pl[to].used[i]=1,pl[to].buff[0]+=2; //114-->[神圣意志]
		}
	}
	else if(func==49){
		pl[to].buff[4]=max(pl[to].buff[4],2);
	}
	else if(func==50){
		if(damage>=pl[to].def)
			pl[to].buff[1]+=2;
	}
	else if(func==51){
		pl[to].buff[rand()%6+1]+=rand()%2+1;
	}
	else if(func==52){
		pl[from].buff[rand()%6+1]+=rand()%2+1;
	}
	else if(func==53){
		pl[rand()%2+1].buff[rand()%6+1]+=rand()%2+1;
	}
	else if(func==54){
		pl[to].buff[rand()%6+1]+=rand()%2+1;
		pl[from].buff[rand()%6+1]+=rand()%2+1;
	}
	else if(func==55){
		for(int i=1;i<=pl[to].cardcnt;i++){
			if(pl[to].used[i]){
				pl[to].handcard[i]=pl[to].heap[(rand()%pl[to].heapn)+1];
				pl[to].used[i]=0;
				if(pl[to].handcard[i].func==42) pl[to].buff[0]++;
			}
		}
		int x=rand()%pl[to].cardcnt+1,y=rand()%pl[to].cardcnt+1;//选取随机槽位
		while(x==y) x=rand()%pl[to].cardcnt+1,y=rand()%pl[to].cardcnt+1;//去重
		pl[to].handcard[x]=pl[to].handcard[y]=lib[7][libcnt[7]+3];
	}
	else if(func==56){
		pl[to].buff[6]=max(pl[to].buff[6],2);
	}
	else if(func==57){
		pl[from].buff[0]=max(pl[from].buff[0]-2,0);
		for(int i=1;i<=pl[from].heapn;i++){
			pl[from].heap[i].ATK*=1.5;
			if(pl[from].heap[i].HEAL>0) pl[from].heap[i].HEAL*=1.2;
			pl[from].heap[i].DEF*=1.2;
			pl[from].heap[i].MISS=max(pl[from].heap[i].MISS-5,0);
		}
	}
	else if(func==58){
		pl[from].buff[0]=max(0,pl[from].buff[0]-3);
	}
	else if(func==59){
		pl[from].buff[0]=max(0,pl[from].buff[0]-3);
		pl[to].maxhp-=80;
		pl[to].hp=min(pl[to].hp,pl[to].maxhp);
		for(int i=1;i<=pl[from].heapn;i++){
			while(pl[from].handcard[i].id==98 && i<=pl[from].heapn){ //98-->[虚空垃圾]
				pl[from].heap[i]=pl[from].heap[pl[from].heapn--];
			}
		}
	}
	else if(func==60){
		pl[from].cost=min(pl[from].cost+2,pl[from].maxcost);
		pl[from].used[++pl[from].cardcnt]=1;
		for(int i=1;i<=pl[from].heapn;i++){
			if(pl[from].heap[i].id==17) { //17-->[冲锋号令]
				pl[from].heap[i].id=-17;
				pl[from].heap[i].cost=2;
				pl[from].heap[i].ATK=40;
			}
			if(pl[from].heap[i].id==21) { //21-->[有能狂怒]
				pl[from].heap[i].id=-21;
				pl[from].heap[i].ATK=20;
			}
			if(pl[from].heap[i].id==25) { //25-->[生命果]
				pl[from].heap[i].id=-25;
				pl[from].heap[i].HEAL=90;
			}
			if(pl[from].heap[i].id==19) { //19-->[互惠共赢]
				pl[from].heap[i].id=-19;
				pl[from].heap[i].cost=0;
			}
		}
	}
	else if(func==61){
		pl[from].buff[0]++;
		pl[from].maxhp+=60;
	}
	else if(func==62){
		pl[from].buff[0]+=3;
		pl[from].maxcost++;
		pl[from].heap[++pl[from].heapn]=fun[1][3][2];
		pl[from].heap[++pl[from].heapn]=fun[1][3][2];
	}
	else if(func==63){
		for(int i=1;i<=pl[from].heapn;i++){
			while(pl[from].heap[i].id==81 && i<=pl[from].heapn){ //81-->[累赘]
				pl[to].heap[++pl[to].heapn]=pl[from].heap[i];
				pl[from].heap[i]=pl[from].heap[pl[from].heapn--];
			}
		}
		return 3;
	}
	else if(func==64){
		pl[from].buff[0]+=3;
		pl[from].heap[++pl[from].heapn]=lib[7][libcnt[7]+1];
		pl[from].heap[++pl[from].heapn]=lib[7][libcnt[7]+1];
		pl[from].heap[++pl[from].heapn]=fun[1][7][2];
		pl[from].heap[++pl[from].heapn]=fun[1][7][2];
	}
	else if(func==66){
		if(rand()%100<25){
			pl[from].buff[0]+=1;
		}
		for(int i=1;i<=pl[from].heapn;i++){
			if(pl[from].heap[i].id==54) pl[from].heap[i].ATK+=15; //54-->[法力成长]
		}
	}
	else if(func==67){
		pl[to].buff[6]=max(pl[to].buff[6],2);
	}
	else if(func==68){
		pl[to].buff[6]=max(pl[to].buff[6],2);
	}
	else if(func==69){
		pl[to].buff[6]=max(pl[to].buff[6],2);
	}
	else if(func==70){
		pl[to].buff[6]=max(pl[to].buff[6],2);
	}
	else if(func==71){
		pl[to].buff[6]=max(pl[to].buff[6],2);
	}
	else if(func==72){
		pl[to].buff[6]=max(pl[to].buff[6],2);
	}
	else if(func==73){
		pl[to].buff[6]=max(pl[to].buff[6],2);
	}
	else if(func==74){
		pl[to].buff[6]=max(pl[to].buff[6],2);
	}

	return 0;
}

void init(int x){
	pl[x].maxhp=job[pl[x].occ].maxhp;
	pl[x].cardcnt=job[pl[x].occ].cardcnt;
	pl[x].maxdef=job[pl[x].occ].maxdef;
	pl[x].maxcost=job[pl[x].occ].maxcost;
	pl[x].hp=pl[x].maxhp;
	//获得职业基本信息
	int tot=libcnt[0];
	for(int i=1;i<=tot;i++){
		pl[x].heap[i]=lib[0][i];
		if(pl[x].occ==3 && pl[x].heap[i].HEAL>=75) i--,tot--; //法师不能抽公共牌库HEAL>=75的牌
		if(pl[x].occ==4 && pl[x].heap[i].HEAL>0) i--,tot--;//战士不能抽公共牌库HEAL牌
		if(pl[x].occ==5 || pl[x].occ==6) i--,tot--;//地精、恶魔不能抽公共牌库的牌
		if(pl[x].occ==7 && pl[x].heap[i].ATK>=80) i--,tot--;//牧师不能抽公共牌库ATK>=80的牌
	}
	pl[x].heapn=tot;
	tot=libcnt[pl[x].occ];
	for(int i=1;i<=tot;i++) {
		pl[x].heap[i+pl[x].heapn]=lib[pl[x].occ][i];
	}
	pl[x].heapn+=tot;
	if(mode==2){//欢乐模式：随机buff
		tot=funcnt[2][0];
		for(int i=1;i<=tot;i++) pl[x].heap[i+pl[x].heapn]=fun[2][0][i];
		pl[x].heapn+=tot;
	}
	if(pl[3-x].occ==7 && pl[x].occ!=7) {
		pl[x].heap[++pl[x].heapn]=lib[7][libcnt[7]+2];//获得[亵渎]
	}
	for(int i=1;i<=pl[x].cardcnt;i++){
		pl[x].handcard[i]=pl[x].heap[(rand()%pl[x].heapn)+1];
	}
	pl[x].maxhp=job[pl[x].occ].maxhp;
	pl[x].cardcnt=job[pl[x].occ].cardcnt;
	pl[x].maxdef=job[pl[x].occ].maxdef;
	pl[x].maxcost=job[pl[x].occ].maxcost;
	pl[x].hp=pl[x].maxhp;
}

bool gettre[5];
void treasure(int now){//竞技模式：宝藏牌
	if(gettre[now]) return;
	if(pl[now].occ==1 && (pl[now].cost==0 && pl[now].hp<=210)){
		for(int i=1;i<=pl[now].cardcnt;i++){
			if(pl[now].used[i]) {
				pl[now].used[i]=0;
				gettre[now]=1;
				pl[now].handcard[i]=fun[1][1][1];
				break;
			}
		}
		if(!gettre[now]){
			pl[now].handcard[rand()%pl[now].cardcnt+1]=fun[1][1][1];
			gettre[now]=1;
		}
	}
	if(pl[now].occ==2 && pl[now].maxhp<=500){
		for(int i=1;i<=pl[now].cardcnt;i++){
			if(pl[now].used[i]) {
				pl[now].used[i]=0;
				gettre[now]=1;
				pl[now].handcard[i]=fun[1][2][1];
				break;
			}
		}
		if(!gettre[now]){
			pl[now].handcard[rand()%pl[now].cardcnt+1]=fun[1][2][1];
			gettre[now]=1;
		}
	}
	if(pl[now].occ==3 && pl[now].hp<=160){
		for(int i=1;i<=pl[now].cardcnt;i++){
			if(pl[now].used[i]) {
				pl[now].used[i]=0;
				gettre[now]=1;
				pl[now].handcard[i]=fun[1][3][1];
				break;
			}
		}
		if(!gettre[now]){
			pl[now].handcard[rand()%pl[now].cardcnt+1]=fun[1][3][1];
			gettre[now]=1;
		}
	}
	if(pl[now].occ==4 && pl[now].buff[0]>=8){
		for(int i=1;i<=pl[now].cardcnt;i++){
			if(pl[now].used[i]) {
				pl[now].used[i]=0;
				gettre[now]=1;
				pl[now].handcard[i]=fun[1][4][1];
				break;
			}
		}
		if(!gettre[now]){
			pl[now].handcard[rand()%pl[now].cardcnt+1]=fun[1][4][1];
			gettre[now]=1;
		}
	}
	if(pl[now].occ==5 && pl[now].maxhp>=320 && pl[now].hp<=240){
		for(int i=1;i<=pl[now].cardcnt;i++){
			if(pl[now].used[i]) {
				pl[now].used[i]=0;
				gettre[now]=1;
				pl[now].handcard[i]=fun[1][5][1];
				break;
			}
		}
		if(!gettre[now]){
			pl[now].handcard[rand()%pl[now].cardcnt+1]=fun[1][5][1];
			gettre[now]=1;
		}
	}
	if(pl[now].occ==6 && pl[now].buff[0]>=5){
		for(int i=1;i<=pl[now].cardcnt;i++){
			if(pl[now].used[i]) {
				pl[now].used[i]=0;
				gettre[now]=1;
				pl[now].handcard[i]=fun[1][6][1];
				break;
			}
		}
		if(!gettre[now]){
			pl[now].handcard[rand()%pl[now].cardcnt+1]=fun[1][6][1];
			gettre[now]=1;
		}
	}
	if(pl[now].occ==7 && pl[now].buff[0]>=4 && pl[now].hp<=300){
		for(int i=1;i<=pl[now].cardcnt;i++){
			if(pl[now].used[i]) {
				pl[now].used[i]=0;
				gettre[now]=1;
				pl[now].handcard[i]=fun[1][7][1];
				break;
			}
		}
		if(!gettre[now]){
			pl[now].handcard[rand()%pl[now].cardcnt+1]=fun[1][7][1];
			gettre[now]=1;
		}
	}
}

void adv(int x){
	if(pl[x].occ==1){
		pl[x].def=40;
	}
	else if(pl[x].occ==2 || pl[x].occ==6){
		pl[x].buff[5]=2;
	}
	else if(pl[x].occ==3){
		pl[x].hp+=40;
		pl[x].maxhp+=40;
	}
	else if(pl[x].occ==4){
		pl[x].def=75;
	}
	else if(pl[x].occ==5){
		pl[x].hp+=10;
		pl[x].maxhp+=10;
		pl[x].def+=25;
		pl[x].maxdef+=25;
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
	printf("的职业◇");//第一行标识谁选择职业
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
		}//各职业名字和简介展示
		SetPos(0,12);
		occ_func(cursor);
		if(mode==1){
			SetPos(0,24);
			occ_treasure(cursor);
		}
		input=getch();
		if(input==UP || input==LEFT || input=='w' || input=='a')
			cursor--;
		if(input==DOWN || input==RIGHT || input=='s' || input=='d')
			cursor++;
		if(input>='1' && input<='8'){
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
		printf("                                   ");
		
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
		if(pl[rnk].buff[6])
			SetColor(5),printf("迷惑 %d : ",pl[rnk].buff[6]);
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
		for(i=1;i<=35-pl[rnk].maxhp/40-pl[rnk].def/20;i++)
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

void UI_other(){
	printf("                                                                                              \n");
	SetColor(7);
	SetPos(0,Row-1);
	printf("P%d",now); 
	SetPos(5,Row-1);
	printf("  剩余手牌:%d/%d",pl[3-server_mode].rest,pl[3-server_mode].cardcnt); 
	if(pl[3-server_mode].occ==7){
		SetColor(6);
		SetPos(30,Row-1);
		printf("  [神圣意志]:%d/%d",pl[3-server_mode].buff[10],pl[3-server_mode].cardcnt); 
	}
	SetColor(7);
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
	for(int i=1;i<=min(appcnt,6);i++) {
		int x=i+max(0,appcnt-6);
		if(appcard[x].func==-2) {
			appcnt--;
			break;
		}
		if(i==min(appcnt,6)) {
			SetColor(11,0);
			SetPos(11,Row+i);
			printf("%d",appcard[x].cost);//费用
			if(appcard[x].func==-1) {
				SetColor(0,8);
				SetPos(14,Row+i);
				printf(appcard[x].Name());//牌名
				printf("                 ");
				SetPos(20+14,Row+i);
				printf("%-3d         ",appcard[x].ATK);
				SetPos(30+14,Row+i);
				printf("%-3d         ",appcard[x].HEAL);
				SetPos(40+14,Row+i);
				printf("%-3d         ",appcard[x].DEF);
				SetPos(50+14,Row+i);
				SetColor(10,8);
				printf("YES");//MISS
				SetPos(11,Row+9);
				SetColor(7,0);
				printf("                                                 ");
			}
			else {
				SetColor(0,8);
				SetPos(14,Row+i);
				printf(appcard[x].Name());//牌名
				printf("                 ");
				SetPos(20+14,Row+i);
				printf("%-3d         ",appcard[x].ATK);
				SetPos(30+14,Row+i);
				printf("%-3d         ",appcard[x].HEAL);
				SetPos(40+14,Row+i);
				printf("%-3d         ",appcard[x].DEF);
				SetPos(50+14,Row+i);
				SetColor(4,8);
				printf("NO");
				SetPos(11,Row+9);
				SetColor(7,0);
				printf(appcard[x].Intro());
			}
			SetColor(7,0);
		}
		else {
			SetColor(11);
			SetPos(11,Row+i);
			printf("%d",appcard[x].cost);//费用
			SetColor(7);
			SetPos(14,Row+i);
			printf(appcard[x].Name());//牌名
			printf("               ");
			SetPos(20+14,Row+i);
			printf("%-3d         ",appcard[x].ATK);
			SetPos(30+14,Row+i);
			printf("%-3d         ",appcard[x].HEAL);
			SetPos(40+14,Row+i);
			printf("%-3d         ",appcard[x].DEF);
			SetPos(50+14,Row+i);
			if(appcard[x].func==-1) {
				SetColor(10);
				printf("YES");//MISS
				SetPos(11,Row+9);
				SetColor(7);
				printf("                                                 ");
			}
			else {
				SetColor(4);
				printf("NO");
				SetPos(11,Row+9);
				SetColor(7);
				printf(appcard[x].Intro());
			}
		}
	}
}

void start_turn(int now){
	for(int i=1;i<=2;i++) pl[i].prehp=pl[i].hp;//记录回合开始血量
	if(pl[now].cost==0 && pl[now].occ==1)  pl[now].cost=1;//浪人[拾荒]
	pl[now].cost=min(pl[now].cost+1,pl[now].maxcost);//加费 
	if(env_now==7)
		pl[now].cost=min(pl[now].cost+1,pl[now].maxcost);//环境加费 

	//职业特性
	if(pl[now].occ==3){
		if(pl[now].buff[0]){
			if(pl[now].hp<=100) pl[now].hp+=min(pl[now].buff[0],(100-pl[now].hp)/20+1)*20;
			pl[now].cost=min(pl[now].maxcost,pl[now].cost+pl[now].buff[0]);
			pl[now].buff[0]=0;
		}
		if(rand()%100<35) pl[now].buff[0]++;
	}//法师<★法力>标记获得费用
	if(pl[now].occ==4) {
		if(pl[now].def==0) pl[now].def=45;
		pl[now].buff[0]=max(0,pl[now].buff[0]-1);
	}//战士叠甲并清除<★疲惫>
	if(pl[now].occ==5) pl[now].cost=3;//地精每回合3费
	if(pl[now].occ==6){
		pl[now].cost=min(pl[now].cost+1,pl[now].maxcost);
		int da=5*pl[now].buff[0];
		if(da<pl[now].def) pl[now].def-=da;
		else {
			if(pl[now].def>0) Shake(3,1);
			pl[now].hp-=da-pl[now].def,pl[now].def=0;
		}
	}//恶魔<★原罪>让自己受伤
	if(pl[now].buff[5])//治疗buff
		pl[now].hp=min(pl[now].hp+30,pl[now].maxhp);
	if(pl[now].buff[1])//燃烧buff
		pl[now].hp-=40;
	if(pl[now].buff[2])//中毒buff
		pl[now].hp-=pl[now].hp*0.2;
	pl[now].UpdateBuff(1);
	//buff前判

	if(mode==2){
		pl[now].buff[rand()%6+1]++;
	}//娱乐模式：随机buff

	if(env_now==5){
		pl[now].hp=min(pl[now].maxhp,(int)(pl[now].hp+pl[now].maxhp*0.05));
	}//彩虹
	if(env_now==6){
		pl[now].hp=(pl[now].hp-pl[now].maxhp*0.05);
	}//酸雨
	//环境判断
}

void another_player_quit(int server_mode){
	system("cls");
	SetPos(11,0);
	printf("对方退出了游戏，他也许是投降了...\n");
	Sleep(1500);
	system("cls");
	winner=server_mode;
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
}
int Ask(int now){
	int option_use=0,option_giveup=0,option_over=0;
	start_turn(now);

	if(mode==1)treasure(now);
	UI();
	if(Check(now)){send_gaming(void_card); return 0;}
	//死亡判断

	for(int i=1;i<=pl[now].cardcnt;i++){
		if(pl[now].used[i]){
			pl[now].used[i]=0;
			if(pl[now].occ==7 && rand()%100<pl[now].buff[0]*2){
				pl[now].handcard[i]=lib[7][libcnt[7]+1];
			}
			else{
				pl[now].handcard[i]=pl[now].heap[(rand()%pl[now].heapn)+1];
			}
		}
	}
	pl[now].rest=pl[now].cardcnt;
	//补充手牌
	if(pl[now].occ==7){
		pl[now].buff[10]=0;
		for(int i=1;i<=pl[now].cardcnt;i++) 
			if(!pl[now].used[i] && pl[now].handcard[i].id==114) pl[now].buff[10]++; //114-->[神圣意志]
		if(pl[now].buff[10]==pl[now].cardcnt) winner=now;
	}//牧师检索[神圣意志]
	use_card=(Card){0,0,0,0,0,-2,0};
	if(send_gaming(use_card)<0) another_player_quit(server_mode);
	int cursor=1;
	SetColor(7);
	while(!winner){
		SetColor(7);
		SetPos(0,Row-1);
		printf("                                                                   "); 
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
			SetPos(5,Row+i);
			if(pl[now].used[i])SetColor(8);
			if(cursor==i && !option_giveup && !option_use) SetColor(color=15),printf("◎%d",i); 
			else if(cursor==i && option_giveup) SetColor(color=15),printf("×%d",i); 
			else if(cursor==i && option_use) SetColor(color=15),printf("●%d",i); 
			else SetColor(color=7),printf("  %d",i); 
			if(pl[now].used[i]){
				SetColor(8);
				printf("                                                                     ");
				SetPos(14,Row+i);
				printf("[Used.]");
				continue;
			}

			SetPos(11,Row+i);
			if(pl[now].handcard[i].func==15) {
				pl[now].handcard[i].ATK=pl[now].cost*35+25;
				pl[now].handcard[i].cost=pl[now].cost;
			}//[背水一战]
			if(pl[now].handcard[i].func==32){
				int da=0;
				for(int i=1;i<=pl[now].cardcnt;i++){
					if(pl[now].used[i]) continue;
					if(pl[now].handcard[i].id==98) da+=60; //98-->[虚空垃圾]
				}
				pl[now].handcard[i].ATK=da;
			}
			if(pl[now].cost>=pl[now].handcard[i].cost)SetColor(11);
			else SetColor(3);//费用是否足够
			printf("%d",pl[now].handcard[i].cost);
			SetColor(color);
			SetPos(14,Row+i);
			printf(pl[now].handcard[i].Name());//牌名
			printf("               ");
			SetPos(20+14,Row+i);
			printf("%-3d",pl[now].handcard[i].ATK);
			if(pl[now].handcard[i].ATK>0){
				int atk=pl[now].handcard[i].cal_atk(now,3-now);
				if(pl[now].handcard[i].ATK!=atk){
					if(pl[now].handcard[i].ATK < atk)SetColor(12);
					else SetColor(8);
					printf("(%d)   ",atk);
					SetColor(7);
				}
			}
			else{
				printf("     ");
			}//ATK
			SetPos(30+14,Row+i);
			printf("%-3d",pl[now].handcard[i].HEAL);
			if(pl[now].handcard[i].HEAL>0){
				int heal=pl[now].handcard[i].cal_heal(now,3-now);
				if(pl[now].handcard[i].HEAL!=heal){
					if(pl[now].handcard[i].HEAL < heal)SetColor(10);
					else SetColor(8);
					printf("(%d)   ",heal);
					SetColor(7);
				}
			}
			else{
				printf("      ");
			}//HEAL
			SetPos(40+14,Row+i);
			printf("%-3d",pl[now].handcard[i].DEF);
			if(pl[now].handcard[i].DEF>0){
				int def=pl[now].handcard[i].cal_def(now,3-now);
				if(pl[now].handcard[i].DEF!=def){
					if(pl[now].handcard[i].DEF < def)SetColor(15);
					else SetColor(8);
					printf("(%d)   ",def);
					SetColor(7);
				}
			}
			else{
				printf("      ");
			}//DEF
			SetPos(50+14,Row+i);
			printf("%-3d%%",pl[now].handcard[i].MISS);//MISS
			int miss=pl[now].handcard[i].cal_miss(now,3-now);
			if(pl[now].handcard[i].MISS!=miss){
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
			printf("                                  ");
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
					if(pl[now].handcard[cursor].Use(now,3-now)) {//牌的使用
						if(pl[now].handcard[cursor].func==40){
							pl[now].used[cursor]=0;
							pl[now].rest++;
							pl[now].handcard[cursor]=pl[3-now].heap[(rand()%pl[3-now].heapn)+1];
							pl[now].handcard[cursor].MISS=pl[now].handcard[cursor].func=0;
						}//[不完全记忆]
						if(pl[now].handcard[cursor].func==60){
							pl[now].used[cursor]=0;
							pl[now].rest++;
							pl[now].handcard[cursor]=lib[1][7];
						}//[熟练拾荒]变为[无中生有]
						if(pl[now].handcard[cursor].func==65){
							Card x=pl[3-now].heap[rand()%pl[3-now].heapn+1];
							x.MISS=x.func=0;
							x.Use(now,3-now);
							SetPos(0,1);SetColor(7,0);
							printf("使用了");
							printf(x.Name());
						}//[精神控制]
					} 
					if(pl[now].occ==5 && rand()%100<80) {
						pl[now].used[cursor]=0;
						pl[now].rest++;
						pl[now].handcard[cursor]=pl[now].heap[(rand()%pl[now].heapn)+1];
					}
					if(pl[now].occ==7){
						for(int i=1;i<=pl[now].cardcnt;i++){
							if(pl[now].used[i]) continue;
							if(pl[now].handcard[i].func==41) pl[now].handcard[i].ATK+=10;
						}
					}//[意识反馈]
					while(pl[now].used[cursor] && cursor<=pl[now].cardcnt+1 && pl[now].rest){
						cursor++;
						if(cursor>pl[now].cardcnt) cursor=1;
					}
					if(pl[now].occ==7){
						pl[now].buff[10]=0;
						for(int i=1;i<=pl[now].cardcnt;i++) 
							if(!pl[now].used[i] && pl[now].handcard[i].id==114) pl[now].buff[10]++; //114-->[神圣意志]
						if(pl[now].buff[10]==pl[now].cardcnt) winner=now;
					}//牧师检索[神圣意志]
					if(send_gaming(use_card)<0) another_player_quit(server_mode);
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
					else if(pl[now].handcard[cursor].func>=27 && pl[now].handcard[cursor].func<=29){
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
				if(pl[now].occ==7){
					pl[now].buff[10]=0;
					for(int i=1;i<=pl[now].cardcnt;i++) 
						if(!pl[now].used[i] && pl[now].handcard[i].id==114) pl[now].buff[10]++; //114-->[神圣意志]
					if(pl[now].buff[10]==pl[now].cardcnt) winner=now;
				}//牧师检索[神圣意志]
				if(send_gaming(void_card)<0) another_player_quit(server_mode);
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
					++pl[now].buff[0];
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
			for(int i=1;i<=pl[now].cardcnt;i++){
				SetColor(8);
				SetPos(0,Row+i);
				printf("                                                                        ");
				SetPos(14,Row+i);
				printf("[Used.]");
			}
			SetPos(11,Row+9);
			printf("                                                                        ");
			if(pl[now].occ==7 && pl[now].cost==0) {
				++pl[now].buff[0];
			}
			Sleep(200);
			while(_kbhit()) getch();
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
	system("cls");
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
		if(cursor!=3) SetColor(7,0);
		else SetColor(0,7);
		printf("游戏模式                                   ");
		SetPos(25,7);
		if(mode==0){
			printf("经典模式");
			SetPos(8,15);SetColor(7,0);
			printf("最经典的游戏模式                                             ");
			printf("\n\t                                          ");
		}
		else if(mode==1){
			printf("竞技模式：宝藏牌");
			SetPos(8,15);SetColor(7,0);
			printf("各职业增加宝藏牌,特殊条件下会置入手牌,每局仅1次                    ");
			printf("\n\t                                          ");
		}
		else if(mode==2){
			printf("欢乐模式：随机buff");
			SetPos(8,15);SetColor(7,0);
			printf("每回合开始时获得1回合随机buff,双方牌库中添加娱乐牌");
			printf("\n\t所有牌ATK/HEAL/DEF降为原先的70%%");
		}
		else if(mode==3){
			printf("");
			SetPos(8,15);SetColor(7,0);
			printf("");
			printf("\n\t");
		}
		//游戏模式设定
		if(server_mode==1){
			input=getch();
			if(input==UP || input==LEFT || input=='w' || input=='a') cursor--;
			if(input==DOWN || input==RIGHT || input=='s' || input=='d') cursor++;
			if(cursor>3) cursor=1;
			if(cursor<1) cursor=3;
			if(input=='z' || input=='x'){
				if(cursor==1){
					env_on^=1;
				}
				if(cursor==2){
					player_bgn=(player_bgn+1)%3;
				}
				if(cursor==3){
					mode=(mode+1)%3;
				}
			}
			if(input==SPACE || input==ENTER){
				send_int(8255); break;
			}
			if(send_int(6000+env_on)<0) another_player_quit(server_mode);
			if(send_int(6100+mode)<0) another_player_quit(server_mode);
			if(send_int(6200+player_bgn)<0) another_player_quit(server_mode);
		}
		else{
			bool quit_option=0;
			for(int i=1;i<=3;i++){
				int recv_val=recv_message();
				if(recv_val<0) another_player_quit(server_mode);
				if(recv_val==8255){
					quit_option=1; break;
				}
			}
			if(quit_option) break;
		}
	}
	return;
}
void Connect(){
	int cursor=1;
	while(1){
		SetPos(1,1);
		if(cursor!=1) SetColor(7,0);
		else SetColor(0,7);
		printf("1.建立服务端        ");

		SetPos(1,2);
		if(cursor!=2)SetColor(7,0);
		else SetColor(0,7);
		printf("2.连接到已有的服务端");

		input=getch();
		if(input==UP || input==LEFT || input=='w' || input=='a') cursor--;
		if(input==DOWN || input==RIGHT || input=='s' || input=='d') cursor++;
		if(cursor>2) cursor=1;
		if(cursor<1) cursor=2;
		if(input=='z' || input==SPACE || input==ENTER){
			server_mode=cursor;
			break;
		}
	}
	return;
}

int main(){
	SetConsoleOutputCP(65001);
	mouse(0);
	SetConsoleTitle("CARD GAME:v2.0.0");
	srand(time(NULL));
	previous();//获得公共牌库和职业牌库
	bool connect_established=0;
	while(!connect_established){
		connect_established=1;
		system("cls");
		Connect();
		SetColor(7,0);
		if(server_mode==2) printf("\n输入服务端ip地址:");
		else system("cls"),printf("等待玩家连入...\n");
		if(TCP_initialize(server_mode)!=0){
			connect_established=0;
			system("cls");
			if(server_mode==1){
				printf("无法设立服务端...请检查是否有同一程序正在运行...\n");
				getch();
			}
			else{
				printf("无法连接至服务器...请检查网络状况及服务器是否正常启动...\n");
				getch();
			}
		}
	}
	if(server_mode==1){
		system("cls");
		SetConsoleTitle("CARD GAME:Options");
		Options();
		SetColor(0,7);
		system("color 07");
		system("cls");//清屏并切换到Choose界面
		if(send_int(6000+env_on)<0) another_player_quit(server_mode);
		if(send_int(6100+mode)<0) another_player_quit(server_mode);
	}
	else {
		// system("cls");
		// printf("正在等待服务端设定游戏模式...\n");
		Options();
	}
	while(_kbhit()) getch();
	system("cls");
	SetConsoleTitle("CARD GAME:Choose Your Identity.");
	Choose(server_mode);//选择职业
	SetPos(0,20);
	printf("P%d的职业是",server_mode);
	printf(occ_name(pl[server_mode].occ));

	if(pl[server_mode].occ!=6) pl[server_mode].cost=3;//初始费用设置
	init(server_mode);//获得相应牌形成牌库
	for(int i=1;i<=pl[server_mode].cardcnt;i++) {
		pl[server_mode].handcard[i]=pl[server_mode].heap[(rand()%pl[server_mode].heapn)+1];
	}
	//初始发牌

	if(server_mode==1) send_int(2010);
	else send_int(2020);
	send_player(pl[server_mode]);
	recv_message();
	SetConsoleTitle("Here we go...");
	Sleep(1500);
	while(_kbhit()) getch();
	system("cls");//清屏并开始游戏
	if(server_mode==1){
		if(!player_bgn){
			player_bgn=rand()%2+1;
		}//随机先手
		now=player_bgn;
		adv(3-player_bgn);//后手补偿
	}

	env_now=0;

	char title[]="CARD GAME:Turn of P1";
	if(server_mode==1){
		if(send_gaming(void_card)<0) another_player_quit(server_mode);
	}
	else if(recv_gaming()<0) another_player_quit(server_mode);
	if(server_mode!=now) Sleep(1500);
	while(!winner && !Check(now)){
		title[19]=now+'0';
		SetConsoleTitle(title);
		if(now==server_mode){
			SetPos(11,Row+13);
			printf("即将轮到你的回合......");
			Sleep(1500);
			while(_kbhit()) getch();
			appcnt=0;
			system("cls");
			if(Ask(now)==1) break;			//origin copy
			if(pl[now].occ==7){
				pl[now].buff[10]=0;
				for(int i=1;i<=pl[now].cardcnt;i++) 
					if(!pl[now].used[i] && pl[now].handcard[i].id==114) pl[now].buff[10]++; //114-->[神圣意志]
				if(pl[now].buff[10]==pl[now].cardcnt) winner=now;
			}//牧师检索[神圣意志]
			pl[now].UpdateBuff(2);
			now++;
			if(now>2) now=1;

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
			}			//origin copy
			// pl[now].rest=pl[now].cardcnt;
			if(send_gaming(void_card)<0) another_player_quit(server_mode);
			SetPos(11,Row+13);
			printf("你的回合即将结束......");
			Sleep(1500);
			while(_kbhit()) getch();
			system("cls");
		}
		else{
			UI();
			UI_other();
			if(recv_gaming()<0) another_player_quit(server_mode);
		}
	}
	Sleep(1500);
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
			printf(pl[now].handcard[i].Name());//牌名
			printf("               ");
			SetPos(20+14,Row+i);
			printf("%-3d",pl[now].handcard[i].ATK);
			//ATK
			SetPos(30+14,Row+i);
			printf("%-3d",pl[now].handcard[i].HEAL);//HEAL
			SetPos(40+14,Row+i);
			printf("%-3d",pl[now].handcard[i].DEF);//DEF
			SetPos(50+14,Row+i);
			printf("%-3d%%",pl[now].handcard[i].MISS);//MISS
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
			printf(pl[now].handcard[i].Name());//牌名
			printf("                 ");
			SetPos(20+14,Row+i);
			printf("%-3d",pl[now].handcard[i].ATK);
			printf("                 ");
			//ATK
			SetPos(30+14,Row+i);
			printf("%-3d",pl[now].handcard[i].HEAL);//HEAL
			printf("                 ");
			SetPos(40+14,Row+i);
			printf("%-3d",pl[now].handcard[i].DEF);//DEF
			printf("                 ");
			SetPos(50+14,Row+i);
			printf("%-3d%%",pl[now].handcard[i].MISS);//MISS

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
