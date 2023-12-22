#include<bits/stdc++.h>
#include<windows.h>
#include<conio.h> 
using namespace std;

int version1=3;
int version2=3;
int version3=1;
string version="3.3.1";

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
inline bool checkq(char c){return c=='q' or c=='Q';}
inline bool checkt(char c){return c=='t' or c=='T';}

#define MAXAPCARDNUM 45
//ASCII 

#include"CARDGAMEsocket.hpp"

//-------------------------------------------------
int UI(int now);
int Sort(int now);

int Check(int now){
	if(pl[now].hp<1){
		winner=3-now;
		return 1;
	}
	return 0;
}

int Card::cal_atk(int from,int to){//����ʵ�ʹ�����
	double damage=ATK;
	if(func==121) return damage;//[��ʹ92]�̶� 
	if(func==15) ;
	else if(func==89) damage+=pl[from].buff[0]*1.5;//[ָ�ӽ���]����buff
	else
	{
		if(pl[from].occ==2 && ATK>0) damage=ATK+5*pl[from].buff[0];//��ʿ
		if(pl[from].occ==5 && ATK>0) damage=ATK+ATK*pl[from].buff[0]*0.09;//�ؾ�
		if(pl[from].occ==4 && pl[from].buff[0])//սʿ
			damage-=0.04*damage*pl[from].buff[0];
		if(func==115 and pl[from].occ==10 and pl[from].buff[0]%11==0) damage*=2;
		//ְҵӰ��
		if(pl[from].buff[3])//��
			damage*=2;
		if(pl[from].buff[4])//����
			damage*=0.7;
		//BuffӰ��
		if(env_now==1)//Ѫ��
			damage*=1.2;
		if(env_now==2)//����
			damage*=0.8;
		//����Ӱ��	
	}
	if(mode[4]) //���buff
		damage*=0.7;
	if(mode[5]) //��ʧ�� 
		damage*=1.5; 
	
	return (int)damage;
}

int Card::cal_heal(int from,int to){//����ʵ�ʻָ���
	double heal=HEAL;
	if(pl[from].buff[4])//����
		heal*=0.7;
	if(mode[4]) //���buff
		heal*=0.7;
	return heal;
}

int Card::cal_def(int from,int to){//����ʵ�ʵ���
	double def=DEF;
	if(mode[4]) //���buff
		def*=0.7;
	return def;
}

int Card::cal_miss(int from,int to){
	int miss=MISS;
	if(110<=func and func<=114) return miss;
	if(func==121) return miss;
	if(func==122) return miss;
	if(env_now==3)
		miss+=10;
	if(env_now==4)
		miss-=10;
	if(pl[from].buff[6])//�Ի�
		miss+=25;
	if(pl[to].occ==10)
		miss+=5;
	if(mode[5]) //��ʧ�� 
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
	if(rad()%100<miss && id!=119){
		SetPos(0,1);
		printf("����ʧ����!");
		if(pl[from].occ==4 && ATK>0) pl[from].buff[0]+=2;
		if(pl[from].occ==10) pl[from].hp=min(pl[from].hp+15*(cost+1),pl[from].maxhp);
		pl[from].cost=min(pl[from].maxcost,pl[from].cost+cost/2);
		UI(from);
		use_card.MISS=1;
		return 0;
	}//ʧЧ�ж�
	int damage,flag=0;
	if(func) flag=Special(from,to);
	if(flag==4)
	{
		for(int i=1;i<=2;i++)
			if(pl[i].occ==10) pl[i].buff[0]=rad()%100; 
		UI(from);
		return 0;
	}
	damage=use_card.ATK=cal_atk(from,to);
	
	if(pl[from].occ==11 and damage!=0 and pl[from].buff[0]>=5) pl[to].buff[4]+=1,pl[from].buff[0]-=5; 
	
	//���ܵ���
	if(pl[to].def>0 && flag!=1){
		if(pl[to].occ==9)
		{
			int sum=min(damage,pl[to].def);
			if(pl[3-to].occ!=8) pl[3-to].hp-=sum*pl[to].buff[0]/10.0;
			else
			{
				int defdamage=sum*pl[to].buff[0]/10.0;
				if(defdamage<=pl[3-to].buff[0]*3) pl[3-to].buff[0]-=defdamage/3,defdamage=0;
				else defdamage-=pl[3-to].buff[0]*3,pl[3-to].buff[0]=0;
				if(defdamage>0) pl[3-to].hp-=defdamage;
			}
		}
		if(damage>=pl[to].def){
			damage-=pl[to].def,pl[to].def=0;
			Shake(3,1);//���ƻ��ܵĶ���
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
	if(damage>=150){//�ػ�����
		if(damage>=3000) system("color E7");
		else system("color 47");
		Shake(10,1);
		system("color 07");
		cls();
		UI(from);
	}
	//����
	int heal=cal_heal(from,to);
	use_card.HEAL=heal;
	pl[from].hp=min(pl[from].maxhp,pl[from].hp+heal);
	int def=cal_def(from,to);
	use_card.DEF=def;
	if(pl[from].occ==9) 
		if(pl[from].def+def>pl[from].maxdef) 
		{
			int defdamage=(pl[from].def+def-pl[from].maxdef)*(10-pl[from].buff[0])/10.0;
			if(pl[3-from].occ!=8) pl[3-from].hp-=defdamage;
			else
			{
				if(defdamage<=pl[3-from].buff[0]*3) pl[3-from].buff[0]-=defdamage/3,defdamage=0;
				else defdamage-=pl[3-from].buff[0]*3,pl[3-from].buff[0]=0;
				if(defdamage>0) pl[3-from].hp-=defdamage;	
			}
		}
	pl[from].def=min(pl[from].maxdef,pl[from].def+def);
	//�׶����Ѫ
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
	if(pl[from].occ==10 and (func!=109 and func!=117 and func!=121)) pl[from].buff[0]=rad()%100; 
	if(pl[to].occ==10) pl[to].buff[0]=rad()%10;
	UI(from);
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
				if(pl[3-to].occ!=8) pl[3-to].hp-=defdamage;
				else
				{
					if(defdamage<=pl[3-to].buff[0]*3) pl[3-to].buff[0]-=defdamage/3,defdamage=0;
					else defdamage-=pl[3-to].buff[0]*3,pl[3-to].buff[0]=0;
					if(defdamage>0) pl[3-to].hp-=defdamage;	
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
		for(int i=1;i<=10;i++) pl[from].buff[i]=0;
	}
	else if(func==32){
		for(int i=1;i<=pl[from].cardcnt;i++){
			if(pl[from].used[i]) continue;
			if(pl[from].handcard[i].id==98){ //98-->[�������]
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
		int x=rad()%pl[to].cardcnt+1;//ѡȡ�����λ
		pl[to].handcard[x]=lib[7][10];//[����ˮ��]
		if(pl[to].used[x]) pl[to].used[x]=0;
	}
	else if(func==47){
		vector<int> qwq;qwq.clear();
		for(int i=1;i<=pl[to].cardcnt;i++)
			if(!pl[to].used[i]) qwq.push_back(i);
		if(!qwq.empty())
		{
			int x=qwq[rad()%(int)qwq.size()];//ѡȡ�����λ
			pl[to].handcard[x].cost+=1;
		}
	}
	else if(func==48){
		for(int i=1;i<=pl[to].cardcnt;i++){
			if(pl[to].used[i]) continue;
			if(pl[to].handcard[i].func==42) pl[to].used[i]=1,pl[to].buff[0]+=2; //42-->[��ʥ��־]
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
		pl[to].buff[rad()%6+1]+=rad()%2+1;
	}
	else if(func==52){
		pl[from].buff[rad()%6+1]+=rad()%2+1;
	}
	else if(func==53){
		pl[rad()%2+1].buff[rad()%6+1]+=rad()%2+1;
	}
	else if(func==54){
		pl[to].buff[rad()%6+1]+=rad()%2+1;
		pl[from].buff[rad()%6+1]+=rad()%2+1;
	}
	else if(func==55){
		int x=rad()%pl[to].cardcnt+1,y=rad()%pl[to].cardcnt+1;//ѡȡ�����λ
		while(x==y) x=rad()%pl[to].cardcnt+1,y=rad()%pl[to].cardcnt+1;//ȥ��
		pl[to].handcard[x]=pl[to].handcard[y]=lib[7][libcnt[7]+3];
		pl[to].used[x]=pl[to].used[y]=0;
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
			while(pl[from].handcard[i].id==98 && i<=pl[from].heapn){ //98-->[�������]
				pl[from].heap[i]=pl[from].heap[pl[from].heapn--];
			}
		}
	}
	else if(func==60){
		pl[from].cost=min(pl[from].cost+2,pl[from].maxcost);
		pl[from].used[++pl[from].cardcnt]=1;
		for(int i=1;i<=pl[from].heapn;i++){
			if(pl[from].heap[i].id==17) { //17-->[������]
				pl[from].heap[i].id=-17;
				pl[from].heap[i].cost=2;
				pl[from].heap[i].ATK=20;
			}
			if(pl[from].heap[i].id==21) { //21-->[���ܿ�ŭ]
				pl[from].heap[i].id=-21;
				pl[from].heap[i].ATK=20;
			}
			if(pl[from].heap[i].id==25) { //25-->[������]
				pl[from].heap[i].id=-25;
				pl[from].heap[i].HEAL=90;
			}
			if(pl[from].heap[i].id==19) { //19-->[���ݹ�Ӯ]
				pl[from].heap[i].id=-19;
				pl[from].heap[i].cost=0;
			}
			if(pl[from].heap[i].id==23) { //23-->[��������]
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
			while(pl[from].heap[i].id==81 && i<=pl[from].heapn){ //81-->[��׸]
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
		pl[to].handcard[x]=fun[1][7][2],pl[to].used[x]=0;
	}
	else if(func==66){
		if(rad()%100<30){
			pl[from].buff[0]+=1;
		}
		for(int i=1;i<=pl[from].heapn;i++){
			if(pl[from].heap[i].id==54) pl[from].heap[i].ATK+=15; //54-->[�����ɳ�]
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
			for(int i=1;i<=6;i++) pl[from].buff[i]=pl[to].buff[i]=0;
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
		pl[from].buff[0]=min(7,pl[from].buff[0]+2);
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
				if(pl[3-from].occ!=8) pl[3-from].hp-=defdamage;
				else
				{
					if(defdamage<=pl[3-from].buff[0]*3) pl[3-from].buff[0]-=defdamage/3,defdamage=0;
					else defdamage-=pl[3-from].buff[0]*3,pl[3-from].buff[0]=0;
					if(defdamage>0) pl[3-from].hp-=defdamage;	
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
				if(pl[3-from].occ!=8) pl[3-from].hp-=defdamage;
				else
				{
					if(defdamage<=pl[3-from].buff[0]*3) pl[3-from].buff[0]-=defdamage/3,defdamage=0;
					else defdamage-=pl[3-from].buff[0]*3,pl[3-from].buff[0]=0;
					if(defdamage>0) pl[3-from].hp-=defdamage;	
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
				if(pl[3-from].occ!=8) pl[3-from].hp-=defdamage;
				else
				{
					if(defdamage<=pl[3-from].buff[0]*3) pl[3-from].buff[0]-=defdamage/3,defdamage=0;
					else defdamage-=pl[3-from].buff[0]*3,pl[3-from].buff[0]=0;
					if(defdamage>0) pl[3-from].hp-=defdamage;	
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
	}
	else if(func==120){
		for(int i=1;i<=pl[to].cardcnt;i++)
			if(!pl[to].used[i]) {
				int func2=pl[to].handcard[i].func;
				if((110<=func2 and func2<=114) or func2==121 or func2==122) continue;
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
		pl[from].buff[0]+=2;
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
			if(pl[from].used[i]==0) pl[from].handcard[i].ATK+=pl[from].buff[0]*4;
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
		pl[from].heap[++pl[from].heapn]=lib[11][6];
	}
	else if(func==138){
		pl[from].buff[0]=0; 
	}
	else if(func==139){
		pl[to].buff[4]+=3;
	}
	else if(func==140){
		pl[to].buff[6]=1;
	}
	else if(func==141){
		pl[from].buff[4]=0;
	} 
	else if(func==142){
		pl[from].heap[++pl[from].heapn]=lib[11][6];
	}
	else if(func==143){
		pl[from].buff[4]/=2;
		pl[to].cost=min(pl[to].maxcost,pl[to].cost+2);
	}
	return 0;
}

void extra(int now,int cursor){
	if(pl[now].handcard[cursor].func==37 || pl[now].handcard[cursor].func==67 || pl[now].handcard[cursor].func==3){
		pl[now].used[cursor]=0;
		pl[now].rest++;
		Card x=pl[now].heap[(rad()%pl[now].heapn)+1];
		while(x.id==pl[now].handcard[cursor].id) x=pl[now].heap[(rad()%pl[now].heapn)+1];
		pl[now].handcard[cursor]=x;
	}//[��������],[Ѫɫŭ��],[���ݹ�Ӯ]����
	if(pl[now].handcard[cursor].func==60){
		pl[now].used[cursor]=0;
		pl[now].rest++;
		pl[now].handcard[cursor]=lib[1][7];
	}//[����ʰ��]��Ϊ[��������]
	if(pl[now].handcard[cursor].func==96){
		pl[now].used[cursor]=0;
		pl[now].rest++;
		pl[now].handcard[cursor]=lib[8][11];
	}//[�����]��Ϊ[ˮ������]
	if(pl[now].handcard[cursor].func==104){
		pl[now].used[cursor]=0;
		pl[now].rest++;
		pl[now].handcard[cursor]=lib[9][11];
	}//[�ػ��ߵ�����]��Ϊ[ʥ����̬]
	if(pl[now].handcard[cursor].func==24){
		for(int i=1;i<=pl[now].cardcnt;i++) {
			pl[now].handcard[i]=pl[now].heap[rad()%pl[now].heapn+1];
			pl[now].used[i]=0;
		}
		pl[now].rest=pl[now].cardcnt;
	}//[����]����
	if(pl[now].handcard[cursor].func==82){
		for(int i=1;i<=pl[now].cardcnt;i++) {
			if(pl[now].used[i]) continue;
			if(pl[now].handcard[i].HEAL!=0) pl[now].handcard[i].ATK+=pl[now].handcard[i].HEAL;
			else pl[now].handcard[i].ATK+=15;
			pl[now].handcard[i].HEAL=0;
		}
	}//[��Ӱ��̬]����
	if(pl[now].handcard[cursor].func==44){
		int x=rad()%pl[now].cardcnt+1;//ѡȡ�����λ
		if(pl[now].used[x]){
			pl[now].rest++;
			pl[now].used[x]=0;
		}
		pl[now].handcard[x]=lib[7][libcnt[7]+1];
	}
	if(pl[now].handcard[cursor].func==90){
		pl[now].buff[0]/=2;
	}
}

bool giveupcard(int now,int cursor){
	if(pl[now].handcard[cursor].func==65) {
		pl[now].buff[4]+=2;
		return 1;
	}//[�������]
	if(pl[now].handcard[cursor].func==94) {
		pl[3-now].def+=10;
		if(pl[3-now].occ==9) 
			if(pl[3-now].def>pl[3-now].maxdef) 
			{
				int defdamage=(pl[3-now].def-pl[3-now].maxdef)*(10-pl[3-now].buff[0])/10.0;
				if(pl[now].occ!=8) pl[now].hp-=defdamage;
				else
				{
					if(defdamage<=pl[now].buff[0]*3) pl[now].buff[0]-=defdamage/3,defdamage=0;
					else defdamage-=pl[now].buff[0]*3,pl[now].buff[0]=0;
					if(defdamage>0) pl[now].hp-=defdamage;	
				}
			}
		pl[3-now].def=min(pl[3-now].maxdef,pl[3-now].def);
		return 1;
	}//[����]
	return 0;
}
				
void init(int x){
	pl[x].maxhp=job[pl[x].occ].maxhp;
	pl[x].cardcnt=job[pl[x].occ].cardcnt;
	pl[x].maxdef=job[pl[x].occ].maxdef;
	pl[x].maxcost=job[pl[x].occ].maxcost;
	pl[x].hp=pl[x].maxhp;
	//���ְҵ������Ϣ
	int tot=libcnt[0];
	for(int i=1;i<=tot;i++){
		pl[x].heap[i]=lib[0][i];
		if(pl[x].occ==3 && pl[x].heap[i].HEAL>=75) i--,tot--; //��ʦ���ܳ鹫���ƿ�HEAL>=75����
		if(pl[x].occ==4 && pl[x].heap[i].HEAL>0) i--,tot--;//սʿ���ܳ鹫���ƿ�HEAL��
		if(pl[x].occ==5 || pl[x].occ==6 || pl[x].occ==8 || pl[x].occ==9 || pl[x].occ==11) i--,tot--;//�ؾ�����ħ�����ˡ����������Ͳ��ܳ鹫���ƿ����
		if(pl[x].occ==7 && pl[x].heap[i].ATK>=80) i--,tot--;//��ʦ���ܳ鹫���ƿ�ATK>=80����
		if(pl[x].occ==10 && pl[x].heap[i].cost!=0) i--,tot--;//��ͽֻ�ܳ鹫���ƿ����=0���� 
	}
	pl[x].heapn=tot;
	tot=libcnt[pl[x].occ];
	for(int i=1;i<=tot;i++) {
		pl[x].heap[i+pl[x].heapn]=lib[pl[x].occ][i];
	}
	pl[x].heapn+=tot;
	if(mode[4]){//����ģʽ�����buff
		tot=funcnt[2][0];
		for(int i=1;i<=tot;i++) pl[x].heap[i+pl[x].heapn]=fun[2][0][i];
		pl[x].heapn+=tot;
	}
}
void treasure(int now){//����ģʽ��������
	if(gettre[now]) return;
	if(pl[now].occ==1 && pl[now].buff[0]>=3)
		for(int i=1;i<=pl[now].cardcnt;i++){
			if(pl[now].used[i]) {
				pl[now].used[i]=0;
				gettre[now]=1;
				pl[now].handcard[i]=fun[1][1][1];
				break;
			}
		}
	if(pl[now].occ==2 && pl[now].maxhp<=500)
		for(int i=1;i<=pl[now].cardcnt;i++){
			if(pl[now].used[i]) {
				pl[now].used[i]=0;
				gettre[now]=1;
				pl[now].handcard[i]=fun[1][2][1];
				break;
			}
		}
	if(pl[now].occ==3 && pl[3-now].hp*10<pl[3-now].maxhp*6)
		for(int i=1;i<=pl[now].cardcnt;i++){
			if(pl[now].used[i]) {
				pl[now].used[i]=0;
				gettre[now]=1;
				pl[now].handcard[i]=fun[1][3][1];
				break;
			}
		}
	if(pl[now].occ==4 && pl[now].buff[0]>=6)
		for(int i=1;i<=pl[now].cardcnt;i++){
			if(pl[now].used[i]) {
				pl[now].used[i]=0;
				gettre[now]=1;
				pl[now].handcard[i]=fun[1][4][1];
				break;
			}
		}
	if(pl[now].occ==5 && pl[now].maxhp>=350)
		for(int i=1;i<=pl[now].cardcnt;i++){
			if(pl[now].used[i]) {
				pl[now].used[i]=0;
				gettre[now]=1;
				pl[now].handcard[i]=fun[1][5][1];
				break;
			}
		}
	if(pl[now].occ==6 && pl[now].buff[0]>=5)
		for(int i=1;i<=pl[now].cardcnt;i++){
			if(pl[now].used[i]) {
				pl[now].used[i]=0;
				gettre[now]=1;
				pl[now].handcard[i]=fun[1][6][1];
				break;
			}
		}
	if(pl[now].occ==7 && pl[now].buff[0]>=4 && pl[now].cost>=3)
		for(int i=1;i<=pl[now].cardcnt;i++){
			if(pl[now].used[i]) {
				pl[now].used[i]=0;
				gettre[now]=1;
				pl[now].handcard[i]=fun[1][7][1];
				break;
			}
		}
	if(pl[now].occ==8 && (pl[now].hp<=120 || pl[now].buff[0]>=24))
		for(int i=1;i<=pl[now].cardcnt;i++){
			if(pl[now].used[i]) {
				pl[now].used[i]=0;
				gettre[now]=1;
				pl[now].handcard[i]=fun[1][8][1];
				break;
			}
		}
	if(pl[now].occ==9 && pl[now].maxdef>=300)
		for(int i=1;i<=pl[now].cardcnt;i++){
			if(pl[now].used[i]) {
				pl[now].used[i]=0;
				gettre[now]=1;
				pl[now].handcard[i]=fun[1][9][1];
				break;
			}
		}
	if(pl[now].occ==10 && pl[now].hp<=200)
		for(int i=1;i<=pl[now].cardcnt;i++){
			if(pl[now].used[i]) {
				pl[now].used[i]=0;
				gettre[now]=1;
				pl[now].handcard[i]=fun[1][10][1];
				break;
			}
		}
	if(pl[now].occ==11 && pl[now].buff[0]>=15)
		for(int i=1;i<=pl[now].cardcnt;i++){
			if(pl[now].used[i]) {
				pl[now].used[i]=0;
				gettre[now]=1;
				pl[now].handcard[i]=fun[1][11][1];
				break;
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
	return;
}

void Choose(int now){
	SetColor(7);
	int cursor=1,lastcursor=1;
	SetPos(6,5);
	printf("��ѡ��P%d",now);printf("��ְҵ��");
	for(int i=1;i<=sumjob+1;i++){
		SetColor(7,0);SetPos(4,i+6);printf("%2d      ",i);printf(occ_name(i));printf("   ");
	}//��ְҵ���ֺͼ��չʾ
	while(1)
	{
		SetPos(9,lastcursor+6);SetColor(7,0);printf("   ");printf(occ_name(lastcursor));printf("   ");
		SetPos(9,cursor+6);SetColor(0,7);printf("   ");printf(occ_name(cursor));printf("   ");
		
		occ_func(cursor);
		if(mode[3]){
			SetPos(26,17);
			occ_treasure(cursor);
		}
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

int UI(int now){
	if(server_mode!=3) Con::show(); 
	for(int rnk=1;rnk<=2;rnk++){
		int i;
		//���/����/ְҵ
		SetColor(15);
		SetPos(5,rnk*4-2);
		printf("#%d ",rnk);
		printf(pl[rnk].name);
		SetColor(7);
		SetPos(5,rnk*4+1-2);
		printf("   ");
		printf(occ_name(pl[rnk].occ));
		
		//buff0
		if(pl[rnk].occ>=2){
			SetColor(13);
			SetPos(8,rnk*4);
			if(pl[rnk].occ!=10) printf("��%d   ",pl[rnk].buff[0]);
			if(pl[rnk].occ==10)
			{
				if(pl[rnk].buff[0]<10) printf("��0%d   ",pl[rnk].buff[0]);
				else 
				{
					if(pl[rnk].buff[0]==92) SetColor(14);
					printf("��%d   ",pl[rnk].buff[0]);
				}
			}
		}

		//���û���
		SetColor(11);
		SetPos(17,rnk*4-2);
		printf("%d/%d ",pl[rnk].cost,pl[rnk].maxcost);
		SetPos(26,rnk*4-2);
		for(i=1;i<=pl[rnk].cost;i++)
			printf("��");
		for(;i<=pl[rnk].maxcost;i++)
			printf("��");
		printf("                                   ");
		
		//Ѫ������
		if(pl[rnk].hprate()>5)SetColor(10);
		else if(pl[rnk].hprate()>2)SetColor(6);
		else SetColor(12);
		SetPos(17,rnk*4+1-2);
		printf("%d/%d            ",pl[rnk].hp,pl[rnk].maxhp);

		//���׻���
		SetColor(15);
		SetPos(17,rnk*4+2-2);
		printf("[%d/%d]            ",pl[rnk].def,pl[rnk].maxdef);
		
		//Buff����
		SetPos(26,rnk*4+2-2);
		int qwqcnt=0;
		if(pl[rnk].buff[1])
			SetColor(6),printf("ȼ�� %d : ",pl[rnk].buff[1]);
		else qwqcnt++;
		if(pl[rnk].buff[2])
			SetColor(2),printf("�ж� %d : ",pl[rnk].buff[2]);
		else qwqcnt++;
		if(pl[rnk].buff[3])
			SetColor(4),printf("�� %d : ",pl[rnk].buff[3]);
		else qwqcnt++;
		if(pl[rnk].buff[4])
			SetColor(11),printf("���� %d : ",pl[rnk].buff[4]);
		else qwqcnt++;
		if(pl[rnk].buff[5])
			SetColor(10),printf("���� %d : ",pl[rnk].buff[5]);
		else qwqcnt++;
		if(pl[rnk].buff[6])
			SetColor(5),printf("�Ի� %d : ",pl[rnk].buff[6]);
		else qwqcnt++;
		printf(string(9*qwqcnt,' '));
		
		//Ѫ������
		SetPos(26,rnk*4+1-2);
		if(pl[rnk].hprate()>5)SetColor(10);
		else if(pl[rnk].hprate()>2)SetColor(6);
		else SetColor(12);
		i=1;
		if(pl[rnk].hp>0) for(;i<=(pl[rnk].hp-1)/40+1;i++)
			printf("��");
		SetColor(8);
		for(;i<=(pl[rnk].prehp-1)/40+1;i++)
			printf("��");
		if(pl[rnk].hprate()>5)SetColor(10);
		else if(pl[rnk].hprate()>2)SetColor(6);
		else SetColor(12);
		for(;i<=(pl[rnk].maxhp-1)/40+1;i++)
			printf("��");
		SetColor(7);
		if(pl[rnk].def) for(i=1;i<=(pl[rnk].def-1)/30+1;i++)
			printf("��");
		for(i=1;i<=36-(pl[rnk].maxhp-1)/40-(pl[rnk].def-1)/30-2;i++)
		printf(" ");
	}

	//��ʾ����
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

void UI_other(){
	printf("                      ");
	SetColor(7);
	SetPos(0,Row-1);
	printf("P%d",now); 
	SetPos(5,Row-1);
	printf("  ʣ������:%d/%d",pl[3-server_mode].rest,pl[3-server_mode].cardcnt); 
	if(pl[3-server_mode].occ==7){
		SetColor(6);
		SetPos(30,Row-1);
		printf("  [��ʥ��־]:%d/%d",pl[3-server_mode].buff[10],pl[3-server_mode].cardcnt); 
	}
	SetColor(7);
	SetPos(5,Row);
	printf("  #"); 
	SetPos(11,Row);SetColor(11);
	printf("��");
	SetPos(20+14,Row);SetColor(7);
	printf("ATK");
	SetPos(30+14,Row);
	printf("HEAL");
	SetPos(40+14,Row);
	printf("DEF");
	SetPos(50+14,Row);
	printf("MISS");
	while(appcnt){
		if(appcard[appcnt].func==-2) appcnt--;
		else break;
	}
	for(int i=1;i<=min(appcnt,6);i++) {
		int x=i+max(0,appcnt-6);
		if(i==min(appcnt,6)) {
			SetColor(11,0);
			SetPos(11,Row+i);
			printf("%d",appcard[x].cost);//����
			SetColor(0,8);
			SetPos(14,Row+i);
			printf(appcard[x].Name());//����
//			SetColor(0,8);
			printf("                 ");
			SetPos(20+14,Row+i);
			printf("%-3d         ",appcard[x].ATK);
			SetPos(30+14,Row+i);
			printf("%-3d         ",appcard[x].HEAL);
			SetPos(40+14,Row+i);
			printf("%-3d         ",appcard[x].DEF);
			SetPos(50+14,Row+i);
			if(appcard[x].MISS==1) {
				SetColor(10,8);
				printf("YES");//MISS
			}
			else if(appcard[x].MISS==2) {
				SetColor(13,8);
				printf("DIS");//give up
			}
			else {
				SetColor(4,8);
				printf("NO ");
			}
			SetColor(7,0);
			SetPos(11,Row+9);
			printf("                                                                    ");
			SetPos(11,Row+9);
			printf(appcard[x].Intro());
		}
		else {
			SetColor(11);
			SetPos(11,Row+i);
			printf("%d",appcard[x].cost);//����
			SetColor(7);
			SetPos(14,Row+i);
			printf(appcard[x].Name());//����
			SetColor(7,0);
			printf("               ");
			SetPos(20+14,Row+i);
			printf("%-3d         ",appcard[x].ATK);
			SetPos(30+14,Row+i);
			printf("%-3d         ",appcard[x].HEAL);
			SetPos(40+14,Row+i);
			printf("%-3d         ",appcard[x].DEF);
			SetPos(50+14,Row+i);
			if(appcard[x].MISS==1) {
				SetColor(10);
				printf("YES");//MISS
			}
			else if(appcard[x].MISS==2) {
				SetColor(13);
				printf("DIS");//give up
			}
			else {
				SetColor(4);
				printf("NO ");
			}
			SetColor(7,0);
			SetPos(11,Row+9);
			printf("                                                                    ");
			SetPos(11,Row+9);
			printf(appcard[x].Intro());
		}
	}
	mouse(0);
}

void start_turn(int now){
	for(int i=1;i<=2;i++) pl[i].prehp=pl[i].hp;//��¼�غϿ�ʼѪ��
	//ְҵ����
	if(pl[now].occ==1) {
		if(pl[now].cost==0){
			if(rad()%100<80) {
				pl[now].cost++;//����[ʰ��]
				pl[now].buff[0]++;
			}
		}
	}
	if(pl[now].occ==3){
		pl[now].hp=min(pl[now].maxhp,pl[now].hp+8*pl[now].buff[0]);
		pl[now].cost=min(pl[now].maxcost,pl[now].cost+pl[now].buff[0]);
		pl[now].buff[0]=0;
		if(rad()%100<45) pl[now].buff[0]++;
	}//��ʦ<�﷨��>��ǻ�÷���
	if(pl[now].occ==4) {
		if(pl[now].def==0) pl[now].def=30;
		pl[now].buff[0]=max(0,pl[now].buff[0]-1);
	}//սʿ���ײ����<��ƣ��>
	if(pl[now].occ==5){
		pl[now].cost=min(pl[now].cost+2,pl[now].maxcost);//�ؾ�ÿ�غ�3��
	}
	if(pl[now].occ==6){
		pl[now].cost=min(pl[now].cost+1,pl[now].maxcost);
		int da=5*pl[now].buff[0];
		if(da<pl[now].def) pl[now].def-=da;
		else {
			if(pl[now].def>0) Shake(3,1);
			pl[now].hp-=da-pl[now].def,pl[now].def=0;
		}
	}//��ħ<��ԭ��>���Լ�����
	if(pl[now].occ==8){
		pl[now].hp-=15;
		pl[now].buff[0]+=10;
	}//���˿�15Ѫ���10<������>
	if(pl[now].occ==10){
		pl[now].hp=max(pl[now].hp,pl[now].buff[0]);
	}//��ͽHP�ظ���<�����> 
	if(pl[now].occ==11){
		pl[now].hp=min(pl[now].maxhp,pl[now].hp+min(30,pl[now].buff[0])); 
	}//���ͻظ�<�｣��>HP
	
	if(env_now==7)
		pl[now].cost=min(pl[now].cost+1,pl[now].maxcost);//�����ӷ� 
	
	pl[now].cost=min(pl[now].cost+1,pl[now].maxcost);//�ӷ� 
	if(pl[now].buff[5])//����buff
		pl[now].hp=min(pl[now].hp+30,pl[now].maxhp);
	if(pl[now].buff[1])//ȼ��buff
		pl[now].hp-=40;
	if(pl[now].buff[2])//�ж�buff
		pl[now].hp-=pl[now].hp*0.2;
	pl[now].UpdateBuff(1);
	//buffǰ��

	if(mode[4]){
		pl[now].buff[rad()%6+1]++;
	}//����ģʽ�����buff

	if(env_now==5){
		pl[now].hp=min(pl[now].maxhp,(int)(pl[now].hp+pl[now].maxhp*0.05));
	}//�ʺ�
	if(env_now==6){
		pl[now].hp=(pl[now].hp-pl[now].maxhp*0.05);
	}//����
	//�����ж�
}

void another_player_quit(int server_mode){
	cls();
	SetPos(11,0);
	printf("�Է��˳�����Ϸ����Ҳ����Ͷ����...\n");
	Sleep(1500);
}
int Ask(int now){
	int option_use=0,option_giveup=0,option_over=0,option_quit=0;
	start_turn(now);
	if(mode[3]) treasure(now);
	UI(now);
	if(Check(now)){send_gaming(void_card); return 1;}
	//�����ж�

	for(int i=1;i<=pl[now].cardcnt;i++){
		if(pl[now].used[i]){
			pl[now].used[i]=0;
			if(pl[now].occ==7 && rad()%100<pl[now].buff[0]*2){
				pl[now].handcard[i]=lib[7][libcnt[7]+1];
			}
			else{
				pl[now].handcard[i]=pl[now].heap[(rad()%pl[now].heapn)+1];
			}
		}
	}
	pl[now].rest=pl[now].cardcnt;
	//��������
	if(pl[now].occ==7){
		pl[now].buff[10]=0;
		for(int i=1;i<=pl[now].cardcnt;i++) 
			if(!pl[now].used[i] && pl[now].handcard[i].func==42) pl[now].buff[10]++; //42-->[��ʥ��־]
	}//��ʦ����[��ʥ��־]
	use_card=(Card){0,0,0,0,0,-2,0};
	if(send_gaming(use_card)<0) {
		another_player_quit(server_mode); 
		winner=server_mode;
		return 1;
	}
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
		printf("��");
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
			if(cursor==i && !option_giveup && !option_use) SetColor(color=15),printf("��"); 
			else if(cursor==i && option_giveup) SetColor(color=15),printf("��"); 
			else if(cursor==i && option_use) SetColor(color=15),printf("��"); 
			else SetColor(color=7),printf("  "); 
			if(pl[now].occ==10 and pl[now].handcard[i].func==121 and pl[now].buff[0]==92) color--;
			SetPos(7,Row+i),printf("%d",i); 
			if(pl[now].used[i]){
				SetColor(8);
				printf("                                                                     ");
				SetPos(14,Row+i);
				printf("[Used.]");
				continue;
			}

			SetPos(11,Row+i);
			if(pl[now].handcard[i].func==15) {
				pl[now].handcard[i].ATK=pl[now].cost*40+25;
				pl[now].handcard[i].cost=pl[now].cost;
			}//[��ˮһս]
			if(pl[now].handcard[i].func==32){
				int da=0;
				for(int i=1;i<=pl[now].cardcnt;i++){
					if(pl[now].used[i]) continue;
					if(pl[now].handcard[i].id==98) da+=60; //98-->[�������]
				}
				pl[now].handcard[i].ATK=da;
			}
			if(pl[now].handcard[i].func==90){
				pl[now].handcard[i].HEAL=pl[now].buff[0]*2;
			}
			if(pl[now].handcard[i].func==102){
				pl[now].handcard[i].ATK=pl[now].def*1.75;
			}
			if(pl[now].handcard[i].func==110){
				if(pl[now].buff[0]%10==6 or pl[now].buff[0]/10==6) pl[now].handcard[i].MISS=0;
				else pl[now].handcard[i].MISS=100;
			}
			if(pl[now].handcard[i].func==111){
				if(pl[now].buff[0]%10==2 or pl[now].buff[0]/10==2) pl[now].handcard[i].MISS=20;
				else if(pl[now].buff[0]%10==3 or pl[now].buff[0]/10==3) pl[now].handcard[i].MISS=20;
				else if(pl[now].buff[0]%10==5 or pl[now].buff[0]/10==5) pl[now].handcard[i].MISS=20;
				else if(pl[now].buff[0]%10==7 or pl[now].buff[0]/10==7) pl[now].handcard[i].MISS=20;
				else pl[now].handcard[i].MISS=100;
			}
			if(pl[now].handcard[i].func==112){
				if(pl[now].buff[0]%10==1 or pl[now].buff[0]/10==1) pl[now].handcard[i].MISS=20;
				else pl[now].handcard[i].MISS=100;
			}
			if(pl[now].handcard[i].func==113){
				if(pl[now].buff[0]%10==1 or pl[now].buff[0]/10==1) pl[now].handcard[i].MISS=20;
				else if(pl[now].buff[0]%10==3 or pl[now].buff[0]/10==3) pl[now].handcard[i].MISS=20;
				else if(pl[now].buff[0]%10==5 or pl[now].buff[0]/10==5) pl[now].handcard[i].MISS=20;
				else if(pl[now].buff[0]%10==7 or pl[now].buff[0]/10==7) pl[now].handcard[i].MISS=20;
				else if(pl[now].buff[0]%10==9 or pl[now].buff[0]/10==9) pl[now].handcard[i].MISS=20;
				else pl[now].handcard[i].MISS=100;
			}
			if(pl[now].handcard[i].func==114){
				if(pl[now].buff[0]%10==8 or pl[now].buff[0]/10==8) pl[now].handcard[i].MISS=0;
				else pl[now].handcard[i].MISS=50;
			}
			if(pl[now].handcard[i].func==118){
				pl[now].handcard[i].HEAL=pl[now].buff[0]*3;
			}
			if(pl[now].handcard[i].func==121){
				if(pl[now].buff[0]==92) pl[now].handcard[i].MISS=0;
				else pl[now].handcard[i].MISS=97;
			}
			if(pl[now].handcard[i].func==122){
				if(pl[now].buff[0]%10==5 or pl[now].buff[0]/10==5) pl[now].handcard[i].MISS=0;
				else pl[now].handcard[i].MISS=100;
			}
			if(pl[now].handcard[i].func==124){
				pl[now].handcard[i].ATK=pl[now].buff[0];
			}
			if(pl[now].handcard[i].func==127){
				pl[now].handcard[i].cost=pl[now].cost;
			}
			if(pl[now].handcard[i].func==128){
				pl[now].handcard[i].ATK=pl[3-now].buff[4]*35;
			}
			if(pl[now].handcard[i].func==130){
				pl[now].handcard[i].ATK=pl[3-now].buff[4]*15+50;
			}
			if(pl[now].handcard[i].func==134){
				pl[now].handcard[i].DEF=pl[now].buff[0]*6;
			}
			if(pl[now].handcard[i].func==138){
				pl[now].handcard[i].HEAL=pl[now].buff[0]*5;
			} 
			if(pl[now].cost>=pl[now].handcard[i].cost)SetColor(11);
			else SetColor(3);//�����Ƿ��㹻
			printf("%d",pl[now].handcard[i].cost);
			SetColor(color);
			SetPos(14,Row+i);
			printf(pl[now].handcard[i].Name());//����
			SetColor(7,0);
			printf("               ");
			SetPos(20+14,Row+i);
			printf("%-3d",pl[now].handcard[i].ATK);
			int atk=pl[now].handcard[i].cal_atk(now,3-now);
			if(pl[now].handcard[i].ATK!=atk){
				if(pl[now].handcard[i].ATK < atk)SetColor(12);
				else SetColor(8);
				printf("(%d)   ",atk);
				SetColor(7);
			}
			else{
				printf("           ");
			}//ATK
			printf("           ");
			SetPos(30+14,Row+i);
			printf("%-3d",pl[now].handcard[i].HEAL);
			int heal=pl[now].handcard[i].cal_heal(now,3-now);
			if(pl[now].handcard[i].HEAL!=heal){
				if(pl[now].handcard[i].HEAL < heal)SetColor(10);
				else SetColor(8);
				printf("(%d)   ",heal);
				SetColor(7);
			}
			else{
				printf("            ");
			}//HEAL
			printf("           ");
			SetPos(40+14,Row+i);
			printf("%-3d",pl[now].handcard[i].DEF);
			int def=pl[now].handcard[i].cal_def(now,3-now);
			if(pl[now].handcard[i].DEF!=def){
				if(pl[now].handcard[i].DEF < def)SetColor(15);
				else SetColor(8);
				printf("(%d)   ",def);
				SetColor(7);
			}
			else{
				printf("         ");
			}//DEF
			printf("           ");
			SetPos(50+14,Row+i);
			printf("%-3d%%",pl[now].handcard[i].MISS);//MISS
			int miss=pl[now].handcard[i].cal_miss(now,3-now);
			if(pl[now].handcard[i].MISS!=miss && pl[now].handcard[i].id!=119){
				SetColor(8);
				printf("(%d%%)  ",pl[now].handcard[i].cal_miss(now,3-now));
				SetColor(7);
			}
			else{
				printf("      ");
			}

		}//��ʾ����
		mouse(0);
		SetColor(7);
		SetPos(11,Row+9);
		printf("                                                                    ");
		SetPos(11,Row+9);
		printf(pl[now].handcard[cursor].Intro());
		input=getch();
		if(checkq(input)){
			SetPos(0,1);
			printf("                ");
			SetPos(11,Row+12);
			option_use=option_over=option_giveup=0;
			printf("                ");
			if(!option_quit){
				option_quit=1;
				SetPos(11,Row+12);
				printf("ȷ��Ҫ������          ");
			}
			else if(option_quit==1){
				option_quit=2;
				SetPos(11,Row+12);
				printf("���ȷ��Ҫ������      ");
			}
			else{
				pl[now].hp=0;
				UI(now);
				SetPos(11,Row+12);
				printf("                       ");
				winner=3-now;
				if(send_gaming(void_card)<0) {
					another_player_quit(server_mode); 
					winner=server_mode;
					return 1;
				}
				break;
			}
		}
		if(input>='1' && input<='9' ){
			if(!pl[now].used[input-'0'] && input-'0'<=pl[now].cardcnt) cursor=input-'0';
		}
		if(checkup(input) or checkleft(input)){
			option_use=option_giveup=option_over=option_quit=0;
			cursor--;
			if(cursor<=0) cursor=pl[now].cardcnt;
			while(pl[now].used[cursor]&&cursor>0){
				cursor--;
				if(cursor<=0) cursor=pl[now].cardcnt;
			}
		}
		if(checkdown(input) or checkright(input)){
			option_use=option_giveup=option_over=option_quit=0;
			cursor++;
			if(cursor>pl[now].cardcnt) cursor=1;
			while(pl[now].used[cursor]&&cursor<=pl[now].cardcnt){
				cursor++;
				if(cursor>pl[now].cardcnt) cursor=1;
			}
		}
		if(checkt(input))
		{
			SetPos(84,23),mouse(1);string x;getline(cin,x); 
			if(x!="")
			{
				if(x.find("�¹���")!=-1 or x.find("chenguoyi")!=-1 or x.find("����")!=-1 or x.find("����")!=-1) x="���������";
				else if((int)x.size()>40) x="�������������";
				else x="P"+int_to_string(now)+":"+x;
				if(send_gaming(conver_card)<0){
					another_player_quit(server_mode); 
					winner=server_mode;
					return 1;
				}
				if(send_int((int)x.size())<0){
					another_player_quit(server_mode); 
					winner=server_mode;
					return 1;
				}
				for(int i=0;i<(int)x.size();i++)
					if(send_int((int)x[i])<0){
						another_player_quit(server_mode); 
						winner=server_mode;
						return 1;
					}
				Con::append(x);	
			}
			cls();UI(now);	
			mouse(0);
		}
		if(cursor<=0)cursor=pl[now].cardcnt;
		if(cursor>pl[now].cardcnt) cursor=1;//�ƶ�����
		if(checkz(input))//ʹ��
		{
			SetPos(0,1);
			printf("                                               ");
			option_giveup=option_over=option_quit=0;
			if(!option_use){
				option_use=1;
			}
			else {
				if(pl[now].used[cursor]){
					SetPos(0,1);
					printf("ѡ�е��Ʋ����ڣ�                   ");
				}
				else if(pl[now].cost<pl[now].handcard[cursor].cost){
					SetPos(0,1);
					printf("���ò����޷�ʹ�ô��ƣ�                 ");
				}
				else{
					pl[now].cost-=pl[now].handcard[cursor].cost;
					pl[now].used[cursor]=1;
					pl[now].rest--;
					if(pl[now].handcard[cursor].Use(now,3-now)) extra(now,cursor);
					if(pl[now].occ==5 && rad()%100<80) {
						pl[now].used[cursor]=0;
						pl[now].rest++;
						pl[now].handcard[cursor]=pl[now].heap[(rad()%pl[now].heapn)+1];
					}
					for(int i=1;i<=pl[now].cardcnt;i++){
						if(pl[now].used[i]) continue;
						if(pl[now].handcard[i].func==41) pl[now].handcard[i].ATK+=10;
					}//func 41 ����ATK
					while(pl[now].used[cursor] && cursor<=pl[now].cardcnt+1 && pl[now].rest){
						cursor++;
						if(cursor>pl[now].cardcnt) cursor=1;
					}
					if(pl[now].occ==7){
						pl[now].buff[10]=0;
						for(int i=1;i<=pl[now].cardcnt;i++) 
							if(!pl[now].used[i] && pl[now].handcard[i].func==42) pl[now].buff[10]++; //42-->[��ʥ��־]
					}//��ʦ����[��ʥ��־]
					if(send_gaming(use_card)<0) {
						another_player_quit(server_mode); 
						winner=server_mode;
						return 1;
					}
				}
				option_use=0;
			}
		}
		if(checkx(input)){//����
			SetPos(0,1);
			printf("                                          ");
			option_use=option_over=option_quit=0;
			if(!option_giveup){
				option_giveup=1;
			}
			else{
				if(pl[now].occ!=6){
					if(giveupcard(now,cursor)){
						use_card=pl[now].handcard[cursor];
						use_card.MISS=2;
						if(send_gaming(use_card)<0) {
							another_player_quit(server_mode); 
							winner=server_mode;
							return 1;
						}
					}
					if(rad()%2 and pl[now].occ==11)
						pl[now].handcard[cursor]=pl[now].heap[(rad()%pl[now].heapn)+1];
					else
					{
						if(pl[now].occ==11) pl[now].buff[0]++; 
						pl[now].used[cursor]=1;
						pl[now].rest--;
						UI(now);
						while(pl[now].used[cursor] && cursor<=pl[now].cardcnt+1 && pl[now].rest){
							cursor++;
							if(cursor>pl[now].cardcnt)cursor=1;
						}
					}
					option_giveup=0;
				}
				else{
					if(pl[now].used[cursor]){
						SetPos(0,1);
						printf("ѡ�е��Ʋ����ڣ�                  ");
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
						UI(now);
					}
					else if(pl[now].cost<pl[now].handcard[cursor].cost){
						SetPos(0,1);
						printf("���ò����޷����ô��ƣ�                 ");
					}
					else{
						pl[now].cost-=pl[now].handcard[cursor].cost;
						pl[now].used[cursor]=1;
						pl[now].rest--;
						giveupcard(now,cursor);
						UI(now);
						swap(pl[now].handcard[cursor].ATK,pl[now].handcard[cursor].HEAL);
						pl[now].handcard[cursor].Use(now,3-now);
						while(pl[now].used[cursor] && cursor<=pl[now].cardcnt+1 && pl[now].rest){
							cursor++;
							if(cursor>pl[now].cardcnt)cursor=1;
						}
						option_giveup=0;
						pl[now].buff[0]++;
						UI(now);
						if(send_gaming(use_card)<0) {
							another_player_quit(server_mode); 
							winner=server_mode;
							return 1;
						}
					}
				}
				if(pl[now].occ==7){
					pl[now].buff[10]=0;
					for(int i=1;i<=pl[now].cardcnt;i++) 
						if(!pl[now].used[i] && pl[now].handcard[i].func==42) pl[now].buff[10]++; //42-->[��ʥ��־]
				}//��ʦ����[��ʥ��־]
				if(send_gaming(void_card)<0) {
					another_player_quit(server_mode); 
					winner=server_mode;
					return 1;
				}
			}
		}
		if(checkenter(input)) {//�����غ�
			option_giveup=option_use=option_quit=0;
			SetPos(0,1);
			printf("                ");
			if(!option_over){
				option_over=1;
				SetPos(11,Row+12);
				printf("ȷ��Ҫ�����غ���    ");
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
		if(pl[now].rest<=0){//�����غ�
			SetPos(0,1);
			printf("                ");
			for(int i=1;i<=pl[now].cardcnt;i++){
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
		Check(now);
	}
	return 0;
}

int Ask_same(int now){
	int option_use=0,option_giveup=0,option_over=0;
	start_turn(now);
	if(mode[3]) treasure(now);
	UI(now);
	if(Check(now)) return 0;
	//�����ж�

	for(int i=1;i<=pl[now].cardcnt;i++){
		if(pl[now].used[i]){
			pl[now].used[i]=0;
			if(pl[now].occ==7 && rad()%100<pl[now].buff[0]*2){
				pl[now].handcard[i]=lib[7][libcnt[7]+1];
			}
			else{
				pl[now].handcard[i]=pl[now].heap[(rad()%pl[now].heapn)+1];
			}
		}
	}
	//��������
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
		printf("��");
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
			if(cursor==i && !option_giveup && !option_use) SetColor(color=15),printf("��"); 
			else if(cursor==i && option_giveup) SetColor(color=15),printf("��"); 
			else if(cursor==i && option_use) SetColor(color=15),printf("��"); 
			else SetColor(color=7),printf("  "); 
			if(pl[now].occ==10 and pl[now].handcard[i].func==121 and pl[now].buff[0]==92) color--;
			SetPos(7,Row+i),printf("%d",i); 
			if(pl[now].used[i]){
				SetColor(8);
				printf("                                                                     ");
				SetPos(14,Row+i);
				printf("[Used.]");
				continue;
			}

			SetPos(11,Row+i);
			if(pl[now].handcard[i].func==15) {
				pl[now].handcard[i].ATK=pl[now].cost*40+25;
				pl[now].handcard[i].cost=pl[now].cost;
			}//[��ˮһս]
			if(pl[now].handcard[i].func==32){
				int da=0;
				for(int i=1;i<=pl[now].cardcnt;i++){
					if(pl[now].used[i]) continue;
					if(pl[now].handcard[i].id==98) da+=60; //98-->[�������]
				}
				pl[now].handcard[i].ATK=da;
			}
			if(pl[now].handcard[i].func==90){
				pl[now].handcard[i].HEAL=pl[now].buff[0]*2;
			}
			if(pl[now].handcard[i].func==102){
				pl[now].handcard[i].ATK=pl[now].def*1.75;
			}
			if(pl[now].handcard[i].func==110){
				if(pl[now].buff[0]%10==6 or pl[now].buff[0]/10==6) pl[now].handcard[i].MISS=0;
				else pl[now].handcard[i].MISS=100;
			}
			if(pl[now].handcard[i].func==111){
				if(pl[now].buff[0]%10==2 or pl[now].buff[0]/10==2) pl[now].handcard[i].MISS=20;
				else if(pl[now].buff[0]%10==3 or pl[now].buff[0]/10==3) pl[now].handcard[i].MISS=20;
				else if(pl[now].buff[0]%10==5 or pl[now].buff[0]/10==5) pl[now].handcard[i].MISS=20;
				else if(pl[now].buff[0]%10==7 or pl[now].buff[0]/10==7) pl[now].handcard[i].MISS=20;
				else pl[now].handcard[i].MISS=100;
			}
			if(pl[now].handcard[i].func==112){
				if(pl[now].buff[0]%10==1 or pl[now].buff[0]/10==1) pl[now].handcard[i].MISS=20;
				else pl[now].handcard[i].MISS=100;
			}
			if(pl[now].handcard[i].func==113){
				if(pl[now].buff[0]%10==1 or pl[now].buff[0]/10==1) pl[now].handcard[i].MISS=20;
				else if(pl[now].buff[0]%10==3 or pl[now].buff[0]/10==3) pl[now].handcard[i].MISS=20;
				else if(pl[now].buff[0]%10==5 or pl[now].buff[0]/10==5) pl[now].handcard[i].MISS=20;
				else if(pl[now].buff[0]%10==7 or pl[now].buff[0]/10==7) pl[now].handcard[i].MISS=20;
				else if(pl[now].buff[0]%10==9 or pl[now].buff[0]/10==9) pl[now].handcard[i].MISS=20;
				else pl[now].handcard[i].MISS=100;
			}
			if(pl[now].handcard[i].func==114){
				if(pl[now].buff[0]%10==8 or pl[now].buff[0]/10==8) pl[now].handcard[i].MISS=0;
				else pl[now].handcard[i].MISS=50;
			}
			if(pl[now].handcard[i].func==118){
				pl[now].handcard[i].HEAL=pl[now].buff[0]*3;
			}
			if(pl[now].handcard[i].func==121){
				if(pl[now].buff[0]==92) pl[now].handcard[i].MISS=0;
				else pl[now].handcard[i].MISS=97;
			}
			if(pl[now].handcard[i].func==122){
				if(pl[now].buff[0]%10==5 or pl[now].buff[0]/10==5) pl[now].handcard[i].MISS=0;
				else pl[now].handcard[i].MISS=100;
			}
			if(pl[now].handcard[i].func==124){
				pl[now].handcard[i].ATK=pl[now].buff[0];
			}
			if(pl[now].handcard[i].func==127){
				pl[now].handcard[i].cost=pl[now].cost;
			}
			if(pl[now].handcard[i].func==128){
				pl[now].handcard[i].ATK=pl[3-now].buff[4]*35;
			}
			if(pl[now].handcard[i].func==130){
				pl[now].handcard[i].ATK=pl[3-now].buff[4]*15+50;
			}
			if(pl[now].handcard[i].func==134){
				pl[now].handcard[i].DEF=pl[now].buff[0]*6;
			} 
			if(pl[now].handcard[i].func==138){
				pl[now].handcard[i].HEAL=pl[now].buff[0]*5;
			} 
			if(pl[now].cost>=pl[now].handcard[i].cost)SetColor(11);
			else SetColor(3);//�����Ƿ��㹻
			printf("%d",pl[now].handcard[i].cost);
			SetColor(color);
			SetPos(14,Row+i);
			printf(pl[now].handcard[i].Name());//����
			SetColor(7,0);
			printf("               ");
			SetPos(20+14,Row+i);
			printf("%-3d",pl[now].handcard[i].ATK);
			int atk=pl[now].handcard[i].cal_atk(now,3-now);
			if(pl[now].handcard[i].ATK!=atk){
				if(pl[now].handcard[i].ATK < atk)SetColor(12);
				else SetColor(8);
				printf("(%d)   ",atk);
				SetColor(7);
			}
			else{
				printf("           ");
			}//ATK
			printf("           ");
			SetPos(30+14,Row+i);
			printf("%-3d",pl[now].handcard[i].HEAL);
			int heal=pl[now].handcard[i].cal_heal(now,3-now);
			if(pl[now].handcard[i].HEAL!=heal){
				if(pl[now].handcard[i].HEAL < heal)SetColor(10);
				else SetColor(8);
				printf("(%d)   ",heal);
				SetColor(7);
			}
			else{
				printf("            ");
			}//HEAL
			printf("           ");
			SetPos(40+14,Row+i);
			printf("%-3d",pl[now].handcard[i].DEF);
			int def=pl[now].handcard[i].cal_def(now,3-now);
			if(pl[now].handcard[i].DEF!=def){
				if(pl[now].handcard[i].DEF < def)SetColor(15);
				else SetColor(8);
				printf("(%d)   ",def);
				SetColor(7);
			}
			else{
				printf("           ");
			}//DEF
			printf("           ");
			SetPos(50+14,Row+i);
			printf("%-3d%%",pl[now].handcard[i].MISS);//MISS
			int miss=pl[now].handcard[i].cal_miss(now,3-now);
			if(pl[now].handcard[i].MISS!=miss && pl[now].handcard[i].id!=119){
				SetColor(8);
				printf("(%d%%)  ",pl[now].handcard[i].cal_miss(now,3-now));
				SetColor(7);
			}
			else{
				printf("          ");
			}
			printf("           ");
		}//��ʾ����
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
		if(cursor>pl[now].cardcnt)cursor=1;//�ƶ�����
		if(checkz(input))//ʹ��
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
					printf("ѡ�е��Ʋ����ڣ�                 ");
				}
				else if(pl[now].cost<pl[now].handcard[cursor].cost){
					SetPos(0,1);
					printf("���ò����޷�ʹ�ô��ƣ�                 ");
				}
				else{
					pl[now].cost-=pl[now].handcard[cursor].cost;
					pl[now].used[cursor]=1;
					pl[now].rest--;
					if(pl[now].handcard[cursor].Use(now,3-now)) extra(now,cursor);
					if(pl[now].occ==5 && rad()%100<80) {
						pl[now].used[cursor]=0;
						pl[now].rest++;
						pl[now].handcard[cursor]=pl[now].heap[(rad()%pl[now].heapn)+1];
					}
					for(int i=1;i<=pl[now].cardcnt;i++){
						if(pl[now].used[i]) continue;
						if(pl[now].handcard[i].func==41) pl[now].handcard[i].ATK+=10;
					}//func 41 ����ATK
					while(pl[now].used[cursor] && cursor<=pl[now].cardcnt+1 && pl[now].rest){
						cursor++;
						if(cursor>pl[now].cardcnt) cursor=1;
					}
				}
				option_use=0;
			}
		}
		if(checkx(input)){//����
			SetPos(0,1);
			printf("                ");
			option_use=option_over=0;
			if(!option_giveup){
				option_giveup=1;
			}
			else{
				if(pl[now].occ!=6){
					giveupcard(now,cursor);
					if(rad()%2 and pl[now].occ==11)
					{
						pl[now].handcard[cursor]=pl[now].heap[(rad()%pl[now].heapn)+1];
						UI(now);
					}
					else
					{
						if(pl[now].occ==11) pl[now].buff[0]++; 
						pl[now].used[cursor]=1;
						pl[now].rest--;
						UI(now);
						while(pl[now].used[cursor] && cursor<=pl[now].cardcnt+1 && pl[now].rest){
							cursor++;
							if(cursor>pl[now].cardcnt)cursor=1;
						}	
					}	
					option_giveup=0;
				}
				else{
					if(pl[now].used[cursor]){
						SetPos(0,1);
						printf("ѡ�е��Ʋ����ڣ�                 ");
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
						UI(now);
					}
					else if(pl[now].cost<pl[now].handcard[cursor].cost){
						SetPos(0,1);
						printf("���ò����޷����ô��ƣ�                 ");
					}
					else{
						pl[now].cost-=pl[now].handcard[cursor].cost;
						pl[now].used[cursor]=1;
						pl[now].rest--;
						giveupcard(now,cursor);
						UI(now);
						swap(pl[now].handcard[cursor].ATK,pl[now].handcard[cursor].HEAL);
						pl[now].handcard[cursor].Use(now,3-now);
						while(pl[now].used[cursor] && cursor<=pl[now].cardcnt+1 && pl[now].rest){
							cursor++;
							if(cursor>pl[now].cardcnt)cursor=1;
						}
						option_giveup=0;
						pl[now].buff[0]++;
						UI(now);
					}
				}
			}
		}
		if(checkenter(input)) {//�����غ�
			SetPos(0,1);
			printf("                ");
			if(!option_over){
				option_over=1;
				SetPos(11,Row+12);
				printf("ȷ��Ҫ�����غ���");
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
		if(pl[now].rest<=0){//�����غ�
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
		printf("�� ���� Options ��");
		int cursor=1;
		while(1){
			for(int i=1;i<=5;i++)
			{
				SetPos(6,6+i);
				if(cursor!=i) SetColor(7,0);
				else SetColor(0,7);
				if(i==1) printf("�������                              ");
				if(i==2) printf("�����仯                              ");
				if(i==3) printf("������                                ");
				if(i==4) printf("���֣����buff                        ");
				if(i==5) printf("���֣���ʧ��                          ");
				SetPos(38,6+i);
				if(i==1)
				{
					if(mode[1]==0) printf("���");
					else printf(" P%d",mode[1]);
				}
				else
				{
					if(mode[i]==1) printf("����");
					else printf("�ء�");
				}
			}
			SetPos(8,15);SetColor(7,0);
			if(cursor==1){
				printf("ѡ��������ң�������һ������                          ");
				printf("\n\t                                                  ");
			}
			if(cursor==2){
				printf("�����仯��Ӱ�쵽ս�֣�����˫����������              ");
				printf("\n\t                                                  ");
			}
			if(cursor==3){
				printf("�������ض�Ҫ��ʱ����������ǿ�������ƣ�ÿ�ֽ�1��     ");
				printf("\n\t                                                  ");
			}
			if(cursor==4){
				printf("ÿ�غϿ�ʼʱ���1�غ����buff,˫���ƿ������������    ");
				printf("\n\t������ATK/HEAL/DEF��Ϊԭ�ȵ�70%                   ");
			}
			if(cursor==5){
				printf("������MISS����25,��ATKͬʱҲ�ϵ�50%%                   ");
				printf("\n\t                                                  ");
			}
			
			
			if(server_mode==1){
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
					send_int(8255); break;
				}
				if(send_int(6000+cursor)<0)
				{
					another_player_quit(server_mode); 
					winner=server_mode;
					return 1;
				}
				for(int i=1;i<=5;i++)
					if(send_int(6000+i*100+mode[i])<0)
					{
						another_player_quit(server_mode); 
						winner=server_mode;
						return 1;
					}
			}
			else{
				bool quit_option=0;
				for(int i=0;i<=5;i++){
					int recv_val=recv_message();
					if(recv_val<0) {
						another_player_quit(server_mode); 
						winner=server_mode;
						return 1;
					}
					if(recv_val/1000==6 and recv_val%1000/100==0) cursor=recv_val%100;
					if(recv_val==8255){
						quit_option=1; break;
					}
				}
				if(quit_option) break;
			}
		}
	}
	else {
		SetPos(6,4);
		printf("�� ���� Options ��");
		int cursor=1;
		while(1){
			for(int i=1;i<=5;i++)
			{
				SetPos(6,6+i);
				if(cursor!=i) SetColor(7,0);
				else SetColor(0,7);
				if(i==1) printf("�������                              ");
				if(i==2) printf("�����仯                              ");
				if(i==3) printf("������                                ");
				if(i==4) printf("���֣����buff                        ");
				if(i==5) printf("���֣���ʧ��                          ");
				SetPos(38,6+i);
				if(i==1)
				{
					if(mode[1]==0) printf("���");
					else printf(" P%d",mode[1]);
				}
				else
				{
					if(mode[i]==1) printf("����");
					else printf("�ء�");
				}
			}
			SetPos(8,15);SetColor(7,0);
			if(cursor==1){
				printf("ѡ��������ң�������һ������                          ");
				printf("\n\t                                                  ");
			}
			if(cursor==2){
				printf("�����仯��Ӱ�쵽ս�֣�����˫����������              ");
				printf("\n\t                                                  ");
			}
			if(cursor==3){
				printf("�������ض�Ҫ��ʱ����������ǿ�������ƣ�ÿ�ֽ�1��     ");
				printf("\n\t                                                  ");
			}
			if(cursor==4){
				printf("ÿ�غϿ�ʼʱ���1�غ����buff,˫���ƿ������������    ");
				printf("\n\t������ATK/HEAL/DEF��Ϊԭ�ȵ�70%                   ");
			}
			if(cursor==5){
				printf("������MISS����25,��ATKͬʱҲ�ϵ�50%%                   ");
				printf("\n\t                                                  ");
			}
			
			//��Ϸģʽ�趨
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
void Connect(){
	cols=88;
	
	SetPos(3,10);
	SetColor(7,0);
	printf("�л���һ��Ŀ                          ");
	SetPos(27,10);
	printf("[w]  [��]");

	SetPos(3,11);
	printf("�л���һ��Ŀ                          ");
	SetPos(27,11);
	printf("[s]  [��]");

	SetPos(3,12);
	printf("�������/��������                          ");
	SetPos(27,12);
	printf("[z]  [+]");

	SetPos(3,13);
	printf("��������                          ");
	SetPos(27,13);
	printf("[x]  [-]");

	SetPos(3,14);
	printf("�����غ�/ȷ��                          ");
	SetPos(27,14);
	printf("[SPACE]  [ENTER]");

	SetPos(3,15);
	printf("����                          ");
	SetPos(27,15);
	printf("[q]");
	
	SetPos(3,16);
	printf("����                          ");
	SetPos(27,16);
	printf("[t]");
	
	SetPos(3,20);
	printf("��Ϸ�汾: v"),printf(version);
	SetPos(3,21);
	printf("��Ϸ��ҳ: https://github.com/cyw580/CARD-GAME");
	SetPos(3,22);
	printf("����Ŷ�: https://www.luogu.com.cn/team/72157");
	
	SetPos(50,6);
	printf("v3.3.1 �������ݣ�");
	SetPos(50,7);
	printf("1.������(����)ս�������칦��");
	SetPos(50,8);
	printf("2.������ϸ�ڵ������Ż�");
	SetPos(50,9);
	printf("3.�Ķ���ʦ��������ͽ�ͽ���");
	
	int cursor=1;
	while(1){
		SetColor(7,0);
		SetPos(3,3);
		if(cursor!=1) SetColor(7,0);
		else SetColor(0,7);
		printf("1.���������        ");

		SetPos(3,4);
		if(cursor!=2)SetColor(7,0);
		else SetColor(0,7);
		printf("2.���ӵ����еķ����");

		SetPos(3,5);
		if(cursor!=3)SetColor(7,0);
		else SetColor(0,7);
		printf("3.�ڱ���������Ϸ    ");
		
		SetPos(3,6);
		if(cursor!=4)SetColor(7,0);
		else SetColor(0,7);
		printf("4.�������ʽ��      ");
		
		SetPos(3,7);
		if(cursor!=5)SetColor(7,0);
		else SetColor(0,7);
		printf("5.�˳���Ϸ          ");
		
		
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
	}
	return;
}

int game(){
	if(server_mode!=3){
		SetConsoleTitle("CARD GAME:Choose Your Identity.");
		Choose(server_mode);//ѡ��ְҵ
		SetPos(5,21);
		printf("P%d��ְҵ��",server_mode);
		printf(occ_name(pl[server_mode].occ));
		if(server_mode==1) send_int(2010);
		else send_int(2020);
		send_player(pl[server_mode]);
		recv_message();
		if(pl[server_mode].occ!=6) pl[server_mode].cost=3;//��ʼ��������
		if(pl[server_mode].occ==9) pl[server_mode].buff[0]=5;//������ʼ5�㾣�� 
		if(pl[server_mode].occ==10) pl[server_mode].buff[0]=rad()%100;//��ͽ��ʼ������� 
		init(server_mode);//�����Ӧ���γ��ƿ�
		if(pl[3-server_mode].occ==7 && pl[server_mode].occ!=7) {//���[����] 
			pl[server_mode].heap[++pl[server_mode].heapn]=lib[7][libcnt[7]+2];
		}
		if(pl[3-server_mode].occ==9 && pl[server_mode].occ!=9) {//���[��������] 
			pl[server_mode].heap[++pl[server_mode].heapn]=lib[9][libcnt[9]+1];
		}
		if(pl[3-server_mode].occ==11 && pl[server_mode].occ!=11){//���[��������]
			pl[server_mode].heap[++pl[server_mode].heapn]=lib[11][libcnt[11]+1];
		}
		for(int i=1;i<=pl[server_mode].cardcnt;i++) {
			pl[server_mode].handcard[i]=pl[server_mode].heap[(rad()%pl[server_mode].heapn)+1];
		}
		cols=130;
		//��ʼ����
		if(server_mode==1) send_int(2010);
		else send_int(2020);
		send_player(pl[server_mode]);
		recv_message();

		SetConsoleTitle("Here we go...");
		Sleep(1500);
		while(_kbhit()) getch();
		cls();//��������ʼ��Ϸ
		if(server_mode==1){
			if(!mode[1]){
				mode[1]=rad()%2+1;
			}//�������
			now=mode[1];
		}
		env_now=0;
		char title[]="CARD GAME:Turn of P1  Turn:   ";
		if(server_mode==1){
			send_int(4000+now*10);
		}
		else if(recv_message()<0) {
			another_player_quit(server_mode);
			winner=server_mode; 
			return 1;
		}
		if(server_mode!=now) Sleep(1500);
		if(server_mode==now) adturn=1;
		while(!winner && !Check(now)){
			if(!if_adv) adv(3-now),if_adv=1; //���ֲ���
			if(pl[3-now].occ==7 && pl[3-now].buff[10]==pl[3-now].cardcnt) {
				cls();
				SetPos(13,0);
				SetColor(14);
				printf("�Է��ܵ���[��ʥ��־]�ľ���,������!");
				Sleep(500);
				winner=3-now;
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
				SetPos(11,Row+12);
				printf("�����ֵ���Ļغ�......");
				Sleep(1500);
				while(_kbhit()) getch();
				appcnt=0;
				cls();
				if(Ask(now)==1) break;			//origin copy
				if(pl[now].occ==7){
					if(pl[now].cost==0) pl[now].buff[0]++;
					pl[now].buff[10]=0;
					for(int i=1;i<=pl[now].cardcnt;i++) 
						if(!pl[now].used[i] && pl[now].handcard[i].func==42) pl[now].buff[10]++; //42-->[��ʥ��־]
					if(pl[now].buff[10]==pl[now].cardcnt)  winner=now;
				}//��ʦ����[��ʥ��־]
				if(pl[now].occ==8){
					for(int i=1;i<=pl[now].cardcnt;i++){
						if(pl[now].used[i]) continue;
						if(pl[now].handcard[i].func==92) pl[now].handcard[i].ATK+=min(pl[now].buff[0],50);
					}
					int damage=pl[now].buff[0];
					if(pl[3-now].occ==9)
					{
						int defdamage=min(damage,pl[3-now].def);
						damage-=defdamage,pl[3-now].def-=defdamage;
						if(damage) pl[3-now].hp-=damage;
						defdamage=defdamage*pl[3-now].buff[0]/10;
						if(defdamage<=pl[now].buff[0]*3) pl[now].buff[0]-=defdamage/3,defdamage=0;
						else defdamage-=pl[now].buff[0]*3,pl[now].buff[0]=0;
						if(defdamage>0) pl[now].hp-=defdamage;
					}
					else
					{
						if(damage>=pl[3-now].def) {
							damage-=pl[3-now].def,pl[3-now].def=0,pl[3-now].hp-=damage;
						}
						else pl[3-now].def-=damage;	
					}
					//[��Ⱥ����]
					Check(3-now);
				}
				pl[now].UpdateBuff(2);
				//�����䶯
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
				}			//origin copy
				// pl[now].rest=pl[now].cardcnt;
				if(!winner){
					SetPos(11,Row+12);
					printf("��Ļغϼ�������......");
					now=3-now;
					if(send_gaming(void_card)<0) {
						another_player_quit(server_mode); 
						winner=server_mode;
						break;
					}
					Sleep(1500);
					while(_kbhit()) getch();
					cls();
				}
				else if(pl[now].occ==7 && pl[now].buff[10]==pl[now].cardcnt){ 
					cls();
					SetPos(11,0);
					SetColor(6);
					printf("[��ʥ��־]ϴ�����������,��ֱ�ӻ����ʤ��!");
					now=3-now;
					if(send_gaming(void_card)<0){
						another_player_quit(server_mode); 
						winner=server_mode;
						break;
					}
					Sleep(500);
				}
				else if(winner==now){
					cls();
					SetPos(13,0);
					SetColor(6);
					printf("��ϲ����ܶ��ֻ����ʤ��!");
					now=3-now;
					if(send_gaming(void_card)<0) {
						another_player_quit(server_mode); 
						winner=server_mode;
						break;
					}
					Sleep(500);
				}
			}
			else{
				UI(server_mode);
				UI_other();
				if(recv_gaming()<0) {
					another_player_quit(server_mode); 
					winner=server_mode;
					break;
				}
			}
		}
	}
	else {
		SetConsoleTitle("CARD GAME:Choose Your Identity.");
		for(int now=1;now<=2;now++){
			Choose(now);//ѡ��ְҵ
			SetPos(5,20+now);
			printf("P%d��ְҵ��",now);
			printf(occ_name(pl[now].occ));
		}
		SetConsoleTitle("Here we go...");
		Sleep(1500);
		while(_kbhit()) getch();
		cls();//��������ʼ��Ϸ

		for(int i=1;i<=2;i++) if(pl[i].occ!=6) pl[i].cost=3;//��ʼ��������
		for(int i=1;i<=2;i++) if(pl[i].occ==9) pl[i].buff[0]=5;//������ʼ5�㾣�� 
		for(int i=1;i<=2;i++) if(pl[i].occ==10) pl[i].buff[0]=rad()%100;//��ͽ��ʼ������� 
		for(int x=1;x<=2;x++) {
			init(x);//�����Ӧ���γ��ƿ�
			for(int i=1;i<=pl[x].cardcnt;i++) {
				pl[x].handcard[i]=pl[x].heap[(rad()%pl[x].heapn)+1];
			}
			if(pl[3-x].occ==7 && pl[x].occ!=7) {//���[����] 
				pl[x].heap[++pl[x].heapn]=lib[7][libcnt[7]+2];
			}
			if(pl[3-x].occ==9 && pl[x].occ!=9) {//���[��������] 
				pl[x].heap[++pl[x].heapn]=lib[9][libcnt[9]+1];
			}
			if(pl[3-x].occ==11 && pl[x].occ!=11){//���[��������]
				pl[x].heap[++pl[x].heapn]=lib[11][libcnt[11]+1];
			}
		}
		//��ʼ����

		if(!mode[1]){
			mode[1]=rad()%2+1;
		}//�������
		int now=mode[1];

		adv(3-mode[1]);//���ֲ���

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
			UI(now);
			if(Ask_same(now)==1) break;
			if(pl[now].occ==7){
				if(pl[now].cost==0) pl[now].buff[0]++;
				bool ssyz=1;
				for(int i=1;i<=pl[now].cardcnt;i++) if(pl[now].used[i] || pl[now].handcard[i].func!=42) ssyz=0;
				if(ssyz) winner=now;
			}//��ʦ����[��ʥ��־]
			if(pl[now].occ==8){
				for(int i=1;i<=pl[now].cardcnt;i++){
					if(pl[now].used[i]) continue;
					if(pl[now].handcard[i].func==92) pl[now].handcard[i].ATK+=min(pl[now].buff[0],50);
				}
				int damage=pl[now].buff[0];
				if(pl[3-now].occ==9)
				{
					int defdamage=min(damage,pl[3-now].def);
					damage-=defdamage,pl[3-now].def-=defdamage;
					if(damage) pl[3-now].hp-=damage;
					defdamage=defdamage*pl[3-now].buff[0]/10;
					if(defdamage<=pl[now].buff[0]*3) pl[now].buff[0]-=defdamage/3,defdamage=0;
					else defdamage-=pl[now].buff[0]*3,pl[now].buff[0]=0;
					if(defdamage>0) pl[now].hp-=defdamage;
				}
				else
				{
					if(damage>=pl[3-now].def) {
						damage-=pl[3-now].def,pl[3-now].def=0,pl[3-now].hp-=damage;
					}
					else pl[3-now].def-=damage;	
				}
				//[��Ⱥ����]
				Check(3-now);
			}
			pl[now].UpdateBuff(2);
			cls();
			now++;
			if(now>2)now=1;

			//�����䶯
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
	}
	return 0;
}

int main(){
	cls();
	srand(time(NULL));
	previous();//��ù����ƿ��ְҵ�ƿ�
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
			if(server_mode==2) WSAstart(),SetPos(26,4),printf("��������ip��ַ:");
			else {
				WSAstart();
				char hostname[256]={0},ip[256]={0};
				gethostname(hostname,sizeof(hostname));
				HOSTENT* host=gethostbyname(hostname);
				strcpy(ip,inet_ntoa(*(in_addr*)*host->h_addr_list));
				SetPos(26,3),printf("���ip��ַ��:%s",ip);
				SetPos(26,4),printf("�ȴ����������...");
			}
			if(TCP_initialize(server_mode)!=0){
				connect_established=0;
				cls();
				if(server_mode==1){
					SetColor(6);
					printf("�޷����������...�����Ƿ���ͬһ�����������л����Ժ�����...\n");
					getch();
				}
				else{
					SetColor(6);
					printf("�޷�������������...��������״�����������Ƿ���������...\n");
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
					printf("˫���汾��ͬ...�޷�������Ϸ...");
					getch();continue;
				}
				
				SetConsoleTitle("CARD GAME:Options");
				if(Options(1)) continue;
				while(_kbhit()) getch();
				SetColor(0,7);
				system("color 07");
				cls();//�������л���Choose����
			}
			else {
				cls();
				int qwq1,qwq2,qwq3;
				send_int(version1),send_int(version2),send_int(version3);
				recv_int(qwq1),recv_int(qwq2),recv_int(qwq3);
				if(version1!=qwq1 or version2!=qwq2 or version3!=qwq3)
				{
					SetColor(6);
					printf("˫���汾��ͬ...�޷�������Ϸ...");
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
			cls();//�������л���Choose����
		}
		while(_kbhit()) getch();
		game();
		Sleep(1200);
		cls();
		UI(server_mode);
		showresult();
	}
	SetColor(7,0);
	return 0;
}


