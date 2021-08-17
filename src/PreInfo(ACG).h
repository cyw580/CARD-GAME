#include<bits/stdc++.h>
#define iv inline void
using namespace std;
iv moveto(int a,int b) //列 行 
{
	HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coordScreen={short(a),short(b)};
	SetConsoleCursorPosition(hConsole,coordScreen);
}
iv mouse(int a)
{
	HANDLE handle=GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(handle,&CursorInfo);
	CursorInfo.bVisible=a;
	SetConsoleCursorInfo(handle,&CursorInfo);
}
inline int ran(int l,int r)
{
	return rand()%(r-l+1)+l;
}
bool found(vector<string> &t,string a)
{
	vector<string>::iterator it=find(t.begin(),t.end(),a);
    if(it!=t.end()) return true;
    else return false;
}
void SetColor(short x);
#define Set SetColor(7)
iv print(string a){for(int i=0;i<a.size();i++){putchar(a[i]);}}
iv red(string u){SetColor(12);print(u);Set;}
iv blue(string u){SetColor(9);print(u);Set;}
iv green(string u){SetColor(10);print(u);Set;}
iv yellow(string u){SetColor(14);print(u);Set;}
iv cyan(string u){SetColor(11);print(u);Set;}
iv purple(string u){SetColor(13);print(u);Set;}
iv white(string u){SetColor(15);print(u);Set;}
iv grey(string u){SetColor(8);print(u);Set;}
iv putspace(int t){for(int i=1;i<=t;i++) putchar(' ');}
template <typename T>
inline string change(T a){stringstream ss;string s;ss.clear();ss.str("");ss<<a;ss>>s;return s;} 
template <typename T>
inline int change2(T a){stringstream ss;int s;ss.clear();ss.str("");ss<<a;ss>>s;return s;}
struct card{
	string name;
	int cost,effect;
	vector<string> speffect; 
	card(string name_,int cost_,int effect_):
		name(name_),cost(cost_),effect(effect_){} 
};
struct skill{
	string name;
	int mana,active,effect;
	void set_up(string name_,int mana_,int active_,int effect_)
	{
		name=name_,mana=mana_,active=active_,effect=effect_;
	}
};
struct buff{
	string name;
	int tim;
	void set_up(string name_,int tim_)
	{
		name=name_,tim=tim_;
	}
};
int foundbuff(vector<buff> *t,string s)
{
	for(int i=0;i<(t->size());i++)
		if((*t)[i].name==s)
			return i;
	return -1;
}
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define SPACE 32
#define ENTER 13
bool upc(char c){if(c=='W' or c=='w' or c==UP) return true;return false;}
bool downc(char c){if(c=='S' or c=='s' or c==DOWN) return true;return false;}
bool leftc(char c){if(c=='A' or c=='a' or c==LEFT) return true;return false;}
bool rightc(char c){if(c=='D' or c=='d' or c==RIGHT) return true;return false;}
bool surec(char c){if(c==SPACE or c==ENTER) return true;return false;}
bool usec(char c){if(c=='Z' or c=='z' or c=='+') return true;return false;}
bool magicc(char c){if(c=='X' or c=='x' or c=='-') return true;return false;}
string explanation[114];skill theskill[514];
string skillexplanation[114];
string soullist[114];int soullong,cardlong;
string soulexplanation[514];
string cardname[1919];
vector<skill> soulskill[114];
iv init()
{
	srand(time(NULL));
	mouse(0);
	
	cardlong=19;
	cardname[1]="惩罚";
	cardname[2]="附带弃置";
	cardname[3]="埋伏";
	cardname[4]="微小法力";
	cardname[5]="合作共赢";
	cardname[6]="中等法力";
	cardname[7]="意志摧毁";
	cardname[8]="天道轮回";
	cardname[9]="终极惩戒";
	cardname[10]="魔幻把戏";
	cardname[11]="可持久化";
	cardname[12]="1919810";
	cardname[13]="一心同体";
	cardname[14]="万心同体"; 
	cardname[15]="赞美太阳"; 
	cardname[16]="时间流逝";
	cardname[17]="绯红之王";
	cardname[18]="公平竞争";
	cardname[19]="变本加厉";
	
	//牌面 
	explanation[1]="令对手从牌库中抽一张牌";
	explanation[2]="（打出此牌后）随机弃掉自己手牌中另一张牌";
	explanation[3]="向牌堆顶加入一张【阻碍】";
	explanation[4]="+20■";
	explanation[5]="（打出此牌后）双方都随机弃掉一张手牌";
	explanation[6]="+50■";
	explanation[7]="对方-20■";
	explanation[8]="将弃牌堆顶的牌加入对方手牌";
	explanation[9]="将手牌中所有的【惩罚】一同无消耗打出";
	explanation[10]="将手牌中所有◆>2的牌◆-1";
	explanation[11]="获得两回合的【持久】效果（每回合初额外+1◆）"; 
	explanation[12]="哼啊啊啊啊啊（随机触发一种有效果的牌的效果）";
	explanation[13]="将你手牌中所有的【一心同体】弃置";
	explanation[14]="把手牌里一张不是【一心同体】的牌变成【一心同体】";
	explanation[15]="增加 12AM 的概率 10%";
	explanation[16]="时间切换到下一个阶段";
	explanation[17]="随机调整时间";
	explanation[18]="（打出此牌后）手牌多的一方弃一张牌";
	explanation[19]="（打出此牌后）手牌多的一方摸一张牌";
	
	theskill[1].set_up("自由意志",40,1,1);
	theskill[2].set_up("命运指针",70,1,2);
	theskill[3].set_up("顶峰爆裂",0,0,3);
	theskill[4].set_up("混沌重组",50,1,4);
	theskill[5].set_up("两极分化",0,0,5);
	theskill[6].set_up("强行转化",40,1,6);
	theskill[7].set_up("这是l，这是r",0,0,7);
	theskill[8].set_up("这些边我不加",0,0,8);
	theskill[9].set_up("并查集查询",70,1,9);
	theskill[10].set_up("因为太强了所以要削",0,0,10);
	theskill[11].set_up("不明所以",0,0,11); 
	theskill[12].set_up("不知所措",20,1,12);
	
	skillexplanation[1]="将牌堆顶的牌的费用化为0并加入手牌";
	skillexplanation[2]="将自己的随机一张牌给予对方";
	skillexplanation[3]="在回合末时若有且仅有一张费用最大的牌则将其弃置";
	skillexplanation[4]="将双方的手牌混在一起随机重组（手牌数不变）";
	skillexplanation[5]="回合开始时不会抽到◆为偶数的牌"; 
	skillexplanation[6]="+1◆";
	skillexplanation[7]="回合初60%你的一张无此效果的手牌得到【加边！加边！加边！】";
	skillexplanation[8]="回合初你没有带【加边！加边！加边！】的牌则+1◆";
	skillexplanation[9]="弃置你所有带有【加边！加边！加边！】的牌";
	skillexplanation[10]="开局时多摸两张牌";
	skillexplanation[11]="每回合初得到的◆为1~3";
	skillexplanation[12]="+1◆,摸1张牌";
	
	soullong=6;
	soullist[1]="自由";
	soulskill[1].push_back(theskill[1]);
	soulskill[1].push_back(theskill[2]);
	soulexplanation[1]="综合型较强，但魔法耗值高";
	soullist[2]="混沌";
	soulskill[2].push_back(theskill[3]);
	soulskill[2].push_back(theskill[4]);
	soulskill[2].push_back(theskill[10]);
	soulexplanation[2]="需要技巧性出牌，尽量发动被动";
	soullist[3]="暴戾";
	soulskill[3].push_back(theskill[5]);
	soulskill[3].push_back(theskill[6]);
	soulexplanation[3]="积累费用，稳健进攻";
	soullist[4]="谔谔";
	soulskill[4].push_back(theskill[7]);
	soulskill[4].push_back(theskill[8]);
	soulskill[4].push_back(theskill[9]);
	soulexplanation[4]="蓄势待发，后期快速清理手牌";
	soullist[5]="迷茫";
	soulskill[5].push_back(theskill[11]);
	soulskill[5].push_back(theskill[12]);
	soulexplanation[5]="比较靠运气，能快速积攒费用";
	soullist[6]="rand";
	soulexplanation[6]="？？？";
}

//From PreInfo.h
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


void Shake(int power,int time){
	int shake_time = 25;
    int shake_distance = power;
    RECT rect;
    HWND window = NULL, oldwindow = NULL;
    int x, y, width, height;
    int i;
    for (i = 0; i < time; i++) {
        window = GetForegroundWindow();
        if (window != oldwindow) {
            GetWindowRect(window, &rect);
            x = rect.left;
            y = rect.top;
            width = rect.right - x;
            height = rect.bottom - y;
            oldwindow = window;
        }
        MoveWindow(window, x - shake_distance, y, width, height, TRUE); 
        Sleep(shake_time); 
        MoveWindow(window, x - shake_distance, y - shake_distance, width, height, TRUE);
        Sleep(shake_time);
        MoveWindow(window, x, y - shake_distance, width, height, TRUE);
        Sleep(shake_time);
        MoveWindow(window, x, y, width, height, TRUE);
        Sleep(shake_time);
    }
    return;
}

