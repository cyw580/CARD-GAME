void SetPos(int x, int y)
{
        COORD coord;
        coord.X = x;
        coord.Y = y;

        //HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
        //SetConsoleCursorPosition(hout, coord);//和下面的代码一样功能

        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
};

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

void SetSize(int x,int y){
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
	string name;
	int cost,ATK,DEF,HEAL,MISS,func;
	void Use(int from,int to);
	int Special(int from,int to);
	string Intro();
}lib[11][1005];

int libcnt[11];

struct spocc{
	int maxhp,cardcnt,maxdef,maxcost;
}job[11];

void previous(){
	//public
	lib[0][1]={"[刮痧]",0,15,0,0,0,0};
	lib[0][2]={"[刮痧II]",0,25,0,0,0,0};
	lib[0][3]={"[回复魔法]",0,0,20,0,0,0};
	lib[0][4]={"[初阶火力]",1,60,0,0,15,0};
	lib[0][5]={"[护甲]",1,0,0,40,0,0};
	lib[0][6]={"[治疗]",2,0,80,0,0,0};
	lib[0][7]={"[塔塔开]",2,100,0,0,0,0};
	lib[0][8]={"[正常的牌]",2,60,25,25,20,0};
	lib[0][9]={"[平凡的牌]",2,20,65,30,25,0};
	lib[0][10]={"[普通的牌]",3,105,40,10,25,0};
	lib[0][11]={"[回复膜法]",5,0,115,70,0,0};
	lib[0][12]={"[终结法术]",5,230,0,0,10,0};
	libcnt[0]=12;
	//travaller
	lib[1][1]={"[冲锋号令]",3,60,0,0,0,1};
	lib[1][2]={"[生产建设]",3,0,60,30,0,2};
	lib[1][3]={"[互惠共赢]",1,0,0,0,0,3};
	lib[1][4]={"[燃起来了]",3,70,0,0,0,4};
	lib[1][5]={"[有能狂怒]",2,0,0,0,0,5};
	lib[1][6]={"[淬毒利刃]",3,60,0,0,0,6};
	lib[1][7]={"[无中生有]",3,60,0,0,0,7};
	lib[1][8]={"[仓储建设]",3,60,0,0,0,8};
	lib[1][9]={"[生命果]",3,60,0,0,0,9};
	lib[1][10]={"[防御机制]",3,60,0,0,0,10};
	lib[1][11]={"[按时作息]",3,60,0,0,0,1};
	libcnt[1]=11;
	job[1]={420,4,120,6};
	//caster
	lib[2][1]={"[灵魂协同]",1,50,-30,0,0,0};
	lib[2][2]={"[自我唤醒]",0,0,-25,0,0,0};
	lib[2][3]={"[凝结]",2,0,-70,0,0,1};
	lib[2][4]={"[点燃灵魂]",2,20,-45,0,0,4};
	lib[2][5]={"[觉醒]",1,0,-50,0,0,5};
	lib[2][6]={"[静养]",3,0,100,20,0,0};
	lib[2][7]={"[最后倔强]",1,80,0,0,0,10};
	lib[2][8]={"[生命之花]",2,0,150,0,0,11};
	lib[2][9]={"[过载]",0,0,0,0,0,22};
	lib[2][10]={"[过载]",0,0,0,0,0,22};
	lib[2][11]={"[涅槃]",1,0,0,0,0,24};
	libcnt[2]=11;
	job[2]={600,4,80,5};
	//wizard
	lib[3][1]={"[点燃]",2,35,0,0,0,4};
	lib[3][2]={"[火焰弹]",0,50,0,0,25,0};
	lib[3][3]={"[炎爆]",7,330,0,0,0,0};
	lib[3][4]={"[投资]",1,0,0,0,0,12};
	lib[3][5]={"[法力屏障]",2,20,0,65,0,0};
	lib[3][6]={"[六边形法术]",6,160,40,30,0,12};
	lib[3][7]={"[血色怒火]",2,15,-40,0,0,5};
	lib[3][8]={"[生命腐蚀]",3,0,0,0,0,13};
	lib[3][9]={"[焕发活力]",2,0,40,0,0,12};
	lib[3][10]={"[会神一击]",1,70,0,0,0,0};
	libcnt[3]=10;
	job[3]={320,4,120,7};
	//warrior
	lib[4][1]={"[坚守阵地]",2,0,0,90,0,0};
	lib[4][2]={"[挑衅]",2,0,90,70,0,14};
	lib[4][3]={"[挑衅]",2,0,90,70,0,14};
	lib[4][4]={"[冲刺]",1,80,0,0,15,0};
	lib[4][5]={"[莽撞]",2,130,0,0,25,0};
	lib[4][6]={"[莽撞]",2,130,0,0,25,0};
	lib[4][7]={"[背水一战]",0,0,0,0,0,15};
	lib[4][8]={"[持盾冲锋]",3,110,0,60,0,0};
	lib[4][9]={"[抛掷燃木]",4,90,-50,0,0,4};
	lib[4][10]={"[停战协议]",3,0,30,30,0,16};
	lib[4][11]={"[停战协议]",3,0,30,30,0,16};
	lib[4][12]={"[决斗]",4,225,0,0,0,0};
	lib[4][13]={"[绝对防御]",3,0,0,170,0,0};
	lib[4][14]={"[绝对防御]",3,0,0,170,0,0};
	lib[4][15]={"[小型急救包]",2,0,60,35,0,0};
	lib[4][16]={"[小型急救包]",2,0,60,35,0,0};
	libcnt[4]=16;
	job[4]={500,4,200,6};
	//goblin
	lib[5][1]={"[毒雾弥漫]",2,0,0,0,0,6};
	lib[5][2]={"[采集]",1,0,10,0,0,18};
	lib[5][3]={"[发育]",1,0,0,0,20,19};
	lib[5][4]={"[回馈之击]",3,25,0,0,0,18};
	lib[5][5]={"[力量训练]",2,25,0,0,0,19};
	lib[5][6]={"[力量训练]",2,25,0,0,0,19};
	lib[5][7]={"[耐力训练]",2,0,30,0,0,19};
	lib[5][8]={"[耐力训练]",2,0,30,0,0,19};
	lib[5][9]={"[防御训练]",2,0,0,25,0,19};
	lib[5][10]={"[全能训练]",3,30,20,10,0,19};
	lib[5][11]={"[全能训练]",3,30,20,10,0,19};
	lib[5][12]={"[潜伏攻击]",1,15,0,0,0,20};
	lib[5][13]={"[潜伏攻击]",1,15,0,0,0,20};
	lib[5][14]={"[潜伏攻击]",1,15,0,0,0,20};
	lib[5][15]={"[潜在危险]",3,70,0,0,0,20};
	lib[5][16]={"[正面交锋]",2,65,0,0,0,0};
	lib[5][17]={"[累赘]",4,0,0,0,0,26};
	libcnt[5]=17;
	job[5]={270,3,0,3};
}

string occ_intro(int x){
	if(x==1)return "没有长处，也同样没有弱点";
	else if(x==2)return "牺牲吾沸腾热血，取敌之生命";
	else if(x==3)return "火力即正义，防御不过次要";
	else if(x==4)return "坚实的防御，强大的力量";
	else if(x==5)return "更灵活更敏捷，通过发育获得优势";
	else if(x==6)return "反正我都会玩，随便来一个";
	return "";
}
void occ_func(int x){
	SetColor(7,0);
	if(x==1){
		printf("HP 420   MAX_DEF 120   手牌上限4   ◆6");printf(" ");
		printf("\n\t");printf("                                              ");
		printf("\n\t 1.所有职业技能牌都中规中矩，且都有特效");printf("                  ");
		printf("\n\t 2.[拾荒] 若回合开始时没有费用则获得1点费用");printf("                ");
		printf("\n");printf("                                                      ");
		printf("\n");printf("                                            ");
	}else if(x==2){
		printf("HP 600   MAX_DEF 80   手牌上限4   ◆5");printf("  ");SetColor(13);
		printf("\n\t <★牺牲> 每个标记使ATK增加5");printf("                    ");;SetColor(7);
		printf("\n\t 1.每次自己出牌而受到伤害会+1<★牺牲>标记");printf("               ");
		printf("\n\t 2.一些职业技能牌会通过削弱自己而获得优势");printf("             ");
		printf("\n");printf("                                                      ");
		printf("\n");printf("                                            ");
	}else if(x==3){
		printf("HP 320   MAX_DEF 120   手牌上限4   ◆7");printf(" ");SetColor(13);
		printf("\n\t<★法力>下回合获得等于标记数量的◆并清空标记");printf("          ");SetColor(7);
		printf("\n\t 1.[精通] 每回合开始有40%概率 +1<★法力>标记");printf("               ");
		printf("\n\t 2.[法力凝聚] 一局游戏中第一个回合额外+1◆");printf("                ");
		printf("\n");printf("                                                      ");
		printf("\n");printf("                                            ");
	}else if(x==4){
		printf("HP 500   MAX_DEF 200   手牌上限4   ◆6");printf(" ");SetColor(13);
		printf("\n\t<★疲惫>标记存在时攻击力变为原来的70%%");printf("         ");SetColor(7);
		printf("\n\t 1.[备战状态] 回合开始时<★疲惫>标记清空");printf("                ");
		printf("\n\t 2.[尽力出击] 使用攻击牌后获得<★疲惫>标记");printf("                    ");
		printf("\n\t 3.[装备精良] 每回合开始时若没有护甲 则护甲+30");printf("                 ");
		printf("\n\t 4.[无畏] 无法抽到公共牌库中治疗牌");printf("  ");
	}else if(x==5){
		printf("HP 270   MAX_DEF 0   手牌上限3   ◆3");printf("   ");SetColor(13);
		printf("\n\t<★成长>每个标记使你ATK+8 HP上限+15 MAX_DEF+10");printf("             ");SetColor(7);
		printf("\n\t 1.[贪婪] 回合开始时变为3◆，使用牌后有80%会抽牌");printf("           ");
		printf("\n\t 2.[健康] 每当获得1个<★成长>标记时恢复20HP");printf("                  ");
		printf("\n\t 3.[与世隔绝] 无法抽到公共牌库中的牌");printf("          ");
		printf("\n\t 4.[敏捷] 部分牌有穿透护盾攻击的能力");
	}
	else if(x==6){
		printf("HP ???   MAX_DEF ???   手牌上限?   ◆?");printf(" ");
		printf("\n                                                            ");
		printf("\n                                                            ");
		printf("\n                                                            ");
		printf("\n                                                            ");
		printf("\n                                                            ");
	}
	return;
}