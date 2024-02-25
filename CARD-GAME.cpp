#include<bits/stdc++.h>
#include<windows.h>
#include<conio.h> 
using namespace std;

int DimlyCheck=0;
int DestructiveStrike=0;
int Tysta=0;

int version1=4;
int version2=0;
int version3=1;
string version="4.0.1";

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define SPACE 32
#define ENTER 13

inline bool checkup(char c){return c==UP or c=='w' or c=='W';}
inline bool checkdown(char c){return c==DOWN or c=='s' or c=='S';}
inline bool checkleft(char c){return c==LEFT or c=='a' or c=='A';}
inline bool checkright(char c){return c==RIGHT or c=='d' or c=='D';}
inline bool checkenter(char c){return c==SPACE or c==ENTER;}
inline bool checkz(char c){return c=='z' or c=='Z' or c=='+';}
inline bool checkx(char c){return c=='x' or c=='X' or c=='-';}
inline bool checkc(char c){return c=='c' or c=='C';}
inline bool checkq(char c){return c=='q' or c=='Q';}
inline bool checkt(char c){return c=='t' or c=='T';}

#define MAXAPCARDNUM 45
//ASCII 

#include"CARDGAMEsocket.hpp"

//-------------------------------------------------
int UI();
int Sort(int now);

int Check(int now){
	if(pl[now].hp<=0){
		if(pl[now].occ==13 and !death[now])
		{
			if(pl[now].buff[0]/1000000==0)
				pl[now].buff[0]+=3000000;
			return 0;
		}
		death[now]=1;
		if(mode[6]==0) winner=3-now;
		if(mode[6]==1)
		{	
			if(now==3) winner=1;
			if(death[1] and death[2]) winner=3;
		}
		return 1;
	}
	return 0;
}

int Card::cal_atk(int from,int to){//计算实际攻击力
	double damage=ATK;
	if(func==121) return damage;//[天使92]固定 
	if(func==15 or func==89) ;
	else
	{
		if(pl[from].occ==2 && ATK>0) damage=ATK+5*pl[from].buff[0];//术士
		if(pl[from].occ==5 && ATK>0) damage=ATK+ATK*pl[from].buff[0]*0.09;//地精
		if(pl[from].occ==4 && pl[from].buff[0])//战士
			damage-=0.04*damage*pl[from].buff[0];
		if(func==115 and pl[from].occ==10 and pl[from].buff[0]%11==0) damage*=2;
		if(func==144 and pl[to].def==0) damage*=2;
		if(func==162 and pl[from].occ==13) damage*=1+(pl[from].buff[0]/1000%1000/3)/100.0;
		if(pl[from].occ==13) damage*=1+(pl[from].buff[0]%1000)/100.0;
		//职业影响
		if(pl[from].buff[3])//狂暴
			damage*=2;
		if(pl[from].buff[4])//虚弱
			damage*=0.7;
		if(pl[to].buff[7])//易伤
			damage*=1+pl[to].buff[7]*0.25; 
		//Buff影响
		if(env_now==1)//血月
			damage*=1.2;
		if(env_now==2)//暴雨
			damage*=0.8;
		//天气影响	
	}
	if(mode[4]) //随机buff
		damage*=0.7;
	if(mode[5]) //高失误 
		damage*=1.5; 
	
	return (int)damage;
}

int Card::cal_heal(int from,int to){//计算实际恢复量
	double heal=HEAL;
	if(pl[from].buff[4])//虚弱
		heal*=0.7;
	if(mode[4]) //随机buff
		heal*=0.7;
	return heal;
}

int Card::cal_def(int from,int to){//计算实际叠盾
	double def=DEF;
	if(mode[4]) //随机buff
		def*=0.7;
	return def;
}

int Card::cal_miss(int from,int to){
	int miss=MISS;
	if(110<=func and func<=114) return miss;
	if(func==121) return miss;
	if(func==122) return miss;
	if(func==187) return miss;
	if(env_now==3)
		miss+=10;
	if(env_now==4)
		miss-=10;
	if(pl[from].buff[6])//迷惑
		miss+=25;
	if(pl[to].occ==10)
		miss+=5;
	if(pl[to].occ==12)
		miss+=pl[to].buff[0]*10;
	if(mode[5]) //高失误 
		miss+=25;
	if(miss>100) miss=100;
	if(miss<0) miss=0;
	return miss;
}

Card use_card;

int Card::Use(int from,int to){
	int miss=cal_miss(from,to);
	use_card=(Card){0,0,0,0,0,-2,0};
	use_card.id=id;
	use_card.cost=cost;
	use_card.func=func;
	if(rad()%100<miss){
		if(from!=3) SetPos(11,Row+10),printf("操作失误了!");
		if(pl[from].occ==4 && ATK>0) pl[from].buff[0]+=2;
		if(pl[from].occ==10) pl[from].hp=min(pl[from].hp+15*(cost+1),pl[from].maxhp);
		if(pl[from].occ==51)
		{
			if(!death[1]) pl[1].cost=max(pl[1].cost-1,0);
			if(!death[2]) pl[2].cost=max(pl[2].cost-1,0);
		}
		else pl[from].cost=min(pl[from].maxcost,pl[from].cost+cost/2);
		if(pl[to].occ==12 and pl[to].buff[0]>0)
		{
			pl[to].buff[0]-=1;
			if(rad()%100<40) pl[to].cost=min(pl[to].cost+1,pl[to].maxcost);
		}	
		UI();
		use_card.MISS=1;
		if(mode[6]==1) use_card.MISS+=to*10;
		return -1;
	}//失效判定
	
	int invalid=0;
	for(int i=1;i<=pl[to].cardcnt;i++)
		if(ATK>=80 and pl[to].used[i]==0 and pl[to].handcard[i].func==157 and pl[to].cost>=2)
			invalid=i;
	if(invalid)
	{
		if(from!=3) SetPos(11,Row+10),printf("被对手[机关陷阱]无效化!");
		pl[to].cost-=2;pl[to].used[invalid]=1,pl[to].rest--;
		use_card.func=func=0,use_card.MISS=3;
	}
	
	if(pl[3].occ==51) pl[3].buff[0]+=rad()%2+1;
	
	int damage,flag=0;
	if(func) flag=Special(from,to);
	if(flag==4)
	{
		for(int i=1;i<=2;i++)
			if(pl[i].occ==10) pl[i].buff[0]=rad()%100;
		if(pl[3].occ==52)
		{
			int s1=pl[3].buff[0]%100,s2=pl[3].buff[0]/100%100,s3=pl[3].buff[0]/10000;
			if(s1>0) s1--;else s1=s2;
			pl[3].buff[0]=s1+s2*100+s3*10000;
			if(rad()%100<4*s1 and from!=3)
			{
				vector<int> tmp={1,2,4,6,7,8};
				pl[from].buff[tmp[rad()%int(tmp.size())]]++;
			}
		}
		UI();
		if(mode[6]==1) use_card.MISS+=to*10;
		return 0;
	}
	damage=use_card.ATK=cal_atk(from,to);
	if(invalid) damage=0; 
	
	if(pl[from].occ==11 and damage!=0 and pl[from].buff[0]>=5) pl[to].buff[4]+=1,pl[from].buff[0]-=5; 

	
	//护盾抵消
	if(pl[to].def>0 && flag!=1){
		if(pl[to].occ==9)
		{
			int sum=min(damage,pl[to].def);
			if(pl[from].occ!=8) pl[from].hp-=sum*pl[to].buff[0]/10.0;
			else
			{
				int defdamage=sum*pl[to].buff[0]/10.0;
				if(defdamage<=pl[from].buff[0]*3) pl[from].buff[0]-=defdamage/3,defdamage=0;
				else defdamage-=pl[from].buff[0]*3,pl[from].buff[0]=0;
				if(defdamage>0) pl[from].hp-=defdamage;
			}
		}
		if(damage>=pl[to].def){
			damage-=pl[to].def,pl[to].def=0;
			Shake(3,1);//击破护盾的动画
		}
		else pl[to].def-=damage,damage=0;
	}
	if(pl[to].occ==8){
		if(damage<=pl[to].buff[0]*3){
			pl[to].buff[0]-=damage/3,damage=0;
		}
		else damage-=pl[to].buff[0]*3,pl[to].buff[0]=0;
	}
	if(damage>0){
		pl[to].hp-=damage;
	}
	if(damage>=150){//重击动画
		if(damage>=3000) system("color E7");
		else if(func==187) system("color D7");
		else system("color 47");
		Shake(10,1);
		system("color 07");
		cls();
		UI();
	}
	//进攻
	if(func==172 and to<3)
	{
		int heal=cal_heal(from,to),def=cal_def(from,to);
		pl[to].hp=min(pl[to].maxhp,pl[to].hp+heal);
		if(pl[to].occ==9) 
			if(pl[to].def+def>pl[to].maxdef) 
				pl[3].hp-=(pl[to].def+def-pl[to].maxdef)*(10-pl[to].buff[0])/10.0;
		pl[to].def=min(pl[to].maxdef,pl[to].def+def);	
	}
	else
	{
		int heal=cal_heal(from,to);
		if(invalid) heal=0;
		use_card.HEAL=heal;
		pl[from].hp=min(pl[from].maxhp,pl[from].hp+heal);
		int def=cal_def(from,to);
		if(invalid) def=0;
		use_card.DEF=def;
		if(pl[from].occ==9) 
			if(pl[from].def+def>pl[from].maxdef) 
			{
				int defdamage=(pl[from].def+def-pl[from].maxdef)*(10-pl[from].buff[0])/10.0;
				if(pl[to].occ!=8) pl[to].hp-=defdamage;
				else
				{
					if(defdamage<=pl[to].buff[0]*3) pl[to].buff[0]-=defdamage/3,defdamage=0;
					else defdamage-=pl[to].buff[0]*3,pl[to].buff[0]=0;
					if(defdamage>0) pl[to].hp-=defdamage;	
				}
			}
		pl[from].def=min(pl[from].maxdef,pl[from].def+def);	
	}
	//套盾与回血
	if(HEAL<0 && pl[from].occ==2) pl[from].buff[0]++;
	if(pl[from].occ==4 && ATK>0) pl[from].buff[0]+=2;
	if(pl[from].occ==5 && flag==2){
		pl[from].buff[0]++;
		pl[from].maxhp+=10;
		pl[from].maxdef+=5;
		pl[from].hp=min(pl[from].maxhp,pl[from].hp+12);
	}
	if(pl[from].occ==5 && flag==3){
		pl[from].buff[0]+=2;
		pl[from].maxhp+=20;
		pl[from].maxdef+=10;
		pl[from].hp=min(pl[from].maxhp,pl[from].hp+24);
	}
	if(from==1 or from==2)
	{
		if(pl[from].occ==10 and (func!=109 and func!=117 and func!=121)) pl[from].buff[0]=rad()%100; 
		if(pl[3-from].occ==10) pl[3-from].buff[0]=rad()%100;	
	}
	else
		for(int i=1;i<=2;i++)
			if(pl[i].occ==10) pl[i].buff[0]=rad()%100; 
	if(pl[from].occ==13)
	{
		int s1=pl[from].buff[0]%1000,s2=pl[from].buff[0]/1000%1000,s3=pl[from].buff[0]/1000000;
		s2+=2,s1=min(s2,s1+15);
		pl[from].buff[0]=s1+s2*1000+s3*1000000;
	}
	if(pl[3].occ==52)
	{
		int s1=pl[3].buff[0]%100,s2=pl[3].buff[0]/100%100,s3=pl[3].buff[0]/10000;
		if(s1>0) s1--;else s1=s2;
		pl[3].buff[0]=s1+s2*100+s3*10000;
		if(rad()%100<4*s1 and from!=3)
		{
			vector<int> tmp={1,2,4,6,7,8};
			pl[from].buff[tmp[rad()%int(tmp.size())]]++;
		}
	}
	UI();
	if(mode[6]==1) use_card.MISS+=to*10;
	if(Check(to)) return 1;
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
		pl[from].cost=min(pl[from].maxcost,pl[from].cost+2);
		pl[to].cost=min(pl[to].maxcost,pl[to].cost+2);
		pl[to].hp=min(pl[to].maxhp,pl[to].hp+20);
	}
	else if(func==4){
		if(damage>=pl[to].def)
			pl[to].buff[1]+=3;
	}
	else if(func==5){
		pl[from].buff[3]+=1;
	}
	else if(func==6){
		if(damage>=pl[to].def||pl[from].occ==5)
			pl[to].buff[2]+=3;
	}
	else if(func==7){
		int cnt=0;
		for(int i=1;i<=pl[from].cardcnt && cnt<2;i++){
			if(pl[from].used[i]){
				pl[from].used[i]=0;
				Card x=pl[from].heap[(rad()%pl[from].heapn)+1];
				while(x.id==23) x=pl[from].heap[(rad()%pl[from].heapn)+1];
				pl[from].handcard[i]=x;
				cnt++;
			}
		}
		pl[from].rest=0;
		for(int i=1;i<=pl[from].cardcnt;i++) if(!pl[from].used[i]) pl[from].rest++;
		return 4;
	}
	else if(func==8){
		if(pl[from].cardcnt<6){
			pl[from].cardcnt++;
			pl[from].used[pl[from].cardcnt]=1;
		}
		else pl[from].cost=min(pl[from].maxcost,pl[from].cost+2);
	}
	else if(func==9){
		pl[from].maxhp+=50;
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
		pl[to].buff[8]+=3;
	}
	else if(func==14){
		pl[to].buff[3]+=1;
	}
	else if(func==16){
		pl[to].cost=max(0,pl[to].cost-1);
		pl[to].buff[4]+=1;
		pl[from].buff[0]=max(0,pl[from].buff[0]-1);
	}
	else if(func==17){
		pl[from].buff[3]+=1;
		pl[to].buff[3]+=1;
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
		pl[to].def+=50;
		if(pl[to].occ==9) 
			if(pl[to].def>pl[to].maxdef) 
			{
				int defdamage=(pl[to].def-pl[to].maxdef)*(10-pl[to].buff[0])/10.0;
				if(pl[from].occ!=8) pl[from].hp-=defdamage;
				else
				{
					if(defdamage<=pl[from].buff[0]*3) pl[from].buff[0]-=defdamage/3,defdamage=0;
					else defdamage-=pl[from].buff[0]*3,pl[from].buff[0]=0;
					if(defdamage>0) pl[from].hp-=defdamage;	
				}
			}
		pl[to].def=min(pl[to].def,pl[to].maxdef);
	}
	else if(func==22){
		pl[from].maxhp-=20;
		pl[from].hp=min(pl[from].hp,pl[from].maxhp);
		pl[from].buff[0]++;
	}
	else if(func==23){
		pl[from].buff[5]+=2;
	}
	else if(func==24){
		pl[from].maxhp-=110;
		pl[from].hp=pl[from].maxhp;
		pl[from].buff[0]=0;
	}
	else if(func==25){
		pl[from].buff[0]=max(pl[from].buff[0]-1,0);
		pl[from].cost=min(pl[from].cost+3,pl[from].maxcost);
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
		if(pl[from].buff[8])
		{
			pl[from].hp-=150;
			system("color 97");Shake(10,1);
			system("color 07");cls();UI();
		}
		for(int i=1;i<=8;i++) pl[from].buff[i]=0;
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
		pl[from].hp=min(pl[from].maxhp,pl[from].hp+8*pl[from].buff[0]);
		pl[from].cost=min(pl[from].cost+pl[from].buff[0],pl[from].maxcost);
		pl[from].buff[0]=0;
	}
	else if(func==38){
		pl[to].buff[6]+=1;
	}
	else if(func==39){
		pl[from].buff[5]+=2;
		pl[to].buff[5]+=2;
	}
	else if(func==40){
		for(int i=1;i<=pl[to].cardcnt;i++)
			pl[to].used[i]=1;
		pl[to].rest=0;
	}
	else if(func==43){
		pl[from].heap[++pl[from].heapn]=lib[7][libcnt[7]+1];
	}
	else if(func==45){
		pl[to].buff[4]+=4;
	}
	else if(func==46){
		pl[from].cost=min(pl[from].maxcost,pl[from].cost+4);
		int x=rad()%pl[to].cardcnt+1;//选取随机槽位
		pl[to].handcard[x]=lib[7][8];//[法力结晶]
		if(pl[to].used[x]) pl[to].used[x]=0,pl[to].rest++;
	}
	else if(func==47){
		vector<int> qwq;qwq.clear();
		for(int i=1;i<=pl[to].cardcnt;i++)
			if(!pl[to].used[i]) qwq.push_back(i);
		if(!qwq.empty())
		{
			int x=qwq[rad()%(int)qwq.size()];//选取随机槽位
			pl[to].handcard[x].cost+=1;
		}
	}
	else if(func==48){
		for(int i=1;i<=pl[to].cardcnt;i++){
			if(pl[to].used[i]) continue;
			if(pl[to].handcard[i].func==42) pl[to].used[i]=1,pl[to].buff[0]+=2,pl[to].rest--; //42-->[神圣意志]
		}
	}
	else if(func==49){
		pl[to].buff[4]+=2;
	}
	else if(func==50){
		if(damage>=pl[to].def)
			pl[to].buff[1]+=2;
	}
	else if(func==51){
		pl[to].buff[rad()%8+1]+=rad()%2+1;
	}
	else if(func==52){
		pl[from].buff[rad()%8+1]+=rad()%2+1;
	}
	else if(func==53){
		pl[rad()%2+1].buff[rad()%8+1]+=rad()%2+1;
	}
	else if(func==54){
		pl[to].buff[rad()%8+1]+=rad()%2+1;
		pl[from].buff[rad()%8+1]+=rad()%2+1;
	}
	else if(func==55){
		int x=rad()%pl[to].cardcnt+1,y=rad()%pl[to].cardcnt+1;//选取随机槽位
		while(x==y) x=rad()%pl[to].cardcnt+1,y=rad()%pl[to].cardcnt+1;//去重
		pl[to].handcard[x]=pl[to].handcard[y]=lib[7][libcnt[7]+3];
		if(pl[to].used[x]==1) pl[to].used[x]=0,pl[to].rest++;
		if(pl[to].used[y]==1) pl[to].used[y]=0,pl[to].rest++;
	}
	else if(func==56){
		pl[to].buff[6]+=2;
	}
	else if(func==57){
		pl[from].buff[0]=max(pl[from].buff[0]-3,0);
		for(int i=1;i<=pl[from].heapn;i++){
			pl[from].heap[i].ATK*=1.4;
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
		pl[to].maxhp-=90;
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
				pl[from].heap[i].ATK=20;
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
			if(pl[from].heap[i].id==23) { //23-->[无中生有]
				pl[from].heap[i].id=-23;
				pl[from].heap[i].MISS=5;
			}
		}
	}
	else if(func==61){
		pl[from].buff[0]+=2;
		pl[from].maxhp+=60;
	}
	else if(func==62){
		pl[from].buff[0]+=3;
		pl[from].maxcost++;
		pl[from].heap[++pl[from].heapn]=fun[1][3][2];
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
		int x=rad()%pl[to].cardcnt+1;
		pl[to].handcard[x]=fun[1][7][2];
		if(pl[to].used[x]==1) pl[to].used[x]=0,pl[to].rest++;
	}
	else if(func==66){
		if(rad()%100<30){
			pl[from].buff[0]+=1;
		}
		for(int i=1;i<=pl[from].heapn;i++){
			if(pl[from].heap[i].id==54) pl[from].heap[i].ATK+=15; //54-->[法力成长]
		}
	}
	else if(func==67){
		pl[from].buff[3]+=1;
	}
	else if(func==68){
		pl[from].buff[0]+=rad()%2+1;
	}
	else if(func==69){
		pl[from].buff[8]++;
	}
	else if(func==70){
		if(pl[from].handcard[pl[from].cardcnt].id==119) pl[from].handcard[pl[from].cardcnt].ATK+=12;
	}
	else if(func==71){
		if(pl[from].handcard[pl[from].cardcnt].id==119) pl[from].handcard[pl[from].cardcnt].ATK+=20;
	}
	else if(func==72){
		pl[from].buff[7]+=5;
	}
	else if(func==73){
		pl[from].buff[8]+=2;
	}
	else if(func==74){
		pl[from].buff[7]+=24;
	}
	else if(func==76){
		pl[from].buff[0]+=rad()%3+2;
	}
	else if(func==77){
		pl[to].buff[4]=max(pl[to].buff[4],1);
	}
	else if(func==78){
		pl[from].cost=min(pl[from].maxcost,pl[from].cost+3);
		pl[from].buff[0]++;
	}
	else if(func==79){
		pl[from].buff[7]+=15;
	}
	else if(func==80){
		pl[from].buff[7]+=8;
	}
	else if(func==81){
		pl[to].buff[0]=max(0,pl[to].buff[0]-6);
		if(pl[from].handcard[pl[from].cardcnt].id==119) pl[from].handcard[pl[from].cardcnt].ATK+=40;
	}
	else if(func==83){
		if(rad()%100<60) pl[from].buff[0]++;
	}
	else if(func==85){
		pl[from].buff[0]+=5;
	}
	else if(func==86){
		pl[from].buff[0]+=9;
	}
	else if(func==87){
		pl[from].buff[0]+=18;
	}
	else if(func==88){
		if(pl[from].buff[0]>=30){
			pl[from].buff[0]-=15;
			pl[from].cost=min(pl[from].cost+3,pl[from].maxcost);
		}
		else pl[from].cost=min(pl[from].cost+cost,pl[from].maxcost);
	}
	else if(func==91){
		pl[from].hp+=(pl[from].maxhp-pl[from].hp)*0.20;
	}
	else if(func==93){
		pl[from].buff[0]+=22;
	}
	else if(func==94){
		pl[from].buff[0]=max(0,pl[from].buff[0]-3);
	}
	else if(func==95){
		if(pl[from].buff[0]>=15){
			pl[from].buff[0]-=6;
			bool flag=false;
			if(pl[from].buff[8]) pl[from].hp-=150,flag=true;
			if(pl[to].buff[8]) pl[to].hp-=150,flag=true;
			if(flag)
			{
				system("color 97");Shake(10,1);
				system("color 07");cls();UI();
			}
			for(int i=1;i<=8;i++) pl[from].buff[i]=pl[to].buff[i]=0;
		}
		else pl[from].cost=min(pl[from].cost+cost,pl[from].maxcost);
	}
	else if(func==96){
		pl[from].buff[0]=(pl[from].buff[0])*1.5;
	}
	else if(func==97){
		int cnt=0;
		for(int i=1;i<=pl[from].cardcnt && cnt<3;i++){
			if(pl[from].used[i]){
				pl[from].used[i]=0;
				Card x=pl[from].heap[(rad()%pl[from].heapn)+1];
				while(x.id==-23) x=pl[from].heap[(rad()%pl[from].heapn)+1];
				pl[from].handcard[i]=x;
				cnt++;
			}
		}
		pl[from].rest=0;
		for(int i=1;i<=pl[from].cardcnt;i++) if(!pl[from].used[i]) pl[from].rest++;
		return 4;
	}
	else if(func==98){
		pl[from].maxdef+=30;
	}
	else if(func==99){
		pl[from].def/=2;
		pl[from].buff[0]=max(0,pl[from].buff[0]-2);
	}
	else if(func==100){
		pl[from].def/=2;
		pl[from].buff[0]=min(6,pl[from].buff[0]+2);
	}
	else if(func==101){
		if(damage>=pl[to].def)
			pl[to].buff[1]+=2;
	}
	else if(func==102){
		pl[from].def=0;
	}
	else if(func==103){
		pl[from].maxdef+=70;
	}
	else if(func==104){
		pl[from].maxhp+=100; 
		pl[from].maxdef+=100;
	}
	else if(func==105){
		int chaotic=pl[from].maxdef;
		pl[from].maxdef=max(0,pl[from].maxdef-50);
		if(pl[from].occ==9) 
			if(pl[from].def>pl[from].maxdef) 
			{
				int defdamage=(pl[from].def-pl[from].maxdef)*(10-pl[from].buff[0])/10.0;
				if(pl[to].occ!=8) pl[to].hp-=defdamage;
				else
				{
					if(defdamage<=pl[to].buff[0]*3) pl[to].buff[0]-=defdamage/3,defdamage=0;
					else defdamage-=pl[to].buff[0]*3,pl[to].buff[0]=0;
					if(defdamage>0) pl[to].hp-=defdamage;	
				}
			}
		pl[from].def=min(pl[from].def,pl[from].maxdef);
		pl[from].cost=min(pl[from].cost+3,pl[from].maxcost);
		if(chaotic<50)
		{
			pl[from].maxhp=max(0,pl[from].maxhp-(50-chaotic));
			pl[from].hp=min(pl[from].hp,pl[from].maxhp);
		}
	}
	else if(func==106){
		int chaotic=pl[from].maxdef;
		pl[from].maxdef=max(0,pl[from].maxdef-100);
		if(pl[from].occ==9) 
			if(pl[from].def>pl[from].maxdef) 
			{
				int defdamage=(pl[from].def-pl[from].maxdef)*(10-pl[from].buff[0])/10.0;
				if(pl[to].occ!=8) pl[to].hp-=defdamage;
				else
				{
					if(defdamage<=pl[to].buff[0]*3) pl[to].buff[0]-=defdamage/3,defdamage=0;
					else defdamage-=pl[to].buff[0]*3,pl[to].buff[0]=0;
					if(defdamage>0) pl[to].hp-=defdamage;	
				}
			}
		pl[from].def=min(pl[from].def,pl[from].maxdef);
		if(chaotic<100)
		{
			pl[from].maxhp=max(0,pl[from].maxhp-(100-chaotic));
			pl[from].hp=min(pl[from].hp,pl[from].maxhp);
		}
	}
	else if(func==107){
		pl[to].def=0;
		pl[to].cost=min(pl[to].cost+2,pl[to].maxcost);
	}
	else if(func==108){
		pl[from].def*=2;
		if(pl[from].occ==9) 
			if(pl[from].def>pl[from].maxdef) 
			{
				int defdamage=(pl[from].def-pl[from].maxdef)*(10-pl[from].buff[0])/10.0;
				if(pl[to].occ!=8) pl[to].hp-=defdamage;
				else
				{
					if(defdamage<=pl[to].buff[0]*3) pl[to].buff[0]-=defdamage/3,defdamage=0;
					else defdamage-=pl[to].buff[0]*3,pl[to].buff[0]=0;
					if(defdamage>0) pl[to].hp-=defdamage;	
				}
			}
		pl[from].def=min(pl[from].def,pl[from].maxdef);
	}
	else if(func==109){
		bool flag=false;
		for(int i=1;i<=pl[from].cardcnt;i++)
			if(pl[from].handcard[i].func==121) flag=true;
		if(rad()%100<3 and flag) pl[from].buff[0]=92;
		else
		{
			vector<int> qwq;
			for(int i=0;i<=11;i++) qwq.push_back(0);
			for(int i=1;i<=pl[from].cardcnt;i++)
				if(!pl[from].used[i])
				{
					if(pl[from].handcard[i].func==110) qwq[6]=1;
					if(pl[from].handcard[i].func==111) qwq[2]=qwq[3]=qwq[5]=qwq[7]=1;
					if(pl[from].handcard[i].func==112) qwq[1]=1;
					if(pl[from].handcard[i].func==113) qwq[1]=qwq[3]=qwq[5]=qwq[7]=qwq[9]=1;
					if(pl[from].handcard[i].func==114) qwq[8]=1;
					if(pl[from].handcard[i].func==115) qwq[10]=1;
					if(pl[from].handcard[i].func==118) qwq[11]=1;
					if(pl[from].handcard[i].func==122) qwq[5]=1;
					if(pl[from].handcard[i].func==124) qwq[11]=1;
				}
			int qwqcnt=0;
			for(int i=0;i<=11;i++) qwqcnt+=qwq[i];
			if(qwqcnt==0)
			{
				pl[from].buff[0]=rad()%100;
				return 0;
			}
			while(1)
			{
				pl[from].buff[0]=rad()%100;
				int flag=0;
				for(int i=0;i<=9;i++)
					if(qwq[i]==1 and (pl[from].buff[0]%10==i or pl[from].buff[0]/10==i))
						flag=1;
				if(qwq[10]==1 and pl[from].buff[0]%11==0) flag=1;
				if(qwq[11]==1 and pl[from].buff[0]>=70) flag=1;
				if(flag==1) break;
			}
		} 
	}
	else if(func==112){
		pl[to].buff[6]+=3;
	}
	else if(func==113){
		pl[from].cost=min(pl[from].cost+2,pl[from].maxcost);
	}
	else if(func==116){
		pl[from].buff[4]+=2;
	}
	else if(func==117){
		pl[from].buff[0]=pl[to].hp%100; 
	}
	else if(func==119){
		pl[to].buff[1]++;
		pl[to].buff[2]++;
		pl[from].buff[3]++;
		pl[to].buff[4]++;
		pl[from].buff[5]++;
		pl[to].buff[6]++;
		pl[to].buff[7]++;
		pl[to].buff[8]++;
	}
	else if(func==120){
		for(int i=1;i<=pl[to].cardcnt;i++)
			if(!pl[to].used[i]) {
				int func2=pl[to].handcard[i].func;
				if((110<=func2 and func2<=114) or func2==121 or func2==122 or func2==187) continue;
				pl[to].handcard[i].MISS+=20;
				if(pl[to].handcard[i].MISS>100) pl[to].handcard[i].MISS=100;
			}
	}
	else if(func==122){
		pl[from].cost=min(pl[from].cost+3,pl[from].maxcost);
	}
	else if(func==123){
		for(int i=1;i<=pl[from].heapn;i++){
			int func2=pl[from].heap[i].func;
			if((110<=func2 and func2<=114) or func2==121 or func2==122) continue;
			pl[from].heap[i].MISS=max(0,pl[from].heap[i].MISS-10);
		}
	} 
	else if(func==125){
		pl[from].buff[0]+=1;
	}
	else if(func==126){
		pl[from].buff[0]=max(0,pl[from].buff[0]-3);
	}
	else if(func==127){
		pl[from].buff[0]+=3*cost;
	}
	else if(func==128){
		pl[from].buff[0]+=pl[to].buff[4]*4;
		pl[to].buff[4]=0;
	}
	else if(func==129){
		pl[from].cost=min(pl[from].cost+pl[to].buff[4],pl[from].maxcost);
	}
	else if(func==131){
		pl[from].buff[4]+=4;
		pl[to].buff[4]+=4;
	}
	else if(func==132){
		pl[from].buff[0]+=3;
		pl[to].buff[4]+=pl[from].buff[0]/5;
		pl[from].buff[0]=pl[from].buff[0]%5;
	}
	else if(func==133){
		pl[from].buff[0]*=2;
		pl[to].buff[4]++;
	}
	else if(func==135){
		for(int i=1;i<=pl[from].cardcnt;i++)
			if(pl[from].used[i]==0) pl[from].handcard[i].ATK+=pl[from].buff[0]*3;
	}
	else if(func==136){
		for(int i=1;i<=pl[from].cardcnt;i++){
			if(pl[from].used[i]){
				pl[from].used[i]=0;
				pl[from].handcard[i]=fun[1][11][2];
			}
		}
		pl[from].rest=pl[from].cardcnt;
		return 4;
	}
	else if(func==137){
		pl[to].buff[4]++;
		pl[from].heap[++pl[from].heapn]=lib[11][5];
	}
	else if(func==138){
		pl[from].buff[0]=0; 
	}
	else if(func==139){
		pl[to].buff[4]+=3;
	}
	else if(func==140){
		pl[to].buff[6]+=1;
	}
	else if(func==141){
		pl[from].buff[4]=0;
	} 
	else if(func==142){
		pl[from].heap[++pl[from].heapn]=lib[11][5];
	}
	else if(func==143){
		pl[from].buff[4]/=2;
		pl[to].cost=min(pl[to].maxcost,pl[to].cost+2);
	}
	else if(func==146){
		if(damage>=pl[to].def)
			pl[to].buff[2]+=1;
	}
	else if(func==147){
		pl[to].buff[6]+=1;
	}
	else if(func==148){
		if(pl[from].buff[0]<5)
		{
			pl[from].buff[0]+=1;
			if(rad()%100<40) pl[from].cost=min(pl[from].cost+1,pl[from].maxcost);	
		}
	}
	else if(func==149){
		if(pl[from].buff[0]<5)
		{
			pl[from].buff[0]=min(pl[from].buff[0]+2,5);
			pl[from].cost=min(pl[from].maxcost,pl[from].cost+2);
			if(rad()%100<40) pl[from].cost=min(pl[from].cost+1,pl[from].maxcost);	
		}
	}
	else if(func==150){
		if(pl[from].buff[0]==0)
		{
			pl[from].buff[0]+=1;
			if(rad()%100<40) pl[from].cost=min(pl[from].cost+1,pl[from].maxcost);
		}
	}
	else if(func==151){
		pl[to].buff[4]+=1;
		for(int i=1;i<=pl[to].cardcnt;i++)
			if(!pl[to].used[i] and pl[to].handcard[i].func==156)
			{
				for(int j=1;j<=2;j++) pl[to].heap[++pl[to].heapn]=lib[12][libcnt[12]+1];
				break;
			}
	}
	else if(func==155){
		int x=rad()%pl[to].cardcnt+1,y=rad()%pl[to].cardcnt+1;//选取随机槽位
		while(x==y) x=rad()%pl[to].cardcnt+1,y=rad()%pl[to].cardcnt+1;//去重
		pl[to].handcard[x]=pl[to].handcard[y]=lib[12][libcnt[12]+1];
		if(pl[to].used[x]==1) pl[to].used[x]=0,pl[to].rest++;
		if(pl[to].used[y]==1) pl[to].used[y]=0,pl[to].rest++;
	}
	else if(func==158){
		if(pl[from].cost==pl[to].cost)
			pl[to].buff[4]+=3,pl[to].maxhp-=100,pl[to].hp=min(pl[to].hp,pl[to].maxhp);
	}
	else if(func==159){
		pl[to].buff[1]+=4;
	}
	else if(func==160){
		if(pl[to].buff[1])
		{
			int s1=pl[from].buff[0]%1000,s2=pl[from].buff[0]/1000%1000,s3=pl[from].buff[0]/1000000;
			s1=min(s2,s1+25);
			pl[from].buff[0]=s1+s2*1000+s3*1000000;
		}
	}
	else if(func==161){
		pl[from].buff[0]+=6000; 
	}
	else if(func==163){
		pl[to].buff[6]+=2;
	}
	else if(func==164){
		pl[to].cost=0;
		for(int i=1;i<=pl[to].cardcnt;i++)
			pl[to].used[i]=1;
		pl[to].rest=0;
	}
	else if(func==165){
		if(pl[from].buff[0]%1000>=30)
			pl[from].cost=min(pl[from].cost+2,pl[from].maxcost);
	}
	else if(func==166){
		pl[from].buff[0]+=14000; 
	}
	else if(func==167){
		pl[from].buff[5]+=3; 
	}
	else if(func==169){
		int s1=pl[from].buff[0]%1000,s2=pl[from].buff[0]/1000%1000,s3=pl[from].buff[0]/1000000;
		s1=min(s2,s1+40);
		pl[from].buff[0]=s1+s2*1000+s3*1000000;
	}
	else if(func==170){
		if(pl[from].buff[8])
		{
			pl[from].hp-=150;
			system("color 97");Shake(10,1);
			system("color 07");cls();UI();
		}
		pl[from].buff[1]=pl[from].buff[2]=pl[from].buff[4]=pl[from].buff[6]=pl[from].buff[7]=pl[from].buff[8]=0;
		pl[from].buff[0]+=6000; 
	}
	else if(func==171){
		int s1=pl[from].buff[0]%1000,s2=pl[from].buff[0]/1000%1000,s3=pl[from].buff[0]/1000000;
		s1=min(s2,s1+ATK);
		pl[from].buff[0]=s1+s2*1000+s3*1000000;
	}
	else if(func==175){
		if(pl[from].buff[8])
		{
			pl[from].hp-=150;
			system("color 97");Shake(10,1);
			system("color 07");cls();UI();
		}
		for(int i=1;i<=8;i++)
		{
			if(i==3 or i==5) continue;
			pl[to].buff[i]+=pl[from].buff[i];
			pl[from].buff[i]=0;
		}
	}
	else if(func==176){
		pl[from].buff[0]+=24;
	}
	else if(func==177){
		pl[to].buff[1]+=2;
		pl[to].buff[4]+=2;
	}
	else if(func==178){
		for(int i=1;i<=5;i++)
			pl[to].heap[++pl[to].heapn]=lib[50][libcnt[50]+1];
	}
	else if(func==179){
		pl[from].maxhp-=500;
		pl[from].hp=pl[from].maxhp;
	}
	else if(func==180){
		if(damage>=pl[to].def)
			pl[to].buff[7]+=3;
	}
	else if(func==181){
		pl[to].buff[8]+=2;
	}
	else if(func==182){
		if(damage>=pl[to].def)
		{
			pl[to].buff[7]+=2;
			pl[from].cost=min(pl[from].cost+1,pl[from].maxcost);
		}
	}
	else if(func==183){
		if(damage>=pl[to].def)
			pl[to].buff[7]+=2;
	}
	else if(func==184){
		pl[to].buff[8]+=1;
	}
	else if(func==185){
		pl[to].buff[8]+=4;
		for(int i=1;i<=7;i++)
			if(pl[to].buff[i])
			{
				pl[to].buff[8]-=3;
				break;
			}
	}
	else if(func==186){
		pl[from].buff[7]+=2;
		pl[to].buff[7]+=2;
	}
	else if(func==187){
		pl[to].buff[7]+=6;
	}
	else if(func==188){
		for(int i=1;i<=pl[from].cardcnt;i++){
			if(pl[from].used[i]){
				pl[from].used[i]=0;
				pl[from].handcard[i]=lib[52][1];
			}
		}
		pl[from].rest=pl[from].cardcnt;
		return 4;
	}
	else if(func==189){
		int s1=pl[3].buff[0]%100,s2=pl[3].buff[0]/100%100,s3=pl[3].buff[0]/10000;
		s2=max(s2-2,0);
		pl[3].buff[0]=s1+s2*100+s3*10000;
	}
	else if(func==191){
		int s1=pl[3].buff[0]%100,s2=pl[3].buff[0]/100%100,s3=pl[3].buff[0]/10000;
		s2=max(s2-1,0);
		pl[3].buff[0]=s1+s2*100+s3*10000;
	}
	return 0;
}

void extra(int from,int to,int cursor){
	if(pl[from].handcard[cursor].func==37 || pl[from].handcard[cursor].func==67 || pl[from].handcard[cursor].func==3){
		pl[from].used[cursor]=0;
		pl[from].rest++;
		Card x=pl[from].heap[(rad()%pl[from].heapn)+1];
		while(x.id==pl[from].handcard[cursor].id) x=pl[from].heap[(rad()%pl[from].heapn)+1];
		pl[from].handcard[cursor]=x;
	}//[急不可耐],[血色怒火],[互惠共赢]抽牌
	if(pl[from].handcard[cursor].func==60){
		pl[from].used[cursor]=0;
		pl[from].rest++;
		pl[from].handcard[cursor]=lib[1][7];
	}//[熟练拾荒]变为[无中生有]
	if(pl[from].handcard[cursor].func==96){
		pl[from].used[cursor]=0;
		pl[from].rest++;
		pl[from].handcard[cursor]=lib[8][11];
	}//[深海滋养]变为[水中疗养]
	if(pl[from].handcard[cursor].func==104){
		pl[from].used[cursor]=0;
		pl[from].rest++;
		pl[from].handcard[cursor]=lib[9][11];
	}//[守护者的审判]变为[圣盾形态]
	if(pl[from].handcard[cursor].func==24){
		for(int i=1;i<=pl[from].cardcnt;i++) {
			pl[from].handcard[i]=pl[from].heap[rad()%pl[from].heapn+1];
			pl[from].used[i]=0;
		}
		pl[from].rest=pl[from].cardcnt;
	}//[涅槃]抽牌
	if(pl[from].handcard[cursor].func==82){
		for(int i=1;i<=pl[from].cardcnt;i++) {
			if(pl[from].used[i]) continue;
			if(pl[from].handcard[i].HEAL!=0) pl[from].handcard[i].ATK+=pl[from].handcard[i].HEAL;
			else pl[from].handcard[i].ATK+=15;
			pl[from].handcard[i].HEAL=0;
		}
	}//[暗影形态]搜索
	if(pl[from].handcard[cursor].func==44){
		int x=rad()%pl[from].cardcnt+1;//选取随机槽位
		if(pl[from].used[x]){
			pl[from].rest++;
			pl[from].used[x]=0;
		}
		pl[from].handcard[x]=lib[7][libcnt[7]+1];
	}
	if(pl[from].handcard[cursor].func==90){
		pl[from].buff[0]/=2;
	}
}

bool giveupcard(int from,int to,int cursor){
	if(pl[from].handcard[cursor].func==65) {
		pl[from].buff[4]+=2;
		return 1;
	}//[精神控制]
	if(pl[from].handcard[cursor].func==94) {
		pl[to].def+=10;
		if(pl[to].occ==9) 
			if(pl[to].def>pl[to].maxdef) 
			{
				int defdamage=(pl[to].def-pl[to].maxdef)*(10-pl[to].buff[0])/10.0;
				if(pl[from].occ!=8) pl[from].hp-=defdamage;
				else
				{
					if(defdamage<=pl[from].buff[0]*3) pl[from].buff[0]-=defdamage/3,defdamage=0;
					else defdamage-=pl[from].buff[0]*3,pl[from].buff[0]=0;
					if(defdamage>0) pl[from].hp-=defdamage;	
				}
			}
		pl[to].def=min(pl[to].maxdef,pl[to].def);
		return 1;
	}//[无力]
	if(pl[from].handcard[cursor].func==156) {
		pl[from].buff[6]+=1;
		return 1;
	}//[草木皆兵]
	return 0;
}
				
void init(int x){
	pl[x].maxhp=job[pl[x].occ].maxhp;
	if(pl[x].occ>=50) pl[x].maxhp*=(50+mode[22]*10)/100.0;
	pl[x].cardcnt=job[pl[x].occ].cardcnt;
	pl[x].maxdef=job[pl[x].occ].maxdef;
	pl[x].maxcost=job[pl[x].occ].maxcost;
	pl[x].hp=pl[x].maxhp;
	//获得职业基本信息
	int tot=libcnt[0];
	for(int i=1;i<=tot;i++){
		if(pl[x].occ==3 && lib[0][i].HEAL>=75) continue; //法师不能抽公共牌库HEAL>=75的牌
		if(pl[x].occ==4 && lib[0][i].HEAL>0) continue;//战士不能抽公共牌库HEAL牌
		if(pl[x].occ==5 || pl[x].occ==6 || pl[x].occ==8 || pl[x].occ==9 || pl[x].occ==11 || pl[x].occ==12 || pl[x].occ==13 || pl[x].occ>=50) continue;
		if(pl[x].occ==7 && lib[0][i].ATK>=80) continue;//牧师不能抽公共牌库ATK>=80的牌
		if(pl[x].occ==10 && lib[0][i].cost!=0) continue;//赌徒只能抽公共牌库费用=0的牌 
		pl[x].heap[++pl[x].heapn]=lib[0][i];
	}
	tot=libcnt[pl[x].occ];
	for(int i=1;i<=tot;i++) {
		if(lib[pl[x].occ][i]==lib[13][3] and DestructiveStrike)
			pl[x].heap[++pl[x].heapn]=lib[51][5];
		else if(lib[pl[x].occ][i]==lib[10][13] and mode[6]==1 and mode[23]==0)
			continue;
		else
			pl[x].heap[++pl[x].heapn]=lib[pl[x].occ][i];
	}
	if(mode[4]){//欢乐模式：随机buff
		tot=funcnt[2][0];
		for(int i=1;i<=tot;i++) pl[x].heap[++pl[x].heapn]=fun[2][0][i];
	}
	if(mode[6]==1 and x!=3){
		for(int i=1;i<=mode[24];i++) pl[x].heap[++pl[x].heapn]=fun[3][0][1];
	}
}

void treasure(int from,int to){//竞技模式：宝藏牌
	if(gettre[from]) return;
	if(pl[from].occ==1 && pl[from].buff[0]>=3)
		for(int i=1;i<=pl[from].cardcnt;i++){
			if(pl[from].used[i]) {
				pl[from].used[i]=0;
				gettre[from]=1;
				pl[from].handcard[i]=fun[1][1][1];
				break;
			}
		}
	if(pl[from].occ==2 && pl[from].maxhp<=500)
		for(int i=1;i<=pl[from].cardcnt;i++){
			if(pl[from].used[i]) {
				pl[from].used[i]=0;
				gettre[from]=1;
				pl[from].handcard[i]=fun[1][2][1];
				break;
			}
		}
	if(pl[from].occ==3 && pl[to].hp*10<pl[to].maxhp*6)
		for(int i=1;i<=pl[from].cardcnt;i++){
			if(pl[from].used[i]) {
				pl[from].used[i]=0;
				gettre[from]=1;
				pl[from].handcard[i]=fun[1][3][1];
				break;
			}
		}
	if(pl[from].occ==4 && pl[from].buff[0]>=6)
		for(int i=1;i<=pl[from].cardcnt;i++){
			if(pl[from].used[i]) {
				pl[from].used[i]=0;
				gettre[from]=1;
				pl[from].handcard[i]=fun[1][4][1];
				break;
			}
		}
	if(pl[from].occ==5 && pl[from].maxhp>=350)
		for(int i=1;i<=pl[from].cardcnt;i++){
			if(pl[from].used[i]) {
				pl[from].used[i]=0;
				gettre[from]=1;
				pl[from].handcard[i]=fun[1][5][1];
				break;
			}
		}
	if(pl[from].occ==6 && pl[from].buff[0]>=5)
		for(int i=1;i<=pl[from].cardcnt;i++){
			if(pl[from].used[i]) {
				pl[from].used[i]=0;
				gettre[from]=1;
				pl[from].handcard[i]=fun[1][6][1];
				break;
			}
		}
	if(pl[from].occ==7 && pl[from].buff[0]>=4 && pl[from].cost>=3)
		for(int i=1;i<=pl[from].cardcnt;i++){
			if(pl[from].used[i]) {
				pl[from].used[i]=0;
				gettre[from]=1;
				pl[from].handcard[i]=fun[1][7][1];
				break;
			}
		}
	if(pl[from].occ==8 && (pl[from].hp<=120 || pl[from].buff[0]>=24))
		for(int i=1;i<=pl[from].cardcnt;i++){
			if(pl[from].used[i]) {
				pl[from].used[i]=0;
				gettre[from]=1;
				pl[from].handcard[i]=fun[1][8][1];
				break;
			}
		}
	if(pl[from].occ==9 && pl[from].maxdef>=300)
		for(int i=1;i<=pl[from].cardcnt;i++){
			if(pl[from].used[i]) {
				pl[from].used[i]=0;
				gettre[from]=1;
				pl[from].handcard[i]=fun[1][9][1];
				break;
			}
		}
	if(pl[from].occ==10 && pl[from].hp<=200)
		for(int i=1;i<=pl[from].cardcnt;i++){
			if(pl[from].used[i]) {
				pl[from].used[i]=0;
				gettre[from]=1;
				pl[from].handcard[i]=fun[1][10][1];
				break;
			}
		}
	if(pl[from].occ==11 && pl[from].buff[0]>=15)
		for(int i=1;i<=pl[from].cardcnt;i++){
			if(pl[from].used[i]) {
				pl[from].used[i]=0;
				gettre[from]=1;
				pl[from].handcard[i]=fun[1][11][1];
				break;
			}
		}
	if(pl[from].occ==12 && pl[from].hp<=100)
		for(int i=1;i<=pl[from].cardcnt;i++){
			if(pl[from].used[i]) {
				pl[from].used[i]=0;
				gettre[from]=1;
				pl[from].handcard[i]=fun[1][12][1];
				break;
			}
		}
	if(pl[from].occ==13 && pl[from].buff[0]/1000%1000>=80)
		for(int i=1;i<=pl[from].cardcnt;i++){
			if(pl[from].used[i]) {
				pl[from].used[i]=0;
				gettre[from]=1;
				pl[from].handcard[i]=fun[1][13][1];
				break;
			}
		}
}

void adv(int x){
	if(mode[6]==1) return;
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
		pl[x].def+=25;
		pl[x].maxdef+=25;
	}
	else if(pl[x].occ==7){
		pl[x].buff[0]+=1;
	}
	else if(pl[x].occ==8){
		pl[x].buff[0]=15;
	}
	else if(pl[x].occ==9){
		pl[x].def=40;
	}
	else if(pl[x].occ==10){
		pl[x].def=50;
	}
	else if(pl[x].occ==11){
		pl[x].buff[0]=3;
	}
	else if(pl[x].occ==12){
		pl[x].buff[0]=1;
		if(rad()%100<40) pl[x].cost=min(pl[x].cost+1,pl[x].maxcost);
	}
	else if(pl[x].occ==13){
		pl[x].def=40;
	}
	return;
}

void Choose(int now){
	SetColor(7);
	int cursor=1,lastcursor=1;
	if(pl[3].occ)
	{
		SetPos(6,2);
		printf("BOSS：");printf(occ_name(pl[3].occ));
	}
	SetPos(6,4);
	printf("◇选择P%d",now);printf("的职业◇");
	for(int i=1;i<=sumjob+1;i++){
		SetColor(7,0);SetPos(4,i+5);printf("%2d",i);printf(string(8-occ_name(i).size()/2,' '));printf(occ_name(i));printf(string(5-occ_name(i).size()/2,' '));
	}//各职业名字和简介展示
	while(1)
	{
		SetPos(9,lastcursor+5);SetColor(7,0);printf(string(5-occ_name(lastcursor).size()/2,' '));printf(occ_name(lastcursor));printf(string(5-occ_name(lastcursor).size()/2,' '));		
		SetPos(9,cursor+5);SetColor(0,7);printf(string(5-occ_name(cursor).size()/2,' '));printf(occ_name(cursor));printf(string(5-occ_name(cursor).size()/2,' '));	
		occ_func(cursor);
		
		if(mode[3]) SetPos(26,16),occ_treasure(cursor);
		SetPos(26,18);occ_intro(cursor);
		
		lastcursor=cursor;
		input=getch();
		if(checkup(input))
			cursor--;
		if(checkdown(input))
			cursor++;
		if(cursor>sumjob+1) cursor=1;
		if(cursor<1) cursor=sumjob+1;
		if(checkenter(input)){
			if(cursor>sumjob){
				pl[now].occ=rad()%sumjob+1;
				return;
			}
			else {
				pl[now].occ=cursor;
				return;
			}
		}
	}
}

void Choose_boss(){
	cls();
	SetPos(5,4),printf("BOSS战模式注意事项：");
	SetPos(5,5),printf("1.BOSS每回合额外+1◆");
	SetPos(5,6),printf("2.战斗中可以按c键切换出牌的对象");
	SetPos(5,7),printf("3.牧师无法通过[神圣意志]获得胜利，但可以用它治疗队友");
	
	SetPos(5,9),printf("P1按任意键跳过");
	
	if(server_mode==1)
	{
		getch();
		send_int(8255);
	}
	else
	{
		while(1)
		{
			int recv_val=recv_message();
			if(recv_val==8255) break;
		}
	}
	
	int tysta_open=int(Tysta or rad()%100==0);
	if(server_mode==1) send_int(tysta_open);
	else recv_int(tysta_open);
	
	cls();
	SetColor(7);
	int cursor=1,lastcursor=1;
	SetPos(6,4);
	printf("◇选择挑战的BOSS◇");
	for(int i=51;i<=sumboss+51;i++){
		SetColor(7,0);SetPos(4,i-50+5);printf("%2d",i-50);printf(string(8-occ_name(i).size()/2,' '));printf(occ_name(i));printf(string(5-occ_name(i).size()/2,' '));
	}
	if(tysta_open){
		SetColor(7,0);SetPos(9,18);printf(" 特娅斯塔 ");
	}
	while(1)
	{
		int cho=50+cursor%(sumboss+2);
		if(lastcursor==sumboss+2){
			SetColor(7,0);SetPos(9,18);printf(" 特娅斯塔 ");
		} 
		else{
			SetPos(9,lastcursor+5);SetColor(7,0);printf(string(5-occ_name(50+lastcursor).size()/2,' '));printf(occ_name(50+lastcursor));printf(string(5-occ_name(50+lastcursor).size()/2,' '));		
		}
		if(cursor==sumboss+2){
			SetColor(0,7);SetPos(9,18);printf(" 特娅斯塔 ");
		}
		else{
			SetPos(9,cursor+5);SetColor(0,7);printf(string(5-occ_name(cho).size()/2,' '));printf(occ_name(cho));printf(string(5-occ_name(cho).size()/2,' '));	
		}
		
		occ_func(cho);
		
		SetPos(26,18);occ_intro(cho);
		
		lastcursor=cursor;
		
		if(server_mode==1){
			input=getch();
			if(checkup(input)) cursor--;
			if(checkdown(input)) cursor++;
			if(cursor>sumboss+1+tysta_open) cursor=1;
			if(cursor<1) cursor=sumboss+1+tysta_open;
			if(checkenter(input)){
				if(cursor==sumboss+1) pl[3].occ=rad()%sumboss+51;
				else pl[3].occ=cho;
				send_int(8255);break; 
			}
			send_int(cursor);
		}
		else{
			int recv_val;recv_int(recv_val);
			if(recv_val/1000==0) cursor=recv_val;
			if(recv_val==8255) break;
		}
	}
	
	if(server_mode==1) send_int(pl[3].occ);
	else recv_int(pl[3].occ);
}

int UI(){
	if(mode[6]==1 and pl[3].occ==51)
		for(int i=1;i<=int(pl[3].buff[0]/2.5);i++)
		{
			SetPos(rad()%(cols-1),rad()%(lines-1));
			string qwq="!@#$%^&*(){}:\"<>,./;'[]|\\?~`";
			if(rad()%2==0) SetColor(4);
			else SetColor(12);
			putchar(qwq[rad()%int(qwq.size())]);
		}
		
	if(server_mode!=3) Con::show();
	for(int rnk=1;rnk<=2+(mode[6]==1);rnk++){
		int i;
		//编号/手牌/职业
		SetColor(15);
		SetPos(5,rnk*4-2);
		printf("#%d ",rnk);
		SetColor(7); 
		printf("%d/%d",pl[rnk].rest,pl[rnk].cardcnt); 
		SetColor(7);
		SetPos(5,rnk*4+1-2);
		printf("   ");
		if(pl[rnk].occ==13 and pl[rnk].buff[0]/1000000) SetColor(4);
		printf(occ_name(pl[rnk].occ));
		
		//buff0
		if(pl[rnk].occ>=2 and pl[rnk].occ!=50){
			SetColor(13);
			SetPos(8,rnk*4);
			if(pl[rnk].occ!=10 and pl[rnk].occ!=13 and pl[rnk].occ!=52) printf("★%d   ",pl[rnk].buff[0]);
			if(pl[rnk].occ==10)
			{
				if(pl[rnk].buff[0]<10) printf("★0%d   ",pl[rnk].buff[0]);
				else 
				{
					if(pl[rnk].buff[0]==92 and !(mode[6]==1 and mode[23]==0)) SetColor(14);
					printf("★%d   ",pl[rnk].buff[0]);
				}
			}
			if(pl[rnk].occ==13)
			{
				printf("★       ");SetPos(8,rnk*4);
				printf("★%d/%d",pl[rnk].buff[0]%1000,pl[rnk].buff[0]/1000%1000);
			}
			if(pl[rnk].occ==52)
			{
				printf("★       ");SetPos(8,rnk*4);
				if(pl[rnk].hp<pl[rnk].maxhp/2 and !DimlyCheck)
				{
					SetColor(12);printf("★[面具]");
				}
				else
				{
					if(pl[rnk].buff[0]%100==0) SetColor(14);
					printf("★%d/%d",pl[rnk].buff[0]%100,pl[rnk].buff[0]/100%100);	
				}
				
			}
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
		SetPos(26,rnk*4);printf(string(54,' '));
		SetPos(26,rnk*4+1);printf(string(54,' '));
		SetPos(26,rnk*4);
		int qwqcnt=0;
		if(pl[rnk].occ==13 and pl[rnk].buff[0]/1000000)
			SetColor(4),printf("勇者特性 %d : ",pl[rnk].buff[0]/1000000),qwqcnt++;
		if(pl[rnk].buff[1])
			SetColor(6),printf("燃烧 %d : ",pl[rnk].buff[1]),qwqcnt++;
		if(pl[rnk].buff[2])
			SetColor(2),printf("中毒 %d : ",pl[rnk].buff[2]),qwqcnt++;
		if(pl[rnk].buff[3])
			SetColor(4),printf("狂暴 %d : ",pl[rnk].buff[3]),qwqcnt++;
		if(qwqcnt==4) SetPos(26,rnk*4+1);
		if(pl[rnk].buff[4])
			SetColor(11),printf("虚弱 %d : ",pl[rnk].buff[4]),qwqcnt++;
		if(qwqcnt==4) SetPos(26,rnk*4+1);
		if(pl[rnk].buff[5])
			SetColor(10),printf("治疗 %d : ",pl[rnk].buff[5]),qwqcnt++;
		if(qwqcnt==4) SetPos(26,rnk*4+1);
		if(pl[rnk].buff[6])
			SetColor(5),printf("迷惑 %d : ",pl[rnk].buff[6]),qwqcnt++;
		if(qwqcnt==4) SetPos(26,rnk*4+1);
		if(pl[rnk].buff[7])
			SetColor(12),printf("易伤 %d : ",pl[rnk].buff[7]),qwqcnt++;
		if(qwqcnt==4) SetPos(26,rnk*4+1);
		if(pl[rnk].buff[8])
			SetColor(9),printf("诅咒 %d : ",pl[rnk].buff[8]),qwqcnt++;
				
		//血条绘制
		int basehp=40,basedef=30;
		if(pl[rnk].occ==50) basehp*=2.3,basedef*=1.2;
		if(pl[rnk].occ==51) basehp*=1.9;
		if(pl[rnk].occ==52) basehp*=1.8;
		if(pl[rnk].occ>=50) basehp*=(50+mode[22]*10)/100.0;
		
		SetPos(26,rnk*4-1);
		if(pl[rnk].hprate()>5)SetColor(10);
		else if(pl[rnk].hprate()>2)SetColor(6);
		else SetColor(12);
		i=1;
		if(pl[rnk].hp>0) for(;i<=(pl[rnk].hp-1)/basehp+1;i++)
			printf("■");
		SetColor(8);
		for(;i<=(pl[rnk].prehp-1)/basehp+1;i++)
			printf("■");
		if(pl[rnk].hprate()>5)SetColor(10);
		else if(pl[rnk].hprate()>2)SetColor(6);
		else SetColor(12);
		for(;i<=(pl[rnk].maxhp-1)/basehp+1;i++)
			printf("□");
		SetColor(7);
		if(pl[rnk].def) for(i=1;i<=(pl[rnk].def-1)/basedef+1;i++)
			printf("■");
		for(i=1;i<=36-(pl[rnk].maxhp-1)/basehp-(pl[rnk].def-1)/basedef-2;i++)
		printf(" ");
	}

	if(mode[6]==1)
	{
		SetColor(11);
		for(int i=0;i<=2;i++)
			SetPos(3,(6-server_mode-chooseto)*4-i),printf(" ");
		for(int i=0;i<=2;i++)
			SetPos(3,chooseto*4-i),printf(">");
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
	SetColor(7);
	
	return 0; 
}

void UI_other(int to){
	SetColor(7);
	SetPos(0,Row-1);
	printf("P%d",now); 
	SetPos(5,Row-1);
	printf("  剩余手牌:%d/%d",pl[to].rest,pl[to].cardcnt); 
	if(pl[to].occ==7 and mode[6]==0){
		SetColor(6);
		SetPos(30,Row-1);
		printf("  [神圣意志]:%d/%d",pl[to].buff[10],pl[to].cardcnt); 
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
	if(mode[6]==1)
	{
		SetPos(58+14,Row);
		printf("TO");
	}
	while(appcnt){
		if(appcard[appcnt].func==-2) appcnt--;
		else break;
	}
	for(int i=1;i<=min(appcnt,6);i++) {
		int x=i+max(0,appcnt-6);
		if(i==min(appcnt,6)) {
			SetColor(11,0);
			SetPos(11,Row+i);
			printf("%d",appcard[x].cost);//费用
			SetColor(0,8);
			SetPos(14,Row+i);
			printf(appcard[x].Name());//牌名
//			SetColor(0,8);
			printf("                 ");
			SetPos(20+14,Row+i);
			printf("%-3d         ",appcard[x].ATK);
			SetPos(30+14,Row+i);
			printf("%-3d         ",appcard[x].HEAL);
			SetPos(40+14,Row+i);
			printf("%-3d         ",appcard[x].DEF);
			SetPos(50+14,Row+i);
			if(appcard[x].MISS%10==1) {
				SetColor(10,8);
				printf("YES ");//MISS
			}
			else if(appcard[x].MISS%10==2) {
				SetColor(13,8);
				printf("DIS ");//give up
			}
			else if(appcard[x].MISS%10==3) {
				SetColor(13,8);
				printf("INV ");//invalid
			}
			else {
				SetColor(4,8);
				printf("NO  ");
			}
			if(mode[6]==1) SetColor(0,8),printf("    P%d ",appcard[x].MISS/10);	
			SetColor(7,0);
			SetPos(11,Row+9);
			printf("                                                                    ");
			SetPos(11,Row+9);
			printf(appcard[x].Intro());
		}
		else {
			SetColor(11);
			SetPos(11,Row+i);
			printf("%d",appcard[x].cost);//费用
			SetColor(7);
			SetPos(14,Row+i);
			printf(appcard[x].Name());//牌名
			SetColor(7,0);
			printf("               ");
			SetPos(20+14,Row+i);
			printf("%-3d         ",appcard[x].ATK);
			SetPos(30+14,Row+i);
			printf("%-3d         ",appcard[x].HEAL);
			SetPos(40+14,Row+i);
			printf("%-3d         ",appcard[x].DEF);
			SetPos(50+14,Row+i);
			if(appcard[x].MISS%10==1) {
				SetColor(10,0);
				printf("YES ");//MISS
			}
			else if(appcard[x].MISS%10==2) {
				SetColor(13,0);
				printf("DIS ");//give up
			}
			else if(appcard[x].MISS%10==3) {
				SetColor(13,0);
				printf("INV ");//invalid
			}
			else {
				SetColor(4,0);
				printf("NO  ");
			}
			if(mode[6]==1) SetColor(7,0),printf("    P%d ",appcard[x].MISS/10);	
			SetColor(7,0);
			SetPos(11,Row+9);
			printf("                                                                    ");
			SetPos(11,Row+9);
			printf(appcard[x].Intro());
		}
	}
	mouse(0);
}

void start_turn(int from,int to){
	for(int i=1;i<=2;i++) pl[i].prehp=pl[i].hp;//记录回合开始血量
	//职业特性
	if(pl[from].occ==1) {
		if(pl[from].cost==0){
			if(rad()%100<80) {
				pl[from].cost++;//浪人[拾荒]
				pl[from].buff[0]++;
			}
		}
	}
	if(pl[from].occ==3){
		pl[from].hp=min(pl[from].maxhp,pl[from].hp+8*pl[from].buff[0]);
		pl[from].cost=min(pl[from].maxcost,pl[from].cost+pl[from].buff[0]);
		pl[from].buff[0]=0;
		if(rad()%100<45) pl[from].buff[0]++;
	}//法师<★法力>标记获得费用
	if(pl[from].occ==4) {
		if(pl[from].def==0) pl[from].def=20;
		pl[from].buff[0]=max(0,pl[from].buff[0]-1);
	}//战士叠甲并清除<★疲惫>
	if(pl[from].occ==5){
		pl[from].cost=min(pl[from].cost+2,pl[from].maxcost);//地精每回合3费
	}
	if(pl[from].occ==6){
		pl[from].cost=min(pl[from].cost+1,pl[from].maxcost);
		int da=5*pl[from].buff[0];
		if(da<pl[from].def) pl[from].def-=da;
		else {
			if(pl[from].def>0) Shake(3,1);
			pl[from].hp-=da-pl[from].def,pl[from].def=0;
		}
	}//恶魔<★原罪>让自己受伤
	if(pl[from].occ==8){
		pl[from].hp-=15;
		pl[from].buff[0]+=10;
	}//鱼人扣15血获得10<★鱼仔>
	if(pl[from].occ==10){
		pl[from].hp=max(pl[from].hp,pl[from].buff[0]);
	}//赌徒HP回复至<★局面> 
	if(pl[from].occ==11){
		pl[from].hp=min(pl[from].maxhp,pl[from].hp+min(15,pl[from].buff[0])); 
	}//剑客回复<★剑气>HP
	if(pl[3].occ==51){
		for(int i=1;i<=3;i++) pl[i].hp-=pl[3].buff[0];
	}//破坏神<★毁灭>效果
	if(pl[from].occ>=50){
		pl[from].cost=min(pl[from].cost+1,pl[from].maxcost);
	}//BOSS每回合2费
	if(pl[3].occ==52 and from!=3){
		int s1=pl[3].buff[0]%100,s2=pl[3].buff[0]/100%100,s3=pl[3].buff[0]/10000;
		s3=rad()%pl[from].cardcnt+1;
		pl[3].buff[0]=s1+s2*100+s3*10000;
	}
	
	if(env_now==7)
		pl[from].cost=min(pl[from].cost+1,pl[from].maxcost);//环境加费 
	
	pl[from].cost=min(pl[from].cost+1,pl[from].maxcost);//加费 
	if(pl[from].buff[5])//治疗buff
		pl[from].hp=min(pl[from].hp+30,pl[from].maxhp);
	if(pl[from].buff[1])//燃烧buff
		pl[from].hp-=40;
	if(pl[from].buff[2])//中毒buff
		if(pl[from].hp>0)
		{
			if(pl[from].occ>=50) 
				pl[from].hp-=pl[from].hp*((10+mode[21]*5)/100.0);
			else 
				pl[from].hp-=pl[from].hp*0.2;
		}
			
	pl[from].UpdateBuff(1);
	//buff前判

	if(mode[4]){
		pl[from].buff[rad()%8+1]++;
	}//娱乐模式：随机buff

	if(env_now==5){
		pl[from].hp=min(pl[from].maxhp,(int)(pl[from].hp+pl[from].maxhp*0.05));
	}//彩虹
	if(env_now==6){
		pl[from].hp=(pl[from].hp-pl[from].maxhp*0.05);
	}//酸雨
	//环境判断
	
	if(mode[3]) treasure(from,to);
	for(int i=1;i<=pl[from].cardcnt;i++){
		if(pl[from].used[i]){
			pl[from].used[i]=0;
			if(pl[from].occ==7 && rad()%100<pl[from].buff[0]*2){
				pl[from].handcard[i]=lib[7][libcnt[7]+1];
			}
			else{
				pl[from].handcard[i]=pl[from].heap[(rad()%pl[from].heapn)+1];
			}
		}
	}
	pl[from].rest=pl[from].cardcnt;
	if(pl[from].occ==7){
		pl[from].buff[10]=0;
		for(int i=1;i<=pl[from].cardcnt;i++) 
			if(!pl[from].used[i] && pl[from].handcard[i].func==42) pl[from].buff[10]++; //42-->[神圣意志]
	}//牧师检索[神圣意志]
	
	for(int i=1;i<=2;i++) Check(i);
	if(mode[6]==1) Check(3);
}

void end_turn(int from,int to){
	if(pl[from].occ==7){
		if(pl[from].cost==0) pl[from].buff[0]++;
		pl[from].buff[10]=0;
		for(int i=1;i<=pl[from].cardcnt;i++) 
			if(!pl[from].used[i] && pl[from].handcard[i].func==42) pl[from].buff[10]++; //42-->[神圣意志]
		if(pl[from].buff[10]==pl[from].cardcnt)  winner=from;
	}//牧师检索[神圣意志]
	if(pl[from].occ==8){
		for(int i=1;i<=pl[from].cardcnt;i++){
			if(pl[from].used[i]) continue;
			if(pl[from].handcard[i].func==92) pl[from].handcard[i].ATK+=min(pl[from].buff[0],50);
		}
		int damage=pl[from].buff[0];
		if(pl[to].occ==9)
		{
			int defdamage=min(damage,pl[to].def);
			damage-=defdamage,pl[to].def-=defdamage;
			if(damage) pl[to].hp-=damage;
			defdamage=defdamage*pl[to].buff[0]/10;
			if(defdamage<=pl[from].buff[0]*3) pl[from].buff[0]-=defdamage/3,defdamage=0;
			else defdamage-=pl[from].buff[0]*3,pl[from].buff[0]=0;
			if(defdamage>0) pl[from].hp-=defdamage;
		}
		else
		{
			if(damage>=pl[to].def) {
				damage-=pl[to].def,pl[to].def=0,pl[to].hp-=damage;
			}
			else pl[to].def-=damage;	
		}
		//[集群攻击]
		Check(to);
	}
	for(int i=1;i<=pl[from].cardcnt;i++)
		if(pl[from].used[i]==0)
		{
			if(pl[from].handcard[i].func==169)
			{
				pl[from].used[i]=1;
				pl[from].rest--;
			}
			if(pl[from].handcard[i].func==168 and pl[from].buff[0]%1000==0 and rad()%100<30)
				pl[from].handcard[i]=lib[13][libcnt[13]+1];
		} 
		
	if(pl[from].occ==13)
	{
		if(pl[from].buff[0]%1000==0) pl[from].hp=min(pl[from].hp+15,pl[from].maxhp);
		pl[from].buff[0]=pl[from].buff[0]/1000*1000;
	}
	int brave=pl[from].buff[0]/1000000; 
	
	if(pl[from].buff[8]==1)
	{
		pl[from].hp-=150;
		system("color 97");
		Shake(10,1);
		system("color 07");
		cls();
		UI();
	}
		
	pl[from].UpdateBuff(2);
	
	if(pl[from].occ==13 and brave==1 and pl[from].hp<=0)
	{
		if(mode[6]==0) winner=to;
		else
		{
			death[from]=1;
			if(death[1] and death[2]) winner=3;
		}
	}
	
	//环境变动
	if(mode[2]){
		if(env_cnt>=2){
			env_rate+=25;
			if(rad()%100 < env_rate){
				int _p=0;
				while((_p=rad()%env_num)==env_now);
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

void another_player_quit(int server_mode){
	cls();
	SetPos(11,0);
	if(mode[6]==0) printf("对方退出了游戏，他也许是投降了...\n"),winner=server_mode;
	if(mode[6]==1) printf("队友退出了游戏，你们最终失败了...\n"),winner=3; 
	Sleep(1500);
}

void checkcard(int from,int to,int i){
	if(pl[from].handcard[i].func==9) {
		pl[from].handcard[i].HEAL=pl[from].maxhp*0.2;
	}
	if(pl[from].handcard[i].func==15) {
		pl[from].handcard[i].ATK=pl[from].cost*40+25;
		pl[from].handcard[i].cost=pl[from].cost;
	}
	if(pl[from].handcard[i].func==32){
		int da=0;
		for(int i=1;i<=pl[from].cardcnt;i++){
			if(pl[from].used[i]) continue;
			if(pl[from].handcard[i].id==98) da+=60; //98-->[虚空垃圾]
		}
		pl[from].handcard[i].ATK=da;
	}
	if(pl[from].handcard[i].func==89){
		pl[from].handcard[i].ATK=pl[from].buff[0]*1.5;
	}
	if(pl[from].handcard[i].func==90){
		pl[from].handcard[i].HEAL=pl[from].buff[0]*2;
	}
	if(pl[from].handcard[i].func==102){
		pl[from].handcard[i].ATK=pl[from].def*1.75;
	}
	if(pl[from].handcard[i].func==110){
		if(pl[from].buff[0]%10==6 or pl[from].buff[0]/10==6) pl[from].handcard[i].MISS=0;
		else pl[from].handcard[i].MISS=100;
	}
	if(pl[from].handcard[i].func==111){
		if(pl[from].buff[0]%10==2 or pl[from].buff[0]/10==2) pl[from].handcard[i].MISS=20;
		else if(pl[from].buff[0]%10==3 or pl[from].buff[0]/10==3) pl[from].handcard[i].MISS=20;
		else if(pl[from].buff[0]%10==5 or pl[from].buff[0]/10==5) pl[from].handcard[i].MISS=20;
		else if(pl[from].buff[0]%10==7 or pl[from].buff[0]/10==7) pl[from].handcard[i].MISS=20;
		else pl[from].handcard[i].MISS=100;
	}
	if(pl[from].handcard[i].func==112){
		if(pl[from].buff[0]%10==1 or pl[from].buff[0]/10==1) pl[from].handcard[i].MISS=20;
		else pl[from].handcard[i].MISS=100;
	}
	if(pl[from].handcard[i].func==113){
		if(pl[from].buff[0]%10==1 or pl[from].buff[0]/10==1) pl[from].handcard[i].MISS=20;
		else if(pl[from].buff[0]%10==3 or pl[from].buff[0]/10==3) pl[from].handcard[i].MISS=20;
		else if(pl[from].buff[0]%10==5 or pl[from].buff[0]/10==5) pl[from].handcard[i].MISS=20;
		else if(pl[from].buff[0]%10==7 or pl[from].buff[0]/10==7) pl[from].handcard[i].MISS=20;
		else if(pl[from].buff[0]%10==9 or pl[from].buff[0]/10==9) pl[from].handcard[i].MISS=20;
		else pl[from].handcard[i].MISS=100;
	}
	if(pl[from].handcard[i].func==114){
		if(pl[from].buff[0]%10==8 or pl[from].buff[0]/10==8) pl[from].handcard[i].MISS=0;
		else pl[from].handcard[i].MISS=40;
	}
	if(pl[from].handcard[i].func==118){
		pl[from].handcard[i].HEAL=pl[from].buff[0]*3;
	}
	if(pl[from].handcard[i].func==121){
		if(pl[from].buff[0]==92) pl[from].handcard[i].MISS=0;
		else pl[from].handcard[i].MISS=100;
	}
	if(pl[from].handcard[i].func==122){
		if(pl[from].buff[0]%10==5 or pl[from].buff[0]/10==5) pl[from].handcard[i].MISS=0;
		else pl[from].handcard[i].MISS=100;
	}
	if(pl[from].handcard[i].func==124){
		pl[from].handcard[i].ATK=pl[from].buff[0];
	}
	if(pl[from].handcard[i].func==127){
		pl[from].handcard[i].cost=pl[from].cost;
	}
	if(pl[from].handcard[i].func==128){
		pl[from].handcard[i].ATK=pl[to].buff[4]*35;
	}
	if(pl[from].handcard[i].func==130){
		pl[from].handcard[i].ATK=pl[to].buff[4]*15+50;
	}
	if(pl[from].handcard[i].func==134){
		pl[from].handcard[i].DEF=pl[from].buff[0]*4;
	}
	if(pl[from].handcard[i].func==138){
		pl[from].handcard[i].HEAL=pl[from].buff[0]*5;
	}
	if(pl[from].handcard[i].func==145){
		pl[from].handcard[i].ATK=(pl[to].cardcnt-pl[to].rest)*45;
	} 
	if(pl[from].handcard[i].func==187){
		if(pl[from].buff[0]%100==0) pl[from].handcard[i].MISS=0;
		else pl[from].handcard[i].MISS=100;
	}
	if(pl[from].handcard[i].func==190){
		pl[from].handcard[i].ATK=pl[from].handcard[i].HEAL=(pl[from].buff[0]%100)*15; 
	} 
} 
void showcard(int from,int to,int cursor,int i,int option_giveup,int option_use){
	int color=0;
	SetPos(5,Row+i);
	if(pl[from].used[i])SetColor(8);
	if(cursor==i && !option_giveup && !option_use) SetColor(color=15),printf("◎"); 
	else if(cursor==i && option_giveup) SetColor(color=15),printf("×"); 
	else if(cursor==i && option_use) SetColor(color=15),printf("●"); 
	else SetColor(color=7),printf("  "); 
	if(pl[from].occ==10 and pl[from].handcard[i].func==121 and pl[from].buff[0]==92) color--;
	if(pl[3].occ==52 and i==pl[3].buff[0]/10000)
	{
		SetPos(7,Row+i),printf("%d",i); 
		color=4;if(cursor==i) color=12;SetColor(color);
		printf("                                                                     ");
		SetPos(14,Row+i),printf("[心灵枷锁]");
		return;
	}
	SetPos(7,Row+i),printf("%d",i); 
	if(pl[from].used[i]){
		SetColor(8);
		printf("                                                                     ");
		SetPos(14,Row+i);
		printf("[Used.]");
		return;
	}

	SetPos(11,Row+i);
	if(pl[from].cost>=pl[from].handcard[i].cost)SetColor(11);
	else SetColor(3);//费用是否足够
	printf("%d",pl[from].handcard[i].cost);
	SetColor(color);
	SetPos(14,Row+i);
	printf(pl[from].handcard[i].Name());//牌名
	SetColor(7,0);
	printf("               ");
	SetPos(20+14,Row+i);
	printf("%-3d",pl[from].handcard[i].ATK);
	int atk=pl[from].handcard[i].cal_atk(from,to);
	if(pl[from].handcard[i].ATK!=atk){
		if(pl[from].handcard[i].ATK < atk)SetColor(12);
		else SetColor(8);
		printf("(%d)   ",atk);
		SetColor(7);
	}
	else{
		printf("           ");
	}//ATK
	printf("           ");
	SetPos(30+14,Row+i);
	printf("%-3d",pl[from].handcard[i].HEAL);
	int heal=pl[from].handcard[i].cal_heal(from,to);
	if(pl[from].handcard[i].HEAL!=heal){
		if(pl[from].handcard[i].HEAL < heal)SetColor(10);
		else SetColor(8);
		printf("(%d)   ",heal);
		SetColor(7);
	}
	else{
		printf("            ");
	}//HEAL
	printf("           ");
	SetPos(40+14,Row+i);
	printf("%-3d",pl[from].handcard[i].DEF);
	int def=pl[from].handcard[i].cal_def(from,to);
	if(pl[from].handcard[i].DEF!=def){
		if(pl[from].handcard[i].DEF < def)SetColor(15);
		else SetColor(8);
		printf("(%d)   ",def);
		SetColor(7);
	}
	else{
		printf("         ");
	}//DEF
	printf("           ");
	SetPos(50+14,Row+i);
	printf("%-3d%%",pl[from].handcard[i].MISS);//MISS
	int miss=pl[from].handcard[i].cal_miss(from,to);
	if(pl[from].handcard[i].MISS!=miss && pl[from].handcard[i].id!=119){
		SetColor(8);
		printf("(%d%%)  ",pl[from].handcard[i].cal_miss(from,to));
		SetColor(7);
	}
	else{
		printf("      ");
	}
}

int Ask(int from,int to){
	int option_use=0,option_giveup=0,option_over=0,option_quit=0;
	start_turn(from,to);
	if(mode[6]==0) chooseto=3-server_mode;
	if(chooseto<3 and death[chooseto] and mode[6]==1) chooseto=3;
	UI();
	if(Check(from) and mode[6]==0){send_gaming(void_card); return 1;}
	if(death[from] and mode[6]==1) return 0;
	
	use_card=(Card){0,0,0,0,0,-2,0};
	if(send_gaming(use_card)<0) {
		another_player_quit(server_mode);
		return 1;
	}
	int cursor=1;
	SetColor(7);
	to=chooseto;
	while(!winner){
		SetPos(0,Row-1);SetColor(7);printf("                                                                   "); 
		SetPos(0,Row);printf("P%d",from);
		SetPos(5,Row);printf("  #"); 
		SetPos(11,Row);SetColor(11);printf("◆");
		SetPos(20+14,Row);SetColor(7);printf("ATK");
		SetPos(30+14,Row);printf("HEAL");
		SetPos(40+14,Row);printf("DEF");
		SetPos(50+14,Row);printf("MISS");
		
		for(int i=1;i<=pl[from].cardcnt;i++){
			checkcard(from,to,i);
			showcard(from,to,cursor,i,option_giveup,option_use);
		}//显示手牌
		mouse(0);
		SetColor(7);
		SetPos(11,Row+9);
		printf("                                                                    ");
		SetPos(11,Row+9);
		if(!(pl[3].occ==52 and pl[3].buff[0]/10000==cursor))
			printf(pl[from].handcard[cursor].Intro());
		input=getch();
		if(checkq(input) and mode[6]==0){
			SetPos(0,1);
			printf("                ");
			SetPos(11,Row+12);
			option_use=option_over=option_giveup=0;
			printf("                ");
			if(!option_quit){
				option_quit=1;
				SetPos(11,Row+12);
				printf("确定要认输吗？          ");
			}
			else if(option_quit==1){
				option_quit=2;
				SetPos(11,Row+12);
				printf("真的确定要认输吗？      ");
			}
			else{
				pl[from].hp=0;
				UI();
				SetPos(11,Row+12);
				printf("                       ");
				winner=to;
				if(send_gaming(void_card)<0) {
					another_player_quit(server_mode); 
					return 1;
				}
				break;
			}
		}
		if(input>='1' && input<='9' ){
			if(!pl[from].used[input-'0'] && input-'0'<=pl[from].cardcnt) cursor=input-'0';
		}
		if(checkup(input) or checkleft(input)){
			option_use=option_giveup=option_over=option_quit=0;
			cursor--;
			if(cursor<=0) cursor=pl[from].cardcnt;
			while(pl[from].used[cursor]&&cursor>0){
				cursor--;
				if(cursor<=0) cursor=pl[from].cardcnt;
			}
		}
		if(checkdown(input) or checkright(input)){
			option_use=option_giveup=option_over=option_quit=0;
			cursor++;
			if(cursor>pl[from].cardcnt) cursor=1;
			while(pl[from].used[cursor]&&cursor<=pl[from].cardcnt){
				cursor++;
				if(cursor>pl[from].cardcnt) cursor=1;
			}
		}
		if(checkt(input)){
			cols=130;cls();UI();
			SetPos(0,Row-1);SetColor(7);printf("                                                                   "); 
			SetPos(0,Row);printf("P%d",from);
			SetPos(5,Row);printf("  #"); 
			SetPos(11,Row);SetColor(11);printf("◆");
			SetPos(20+14,Row);SetColor(7);printf("ATK");
			SetPos(30+14,Row);printf("HEAL");
			SetPos(40+14,Row);printf("DEF");
			SetPos(50+14,Row);printf("MISS");
			for(int i=1;i<=pl[from].cardcnt;i++){
				checkcard(from,to,i);
				showcard(from,to,cursor,i,option_giveup,option_use);
			}//显示手牌
			SetColor(7,0);
			for(int i=0;i<lines;i++)
				SetPos(82,i),cout<<'|';	
			
			SetPos(84,23),mouse(1);string x;getline(cin,x); 
			if(x!="")
			{
				if(x.find("陈果熠")!=-1 or x.find("chenguoyi")!=-1 or x.find("你妈")!=-1 or x.find("他妈")!=-1) x="请文明用语！";
				else if((int)x.size()>40) x="发言请勿过长！";
				else x="P"+int_to_string(from)+":"+x;
				if(send_gaming(conver_card)<0){
					another_player_quit(server_mode); 
					return 1;
				}
				if(send_int((int)x.size())<0){
					another_player_quit(server_mode); 
					return 1;
				}
				for(int i=0;i<(int)x.size();i++)
					if(send_int((int)x[i])<0){
						another_player_quit(server_mode); 
						return 1;
					}
				Con::append(x),cols=130;
			}
			if(Con::conver.empty()) cols=88;
			cls();UI();	
			mouse(0);
		}
		if(cursor<=0)cursor=pl[from].cardcnt;
		if(cursor>pl[from].cardcnt) cursor=1;//移动焦点
		SetPos(11,Row+10);
		printf("                                                ");
		if(checkz(input))//使用
		{
			SetPos(0,1);
			printf("                                               ");
			option_giveup=option_over=option_quit=0;
			if(!option_use){
				if(pl[3].occ==52 and pl[3].buff[0]/10000==cursor){
					SetPos(11,Row+10),printf("该卡牌槽被禁锢，无法打出！             ");
				}
				else option_use=1;
			}
			else if(option_use==1 and mode[6]==1 and chooseto<3 and pl[from].handcard[cursor].ATK>=50){
				
				option_use=2;
			}
			else {
				if(pl[from].used[cursor]){
					SetPos(11,Row+10),printf("选中的牌不存在！                     ");
				}
				else if(pl[from].cost<pl[from].handcard[cursor].cost){
					SetPos(11,Row+10),printf("费用不足无法使用此牌！                 ");
				}
				else{
					pl[from].cost-=pl[from].handcard[cursor].cost;
					pl[from].used[cursor]=1;
					pl[from].rest--;
					int qwq=pl[from].handcard[cursor].Use(from,to);
					if(qwq==1)
					{
						extra(from,to,cursor);
						if(pl[from].occ==5 && rad()%100<80) {
							pl[from].used[cursor]=0;
							pl[from].rest++;
							pl[from].handcard[cursor]=pl[from].heap[(rad()%pl[from].heapn)+1];
						}
						for(int i=1;i<=pl[from].cardcnt;i++){
							if(pl[from].used[i]) continue;
							if(pl[from].handcard[i].func==41) pl[from].handcard[i].ATK+=10;
							if(pl[from].handcard[i].func==171) pl[from].handcard[i].ATK+=8;
						}//func 41 171 增加ATK
					}
					if(qwq==-1 and pl[to].occ==12)
					{
						SetPos(5,Row+cursor),printf("  %d",cursor); 
						SetColor(8);printf("                                                                     ");
						SetPos(14,Row+cursor);printf("[Used.]");
						option_giveup=option_use=option_quit=0;option_over=1;input=ENTER;
					}
					while(pl[from].used[cursor] && cursor<=pl[from].cardcnt+1 && pl[from].rest){
						cursor++;
						if(cursor>pl[from].cardcnt) cursor=1;
					}
					if(pl[from].occ==7){
						pl[from].buff[10]=0;
						for(int i=1;i<=pl[from].cardcnt;i++) 
							if(!pl[from].used[i] && pl[from].handcard[i].func==42) pl[from].buff[10]++; //42-->[神圣意志]
					}//牧师检索[神圣意志]
					if(send_gaming(use_card)<0) {
						another_player_quit(server_mode); 
						return 1;
					}
					UI();
				}
				option_use=0;
			}
		}
		if(checkx(input)){//弃牌
			SetPos(0,1);
			printf("                                          ");
			option_use=option_over=option_quit=0;
			if(!option_giveup){
				if(pl[3].occ==52 and pl[3].buff[0]/10000==cursor){
					SetPos(11,Row+10),printf("该卡牌槽被禁锢，无法弃置！             ");
				}
				else option_giveup=1;
			}
			else{
				if(pl[from].occ!=6){
					if(giveupcard(from,to,cursor)){
						use_card=pl[from].handcard[cursor];
						use_card.MISS=2;
						if(mode[6]==1) use_card.MISS+=to*10;
						if(send_gaming(use_card)<0) {
							another_player_quit(server_mode); 
							return 1;
						}
					}
					if(rad()%2 and pl[from].occ==11)
						pl[from].handcard[cursor]=pl[from].heap[(rad()%pl[from].heapn)+1];
					else
					{
						if(pl[from].occ==11) pl[from].buff[0]++; 
						pl[from].used[cursor]=1;
						pl[from].rest--;
						UI();
						while(pl[from].used[cursor] && cursor<=pl[from].cardcnt+1 && pl[from].rest){
							cursor++;
							if(cursor>pl[from].cardcnt)cursor=1;
						}
					}
					option_giveup=0;
				}
				else{
					if(pl[from].used[cursor]){
						SetPos(11,Row+10),printf("选中的牌不存在！                  ");
					}
					else if(pl[from].handcard[cursor].func>=27 && pl[from].handcard[cursor].func<=29){
						pl[from].used[cursor]=1;
						pl[from].rest--;
						while(pl[from].used[cursor] && cursor<=pl[from].cardcnt+1 && pl[from].rest){
							cursor++;
							if(cursor>pl[from].cardcnt) cursor=1;
						}
						option_giveup=0;
						pl[from].buff[0]++;
						UI();
					}
					else if(pl[from].cost<pl[from].handcard[cursor].cost){
						SetPos(11,Row+10),printf("费用不足无法弃置此牌！                 ");
					}
					else{
						pl[from].cost-=pl[from].handcard[cursor].cost;
						pl[from].used[cursor]=1;
						pl[from].rest--;
						giveupcard(from,to,cursor);
						UI();
						swap(pl[from].handcard[cursor].ATK,pl[from].handcard[cursor].HEAL);
						pl[from].handcard[cursor].Use(from,to);
						while(pl[from].used[cursor] && cursor<=pl[from].cardcnt+1 && pl[from].rest){
							cursor++;
							if(cursor>pl[from].cardcnt)cursor=1;
						}
						option_giveup=0;
						pl[from].buff[0]++;
						UI();
						if(send_gaming(use_card)<0) {
							another_player_quit(server_mode); 
							return 1;
						}
					}
				}
				if(pl[from].occ==7){
					pl[from].buff[10]=0;
					for(int i=1;i<=pl[from].cardcnt;i++) 
						if(!pl[from].used[i] && pl[from].handcard[i].func==42) pl[from].buff[10]++; //42-->[神圣意志]
				}//牧师检索[神圣意志]
				if(send_gaming(void_card)<0) {
					another_player_quit(server_mode); 
					return 1;
				}
			}
		}
		if(checkc(input) and mode[6]==1){
			option_giveup=option_use=option_quit=option_over=0;
			to=chooseto=6-server_mode-chooseto;
			if(chooseto<3 and death[chooseto]) chooseto=3;
			SetColor(11);
			for(int i=0;i<=2;i++)
				SetPos(3,(6-server_mode-chooseto)*4-i),printf(" ");
			for(int i=0;i<=2;i++)
				SetPos(3,chooseto*4-i),printf(">");
		}
		if(checkenter(input)) {//结束回合
			option_giveup=option_use=option_quit=0;
			SetPos(0,1);
			printf("                ");
			if(!option_over){
				option_over=1;
				SetPos(11,Row+12);
				printf("确定要结束回合吗？    ");
			}else{
				option_over=0;
				SetPos(11,Row+12);
				printf("                    ");
				break;
			}
		}
		if(!option_over && !option_quit){
			SetPos(11,Row+12);
			printf("                      ");
		}
		if(pl[from].rest<=0){//结束回合
			SetPos(0,1);
			printf("                ");
			for(int i=1;i<=pl[from].cardcnt;i++){
				SetColor(8);
				SetPos(5,Row+i);
				printf("  ");
				SetPos(9,Row+i);
				printf("                                                                        ");
				SetPos(14,Row+i);
				printf("[Used.]");
			}
			SetColor(7);
			SetPos(11,Row+9);
			printf("                                                                     ");
			Sleep(200);
			while(_kbhit()) getch();
			break;
		} 
		Check(from);
		if(death[from])
		{
			appcnt=0;cls();UI();
			break;
		}
	}
	return 0;
}

struct pridata{
	int cursor,to,num;
};

vector<pridata> boss_calpri(){
	vector<pridata> res;res.clear();
	for(int i=1;i<=pl[3].cardcnt;i++)
	{
		if(pl[3].used[i]) continue; 
		Card &nowcard=pl[3].handcard[i];
		for(int to=1;to<=2;to++)
		{
			if(death[to]) continue;
			int pri=0;
			if(boss_cardkind[nowcard.id]==1){
				pri+=350-(pl[to].hp*pow(0.75,pl[to].buff[2])+pl[to].def-40*pl[to].buff[1]+35*pl[to].buff[5])*0.5+(nowcard.cal_atk(3,to)-nowcard.ATK);
				if(pl[to].occ==5) pri+=max(120-pl[to].buff[0]*6,0);
				if(pl[to].occ==7) pri+=100;
				if(pl[to].occ==8) pri+=pl[to].buff[0]*3.5+min(pl[to].buff[0],(int)(nowcard.cal_atk(3,to)/3))*3.5;
				if(pl[to].occ==9) pri+=pl[to].def*0.5+min(pl[to].def,nowcard.cal_atk(3,to))*0.5;
				if(pl[to].occ==13 and pl[to].buff[0]/1000000!=0) pri+=min(int(pl[to].hp*1.2)+(pl[to].buff[0]/1000000)*45,0);
				if(nowcard.cal_atk(3,to)>=pl[to].hp+pl[to].def and pl[to].occ!=13) pri+=50;
				if(pl[3].occ==52) pri+=pl[to].buff[7]*10;
			}
			if(boss_cardkind[nowcard.id]==2)
				pri+=350-(pl[3].hp*pow(0.75,pl[3].buff[2])+pl[3].def-40*pl[3].buff[1]+35*pl[3].buff[5])*0.25+(nowcard.cal_heal(3,to)-nowcard.HEAL);
			
			if(nowcard.id==245){
				pri+=30*(pl[3].buff[1]+pl[3].buff[2]+pl[3].buff[4]+pl[3].buff[6]);
				for(int i=1;i<=6;i++){
					if(i==3 or i==5) continue;
					if(pl[3].buff[i]>=5) pri+=200;
				}
			}
			if(nowcard.id==246){
				pri+=60-pl[3].def;
				if(pl[3].def==0) pri+=20;
			}
			if(nowcard.id==248){
				int qwq=max(int(185-pl[3].buff[0]),0);
				if(death[1] or death[2]) qwq/=2;
				pri+=qwq;
			}
			if(nowcard.id==249)
				pri+=pl[to].cost*20;
			if(nowcard.id==252){
				int qwq=450;
				for(int i=1;i<=pl[to].heapn;i++)
					if(pl[to].heap[i]==lib[50][libcnt[50]+1])
						qwq-=18; 
				pri+=max(qwq,0);
			}
			if(nowcard.id==254){
				for(int i=1;i<=pl[to].heapn;i++)
					if(pl[to].heap[i]==lib[50][libcnt[50]+1])
						pri+=5;
			}
			if(nowcard.id==255){
				if(pl[3].hp<=200+(pl[1].cost+pl[2].cost)*25 and pl[3].hp<pl[3].maxhp-550)
					pri+=300;
			}
			if(nowcard.id==265){
				int cnt=1;
				for(int i=1;i<=pl[3].cardcnt;i++)
					if(pl[3].used[i]) cnt++;
				int maxn=pl[1].buff[7];
				if(death[1] or (!death[2] and pl[2].buff[7]>maxn)) maxn=pl[2].buff[7];
				pri+=cnt*(20*(1+maxn*0.25)*(1+int(pl[3].buff[3]>0)*1)*(1-int(pl[3].buff[4]>0)*0.3)*(1+int(env_now==1)*0.2)*(1-int(env_now==2)*0.2));
			}
			if(nowcard.id==266){
				pri+=pl[3].buff[0]/100%100*30;
			}
			if(nowcard.id==267){
				pri+=(pl[3].buff[0]%100-6)*30;
			}
			if(nowcard.id==268){
				pri+=pl[3].buff[0]/100%100*8;
			}
			
			if(nowcard.id==172)
				if(pl[to].def>90+40*min(2,pl[to].maxcost-pl[to].cost))
					pri+=500;
			if(nowcard.id==210 and pl[3].occ!=51)
				pri+=pl[3].buff[4]*19+10*min(2,pl[to].maxcost-pl[to].cost); 
			
			if(pl[3].cost+2+int(env_now==7)>pl[3].maxcost) pri+=30;
			if(pl[to].occ==12 and pl[to].cost>=2 and pl[to].rest>0 and nowcard.ATK>=80) pri+=120-nowcard.cost*30;
			if(pl[to].occ==12) pri+=max(0,int((100/(nowcard.cost+1))+pow(3,pl[to].buff[0])-50)); 
			if(nowcard.cost==0) pri+=300;
			for(int i=1;i<=2;i++)
				if(!death[i] and pl[i].occ==10 and pl[to].buff[0]==92 and mode[23]==1)
					pri+=1000;
			
			pri*=(100-nowcard.cal_miss(3,to))/100.0;
			
			for(int i=1;i<=2;i++)
				if(!death[i] and pl[i].occ==12 and pl[3].rest<=pl[3].cardcnt/2-1)
					pri=-114514;
			
			if(nowcard.id==223)
				pri=lim1-1;
			
			res.push_back(pridata{i,to,pri});
		}
	}
	return res;
}

namespace Joker{
	int s1[15],s2[15],s3[15],s4[15],cnt,cost,buff0,buff6;
	struct two{
		double t1;int t2;
	};
	struct five{
		int n,t1,t2,t3,t4;
	};
	two dfs()
	{
		double n1=0;int n2=0;
		for(int i=1;i<=pl[3].cardcnt;i++)
		{
			if(s3[i]==1) continue;
			if(s3[i]==0 and cost>=s1[i])
			{
				vector<five> pre;pre.clear();
				int thes2=s2[i];
				cost-=s1[i],s3[i]=1,buff0--;
				if(s4[i]==188)
				{
					for(int j=1;j<=pl[3].cardcnt;j++)
						if(s3[j]==1)
						{
							pre.push_back(five{j,s1[j],s2[j],s3[j],s4[j]});
							s1[j]=s2[j]=s3[j]=s4[j]=0;
						}
				}
				two res=dfs();
				double delta=(100-min(100,thes2+int(buff6>0)*25))/100.0;
				if(res.t1+delta>n1) n1=res.t1+delta,n2=i;
				for(five x:pre)
					s1[x.n]=x.t1,s2[x.n]=x.t2,s3[x.n]=x.t3,s4[x.n]=x.t4;
				cost+=s1[i],s3[i]=0;
			}
			if(s3[i]==0 or (s3[i]==-1 and cnt>1))
			{
				int pres3=s3[i],precnt=cnt,prebuff6=buff6;
				if(s3[i]==-1) cnt--;
				if(s4[i]==156) buff6++;
				s3[i]=1;
				two res=dfs();
				if(res.t1>n1) n1=res.t1,n2=-i;
				s3[i]=pres3,cnt=precnt,buff6=prebuff6;
			}
		}
		return two{n1,n2};
	}
	int check()
	{
		cnt=0,cost=pl[3].cost,buff0=pl[3].buff[0]%100,buff6=pl[3].buff[6];
		for(int i=1;i<=pl[3].cardcnt;i++)
			if(!pl[3].used[i])
			{
				s1[i]=pl[3].handcard[i].cost,s2[i]=pl[3].handcard[i].MISS,s3[i]=0,s4[i]=pl[3].handcard[i].func;
				if(pl[3].handcard[i].func==187) cnt++,s3[i]=-1;
			}
			else s3[i]=1;
		if(buff0==0) return 0;
		if(cnt==0) return 0;
		two res=dfs();
		if(res.t1+0.1>=buff0) return res.t2;
		return 0;
	}
}//一个用于检测能不能凑出[完美谢幕]的模块。然而并不是那么完美。 

int Ask_boss(int from){
	start_turn(now,0);
	UI();
	if(Check(from)){send_gaming(void_card); return 1;}
	use_card=(Card){0,0,0,0,0,-2,0};
	if(send_gaming(use_card)<0) {
		another_player_quit(server_mode); 
		return 1;
	}
	UI();UI_other(from);
	Sleep(500);while(_kbhit()) getch();
	while(!winner)
	{
		UI();UI_other(from);
		for(int i=1;i<=pl[3].cardcnt;i++)
			checkcard(3,0,i);
		vector<pridata> x=boss_calpri();
		if(DimlyCheck)
		{
			SetPos(0,1),printf("%d %d   ",lim2,lim1);
			for(int i=0;i<x.size();i++)
			{
				SetPos(10,21+i);
				printf(pl[from].handcard[x[i].cursor].Name());
				printf("%d %d %d",x[i].cursor,x[i].to,x[i].num);
			}
			for(int i=x.size();i<8;i++)
			{
				SetPos(10,21+i);
				printf("                             ");
			}	
		}
		
		
		int cnt1=0,cnt2=0,cnt3=0;
		for(int i=0;i<x.size();i++)
			if(x[i].num<lim2) cnt1++;
			else if(x[i].num<lim1) cnt2++;
			else cnt3++;
		
		
		int kind=3,cursor=0,to=0,maxn=lim2,minn=lim1;
		for(int i=0;i<x.size();i++)
			if((x[i].num>maxn or (x[i].num==maxn and rad()%2==0)) and pl[from].cost>=pl[from].handcard[x[i].cursor].cost)
			{
				maxn=x[i].num;
				kind=1,cursor=x[i].cursor,to=x[i].to;
			}
		if(kind==1 and pl[from].handcard[cursor].func==188)
		{
			minn=114514;
			for(int i=0;i<x.size();i++)
				if(((x[i].num<minn and x[i].num!=-114514) or (x[i].num==minn and rad()%2==0)) and pl[from].handcard[x[i].cursor].func!=188)
				{
					minn=x[i].num;
					kind=2,cursor=x[i].cursor,to=x[i].to;
				}
		}
		else if(kind==3)
		{
			for(int i=0;i<x.size();i++)
				if((x[i].num<minn and x[i].num!=-114514) or (x[i].num==minn and rad()%2==0))
				{
					minn=x[i].num;
					kind=2,cursor=x[i].cursor,to=x[i].to;
				}
		}
		
		if(pl[from].occ==52)
		{
			int res=Joker::check();
			if(res>0)
			{
				kind=1,cursor=res;
				if(death[1]) to=2;
				else if(death[2]) to=1;
				else if(pl[from].handcard[cursor].cal_miss(from,1)<pl[from].handcard[cursor].cal_miss(from,2) or pl[2].occ==12) to=1;else to=2;
			}
			if(res<0)
			{
				kind=2,cursor=-res;
				if(death[1]) to=2;
				else if(death[2]) to=1;
				else to=rad()%2+1;
			}
//			SetPos(0,1),printf("%d",res);
		}
		
		if(kind==1)
		{
			pl[from].cost-=pl[from].handcard[cursor].cost;
			pl[from].used[cursor]=1;
			pl[from].rest--;
			int qwq=pl[from].handcard[cursor].Use(from,to);
			if(qwq==1) extra(from,to,cursor);
			if(qwq==-1 and pl[to].occ==12) kind=3;
			if(send_gaming(use_card)<0) {
				another_player_quit(server_mode); 
				return 1;
			}
			appcard[++appcnt]=use_card; 
			UI();UI_other(from);
		}
		if(kind==2)
		{
			if(giveupcard(from,to,cursor)){
				use_card=pl[from].handcard[cursor];
				use_card.MISS=2;
				use_card.MISS+=to*10;
				if(send_gaming(use_card)<0) {
					another_player_quit(server_mode); 
					return 1;
				}
				appcard[++appcnt]=use_card; 
			}
			pl[from].used[cursor]=1;
			pl[from].rest--;
			if(send_gaming(void_card)<0) {
				another_player_quit(server_mode); 
				return 1;
			}
			UI();UI_other(from);
		}	
		if(kind==3 or pl[from].rest==0) break;
		
		if(cnt2==0 and cnt3==0) lim2=max(1,int(lim2-rad()%5+5));
		if(cnt1==0) lim2+=rad()%5+5;
		if(cnt1==0 and cnt2==0) lim1+=rad()%5+5;
		if(cnt3==0) lim1-=rad()%5+5; 
		
		Sleep(200+rad()%1100);while(_kbhit()) getch();
	}
	return 0;
}

int Ask_same(int now){
	int option_use=0,option_giveup=0,option_over=0;
	start_turn(now,3-now);
	UI();
	if(Check(now)) return 0;

	int cursor=1;
	SetColor(7);
	while(!winner){
		SetPos(0,Row-1);SetColor(7);printf("                                                                   "); 
		SetPos(0,Row);printf("P%d",now);
		SetPos(5,Row);printf("  #"); 
		SetPos(11,Row);SetColor(11);printf("◆");
		SetPos(20+14,Row);SetColor(7);printf("ATK");
		SetPos(30+14,Row);printf("HEAL");
		SetPos(40+14,Row);printf("DEF");
		SetPos(50+14,Row);printf("MISS");
		
		for(int i=1;i<=pl[now].cardcnt;i++){
			checkcard(now,3-now,i);
			showcard(now,3-now,cursor,i,option_giveup,option_use);
		}//显示手牌
		mouse(0);
		SetColor(7);
		SetPos(11,Row+9);
		printf("                                                                         ");
		SetPos(11,Row+9);
		printf(pl[now].handcard[cursor].Intro());
		input=getch();
		if(input>='1' && input<='9' ){
			if(!pl[now].used[input-'0'] && input-'0'<=pl[now].cardcnt) cursor=input-'0';
		}
		if(checkleft(input) or checkup(input)){
			option_use=option_giveup=option_over=0;
			cursor--;
			if(cursor<=0) cursor=pl[now].cardcnt;
			while(pl[now].used[cursor]&&cursor>0){
				cursor--;
				if(cursor<=0) cursor=pl[now].cardcnt;
			}
		}
		if(checkright(input) or checkdown(input)){
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
		SetPos(11,Row+10);
		printf("                                                ");
		if(checkz(input))//使用
		{
			SetPos(0,1);
			printf("                                  ");
			option_giveup=option_over=0;
			if(!option_use){
				option_use=1;
			}
			else {
				if(pl[now].used[cursor]){
					SetPos(11,Row+10),printf("选中的牌不存在！                 ");
				}
				else if(pl[now].cost<pl[now].handcard[cursor].cost){
					SetPos(11,Row+10),printf("费用不足无法使用此牌！                 ");
				}
				else{
					pl[now].cost-=pl[now].handcard[cursor].cost;
					pl[now].used[cursor]=1;
					pl[now].rest--;
					int qwq=pl[now].handcard[cursor].Use(now,3-now);
					if(qwq==1)
					{
						extra(now,3-now,cursor);
						if(pl[now].occ==5 && rad()%100<80) {
							pl[now].used[cursor]=0;
							pl[now].rest++;
							pl[now].handcard[cursor]=pl[now].heap[(rad()%pl[now].heapn)+1];
						}
						for(int i=1;i<=pl[now].cardcnt;i++){
							if(pl[now].used[i]) continue;
							if(pl[now].handcard[i].func==41) pl[now].handcard[i].ATK+=10;
							if(pl[now].handcard[i].func==171) pl[now].handcard[i].ATK+=8;
						}//func 41 171 增加ATK
					}
					if(qwq==-1 and pl[3-now].occ==12)
					{
						SetPos(5,Row+cursor),printf("  %d",cursor); 
						SetColor(8);printf("                                                                     ");
						SetPos(14,Row+cursor);printf("[Used.]");
						option_giveup=option_use=0;option_over=1;input=ENTER;
					}
					while(pl[now].used[cursor] && cursor<=pl[now].cardcnt+1 && pl[now].rest){
						cursor++;
						if(cursor>pl[now].cardcnt) cursor=1;
					}
					UI();
				}
				option_use=0;
			}
		}
		if(checkx(input)){//弃牌
			SetPos(0,1);
			printf("                ");
			option_use=option_over=0;
			if(!option_giveup){
				option_giveup=1;
			}
			else{
				if(pl[now].occ!=6){
					giveupcard(now,3-now,cursor);
					if(rad()%2 and pl[now].occ==11)
					{
						pl[now].handcard[cursor]=pl[now].heap[(rad()%pl[now].heapn)+1];
						UI();
					}
					else
					{
						if(pl[now].occ==11) pl[now].buff[0]++; 
						pl[now].used[cursor]=1;
						pl[now].rest--;
						UI();
						while(pl[now].used[cursor] && cursor<=pl[now].cardcnt+1 && pl[now].rest){
							cursor++;
							if(cursor>pl[now].cardcnt)cursor=1;
						}	
					}	
					option_giveup=0;
				}
				else{
					if(pl[now].used[cursor]){
						SetPos(11,Row+10),printf("选中的牌不存在！                 ");
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
						SetPos(11,Row+10),printf("费用不足无法弃置此牌！                 ");
					}
					else{
						pl[now].cost-=pl[now].handcard[cursor].cost;
						pl[now].used[cursor]=1;
						pl[now].rest--;
						giveupcard(now,3-now,cursor);
						UI();
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
		if(checkenter(input)) {//结束回合
			SetPos(0,1);
			printf("                ");
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
		if(pl[now].rest<=0){//结束回合
			SetPos(0,1);
			printf("                ");
			Sleep(200);
			while(_kbhit()) getch();
			break;
		} 
		Check(now);
	}
	return 0;
}

bool Options(bool x){
	if(x){
		cls();
		SetPos(6,4);
		printf("※ 设置 Options ※");
		int cursor=1;
		while(1){
			for(int i=1;i<=6;i++)
			{
				SetPos(6,6+i);
				if(cursor!=i) SetColor(7,0);
				else SetColor(0,7);
				if(i==1) printf("先手玩家                              ");
				if(i==2) printf("天气变化                              ");
				if(i==3) printf("宝藏牌                                ");
				if(i==4) printf("娱乐：随机buff                        ");
				if(i==5) printf("娱乐：高失误                          ");
				if(i==6) printf("游戏模式                              ");
				SetPos(38,6+i);
				if(i==1)
				{
					if(mode[1]==0) printf("随机");
					else printf(" P%d",mode[1]);
				}
				else if(i==6)
				{
					if(mode[6]==0) printf("经典");
					else printf("BOSS");
				} 
				else
				{
					if(mode[i]==1) printf("■开");
					else printf("关■");
				}
			}
			SetPos(8,15);SetColor(7,0);
			if(cursor==1){
				printf("选择先手玩家，后手有一定补偿                          ");
				printf("\n\t                                                  ");
			}
			if(cursor==2){
				printf("天气变化将影响到战局，带给双方增益或减益              ");
				printf("\n\t                                                  ");
			}
			if(cursor==3){
				printf("在满足特定要求时将获得特殊的强力宝藏牌，每局仅1次     ");
				printf("\n\t                                                  ");
			}
			if(cursor==4){
				printf("每回合开始时获得1回合随机buff,玩家牌库中添加娱乐牌    ");
				printf("\n\t所有牌ATK/HEAL/DEF降为原先的70%%                   ");
			}
			if(cursor==5){
				printf("所有牌MISS增加25,但ATK同时也上调50%%                   ");
				printf("\n\t                                                  ");
			}
			if(cursor==6){
				if(mode[6]==0)
				{
					printf("最经典的游戏模式，两人对战，殊死搏斗                   ");
					printf("\n\t                                                  ");	
				}
				if(mode[6]==1)
				{
					printf("两名玩家合作共同挑战BOSS                               ");
					printf("\n\t                                                  ");	
				}
			}
			
			if(server_mode==1){
				input=getch();
				if(checkup(input)) cursor--;
				if(checkdown(input)) cursor++;
				if(cursor>6) cursor=1;
				if(cursor<1) cursor=6;
				if(checkz(input)){
					if(cursor==1){
						if(mode[6]==0)
							mode[1]=(mode[1]+1)%3;
					}
					else if(cursor==6)
					{
						mode[6]=(mode[6]+1)%2;
						if(mode[6]==1) mode[1]=1;
					}
					else mode[cursor]^=1;
				}
				if(checkenter(input)){
					send_int(8255); break;
				}
				if(send_int(6000+cursor)<0)
				{
					another_player_quit(server_mode); 
					return 1;
				}
				for(int i=1;i<=40;i++)
					if(send_int(mode[i])<0)
					{
						another_player_quit(server_mode); 
						return 1;
					}
			}
			else{
				int recv_val=recv_message();
				if(recv_val<0) {
					another_player_quit(server_mode); 
					return 1;
				}
				if(recv_val/1000==6 and recv_val%1000/100==0) cursor=recv_val%100;
				if(recv_val==8255) break;
				
			}
		}
	}
	else {
		SetPos(6,4);
		printf("※ 设置 Options ※");
		int cursor=1;
		while(1){
			for(int i=1;i<=5;i++)
			{
				SetPos(6,6+i);
				if(cursor!=i) SetColor(7,0);
				else SetColor(0,7);
				if(i==1) printf("先手玩家                              ");
				if(i==2) printf("天气变化                              ");
				if(i==3) printf("宝藏牌                                ");
				if(i==4) printf("娱乐：随机buff                        ");
				if(i==5) printf("娱乐：高失误                          ");
				SetPos(38,6+i);
				if(i==1)
				{
					if(mode[1]==0) printf("随机");
					else printf(" P%d",mode[1]);
				}
				else
				{
					if(mode[i]==1) printf("■开");
					else printf("关■");
				}
			}
			SetPos(8,15);SetColor(7,0);
			if(cursor==1){
				printf("选择先手玩家，后手有一定补偿                          ");
				printf("\n\t                                                  ");
			}
			if(cursor==2){
				printf("天气变化将影响到战局，带给双方增益或减益              ");
				printf("\n\t                                                  ");
			}
			if(cursor==3){
				printf("在满足特定要求时将获得特殊的强力宝藏牌，每局仅1次     ");
				printf("\n\t                                                  ");
			}
			if(cursor==4){
				printf("每回合开始时获得1回合随机buff,玩家牌库中添加娱乐牌    ");
				printf("\n\t所有牌ATK/HEAL/DEF降为原先的70%%                   ");
			}
			if(cursor==5){
				printf("所有牌MISS增加25,但ATK同时也上调50%%                   ");
				printf("\n\t                                                  ");
			}
			
			//游戏模式设定
			mouse(0);
			input=getch();
			if(checkup(input)) cursor--;
			if(checkdown(input)) cursor++;
			if(cursor>5) cursor=1;
			if(cursor<1) cursor=5;
			if(checkz(input)){
				if(cursor==1){
					mode[1]=(mode[1]+1)%3;
				}
				else mode[cursor]^=1;
			}
			if(checkenter(input)){
				break;
			}
		}
	}
	return 0;
}

bool Options_boss(){
	cls();
	SetPos(6,4);
	printf("※ 设置 Options ※");
	int cursor=1;
	while(1){
		for(int i=1;i<=4;i++)
		{
			SetPos(6,6+i);
			if(cursor!=i) SetColor(7,0);
			else SetColor(0,7);
			if(i==1) printf("BOSS中毒比率                          ");
			if(i==2) printf("BOSS血量倍率                          ");
			if(i==3) printf("赌徒[天使92]                          ");
			if(i==4) printf("[窒碍难行]数量                        ");
			SetPos(38,6+i);
			if(i==1)
				printf("%d%%",10+mode[21]*5);
			else if(i==2)
				printf("%d%%",50+mode[22]*10);
			else if(i==4)
				printf(" %d",mode[24]);
			else
			{
				if(mode[i+20]==1) printf("■开");
				else printf("关■");
			}
		}
		SetPos(8,15);SetColor(7,0);
		if(cursor==1)
			printf("BOSS受到[中毒]buff影响所损失的血量百分比              ");
		if(cursor==2)
			printf("BOSS初始血量相对于正常血量的倍率                      ");
		if(cursor==3)
			printf("若关闭则赌徒牌库中不刷入[天使92]                      ");
		if(cursor==4)
			printf("两名玩家初始牌库中刷入[窒碍难行]的数量                ");
		
		if(server_mode==1){
			input=getch();
			if(checkup(input)) cursor--;
			if(checkdown(input)) cursor++;
			if(cursor>4) cursor=1;
			if(cursor<1) cursor=4;
			if(checkz(input)){
				if(cursor==1)
					mode[21]=(mode[21]+1)%4;
				else if(cursor==2)
					mode[22]=(mode[22]+1)%11;
				else if(cursor==4)
					mode[24]=(mode[24]+1)%6;
				else mode[20+cursor]^=1;
			}
			if(checkenter(input)){
				send_int(8255); break;
			}
			if(send_int(6000+cursor)<0)
			{
				another_player_quit(server_mode); 
				return 1;
			}
			for(int i=1;i<=40;i++)
				if(send_int(mode[i])<0)
				{
					another_player_quit(server_mode); 
					return 1;
				}
		}
		else{
			int recv_val=recv_message();
			if(recv_val<0) {
				another_player_quit(server_mode); 
				return 1;
			}
			if(recv_val/1000==6 and recv_val%1000/100==0) cursor=recv_val%100;
			if(recv_val==8255) break;
		}
	}
	return 0;
}

void Connect(){
	cols=88,pl[3].occ=0;
	
	SetPos(3,10);
	SetColor(7,0);
	printf("切换上一条目                          ");
	SetPos(27,10);
	printf("[w]  [↑]");

	SetPos(3,11);
	printf("切换下一条目                          ");
	SetPos(27,11);
	printf("[s]  [↓]");

	SetPos(3,12);
	printf("打出手牌/更改设置                          ");
	SetPos(27,12);
	printf("[z]  [+]");

	SetPos(3,13);
	printf("弃置手牌                          ");
	SetPos(27,13);
	printf("[x]  [-]");

	SetPos(3,14);
	printf("结束回合/确认                          ");
	SetPos(27,14);
	printf("[SPACE]  [ENTER]");

	SetPos(3,15);
	printf("认输                          ");
	SetPos(27,15);
	printf("[q]");
	
	SetPos(3,16);
	printf("聊天                          ");
	SetPos(27,16);
	printf("[t]");
	
	SetPos(3,20);
	printf("游戏版本: v");
	if(Tysta) printf("Tysta");
	else printf(version);
	SetPos(3,21);
	printf("游戏主页: https://github.com/cyw580/CARD-GAME");
	SetPos(3,22);
	printf("洛谷团队: https://www.luogu.com.cn/team/72157");
	
	SetPos(50,6);
	printf("v4.0.0 更新内容：");
	SetPos(50,7);
	printf("1.推出全新BOSS战模式（测试版）");
	SetPos(50,8);
	printf("  新BOSS：破坏神、小丑魔");
	if(Tysta or rad()%10==0) SetColor(8),printf("、特娅斯塔"),SetColor(7);
	SetPos(50,9);
	printf("2.增加易伤、诅咒buff以及一系列牌");
	SetPos(50,10);
	printf("3.剑客、忍者大幅改动");
	
	SetPos(50,12);
	printf("v4.0.1 更新内容：");
	SetPos(50,13);
	printf("1.小丑魔AI优化和强度调整");
	SetPos(50,14);
	printf("2.修复BUG；部分其他改动");
	
	
	int cursor=1;
	string qwqin="";
	while(1){
		SetColor(7,0);
		SetPos(3,3);
		if(cursor!=1) SetColor(7,0);
		else SetColor(0,7);
		printf("1.建立服务端        ");

		SetPos(3,4);
		if(cursor!=2)SetColor(7,0);
		else SetColor(0,7);
		printf("2.连接到已有的服务端");

		SetPos(3,5);
		if(cursor!=3)SetColor(7,0);
		else SetColor(0,7);
		printf("3.在本机进行游戏    ");
		
		SetPos(3,6);
		if(cursor!=4)SetColor(7,0);
		else SetColor(0,7);
		printf("4.常见疑问解答      ");
		
		SetPos(3,7);
		if(cursor!=5)SetColor(7,0);
		else SetColor(0,7);
		printf("5.退出游戏          ");
		
		
		mouse(0);
		input=getch();
		if(checkup(input)) cursor--;
		if(checkdown(input)) cursor++;
		if(input>='1' && input<='5') cursor=input-'0';
		if(cursor>5) cursor=1;
		if(cursor<1) cursor=5;
		if(checkenter(input)){
			server_mode=cursor;
			break;
		}
		qwqin+=input;
		SetColor(7,0);
		if(qwqin.size()>=5)
			if(qwqin.substr(qwqin.size()-5)=="tysta")
			{
				SetPos(60,24);
				printf("[Tysta]            ");
				Tysta=1;
			}
		if(qwqin.size()>=10)
			if(qwqin.substr(qwqin.size()-10)=="dimlycheck")
			{
				SetPos(60,24);
				printf("[DimlyCheck]       ");
				DimlyCheck=1;
			}
		if(qwqin.size()>=17)
			if(qwqin.substr(qwqin.size()-17)=="destructivestrike")
			{
				SetPos(60,24);
				printf("[DestructiveStrike]");
				DestructiveStrike=1;
			}
	}
}

void set_player_init(int from,int to){
	if(pl[from].occ!=6 and pl[from].occ<=50) pl[from].cost=3;//初始费用设置
	if(pl[from].occ==9) pl[from].buff[0]=4;//盾卫初始4点荆棘 
	if(pl[from].occ==10) pl[from].buff[0]=rad()%100;//赌徒初始随机局面
	if(pl[from].occ==52) pl[from].buff[0]=909;
	if(pl[from].occ==13){
		int base=40;
		if(pl[to].occ!=5 and pl[to].occ!=6 and pl[to].occ!=8) base+=10;
		pl[from].buff[0]=base*1000;	
	}
	init(from);//获得相应牌形成牌库
	for(int i=1;i<=1+int(from==3);i++)
	{
		int t=i;
		if(from<3) t=to;
		if(pl[t].occ==7 && pl[from].occ!=7) {//获得[亵渎] 
			pl[from].heap[++pl[from].heapn]=lib[7][libcnt[7]+2];
		}
		if(pl[t].occ==9 && pl[from].occ!=9) {//获得[防御攻破] 
			pl[from].heap[++pl[from].heapn]=lib[9][libcnt[9]+1];
		}
		if(pl[t].occ==11 && pl[from].occ!=11){//获得[虚弱疗愈]
			pl[from].heap[++pl[from].heapn]=lib[11][libcnt[11]+1];
		}
		for(int i=1;i<=pl[from].cardcnt;i++) {
			pl[from].handcard[i]=pl[from].heap[(rad()%pl[from].heapn)+1];
		}	
	}
	pl[from].rest=pl[from].cardcnt;
}

void pregame(){
	SetConsoleTitle("CARD GAME:Choose Your Identity.");
	if(server_mode==3)
	{
		for(int now=1;now<=2;now++){
			Choose(now);//选择职业
			SetPos(5,20+now);SetColor(7,0);
			printf("P%d的职业是",now);
			printf(occ_name(pl[now].occ));
		}
		for(int i=1;i<=2;i++)
			set_player_init(i,3-i);
	}
	else
	{
		if(mode[6]==1)
		{
			lib[7][libcnt[7]+1].func=172;
			Options_boss();
			Choose_boss();
			cls();
		}
		else
			lib[7][libcnt[7]+1].func=42;
		Choose(server_mode);//选择职业
		SetPos(5,21);SetColor(7,0);
		printf("P%d的职业是",server_mode);
		printf(occ_name(pl[server_mode].occ));
		if(mode[6]==0)
			set_player_init(server_mode,3-server_mode);
		if(mode[6]==1)
			set_player_init(server_mode,3);
		if(server_mode==1) send_int(2010);
		else send_int(2020);
		send_player(pl[server_mode]);
		recv_message();
		
		if(mode[6]==1)
		{
			lines=30,Row=15;
			if(server_mode==1)
			{
				set_player_init(3,0);
				send_int(2030);
				send_player(pl[3]);
			}
			else
				recv_message();
		}
	}
	SetConsoleTitle("Here we go...");
	Sleep(1500);
	while(_kbhit()) getch();
	cls();//清屏并开始游戏
}

int game(){
	if(server_mode!=3){
		pregame();
		if(server_mode==1){
			if(!mode[1]){
				mode[1]=rad()%2+1;
			}//随机先手
			now=mode[1];
		}
		env_now=0;
		char title[]="CARD GAME:Turn of P1  Turn:   ";
		if(server_mode==1){
			send_int(4000+now*10);
		}
		else if(recv_message()<0) {
			another_player_quit(server_mode);
			return 1;
		}
		if(server_mode==now) 
		{
			SetPos(11,Row+12);
			printf("即将轮到你的回合......");
			adturn=1;
		}
		Sleep(1500);while(_kbhit()) getch();
		
		int to=3-server_mode;
		if(mode[6]==1) to=3;
		while(!winner){
			if(!if_adv) adv(to),if_adv=1; //后手补偿
			if(pl[to].occ==7 && pl[to].buff[10]==pl[to].cardcnt) {
				cls();
				SetPos(13,0);
				SetColor(14);
				printf("对方受到了[神圣意志]的净化,你输了!");
				Sleep(500);
				winner=to;
				break;
			}
			if(!adturn) turn++,adturn=1;
			title[19]=now+'0';
			title[27]=turn/100+'0';
			title[28]=turn/10%10+'0';
			title[29]=turn%10+'0';
			SetConsoleTitle(title);
			if(now==server_mode){
				turn++;
				adturn=0;
				title[19]=now+'0';
				title[27]=turn/100+'0';
				title[28]=turn/10%10+'0';
				title[29]=turn%10+'0';
				SetConsoleTitle(title);
				appcnt=0;
				cls();UI();
				if(Ask(now,to)==1) break;			//origin copy
				if(!death[now]) end_turn(now,to);
				if(!winner){
					if(mode[6]==0) now=to;
					if(mode[6]==1) 
					{
						now=now%3+1;
						if(death[now]) now=now%3+1;
					}
					if(send_gaming(void_card)<0) {
						another_player_quit(server_mode); 
						break;
					}
					
					SetPos(11,Row+12);
					if(now==server_mode and !death[now]) printf("即将轮到你的回合......");
					if(now!=server_mode and now<=2)
					{
						if(mode[6]==0) printf("你的回合即将结束......");
						if(mode[6]==1) printf("即将轮到P%d的回合......",3-server_mode);
					}
					if(now==3) printf("即将轮到BOSS的回合......");
					Sleep(1500);while(_kbhit()) getch();
					appcnt=0;cls();
				}
				else if(pl[now].occ==7 && pl[now].buff[10]==pl[now].cardcnt){ 
					cls();
					SetPos(11,0);
					SetColor(6);
					printf("[神圣意志]洗涤了你的心灵,你直接获得了胜利!");
					now=to;
					if(send_gaming(void_card)<0){
						another_player_quit(server_mode); 
						break;
					}
					Sleep(500);
				}
				else if(winner==now){
					cls();
					SetPos(13,0);
					SetColor(6);
					printf("恭喜你击败对手获得了胜利!");
					now=to;
					if(send_gaming(void_card)<0) {
						another_player_quit(server_mode); 
						break;
					}
					Sleep(500);
				}
			}
			else{
				if(server_mode==2 or now==2)
				{
					UI();UI_other(now);
					int fnow=now;
					if(recv_gaming()<0) {
						another_player_quit(server_mode); 
						break;
					}
					if(fnow!=now)
					{
						SetPos(11,Row+12);
						if(now==server_mode and !death[now]) printf("即将轮到你的回合......");
						if(now!=server_mode and now<=2)
						{
							if(mode[6]==0) printf("你的回合即将结束......");
							if(mode[6]==1) printf("即将轮到P%d的回合......",3-server_mode);
						}
						if(now==3) printf("即将轮到BOSS的回合......");
						Sleep(1500);while(_kbhit()) getch();
						appcnt=0;cls();
					}
				}
				else
				{
					turn++;
					title[19]=now+'0';
					title[27]=turn/100+'0';
					title[28]=turn/10%10+'0';
					title[29]=turn%10+'0';
					SetConsoleTitle(title);
					cls();UI();
					if(Ask_boss(now)==1) break;
					end_turn(now,0);
					if(!winner){
						now=1;
						if(death[1]) now=2; 
						if(send_gaming(void_card)<0) {
							another_player_quit(server_mode); 
							break;
						}
						
						SetPos(11,Row+12);
						if(now==server_mode and !death[now]) printf("即将轮到你的回合......");
						if(now!=server_mode and now<=2)
						{
							if(mode[6]==0) printf("你的回合即将结束......");
							if(mode[6]==1) printf("即将轮到P%d的回合......",3-server_mode);
						}
						if(now==3) printf("即将轮到BOSS的回合......");
						Sleep(1500);while(_kbhit()) getch();
						appcnt=0;cls();
					}
					else if(winner==now){
						cls();
						SetPos(13,0);
						SetColor(6);
						printf("你们最终被击败了......");
						now=to;
						if(send_gaming(void_card)<0) {
							another_player_quit(server_mode); 
							break;
						}
						Sleep(500);
					}
				}
			}
		}
	}
	else {
		pregame();

		if(!mode[1]){
			mode[1]=rad()%2+1;
		}//随机先手
		int now=mode[1];
		adv(3-mode[1]);//后手补偿
		env_now=0;

		char title[]="CARD GAME:Turn of P1  Turn:   ";
		title[19]=now+'0';
		title[27]=turn/100+'0';
		title[28]=turn/10%10+'0';
		title[29]=turn%10+'0';
		SetConsoleTitle(title);
		while(!winner && !Check(now)){
			turn++;
			title[19]=now+'0';
			title[27]=turn/100+'0';
			title[28]=turn/10%10+'0';
			title[29]=turn%10+'0';
			SetConsoleTitle(title);
			UI();
			if(Ask_same(now)==1) break;
			end_turn(now,3-now);
			now=3-now;cls();
		}
	}
	return 0;
}

int main(){
	cls();
	srand(time(NULL));
	previous();//获得公共牌库和职业牌库
	if(rad()%100<3) Tysta=1;
	while(1){
		mouse(0);
		server_mode=0;
		prepare();
		SetConsoleTitle(("CARD GAME:v"+version).c_str());
		bool connect_established=0;
		while(!connect_established){
			connect_established=1;
			cls();
			Connect();
			if(server_mode==3 || server_mode==4 || server_mode==5 || server_mode==6) break;
			SetColor(7,0);
			if(server_mode==2) WSAstart(),SetPos(26,4),printf("输入服务端ip地址:");
			else {
				WSAstart();
				char hostname[256]={0},ip[256]={0};
				gethostname(hostname,sizeof(hostname));
				HOSTENT* host=gethostbyname(hostname);
				strcpy(ip,inet_ntoa(*(in_addr*)*host->h_addr_list));
				SetPos(26,3),printf("你的ip地址是:%s",ip);
				SetPos(26,4),printf("等待玩家连入中...");
			}
			if(TCP_initialize(server_mode)!=0){
				connect_established=0;
				cls();
				if(server_mode==1){
					SetColor(6);
					printf("无法设立服务端...请检查是否有同一程序正在运行或者稍后再试...\n");
					getch();
				}
				else{
					SetColor(6);
					printf("无法连接至服务器...请检查网络状况及服务器是否正常启动...\n");
					getch();
				}
			}
		}
		if(server_mode==4){
			FAQ();
			continue;
		}
		if(server_mode==5) break;
		if(server_mode!=3){
			if(server_mode==1){
				cls();
				int qwq1,qwq2,qwq3;
				recv_int(qwq1),recv_int(qwq2),recv_int(qwq3);
				send_int(version1),send_int(version2),send_int(version3);
				if(version1!=qwq1 or version2!=qwq2 or version3!=qwq3)
				{
					SetColor(6);
					printf("双方版本不同...无法进行游戏...");
					getch();continue;
				}
				
				SetConsoleTitle("CARD GAME:Options");
				if(Options(1)) continue;
				while(_kbhit()) getch();
				SetColor(0,7);
				system("color 07");
				cls();//清屏并切换到Choose界面
			}
			else {
				cls();
				int qwq1,qwq2,qwq3;
				send_int(version1),send_int(version2),send_int(version3);
				recv_int(qwq1),recv_int(qwq2),recv_int(qwq3);
				if(version1!=qwq1 or version2!=qwq2 or version3!=qwq3)
				{
					SetColor(6);
					printf("双方版本不同...无法进行游戏...");
					getch();continue;
				}
				
				if(Options(1)) continue;
				while(_kbhit()) getch();
			}
			while(_kbhit()) getch();
			cls();
		}
		else {
			SetColor(7,0);
			cls();
			SetConsoleTitle("CARD GAME:Options");
			Options(0);
			SetColor(0,7);
			system("color 07");
			cls();//清屏并切换到Choose界面
		}
		while(_kbhit()) getch();
		game();
		Sleep(1200);
		cls();
		UI();
		showresult();
	}
	SetColor(7,0);
	return 0;
}

