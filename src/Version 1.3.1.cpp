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
int mode;

struct player{
	string name;
	int cost,maxcost,rest,heapn,occ;
	int hp,def,cardcnt,prehp;
	int maxhp,maxdef;
	Card handcard[15],heap[1005];
	int used[15];
	int buff[20];
	//0-ְҵ����,1-ȼ��,2-�ж�,3-��,4-����,5-����,6-�Ի�
	int hprate(){
		return 10*hp/maxhp;
	}
	void UpdateBuff(int x){
		if(x==1) {//�غϿ�ʼ����
			buff[1]=max(0,buff[1]-1);//ȼ��
			buff[2]=max(0,buff[2]-1);//�ж�
			buff[5]=max(0,buff[5]-1);//����
		}
		if(x==2) {//�غϽ�������
			buff[3]=max(0,buff[3]-1);//��
			buff[4]=max(0,buff[4]-1);//����
			buff[6]=max(0,buff[6]-1);//�Ի�
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

int Card::cal_atk(int from,int to){//����ʵ�ʹ�����
	int damage=ATK;
	if(func==15) return damage;//[��ˮһս]����buff
	if(pl[from].occ==2 && ATK>0) damage=ATK+6*pl[from].buff[0];//��ʿ
	if(pl[from].occ==5 && ATK>0) damage=ATK+ATK*pl[from].buff[0]*0.08;//�ؾ�
	if(pl[from].occ==4 && pl[from].buff[0])//սʿ
		damage-=0.04*damage*pl[from].buff[0];
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
	if(mode==2) //����ģʽ�����buff
		damage*=0.6;
	return damage;
}

bool Card::check_atk(int from,int to){//�Ƿ���ʾʵ�ʹ�����
	if(func==15) return 0;//[��ˮһս]����buff
	if(pl[from].occ==2 && ATK>0 && pl[from].buff[0]>0) return 1;
	if(pl[from].occ==5 && ATK>0 && pl[from].buff[0]>0) return 1;
	if(pl[from].occ==4 && (ATK>0 && func!=15) && pl[from].buff[0]>0) return 1;
	//ְҵӰ��
	if(pl[from].buff[3] || pl[from].buff[4])
		return 1;
	//BuffӰ��
	if(env_now==1 || env_now==2)
		return 1;
	//����Ӱ��
	if(mode==2) return 1;
	return 0;
}

int Card::cal_heal(int from,int to){//����ʵ�ʻָ���
	int heal=HEAL;
	if(heal>0 && pl[from].buff[4])//����
		heal*=0.7;
	return heal;
}

bool Card::check_heal(int from,int to){//�Ƿ���ʾʵ�ʻָ���
	int heal=HEAL;
	if(heal>0 && pl[from].buff[4])
		return 1;
	return 0;
}

int Card::cal_def(int from,int to){//����ʵ�ʵ���
	int def=DEF;
	return def;
}

bool Card::check_def(int from,int to){//�Ƿ���ʾʵ�ʵ���
	return 0;
}

int Card::cal_miss(int from,int to){
	int miss=MISS;
	if(env_now==3)
		miss+=10;
	if(env_now==4)
		miss-=10;
	//������ʧ���ʵ�Ӱ��
	if(pl[from].buff[6])//�Ի�
		miss+=25;
	if(miss>100) miss=100;
	if(miss<0) miss=0;
	return miss;
}
bool Card::check_miss(int from,int to){
	if(env_now==3)
		return 1;
	if(env_now==4)
		return 1;
	//������ʧ���ʵ�Ӱ��
	if(pl[from].buff[6])//�Ի�
		return 1;
	return 0;
}

int Card::Use(int from,int to){
	int miss=cal_miss(from,to);
	if(rand()%100<miss){
		SetPos(0,1);
		printf("����ʧ����!");
		if(pl[from].occ==4 && ATK>0) pl[from].buff[0]+=2;
		UI();
		return 0;
	}//ʧЧ�ж�
	int damage,damage2,flag;
	if(func) flag=Special(from,to);
	damage=damage2=cal_atk(from,to);
	//���ܵ���
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
	if(damage>=150){//�ػ�����
		system("color 47");
		Shake(10,1);
		system("color 07");
		system("cls");
		UI();
	}
	//����
	int heal=cal_heal(from,to);
	pl[from].hp=min(pl[from].maxhp,pl[from].hp+heal);
	int def=cal_def(from,to);
	pl[from].def=min(pl[from].maxdef,pl[from].def+def);
	//�׶����Ѫ
	if(HEAL<0 && pl[from].occ==2) pl[from].buff[0]++;
	if(pl[from].occ==4 && ATK>0) pl[from].buff[0]+=2;
	if(pl[from].occ==5 && flag==2){
		pl[from].buff[0]++;
		pl[from].maxhp+=10;
		pl[from].maxdef+=10;
		pl[from].hp=min(pl[from].maxhp,pl[from].hp+20);
	}
	if(pl[from].occ==5 && flag==3){
		pl[from].buff[0]+=2;
		pl[from].maxhp+=20;
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
			if(pl[from].handcard[i].name=="[�������]"){
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
		int x=rand()%pl[from].cardcnt+1;//ѡȡ�����λ
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
		int x=rand()%pl[to].cardcnt+1;//ѡȡ�����λ
		pl[to].handcard[x]=lib[7][10];//[����ˮ��]
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
		int x=rand()%pl[to].cardcnt+1;//ѡȡ�����λ
		pl[to].handcard[x].cost+=1;
	}
	else if(func==48){
		for(int i=1;i<=pl[to].cardcnt;i++){
			if(pl[to].used[i]) continue;
			if(pl[to].handcard[i].name=="[��ʥ��־]") pl[to].used[i]=1,pl[to].buff[0]+=2;
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
		int x=rand()%pl[to].cardcnt+1,y=rand()%pl[to].cardcnt+1;//ѡȡ�����λ
		while(x==y) x=rand()%pl[to].cardcnt+1,y=rand()%pl[to].cardcnt+1;//ȥ��
		pl[to].handcard[x]=pl[to].handcard[y]=lib[7][libcnt[7]+3];
	}
	else if(func==56){
		pl[to].buff[6]=max(pl[to].buff[6],2);
	}
	else if(func==57){
		if(pl[from].buff[0]>=10){
			pl[from].buff[0]=max(pl[from].buff[0]-3,0);
			for(int i=1;i<=pl[from].heapn;i++){
				pl[from].heap[i].ATK*=1.2;
				if(pl[from].heap[i].HEAL>0) pl[from].heap[i].HEAL*=1.1;
				pl[from].heap[i].DEF*=1.1;
				pl[from].heap[i].MISS=max(pl[from].heap[i].MISS-3,0);
			}
		}
	}
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
	if(mode==3){//����ģʽ��ȫԱ���
		tot=funcnt[3][0];
		for(int i=1;i<=tot;i++) pl[x].heap[i]=fun[3][0][i];
		pl[x].heapn+=tot;
		return;
	}
	for(int i=1;i<=tot;i++){
		pl[x].heap[i]=lib[0][i];
		if(pl[x].occ==3 && pl[x].heap[i].HEAL>=75) i--,tot--; //��ʦ���ܳ鹫���ƿ�HEAL>=75����
		if(pl[x].occ==4 && pl[x].heap[i].HEAL>0) i--,tot--;//սʿ���ܳ鹫���ƿ�HEAL��
		if(pl[x].occ==5 || pl[x].occ==6) i--,tot--;//�ؾ�����ħ���ܳ鹫���ƿ����
		if(pl[x].occ==7 && pl[x].heap[i].ATK>=80) i--,tot--;//��ʦ���ܳ鹫���ƿ�ATK>=80����
		if(mode==1) pl[x].heap[i].MISS+=30;//����ģʽ����ʧ��
		if(mode==2) pl[x].heap[i].HEAL*=0.6,pl[x].heap[i].DEF*=0.6;//����ģʽ�����buff
	}
	pl[x].heapn=tot;
	tot=libcnt[pl[x].occ];
	for(int i=1;i<=tot;i++) {
		pl[x].heap[i+pl[x].heapn]=lib[pl[x].occ][i];
		if(mode==1) pl[x].heap[i+pl[x].heapn].MISS+=30;//����ģʽ����ʧ��
		if(mode==2) pl[x].heap[i+pl[x].heapn].HEAL*=0.6,pl[x].heap[i+pl[x].heapn].DEF*=0.6;//����ģʽ�����buff
	}
	pl[x].heapn+=tot;
	if(mode==2){//����ģʽ�����buff
		tot=funcnt[2][0];
		for(int i=1;i<=tot;i++) pl[x].heap[i+pl[x].heapn]=fun[2][0][i];
		pl[x].heapn+=tot;
	}
	if(pl[3-x].occ==7 && pl[x].occ!=7) {
		pl[x].heap[++pl[x].heapn]=lib[7][libcnt[7]+2];//���[����]
		if(mode==2) if(mode==1) pl[x].heap[pl[x].heapn].MISS+=30;//����ģʽ����ʧ��
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
	if(x==1)return "����";
	else if(x==2)return "��ʿ";
	else if(x==3)return "��ʦ";
	else if(x==4)return "սʿ";
	else if(x==5)return "�ؾ�";
	else if(x==6)return "��ħ";
	else if(x==7)return "��ʦ";
	else if(x==8)return "��Ե";
	return "";
}

void Choose(int now){
	SetPos(0,1);
	printf("          ��ѡ��P%d ",now);
	printf(pl[now].name);
	printf("��ְҵ��");//��һ�б�ʶ˭ѡ��ְҵ
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
		}//��ְҵ���ֺͼ��չʾ
		SetPos(0,12);
		occ_func(cursor);
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
		//���/����/ְҵ
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
			printf("��%d   ",pl[rnk].buff[0]);
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
		if(pl[rnk].buff[1])
			SetColor(6),printf("ȼ�� %d : ",pl[rnk].buff[1]);
		if(pl[rnk].buff[2])
			SetColor(2),printf("�ж� %d : ",pl[rnk].buff[2]);
		if(pl[rnk].buff[3])
			SetColor(4),printf("�� %d : ",pl[rnk].buff[3]);
		if(pl[rnk].buff[4])
			SetColor(11),printf("���� %d : ",pl[rnk].buff[4]);
		if(pl[rnk].buff[5])
			SetColor(10),printf("���� %d : ",pl[rnk].buff[5]);
		if(pl[rnk].buff[6])
			SetColor(5),printf("�Ի� %d : ",pl[rnk].buff[6]);
		printf("                                                 ");
		
		//Ѫ������
		SetPos(26,rnk*4+1-2);
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
		for(i=1;i<=32-pl[rnk].maxhp/40-pl[rnk].def/20;i++)
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

	return 0; 
}

void start_turn(int now){
	for(int i=1;i<=2;i++) pl[i].prehp=pl[i].hp;//��¼�غϿ�ʼѪ��
	if(pl[now].cost==0 && pl[now].occ==1)  pl[now].cost=1;//����[ʰ��]
	pl[now].cost=min(pl[now].cost+1,pl[now].maxcost);//�ӷ� 
	if(env_now==7)
		pl[now].cost=min(pl[now].cost+1,pl[now].maxcost);//�����ӷ� 

	//ְҵ����
	if(pl[now].occ==3){
		if(pl[now].buff[0]){
			if(pl[now].hp<=100) pl[now].hp+=min(pl[now].buff[0],(100-pl[now].hp)/20+1)*20;
			pl[now].cost=min(pl[now].maxcost,pl[now].cost+pl[now].buff[0]);
			pl[now].buff[0]=0;
		}
		if(rand()%100<35) pl[now].buff[0]++;
	}//��ʦ<�﷨��>��ǻ�÷���
	if(pl[now].occ==4) {
		if(pl[now].def==0) pl[now].def=45;
		pl[now].buff[0]=max(0,pl[now].buff[0]-1);
	}//սʿ���ײ����<��ƣ��>
	if(pl[now].occ==5) pl[now].cost=3;//�ؾ�ÿ�غ�3��
	if(pl[now].occ==6){
		pl[now].cost=min(pl[now].cost+1,pl[now].maxcost);
		int da=5*pl[now].buff[0];
		if(da<pl[now].def) pl[now].def-=da;
		else {
			if(pl[now].def>0) Shake(3,1);
			pl[now].hp-=da-pl[now].def,pl[now].def=0;
		}
	}//��ħ<��ԭ��>���Լ�����
	if(pl[now].buff[5])//����buff
		pl[now].hp=min(pl[now].hp+30,pl[now].maxhp);
	if(pl[now].buff[1])//ȼ��buff
		pl[now].hp-=40;
	if(pl[now].buff[2])//�ж�buff
		pl[now].hp-=pl[now].hp*0.2;
	pl[now].UpdateBuff(1);
	//buffǰ��

	if(mode==2){
		pl[now].buff[rand()%6+1]++;
	}//����ģʽ�����buff

	if(env_now==5){
		pl[now].hp=min(pl[now].maxhp,(int)(pl[now].hp+pl[now].maxhp*0.05));
	}//�ʺ�
	if(env_now==6){
		pl[now].hp=(pl[now].hp-pl[now].maxhp*0.05);
	}//����
	//�����ж�
}

int Ask(int now){
	int option_use=0,option_giveup=0,option_over=0;
	start_turn(now);
	UI();
	if(Check(now)) return 0;
	//�����ж�

	for(int i=1;i<=pl[now].cardcnt;i++){
		if(pl[now].used[i]){
			pl[now].used[i]=0;
			if(mode==3){//����ģʽ��ȫԱ���
				pl[now].handcard[i]=pl[now].heap[(rand()%pl[now].heapn)+1];
				int co=pl[now].handcard[i].cost;
				pl[now].handcard[i].ATK=rand()%(co*50)+1;
				pl[now].handcard[i].HEAL=rand()%(co*40)+1;
				pl[now].handcard[i].DEF=rand()%(co*35)+1;
				pl[now].handcard[i].MISS=rand()%101;
				if(pl[now].handcard[i].MISS>30) pl[now].handcard[i].MISS=rand()%101;
				if(pl[now].handcard[i].MISS>50) pl[now].handcard[i].MISS=rand()%101;
				if(pl[now].handcard[i].MISS>70) pl[now].handcard[i].MISS=rand()%101;
				if(pl[now].handcard[i].MISS>90) pl[now].handcard[i].MISS=rand()%101;
				continue;
			}
			if(pl[now].occ==7 && rand()%100<pl[now].buff[0]*2){
				pl[now].handcard[i]=lib[7][libcnt[7]+1];
			}
			else{
				pl[now].handcard[i]=pl[now].heap[(rand()%pl[now].heapn)+1];
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
			if(cursor==i && !option_giveup && !option_use) SetColor(color=15),printf("��%d",i); 
			else if(cursor==i && option_giveup) SetColor(color=15),printf("��%d",i); 
			else if(cursor==i && option_use) SetColor(color=15),printf("��%d",i); 
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
			}//[��ˮһս]
			if(pl[now].handcard[i].func==32){
				int da=0;
				for(int i=1;i<=pl[now].cardcnt;i++){
					if(pl[now].used[i]) continue;
					if(pl[now].handcard[i].name=="[�������]") da+=60;
				}
				pl[now].handcard[i].ATK=da;
			}
			if(pl[now].cost>=pl[now].handcard[i].cost)SetColor(11);
			else SetColor(3);//�����Ƿ��㹻
			printf("%d",pl[now].handcard[i].cost);
			SetColor(color);
			SetPos(14,Row+i);
			printf(pl[now].handcard[i].name);//����
			printf("               ");
			SetPos(20+14,Row+i);
			printf("%-3d",pl[now].handcard[i].ATK);
			if(pl[now].handcard[i].ATK>0 && pl[now].handcard[i].check_atk(now,3-now)){
				int atk=pl[now].handcard[i].cal_atk(now,3-now);
				if(pl[now].handcard[i].ATK < atk)SetColor(12);
				else if(pl[now].handcard[i].ATK == atk)SetColor(7);
				else SetColor(8);
				printf("(%d)   ",atk);
				SetColor(7);
			}
			else{
				printf("     ");
			}//ATK
			SetPos(30+14,Row+i);
			printf("%-3d",pl[now].handcard[i].HEAL);
			if(pl[now].handcard[i].HEAL>0 && pl[now].handcard[i].check_heal(now,3-now)){
				int heal=pl[now].handcard[i].cal_heal(now,3-now);
				if(pl[now].handcard[i].HEAL < heal)SetColor(10);
				else if(pl[now].handcard[i].HEAL == heal)SetColor(7);
				else SetColor(8);
				printf("(%d)   ",heal);
				SetColor(7);
			}
			else{
				printf("      ");
			}//HEAL
			SetPos(40+14,Row+i);
			printf("%-3d",pl[now].handcard[i].DEF);
			if(pl[now].handcard[i].DEF>0 && pl[now].handcard[i].check_def(now,3-now)){
				int def=pl[now].handcard[i].cal_def(now,3-now);
				if(pl[now].handcard[i].DEF < def)SetColor(15);
				else if(pl[now].handcard[i].DEF == def)SetColor(7);
				else SetColor(8);
				printf("(%d)   ",def);
				SetColor(7);
			}
			else{
				printf("      ");
			}//DEF
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

		}//��ʾ����
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
		if(cursor>pl[now].cardcnt)cursor=1;//�ƶ�����
		if(input=='z' || input=='+')//ʹ��
		{
			SetPos(0,1);
			printf("                          ");
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
					if(pl[now].handcard[cursor].Use(now,3-now) && pl[now].handcard[cursor].func==40) {
						pl[now].used[cursor]=0;
						pl[now].rest++;
						pl[now].handcard[cursor]=pl[3-now].heap[(rand()%pl[3-now].heapn)+1];
						pl[now].handcard[cursor].MISS=pl[now].handcard[cursor].func=0;
					}//�Ƶ�ʹ�� �� [����ȫ����]
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
					}//[��ʶ����]
					while(pl[now].used[cursor] && cursor<=pl[now].cardcnt+1 && pl[now].rest){
						cursor++;
						if(cursor>pl[now].cardcnt)cursor=1;
					}
				}
				option_use=0;
			}
		}
		if(input=='x' || input=='-'){//����
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
						UI();
					}
					else if(pl[now].cost<pl[now].handcard[cursor].cost){
						SetPos(0,1);
						printf("���ò����޷����ô��ƣ�                 ");
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
		if(input==SPACE || input==ENTER) {//�����غ�
			SetPos(0,1);
			printf("                ");
			if(!option_over){
				option_over=1;
				SetPos(11,Row+12);
				printf("ȷ��Ҫ�����غ���");
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
		if(pl[now].rest<=0){//�����غ�
			SetPos(0,1);
			printf("                ");
			if(pl[now].occ==7 && pl[now].cost==0) {
				++pl[now].buff[0];
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
	printf("�� ���� Options ��");
	int cursor=1;
	while(1){

		SetPos(1,3);
		if(cursor!=1)SetColor(7,0);
		else SetColor(0,7);
		printf("���������仯                          ");
		SetPos(25,3);
		if(env_on==1)printf("����");
		else printf("�ء�");//�����趨

		SetPos(1,5);
		if(cursor!=2)SetColor(7,0);
		else SetColor(0,7);
		printf("�������                             ");
		SetPos(25,5);
		if(player_bgn>0)
			printf("P%d",player_bgn);
		else
			printf("���");//��������趨

		SetPos(1,7);
		if(cursor!=3) SetColor(7,0);
		else SetColor(0,7);
		printf("��Ϸģʽ                                   ");
		SetPos(25,7);
		if(mode==0){
			printf("����ģʽ");
			SetPos(8,15);SetColor(7,0);
			printf("������Ϸģʽ                                             ");
			printf("\n\t                                          ");
		}
		else if(mode==1){
			printf("����ģʽ����ʧ��");
			SetPos(8,15);SetColor(7,0);
			printf("������ʧ��������30%%                                         ");
			printf("\n\t                                          ");
		}
		else if(mode==2){
			printf("����ģʽ�����buff");
			SetPos(8,15);SetColor(7,0);
			printf("ÿ�غϿ�ʼʱ���1�غ����buff����������ƿ������������");
			printf("\n\t������ATK/HEAL/DEF��Ϊԭ�ȵ�60%%");
		}
		else if(mode==3){
			printf("����ģʽ��ȫԱ���");
			SetPos(8,15);SetColor(7,0);
			printf("������ұ�����ˣ������������                                         ");
			printf("\n\t                                          ");
		}
		//��Ϸģʽ�趨
		
		input=getch();
		if(input==UP || input==LEFT) cursor--;
		if(input==DOWN || input==RIGHT) cursor++;
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
				mode=(mode+1)%4;
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
	SetConsoleTitle("CARD GAME:v1.3.0");
	srand(time(NULL));
	previous();//��ù����ƿ��ְҵ�ƿ�
	printf("WELCOME!!\n");
	for(int i=1;i<=2;i++){
		printf("������P%d������:",i);
		cin>>pl[i].name;
	}//��������
	Sleep(200);
	system("cls");//�������л���Option����

	SetConsoleTitle("CARD GAME:Options");
	Options();
	SetColor(0,7);
	system("color 07");
	system("cls");//�������л���Choose����

	if(mode!=3){
		SetConsoleTitle("CARD GAME:Choose Your Identity.");
		for(int now=1;now<=2;now++){
			Choose(now);//ѡ��ְҵ
			SetPos(0,19+now);
			printf("P%d��ְҵ��",now);
			printf(occ_name(pl[now].occ));
		}
	}
	else{
		pl[1].occ=pl[2].occ=1;
	}
	SetConsoleTitle("Here we go...");
	Sleep(1500);
	system("cls");//��������ʼ��Ϸ

	for(int i=1;i<=2;i++) if(pl[i].occ!=6) pl[i].cost=3;//��ʼ��������
	for(int x=1;x<=2;x++) {
		init(x);//�����Ӧ���γ��ƿ�
		for(int i=1;i<=pl[x].cardcnt;i++) {
			if(mode==3){//����ģʽ��ȫԱ���
				pl[x].handcard[i]=pl[x].heap[(rand()%pl[x].heapn)+1];
				int co=pl[x].handcard[i].cost;
				pl[x].handcard[i].ATK=rand()%(co*50)+1;
				pl[x].handcard[i].HEAL=rand()%(co*40)+1;
				pl[x].handcard[i].DEF=rand()%(co*35)+1;
				pl[x].handcard[i].MISS=rand()%101;
				continue;
			}
			pl[x].handcard[i]=pl[x].heap[(rand()%pl[x].heapn)+1];
		}
	}//��ʼ����

	if(!player_bgn){
		player_bgn=rand()%2+1;
	}//�������
	int now=player_bgn;

	adv(3-player_bgn);//���ֲ���

	env_now=0;

	char title[]="CARD GAME:Turn of P1";

	while(!winner && !Check(now)){
		title[19]=now+'0';
		SetConsoleTitle(title);
		UI();
		if(Ask(now)==1) break;
		if(pl[now].occ==7){
			bool ssyz=1;
			for(int i=1;i<=pl[now].cardcnt;i++) if(pl[now].used[i] || pl[now].handcard[i].name!="[��ʥ��־]") ssyz=0;
			if(ssyz) winner=now;
		}//��ʦ����[��ʥ��־]
		if(pl[now].occ==4 && pl[now].buff[0]>=12 && pl[now].heap[pl[now].heapn].func!=57){
			pl[now].heap[++pl[now].heapn]=lib[4][libcnt[4]+1];
		}//սʿˢ��[����ʿ��]
		pl[now].UpdateBuff(2);
		system("cls");
		now++;
		if(now>2)now=1;

		//�����䶯
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
			printf(pl[now].handcard[i].name);//����
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
			printf(pl[now].handcard[i].name);//����
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
	Sleep(4000);//4s�����չʾ
	system("cls");
	printf("#%d ",winner);
	printf(pl[winner].name);
	printf("�����ʤ��!!");
	Shake(10,1);
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
