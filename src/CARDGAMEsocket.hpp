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
char charmsg[1010];
char* player_to_char(player pl){
    printf("msg transform start.\n");
    memcpy(charmsg,&pl,sizeof(player));
    printf("msg transform ok.\n");
    return charmsg;
}
int send_message(char* msg_to_client){
    memcpy(charmsg,msg_to_client,sizeof(charmsg));
    int sendval=send(Client,charmsg,strlen(charmsg),0);
    if(sendval<0){
        closesocket(Client); return -1;
    }
    return 0;
}
player precv_message(int &return_val){
    char msg_from_client[1010];
    int recvval=recv(Client,msg_from_client,1010,0);
    if(recvval<0){
        closesocket(Client); return_val+=-1;
    }
    player pl;
    memcpy(&pl,msg_from_client,sizeof(pl));
    return pl;
}
Card crecv_message(int &return_val){
    char msg_from_client[1010];
    int recvval=recv(Client,msg_from_client,1010,0);
    if(recvval<0){
        closesocket(Client); return_val+=-1;
    }
    Card card;
    memcpy(&card,msg_from_client,sizeof(card));
    return card;
}
int recv_message(){
    int return_val=0;
    char msg_from_client[1010];
    return_val+=recv(Client,msg_from_client,4,0);
    int command;
    sscanf(msg_from_client,"%d",&command);
    if(command==2){
        return_val+=recv(Client,msg_from_client,1010,0);
        int pl_num;
        sscanf(msg_from_client,"%d %d",&command,&pl_num);
        pl[pl_num]=precv_message(return_val);
    }
    if(command==3){
        
    }
    return return_val<0?-1:0;
}
/*

*/