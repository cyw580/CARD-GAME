#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/hash_policy.hpp>
#include<windows.h>
#include<conio.h>
#include<assert.h>
#include<tchar.h>
#include"PreInfo(ACG).h"
#define cls system("cls")
#define iv inline void
#define Get GetStdHandle(STD_OUTPUT_HANDLE)
#define set1 SetConsoleTextAttribute
#define set2 Get,FOREGROUND_INTENSITY
#define set3 Get,BACKGROUND_INTENSITY
#define sred FOREGROUND_RED
#define sgreen FOREGROUND_GREEN
#define sblue FOREGROUND_BLUE
#define set set1(set2 | sred | sgreen | sblue)
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0) 
#define REP(x) for(int i=1;i<=x;i++)
using namespace std;
using namespace __gnu_pbds;
iv moveto(int a,int b) //�� �� 
{
	HANDLE hConsole=Get;
	COORD coordScreen={short(a),short(b)};
	SetConsoleCursorPosition(hConsole,coordScreen);
}
iv mouse(int a)
{
	HANDLE handle=Get;
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
iv print(string a){for(int i=0;i<a.size();i++){putchar(a[i]);}}
iv red(string u){set1(set2 | sred);print(u);set;}
iv blue(string u){set1(set2 | sblue);print(u);set;}
iv green(string u){set1(set2 | sgreen);print(u);set;}
iv yellow(string u){set1(set2 | sred | sgreen);print(u);set;}
iv cyan(string u){set1(set2 | sgreen | sblue);print(u);set;}
iv purple(string u){set1(set2 | sred | sblue);print(u);set;}
iv white(string u){set1(set2 | sred | sblue | sgreen);print(u);set;}
iv red2(string u){set1(set3 | sred);print(u);set;}
iv blue2(string u){set1(set3 | sblue);print(u);set;}
iv green2(string u){set1(set3 | sgreen);print(u);set;}
iv yellow2(string u){set1(set3 | sred | sgreen);print(u);set;}
iv cyan2(string u){set1(set3 | sgreen | sblue);print(u);set;}
iv purple2(string u){set1(set3 | sred | sblue);print(u);set;}
iv white2(string u){set1(set3 | sred | sblue | sgreen);print(u);set;}
iv putspace(int t){for(int i=1;i<=t;i++) putchar(' ');}
template <typename T>
inline string change(T a){stringstream ss;string s;ss.clear();ss.str("");ss<<a;ss>>s;return s;} 
template <typename T>
inline int change2(T a){stringstream ss;int s;ss.clear();ss.str("");ss<<a;ss>>s;return s;}//���϶���ƽ��д��Ϸ��ģ�壨���� 
iv printhandcard(int choose);
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
vector<card> cardheap,recardheap;
vector<card> handcard1,handcard2;
vector<skill> skills1,skills2,simpleskills1,simpleskills2;
vector<skill> soulskill[114];
vector<buff> buff1,buff2;
string explanation[114];skill theskill[514];
string skillexplanation[114];
string soullist[114];int soullong,cardlong;
string soulexplanation[514];
string cardname[1919];
bool visskill[114];
string soul1,soul2;
int cost1,cost2,costmax1,costmax2;
int mana1,mana2,manamax1,manamax2;
int handcardmax1,handcardmax2;
int turn;
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
iv init()
{
	srand(time(NULL));
	mouse(0);
	
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
	
	//���� 
	cardlong=12; 
	explanation[1]="����ִ��ƿ��г�һ����";
	explanation[2]="��������Լ���������һ����";
	explanation[3]="���ƶѶ�����һ�š��谭��";
	explanation[4]="+20��";
	explanation[5]="˫�����������һ������";
	explanation[6]="+50��";
	explanation[7]="�Է�-20��";
	explanation[8]="�����ƶѶ����Ƽ���Է�����";
	explanation[9]="�����������еġ��ͷ���һͬ�����Ĵ��";
	explanation[10]="�����������С�>2���ơ�-1";
	explanation[11]="������غϵġ��־á�Ч����ÿ�غϳ�����+1����"; 
	explanation[12]="�߰������������������һ����Ч�����Ƶ�Ч����";
	explanation[13]="�������������еġ�һ��ͬ�塿����";
	explanation[14]="��������һ�Ų��ǡ�һ��ͬ�塿���Ʊ�ɡ�һ��ͬ�塿";
	
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
iv addcard(int kind,card t) //���ƶ������ 
{
	if(cardheap.size()==0)
	{
		cardheap.push_back(t);
		return;
	}
	if(kind==1) //�������λ�� 
	{
		int s=ran(0,cardheap.size());
		if(s==cardheap.size()) cardheap.push_back(t);
		else cardheap.insert(cardheap.begin()+s,t);
	}
	if(kind==2) //�����ƶѶ� 
		cardheap.insert(cardheap.begin(),t);
}
iv recard_heap() //ϴ�� 
{
	random_shuffle(recardheap.begin(),recardheap.end());
	swap(recardheap,cardheap);
	recardheap.clear();
}
iv dealcard() //���ַ���
{
	cardheap.clear();
	REP(15) addcard(1,card("�ͷ�",1,1));
	REP(5) addcard(1,card("��������",2,2));
	REP(3) addcard(1,card("���",1,3));
	REP(5) addcard(1,card("΢С����",1,4));
	REP(3) addcard(1,card("������Ӯ",1,5)); 
	REP(4) addcard(1,card("�еȷ���",2,6));
	REP(4) addcard(1,card("��־�ݻ�",2,7));
	REP(4) addcard(1,card("����ֻ�",1,8));
	REP(2) addcard(1,card("�ռ��ͽ�",2,9));
	REP(3) addcard(1,card("ħ�ð�Ϸ",1,10));
	REP(4) addcard(1,card("�ɳ־û�",1,11));
	REP(2) addcard(1,card("1919810",1,12));
	REP(8) addcard(1,card("һ��ͬ��",1,13));
	REP(3) addcard(1,card("����ͬ��",1,14));
	addcard(1,card("�谭",3,0));
}
card drawcard() //���� 
{
	if(cardheap.size()==0) recard_heap();
	card t=cardheap[0];
	cardheap.erase(cardheap.begin());
	return t;
}
iv startgame() //��ʼ��Ϸ 
{
	dealcard();
	REP(4) handcard1.push_back(drawcard());
	REP(4) handcard2.push_back(drawcard());
	if(soul1=="����") handcard1.push_back(drawcard()),handcard1.push_back(drawcard());
	if(soul2=="����") handcard2.push_back(drawcard()),handcard2.push_back(drawcard());
	cost1=cost2=0;
	costmax1=costmax2=5;
	mana1=mana2=manamax1=manamax2=100;
	handcardmax1=handcardmax2=10;
	turn=1;
}
vector<card> *cardmine,*cardhis;
int *costmine,*costmaxmine,*costhis,*costmaxhis,*manamine,*manahis,*manamaxmine,*manamaxhis;
int *handcardmaxmine,*handcardmaxhis;
vector<skill> *skillsmine,*simpleskillsmine;
string *soulmine,*soulhis;
vector<buff> *buffmine,*buffhis;
int Choose=1,sure_to_use=0,sure_to_magic=0,sure_to_end=0;
int handkind=1;
iv printground()
{
	moveto(6,2);print("#1");
	moveto(14,2);cyan(change(cost1)+"/"+change(costmax1));
	moveto(24,2);REP(costmax1) cyan("��");
	moveto(24,2);REP(cost1) cyan("��");
	moveto(6,3);print(soul1);
	moveto(14,3);print("       ");
	moveto(14,3);blue(change(mana1)+"/"+change(manamax1));
	moveto(24,3);REP(manamax1/20) blue("��");
	moveto(24,3);REP(mana1/20) blue("��");
	moveto(14,4);print("     ");
	moveto(14,4);print(change(handcard1.size())+"/"+change(handcardmax1));
	moveto(24,4);
	for(int i=0;i<buff1.size();i++)
	{
		if(i) print(" | ");
		if(buff1[i].name=="�־�") cyan("�־� "+change(buff1[i].tim));
	}

	moveto(6,6);print("#2");
	moveto(14,6);cyan(change(cost2)+"/"+change(costmax2));
	moveto(24,6);REP(costmax2) cyan("��");
	moveto(24,6);REP(cost2) cyan("��");
	moveto(6,7);print(soul2);
	moveto(14,7);print("       ");
	moveto(14,7);blue(change(mana2)+"/"+change(manamax2));
	moveto(24,7);REP(manamax2/20) blue("��");
	moveto(24,7);REP(mana2/20) blue("��");
	moveto(14,8);print("     ");
	moveto(14,8);print(change(handcard2.size())+"/"+change(handcardmax2));
	moveto(24,8);
	for(int i=0;i<buff2.size();i++)
	{
		if(i) print(" | ");
		if(buff2[i].name=="�־�") cyan("�־� "+change(buff2[i].tim));
	}
	
	moveto(0,11);print("P"+change(turn));
	moveto(8,11);print("#");
	moveto(12,11);
	if(handkind==1) cyan("��");
	if(handkind==2) blue("��");
	
	printhandcard(Choose);
}
iv printhandcard(int choose)
{
	if(handkind==1)
	{
		for(int i=0;i<(cardmine->size());i++)
		{
			moveto(6,12+i);printf("  ");
			moveto(8,12+i);printf("  ");moveto(8,12+i);print(change(i+1));
			moveto(12,12+i);if((*cardmine)[i].cost<=*costmine)cyan(change((*cardmine)[i].cost));else blue(change((*cardmine)[i].cost));
			moveto(15,12+i);print(string(50,' '));
			moveto(15,12+i);print("["+(*cardmine)[i].name+"]");
			moveto(30,12+i);
			for(int j=0;j<(*cardmine)[i].speffect.size();j++)
				print("["+(*cardmine)[i].speffect[j]+"]");
		}
		moveto(6,11+choose);
		if(sure_to_use) print("��");
		else print("��");
		moveto(8,11+choose);green(change(choose));
		moveto(12,11+choose);green(change((*cardmine)[choose-1].cost));
		moveto(15,11+choose);green("["+(*cardmine)[choose-1].name+"]");
		moveto(30,11+choose);
		for(int j=0;j<(*cardmine)[choose-1].speffect.size();j++)
			green("["+(*cardmine)[choose-1].speffect[j]+"]");
		
		moveto(10,24);print(string(50,' '));
		moveto(10,24);
		if((*cardmine)[choose-1].effect) print(explanation[(*cardmine)[choose-1].effect]);
	}
	if(handkind==2)
	{
		for(int i=0;i<(simpleskillsmine->size());i++)
		{
			moveto(6,12+i);printf("  ");
			moveto(8,12+i);printf("  ");moveto(8,12+i);
			if(visskill[i]) red(change(i+1));else print(change(i+1));
			moveto(12,12+i);
			if(visskill[i]) red(change((*simpleskillsmine)[i].mana));
			else blue(change((*simpleskillsmine)[i].mana));
			moveto(15,12+i);print(string(15,' '));
			moveto(15,12+i);
			if(visskill[i]) red("["+(*simpleskillsmine)[i].name+"]");
			else print("["+(*simpleskillsmine)[i].name+"]");
		}
		moveto(6,11+choose);
		if(sure_to_use) print("��");
		else print("��");
		moveto(8,11+choose);green(change(choose));
		moveto(12,11+choose);green(change((*simpleskillsmine)[choose-1].mana));
		moveto(15,11+choose);green("["+(*simpleskillsmine)[choose-1].name+"]");
		
		moveto(10,24);print(string(50,' '));
		moveto(10,24);
		if((*simpleskillsmine)[choose-1].effect) print(skillexplanation[(*simpleskillsmine)[choose-1].effect]);
	}
}
iv choosechanged(int lastchoose,int choose)
{
	if(handkind==1)
	{
		moveto(6,11+lastchoose);print("  "+change(lastchoose));
		moveto(12,11+lastchoose);if((*cardmine)[lastchoose-1].cost<=*costmine) cyan(change((*cardmine)[lastchoose-1].cost));else blue(change((*cardmine)[lastchoose-1].cost));
		moveto(15,11+lastchoose);print("["+(*cardmine)[lastchoose-1].name+"]");
		moveto(30,11+lastchoose);
		for(int j=0;j<(*cardmine)[lastchoose-1].speffect.size();j++)
			print("["+(*cardmine)[lastchoose-1].speffect[j]+"]");
			
		moveto(6,11+choose);
		if(sure_to_use) print("��");
		else print("��");
		moveto(8,11+choose);green(change(choose));
		moveto(12,11+choose);green(change((*cardmine)[choose-1].cost));
		moveto(15,11+choose);green("["+(*cardmine)[choose-1].name+"]");
		moveto(30,11+choose);
		for(int j=0;j<(*cardmine)[choose-1].speffect.size();j++)
			green("["+(*cardmine)[choose-1].speffect[j]+"]");
			
		moveto(10,24);print(string(50,' '));
		moveto(10,24);
		if((*cardmine)[choose-1].effect) print(explanation[(*cardmine)[choose-1].effect]);
	}
	if(handkind==2)
	{
		moveto(6,11+lastchoose);if(visskill[lastchoose-1]) red("  "+change(lastchoose));else print("  "+change(lastchoose));
		moveto(12,11+lastchoose);if(visskill[lastchoose-1]) red(change((*simpleskillsmine)[lastchoose-1].mana));else blue(change((*simpleskillsmine)[lastchoose-1].mana));
		moveto(15,11+lastchoose);if(visskill[lastchoose-1]) red("["+(*simpleskillsmine)[lastchoose-1].name+"]");else print("["+(*simpleskillsmine)[lastchoose-1].name+"]");
		
		moveto(6,11+choose);
		if(sure_to_use) print("��");
		else print("��");
		moveto(8,11+choose);green(change(choose));
		moveto(12,11+choose);green(change((*simpleskillsmine)[choose-1].mana));
		moveto(15,11+choose);green("["+(*simpleskillsmine)[choose-1].name+"]");
		
		moveto(10,24);print(string(50,' '));
		moveto(10,24);
		if((*simpleskillsmine)[choose-1].effect) print(skillexplanation[(*simpleskillsmine)[choose-1].effect]);
	}
}
iv win()
{
	if(handcard1.size()==0 and handcard2.size()==0)
	{
		system("cls");
		print("ƽ��");
		exit(0);
	}
	if(handcard1.size()==0)
	{
		system("cls");
		print("P1ʤ��\n");
		exit(0);
	}
	if(handcard2.size()==0)
	{
		system("cls");
		print("P2ʤ��\n");
		exit(0);
	}
}
iv throwcard(vector<card> *t,int pos)
{
	if((t->size()==0))
		return;
	moveto(5,11+(t->size()));print(string(50,' '));
	recardheap.push_back((*t)[pos-1]);
	t->erase(t->begin()+pos-1);
	if((t->size())==0)
		win();
}
iv increasecost(int *cos,int *cosmax,int delta)
{
	if(*cos+delta>*cosmax) *cos=*cosmax;
	else if(*cos+delta<0) *cos=0;
	else *cos+=delta;
}
iv increasemana(int *man,int *manmax,int delta)
{
	if(*man+delta>*manmax) *man=*manmax;
	else if(*man+delta<0) *man=0;
	else *man+=delta;
}
iv addhandcard(vector<card> *t,card t2,int *handcardmax)
{
	t->push_back(t2);
	if(t->size()>*handcardmax)
		throwcard(t,t->size());
}
iv usecard(card t,int pos)
{
	if(t.name!="114514")
		throwcard(cardmine,pos);
	if(t.effect==1) 
		addhandcard(cardhis,drawcard(),handcardmaxhis);
	if(t.effect==2)
	{
		int pp=ran(1,(cardmine->size()));
		throwcard(cardmine,pp);
	}
	if(t.effect==3)
		addcard(2,card("�谭",3,0));
	if(t.effect==4)
		increasemana(manamine,manamaxmine,20);
	if(t.effect==5)
	{
		int pp=ran(1,(cardmine->size()));
		throwcard(cardmine,pp);
		pp=ran(1,(cardhis->size()));
		throwcard(cardhis,pp);
	}
	if(t.effect==6)
		increasemana(manamine,manamaxmine,50);
	if(t.effect==7)
		increasemana(manahis,manamaxhis,-20);
	if(t.effect==8)
		if(recardheap.size()>1)
		{
			card s=recardheap[recardheap.size()-2];
			recardheap.erase(recardheap.begin()+(recardheap.size()-2));
			addhandcard(cardhis,s,handcardmaxhis);
		}
	if(t.effect==9)
	{
		bool flagg=1;
		while(flagg)
		{
			flagg=0;
			for(int i=0;i<(cardmine->size());i++)
				if((*cardmine)[i].name=="�ͷ�")
				{
					flagg=1;
					usecard((*cardmine)[i],i+1);
					break;
				}
		}
	}
	if(t.effect==10)
		for(int i=0;i<(cardmine->size());i++)
			if((*cardmine)[i].cost>2)
				(*cardmine)[i].cost--;
	if(t.effect==11)
	{
		int del=0;
		if(foundbuff(buffmine,"�־�")==-1)
		{
			buff s;s.set_up("�־�",0);
			buffmine->push_back(s);
			del=1;
		}
		(*buffmine)[foundbuff(buffmine,"�־�")].tim+=2+del;
	}
	if(t.effect==12)
	{
		int s=ran(1,cardlong);
		while(s==12) s=ran(1,cardlong);
		usecard(card("114514",0,s),pos);
		moveto(0,1);print("��ʹ���ˡ�"+cardname[s]+"��");
	}
	if(t.effect==13)
	{
		bool flagg=1;
		while(flagg)
		{
			flagg=0;
			for(int i=0;i<(cardmine->size());i++)
				if((*cardmine)[i].name=="һ��ͬ��")
				{
					flagg=1;
					throwcard(cardmine,i+1);
					break;
				}
		}
	}	
	if(t.effect==14)
	{
		vector<int> cywakioi;cywakioi.clear();
		for(int i=0;i<(cardmine->size());i++)
			if((*cardmine)[i].name!="һ��ͬ��")
				cywakioi.push_back(i);
		if(cywakioi.size()) (*cardmine)[cywakioi[ran(0,cywakioi.size()-1)]]=card("һ��ͬ��",1,13);
	}
}
iv usemagic(skill t)
{
	if(t.effect==1)
	{
		card a=drawcard();a.cost=0;
		addhandcard(cardmine,a,handcardmaxmine);
	}
	if(t.effect==2)
	{
		int pp=ran(1,cardmine->size());
		card a=(*cardmine)[pp-1];
		throwcard(cardmine,pp);
		addhandcard(cardhis,a,handcardmaxhis);
	}
	if(t.effect==4)
	{
		vector<card> qwq;qwq.clear();
		int size1=handcard1.size(),size2=handcard2.size();
		for(int i=0;i<size1;i++)
			qwq.push_back(handcard1[i]);
		for(int i=0;i<size2;i++)
			qwq.push_back(handcard2[i]);
		handcard1.clear();handcard2.clear();
		random_shuffle(qwq.begin(),qwq.end());
		for(int i=0;i<size1;i++)
			handcard1.push_back(qwq[i]);
		for(int i=size1;i<qwq.size();i++)
			handcard2.push_back(qwq[i]);
	}
	if(t.effect==6)
		increasecost(costmine,costmaxmine,1);
	if(t.effect==9)
	{
		bool flagg=1;
		while(flagg)
		{
			flagg=0;
			for(int i=0;i<(cardmine->size());i++)
				if(found((*cardmine)[i].speffect,"�ӱߣ��ӱߣ��ӱߣ�"))
				{
					flagg=1;
					throwcard(cardmine,i+1);
					break;
				}
		}
	}
	if(t.effect==12)
	{
		increasecost(costmine,costmaxmine,1);
		addhandcard(cardmine,drawcard(),handcardmaxmine);
	}
}
iv ending()
{
	for(int i=0;i<(skillsmine->size());i++)
		if(!((*skillsmine)[i].active))
		{
			if((*skillsmine)[i].effect==3)
			{
				int costm=-1,pos=0,ss=0;
				for(int j=0;j<(cardmine->size());j++)
				{
					if((*cardmine)[j].cost>costm)
					{
						costm=(*cardmine)[j].cost;
						pos=j+1;
						ss=1;
					}
					else if((*cardmine)[j].cost==costm)
						ss++;
				}
				if(ss==1) throwcard(cardmine,pos);
			}
		}
}
iv game()
{
	if(turn==1)
	{
		cardmine=&handcard1;costmine=&cost1;costmaxmine=&costmax1;
		cardhis=&handcard2;costhis=&cost2;costmaxhis=&costmax2;
		manamine=&mana1;manamaxmine=&manamax1;
		manahis=&mana2;manamaxhis=&manamax2;
		skillsmine=&skills1,simpleskillsmine=&simpleskills1;
		handcardmaxmine=&handcardmax1,handcardmaxhis=&handcardmax2; 
		soulmine=&soul1,soulhis=&soul2;
		buffmine=&buff1,buffhis=&buff2;
	}
	else
	{
		cardmine=&handcard2;costmine=&cost2;costmaxmine=&costmax2;
		cardhis=&handcard1;costhis=&cost1;costmaxhis=&costmax1;	
		manamine=&mana2;manamaxmine=&manamax2;
		manahis=&mana1;manamaxhis=&manamax1;
		skillsmine=&skills2,simpleskillsmine=&simpleskills2;
		handcardmaxmine=&handcardmax2,handcardmaxhis=&handcardmax1; 
		soulmine=&soul2,soulhis=&soul1;
		buffmine=&buff2,buffhis=&buff1;
	}
	for(int i=0;i<(buffmine->size());i++)
		(*buffmine)[i].tim--;
	for(int i=0;i<(buffmine->size());i++)
		if((*buffmine)[i].tim==0)
		{
			buffmine->erase(buffmine->begin()+i);
			i--;
		}
	int ddd=0;
	if(*soulmine=="��ã") ddd=ran(1,3)-2;
	increasecost(costmine,costmaxmine,2+ddd);
	if(foundbuff(buffmine,"�־�")!=-1)
		 increasecost(costmine,costmaxmine,1);
	increasemana(manamine,manamaxmine,10);
	addhandcard(cardmine,drawcard(),handcardmaxmine);
	if(*soulmine=="����")
		while((*cardmine)[(cardmine->size())-1].cost%2==0)
		{
			throwcard(cardmine,cardmine->size());
			addhandcard(cardmine,drawcard(),handcardmaxmine);
		}
	if(*soulmine=="����")
	{
		if(ran(1,10)<=6)
		{
			vector<int> ee;ee.clear();
			for(int i=0;i<(cardmine->size());i++)
				if(!found((*cardmine)[i].speffect,"�ӱߣ��ӱߣ��ӱߣ�"))
					ee.push_back(i);
			if(ee.size())
				(*cardmine)[ee[ran(0,ee.size()-1)]].speffect.push_back("�ӱߣ��ӱߣ��ӱߣ�");	
		}
		bool eee=1;
		for(int i=0;i<(cardmine->size());i++)
			if(found((*cardmine)[i].speffect,"�ӱߣ��ӱߣ��ӱߣ�"))
			{
				eee=0;
				break;
			}
		if(eee) increasecost(costmine,costmaxmine,1);
	}
	sure_to_use=0;sure_to_magic=0;sure_to_end=0;
	handkind=1;
	memset(visskill,0,sizeof(visskill));
	system("cls");
	char c='T';
	Choose=1;
	printground();
	int lastchoose=1;
	while(1)
	{
		choosechanged(lastchoose,Choose);
		lastchoose=Choose;
		c=getch();
		moveto(0,1);print(string(24,' '));
		if(handkind==1)
		{
			if(sure_to_end==1 or sure_to_magic==1) moveto(10,27),print(string(18,' '));
			if(upc(c) or leftc(c)) Choose--,sure_to_use=0,sure_to_magic=0,sure_to_end=0;
			if(downc(c) or rightc(c)) Choose++,sure_to_use=0,sure_to_magic=0,sure_to_end=0;
			if(Choose==0) Choose=cardmine->size();
			if(Choose==(cardmine->size())+1) Choose=1;
			if(usec(c)) sure_to_use++,sure_to_magic=0,sure_to_end=0;
			if(surec(c)) sure_to_end++,sure_to_magic=0,sure_to_use=0;
			if(magicc(c)) sure_to_magic++,sure_to_use=0,sure_to_end=0;
			if(sure_to_use==2)
			{
				moveto(0,1);
				if((*cardmine)[Choose-1].cost>*costmine) print("���ò����޷�ʹ�ô��ƣ�");
				else
				{
					print(string(24,' '));
					*costmine-=(*cardmine)[Choose-1].cost;
					usecard((*cardmine)[Choose-1],Choose);
					if(Choose>=(cardmine->size())) Choose=cardmine->size();
					lastchoose=Choose;
					printground();
				}
				sure_to_use=0;
			}
			if(sure_to_magic==1)
			{
				moveto(10,27);
				print("ȷ��Ҫ�л�ģʽ��");
			}
			if(sure_to_magic==2)
			{
				handkind=2;
				lastchoose=Choose=1;
				sure_to_magic=0;
				system("cls");
				printground(); 
			}
			if(sure_to_end==1)
			{
				moveto(10,27);
				print("ȷ��Ҫ�����غ���");
			}
			if(sure_to_end==2)
				break;	
		}
		else if(handkind==2)
		{
			if(sure_to_end==1 or sure_to_magic==1) moveto(10,27),print(string(18,' '));
			if(upc(c) or leftc(c)) Choose--,sure_to_use=0,sure_to_magic=0,sure_to_end=0;
			if(downc(c) or rightc(c)) Choose++,sure_to_use=0,sure_to_magic=0,sure_to_end=0;
			if(Choose==0) Choose=simpleskillsmine->size();
			if(Choose==(simpleskillsmine->size())+1) Choose=1;
			if(usec(c)) sure_to_use++,sure_to_magic=0,sure_to_end=0;
			if(surec(c)) sure_to_end++,sure_to_magic=0,sure_to_use=0;
			if(magicc(c)) sure_to_magic++,sure_to_use=0,sure_to_end=0;
			if(sure_to_use==2)
			{
				moveto(0,1);
				if((*simpleskillsmine)[Choose-1].mana>*manamine) print("ħ�������޷�ʹ�øü��ܣ�");
				else if(visskill[Choose-1]) print("�˻غ��Ѿ�ʹ�ù��ü��ܣ�");
				else
				{
					print(string(24,' '));
					*manamine-=(*simpleskillsmine)[Choose-1].mana;
					usemagic((*simpleskillsmine)[Choose-1]);	
					visskill[Choose-1]=1;	
					printground();
				}
				sure_to_use=0;
			}
			if(sure_to_magic==1)
			{
				moveto(10,27);
				print("ȷ��Ҫ�л�ģʽ��");
			}
			if(sure_to_magic==2)
			{
				handkind=1;
				lastchoose=Choose=1;
				sure_to_magic=0;
				system("cls");
				printground(); 
			}
			if(sure_to_end==1)
			{
				moveto(10,27);
				print("ȷ��Ҫ�����غ���");
			}
			if(sure_to_end==2)
				break;	
		}
	}
	ending();
	turn=3-turn;
	game();
}
iv choosesoul()
{
	for(int i=1;i<=2;i++)
	{ 
		vector<skill> *skills,*simpleskills;
		string *thesoul;
		if(i==1) skills=&skills1,simpleskills=&simpleskills1,thesoul=&soul1;
		if(i==2) skills=&skills2,simpleskills=&simpleskills2,thesoul=&soul2;
		
		system("cls");
		print("        ѡ��P"+change(i)+"��������ʣ�");
		char c='T';
		int choose=1,sure_to_choose=0;
		while(1)
		{
			for(int j=1;j<=soullong;j++)
				moveto(4,j),print("  "+change(j)+" "+soullist[j]+" | "+soulexplanation[j]);
			moveto(4,choose);
			if(sure_to_choose==0) print("��");
			if(sure_to_choose==1) print("��"); 
			green(change(choose)+" "+soullist[choose]+" | "+soulexplanation[choose]);
			
			moveto(0,10);REP(5) print(string(75,' ')+"\n");moveto(0,10); 
			for(int j=0;j<soulskill[choose].size();j++)
			{
				string activeword="";
				if(soulskill[choose][j].active) activeword="("+change(soulskill[choose][j].mana)+"��)";
				print(change(j+1)+".["+soulskill[choose][j].name+"]"+skillexplanation[soulskill[choose][j].effect]+activeword+"\n");
			}
			
			c=getch();
			if(upc(c) or leftc(c)) choose--,sure_to_choose=0;
			if(downc(c) or rightc(c)) choose++,sure_to_choose=0;
			if(usec(c) or surec(c)) sure_to_choose++;
			if(sure_to_choose==2)
				break;
			if(choose==soullong+1) choose=1;
			if(choose==0) choose=soullong;
		}
		if(choose==soullong) choose=ran(1,soullong-1);
		*thesoul=soullist[choose];
		for(int j=0;j<soulskill[choose].size();j++)
			skills->push_back(soulskill[choose][j]);
		for(int j=0;j<(skills->size());j++)
			if((*skills)[j].active)
				simpleskills->push_back((*skills)[j]);
	}
}
void init_introduce()
{
	printf("������Ϣ��\nLuogu:ʱ��\nGithub:wzheian21219014\n�����������\n");
	getch();
}
int main()
{
	init_introduce();
	init();
	choosesoul();
	startgame();
	game();
}
