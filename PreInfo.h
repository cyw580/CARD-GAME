#include<windows.h>
void SetPos(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;

    //HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
    //SetConsoleCursorPosition(hout, coord);//������Ĵ���һ������

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
};

void mouse(int a)
{
	HANDLE handle=GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(handle,&CursorInfo);
	CursorInfo.bVisible=a;
	SetConsoleCursorInfo(handle,&CursorInfo);
}

void SetColor(short x)
{
  	if(x>=0 && x<=15)
    	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), x);
  	else
   	 	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

void SetColor(short x,short y)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), x+16*y);
}

void SetSize(short x,short y){
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SMALL_RECT rc = {1,1,x,y};
	SetConsoleWindowInfo(hOut ,true ,&rc);
}

void printf(string s){
	cout<<s;
}

void Shake(int power,int time){
	int shake_time = 25; //���ߵ�ʱ�䣬Ϊ5����
    int shake_distance = power; //�ƶ���10����
    RECT rect; //RECT��һ�����νṹ�壬�൱�ڱ�����һ�����ε������ߵ�����
    HWND window = NULL, oldwindow = NULL; //�������ھ��
    int x=0, y=0, width=0, height=0; //�������洰�ں�������Ϳ�ȡ��߶ȵı���
    int i;
    for (i = 0; i < time; i++) {
        window = GetForegroundWindow(); //�õ������
        if (window != oldwindow) {
            //��ȡָ�����ڵ�λ��
            GetWindowRect(window, &rect);
            x = rect.left;
            y = rect.top;
            width = rect.right - x;
            height = rect.bottom - y;
            oldwindow = window;
        }
        MoveWindow(window, x - shake_distance, y, width, height, TRUE); //�ƶ����ڣ������ƶ���10���أ���ͬ
        Sleep(shake_time);  //����time���룬�߳�����
        MoveWindow(window, x - shake_distance, y - shake_distance, width, height, TRUE);
        Sleep(shake_time);
        MoveWindow(window, x, y - shake_distance, width, height, TRUE);
        Sleep(shake_time);
        MoveWindow(window, x, y, width, height, TRUE);
        Sleep(shake_time);
    }
    return;
}

void cline(){
	printf("                                                 ");
	return;
}


//--------------------------------------------------

struct Card{
	int cost,ATK,HEAL,DEF,MISS,func,id;
	int Use(int from,int to);
	int Special(int from,int to);
	int cal_atk(int from,int to);
	int cal_heal(int from,int to);
	int cal_def(int from,int to);
	int cal_miss(int from,int to);
	string Name();
	string Intro();
}lib[105][1005],fun[25][105][105];//fun[mode][occ][card]

struct player{
	string name;
	int cost,maxcost,rest,heapn,occ;
	int hp,def,cardcnt,prehp;
	int maxhp,maxdef;
	Card handcard[11],heap[305];
	int used[11];
	int buff[11];
	//0-ְҵ����,1-ȼ��,2-�ж�,3-��,4-����,5-����,6-�Ի�,7-[����],8-[����],10-��ʥ��־
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
}pl[5];

mt19937 rad(std::chrono::system_clock::now().time_since_epoch().count());
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
int turn;
bool adturn;
Card appcard[MAXAPCARDNUM];
int appcnt;
Card void_card=(Card){0,0,0,0,0,-2,0};
int changejob;
int sumjob=10;
bool fight;
int havewon,havelost;
bool gettre[6];
bool if_adv;

char bottom[20]={' ','w','s','z','x',' ','q'};//1�ϣ�2�£�3ʹ�û�������ã�4���ƣ�5�����غϻ�ȷ�ϣ�6����

int libcnt[105],funcnt[105][105];//funcnt[mode][occ]

struct spocc{
	int maxhp,cardcnt,maxdef,maxcost;
}job[105];

void prepare(){
	winner=0;
	now=0;
	env_now=env_cnt=env_rate=0;
	env_on=0;
	Row=11;
	cost_bgn=3;
	player_bgn=1;
	mode=0;
	turn=0;
	adturn=0;
	appcnt=0;
	changejob=0;
	if_adv=0;
	for(int i=1;i<=2;i++){
		pl[i].cost=pl[i].maxcost=pl[i].rest=pl[i].heapn=pl[i].occ=0;
		pl[i].hp=pl[i].def=pl[i].cardcnt=pl[i].prehp=0;
		pl[i].maxhp=pl[i].maxdef=0;
		for(int j=0;j<=10;j++) pl[i].used[j]=pl[i].buff[j]=0;
		gettre[i]=0;
	}
	
}

void fightresult(bool r,int x){
	if(r){
		system("cls");
		for(int i=1;i<=450;i++){
			SetColor(rad()%16);
			SetPos(rad()%100,rad()%30);
			printf("#%d ",x);
			printf(pl[x].name);
			printf("�Ǿ���֮��!!");
			Sleep(5);
		}
		system("cls");
		while(_kbhit()) getch();
	}
	else {
		system("cls");
		for(int i=1;i<=450;i++){
			SetColor(rad()%16);
			SetPos(rad()%100,rad()%30);
			printf("#%d ",x);
			printf(pl[x].name);
			printf("���ΰܱ�!!");
			Sleep(2);
		}
		system("cls");
		while(_kbhit()) getch();
	}
}

void showresult(){
	SetColor(7);
	SetPos(0,Row);
	printf("GAME OVER!!! Result:");
	Row++;
	for(now=1;now<=2;now++){
		SetColor(7);
		SetPos(1,Row);
		printf("P%d:",now);
		for(int i=1;i<=pl[now].cardcnt;i++){
				int color=0;
				// SetPos(1,11+i);
				// printf("%d",pl[now].used[i]);//debug
				SetPos(5,Row+i);
				if(pl[now].used[i])SetColor(8);
				SetColor(color=7),printf("  %d ",i); 
				if(pl[now].used[i]){
					SetColor(8);
					printf("                                                                   ");
					SetPos(14,Row+i);
					printf("[Used.]");
					continue;
				}
				printf(" %d ",pl[now].handcard[i].cost);
				SetColor(color);
				SetPos(14,Row+i);
				printf(pl[now].handcard[i].Name());//����
				SetColor(7,0);
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
		Row+=pl[now].cardcnt+1;
	}
	Sleep(3600);//3.6s�����չʾ
	system("cls");
	for(int i=1;i<=250;i++){
		SetColor(rad()%16);
		SetPos(rad()%100,rad()%30);
		printf("#%d ",winner);
		printf(pl[winner].name);
		printf("�����ʤ��!!");
		Sleep(2);
	}
	SetColor(7);
	system("cls");
	while(_kbhit()) getch();
}

string Card::Intro(){
	if(func==1) return "+4��";
	if(func==2)return "+1����+1������";
	if(func==3)return "������+2��,�Լ���һ����,�Է��ָ�20HP";
	if(func==4)return "����[ȼ��]3�غϣ����ƻ���ʱ����Ч��";
	if(func==5)return "����[��]���غϽ���";
	if(func==6)return "����[�ж�]3�غϣ�ְҵΪ�ؾ�����ƻ���ʱ����Ч��";
	if(func==7)return "���̳�ȡ����2����";
	if(func==8)return "��������+1(���Ϊ6),�ﵽ���޺��Ϊ+2��";
	if(func==9)return "HP����+40";
	if(func==10)return "-1������";
	if(func==11)return "HP����-60";
	if(func==12)return "+2<�﷨��>���";
	if(func==13)return "������ʧ����HP��25%";
	if(func==14)return "�����»غ�[��]";
	if(func==15)return "�������С�,���(40*��+25)�˺�,���ܻ���/buffӰ��";
	if(func==16)return "����-1��,�Է��»غ�[����],�Լ�-1<��ƣ��>";
	if(func==17)return "���ڱ��غϡ��������»غ϶�[��]";
	if(func==18)return "+2��";
	if(func==19)return "+1<��ɳ�>���";
	if(func==20)return "��͸���ܵ���ʵ�˺�";
	if(func==21)return "���ֻ��50����";
	if(func==22)return "HP����-20,+1<������>���";
	if(func==23)return "����[����]2�غ�";
	if(func==24)return "HP����-110,���<������>;�ָ�����Ѫ,������ƺ���";
	if(func==25)return "+3����-1<��ƣ��>���";
	if(func==26)return "��Ҫ̫̰��!";
	if(func==27)return "ˢ1��[�������]�����ƿ�;������ʱ���ķѲ���Ч";
	if(func==28)return "+3��,ˢ3��[�������]���ƿ�;������ʱ���ķѲ���Ч";
	if(func==29)return "������ʱ���ķѲ���Ч";
	if(func==30)return "ˢ1��[�������]���ƿ�";
	if(func==31)return "�������Buff�����ָֻ�20HP��ˢ1��[�������]���ƿ�";
	if(func==32)return "���������[�������]��ÿ��ʹ������60ATK(������ظ�60HEAL)";
	if(func==33)return "ˢ1��[�������]���ƿ�";
	if(func==34)return "+2<��ɳ�>���";
	if(func==35)return "����[����]2�غ�";
	if(func==36)return "����[ȼ��]2�غ�";
	if(func==37)return "������ձ�ǻ�á�,��һ����";
	if(func==38)return "�����»غ�[�Ի�]";
	if(func==39)return "˫��[����]2�غ�";
	if(func==40)return "�������ƿ��һ���������Ч��MISS���������";
	if(func==41)return "������������ʱ,��ÿ���һ����ATK+10";
	if(func==42)return "�غϽ���ʱ,��������ȫ��[��ʥ��־],ֱ�ӻ�ʤ";
	if(func==43)return "ˢ1��[��ʥ��־]�����ƿ�";
	if(func==44)return "�����һ�����Ʋ�λ��Ϊ[��ʥ��־]";
	if(func==45)return "�������ƿ��һ������Ч��Ϊ\"+2��\"��ϴ���ƿ�";
	if(func==46)return "�����+4�������������һ����λ��Ϊ[�����ᾧ]";
	if(func==47)return "��������������1������ +1������";
	if(func==48)return "��ն���������[��ʥ��־],ÿ��һ���ö���+2<������>";
	if(func==49)return "������2�غ�[����]";
	if(func==50)return "����[ȼ��]2�غϣ����ƻ���ʱ����Ч��";
	if(func==51)return "����������1-2�غ����buff";
	if(func==52)return "����������1-2�غ����buff";
	if(func==53)return "���ѡ����������,������1-2�غ����buff";
	if(func==54)return "����Ͷ���������1-2�غ����buff";
	if(func==55)return "��������������2�����Ʋ۱�Ϊ[����˼ά]";
	if(func==56)return "������2�غ�[�Ի�]";
	if(func==57)return "(����)-3<��ƣ��>�������ƿ�������";
	if(func==58)return "-3<��ƣ��>";
	if(func==59)return "(����)-3<��ԭ��>,����hp����-90,����ƿ��[�������]";
	if(func==60)return "(����)+2��,��������+1,��ǿ�����ƿ���,ʹ�ú���Ʊ�Ϊ[��������]";
	if(func==61)return "(����)+2<������>,+60hp����";
	if(func==62)return "(����)+3<�﷨��>,+1������,���ƿ�ˢ��3��[�����ɳ�]";
	if(func==63)return "(����)+2<��ɳ�>,����ƿ���[��׸],������[��׸]ˢ������ƿ�";
	if(func==64)return "(����)+3<������>,�ƿ�+2[��ʥ��־],����1�����Ʋ۱�Ϊ[�������]";
	if(func==65)return "�������򱾻غϺ��»غ�[����]";
	if(func==66)return "�ƿ�������[�����ɳ�]+15ATK,��30%����+1<�﷨��>";
	if(func==67)return "����[��]���غϽ���,��һ����";
	if(func==68)return "���+(1-2��)<�����>";
	if(func==69)return "����ʣ��ʱ����,�Լ��غϿ�ʼʱ+1<�����>";
	if(func==70)return "[����֮��]+12ATK";
	if(func==71)return "[����֮��]+20ATK";
	if(func==72)return "����ʣ��ʱ����,�Լ��غϿ�ʼʱ[����֮��]+5ATK";
	if(func==73)return "����ʣ��ʱ����,�Լ��غϿ�ʼʱ+2<�����>";
	if(func==74)return "����ʣ��ʱ����,�Լ��غϿ�ʼʱ[����֮��]+24ATK";
	if(func==75)return "��ÿ���һ����ʱ����ATK+5,ATK��400��[�Ʒ�]��������";
	if(func==76)return "���+(2-4��)<�����>";
	if(func==77)return "�����»غ�[����]";
	if(func==78)return "+3����+1<�����>";
	if(func==79)return "����ʣ��ʱ����,�Լ��غϿ�ʼʱ[����֮��]+15ATK";
	if(func==80)return "����ʣ��ʱ����,�Լ��غϿ�ʼʱ[����֮��]+8ATK";
	if(func==81)return "����-6<�����>,[����֮��]+40ATK";
	if(func==82)return "ÿ�����Ƶ�HEAL��պ�ӵ���ATK,��HEAL=0��ATK+15";
	if(func==83)return "60% +1<�﷨��>";
	if(func==84)return "����������2�غ�[����]";
	if(func==85)return "+5<������>";
	if(func==86)return "+9<������>";
	if(func==87)return "+18<������>";
	if(func==88)return "���<������>��30��-15<������>��+3��,������Ϊ����";
	if(func==89)return "ֱ�ӶԵз����1.5*<������>���˺�,���ܻ���/buffӰ��";
	if(func==90)return "�ָ���HEAL=2*<������>,����<������>��������";
	if(func==91)return "�ָ�����ʧHP��20%";
	if(func==92)return "�غϽ���������<������>������ATK(ÿ�غ�����50)";
	if(func==93)return "+22<������>";
	if(func==94)return "�����-3<������>,�����ú����+10DEF";
	if(func==95)return "���<������>��15��-6<������>�����˫������buff,������Ϊ����";
	if(func==96)return "(����)<������>*1.5,ʹ�ú���Ʊ�Ϊ[ˮ������]";
	if(func==97)return "���̳�ȡ����3����";
	if(func==98)return "����30��DEF����";
	if(func==99)return "���ܼ��룬-2<�ﾣ��>";
	if(func==100)return "���ܼ��룬+2<�ﾣ��>";
	if(func==101)return "����[ȼ��]2�غϣ����ƻ���ʱ����Ч��";
	if(func==102)return "�����еĻ���ת��Ϊ1.75���˺������Է�";
	if(func==103)return "����70��DEF����";
	if(func==104)return "(����)HP,DEF��������100,ʹ�ú���Ʊ�Ϊ[ʥ����̬]";
	if(func==105)return "����50DEF����(���ѿ������Ѫ������),+3��";
	if(func==106)return "����100DEF����(���ѿ������Ѫ������)";
	if(func==107)return "��նԷ���DEF,�Է�+2��"; 
	if(func==108)return "DEF*=2";
	if(func==109)return "MISS�ʹ̶�Ϊ30%,�ɹ����ʹ<�����>�������������";
	if(func==110)return "+5<�����>";
	if(func==111)return "+11<�����>";
	if(func==112)return "����<�����>�Իظ�����HP,��������ظ�����DEF,�ֻ�����ֹͣ";
	if(func==113)return "�Ե������<�����>�����˺��������Ƿ�MISS��ʹ<�����>���";
	if(func==114)return "+2��,+10<�����>";
	if(func==115)return "����[�Ի�]2�غ�"; 
	if(func==116)return "����[�Ի�]2�غ�";
	if(func==117)return "+3<�����>"; 
	if(func==118)return "+18<�����>"; 
	if(func==119)return "MISS�ʹ̶�Ϊ97%,��[����ȫ����]����ʱATK���";
	if(func==120)return "<�����>*1.5";
	if(func==121)return "�۳�max(70,HP/5)��HP,<�����>*1.5";
	if(func==122)return "(����)MISS�ʹ̶�Ϊ0%,<�����>*2";
	if(func==123)return "��<�����>ȫ��ת��Ϊ<�����>";
	if(func==124)return "�Է���������MISS+20%";
	return "                                                     ";
}

void previous(){
	//��50 ATK��45 HEAL��35 DEFΪ1�ѱ�׼�����߷��ʵ�����
	//public
	lib[0][1]=(Card){0,15,0,0,0,0,1};
	lib[0][2]=(Card){0,25,0,0,0,0,2};
	lib[0][3]=(Card){0,0,20,0,0,0,3};
	lib[0][4]=(Card){1,55,0,0,15,0,4};
	lib[0][5]=(Card){1,0,0,35,0,0,5};
	lib[0][6]=(Card){2,0,75,0,0,0,6};
	lib[0][7]=(Card){2,95,0,0,0,0,7};
	lib[0][8]=(Card){2,65,35,25,15,0,8};
	lib[0][9]=(Card){2,25,70,35,25,0,9};
	lib[0][10]=(Card){3,120,45,20,25,0,10};
	lib[0][11]=(Card){4,0,185,40,0,0,11};
	lib[0][12]=(Card){5,240,0,0,10,0,12};
	lib[0][13]=lib[0][8];
	lib[0][14]=lib[0][9];
	libcnt[0]=14;
	fun[2][0][1]=(Card){1,0,0,0,0,51,13};
	fun[2][0][2]=(Card){1,0,0,0,0,52,14};
	fun[2][0][3]=(Card){1,0,0,0,0,53,15};
	fun[2][0][4]=(Card){1,0,0,0,0,54,16};
	funcnt[2][0]=4;
	//travaller
	lib[1][1]=(Card){3,45,0,0,0,1,17};
	lib[1][2]=(Card){3,0,35,25,0,2,18};
	lib[1][3]=(Card){1,0,0,0,0,3,19};
	lib[1][4]=(Card){3,60,0,0,0,50,20};
	lib[1][5]=(Card){2,0,0,0,0,5,21};
	lib[1][6]=(Card){3,35,0,0,0,6,22};
	lib[1][7]=(Card){0,0,0,0,20,7,23};
	lib[1][8]=(Card){4,0,30,50,0,8,24};
	lib[1][9]=(Card){2,0,60,0,0,9,25};
	lib[1][10]=(Card){1,0,0,75,0,21,26};
	lib[1][11]=(Card){2,0,30,0,0,23,27};
	lib[1][12]=lib[1][3];
	lib[1][13]=lib[1][10];
	lib[1][14]=(Card){3,75,0,0,0,23,159};
	libcnt[1]=14;
	fun[1][1][1]=(Card){1,0,0,0,0,60,28};
	funcnt[1][1]=1;
	job[1]={420,4,120,6};
	//caster
	lib[2][1]=(Card){1,55,-40,0,0,0,29};
	lib[2][2]=(Card){0,0,-25,0,0,0,30};
	lib[2][3]=(Card){2,0,-75,0,0,1,31};
	lib[2][4]=(Card){2,35,-45,0,0,50,32};
	lib[2][5]=(Card){1,0,-60,0,0,5,33};
	lib[2][6]=(Card){3,0,120,35,0,0,34};
	lib[2][7]=(Card){1,80,0,0,0,10,35};
	lib[2][8]=(Card){2,0,150,0,0,11,36};
	lib[2][9]=(Card){0,0,0,0,0,22,37};
	lib[2][10]=lib[2][9];
	lib[2][11]=(Card){1,0,0,0,0,24,38};
	lib[2][12]=lib[2][3];
	libcnt[2]=12;
	fun[1][2][1]=(Card){1,5,75,0,0,61,39};
	funcnt[2][1]=1;
	job[2]={580,4,80,5};
	//wizard
	lib[3][1]=(Card){2,25,0,0,0,50,40};
	lib[3][2]=(Card){0,45,0,0,30,0,41};
	lib[3][3]=(Card){7,330,0,0,0,0,42};
	lib[3][4]=(Card){1,0,0,0,0,12,43};
	lib[3][5]=(Card){2,20,0,55,0,0,44};
	lib[3][6]=(Card){6,135,35,30,0,12,45};
	lib[3][7]=(Card){2,15,-40,0,0,67,46};
	lib[3][8]=(Card){2,0,0,0,0,13,47};
	lib[3][9]=(Card){2,0,35,0,0,12,48};
	lib[3][10]=(Card){1,65,0,0,0,0,49};
	lib[3][11]=(Card){2,125,50,0,40,35,50};
	lib[3][12]=(Card){3,155,0,0,20,36,51};
	lib[3][13]=(Card){1,0,-20,0,0,37,52};
	lib[3][14]=(Card){3,85,40,0,0,83,140};
	libcnt[3]=14;
	fun[1][3][1]=(Card){2,0,120,0,0,62,53};
	fun[1][3][2]=(Card){2,50,0,0,0,66,54};
	funcnt[1][3]=2;
	job[3]={330,4,120,7};
	//warrior
	lib[4][1]=(Card){2,0,0,85,0,0,55};
	lib[4][2]=(Card){1,0,50,55,0,14,56};
	lib[4][3]=lib[4][2];
	lib[4][4]=(Card){1,75,0,0,15,0,57};
	lib[4][5]=(Card){2,120,0,0,20,0,58};
	lib[4][6]=lib[4][5];
	lib[4][7]=(Card){0,0,0,0,0,15,59};
	lib[4][8]=(Card){3,125,0,50,0,0,60};
	lib[4][9]=(Card){4,120,-30,0,0,4,61};
	lib[4][10]=(Card){2,0,20,20,0,16,62};
	lib[4][11]=(Card){4,220,0,0,10,0,63};
	lib[4][12]=(Card){3,0,0,140,0,0,64};
	lib[4][13]=(Card){2,0,55,30,0,0,65};
	lib[4][14]=lib[4][13];
	lib[4][15]=(Card){2,0,0,10,0,25,66};
	lib[4][16]=(Card){2,20,-30,0,0,17,67};
	lib[4][17]=(Card){2,0,50,0,0,58,68};
	libcnt[4]=17;
	fun[1][4][1]=(Card){1,0,10,10,0,57,69};
	funcnt[1][4]=1;
	job[4]={500,4,240,6};
	//goblin
	lib[5][1]=(Card){2,0,0,0,0,6,70};
	lib[5][2]=(Card){1,0,10,0,0,18,71};
	lib[5][3]=(Card){1,0,0,0,20,19,72};
	lib[5][4]=(Card){3,30,0,0,0,18,73};
	lib[5][5]=(Card){2,50,0,0,0,19,74};
	lib[5][6]=lib[5][5];
	lib[5][7]=(Card){2,0,50,0,0,19,75};
	lib[5][8]=(Card){2,0,0,40,0,19,76};
	lib[5][9]=(Card){3,40,35,25,0,19,77};
	lib[5][10]=lib[5][9];
	lib[5][11]=(Card){1,25,0,0,0,20,78};
	lib[5][12]=lib[5][11];
	lib[5][13]=lib[5][11];
	lib[5][14]=(Card){3,80,0,0,0,20,79};
	lib[5][15]=(Card){2,65,0,0,0,0,80};
	lib[5][16]=(Card){5,0,0,0,0,26,81};
	lib[5][17]=lib[5][3];
	lib[5][18]=(Card){3,0,35,0,0,34,82};
	libcnt[5]=18;
	fun[1][5][1]=(Card){3,0,75,0,0,63,83};
	funcnt[1][5]=1;
	job[5]={280,3,0,4};
	//demon
	lib[6][1]=(Card){1,0,45,0,0,31,84};
	lib[6][2]=(Card){2,0,80,0,0,33,85};
	lib[6][3]=(Card){2,75,5,0,10,0,86};
	lib[6][4]=(Card){1,35,5,10,0,30,87};
	lib[6][5]=(Card){5,235,20,20,0,29,88};
	lib[6][6]=(Card){1,0,0,0,0,28,89};
	lib[6][7]=(Card){2,100,0,0,0,30,90};
	lib[6][8]=(Card){3,120,5,50,0,30,91};
	lib[6][9]=(Card){0,15,0,0,0,0,92};
	lib[6][10]=(Card){4,160,10,10,0,27,93};
	lib[6][11]=(Card){3,95,40,0,10,0,94};
	lib[6][12]=(Card){4,140,20,0,10,29,95};
	lib[6][13]=lib[6][1];
	lib[6][14]=(Card){1,45,0,0,10,0,96};
	lib[6][15]=lib[6][3];
	lib[6][16]=lib[6][14];
	lib[6][17]=(Card){3,75,25,0,0,33,97};
	libcnt[6]=17;
	lib[6][libcnt[6]+1]=(Card){1,0,0,0,0,27,98};
	lib[6][libcnt[6]+2]=(Card){2,0,0,0,0,32,99};
	fun[1][6][1]=(Card){2,85,55,0,0,59,100};
	funcnt[1][6]=1;
	job[6]={450,5,100,5};
	//minister
	lib[7][1]=(Card){1,30,0,0,0,38,101};
	lib[7][2]=(Card){2,0,65,0,0,39,102};
	lib[7][3]=(Card){1,0,20,0,15,40,103};
	lib[7][4]=(Card){2,0,40,20,0,56,104};
	lib[7][5]=(Card){1,5,0,0,0,41,105};
	lib[7][6]=(Card){2,10,70,0,0,43,106};
	lib[7][7]=(Card){1,0,40,0,0,43,107};
	lib[7][8]=(Card){2,0,55,15,10,44,108};
	lib[7][9]=(Card){3,0,65,30,0,45,109};
	lib[7][10]=(Card){2,0,0,0,0,46,110};
	lib[7][11]=(Card){2,0,60,10,0,47,111};
	lib[7][12]=(Card){2,0,20,10,0,55,112};
	lib[7][13]=lib[7][5];
	lib[7][14]=(Card){3,40,60,0,0,49,113};
	lib[7][15]=(Card){1,15,0,0,0,82,139};
	libcnt[7]=15;
	lib[7][libcnt[7]+1]=(Card){5,0,180,60,0,42,114};
	lib[7][libcnt[7]+2]=(Card){2,20,0,0,0,48,115};
	lib[7][libcnt[7]+3]=(Card){0,0,15,0,0,0,116};
	fun[1][7][1]=(Card){2,0,120,0,0,64,117};
	fun[1][7][2]=(Card){2,0,0,0,0,65,118};
	funcnt[1][7]=2;
	job[7]={450,4,150,6};
	//Murloc
	lib[8][1]=(Card){2,0,20,0,0,49,141};
	lib[8][2]=(Card){0,6,0,0,0,85,142};
	lib[8][3]=(Card){2,40,15,0,0,86,143};
	lib[8][4]=(Card){4,115,40,0,0,87,144};
	lib[8][5]=(Card){1,0,0,0,0,88,145};
	lib[8][6]=(Card){1,0,-40,0,0,89,146};
	lib[8][7]=(Card){1,0,0,0,0,90,147};
	lib[8][8]=(Card){1,0,45,0,0,0,148};
	lib[8][9]=(Card){3,50,60,0,0,86,149};
	lib[8][10]=(Card){2,90,0,0,0,0,150};
	lib[8][11]=(Card){1,0,0,0,0,91,151};
	lib[8][12]=(Card){6,0,0,0,0,92,152};
	lib[8][13]=(Card){2,0,55,0,0,85,153};
	lib[8][14]=(Card){0,15,0,0,0,0,154};
	lib[8][15]=(Card){2,0,-30,0,0,93,155};
	lib[8][16]=(Card){0,0,0,0,0,94,156};
	lib[8][17]=(Card){1,0,0,0,0,95,157};
	libcnt[8]=17;
	fun[1][8][1]=(Card){1,20,20,0,0,96,158};
	funcnt[1][8]=1;
	job[8]={400,5,0,6};
	//���� 
	lib[9][1]=(Card){1,0,10,45,0,0,160};
	lib[9][2]=lib[9][1];
	lib[9][3]=(Card){2,0,20,105,0,0,161};
	lib[9][4]=(Card){1,0,0,10,0,98,162};
	lib[9][5]=(Card){1,0,0,0,0,99,163};
	lib[9][6]=(Card){1,0,0,0,0,100,164};
	lib[9][7]=(Card)(Card){1,10,10,10,0,101,165};
	lib[9][8]=(Card){3,0,30,30,0,102,166};
	lib[9][9]=lib[9][1];
	lib[9][10]=(Card){3,0,30,50,0,103,167};
	lib[9][11]=(Card){1,0,0,0,0,105,169};
	lib[9][12]=(Card){0,30,10,10,0,0,170};
	lib[9][13]=(Card){1,0,150,30,0,106,171};
	lib[9][14]=(Card){3,0,0,0,0,108,173};
	lib[9][15]=(Card){1,70,0,5,0,0,190};
	libcnt[9]=15;
	lib[9][libcnt[9]+1]=(Card){2,0,0,0,0,107,172};
	fun[1][9][1]=(Card){1,50,50,100,0,104,168};
	funcnt[1][9]=1;
	job[9]={420,4,200,5};
	//��ͽ 
	lib[10][1]=(Card){1,0,-10,0,30,109,174};
	lib[10][2]=(Card){1,10,10,10,15,110,175};
	lib[10][3]=(Card){2,65,-10,0,15,111,176};
	lib[10][4]=(Card){2,0,0,0,0,112,177};
	lib[10][5]=lib[10][2];
	lib[10][6]=(Card){2,0,0,0,30,113,178};
	lib[10][7]=(Card){1,20,-50,0,20,114,179};
	lib[10][8]=(Card){1,0,150,0,25,115,180};
	lib[10][9]=(Card){2,20,-10,0,15,116,181};
	lib[10][10]=(Card){0,10,40,0,30,117,182};
	lib[10][11]=(Card){2,40,10,10,30,118,183};
	lib[10][12]=(Card){0,9999,0,0,97,119,184};
	lib[10][13]=(Card){3,90,50,20,15,120,185};
	lib[10][14]=(Card){1,0,0,0,0,121,186};
	lib[10][15]=(Card){2,0,0,0,0,123,188};
	lib[10][16]=(Card){1,20,-20,0,30,124,189};
	libcnt[10]=16;
	fun[1][10][1]=(Card){1,50,100,100,0,122,187};
	funcnt[1][10]=1;
	job[10]={500,4,100,5};
	//����֮ս
	lib[20][1]=(Card){1,0,15,5,0,68,120};//+(1-2)<�����>
	lib[20][2]=(Card){3,0,45,20,0,69,121};//����ʣ��ʱ����,�Լ��غϿ�ʼʱ+1<�����>
	lib[20][3]=(Card){1,65,0,0,10,70,122};//[����֮��]+15ATK
	lib[20][4]=(Card){2,95,0,0,10,71,123};//[����֮��]+25ATK
	lib[20][5]=(Card){3,125,0,0,0,72,124};//����ʣ��ʱ����,�Լ��غϿ�ʼʱ[����֮��]+5ATK
	lib[20][6]=(Card){4,110,70,0,0,76,125};
	lib[20][7]=(Card){2,35,25,30,0,70,126};
	lib[20][8]=(Card){7,20,165,65,0,73,127};//����ʣ��ʱ����,�Լ��غϿ�ʼʱ+2<�����>
	lib[20][9]=(Card){5,180,0,20,0,74,128};//����ʣ��ʱ����,�Լ��غϿ�ʼʱ[����֮��]+24ATK
	lib[20][10]=(Card){2,75,0,0,0,72,129};
	lib[20][11]=(Card){2,20,20,30,0,77,130};
	lib[20][12]=(Card){4,50,70,50,0,77,131};
	lib[20][13]=(Card){3,85,10,0,0,49,132};
	lib[20][14]=(Card){1,25,0,0,0,38,101};
	lib[20][15]=(Card){2,0,30,10,0,78,133};
	lib[20][16]=(Card){2,85,0,0,0,38,134};
	lib[20][17]=(Card){3,0,45,30,0,76,135};
	lib[20][18]=(Card){0,0,0,0,30,7,23};
	lib[20][19]=(Card){2,40,0,0,0,80,136};
	lib[20][20]=(Card){4,135,20,20,0,79,137};//����ʣ��ʱ����,�Լ��غϿ�ʼʱ[����֮��]+15ATK
	libcnt[20]=20;
	lib[20][libcnt[20]+1]=(Card){2,80,0,0,0,81,138};
	job[20]={600,5,120,8};
}

string Card::Name(){
	if(id==1) return "[����]";
	if(id==2) return "[����II]";
	if(id==3) return "[�ظ�ħ��]";
	if(id==4) return "[���׻���]";
	if(id==5) return "[����]";
	if(id==6) return "[����]";
	if(id==7) return "[������]";
	if(id==8) return "[��������]";
	if(id==9) return "[ƽ������]";
	if(id==10) return "[��ͨ����]";
	if(id==11) return "[�ظ�Ĥ��]";
	if(id==12) return "[�սᷨ��]";
	if(id==13) return "[�𻵵�ħ��I]";
	if(id==14) return "[�𻵵�ħ��II]";
	if(id==15) return "[�𻵵�ħ��III]";
	if(id==16) return "[�𻵵�ħ��IV]";
	if(id==17) return "[������]";
	if(id==-17) return "[������II]";
	if(id==18) return "[��������]";
	if(id==19) return "[���ݹ�Ӯ]";
	if(id==-19) return "[���ݹ�ӮII]";
	if(id==20) return "[ȼ������]";
	if(id==21) return "[���ܿ�ŭ]";
	if(id==-21) return "[���ܿ�ŭII]";
	if(id==22) return "[�㶾����]";
	if(id==23) return "[��������]";
	if(id==-23) return "[��������II]";
	if(id==24) return "[�ִ�����]";
	if(id==25) return "[������]";
	if(id==-25) return "[������II]";
	if(id==26) return "[��������]";
	if(id==27) return "[��ʱ��Ϣ]";
	if(id==28) return "[����ʰ��]";
	if(id==29) return "[���Эͬ]";
	if(id==30) return "[���һ���]";
	if(id==31) return "[����]";
	if(id==32) return "[��ȼ���]";
	if(id==33) return "[����]";
	if(id==34) return "[����]";
	if(id==35) return "[����ǿ]";
	if(id==36) return "[����֮��]";
	if(id==37) return "[����]";
	if(id==38) return "[����]";
	if(id==39) return "[��������]";
	if(id==40) return "[��ȼ]";
	if(id==41) return "[���浯]";
	if(id==42) return "[�ױ�]";
	if(id==43) return "[Ͷ��]";
	if(id==44) return "[��������]";
	if(id==45) return "[�����η���]";
	if(id==46) return "[Ѫɫŭ��]";
	if(id==47) return "[������ʴ]";
	if(id==48) return "[��������]";
	if(id==49) return "[����һ��]";
	if(id==50) return "[���ȶ�Ԫ��]";
	if(id==51) return "[�����Ϸ]";
	if(id==52) return "[��������]";
	if(id==53) return "[�������]";
	if(id==54) return "[�����ɳ�]";
	if(id==55) return "[�������]";
	if(id==56) return "[����]";
	if(id==57) return "[���]";
	if(id==58) return "[çײ]";
	if(id==59) return "[��ˮһս]";
	if(id==60) return "[�ֶܳ��]";
	if(id==61) return "[����ȼľ]";
	if(id==62) return "[ͣսЭ��]";
	if(id==63) return "[����]";
	if(id==64) return "[���Է���]";
	if(id==65) return "[С�ͼ��Ȱ�]";
	if(id==66) return "[����װ��]";
	if(id==67) return "[��ŭ����]";
	if(id==68) return "[������Ϣ]";
	if(id==69) return "[����ʿ��]";
	if(id==70) return "[��������]";
	if(id==71) return "[�ɼ�]";
	if(id==72) return "[����]";
	if(id==73) return "[����֮��]";
	if(id==74) return "[����ѵ��]";
	if(id==75) return "[����ѵ��]";
	if(id==76) return "[����ѵ��]";
	if(id==77) return "[ȫ��ѵ��]";
	if(id==78) return "[Ǳ������]";
	if(id==79) return "[Ǳ��Σ��]";
	if(id==80) return "[���潻��]";
	if(id==81) return "[��׸]";
	if(id==82) return "[���ٷ���]";
	if(id==83) return "[�ɳ�ת��]";
	if(id==84) return "[�������]";
	if(id==85) return "[�������]";
	if(id==86) return "[�����ۻ�]";
	if(id==87) return "[���ħ��]";
	if(id==88) return "[��Ӱը��]";
	if(id==89) return "[�������]";
	if(id==90) return "[���֮��]";
	if(id==91) return "[�������]";
	if(id==92) return "[�ڰ���Ϣ]";
	if(id==93) return "[�������]";
	if(id==94) return "[����ӿ��]";
	if(id==95) return "[���ܱ���]";
	if(id==96) return "[��Ӱ��]";
	if(id==97) return "[�ڶ�����]";
	if(id==98) return "[�������]";
	if(id==99) return "[�������]";
	if(id==100) return "[�������]";
	if(id==101) return "[˵��]";
	if(id==102) return "[���ƹ⻷]";
	if(id==103) return "[����ȫ����]";
	if(id==104) return "[��ʶ����]";
	if(id==105) return "[��ʶ����]";
	if(id==106) return "[����]";
	if(id==107) return "[�������]";
	if(id==108) return "[ڤ��]";
	if(id==109) return "[��������]";
	if(id==110) return "[�����ᾧ]";
	if(id==111) return "[����˼άI]";
	if(id==112) return "[����˼άII]";
	if(id==113) return "[ʥ��ϴ��]";
	if(id==114) return "[��ʥ��־]";
	if(id==115) return "[����]";
	if(id==116) return "[����˼ά]";
	if(id==117) return "[��������]";
	if(id==118) return "[�������]";
	if(id==119) return "[����֮��]";
	if(id==120) return "[��������I]";
	if(id==121) return "[��װI]";
	if(id==122) return "[����I]";
	if(id==123) return "[����III]";
	if(id==124) return "[����II]";
	if(id==125) return "[����]";
	if(id==126) return "[����II]";
	if(id==127) return "[��װII]";
	if(id==128) return "[��������II]";
	if(id==129) return "[����I]";
	if(id==130) return "[��ս����]";
	if(id==131) return "[����]";
	if(id==132) return "[��������]";
	if(id==133) return "[���ҳɳ�]";
	if(id==134) return "[ɧ�Ź���]";
	if(id==135) return "[��������II]";
	if(id==136) return "[�����ŷ�]";
	if(id==137) return "[��������I]";
	if(id==138) return "[�Ʒ�]";
	if(id==139) return "[��Ӱ��̬]";
	if(id==140) return "[��������]";
	if(id==141) return "[��û]";
	if(id==142) return "[��Ȼ����]";
	if(id==143) return "[����]";
	if(id==144) return "[�������]";
	if(id==145) return "[��������]";
	if(id==146) return "[ָ�ӽ���]";
	if(id==147) return "[��������]";
	if(id==148) return "[����ҩ��]";
	if(id==149) return "[�Ѫ��]";
	if(id==150) return "[��Ѫ]";
	if(id==151) return "[ˮ������]";
	if(id==152) return "[�����]";
	if(id==153) return "[����ף��]";
	if(id==154) return "[ͻϮ]";
	if(id==155) return "[��������]";
	if(id==156) return "[����]";
	if(id==157) return "[��ˮϴ��]";
	if(id==158) return "[�����]";
	if(id==159) return "[���ҷ���]";
	if(id==160) return "[����]";
	if(id==161) return "[����II]";
	if(id==162) return "[�̶�]";
	if(id==163) return "[ǰ��]";
	if(id==164) return "[����]";
	if(id==165) return "[�������]";
	if(id==166) return "[�ػ���֮ŭ]";
	if(id==167) return "[�̶�II]";
	if(id==168) return "[�ػ��ߵ�����]";
	if(id==169) return "[ʥ����̬]";
	if(id==170) return "[���ɹ���]";
	if(id==171) return "[�����Ծ�]";
	if(id==172) return "[��������]";
	if(id==173) return "[ǿ������]";
	if(id==174) return "[�ľֿ���]";
	if(id==175) return "[�˿�ϷŪ]";
	if(id==176) return "[Ӯ��ͷ��]";
	if(id==177) return "[�ֻ�ת��]";
	if(id==178) return "[��עһ��]";
	if(id==179) return "[б������]";
	if(id==180) return "[������ƭ]";
	if(id==181) return "[һҶ��Ŀ]";
	if(id==182) return "[���Ͷ���]";
	if(id==183) return "[����Ͷ��]";
	if(id==184) return "[����]";
	if(id==185) return "[�������]";
	if(id==186) return "[��������]";
	if(id==187) return "[��˹ά��˹]";
	if(id==188) return "[������]";
	if(id==189) return "[���»�]";
	if(id==190) return "[������ײ]";
	return "[δ����]";
}

void occ_func(int x){
	SetColor(7,0);
	int pos1=26,pos2=8;
	if(x==1){
		SetPos(pos1,pos2-1),printf("HP 420   MAX_DEF 120   ��������4   ��3/6   ");printf("   ");
		SetPos(pos1,pos2+1),printf("                                                           ");
		SetPos(pos1,pos2+2),printf("1.[����] ����ְҵ�����ƶ��й��о�,�Ҷ�����Ч               ");
		SetPos(pos1,pos2+3),printf("2.[ʰ��] ���غϿ�ʼʱû�С�����80%% +1��                   ");
		SetPos(pos1,pos2+4),printf("                                                           ");
		SetPos(pos1,pos2+5),printf("                                                           ");
		SetPos(pos1,pos2+6),printf("                                                           ");
	}else if(x==2){
		SetPos(pos1,pos2-1),printf("HP 600   MAX_DEF 80    ��������4   ��3/5   ");printf("   ");SetColor(13);
		SetPos(pos1,pos2+1),printf("<������>ÿ�����ʹATK����5                                 ");SetColor(7);
		SetPos(pos1,pos2+2),printf("1.[ԡ������] ÿ���Լ����ƶ��ܵ��˺���+1<������>            ");
		SetPos(pos1,pos2+3),printf("2.[�׼�] һЩְҵ�����ƻ�ͨ�������Լ����������            ");
		SetPos(pos1,pos2+4),printf("                                                           ");
		SetPos(pos1,pos2+5),printf("                                                           ");
		SetPos(pos1,pos2+6),printf("                                                           ");
	}else if(x==3){
		SetPos(pos1,pos2-1),printf("HP 320   MAX_DEF 120   ��������4   ��3/7   ");printf("   ");SetColor(13);
		SetPos(pos1,pos2+1),printf("<�﷨��>�»غϻ�õ��ڱ�������ġ�����ձ��               ");SetColor(7);
		SetPos(pos1,pos2+2),printf("1.[��ͨ] ÿ�غϿ�ʼ��45%���� +1<�﷨��>                    ");
		SetPos(pos1,pos2+3),printf("2.[��������] ÿ��1��<�﷨��>���ʱ�ָ�8HP                  ");
		SetPos(pos1,pos2+4),printf("3.[�ع�] �޷��鵽�����ƿ���HEAL��75����                    ");
		SetPos(pos1,pos2+5),printf("                                                           ");
		SetPos(pos1,pos2+6),printf("                                                           ");
	}else if(x==4){
		SetPos(pos1,pos2-1),printf("HP 500   MAX_DEF 240   ��������4   ��3/6   ");printf("   ");SetColor(13);
		SetPos(pos1,pos2+1),printf("<��ƣ��>ÿ�����ʹ����������4%%                            ");SetColor(7);
		SetPos(pos1,pos2+2),printf("1.[��ս״̬] �غϿ�ʼʱ-1<��ƣ��>                          ");
		SetPos(pos1,pos2+3),printf("2.[��������] ʹ�ù����ƺ�+2<��ƣ��>                        ");
		SetPos(pos1,pos2+4),printf("3.[װ������] ÿ�غϿ�ʼʱ��û�л��� �򻤼�+30              ");
		SetPos(pos1,pos2+5),printf("4.[��η] �޷��鵽�����ƿ���������                          ");
		SetPos(pos1,pos2+6),printf("                                                           ");
	}else if(x==5){
		SetPos(pos1,pos2-1),printf("HP 280   MAX_DEF 0     ��������3   ��3/4   ");printf("   ");SetColor(13);
		SetPos(pos1,pos2+1),printf("<��ɳ�>ÿ�����ʹ��ATK+9%% HP����+10 MAX_DEF+5            ");SetColor(7);
		SetPos(pos1,pos2+2),printf("1.[̰��] ÿ�غ϶���+2��,ʹ���ƺ���80%�����                ");
		SetPos(pos1,pos2+3),printf("2.[����] ÿ�����1��<��ɳ�>ʱ�ָ�12HP                     ");
		SetPos(pos1,pos2+4),printf("3.[����] �������д�͸���ܹ���������                        ");
		SetPos(pos1,pos2+5),printf("4.[��������] �޷��鵽�����ƿ��е���                        ");
		SetPos(pos1,pos2+6),printf("                                                           ");
	}
	else if(x==6){
		SetPos(pos1,pos2-1),printf("HP 450   MAX_DEF 100   ��������5   ��0/5   ");printf("   ");SetColor(13);
		SetPos(pos1,pos2+1),printf("<��ԭ��>�غϿ�ʼʱÿ����Ƕ������5���˺�                  ");SetColor(7);
		SetPos(pos1,pos2+2),printf("1.[Դ�����] ����ʱ0��,ÿ�غ϶���+1��                      ");
		SetPos(pos1,pos2+3),printf("2.[��հ�Ϸ] ������������Ӧ���ò���Ϊ���,ATK��HEAL����    ");
		SetPos(pos1,pos2+4),printf("3.[���] ÿ������ʱ(�����Ƿ���Ϊ���)+1<��ԭ��>            ");
		SetPos(pos1,pos2+5),printf("4.[��������] �޷��鵽�����ƿ��е���                        ");
		SetPos(pos1,pos2+6),printf("                                                           ");
	}
	else if(x==7){
		SetPos(pos1,pos2-1),printf("HP 450   MAX_DEF 120   ��������4   ��3/6   ");printf("   ");SetColor(13);
		SetPos(pos1,pos2+1),printf("<������>ÿ����Ƕ�������2%�鵽[��ʥ��־]�ĸ���             ");SetColor(7);
		SetPos(pos1,pos2+2),printf("1.[��] ������ȫ��[��ʥ��־]��ֱ�ӻ�ʤ                    ");
		SetPos(pos1,pos2+3),printf("2.[����] ���غϽ���ʱû�з��� ��+1<������>                 ");
		SetPos(pos1,pos2+4),printf("3.[���˳��] ��ķ���ʦ�����ƿ�������[����]��              ");
		SetPos(pos1,pos2+5),printf("4.[��ƽ] �޷��鵽�����ƿ���ATK��80����                     ");
		SetPos(pos1,pos2+6),printf("                                                           ");
	}
	else if(x==8){
		SetPos(pos1,pos2-1),printf("HP 400   MAX_DEF 0     ��������5   ��3/6   ");printf("   ");SetColor(13);
		SetPos(pos1,pos2+1),printf("<������>ÿ�����Ϊ�㵲��3���˺�����ʧ                      ");SetColor(7);
		SetPos(pos1,pos2+2),printf("1.[���ٷ�ֳ] ÿ�غϿ�ʼ-15HP��+10<������>                  ");
		SetPos(pos1,pos2+3),printf("2.[��Ⱥ����] �غϽ���ʱ�Եз���ɡ�������˺�              ");
		SetPos(pos1,pos2+4),printf("3.[��������] �޷��鵽�����ƿ��е���                        ");
		SetPos(pos1,pos2+5),printf("                                                           ");
		SetPos(pos1,pos2+6),printf("                                                           ");
	}
	else if(x==9){
		SetPos(pos1,pos2-1),printf("HP 420   MAX_DEF 200   ��������4   ��3/6   ");printf("   ");SetColor(13);
		SetPos(pos1,pos2+1),printf("<�ﾣ��>�����뷴���˺�������˺���� ��ʼ5�� ��Χ��0��7��  ");SetColor(7);
		SetPos(pos1,pos2+2),printf("1.[����] <�ﾣ��>*10%���������˺�ת��Ϊ��ʵ�˺������Է�    ");
		SetPos(pos1,pos2+3),printf("2.[����] 100%-<�ﾣ��>*10%����Ķ���ת��Ϊ��ʵ�˺������Է� ");
		SetPos(pos1,pos2+4),printf("3.[����] ��ķǶ��������ƿ�������[��������]��              ");
		SetPos(pos1,pos2+5),printf("4.[��������] �޷��鵽�����ƿ��е���                        ");
		SetPos(pos1,pos2+6),printf("                                                           ");
	}
	else if(x==10){
		SetPos(pos1,pos2-1),printf("HP 500   MAX_DEF 100   ��������4   ��3/5   ");printf("   ");SetColor(13);
		SetPos(pos1,pos2+1),printf("<�����>�����ĳ�����ս���ж�������,����Ϊ1000��            ");
		SetPos(pos1,pos2+2),printf("<�����>��[�ľֿ���]��ʧ�ĳ��뽫������,����Ϊ500��         ");SetColor(7);
		SetPos(pos1,pos2+3),printf("1.[�ĳ�] �غϿ�ʼʱ��ĵ�4���Ʊ��Զ��滻Ϊ[�ľֿ���]       ");
		SetPos(pos1,pos2+4),printf("2.[�������] ÿ�γ���ʧ��ظ�(���Ʒ���+1)*15HP             ");
		SetPos(pos1,pos2+5),printf("3.[����֮ͽ] ��HP�������HP��50%ʱ����MISS�õ�15%�ļ���    ");
		SetPos(pos1,pos2+6),printf("4.[��������] �޷��鵽�����ƿ��е���                        ");
	}
	else if(x==11){
		SetPos(pos1,pos2-1),printf("HP ???   MAX_DEF ???   ��������?   ��?/?   ");printf("   ");
		SetPos(pos1,pos2+1),printf("                                                           ");
		SetPos(pos1,pos2+2),printf("                                                           ");
		SetPos(pos1,pos2+3),printf("                                                           ");
		SetPos(pos1,pos2+4),printf("                                                           ");
		SetPos(pos1,pos2+5),printf("                                                           ");
		SetPos(pos1,pos2+6),printf("                                                           ");
	}
	return;
}
void occ_treasure(int x){
	SetColor(7,0);
	if(x==1){
		printf("���ػ�ȡ����:����3��[ʰ��]");printf("                 ");
	}
	else if(x==2){
		printf("���ػ�ȡ����:�غϿ�ʼʱ���HP��500");printf("                   ");
	}
	else if(x==3){
		printf("���ػ�ȡ����:�غϿ�ʼʱ����HP������HP����60%");printf("        ");
	}
	else if(x==4){
		printf("���ػ�ȡ����:�غϿ�ʼʱ<��ƣ��>��6");printf("                   ");
	}
	else if(x==5){
		printf("���ػ�ȡ����:�غϿ�ʼʱ���HP��350");printf("            ");
	}
	else if(x==6){
		printf("���ػ�ȡ����:�غϿ�ʼʱ<��ԭ��>��5");printf("                    ");
	}
	else if(x==7){
		printf("���ػ�ȡ����:�غϿ�ʼʱ<������>��4�ҡ���3");printf("           ");
	}
	else if(x==8){
		printf("���ػ�ȡ����:�غϿ�ʼʱ<������>��24��HP��120");printf("         ");
	}
	else if(x==9){
		printf("���ػ�ȡ����:�غϿ�ʼʱDEF���ޡ�300");printf("                      ");
	}
	else if(x==10){
		printf("���ػ�ȡ����:�غϿ�ʼʱ<�����>��200");printf("                      ");
	}
	else if(x==11){
		printf("                                                     ");
	}
	return;
}

int env_num=8;
string env_name(int id){
	if(id==1) return "Ѫ��";
	else if(id==2) return "����";
	else if(id==3) return "����";
	else if(id==4) return "����";
	else if(id==5) return "�ʺ�";
	else if(id==6) return "����";
	else if(id==7) return "����";
	return "����";
}

int env_color(int id){
	if(id==1) return 12;
	else if(id==2) return 15;
	else if(id==3) return 8;
	else if(id==4) return 11;
	else if(id==5) return rand()%6+9;
	else if(id==6) return 2;
	else if(id==7) return 15;
	return 7;
}

string env_brief(int id){
	if(id==1)      return "˫��ATK+20%          ";
	else if(id==2) return "˫��ATK-20%          ";
	else if(id==3) return "˫��MISS+10%          ";
	else if(id==4) return "˫��MISS-10%          ";
	else if(id==5) return "˫��ÿ�غϻظ�5%���hp��hp ";
	else if(id==6) return "˫��ÿ�غ���ʧ5%���hp��hp ";
	else if(id==7) return "˫��ÿ�غ϶���+1��   ";
	else           return "������Ч��                 ";
}
