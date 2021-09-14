#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/hash_policy.hpp>
#define iv inline void
using namespace std;
using namespace __gnu_pbds;
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
int founds(vector<string> &t,string a)
{
	for(int i=0;i<t.size();i++)
		if(t[i]==a)
			return i;
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

vector<card> cardheap,recardheap;
vector<card> handcard1,handcard2;
vector<skill> skills1,skills2,simpleskills1,simpleskills2;
vector<buff> buff1,buff2,buffclear;
string soul1,soul2;
int cost1,cost2,costmax1,costmax2;
int mana1,mana2,manamax1,manamax2;
int handcardmax1,handcardmax2;
int trunk1,trunk2;
int clocks=0,clocksum=0;
int tim=0,twelveam=0,timrate=0;
int turn;

string explanation[114];skill theskill[514];
string skillexplanation[114];
string soullist[114];int soullong,cardlong;
string soulexplanation[514];
string cardname[1919];
vector<skill> soulskill[114];
gp_hash_table<string,string> speffectexplanation;
string cardsyst[19],cardsystexplanation[19];
int cardsystlong;
bool thecardsyst[19];
string gamemodename[19],gamemodeexplanation[19];int gamemodelong;
iv init()
{
	srand(time(NULL));
	mouse(0);
	
	cardlong=34;
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
	cardname[20]="���˵Ķ���";
	cardname[21]="��ʿ�Ķ���";
	cardname[22]="��ʦ�Ķ���";
	cardname[23]="սʿ�Ķ���";
	cardname[24]="�ؾ��Ķ���";
	cardname[25]="��ħ�Ķ���";
	cardname[26]="��ʦ�Ķ���";
	cardname[27]="���˵Ķ���";
	cardname[28]="��Ե�Ķ���";
	cardname[29]="����˹ת��";
	cardname[30]="���II";
	cardname[31]="��ɱ";
	cardname[32]="������";
	cardname[33]="έ��ʮ��ն"; 
	cardname[34]="����ն";
	
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
	explanation[20]="������һ���ޡ����˵Ķ��͡����Ƶõ������˵Ķ��͡�";
	explanation[21]="�����������ޡ���ʿ�Ķ��͡����Ƶõ�����ʿ�Ķ��͡�";
	explanation[22]="�����������ޡ���ʦ�Ķ��͡����Ƶõ�����ʦ�Ķ��͡�";
	explanation[23]="������һ���ޡ�սʿ�Ķ��͡����Ƶõ���սʿ�Ķ��͡�";
	explanation[24]="�����������ޡ��ؾ��Ķ��͡����Ƶõ����ؾ��Ķ��͡�";
	explanation[25]="�����������ޡ���ħ�Ķ��͡����Ƶõ�����ħ�Ķ��͡�";
	explanation[26]="������һ���ޡ���ʦ�Ķ��͡����Ƶõ�����ʦ�Ķ��͡�";
	explanation[27]="��������޶���Ч�����Ƶõ������˵Ķ��͡�";
	explanation[28]="���ʹ��һ�Ŷ�����";
	explanation[29]="50%��Է����������һ�š�����˹ת�̡�����������Լ��ġ�";
	explanation[30]="���ƶѶ����ơ�+2";
	explanation[31]="�ڶԷ�ʧ��ʱֱ�ӻ�ɱ�Է���������Ч��";
	explanation[32]="����20��������";
	explanation[33]="��������ƺ󣩶Է����� ˫������������*6 ��������";
	explanation[34]="�Է����� (100-�Է�������)/2 ��������";
	
	theskill[1].set_up("������־",40,1,1);
	theskill[2].set_up("����ָ��",70,1,2);
	theskill[3].set_up("���屬��",0,0,3);
	theskill[4].set_up("��������",50,1,4);
	theskill[5].set_up("�����ֻ�",0,0,5);
	theskill[6].set_up("ǿ��ת��",40,1,6);
	theskill[7].set_up("����l������r",0,0,7);
	theskill[8].set_up("��Щ���Ҳ���",0,0,8);
	theskill[9].set_up("���鼯��ѯ",70,1,9);
	theskill[10].set_up("���ȶ�����",0,0,10);
	theskill[11].set_up("��������",0,0,11); 
	theskill[12].set_up("��֪����",20,1,12);
	theskill[13].set_up("ȼ��ŭ��",80,1,13);
	theskill[14].set_up("����",0,0,14);
	theskill[15].set_up("����",0,0,15);
	theskill[16].set_up("׹��",80,1,16);
	
	skillexplanation[1]="���ƶѶ����Ƶķ��û�Ϊ0����������";
	skillexplanation[2]="���Լ������һ���Ƹ���Է�";
	skillexplanation[3]="�ڻغ�ĩʱ�����ҽ���һ�ŷ�����������������";
	skillexplanation[4]="��˫�������ƻ���һ��������飨���������䣩";
	skillexplanation[5]="�غϿ�ʼʱ����鵽��Ϊż������"; 
	skillexplanation[6]="+1��";
	skillexplanation[7]="�غϳ�60%���һ���޴�Ч�������Ƶõ����ӱߣ��ӱߣ��ӱߣ���";
	skillexplanation[8]="�غϳ���û�д����ӱߣ��ӱߣ��ӱߣ���������+1��";
	skillexplanation[9]="���������д��С��ӱߣ��ӱߣ��ӱߣ�������";
	skillexplanation[10]="����ʱ����һ����";
	skillexplanation[11]="ÿ�غϳ��õ��ġ�Ϊ1~3";
	skillexplanation[12]="+1��,��1����";
	skillexplanation[13]="���2�غϡ���ŭ��buff����Է�����Ч����õ����Ʊ�����";
	skillexplanation[14]="��ʼʱ��������-2";
	skillexplanation[15]="�򳬹��������޶�����������50%���ʽ�������";
	skillexplanation[16]="��������-1";
	
	soullong=7;
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
	soulskill[3].push_back(theskill[13]);
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
	soullist[6]="�侲";
	soulskill[6].push_back(theskill[14]);
	soulskill[6].push_back(theskill[15]);
	soulskill[6].push_back(theskill[16]);
	soulexplanation[6]="�𽥼�����������";
	soullist[7]="rand";
	soulexplanation[7]="������";
	
	speffectexplanation["�ӱߣ��ӱߣ��ӱߣ�"]="ʹ�á����鼯��ѯ��ʱ���ƽ�������";
	speffectexplanation["���˵Ķ���"]="����ӵ�ж��͵���������>3ʱ�����ƽ�������";
	speffectexplanation["��ʿ�Ķ���"]="���������ʱ+15��";
	speffectexplanation["��ʦ�Ķ���"]="�غϿ�ʼʱ�д��С���ʦ�Ķ��͡�����������*10%�ĸ���+1��";
	speffectexplanation["սʿ�Ķ���"]="�ܵ����ͷ���ʱ����Ч������������Ƶġ�սʿ�Ķ��͡���ʧ";
	speffectexplanation["�ؾ��Ķ���"]="ʹ���ؾ��Ķ��͡�buff��2�غϣ����ؾ��Ķ��͡�Ϊ3�غ�ʱת��Ϊ1��";
	speffectexplanation["��ħ�Ķ���"]="�غϿ�ʼʱ�д��С���ħ�Ķ��͡�����������*10%�ĸ�����һ����";
	speffectexplanation["��ʦ�Ķ���"]="������������ƶ����С���ʦ�Ķ��͡�ʱֱ�ӻ�ʤ";
	speffectexplanation["���˵Ķ���"]="���������ʱ+5��";
	
	cardsystlong=4;
	cardsyst[1]="��������";
	cardsyst[2]="ʱ������";
	cardsyst[3]="��������";
	cardsyst[4]="ֻ������";
	
	cardsystexplanation[1]="��Ϊ���������ͣ����ɹر�";
	cardsystexplanation[2]="��Ҫ��ʱ��ģʽ���ܴ�";
	cardsystexplanation[3]="���� CARD-GAME ��ְҵ����Ķ���";
	cardsystexplanation[4]="��Ҫ���� ����ģʽ��ֻ�� �ſɴ�";
	
	thecardsyst[1]=thecardsyst[2]=thecardsyst[3]=1;
	
	gamemodelong=3;
	gamemodename[1]="����ģʽ";
	gamemodename[2]="����ģʽ����ʱ";
	gamemodename[3]="����ģʽ��ֻ��";
	
	gamemodeexplanation[1]="������Ϸģʽ";
	gamemodeexplanation[2]="ÿ�غϿ��ֶ���1���ƣ���20�غϺ����������ٵ��˻�ʤ";
	gamemodeexplanation[3]="ÿ�õ�һ�����Ʊ����������������������������Է���ɱ";
}

void wolf()
{
	printf("����ϸ�Ķ���Ϸ����:\n\n");
	printf("1.ֻ��ģʽ��˫����������Ϊ100��������������������ʱ��ʧȥƽ��\n\n");
	printf("2.�ڶԷ�ʧ��ʱ�����á���ɱ��ֱ�ӻ�ɱ�Է����ʤ��\n\n");
	printf("3.ÿ�õ�һ������������ ���Ʒ���*8 ��������\n\n");
	printf("4.ÿ�غϳ�������ǰ������ 20*(��������-��������)/����������������\n\n");
	printf("5.ֻ�����������һЩ�ƿ��Զ����������Ӱ��\n\n");
	printf("6.������ƴ��겻��ֱ��ʤ������������һ���Ʋ��ҶԷ����� 20 ������\n\n");
	printf("7.���¿ո����Enter����");
}

//From PreInfo.h
void SetColor(short x)
{
  if(x>=0 && x<=15)
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), x);
  else
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
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
