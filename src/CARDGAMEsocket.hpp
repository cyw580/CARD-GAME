#include<winsock.h>
SOCKADDR_IN addrServer,addrClient;
SOCKET Client;

int TCP_initialize(int server_mode){
    WORD w_req=MAKEWORD(2,2);
	WSADATA wsadata;
	int err=WSAStartup(w_req, &wsadata);
    if(server_mode==1){
        SOCKET Listen=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
        if(Listen==INVALID_SOCKET) return 1;
        addrServer.sin_family=AF_INET;
        addrServer.sin_port=htons(11451);
        addrServer.sin_addr.S_un.S_addr=inet_addr("0.0.0.0");
        if(bind(Listen,(struct sockaddr*)&addrServer,sizeof(addrServer))) return 2;
        if(listen(Listen,1)) return 3;
        int sockaddr_siz=sizeof(sockaddr_in);
        Client=accept(Listen,(struct sockaddr*)&addrClient,&sockaddr_siz);
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
char charmsg[10000];
char* player_to_char(player pl){
    memset(charmsg,0,sizeof(charmsg));
    memcpy(charmsg,&pl,sizeof(player));
    return charmsg;
}
int send_message(char* msg_to_client,int len){
    memset(charmsg,0,sizeof(charmsg));
    memcpy(charmsg,msg_to_client,len);
    int sendval=send(Client,charmsg,len,0);
    if(sendval<0){
        closesocket(Client); return -1;
    }
    return 0;
}
player precv_message(int &return_val){
    char msg_from_client[10000]={0};
    int recvval=recv(Client,msg_from_client,10000,0);
    if(recvval<0){
        closesocket(Client); return_val+=-1;
    }
    player pl;
    memcpy(&pl,msg_from_client,sizeof(pl));
    return pl;
}
Card crecv_message(int &return_val){
    char msg_from_client[10000]={0};
    int recvval=recv(Client,msg_from_client,10000,0);
    if(recvval<0){
        closesocket(Client); return_val+=-1;
    }
    Card card;
    memcpy(&card,msg_from_client,sizeof(card));
    return card;
}
int recv_message(){
    int return_val=0;
    char msg_from_client[10000]={0};
    if(recv(Client,msg_from_client,4,0)<0) return -1;
    int command;
    sscanf(msg_from_client,"%d",&command);
    if(command==2){
        int pl_num;
        sscanf(msg_from_client,"%d %d",&command,&pl_num);
        pl[pl_num]=precv_message(return_val);
    }
    if(command==3){
        
    }
    if(command==4){
        sscanf(msg_from_client,"%d %d",&command,&now);
    }
    if(command==5){
        sscanf(msg_from_client,"%d %d",&command,&env_now);
    }
    return return_val<0?-1:0;
}
/*

*/
inline int recv_gaming(){
    for(int i=1;i<=4;i++) if(recv_message()<0) return -1;
    return 0;
}
inline int send_gaming(){
    int return_val=0;
    if(send_message("2 1 ",4)<0) return -1;
    if(send_message((char*)&pl[1],10000)<0) return -1;
    if(send_message("2 2 ",4)<0) return -1;
    if(send_message((char*)&pl[2],10000)<0) return -1;
    if(now==1) {if(send_message("4 1 ",4)<0) return -1;}
    else if(send_message("4 2 ",4)<0) return -1;
    char msg_of_weather[1010]={0};
    sprintf(msg_of_weather,"5 %d ",env_now);
    if(send_message(msg_of_weather,4)<0) return -1;
    return 0;
}