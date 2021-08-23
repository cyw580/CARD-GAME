#include<bits/stdc++.h>
#include<windows.h>
#include<conio.h> 
#include<iostream>
#include<cstdio>
using namespace std;
#include"PreInfo.h"
struct player{
	string name;
	int cost,maxcost,rest,heapn,occ;
	int hp,def,cardcnt,prehp;
	int maxhp,maxdef;
	Card handcard[15],heap[65];
	int used[15];
	int buff[20];
	//0-职业特性,1-燃烧,2-中毒,3-狂暴,4-虚弱,5-治疗,6-迷惑
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
int appcnt,now,env_now,env_rate,env_cnt,player_bgn,mode,env_on;
Card appcard[55];
#include"CARDGAMEsocket.hpp"
Card xsd=(Card){1,1,1,1,1,1,1};
int main(){
	SetConsoleOutputCP(65001);
	int s;
	scanf("%d",&s);
	if(s==1) {
		TCP_initialize(s);
		send_card(xsd);
		closesocket(Client);
	}
	else {
		TCP_initialize(s);
		recv_card(xsd);
		closesocket(Client),printf("%d",xsd.ATK);
	}
	system("pause");
	return 0;
}