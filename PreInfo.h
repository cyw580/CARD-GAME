#include<windows.h>
void SetPos(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;

    //HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
    //SetConsoleCursorPosition(hout, coord);//和下面的代码一样功能

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
	int shake_time = 25; //休眠的时间，为5毫秒
    int shake_distance = power; //移动了10像素
    RECT rect; //RECT是一个矩形结构体，相当于保存了一个矩形的四条边的坐标
    HWND window = NULL, oldwindow = NULL; //两个窗口句柄
    int x=0, y=0, width=0, height=0; //用来保存窗口横纵坐标和宽度、高度的变量
    int i;
    for (i = 0; i < time; i++) {
        window = GetForegroundWindow(); //拿到活动窗口
        if (window != oldwindow) {
            //获取指定窗口的位置
            GetWindowRect(window, &rect);
            x = rect.left;
            y = rect.top;
            width = rect.right - x;
            height = rect.bottom - y;
            oldwindow = window;
        }
        MoveWindow(window, x - shake_distance, y, width, height, TRUE); //移动窗口，向左移动了10像素，下同
        Sleep(shake_time);  //休眠time毫秒，线程休眠
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
	//0-职业特性,1-燃烧,2-中毒,3-狂暴,4-虚弱,5-治疗,6-迷惑,7-[蓄力],8-[防御],10-神圣意志
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

char bottom[20]={' ','w','s','z','x',' ','q'};//1上，2下，3使用或更改设置，4弃牌，5结束回合或确认，6认输

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
			printf("是竞技之王!!");
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
			printf("无奈败北!!");
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
				printf(pl[now].handcard[i].Name());//牌名
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
	Sleep(3600);//3.6s的情况展示
	system("cls");
	for(int i=1;i<=250;i++){
		SetColor(rad()%16);
		SetPos(rad()%100,rad()%30);
		printf("#%d ",winner);
		printf(pl[winner].name);
		printf("获得了胜利!!");
		Sleep(2);
	}
	SetColor(7);
	system("cls");
	while(_kbhit()) getch();
}

string Card::Intro(){
	if(func==1) return "+4◆";
	if(func==2)return "+1◆并+1◆上限";
	if(func==3)return "所有人+2◆,自己抽一张牌,对方恢复20HP";
	if(func==4)return "对手[燃烧]3回合（击破护盾时才生效）";
	if(func==5)return "自身[狂暴]至回合结束";
	if(func==6)return "对手[中毒]3回合（职业为地精或击破护盾时才生效）";
	if(func==7)return "立刻抽取至多2张牌";
	if(func==8)return "手牌上限+1(最大为6),达到上限后改为+2◆";
	if(func==9)return "HP上限+40";
	if(func==10)return "-1◆上限";
	if(func==11)return "HP上限-60";
	if(func==12)return "+2<★法力>标记";
	if(func==13)return "对手损失现有HP的25%";
	if(func==14)return "对手下回合[狂暴]";
	if(func==15)return "消耗所有◆,造成(40*◆+25)伤害,不受环境/buff影响";
	if(func==16)return "对手-1◆,对方下回合[虚弱],自己-1<★疲惫>";
	if(func==17)return "你在本回合、对手在下回合都[狂暴]";
	if(func==18)return "+2◆";
	if(func==19)return "+1<★成长>标记";
	if(func==20)return "穿透护盾的真实伤害";
	if(func==21)return "对手获得50护甲";
	if(func==22)return "HP上限-20,+1<★牺牲>标记";
	if(func==23)return "自身[治疗]2回合";
	if(func==24)return "HP上限-110,清空<★牺牲>;恢复到满血,清空手牌后补满";
	if(func==25)return "+3◆并-1<★疲惫>标记";
	if(func==26)return "不要太贪心!";
	if(func==27)return "刷1张[虚空垃圾]进入牌库;被弃置时不耗费不生效";
	if(func==28)return "+3◆,刷3张[虚空垃圾]至牌库;被弃置时不耗费不生效";
	if(func==29)return "被弃置时不耗费不生效";
	if(func==30)return "刷1张[虚空垃圾]至牌库";
	if(func==31)return "清空自身Buff，对手恢复20HP，刷1张[虚空垃圾]至牌库";
	if(func==32)return "清空手牌中[虚空垃圾]且每张使对手受60ATK(弃牌则回复60HEAL)";
	if(func==33)return "刷1张[清理虚空]至牌库";
	if(func==34)return "+2<★成长>标记";
	if(func==35)return "对手[治疗]2回合";
	if(func==36)return "自身[燃烧]2回合";
	if(func==37)return "立即清空标记获得◆,抽一张牌";
	if(func==38)return "对手下回合[迷惑]";
	if(func==39)return "双方[治疗]2回合";
	if(func==40)return "将对手牌库的一张牌清除特效和MISS后放入手牌";
	if(func==41)return "此牌在手牌中时,你每打出一张牌ATK+10";
	if(func==42)return "回合结束时,若手牌里全是[神圣意志],直接获胜";
	if(func==43)return "刷1张[神圣意志]进入牌库";
	if(func==44)return "将随机一个手牌槽位变为[神圣意志]";
	if(func==45)return "将对手牌库的一张牌特效改为\"+2◆\"后洗入牌库";
	if(func==46)return "打出后+4◆且随机将对手一个槽位变为[法力结晶]";
	if(func==47)return "打出后随机将对手1张手牌 +1◆消耗";
	if(func==48)return "清空对手手牌中[神圣意志],每清一张让对手+2<★信仰>";
	if(func==49)return "对手下2回合[虚弱]";
	if(func==50)return "对手[燃烧]2回合（击破护盾时才生效）";
	if(func==51)return "对手随机获得1-2回合随机buff";
	if(func==52)return "自身随机获得1-2回合随机buff";
	if(func==53)return "随机选择自身或对手,随机获得1-2回合随机buff";
	if(func==54)return "自身和对手随机获得1-2回合随机buff";
	if(func==55)return "打出后随机将对手2个手牌槽变为[激活思维]";
	if(func==56)return "对手下2回合[迷惑]";
	if(func==57)return "(宝藏)-3<★疲惫>并升级牌库所有牌";
	if(func==58)return "-3<★疲惫>";
	if(func==59)return "(宝藏)-3<★原罪>,对手hp上限-90,清空牌库的[虚空垃圾]";
	if(func==60)return "(宝藏)+2◆,手牌上限+1,增强部分牌库牌,使用后此牌变为[无中生有]";
	if(func==61)return "(宝藏)+2<★牺牲>,+60hp上限";
	if(func==62)return "(宝藏)+3<★法力>,+1◆上限,向牌库刷入3张[法力成长]";
	if(func==63)return "(宝藏)+2<★成长>,清空牌库中[累赘],将等量[累赘]刷入对手牌库";
	if(func==64)return "(宝藏)+3<★信仰>,牌库+2[神圣意志],对手1个手牌槽变为[精神控制]";
	if(func==65)return "被弃置则本回合和下回合[虚弱]";
	if(func==66)return "牌库中所有[法力成长]+15ATK,有30%概率+1<★法力>";
	if(func==67)return "自身[狂暴]至回合结束,抽一张牌";
	if(func==68)return "随机+(1-2个)<★防御>";
	if(func==69)return "本局剩余时间内,自己回合开始时+1<★防御>";
	if(func==70)return "[蓄力之击]+12ATK";
	if(func==71)return "[蓄力之击]+20ATK";
	if(func==72)return "本局剩余时间内,自己回合开始时[蓄力之击]+5ATK";
	if(func==73)return "本局剩余时间内,自己回合开始时+2<★防御>";
	if(func==74)return "本局剩余时间内,自己回合开始时[蓄力之击]+24ATK";
	if(func==75)return "你每打出一张牌时此牌ATK+5,ATK≥400后将[破防]置入手牌";
	if(func==76)return "随机+(2-4个)<★防御>";
	if(func==77)return "对手下回合[虚弱]";
	if(func==78)return "+3◆并+1<★防御>";
	if(func==79)return "本局剩余时间内,自己回合开始时[蓄力之击]+15ATK";
	if(func==80)return "本局剩余时间内,自己回合开始时[蓄力之击]+8ATK";
	if(func==81)return "对手-6<★防御>,[蓄力之击]+40ATK";
	if(func==82)return "每张手牌的HEAL清空后加等量ATK,若HEAL=0则ATK+15";
	if(func==83)return "60% +1<★法力>";
	if(func==84)return "被弃置则下2回合[虚弱]";
	if(func==85)return "+5<★鱼仔>";
	if(func==86)return "+9<★鱼仔>";
	if(func==87)return "+18<★鱼仔>";
	if(func==88)return "如果<★鱼仔>≥30则-15<★鱼仔>并+3◆,否则视为弃置";
	if(func==89)return "直接对敌方造成1.5*<★鱼仔>的伤害,不受环境/buff影响";
	if(func==90)return "恢复量HEAL=2*<★鱼仔>,现有<★鱼仔>数量减半";
	if(func==91)return "恢复已损失HP的20%";
	if(func==92)return "回合结束增加与<★鱼仔>等量的ATK(每回合至多50)";
	if(func==93)return "+22<★鱼仔>";
	if(func==94)return "打出后-3<★鱼仔>,或弃置后对手+10DEF";
	if(func==95)return "如果<★鱼仔>≥15则-6<★鱼仔>并清空双方所有buff,否则视为弃置";
	if(func==96)return "(宝藏)<★鱼仔>*1.5,使用后此牌变为[水中疗养]";
	if(func==97)return "立刻抽取至多3张牌";
	if(func==98)return "增加30点DEF上限";
	if(func==99)return "护盾减半，-2<★荆棘>";
	if(func==100)return "护盾减半，+2<★荆棘>";
	if(func==101)return "对手[燃烧]2回合（击破护盾时才生效）";
	if(func==102)return "将所有的护盾转化为1.75倍伤害攻击对方";
	if(func==103)return "增加70点DEF上限";
	if(func==104)return "(宝藏)HP,DEF上限增加100,使用后此牌变为[圣盾形态]";
	if(func==105)return "减少50DEF上限(若已扣完则扣血量上限),+3◆";
	if(func==106)return "减少100DEF上限(若已扣完则扣血量上限)";
	if(func==107)return "清空对方的DEF,对方+2◆"; 
	if(func==108)return "DEF*=2";
	if(func==109)return "MISS率固定为30%,成功打出使<★筹码>翻倍，否则减半";
	if(func==110)return "+5<★筹码>";
	if(func==111)return "+11<★筹码>";
	if(func==112)return "消耗<★筹码>以回复等量HP,若回满则回复等量DEF,又回满则停止";
	if(func==113)return "对敌人造成<★筹码>等量伤害，无论是否MISS都使<★筹码>清空";
	if(func==114)return "+2◆,+10<★筹码>";
	if(func==115)return "自身[迷惑]2回合"; 
	if(func==116)return "对手[迷惑]2回合";
	if(func==117)return "+3<★筹码>"; 
	if(func==118)return "+18<★筹码>"; 
	if(func==119)return "MISS率固定为97%,被[不完全记忆]夺走时ATK清空";
	if(func==120)return "<★筹码>*1.5";
	if(func==121)return "扣除max(70,HP/5)的HP,<★筹码>*1.5";
	if(func==122)return "(宝藏)MISS率固定为0%,<★筹码>*2";
	if(func==123)return "将<★底牌>全部转化为<★筹码>";
	if(func==124)return "对方所有手牌MISS+20%";
	return "                                                     ";
}

void previous(){
	//以50 ATK，45 HEAL，35 DEF为1费标准量，高费适当调低
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
	//盾卫 
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
	//赌徒 
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
	//攻防之战
	lib[20][1]=(Card){1,0,15,5,0,68,120};//+(1-2)<★防御>
	lib[20][2]=(Card){3,0,45,20,0,69,121};//本局剩余时间内,自己回合开始时+1<★防御>
	lib[20][3]=(Card){1,65,0,0,10,70,122};//[蓄力之击]+15ATK
	lib[20][4]=(Card){2,95,0,0,10,71,123};//[蓄力之击]+25ATK
	lib[20][5]=(Card){3,125,0,0,0,72,124};//本局剩余时间内,自己回合开始时[蓄力之击]+5ATK
	lib[20][6]=(Card){4,110,70,0,0,76,125};
	lib[20][7]=(Card){2,35,25,30,0,70,126};
	lib[20][8]=(Card){7,20,165,65,0,73,127};//本局剩余时间内,自己回合开始时+2<★防御>
	lib[20][9]=(Card){5,180,0,20,0,74,128};//本局剩余时间内,自己回合开始时[蓄力之击]+24ATK
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
	lib[20][20]=(Card){4,135,20,20,0,79,137};//本局剩余时间内,自己回合开始时[蓄力之击]+15ATK
	libcnt[20]=20;
	lib[20][libcnt[20]+1]=(Card){2,80,0,0,0,81,138};
	job[20]={600,5,120,8};
}

string Card::Name(){
	if(id==1) return "[刮痧]";
	if(id==2) return "[刮痧II]";
	if(id==3) return "[回复魔法]";
	if(id==4) return "[初阶火力]";
	if(id==5) return "[护甲]";
	if(id==6) return "[治疗]";
	if(id==7) return "[塔塔开]";
	if(id==8) return "[正常的牌]";
	if(id==9) return "[平凡的牌]";
	if(id==10) return "[普通的牌]";
	if(id==11) return "[回复膜法]";
	if(id==12) return "[终结法术]";
	if(id==13) return "[损坏的魔杖I]";
	if(id==14) return "[损坏的魔杖II]";
	if(id==15) return "[损坏的魔杖III]";
	if(id==16) return "[损坏的魔杖IV]";
	if(id==17) return "[冲锋号令]";
	if(id==-17) return "[冲锋号令II]";
	if(id==18) return "[生产建设]";
	if(id==19) return "[互惠共赢]";
	if(id==-19) return "[互惠共赢II]";
	if(id==20) return "[燃起来了]";
	if(id==21) return "[有能狂怒]";
	if(id==-21) return "[有能狂怒II]";
	if(id==22) return "[淬毒利刃]";
	if(id==23) return "[无中生有]";
	if(id==-23) return "[无中生有II]";
	if(id==24) return "[仓储建设]";
	if(id==25) return "[生命果]";
	if(id==-25) return "[生命果II]";
	if(id==26) return "[防御机制]";
	if(id==27) return "[按时作息]";
	if(id==28) return "[熟练拾荒]";
	if(id==29) return "[灵魂协同]";
	if(id==30) return "[自我唤醒]";
	if(id==31) return "[凝结]";
	if(id==32) return "[点燃灵魂]";
	if(id==33) return "[觉醒]";
	if(id==34) return "[静养]";
	if(id==35) return "[最后倔强]";
	if(id==36) return "[生命之花]";
	if(id==37) return "[过载]";
	if(id==38) return "[涅槃]";
	if(id==39) return "[活力精华]";
	if(id==40) return "[点燃]";
	if(id==41) return "[火焰弹]";
	if(id==42) return "[炎爆]";
	if(id==43) return "[投资]";
	if(id==44) return "[法力屏障]";
	if(id==45) return "[六边形法术]";
	if(id==46) return "[血色怒火]";
	if(id==47) return "[生命腐蚀]";
	if(id==48) return "[焕发活力]";
	if(id==49) return "[会神一击]";
	if(id==50) return "[不稳定元素]";
	if(id==51) return "[火焰把戏]";
	if(id==52) return "[急不可耐]";
	if(id==53) return "[法力溢出]";
	if(id==54) return "[法力成长]";
	if(id==55) return "[坚守阵地]";
	if(id==56) return "[挑衅]";
	if(id==57) return "[冲刺]";
	if(id==58) return "[莽撞]";
	if(id==59) return "[背水一战]";
	if(id==60) return "[持盾冲锋]";
	if(id==61) return "[抛掷燃木]";
	if(id==62) return "[停战协议]";
	if(id==63) return "[决斗]";
	if(id==64) return "[绝对防御]";
	if(id==65) return "[小型急救包]";
	if(id==66) return "[更新装备]";
	if(id==67) return "[愤怒传递]";
	if(id==68) return "[休养生息]";
	if(id==69) return "[提振士气]";
	if(id==70) return "[毒雾弥漫]";
	if(id==71) return "[采集]";
	if(id==72) return "[发育]";
	if(id==73) return "[回馈之击]";
	if(id==74) return "[力量训练]";
	if(id==75) return "[耐力训练]";
	if(id==76) return "[防御训练]";
	if(id==77) return "[全能训练]";
	if(id==78) return "[潜伏攻击]";
	if(id==79) return "[潜在危险]";
	if(id==80) return "[正面交锋]";
	if(id==81) return "[累赘]";
	if(id==82) return "[快速发育]";
	if(id==83) return "[成长转型]";
	if(id==84) return "[虚空重生]";
	if(id==85) return "[虚空休养]";
	if(id==86) return "[暗能累积]";
	if(id==87) return "[虚空魔法]";
	if(id==88) return "[暗影炸弹]";
	if(id==89) return "[虚空凝聚]";
	if(id==90) return "[虚空之力]";
	if(id==91) return "[虚空物质]";
	if(id==92) return "[黑暗气息]";
	if(id==93) return "[虚空能量]";
	if(id==94) return "[暗能涌动]";
	if(id==95) return "[暗能爆发]";
	if(id==96) return "[暗影术]";
	if(id==97) return "[黑洞吞噬]";
	if(id==98) return "[虚空垃圾]";
	if(id==99) return "[清理虚空]";
	if(id==100) return "[激活虚空]";
	if(id==101) return "[说教]";
	if(id==102) return "[治疗光环]";
	if(id==103) return "[不完全记忆]";
	if(id==104) return "[意识混沌]";
	if(id==105) return "[意识反馈]";
	if(id==106) return "[灵修]";
	if(id==107) return "[清除杂念]";
	if(id==108) return "[冥想]";
	if(id==109) return "[法力赋予]";
	if(id==110) return "[法力结晶]";
	if(id==111) return "[禁锢思维I]";
	if(id==112) return "[禁锢思维II]";
	if(id==113) return "[圣洁洗礼]";
	if(id==114) return "[神圣意志]";
	if(id==115) return "[亵渎]";
	if(id==116) return "[激活思维]";
	if(id==117) return "[坚守信念]";
	if(id==118) return "[精神控制]";
	if(id==119) return "[蓄力之击]";
	if(id==120) return "[建立防线I]";
	if(id==121) return "[武装I]";
	if(id==122) return "[进击I]";
	if(id==123) return "[进击III]";
	if(id==124) return "[力量II]";
	if(id==125) return "[坚守]";
	if(id==126) return "[进击II]";
	if(id==127) return "[武装II]";
	if(id==128) return "[力量积累II]";
	if(id==129) return "[力量I]";
	if(id==130) return "[休战调养]";
	if(id==131) return "[威慑]";
	if(id==132) return "[击中弱点]";
	if(id==133) return "[自我成长]";
	if(id==134) return "[骚扰攻击]";
	if(id==135) return "[建立防线II]";
	if(id==136) return "[能量迸发]";
	if(id==137) return "[力量积累I]";
	if(id==138) return "[破防]";
	if(id==139) return "[暗影形态]";
	if(id==140) return "[奥术能量]";
	if(id==141) return "[淹没]";
	if(id==142) return "[自然生长]";
	if(id==143) return "[繁衍]";
	if(id==144) return "[鱼多势众]";
	if(id==145) return "[凝聚力量]";
	if(id==146) return "[指挥进攻]";
	if(id==147) return "[快速愈合]";
	if(id==148) return "[活力药剂]";
	if(id==149) return "[深海血脉]";
	if(id==150) return "[冷血]";
	if(id==151) return "[水中疗养]";
	if(id==152) return "[深海进化]";
	if(id==153) return "[人鱼祝福]";
	if(id==154) return "[突袭]";
	if(id==155) return "[种族延续]";
	if(id==156) return "[无力]";
	if(id==157) return "[净水洗涤]";
	if(id==158) return "[深海滋养]";
	if(id==159) return "[自我防卫]";
	if(id==160) return "[立盾]";
	if(id==161) return "[立盾II]";
	if(id==162) return "[固盾]";
	if(id==163) return "[前进]";
	if(id==164) return "[后退]";
	if(id==165) return "[热情高涨]";
	if(id==166) return "[守护者之怒]";
	if(id==167) return "[固盾II]";
	if(id==168) return "[守护者的审判]";
	if(id==169) return "[圣盾形态]";
	if(id==170) return "[灵巧攻击]";
	if(id==171) return "[紧急自救]";
	if(id==172) return "[防御攻破]";
	if(id==173) return "[强力构筑]";
	if(id==174) return "[赌局开盘]";
	if(id==175) return "[扑克戏弄]";
	if(id==176) return "[赢得头彩]";
	if(id==177) return "[轮回转生]";
	if(id==178) return "[孤注一掷]";
	if(id==179) return "[斜面轮盘]";
	if(id==180) return "[自我欺骗]";
	if(id==181) return "[一叶障目]";
	if(id==182) return "[不劳而获]";
	if(id==183) return "[风险投资]";
	if(id==184) return "[命运]";
	if(id==185) return "[流光溢彩]";
	if(id==186) return "[暗无天日]";
	if(id==187) return "[拉斯维加斯]";
	if(id==188) return "[秩序背叛]";
	if(id==189) return "[神经致幻]";
	if(id==190) return "[蛮力冲撞]";
	return "[未命名]";
}

void occ_func(int x){
	SetColor(7,0);
	int pos1=26,pos2=8;
	if(x==1){
		SetPos(pos1,pos2-1),printf("HP 420   MAX_DEF 120   手牌上限4   ◆3/6   ");printf("   ");
		SetPos(pos1,pos2+1),printf("                                                           ");
		SetPos(pos1,pos2+2),printf("1.[经世] 所有职业技能牌都中规中矩,且都有特效               ");
		SetPos(pos1,pos2+3),printf("2.[拾荒] 若回合开始时没有◆则有80%% +1◆                   ");
		SetPos(pos1,pos2+4),printf("                                                           ");
		SetPos(pos1,pos2+5),printf("                                                           ");
		SetPos(pos1,pos2+6),printf("                                                           ");
	}else if(x==2){
		SetPos(pos1,pos2-1),printf("HP 600   MAX_DEF 80    手牌上限4   ◆3/5   ");printf("   ");SetColor(13);
		SetPos(pos1,pos2+1),printf("<★牺牲>每个标记使ATK增加5                                 ");SetColor(7);
		SetPos(pos1,pos2+2),printf("1.[浴火重生] 每次自己出牌而受到伤害会+1<★牺牲>            ");
		SetPos(pos1,pos2+3),printf("2.[献祭] 一些职业技能牌会通过削弱自己而获得优势            ");
		SetPos(pos1,pos2+4),printf("                                                           ");
		SetPos(pos1,pos2+5),printf("                                                           ");
		SetPos(pos1,pos2+6),printf("                                                           ");
	}else if(x==3){
		SetPos(pos1,pos2-1),printf("HP 320   MAX_DEF 120   手牌上限4   ◆3/7   ");printf("   ");SetColor(13);
		SetPos(pos1,pos2+1),printf("<★法力>下回合获得等于标记数量的◆并清空标记               ");SetColor(7);
		SetPos(pos1,pos2+2),printf("1.[精通] 每回合开始有45%概率 +1<★法力>                    ");
		SetPos(pos1,pos2+3),printf("2.[法力凝聚] 每当1个<★法力>清空时恢复8HP                  ");
		SetPos(pos1,pos2+4),printf("3.[特攻] 无法抽到公共牌库中HEAL≥75的牌                    ");
		SetPos(pos1,pos2+5),printf("                                                           ");
		SetPos(pos1,pos2+6),printf("                                                           ");
	}else if(x==4){
		SetPos(pos1,pos2-1),printf("HP 500   MAX_DEF 240   手牌上限4   ◆3/6   ");printf("   ");SetColor(13);
		SetPos(pos1,pos2+1),printf("<★疲惫>每个标记使攻击力降低4%%                            ");SetColor(7);
		SetPos(pos1,pos2+2),printf("1.[备战状态] 回合开始时-1<★疲惫>                          ");
		SetPos(pos1,pos2+3),printf("2.[尽力出击] 使用攻击牌后+2<★疲惫>                        ");
		SetPos(pos1,pos2+4),printf("3.[装备精良] 每回合开始时若没有护甲 则护甲+30              ");
		SetPos(pos1,pos2+5),printf("4.[无畏] 无法抽到公共牌库中治疗牌                          ");
		SetPos(pos1,pos2+6),printf("                                                           ");
	}else if(x==5){
		SetPos(pos1,pos2-1),printf("HP 280   MAX_DEF 0     手牌上限3   ◆3/4   ");printf("   ");SetColor(13);
		SetPos(pos1,pos2+1),printf("<★成长>每个标记使你ATK+9%% HP上限+10 MAX_DEF+5            ");SetColor(7);
		SetPos(pos1,pos2+2),printf("1.[贪婪] 每回合额外+2◆,使用牌后有80%会抽牌                ");
		SetPos(pos1,pos2+3),printf("2.[健康] 每当获得1个<★成长>时恢复12HP                     ");
		SetPos(pos1,pos2+4),printf("3.[敏捷] 部分牌有穿透护盾攻击的能力                        ");
		SetPos(pos1,pos2+5),printf("4.[与世隔绝] 无法抽到公共牌库中的牌                        ");
		SetPos(pos1,pos2+6),printf("                                                           ");
	}
	else if(x==6){
		SetPos(pos1,pos2-1),printf("HP 450   MAX_DEF 100   手牌上限5   ◆0/5   ");printf("   ");SetColor(13);
		SetPos(pos1,pos2+1),printf("<★原罪>回合开始时每个标记对你造成5点伤害                  ");SetColor(7);
		SetPos(pos1,pos2+2),printf("1.[源于虚空] 开局时0◆,每回合额外+1◆                      ");
		SetPos(pos1,pos2+3),printf("2.[虚空把戏] 弃牌需消耗相应费用并视为打出,ATK与HEAL交换    ");
		SetPos(pos1,pos2+4),printf("3.[舞弊] 每次弃牌时(无论是否视为打出)+1<★原罪>            ");
		SetPos(pos1,pos2+5),printf("4.[与世隔绝] 无法抽到公共牌库中的牌                        ");
		SetPos(pos1,pos2+6),printf("                                                           ");
	}
	else if(x==7){
		SetPos(pos1,pos2-1),printf("HP 450   MAX_DEF 120   手牌上限4   ◆3/6   ");printf("   ");SetColor(13);
		SetPos(pos1,pos2+1),printf("<★信仰>每个标记额外增加2%抽到[神圣意志]的概率             ");SetColor(7);
		SetPos(pos1,pos2+2),printf("1.[虔诚] 手牌中全是[神圣意志]则直接获胜                    ");
		SetPos(pos1,pos2+3),printf("2.[奉献] 若回合结束时没有费用 则+1<★信仰>                 ");
		SetPos(pos1,pos2+4),printf("3.[个人崇拜] 你的非牧师对手牌库中增加[亵渎]牌              ");
		SetPos(pos1,pos2+5),printf("4.[和平] 无法抽到公共牌库中ATK≥80的牌                     ");
		SetPos(pos1,pos2+6),printf("                                                           ");
	}
	else if(x==8){
		SetPos(pos1,pos2-1),printf("HP 400   MAX_DEF 0     手牌上限5   ◆3/6   ");printf("   ");SetColor(13);
		SetPos(pos1,pos2+1),printf("<★鱼仔>每个标记为你挡下3点伤害后消失                      ");SetColor(7);
		SetPos(pos1,pos2+2),printf("1.[快速繁殖] 每回合开始-15HP并+10<★鱼仔>                  ");
		SetPos(pos1,pos2+3),printf("2.[集群攻击] 回合结束时对敌方造成★等量的伤害              ");
		SetPos(pos1,pos2+4),printf("3.[与世隔绝] 无法抽到公共牌库中的牌                        ");
		SetPos(pos1,pos2+5),printf("                                                           ");
		SetPos(pos1,pos2+6),printf("                                                           ");
	}
	else if(x==9){
		SetPos(pos1,pos2-1),printf("HP 420   MAX_DEF 200   手牌上限4   ◆3/6   ");printf("   ");SetColor(13);
		SetPos(pos1,pos2+1),printf("<★荆棘>数量与反弹伤害和溢出伤害相关 初始5点 范围在0到7间  ");SetColor(7);
		SetPos(pos1,pos2+2),printf("1.[反甲] <★荆棘>*10%盾牌吸收伤害转化为真实伤害反击对方    ");
		SetPos(pos1,pos2+3),printf("2.[过载] 100%-<★荆棘>*10%溢出的盾牌转化为真实伤害攻击对方 ");
		SetPos(pos1,pos2+4),printf("3.[力竭] 你的非盾卫对手牌库中增加[防御攻破]牌              ");
		SetPos(pos1,pos2+5),printf("4.[与世隔绝] 无法抽到公共牌库中的牌                        ");
		SetPos(pos1,pos2+6),printf("                                                           ");
	}
	else if(x==10){
		SetPos(pos1,pos2-1),printf("HP 500   MAX_DEF 100   手牌上限4   ◆3/5   ");printf("   ");SetColor(13);
		SetPos(pos1,pos2+1),printf("<★筹码>足量的筹码在战斗中对你有益,上限为1000个            ");
		SetPos(pos1,pos2+2),printf("<★底牌>因[赌局开盘]损失的筹码将被储存,上限为500个         ");SetColor(7);
		SetPos(pos1,pos2+3),printf("1.[赌场] 回合开始时你的第4张牌被自动替换为[赌局开盘]       ");
		SetPos(pos1,pos2+4),printf("2.[借酒消愁] 每次出牌失误回复(卡牌费用+1)*15HP             ");
		SetPos(pos1,pos2+5),printf("3.[亡命之徒] 当HP低于最大HP的50%时出牌MISS得到15%的减少    ");
		SetPos(pos1,pos2+6),printf("4.[与世隔绝] 无法抽到公共牌库中的牌                        ");
	}
	else if(x==11){
		SetPos(pos1,pos2-1),printf("HP ???   MAX_DEF ???   手牌上限?   ◆?/?   ");printf("   ");
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
		printf("宝藏获取条件:触发3次[拾荒]");printf("                 ");
	}
	else if(x==2){
		printf("宝藏获取条件:回合开始时最大HP≤500");printf("                   ");
	}
	else if(x==3){
		printf("宝藏获取条件:回合开始时对手HP少于其HP上限60%");printf("        ");
	}
	else if(x==4){
		printf("宝藏获取条件:回合开始时<★疲惫>≥6");printf("                   ");
	}
	else if(x==5){
		printf("宝藏获取条件:回合开始时最大HP≥350");printf("            ");
	}
	else if(x==6){
		printf("宝藏获取条件:回合开始时<★原罪>≥5");printf("                    ");
	}
	else if(x==7){
		printf("宝藏获取条件:回合开始时<★信仰>≥4且◆≥3");printf("           ");
	}
	else if(x==8){
		printf("宝藏获取条件:回合开始时<★鱼仔>≥24或HP≤120");printf("         ");
	}
	else if(x==9){
		printf("宝藏获取条件:回合开始时DEF上限≥300");printf("                      ");
	}
	else if(x==10){
		printf("宝藏获取条件:回合开始时<★筹码>≥200");printf("                      ");
	}
	else if(x==11){
		printf("                                                     ");
	}
	return;
}

int env_num=8;
string env_name(int id){
	if(id==1) return "血月";
	else if(id==2) return "暴雨";
	else if(id==3) return "大雾";
	else if(id==4) return "晴天";
	else if(id==5) return "彩虹";
	else if(id==6) return "酸雨";
	else if(id==7) return "满月";
	return "阴天";
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
	if(id==1)      return "双方ATK+20%          ";
	else if(id==2) return "双方ATK-20%          ";
	else if(id==3) return "双方MISS+10%          ";
	else if(id==4) return "双方MISS-10%          ";
	else if(id==5) return "双方每回合回复5%最大hp的hp ";
	else if(id==6) return "双方每回合损失5%最大hp的hp ";
	else if(id==7) return "双方每回合额外+1◆   ";
	else           return "无特殊效果                 ";
}
