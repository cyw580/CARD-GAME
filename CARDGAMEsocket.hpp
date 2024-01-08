#include<bits/stdc++.h>
#include<windows.h>
#include<conio.h> 
#include<iostream>
#include<cstdio>
#include<winsock.h>
#include"PreInfo.h"
SOCKADDR_IN addrServer,addrClient;
SOCKET Client;
WORD w_req;
WSADATA wsadata;
int WSAstart(){
    w_req=MAKEWORD(2,2);
	return WSAStartup(w_req, &wsadata);
}
int TCP_initialize(int server_mode){
	closesocket(Client);
    if(server_mode==1){
        SOCKET Listen=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
        if(Listen==INVALID_SOCKET) return 1;
        addrServer.sin_family=AF_INET;
        addrServer.sin_port=htons(11451);
        addrServer.sin_addr.S_un.S_addr=inet_addr("0.0.0.0");
        if(bind(Listen,(struct sockaddr*)&addrServer,sizeof(addrServer)))return 2;
        if(listen(Listen,1)) return 3;
        int sockaddr_siz=sizeof(sockaddr_in);
        Client=accept(Listen,(struct sockaddr*)&addrClient,&sockaddr_siz);
        closesocket(Listen);
    }
    else{
        char Server_addrstr[16]={0};
        scanf("%s",Server_addrstr);
        addrServer.sin_addr.S_un.S_addr=inet_addr(Server_addrstr);
        addrServer.sin_family=AF_INET;
        addrServer.sin_port=htons(11451);
        Client=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
        if(connect(Client,(struct sockaddr*)&addrServer,sizeof(addrServer))){
            closesocket(Client); return -1;
        }
    }
    return 0;
}
char charmsg[15000];
inline void stradd_int(char dst[],int src,int &num){
    char tmp[4]={0};
    memcpy(tmp, (char*)&src, 4);
    // strcpy(tmp,(char*)&src);
    for(int i=num*4;i<num*4+4;i++) dst[i]=tmp[i-num*4];
    num++;
    return;
}
inline void stradd_card(char dst[],Card src,int &num){
    stradd_int(dst,src.id,num);
    stradd_int(dst,src.cost,num),stradd_int(dst,src.ATK,num),stradd_int(dst,src.HEAL,num),stradd_int(dst,src.DEF,num);
    stradd_int(dst,src.MISS,num),stradd_int(dst,src.func,num);
    return;
}
int send_int(int s){
    return send(Client,(char*)&s,4,0)<0?-1:0;
}
int send_char(const char msg[],int len){
    return send(Client,msg,len,0)<0?-1:0;
}
int recv_char(char val[]){
    return recv(Client,val,21,0)<0?-1:0;
}
int recv_int(int &val){
    return recv(Client,(char*)&val,4,0)<0?-1:0;
}
int send_card(Card cr){
    int return_value=0,num=0;
    char msg[480]={0};
    stradd_card(msg,cr,num);
    return_value=send_char(msg,num*4);
    return return_value<0?-1:0;
}
int send_player(player pl){
    int return_value=0,num=0;
    char msg[2800]={0};
    stradd_int(msg,pl.cost,num),stradd_int(msg,pl.maxcost,num),stradd_int(msg,pl.rest,num);
    stradd_int(msg,pl.heapn,num),stradd_int(msg,pl.occ,num),stradd_int(msg,pl.hp,num),stradd_int(msg,pl.def,num);
    stradd_int(msg,pl.cardcnt,num),stradd_int(msg,pl.prehp,num);
    stradd_int(msg,pl.maxhp,num),stradd_int(msg,pl.maxdef,num);
    for(int i=0;i<11;i++) stradd_card(msg,pl.handcard[i],num);
    for(int i=0;i<65;i++) stradd_card(msg,pl.heap[i],num);
    for(int i=0;i<11;i++) stradd_int(msg,pl.used[i],num);
    for(int i=0;i<11;i++) stradd_int(msg,pl.buff[i],num);
    return_value=send_char(msg,num*4);
    return return_value<0?-1:0;
}
int recv_card(Card &cr){
    int return_value=0;
    return_value+=recv_int(cr.id);
    return_value+=recv_int(cr.cost)+recv_int(cr.ATK)+recv_int(cr.HEAL);
    return_value+=recv_int(cr.DEF)+recv_int(cr.MISS)+recv_int(cr.func);
    return return_value<0?-1:0;
}
int recv_player(player &pl){
    int return_value=0;
    return_value+=recv_int(pl.cost)+recv_int(pl.maxcost)+recv_int(pl.rest);
    return_value+=recv_int(pl.heapn)+recv_int(pl.occ)+recv_int(pl.hp)+recv_int(pl.def);
    return_value+=recv_int(pl.cardcnt)+recv_int(pl.prehp);
    return_value+=recv_int(pl.maxhp)+recv_int(pl.maxdef);
    for(int i=0;i<11;i++) return_value+=recv_card(pl.handcard[i]);
    for(int i=0;i<65;i++) return_value+=recv_card(pl.heap[i]);
    for(int i=0;i<11;i++) return_value+=recv_int(pl.used[i]);
    for(int i=0;i<11;i++) return_value+=recv_int(pl.buff[i]);
    return return_value<0?-1:0;
}
bool converflag=false;
int recv_message(){
    int return_val=0;
    char msg_from_client[15000]={0};
    int command;
    if(recv_int(command)<0) return -1;
    if(command/1000==2){
        int pl_num=command%100/10;
        if(recv_player(pl[pl_num])<0) return -1;
    }
    if(command/1000==3){
        if(recv_card(appcard[++appcnt])<0) return -1;
        if(appcard[appcnt]==conver_card) converflag=1,appcnt--;
        else
        {
	        int damage=appcard[appcnt].ATK;
			if(damage>=150 and appcard[appcnt].MISS!=3){//ÖØ»÷¶¯»­
				if(damage>=3000) system("color E7");
				else system("color 47");
				Shake(10,1);
				system("color 07");
				cls();
			}	
		}
    }
    if(command/1000==4){
        now=command%100/10;
    }
    if(command/1000==5){
        if(command/100==50) env_now=command%100/10;
        else if(command/100==51) env_cnt=command%100;
        else recv_int(env_rate);
    }
    if(command/1000==6){
    	if(command%1000/100==0) return command;
        mode[command%1000/100]=command%100;
    }
    if(command==8255){
        return 8255;
    }
    return return_val<0?-1:0;
}
/*

*/
inline int recv_gaming(){
	converflag=false;
    for(int i=1;i<=7;i++) if(recv_message()<0) return -1;
    if(converflag)
    {
    	int siz=0;if(recv_int(siz)<0) return -1;
    	string res="";
    	for(int i=0,tmp=0;i<siz;i++)
    		if(recv_int(tmp)<0) return -1;
    		else res.push_back(char(tmp));
    	Con::append(res),cols=130;cls();
	}
    return 0;
}
inline int send_gaming(Card use_card){
    int return_val=0;
    if(send_int(2010)<0) return -1;
    if(send_player(pl[1])<0) return -1;
    if(send_int(2020)<0) return -1;
    if(send_player(pl[2])<0) return -1;
    if(send_int(3000)<0) return -1;
    if(send_card(use_card)<0) return -1;
    if(now==1) {if(send_int(4010)<0) return -1;}
    else if(send_int(4020)<0) return -1;
    if(send_int(5000+env_now*10)<0) return -1;
    if(send_int(5100+env_cnt)<0) return -1;
    if(send_int(5200)<0) return -1;
    if(send_int(env_rate)<0) return -1;
    return 0;
}
// inline int send_option(){
//     send_int(6000+)
// }
// inline int recv_option(){

// }

