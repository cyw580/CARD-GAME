#include<bits/stdc++.h>
#define iv inline void
using namespace std;
iv moveto(int a,int b) //�� �� 
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
	cardname[1]="�ͷ�";
	cardname[2]="��������";
	cardname[3]="���";
	cardname[4]="΢С����";
	cardname[5]="������Ӯ";
	cardname[6]="�еȷ���";
	cardname[7]="��־�ݻ�";
	cardname[8]="����ֻ�";
	cardname[9]="�ռ��ͽ�";
	cardname[10]="ħ�ð�Ϸ";
	cardname[11]="�ɳ־û�";
	cardname[12]="1919810";
	cardname[13]="һ��ͬ��";
	cardname[14]="����ͬ��"; 
	cardname[15]="����̫��"; 
	cardname[16]="ʱ������";
	cardname[17]="糺�֮��";
	cardname[18]="��ƽ����";
	cardname[19]="�䱾����";
	
	//���� 
	explanation[1]="����ִ��ƿ��г�һ����";
	explanation[2]="��������ƺ���������Լ���������һ����";
	explanation[3]="���ƶѶ�����һ�š��谭��";
	explanation[4]="+20��";
	explanation[5]="��������ƺ�˫�����������һ������";
	explanation[6]="+50��";
	explanation[7]="�Է�-20��";
	explanation[8]="�����ƶѶ����Ƽ���Է�����";
	explanation[9]="�����������еġ��ͷ���һͬ�����Ĵ��";
	explanation[10]="�����������С�>2���ơ�-1";
	explanation[11]="������غϵġ��־á�Ч����ÿ�غϳ�����+1����"; 
	explanation[12]="�߰������������������һ����Ч�����Ƶ�Ч����";
	explanation[13]="�������������еġ�һ��ͬ�塿����";
	explanation[14]="��������һ�Ų��ǡ�һ��ͬ�塿���Ʊ�ɡ�һ��ͬ�塿";
	explanation[15]="���� 12AM �ĸ��� 10%";
	explanation[16]="ʱ���л�����һ���׶�";
	explanation[17]="�������ʱ��";
	explanation[18]="��������ƺ����ƶ��һ����һ����";
	explanation[19]="��������ƺ����ƶ��һ����һ����";
	
	theskill[1].set_up("������־",40,1,1);
	theskill[2].set_up("����ָ��",70,1,2);
	theskill[3].set_up("���屬��",0,0,3);
	theskill[4].set_up("��������",50,1,4);
	theskill[5].set_up("�����ֻ�",0,0,5);
	theskill[6].set_up("ǿ��ת��",40,1,6);
	theskill[7].set_up("����l������r",0,0,7);
	theskill[8].set_up("��Щ���Ҳ���",0,0,8);
	theskill[9].set_up("���鼯��ѯ",70,1,9);
	theskill[10].set_up("��Ϊ̫ǿ������Ҫ��",0,0,10);
	theskill[11].set_up("��������",0,0,11); 
	theskill[12].set_up("��֪����",20,1,12);
	
	skillexplanation[1]="���ƶѶ����Ƶķ��û�Ϊ0����������";
	skillexplanation[2]="���Լ������һ���Ƹ���Է�";
	skillexplanation[3]="�ڻغ�ĩʱ�����ҽ���һ�ŷ�����������������";
	skillexplanation[4]="��˫�������ƻ���һ��������飨���������䣩";
	skillexplanation[5]="�غϿ�ʼʱ����鵽��Ϊż������"; 
	skillexplanation[6]="+1��";
	skillexplanation[7]="�غϳ�60%���һ���޴�Ч�������Ƶõ����ӱߣ��ӱߣ��ӱߣ���";
	skillexplanation[8]="�غϳ���û�д����ӱߣ��ӱߣ��ӱߣ���������+1��";
	skillexplanation[9]="���������д��С��ӱߣ��ӱߣ��ӱߣ�������";
	skillexplanation[10]="����ʱ����������";
	skillexplanation[11]="ÿ�غϳ��õ��ġ�Ϊ1~3";
	skillexplanation[12]="+1��,��1����";
	
	soullong=6;
	soullist[1]="����";
	soulskill[1].push_back(theskill[1]);
	soulskill[1].push_back(theskill[2]);
	soulexplanation[1]="�ۺ��ͽ�ǿ����ħ����ֵ��";
	soullist[2]="����";
	soulskill[2].push_back(theskill[3]);
	soulskill[2].push_back(theskill[4]);
	soulskill[2].push_back(theskill[10]);
	soulexplanation[2]="��Ҫ�����Գ��ƣ�������������";
	soullist[3]="����";
	soulskill[3].push_back(theskill[5]);
	soulskill[3].push_back(theskill[6]);
	soulexplanation[3]="���۷��ã��Ƚ�����";
	soullist[4]="����";
	soulskill[4].push_back(theskill[7]);
	soulskill[4].push_back(theskill[8]);
	soulskill[4].push_back(theskill[9]);
	soulexplanation[4]="���ƴ��������ڿ�����������";
	soullist[5]="��ã";
	soulskill[5].push_back(theskill[11]);
	soulskill[5].push_back(theskill[12]);
	soulexplanation[5]="�ȽϿ��������ܿ��ٻ��ܷ���";
	soullist[6]="rand";
	soulexplanation[6]="������";
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

