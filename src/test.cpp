#include<bits/stdc++.h>
#include<windows.h>
#include<conio.h> 
#include<iostream>
using namespace std;

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define SPACE 32
#define ENTER 13
//ASCII 

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

char input;

struct Card{
	string name;
	int cost,func;
	int ATK,DEF,HEAL,MISS;
	int tag;
	void Use(int from,int to);
	int Special(int from,int to);
	string Intro();
};
//Card heap[1005];
//int heapn;
int winner;

struct player{
	string name,job;
	int cost,maxcost,rest,heapn,occ;
	int hp,def,cardcnt,prehp;
	int maxhp,maxdef;
	Card handcard[10],heap[105];
	int used[10];
	int buff[20];
	//0-职业特性,1-燃烧,2-中毒,3-狂暴,4-虚弱,5-治疗
	player(){
		hp=maxhp=400;def=0;cardcnt=4;
		maxdef=100;maxcost=6;
	}
	int hprate(){
		return 10*hp/maxhp;
	}
	void UpdateBuff(){
		for(int i=1;i<=10;i++){
			buff[i]=max(0,buff[i]-1);
		}
	}
}pl[5];

//-------------------------------------------------
int UI();
int Sort(int now);

int Check(int now){
	if(pl[now].hp<1){
		winner=3-now;
		return 1;
	}
	return 0;
}

void Card::Use(int from,int to){
	if(rand()%100<MISS){
		SetPos(0,1);
		printf("操作失误了!");
		if(pl[from].occ==4 && ATK>0) pl[from].buff[0]=1;
		UI();
		return;
	}//失效判定
	int damage=ATK,damage2=ATK,flag;
	if(func) flag=Special(from,to);
	if(pl[from].occ==2 && ATK>0) damage=damage2=ATK+5*pl[from].buff[0];
	if(pl[from].occ==5 && ATK>0) damage=damage2=ATK+6*pl[from].buff[0];
	if(pl[from].buff[3]){
		damage*=2;damage2*=2;
	}
	if(pl[from].occ==4 && pl[from].buff[0]){
		damage*=0.7;damage2*=0.7;
	}
	if(pl[to].def>0 && flag!=1){
		damage=max(0,damage-pl[to].def);
		pl[to].def=max(0,pl[to].def-damage2);
		if(pl[to].def==0){
			Shake(3,1);//击破护盾的动画
		}
	}
	if(damage>0){
		pl[to].hp-=damage;
	}
	if(damage>=150){
		Shake(10,1);
	}
	//进攻
	pl[from].def=min(pl[from].maxdef,pl[from].def+DEF);
	pl[from].hp=min(pl[from].maxhp,pl[from].hp+HEAL);
	//套盾与回血
	if(HEAL<0 && pl[from].occ==2) pl[from].buff[0]++;
	if(pl[from].occ==4 && ATK>0) pl[from].buff[0]=1;
	UI();
	Check(to);Check(from);
}

int Card::Special(int from,int to){
	int damage=ATK;
	if(pl[from].occ==2 && ATK>0) damage=ATK+5*pl[from].buff[0];
	if(pl[from].occ==5 && ATK>0) damage=ATK+6*pl[from].buff[0];
	if(pl[from].buff[3]){
		damage*=2;
	}
	if(func==1){
		pl[from].cost=min(pl[from].maxcost,pl[from].cost+4);
	}
	else if(func==2){
		pl[from].maxcost++;
		pl[from].cost++;
	}
	else if(func==3){
		for(int now=1;now<=2;now++)
			pl[now].cost=min(pl[now].maxcost,pl[now].cost+2);
	}
	else if(func==4){
		if(damage>=pl[to].def)
			pl[to].buff[1]+=3;
	}
	else if(func==5){
		pl[from].buff[3]=1;
	}
	else if(func==6){
		if(damage>=pl[to].def||pl[from].occ==5)
			pl[to].buff[2]+=3;
	}
	else if(func==7){
		for(int i=1;i<=pl[from].cardcnt;i++){
			if(pl[from].used[i]){
				pl[from].used[i]=0;
				pl[from].handcard[i]=pl[from].heap[(rand()%pl[from].heapn)+1];
			}
		}
		pl[from].rest=pl[from].cardcnt;
	}
	else if(func==8){
		pl[from].cardcnt++;
		pl[from].used[pl[from].cardcnt]=1;
	}
	else if(func==9){
		pl[from].maxhp+=80;
	}
	else if(func==10){
		pl[from].maxcost--;
	}
	else if(func==11){
		pl[from].maxhp-=80;
		pl[from].hp=min(pl[from].hp,pl[from].maxhp);
	}
	else if(func==12){
		pl[from].buff[0]+=2;
	}
	else if(func==13){
		pl[to].hp-=pl[to].hp*0.4;
	}
	else if(func==14){
		pl[to].buff[3]=2;
	}
	else if(func==15){
		int da=20+30*pl[from].cost;
		if(pl[from].buff[3]) da=da*2;
		if(pl[from].buff[0]) da=da*0.7;
		pl[from].cost=0;
		if(da<pl[to].def) pl[to].def-=da;
		else pl[to].hp-=da-pl[to].def,pl[to].def=0;
		pl[from].buff[0]=1;
	}
	else if(func==16){
		pl[to].cost=max(0,pl[to].cost-3);
	}
	else if(func==17){
		pl[from].buff[3]=1;
		pl[to].buff[3]=2;
	}
	else if(func==18){
		pl[from].cost=min(pl[from].maxcost,pl[from].cost+2);
	}
	else if(func==19){
		pl[from].buff[0]++;
		pl[from].maxhp+=15;
		pl[from].maxdef+=10;
		pl[from].hp=min(pl[from].maxhp,pl[from].hp+30);
	}
	else if(func==20){
		return 1;
	}
	else if(func==21){
		pl[to].def=min(pl[to].def+50,pl[to].maxdef);
	}
	else if(func==22){
		pl[from].maxhp-=20;
		pl[from].hp=min(pl[from].hp,pl[from].maxhp);
		pl[from].buff[0]++;
	}
	else if(func==23){
		pl[from].buff[5]+=3;
	}
	else if(func==24){
		pl[from].maxhp-=60;
		pl[from].hp=pl[from].maxhp;
		pl[from].buff[0]=0;
	}
	else if(func==25){
		pl[from].buff[0]=0;
		pl[from].cost+=2;
	}
	return 0;
}

string Card::Intro(){
	if(func==1) return "+4◆";
	else if(func==2)return "+1◆并+1◆上限";
	else if(func==3)return "所有人+2◆";
	else if(func==4)return "对方被点燃3回合（每回合损失40HP）（击破护盾时才生效）";
	else if(func==5)return "自身进入狂暴状态至回合结束（造成的伤害变为原来的2倍）";
	else if(func==6)return "对方中毒3回合（每回合损失现有HP的20%）（职业为地精或击破护盾时才生效）";
	else if(func==7)return "立刻补充你的手牌";
	else if(func==8)return "手牌上限+1";
	else if(func==9)return "HP上限+80";
	else if(func==10)return "-1◆上限";
	else if(func==11)return "HP上限-80";
	else if(func==12)return "下回合额外+2◆";
	else if(func==13)return "对方损失现有HP的40%";
	else if(func==14)return "对方下回合结束前 对方进入狂暴状态（造成的伤害变为原来的2倍）";
	else if(func==15)return "消耗所有费用，造成（30*费用+20）的伤害";
	else if(func==16)return "对方-3◆";
	else if(func==17)return "你在本回合、对方在下回合都进入狂暴状态（造成的伤害变为原来的2倍）";
	else if(func==18)return "+2◆";
	else if(func==19)return "+1<★成长>标记";
	else if(func==20)return "穿透护盾的真实伤害";
	else if(func==21)return "对方获得50护甲";
	else if(func==22)return "HP上限-20，+1<★牺牲>标记";
	else if(func==23)return "接下去3回合在每回合开始时恢复30HP";
	else if(func==24)return "HP上限-60，清空<★牺牲>标记，恢复到满血";
	else if(func==25)return "+2◆并清除<★疲惫>标记";
	return "";
}

void init(int x){
	string s=pl[x].job+".inf";
	freopen("public.inf","r",stdin);
	int tot;
	scanf("%d",&tot);
	for(int i=1;i<=tot;i++){
		cin>>pl[x].heap[i].name;
		scanf("%d%d%d%d%d%d",&pl[x].heap[i].cost,&pl[x].heap[i].ATK,&pl[x].heap[i].HEAL,&pl[x].heap[i].DEF,&pl[x].heap[i].MISS,&pl[x].heap[i].func);
		pl[x].heap[i].tag=i;
		if(pl[x].occ==4 && pl[x].heap[i].HEAL>0) i--,tot--;
		if(pl[x].occ==5) i--,tot--;
	}
	pl[x].heapn=tot;
	fclose(stdin);
	ifstream fin;
	fin.open(s);
	fin>>tot;
	for(int i=pl[x].heapn+1;i<=pl[x].heapn+tot;i++){
		fin>>pl[x].heap[i].name>>pl[x].heap[i].cost>>pl[x].heap[i].ATK>>pl[x].heap[i].HEAL>>pl[x].heap[i].DEF>>pl[x].heap[i].MISS>>pl[x].heap[i].func;
		pl[x].heap[i].tag=i;
	}
	pl[x].heapn+=tot;
	for(int i=1;i<=pl[x].cardcnt;i++){
		pl[x].handcard[i]=pl[x].heap[(rand()%pl[x].heapn)+1];
	}
	fin>>pl[x].maxhp>>pl[x].cardcnt>>pl[x].maxdef>>pl[x].maxcost;
	pl[x].hp=pl[x].maxhp;
	fin.close();
	if(pl[x].occ==3) pl[x].buff[0]=1;
}

string occ_name(int x){
	if(x==1)return "浪人";
	else if(x==2)return "术士";
	else if(x==3)return "法师";
	else if(x==4)return "战士";
	else if(x==5)return "地精";
	return "";
}

string occ_file(int x){
	if(x==1)return "traveller";
	else if(x==2)return "caster";
	else if(x==3)return "wizard";
	else if(x==4)return "fighter";
	else if(x==5)return "goblin";
	return "";
}

string occ_intro(int x){
	if(x==1)return "没有长处，也同样没有弱点";
	else if(x==2)return "牺牲吾沸腾热血，取敌之生命";
	else if(x==3)return "火力即正义，防御不过次要";
	else if(x==4)return "坚实的防御，强大的力量";
	else if(x==5)return "更灵活更敏捷，通过发育获得优势";
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
		printf("\n\t<★疲惫>标记存在时攻击力变为原来的60%%");printf("         ");SetColor(7);
		printf("\n\t 1.[备战状态] 回合开始时标记清空");printf("                ");
		printf("\n\t 2.[尽力出击] 使用攻击牌后获得<★疲惫>标记");printf("                    ");
		printf("\n\t 3.[装备精良] 每回合开始时若没有护甲 则护甲+30");printf("                 ");
		printf("\n\t 4.[无畏] 无法抽到公共牌库中治疗牌");printf("  ");
	}else if(x==5){
		printf("HP 260   MAX_DEF 0   手牌上限4   ◆3");printf("   ");SetColor(13);
		printf("\n\t<★成长>每个标记使你ATK+6 HP上限+15 MAX_DEF+10");printf("             ");SetColor(7);
		printf("\n\t 1.[贪婪] 回合开始时变为3◆，使用牌后有80%会抽牌");printf("           ");
		printf("\n\t 2.[健康] 每当获得1个<★成长>标记时恢复30HP");printf("                  ");
		printf("\n\t 3.[与世隔绝] 无法抽到公共牌库中的牌");printf("          ");
		printf("\n\t 4.[敏捷] 部分牌有穿透护盾攻击的能力");
	}
	return;
}

void Choose(int now){
	SetPos(0,5);
	printf("◇选择P%d的职业◇",now);
	int curse=1;
	while(1){
		for(int i=1;i<=5;i++){	
			if(curse!=i)
				SetColor(7,0);
			else
				SetColor(0,7);
			SetPos(0,5+i);
			printf("                                                             ");
			SetPos(0,5+i);
			printf("%d ",i);
			printf(occ_name(i));
			printf(" | ");
			printf(occ_intro(i));
		}
		SetPos(0,12);
		occ_func(curse);
		input=getch();
		if(input==UP || input==LEFT)
			curse--;
		if(input==DOWN || input==RIGHT)
			curse++;
		if(input>='1' && input<='5'){
			if(input-'0'==curse){
				pl[now].occ=curse;
				pl[now].job=occ_file(curse);
				return;
			}
			else
				curse=input-'0';
		}
		if(curse>5)curse=1;
		if(curse<1)curse=5;
		if(input==SPACE || input==ENTER || input=='z'){
			pl[now].occ=curse;
			pl[now].job=occ_file(curse);
			return;
		}
	}
}

int UI(){
	for(int rnk=1;rnk<=2;rnk++){
		int i;
		//编号/名字/职业
		SetColor(15);
		SetPos(5,rnk*4);
		printf("#%d ",rnk);
		printf(pl[rnk].name);
		SetPos(5,rnk*4+1);
		printf("   ");
		printf(occ_name(pl[rnk].occ));
		
		//buff0
		if(pl[rnk].occ>=2){
			SetColor(13);
			SetPos(8,rnk*4+2);
			printf("★%d   ",pl[rnk].buff[0]);
		}

		//费用绘制
		SetColor(11);
		SetPos(17,rnk*4);
		printf("%d/%d ",pl[rnk].cost,pl[rnk].maxcost);
		SetPos(26,rnk*4);
		for(i=1;i<=pl[rnk].cost;i++)
			printf("◆");
		for(;i<=pl[rnk].maxcost;i++)
			printf("◇");
		printf("                                               ");
		
		//血量绘制
		if(pl[rnk].hprate()>5)SetColor(10);
		else if(pl[rnk].hprate()>2)SetColor(6);
		else SetColor(12);
		SetPos(17,rnk*4+1);
		printf("%d/%d            ",pl[rnk].hp,pl[rnk].maxhp);

		//护甲绘制
		SetColor(15);
		SetPos(17,rnk*4+2);
		printf("[%d/%d]            ",pl[rnk].def,pl[rnk].maxdef);
		
		//Buff绘制
		SetPos(26,rnk*4+2);
		if(pl[rnk].buff[1])
			SetColor(6),printf("燃烧 %d : ",pl[rnk].buff[1]);
		if(pl[rnk].buff[2])
			SetColor(2),printf("中毒 %d : ",pl[rnk].buff[2]);
		if(pl[rnk].buff[3])
			SetColor(4),printf("狂暴 %d : ",pl[rnk].buff[3]);
		if(pl[rnk].buff[4])
			SetColor(4),printf("XX %d : ",pl[rnk].buff[4]);
		if(pl[rnk].buff[5])
			SetColor(4),printf("治疗 %d : ",pl[rnk].buff[5]);//修改颜色
		printf("                                                 ");
		
		//血条绘制
		SetPos(26,rnk*4+1);
		if(pl[rnk].hprate()>5)SetColor(10);
		else if(pl[rnk].hprate()>2)SetColor(6);
		else SetColor(12);
		
		for(i=1;i<= pl[rnk].hp/40;i++)
			printf("■");
		SetColor(8);
		for(;i<= pl[rnk].prehp/40;i++)
			printf("■");
		if(pl[rnk].hprate()>5)SetColor(10);
		else if(pl[rnk].hprate()>2)SetColor(6);
		else SetColor(12);
		for(;i<=pl[rnk].maxhp/40;i++)
			printf("□");
		SetColor(7);
		for(i=1;i<=pl[rnk].def/20;i++)
			printf("■");
		printf("                                                       ");
	}
	return 0; 
}

int Sort(int now){//补充手牌
	for(int i=1;i<=pl[now].cardcnt;i++){
		if(pl[now].used[i]){
			pl[now].handcard[i]=pl[now].heap[(rand()%pl[now].heapn)+1];
			pl[now].used[i]=0;
		}
	}
	return 0;
}

int Ask(int now){
	int Row=13,option=0;
	if(pl[now].occ==3){
		if(pl[now].buff[0]){
			pl[now].cost=min(pl[now].maxcost,pl[now].cost+pl[now].buff[0]);
			pl[now].buff[0]=0;
		}
		if(rand()%100<40) pl[now].buff[0]++;
	}
	if(pl[now].occ==4) {
		if(pl[now].def==0) pl[now].def=30;
		pl[now].buff[0]=0;
	}
	if(pl[now].occ==5) pl[now].cost=3;
	if(pl[now].buff[5])
		pl[now].hp+=30;
	if(pl[now].buff[1])
		pl[now].hp-=40;
	if(pl[now].buff[2])
		pl[now].hp-=pl[now].hp*0.2;
	pl[now].UpdateBuff();
	UI();
	if(Check(now))
		return 0;
	//buff前判
	Sort(now);
	pl[now].rest=pl[now].cardcnt;
	SetColor(7);
	int curse=1;
	SetPos(0,Row);
	printf("P%d",now); 
	SetPos(5,Row);
	printf("  #"); 
	SetPos(11,Row);SetColor(11);
	printf("◆");
	SetPos(18+14,Row);SetColor(7);
	printf("ATK");
	SetPos(26+14,Row);
	printf("HEAL");
	SetPos(34+14,Row);
	printf("DEF");
	SetPos(42+14,Row);
	printf("MISS");
	while(!winner){
		for(int i=1;i<=pl[now].cardcnt;i++){
			int color=0;
			// SetPos(1,11+i);
			// printf("%d",pl[now].used[i]);//debug
			SetPos(5,Row+i);
			if(pl[now].used[i])SetColor(8);
			if(curse==i && !option) SetColor(color=15),printf("◎%d",i); 
			else if(curse==i && option) SetColor(color=15),printf("×%d",i); 
			else SetColor(color=7),printf("  %d",i); 
			if(pl[now].used[i]){
				SetColor(8);
				printf("                                                              ");
				SetPos(14,Row+i);
				printf("[Used.]");
				continue;
			}
			SetPos(11,Row+i);
			if(pl[now].cost>=pl[now].handcard[i].cost)SetColor(11);
			else SetColor(3);
			printf("%d",pl[now].handcard[i].cost);
			SetColor(color);
			SetPos(14,Row+i);
			printf(pl[now].handcard[i].name);
			printf("               ");
			SetPos(18+14,Row+i);
			printf("%d  ",pl[now].handcard[i].ATK);
			SetPos(26+14,Row+i);
			printf("%d  ",pl[now].handcard[i].HEAL);
			SetPos(34+14,Row+i);
			printf("%d  ",pl[now].handcard[i].DEF);
			SetPos(42+14,Row+i);
			printf("%d%%  ",pl[now].handcard[i].MISS);
		}//显示手牌
		SetColor(7);
		SetPos(0,0);
		printf("%d",curse);
		SetPos(11,Row+7);
		printf("                                                                    ");
		SetPos(11,Row+7);
		printf(pl[now].handcard[curse].Intro());
		input=getch();
		if(input==LEFT || input==UP){
			option=0;
			curse--;
			while(pl[now].used[curse]&&curse>0){
				curse--;
				if(curse<=0)curse=pl[now].cardcnt;
			}
		}
		if(input==RIGHT || input==DOWN){
			option=0;
			curse++;
			while(pl[now].used[curse]&&curse<=pl[now].cardcnt+1){
				curse++;
				if(curse>pl[now].cardcnt)curse=1;
			}
		}
		if(curse<=0)curse=pl[now].cardcnt;
		if(curse>pl[now].cardcnt)curse=1;//移动焦点
		if(input=='z')//使用
		{
			option=0;
			if(pl[now].used[curse]){
				SetPos(0,1);
				printf("选中的牌不存在！                 ");
			}
			else if(pl[now].cost<pl[now].handcard[curse].cost){
				SetPos(0,1);
				printf("费用不足无法使用此牌！                 ");
			}
			else{
				pl[now].cost-=pl[now].handcard[curse].cost;
				pl[now].used[curse]=1;
				pl[now].rest--;
				pl[now].handcard[curse].Use(now,3-now);
				if(pl[now].occ==5 && rand()%100<80) {
					pl[now].used[curse]=0;
					pl[now].rest++;
					pl[now].handcard[curse]=pl[now].heap[(rand()%pl[now].heapn)+1];
				}
				while(pl[now].used[curse] && curse<=pl[now].cardcnt+1 && pl[now].rest){
					curse++;
					if(curse>pl[now].cardcnt)curse=1;
				}
			}
		}
		if(input=='x'){//弃牌
			if(!option){
				option=1;
			}
			else{
				pl[now].used[curse]=1;
				pl[now].rest--;
				while(pl[now].used[curse] && curse<=pl[now].cardcnt+1 && pl[now].rest){
					curse++;
					if(curse>pl[now].cardcnt)curse=1;
				}
				option=0;
			}
		}
		if(input==SPACE || input==ENTER || pl[now].rest<=0)//结束回合
			break; 
		Check(now);
	}
	return 0;
}

int main(){
//	SetConsoleOutputCP(65001);
	srand(time(NULL));
	printf("WELCOME!!\n");
	for(int i=1;i<=2;i++){
		printf("请输入P%d的名字:",i);
		cin>>pl[i].name;
	}
	printf("请输入初始费用:");
	int start=0;
	scanf("%d",&start);
	for(int now=1;now<=2;now++){
		Choose(now);//选择职业
		SetPos(0,17+now);
		printf("P%d的职业是",now);
		printf(occ_name(pl[now].occ));
	}
	for(int i=1;i<=2;i++){
		pl[i].cost=min(start,pl[i].maxcost);
	}
	Sleep(1500);
	system("cls");
	for(int i=1;i<=2;i++) init(i);
	int now=1;
	while(!Check(now)){
		for(int i=1;i<=2;i++){
			pl[i].prehp=pl[i].hp;
		}
		if(pl[now].cost==0 && pl[now].occ==1)  pl[now].cost=1;
		pl[now].cost=min(pl[now].cost+1,pl[now].maxcost);//加费 
		UI();
		Ask(now);
		//debug
		system("cls");
		now++;
		if(now>2)now=1;
	}
	system("cls");
	printf("#%d ",winner);
	printf(pl[winner].name);
	printf("获得了胜利!!");
	while(1){
		SetColor(rand()%16);
		SetPos(rand()%100,rand()%30);
		printf("#%d ",winner);
		printf(pl[winner].name);
		printf("获得了胜利!!");
		Sleep(10);
	}
	return 0;
}
