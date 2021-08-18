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
    int x, y, width, height; //用来保存窗口横纵坐标和宽度、高度的变量
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
	printf("                                        ");
	return;
}


//--------------------------------------------------

struct Card{
	char name[21];
	int cost,ATK,HEAL,DEF,MISS,func;
	int Use(int from,int to);
	int Special(int from,int to);
	int cal_atk(int from,int to);
	bool check_atk(int from,int to);
	int cal_heal(int from,int to);
	bool check_heal(int from,int to);
	int cal_def(int from,int to);
	bool check_def(int from,int to);
	int cal_miss(int from,int to);
	bool check_miss(int from,int to);
	string Intro();
}lib[11][1005],fun[11][11][105];//fun[mode][occ][card]

int libcnt[11],funcnt[11][11];//funcnt[mode][occ]

struct spocc{
	int maxhp,cardcnt,maxdef,maxcost;
}job[11];

string Card::Intro(){
	if(func==1) return "+4◆";
	else if(func==2)return "+1◆并+1◆上限";
	else if(func==3)return "所有人+2◆";
	else if(func==4)return "对手[燃烧]3回合（击破护盾时才生效）";
	else if(func==5)return "自身[狂暴]至回合结束";
	else if(func==6)return "对手[中毒]3回合（职业为地精或击破护盾时才生效）";
	else if(func==7)return "立刻补充你的手牌";
	else if(func==8)return "手牌上限+1";
	else if(func==9)return "HP上限+40";
	else if(func==10)return "-1◆上限";
	else if(func==11)return "HP上限-60";
	else if(func==12)return "+2<★法力>标记";
	else if(func==13)return "对手损失现有HP的25%";
	else if(func==14)return "对手下回合[狂暴]";
	else if(func==15)return "消耗所有◆,造成(35*◆+25)伤害,不受环境/buff影响";
	else if(func==16)return "对手-3◆";
	else if(func==17)return "你在本回合、对手在下回合都[狂暴]";
	else if(func==18)return "+2◆";
	else if(func==19)return "+1<★成长>标记";
	else if(func==20)return "穿透护盾的真实伤害";
	else if(func==21)return "对手获得50护甲";
	else if(func==22)return "HP上限-20,+1<★牺牲>标记";
	else if(func==23)return "自身[治疗]3回合";
	else if(func==24)return "HP上限-100,清空<★牺牲>标记;恢复到满血";
	else if(func==25)return "+2◆并-1<★疲惫>标记";
	else if(func==26)return "不要太贪心！";
	else if(func==27)return "刷1张[虚空垃圾]进入牌库;被弃置时不耗费不生效";
	else if(func==28)return "+3◆,刷3张[虚空垃圾]至牌库;被弃置时不耗费不生效";
	else if(func==29)return "被弃置时不耗费不生效";
	else if(func==30)return "刷1张[虚空垃圾]至牌库";
	else if(func==31)return "清空自身Buff，对手恢复20HP，刷1张[虚空垃圾]至牌库";
	else if(func==32)return "清空手牌中[虚空垃圾]且每张[虚空垃圾]使对手受60ATK";
	else if(func==33)return "刷1张[清理虚空]至牌库";
	else if(func==34)return "+2<★成长>标记";
	else if(func==35)return "对手[治疗]2回合";
	else if(func==36)return "自身[燃烧]2回合";
	else if(func==37)return "立即清空标记获得◆";
	else if(func==38)return "对手下回合[迷惑]";
	else if(func==39)return "双方[治疗]2回合";
	else if(func==40)return "将对手牌库的一张牌清除特效和MISS后放入手牌";
	else if(func==41)return "此牌在手牌中时，你每出一张牌ATK+10(无论是否MISS)";
	else if(func==42)return "回合结束时,若手牌里全是[神圣意志],直接获胜";
	else if(func==43)return "刷1张[神圣意志]进入牌库";
	else if(func==44)return "将随机一个手牌槽位变为[神圣意志]";
	else if(func==45)return "将对手牌库的一张牌特效改为\"+2◆\"后洗入牌库";
	else if(func==46)return "打出后+4◆且随机将对手一个槽位变为[法力结晶]";
	else if(func==47)return "打出后随机将对手1张手牌 +1◆消耗";
	else if(func==48)return "清空对手手牌中[神圣意志],每清一张让对手+2<★信仰>";
	else if(func==49)return "对手下2回合[虚弱]";
	else if(func==50)return "对手[燃烧]2回合";
	else if(func==51)return "对手随机获得1-2回合随机buff";
	else if(func==52)return "自身随机获得1-2回合随机buff";
	else if(func==53)return "随机选择自身或对手,随机获得1-2回合随机buff";
	else if(func==54)return "自身和对手随机获得1-2回合随机buff";
	else if(func==55)return "打出后随机将对手2个手牌槽位变为[激活思维]";
	else if(func==56)return "对手下2回合[迷惑]";
	else if(func==57)return "(宝藏)-2<★疲惫>并升级牌库所有牌";
	else if(func==58)return "-3<★疲惫>";
	else if(func==59)return "(宝藏)-3<★原罪>,对手hp上限-80,清空牌库的[虚空垃圾]";
	else if(func==60)return "(宝藏)+2◆,手牌上限+1,增强部分牌库牌,使用后此牌变为[无中生有]";
	else if(func==61)return "(宝藏)+1<★牺牲>,+60hp上限";
	else if(func==62)return "(宝藏)+3<★法力>,+1◆上限，向牌库刷入2张[法力成长]";
	else if(func==63)return "(宝藏)+2<★成长>,清空牌库中[累赘],将等量[累赘]刷入对手牌库";
	else if(func==64)return "(宝藏)+3<★信仰>,向牌库中刷入[神圣意志][精神控制]各2张";
	else if(func==65)return "随机选对手牌库一张牌,清除特效和MISS后视为你打出且不耗费";
	else if(func==66)return "25% +1<★法力>,牌库中所有[法力成长] +15ATK";
	return "";
}

void previous(){
	//以50 ATK，45 HEAL，35 DEF为1费标准量，高费适当调低
	//public
	lib[0][1]=(Card){"[刮痧]",0,15,0,0,0,0};
	lib[0][2]=(Card){"[刮痧II]",0,25,0,0,0,0};
	lib[0][3]=(Card){"[回复魔法]",0,0,20,0,0,0};
	lib[0][4]=(Card){"[初阶火力]",1,55,0,0,15,0};
	lib[0][5]=(Card){"[护甲]",1,0,0,35,0,0};
	lib[0][6]=(Card){"[治疗]",2,0,75,0,0,0};
	lib[0][7]=(Card){"[塔塔开]",2,95,0,0,0,0};
	lib[0][8]=(Card){"[正常的牌]",2,65,35,25,15,0};
	lib[0][9]=(Card){"[平凡的牌]",2,25,75,35,25,0};
	lib[0][10]=(Card){"[普通的牌]",3,120,45,20,25,0};
	lib[0][11]=(Card){"[回复膜法]",5,0,130,75,0,0};
	lib[0][12]=(Card){"[终结法术]",5,240,0,0,10,0};
	libcnt[0]=12;
	fun[2][0][1]=(Card){"[损坏的魔杖I]",1,0,0,0,0,51};
	fun[2][0][2]=(Card){"[损坏的魔杖II]",1,0,0,0,0,52};
	fun[2][0][3]=(Card){"[损坏的魔杖III]",1,0,0,0,0,53};
	fun[2][0][4]=(Card){"[损坏的魔杖IV]",1,0,0,0,0,54};
	funcnt[2][0]=4;
	//travaller
	lib[1][1]=(Card){"[冲锋号令]",3,45,0,0,0,1};
	lib[1][2]=(Card){"[生产建设]",3,0,40,25,0,2};
	lib[1][3]=(Card){"[互惠共赢]",1,0,0,0,0,3};
	lib[1][4]=(Card){"[燃起来了]",3,50,0,0,0,4};
	lib[1][5]=(Card){"[有能狂怒]",2,0,0,0,0,5};
	lib[1][6]=(Card){"[淬毒利刃]",3,45,0,0,0,6};
	lib[1][7]=(Card){"[无中生有]",0,0,0,0,25,7};
	lib[1][8]=(Card){"[仓储建设]",4,0,30,50,0,8};
	lib[1][9]=(Card){"[生命果]",2,0,60,0,0,9};
	lib[1][10]=(Card){"[防御机制]",1,0,0,80,0,21};
	lib[1][11]=(Card){"[按时作息]",2,0,30,0,0,23};
	libcnt[1]=11;
	fun[1][1][1]=(Card){"[熟练拾荒]",1,0,0,0,0,60};//宝藏
	funcnt[1][1]=1;
	job[1]={420,4,120,6};
	//caster
	lib[2][1]=(Card){"[灵魂协同]",1,55,-40,0,0,0};
	lib[2][2]=(Card){"[自我唤醒]",0,0,-25,0,0,0};
	lib[2][3]=(Card){"[凝结]",2,0,-75,0,0,1};
	lib[2][4]=(Card){"[点燃灵魂]",2,35,-45,0,0,50};
	lib[2][5]=(Card){"[觉醒]",1,0,-60,0,0,5};
	lib[2][6]=(Card){"[静养]",3,0,120,35,0,0};
	lib[2][7]=(Card){"[最后倔强]",1,80,0,0,0,10};
	lib[2][8]=(Card){"[生命之花]",2,0,160,0,0,11};
	lib[2][9]=(Card){"[过载]",0,0,0,0,0,22};
	lib[2][10]=(Card){"[过载]",0,0,0,0,0,22};
	lib[2][11]=(Card){"[涅槃]",1,0,0,0,0,24};
	lib[2][12]=(Card){"[凝结]",2,0,-70,0,0,1};
	libcnt[2]=12;
	fun[1][2][1]=(Card){"[活力精华]",1,1,70,0,0,61};//宝藏
	funcnt[2][1]=1;
	job[2]={600,4,80,5};
	//wizard
	lib[3][1]=(Card){"[点燃]",2,25,0,0,0,50};
	lib[3][2]=(Card){"[火焰弹]",0,45,0,0,30,0};
	lib[3][3]=(Card){"[炎爆]",7,330,0,0,0,0};
	lib[3][4]=(Card){"[投资]",1,0,0,0,0,12};
	lib[3][5]=(Card){"[法力屏障]",2,20,0,55,0,0};
	lib[3][6]=(Card){"[六边形法术]",6,135,35,30,0,12};
	lib[3][7]=(Card){"[血色怒火]",2,15,-45,0,0,5};
	lib[3][8]=(Card){"[生命腐蚀]",2,0,0,0,0,13};
	lib[3][9]=(Card){"[焕发活力]",2,0,35,0,0,12};
	lib[3][10]=(Card){"[会神一击]",1,65,0,0,0,0};
	lib[3][11]=(Card){"[不稳定元素]",2,155,50,0,40,35};
	lib[3][12]=(Card){"[火焰把戏]",3,185,0,0,20,36};//燃烧自己2回合
	lib[3][13]=(Card){"[急不可耐]",1,0,-20,0,0,37};//立即清空标记获得法力
	libcnt[3]=13;
	fun[1][3][1]=(Card){"[法力溢出]",2,0,120,0,0,62};//宝藏
	fun[1][3][2]=(Card){"[法力成长]",2,70,0,0,0,66};//衍生牌
	funcnt[1][3]=2;
	job[3]={320,4,120,7};
	//warrior
	lib[4][1]=(Card){"[坚守阵地]",2,0,0,95,0,0};
	lib[4][2]=(Card){"[挑衅]",1,0,50,55,0,14};
	lib[4][3]=(Card){"[挑衅]",1,0,50,55,0,14};
	lib[4][4]=(Card){"[冲刺]",1,80,0,0,15,0};
	lib[4][5]=(Card){"[莽撞]",2,135,0,0,20,0};
	lib[4][6]=(Card){"[莽撞]",2,135,0,0,20,0};
	lib[4][7]=(Card){"[背水一战]",0,0,0,0,0,15};
	lib[4][8]=(Card){"[持盾冲锋]",3,125,0,60,0,0};
	lib[4][9]=(Card){"[抛掷燃木]",4,125,-30,0,0,4};
	lib[4][10]=(Card){"[停战协议]",2,0,10,10,0,16};
	lib[4][11]=(Card){"[决斗]",4,245,0,0,10,0};
	lib[4][12]=(Card){"[绝对防御]",3,0,0,150,0,0};
	lib[4][13]=(Card){"[小型急救包]",2,0,55,30,0,0};
	lib[4][14]=(Card){"[小型急救包]",2,0,55,30,0,0};
	lib[4][15]=(Card){"[更新装备]",1,0,0,10,0,25};
	lib[4][16]=(Card){"[愤怒传递]",2,20,-30,0,0,17};
	lib[4][17]=(Card){"[休养生息]",2,0,50,0,0,58};
	libcnt[4]=17;
	fun[1][4][1]=(Card){"[提振士气]",1,0,0,0,0,57};//宝藏
	funcnt[1][4]=1;
	job[4]={500,4,240,6};
	//goblin
	lib[5][1]=(Card){"[毒雾弥漫]",2,0,0,0,0,6};
	lib[5][2]=(Card){"[采集]",1,0,10,0,0,18};
	lib[5][3]=(Card){"[发育]",1,0,0,0,20,19};
	lib[5][4]=(Card){"[回馈之击]",3,30,0,0,0,18};
	lib[5][5]=(Card){"[力量训练]",2,50,0,0,0,19};
	lib[5][6]=(Card){"[力量训练]",2,50,0,0,0,19};
	lib[5][7]=(Card){"[耐力训练]",2,0,50,0,0,19};
	lib[5][8]=(Card){"[防御训练]",2,0,0,40,0,19};
	lib[5][9]=(Card){"[全能训练]",3,40,35,25,0,19};
	lib[5][10]=(Card){"[全能训练]",3,40,35,25,0,19};
	lib[5][11]=(Card){"[潜伏攻击]",1,25,0,0,0,20};
	lib[5][12]=(Card){"[潜伏攻击]",1,25,0,0,0,20};
	lib[5][13]=(Card){"[潜伏攻击]",1,25,0,0,0,20};
	lib[5][14]=(Card){"[潜在危险]",3,80,0,0,0,20};
	lib[5][15]=(Card){"[正面交锋]",2,65,0,0,0,0};
	lib[5][16]=(Card){"[累赘]",4,0,0,0,0,26};
	lib[5][17]=(Card){"[发育]",1,0,0,0,20,19};
	lib[5][18]=(Card){"[快速发育]",3,0,35,0,0,34};
	libcnt[5]=18;
	fun[1][5][1]=(Card){"[成长转型]",3,0,75,0,0,63};//宝藏
	funcnt[1][5]=1;
	job[5]={280,3,0,3};
	//demon
	lib[6][1]=(Card){"[虚空重生]",1,0,45,0,0,31};
    lib[6][2]=(Card){"[虚空休养]",2,0,70,0,0,33};
	lib[6][3]=(Card){"[暗能累积]",2,60,20,0,10,0};
	lib[6][4]=(Card){"[虚空魔法]",1,20,20,10,0,30};
    lib[6][5]=(Card){"[暗影炸弹]",5,220,30,30,0,29};
    lib[6][6]=(Card){"[虚空凝聚]",1,0,0,0,0,28};
    lib[6][7]=(Card){"[虚空之力]",2,95,0,0,0,30};
    lib[6][8]=(Card){"[虚空物质]",3,100,20,30,0,30};
    lib[6][9]=(Card){"[黑暗气息]",0,15,0,0,0,0};
    lib[6][10]=(Card){"[虚空能量]",4,160,25,10,0,27};
    lib[6][11]=(Card){"[暗能涌动]",3,90,40,0,10,0};
    lib[6][12]=(Card){"[暗能爆发]",4,130,35,0,10,29};
    lib[6][13]=(Card){"[虚空重生]",1,0,45,0,0,31};
    lib[6][14]=(Card){"[暗影术]",1,45,0,0,10,0};
    lib[6][15]=(Card){"[暗能累积]",2,65,20,0,10,0};
    lib[6][16]=(Card){"[暗影术]",1,45,0,0,10,0};
	lib[6][17]=(Card){"[黑洞吞噬]",3,90,50,0,0,33};
    libcnt[6]=17;
	lib[6][libcnt[6]+1]=(Card){"[虚空垃圾]",1,0,0,0,0,27};
	lib[6][libcnt[6]+2]=(Card){"[清理虚空]",2,0,0,0,0,32};
	fun[1][6][1]=(Card){"[激活虚空]",2,80,45,0,0,59};//宝藏
	funcnt[1][6]=1;
	job[6]={400,4,40,5};
	//minister
	lib[7][1]=(Card){"[说教]",1,30,0,0,0,38};//对手下回合[迷惑]
	lib[7][2]=(Card){"[治疗光环]",2,0,65,0,0,39};//双方获得2回合治疗
	lib[7][3]=(Card){"[不完全记忆]",1,0,20,0,15,40};//将对手牌库的一张牌清除特效和MISS后放入手牌
	lib[7][4]=(Card){"[意识混沌]",2,0,40,20,0,56};//对手下2回合[迷惑]
	lib[7][5]=(Card){"[意识反馈]",1,0,0,0,0,41};//此牌在手牌中时，你每出一张牌ATK+10(无论是否MISS)
	lib[7][6]=(Card){"[灵修]",2,10,70,0,0,43};//刷1张[神圣意志]进入牌库
	lib[7][7]=(Card){"[清除杂念]",1,0,40,0,0,43};
	lib[7][8]=(Card){"[冥想]",2,0,50,15,0,44};//将随机一个手牌槽位变为[神圣意志]
	lib[7][9]=(Card){"[法力赋予]",2,0,45,25,0,45};//将对手牌库的一张牌特效改为"+2◆"后洗入牌库
	lib[7][10]=(Card){"[法力结晶]",2,0,0,0,0,46};//打出后+4◆且随机将对手一个槽位变为[法力结晶]
	lib[7][11]=(Card){"[禁锢思维I]",2,0,60,10,0,47};//打出后随机将对手1张手牌 +1◆消耗
	lib[7][12]=(Card){"[禁锢思维II]",2,0,25,10,0,55};//打出后随机将对手2张手牌变为[激活思维]
	lib[7][13]=(Card){"[意识反馈]",1,0,0,0,0,41};
	lib[7][14]=(Card){"[圣洁洗礼]",3,40,55,0,0,49};//对手[虚弱]2回合
	libcnt[7]=14;
	lib[7][libcnt[7]+1]=(Card){"[神圣意志]",4,0,150,50,0,42};//若在手牌中有4张[神圣意志]，直接赢得游戏
	lib[7][libcnt[7]+2]=(Card){"[亵渎]",2,20,0,0,0,48};//清空对手手牌中[神圣意志]，每一张让对手+2<★信仰>
	lib[7][libcnt[7]+3]=(Card){"[激活思维]",0,0,15,0,0,0};
	fun[1][7][1]=(Card){"[坚守信念]",2,0,120,0,0,64};//宝藏
	fun[1][7][2]=(Card){"[精神控制]",0,0,0,0,20,65};//衍生牌
	funcnt[1][7]=2;
	job[7]={480,4,120,6};
}

string occ_intro(int x){
	if(x==1)return "没有长处，也同样没有弱点";
	else if(x==2)return "牺牲吾沸腾热血，取敌之生命";
	else if(x==3)return "火力即正义，防御不过次要";
	else if(x==4)return "坚实的防御，强大的力量";
	else if(x==5)return "更灵活更敏捷，通过发育获得优势";
	else if(x==6)return "受虚空力量滋养，有着多样的特效与出牌方式";
	else if(x==7)return "受到神圣的祝福，强大的治疗与控制能力";
	else if(x==8)return "反正我都会玩，随便来一个";
	return "";
}

void occ_func(int x){
	SetColor(7,0);
	if(x==1){
		printf("HP 420   MAX_DEF 120   手牌上限4   ◆6");printf(" ");
		printf("\n\t");printf("                                                ");
		printf("\n\t 1.所有职业技能牌都中规中矩,且都有特效");printf("                  ");
		printf("\n\t 2.[拾荒] 若回合开始时没有费用则获得1点费用");printf("                 ");
		printf("\n");printf("                                                        ");
		printf("\n");printf("                                               ");
	}else if(x==2){
		printf("HP 600   MAX_DEF 80   手牌上限4   ◆5");printf("  ");SetColor(13);
		printf("\n\t<★牺牲> 每个标记使ATK增加6");printf("                     ");;SetColor(7);
		printf("\n\t 1.每次自己出牌而受到伤害会+1<★牺牲>");printf("               ");
		printf("\n\t 2.一些职业技能牌会通过削弱自己而获得优势");printf("               ");
		printf("\n");printf("                                                        ");
		printf("\n");printf("                                               ");
	}else if(x==3){
		printf("HP 320   MAX_DEF 120   手牌上限4   ◆7");printf(" ");SetColor(13);
		printf("\n\t<★法力>下回合获得等于标记数量的◆并清空标记");printf("          ");SetColor(7);
		printf("\n\t 1.[精通] 每回合开始有35%概率 +1<★法力>");printf("               ");
		printf("\n\t 2.[法力凝聚] 若HP≤100,每当1个<★法力>清空时恢复20HP");printf("         ");
		printf("\n");printf("                                                        ");
		printf("\n");printf("                                               ");
	}else if(x==4){
		printf("HP 500   MAX_DEF 240   手牌上限4   ◆6");printf(" ");SetColor(13);
		printf("\n\t<★疲惫>每个标记使攻击力降低4%%");printf("                      ");SetColor(7);
		printf("\n\t 1.[备战状态] 回合开始时-1<★疲惫>");printf("                ");
		printf("\n\t 2.[尽力出击] 使用攻击牌后+2<★疲惫>");printf("                       ");
		printf("\n\t 3.[装备精良] 每回合开始时若没有护甲 则护甲+45");printf("                 ");
		printf("\n\t 4.[无畏] 无法抽到公共牌库中治疗牌");printf("      ");
	}else if(x==5){
		printf("HP 280   MAX_DEF 0   手牌上限3   ◆3");printf("   ");SetColor(13);
		printf("\n\t<★成长>每个标记使你ATK+8%% HP上限+10 MAX_DEF+10");printf("             ");SetColor(7);
		printf("\n\t 1.[贪婪] 回合开始时变为3◆,使用牌后有80%会抽牌");printf("           ");
		printf("\n\t 2.[健康] 每当获得1个<★成长>时恢复20HP");printf("                    ");
		printf("\n\t 3.[与世隔绝] 无法抽到公共牌库中的牌");printf("            ");
		printf("\n\t 4.[敏捷] 部分牌有穿透护盾攻击的能力");printf("      ");
	}
	else if(x==6){
		printf("HP 400   MAX_DEF 40   手牌上限4   ◆5");printf("   ");SetColor(13);
		printf("\n\t<★原罪>回合开始时每个标记对你造成5点伤害");printf("             ");SetColor(7);
		printf("\n\t 1.[源于虚空] 开局时0◆,每回合额外获得1◆");printf("               ");
		printf("\n\t 2.[虚空把戏] 弃牌需消耗相应费用并视为打出,ATK与HEAL交换");printf("          ");
		printf("\n\t 3.[舞弊] 每次弃牌时(无论是否视为打出)+1<★原罪>");printf("        ");
		printf("\n\t 4.[与世隔绝] 无法抽到公共牌库中的牌");printf("        ");
	}
	else if(x==7){
		printf("HP 480   MAX_DEF 120   手牌上限4   ◆6");printf("   ");SetColor(13);
		printf("\n\t<★信仰>每个标记额外增加2%抽到[神圣意志]的概率");printf("             ");SetColor(7);
		printf("\n\t 1.[虔诚] 手牌中全是[神圣意志]则直接获胜");printf("               ");
		printf("\n\t 2.[奉献] 若回合结束时没有费用 则+1<★信仰>");printf("                 ");
		printf("\n\t 3.[个人崇拜] 你的对手牌库中增加[亵渎]牌");printf("         ");
		printf("\n\t 4.[和平] 无法抽到公共牌库中ATK≥80的牌");printf("          ");
	}
	else if(x==8){
		printf("HP ???   MAX_DEF ???   手牌上限?   ◆?");printf(" ");
		printf("\n                                                                 ");
		printf("\n                                                                 ");
		printf("\n                                                                 ");
		printf("\n                                                                 ");
		printf("\n                                                                 ");
	}
	return;
}
void occ_treasure(int x){
	SetColor(7,0);
	if(x==1){
		printf("宝藏获取条件:回合开始时◆为0且HP≤210");printf("            ");
	}
	else if(x==2){
		printf("宝藏获取条件:回合开始时最大HP≤500");printf("              ");
	}
	else if(x==3){
		printf("宝藏获取条件:回合开始时HP≤160");printf("                   ");
	}
	else if(x==4){
		printf("宝藏获取条件:回合开始时<★疲惫>≥8");printf("               ");
	}
	else if(x==5){
		printf("宝藏获取条件:回合开始时最大HP≥320且HP≤240");printf("     ");
	}
	else if(x==6){
		printf("宝藏获取条件:回合开始时<★原罪>≥5");printf("               ");
	}
	else if(x==7){
		printf("宝藏获取条件:回合开始时<★信仰>≥4且HP≤300");printf("      ");
	}
	else if(x==8){
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
	if(id==1)      return "双方的伤害增加20%          ";
	else if(id==2) return "双方的伤害减少20%          ";
	else if(id==3) return "双方失误率增加10%          ";
	else if(id==4) return "双方失误率减少10%          ";
	else if(id==5) return "双方每回合回复5%最大hp的hp ";
	else if(id==6) return "双方每回合损失5%最大hp的hp ";
	else if(id==7) return "双方每回合额外获得1点费用   ";
	else           return "无特殊效果                 ";
}