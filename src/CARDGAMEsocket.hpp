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
char charmsg[15000];
// char* player_to_char(player pl){
//     memset(charmsg,0,sizeof(charmsg));
//     memcpy(charmsg,&pl,sizeof(player));
//     return charmsg;
// }
// int send_message(const char* msg_to_client,int len){
//     memset(charmsg,0,sizeof(charmsg));
//     memcpy(charmsg,msg_to_client,len);
//     int sendval=send(Client,charmsg,len,0);
//     if(sendval<0){
//         closesocket(Client); return -1;
//     }
//     return 0;
// }
int send_int(int s){
    return send(Client,(char*)&s,4,0)<0?-1:0;
}
int send_char(const char msg[]){
    char ch[21]={0};
    strcpy(ch,msg);
    return send(Client,ch,21,0)<0?-1:0;
}
int recv_char(char val[]){
    return recv(Client,val,21,0)<0?-1:0;
}
int recv_int(int &val){
    char msg_from_client[4]={0};
    return recv(Client,(char*)&val,4,0)<0?-1:0;
}
int send_card(Card cr){
    int return_value=0;
    return_value+=send_int(cr.id);
    return_value+=send_int(cr.cost)+send_int(cr.ATK)+send_int(cr.HEAL)+send_int(cr.DEF);
    return_value+=send_int(cr.MISS)+send_int(cr.func);
    return return_value<0?-1:0;
}
int send_player(player pl){
    int return_value=0;
    return_value+=send_int(pl.cost)+send_int(pl.maxcost)+send_int(pl.rest);
    return_value+=send_int(pl.heapn)+send_int(pl.occ)+send_int(pl.hp)+send_int(pl.def);
    return_value+=send_int(pl.cardcnt)+send_int(pl.prehp);
    return_value+=send_int(pl.maxhp)+send_int(pl.maxdef);
    for(int i=0;i<11;i++) return_value+=send_card(pl.handcard[i]);
    for(int i=0;i<65;i++) return_value+=send_card(pl.heap[i]);
    for(int i=0;i<11;i++) return_value+=send_int(pl.used[i]);
    for(int i=0;i<11;i++) return_value+=send_int(pl.buff[i]);
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
player precv_message(int &return_val){
    char msg_from_client[15000]={0};
    int recvval=recv(Client,msg_from_client,15000,0);
    if(recvval<0){
        closesocket(Client); return_val+=-1;
    }
    player pl;
    memcpy(&pl,msg_from_client,sizeof(pl));
    return pl;
}
Card crecv_message(int &return_val){
    char msg_from_client[15000]={0};
    int recvval=recv(Client,msg_from_client,15000,0);
    if(recvval<0){
        closesocket(Client); return_val+=-1;
    }
    Card card;
    memcpy(&card,msg_from_client,sizeof(card));
    return card;
}
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
    }
    if(command/1000==4){
        now=command%100/10;
    }
    if(command/1000==5){
        env_now=command%100/10;
    }
    return return_val<0?-1:0;
}
/*

*/
inline int recv_gaming(){
    for(int i=1;i<=5;i++) if(recv_message()<0) return -1;
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
    return 0;
}