#include<bits/stdc++.h>
#include<winsock2.h>
#include<windows.h>
#include<conio.h> 
#include<winsock.h>
#include"PreInfo(ACG).hpp"
using namespace std;
SOCKADDR_IN addrServer,addrClient;
SOCKET Client;
WORD w_req;
WSADATA wsadata;
const int MAX_BUFF_LEN = 2048;
char buffer[MAX_BUFF_LEN];
int WSAstart()
{
    w_req=MAKEWORD(2,2);
	return WSAStartup(w_req, &wsadata);
}
int TCP_initialize(int server_mode)
{
	closesocket(Client);
    if(server_mode==1)
	{
        SOCKET Listen=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
        if(Listen==INVALID_SOCKET) return 1;
        addrServer.sin_family=AF_INET;
        addrServer.sin_port=htons(113);
        addrServer.sin_addr.S_un.S_addr=inet_addr("0.0.0.0");
        if(bind(Listen,(struct sockaddr*)&addrServer,sizeof(addrServer)))return 2;
        if(listen(Listen,1)) return 3;
        int sockaddr_siz=sizeof(sockaddr_in);
        Client=accept(Listen,(struct sockaddr*)&addrClient,&sockaddr_siz);
        closesocket(Listen);
    }
    else
	{
        char Server_addrstr[16]={0};
        scanf("%s",Server_addrstr);
        addrServer.sin_addr.S_un.S_addr=inet_addr(Server_addrstr);
        addrServer.sin_family=AF_INET;
        addrServer.sin_port=htons(113);
        Client=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
        if(connect(Client,(struct sockaddr*)&addrServer,sizeof(addrServer)))
		{
            closesocket(Client);
			return -1;
        }
    }
    return 0;
}
void send_int(int t)
{
	memset(buffer, 0, sizeof(buffer));
	int len=0;
	if(t==0) len=1,buffer[0]='0';
	else
	{
		if(t<0) t=0-t,len++,buffer[0]='-';
		string g;
		while(t)
		{
			g.push_back(char('0'+t%10));
			t/=10;
		}	
		for(int i=g.size()-1;i>=0;i--)
			buffer[len++]=g[i];
	} 
	buffer[len]='\0';
    send(Client, buffer, len, 0);  
}
int recv_int()
{
	sockaddr_in addr;
    int len = sizeof(addr);
    int buflen = -1;
    while(buflen==-1)
	{
		while(!_kbhit())
		{
			buflen=recv(Client, buffer, MAX_BUFF_LEN, 0);
			if(buflen!=-1) break;
		}
		if(buflen!=-1) break;
		getch();
	}
    buffer[buflen] = 0x00;
    int sum=0,tt=1;
    if(buffer[0]=='-') tt=-1;
    for(int i=0;i<buflen;i++)
    	if(buffer[i]>='0' and buffer[i]<='9')
    		sum=sum*10+buffer[i]-'0';  
    return sum*tt;
}
void send_string(string t)
{
	memset(buffer, 0, sizeof(buffer));
	int len=0;
	for(int i=0;i<t.size();i++)
		buffer[len++]=t[i];
	buffer[len]='\0';
    send(Client, buffer, len, 0);  
}
string recv_string()
{
	sockaddr_in addr;
    int len = sizeof(addr);
    int buflen = -1;
    while(buflen==-1)
	{
		while(!_kbhit())
		{
			buflen=recv(Client, buffer, MAX_BUFF_LEN, 0);
			if(buflen!=-1) break;
		}
		if(buflen!=-1) break;
		getch();
	}
    buffer[buflen] = 0x00;
    string g;
    for(int i=0;i<buflen;i++)
    	g.push_back(buffer[i]);
    return g;
}
void send_int2(int t)
{
	send_int(t);
	string g=recv_string();
	while(g!="Sure") g=recv_string();
}
int recv_int2()
{
	int g=recv_int();
	send_string("Sure");
	return g;
}
void send_string2(string t)
{
	send_string(t);
	string g=recv_string();
	while(g!="Sure") g=recv_string();
}
string recv_string2()
{
	string g=recv_string();
	send_string("Sure");
	return g;
}
void send_vector_string(vector<string> &t)
{
	send_int2(t.size());
	for(int i=0;i<t.size();i++)
		send_string2(t[i]);
}
vector<string> recv_vector_string()
{
	vector<string> g;g.clear();
	int p=recv_int2();
	for(int i=0;i<p;i++)
		g.push_back(recv_string2());
	return g; 
}
void send_card(card t)
{
	send_string2(t.name);
	send_int2(t.cost);
	send_int2(t.effect);
	send_vector_string(t.speffect);
}
card recv_card()
{
	card g("",0,0);
	g.name=recv_string2();
	g.cost=recv_int2();
	g.effect=recv_int2();
	g.speffect=recv_vector_string();
	return g;
}
void send_skill(skill t)
{
	send_string2(t.name);
	send_int2(t.mana);
	send_int2(t.active);
	send_int2(t.effect);
}
skill recv_skill()
{
	skill g;
	g.name=recv_string2();
	g.mana=recv_int2();
	g.active=recv_int2();
	g.effect=recv_int2();
	return g;
}
void send_buff(buff t)
{
	send_string2(t.name);
	send_int2(t.tim);
}
buff recv_buff()
{
	buff g;
	g.name=recv_string2();
	g.tim=recv_int2();
	return g;
}
void send_vector_card(vector<card> &t)
{
	send_int2(t.size());
	for(int i=0;i<t.size();i++)
		send_card(t[i]);
}
vector<card> recv_vector_card()
{
	vector<card> g;g.clear();
	int p=recv_int2();
	for(int i=0;i<p;i++)
		g.push_back(recv_card());
	return g; 
}
void send_vector_skill(vector<skill> &t)
{
	send_int2(t.size());
	for(int i=0;i<t.size();i++)
		send_skill(t[i]);
}
vector<skill> recv_vector_skill()
{
	vector<skill> g;g.clear();
	int p=recv_int2();
	for(int i=0;i<p;i++)
		g.push_back(recv_skill());
	return g; 
}
void send_vector_buff(vector<buff> &t)
{
	send_int2(t.size());
	for(int i=0;i<t.size();i++)
		send_buff(t[i]);
}
vector<buff> recv_vector_buff()
{
	vector<buff> g;g.clear();
	int p=recv_int2();
	for(int i=0;i<p;i++)
		g.push_back(recv_buff());
	return g; 
}
void send_situation()
{
	send_vector_card(cardheap);send_vector_card(recardheap);
	send_vector_card(handcard1);send_vector_card(handcard2);
	send_vector_skill(skills1);send_vector_skill(skills2);
	send_vector_skill(simpleskills1);send_vector_skill(simpleskills2);
	send_vector_buff(buff1);send_vector_buff(buff2);
	send_string2(soul1);send_string2(soul2);
	send_int2(cost1);send_int2(cost2);send_int2(costmax1);send_int2(costmax2);
	send_int2(mana1);send_int2(mana2);send_int2(manamax1);send_int2(manamax2);
	send_int2(handcardmax1);send_int2(handcardmax2);
	send_int2(trunk1);send_int2(trunk2);
	send_int2(clocks);send_int2(clocksum);
	send_int2(tim);send_int2(twelveam);send_int2(timrate);
	send_int2(turn);
}
void recv_situation()
{
	cardheap=recv_vector_card();recardheap=recv_vector_card();
	handcard1=recv_vector_card();handcard2=recv_vector_card();
	skills1=recv_vector_skill();skills2=recv_vector_skill();
	simpleskills1=recv_vector_skill();simpleskills2=recv_vector_skill();
	buff1=recv_vector_buff();buff2=recv_vector_buff();
	soul1=recv_string2();soul2=recv_string2();
	cost1=recv_int2();cost2=recv_int2();costmax1=recv_int2();costmax2=recv_int2();
	mana1=recv_int2();mana2=recv_int2();manamax1=recv_int2();manamax2=recv_int2();
	handcardmax1=recv_int2();handcardmax2=recv_int2();
	trunk1=recv_int2();trunk2=recv_int2();
	clocks=recv_int2();clocksum=recv_int2();
	tim=recv_int2();twelveam=recv_int2();timrate=recv_int2();
	turn=recv_int2();
}
