#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/hash_policy.hpp>
#include<windows.h>
#include<conio.h>
#include<assert.h>
#include<tchar.h>
#include"PreInfo(ACG).h"
#define iv inline void
#define REP(x) for(int i=1;i<=x;i++)
using namespace std;
using namespace __gnu_pbds;
iv printhandcard(int choose);
vector<card> cardheap,recardheap;
vector<card> handcard1,handcard2;
vector<skill> skills1,skills2,simpleskills1,simpleskills2;
vector<buff> buff1,buff2;
bool visskill[114];
string soul1,soul2;
int cost1,cost2,costmax1,costmax2;
int mana1,mana2,manamax1,manamax2;
int handcardmax1,handcardmax2;
int turn;
int firsthand=1;
int timemode=0,tim=0,twelveam=0;
iv addcard(int kind,card t) //往牌堆里加牌 
{
	if(cardheap.size()==0)
	{
		cardheap.push_back(t);
		return;
	}
	if(kind==1) //插入随机位置 
	{
		int s=ran(0,cardheap.size());
		if(s==cardheap.size()) cardheap.push_back(t);
		else cardheap.insert(cardheap.begin()+s,t);
	}
	if(kind==2) //插入牌堆顶 
		cardheap.insert(cardheap.begin(),t);
}
iv dealcard() //发牌
{
	cardheap.clear();
	REP(15) addcard(1,card("惩罚",1,1));
	REP(5) addcard(1,card("附带弃置",2,2));
	REP(3) addcard(1,card("埋伏",1,3));
	REP(5) addcard(1,card("微小法力",1,4));
	REP(3) addcard(1,card("合作共赢",1,5)); 
	REP(4) addcard(1,card("中等法力",2,6));
	REP(4) addcard(1,card("意志摧毁",2,7));
	REP(4) addcard(1,card("天道轮回",1,8));
	REP(2) addcard(1,card("终极惩戒",2,9));
	REP(3) addcard(1,card("魔幻把戏",1,10));
	REP(4) addcard(1,card("可持久化",1,11));
	REP(2) addcard(1,card("1919810",1,12));
	REP(8) addcard(1,card("一心同体",2,13));
	REP(3) addcard(1,card("万心同体",1,14));
	REP(3) addcard(1,card("公平竞争",2,18));
	addcard(1,card("阻碍",3,0));
	if(timemode)
	{
		REP(4) addcard(1,card("赞美太阳",2,15));
		REP(8) addcard(1,card("时间流逝",2,16));
		REP(3) addcard(1,card("绯红之王",1,17));
	}
}
card drawcard() //抽牌 
{
	if(cardheap.size()==0) dealcard();
	card t=cardheap[0];
	cardheap.erase(cardheap.begin());
	return t;
}
iv startgame() //开始游戏 
{
	dealcard();
	REP(4) handcard1.push_back(drawcard());
	REP(4) handcard2.push_back(drawcard());
	if(soul1=="混沌") handcard1.push_back(drawcard()),handcard1.push_back(drawcard());
	if(soul2=="混沌") handcard2.push_back(drawcard()),handcard2.push_back(drawcard());
	cost1=cost2=0;
	costmax1=costmax2=5;
	mana1=mana2=manamax1=manamax2=100;
	handcardmax1=handcardmax2=10;
	if(firsthand!=3) turn=firsthand;
	else turn=ran(1,2);
	if(firsthand==1) cost2++;
	else cost1++;
	if(timemode) tim=ran(1,4);
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
	if(timemode)
	{
		moveto(10,0);print(string(50,' '));
		moveto(10+tim%2,0);
		if(tim==1) cyan("6AM | 回合开始时额外获得1◆");
		if(tim==2) red("12AM | 打出牌时有"+change(twelveam*10)+"%概率获得1◆");
		if(tim==3) yellow("6PM | 回合开始时多摸1张牌");
		if(tim==4) grey("12PM | 无特殊效果");
	}
	moveto(6,2);print("#1");
	moveto(14,2);cyan(change(cost1)+"/"+change(costmax1));
	moveto(24,2);REP(costmax1) cyan("◇");
	moveto(24,2);REP(cost1) cyan("◆");
	moveto(6,3);print(soul1);
	moveto(14,3);print("       ");
	moveto(14,3);blue(change(mana1)+"/"+change(manamax1));
	moveto(24,3);REP(manamax1/20) blue("□");
	moveto(24,3);REP(mana1/20) blue("■");
	moveto(14,4);print("     ");
	moveto(14,4);print(change(handcard1.size())+"/"+change(handcardmax1));
	moveto(24,4);
	for(int i=0;i<buff1.size();i++)
	{
		if(i) print(" | ");
		if(buff1[i].name=="持久") cyan("持久 "+change(buff1[i].tim));
	}

	moveto(6,6);print("#2");
	moveto(14,6);cyan(change(cost2)+"/"+change(costmax2));
	moveto(24,6);REP(costmax2) cyan("◇");
	moveto(24,6);REP(cost2) cyan("◆");
	moveto(6,7);print(soul2);
	moveto(14,7);print("       ");
	moveto(14,7);blue(change(mana2)+"/"+change(manamax2));
	moveto(24,7);REP(manamax2/20) blue("□");
	moveto(24,7);REP(mana2/20) blue("■");
	moveto(14,8);print("     ");
	moveto(14,8);print(change(handcard2.size())+"/"+change(handcardmax2));
	moveto(24,8);
	for(int i=0;i<buff2.size();i++)
	{
		if(i) print(" | ");
		if(buff2[i].name=="持久") cyan("持久 "+change(buff2[i].tim));
	}
	
	moveto(0,11);print("P"+change(turn));
	moveto(8,11);print("#");
	moveto(12,11);
	if(handkind==1) cyan("◆");
	if(handkind==2) blue("■");
	
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
		if(sure_to_use) print("●");
		else print("◎");
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
		if(sure_to_use) print("●");
		else print("◎");
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
		if(sure_to_use) print("●");
		else print("◎");
		moveto(8,11+choose);green(change(choose));
		moveto(12,11+choose);green(change((*cardmine)[choose-1].cost));
		moveto(15,11+choose);green("["+(*cardmine)[choose-1].name+"]");
		moveto(30,11+choose);
		for(int j=0;j<(*cardmine)[choose-1].speffect.size();j++)
			green("["+(*cardmine)[choose-1].speffect[j]+"]");
			
		moveto(10,24);print(string(50,' '));
		moveto(10,24);
		if((*cardmine)[choose-1].effect) print(explanation[(*cardmine)[choose-1].effect]);
		moveto(10,25);print(string(50,' '));moveto(10,25);
		if((*cardmine)[choose-1].effect==8)
		{
			if(recardheap.size()==0) print("当前弃牌堆没有牌，使用这张牌没有任何效果");
			else print("当前牌堆顶的牌为【"+recardheap[recardheap.size()-1].name+"】");
		}
	}
	if(handkind==2)
	{
		moveto(6,11+lastchoose);if(visskill[lastchoose-1]) red("  "+change(lastchoose));else print("  "+change(lastchoose));
		moveto(12,11+lastchoose);if(visskill[lastchoose-1]) red(change((*simpleskillsmine)[lastchoose-1].mana));else blue(change((*simpleskillsmine)[lastchoose-1].mana));
		moveto(15,11+lastchoose);if(visskill[lastchoose-1]) red("["+(*simpleskillsmine)[lastchoose-1].name+"]");else print("["+(*simpleskillsmine)[lastchoose-1].name+"]");
		
		moveto(6,11+choose);
		if(sure_to_use) print("●");
		else print("◎");
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
		print("平局");
		exit(0);
	}
	if(handcard1.size()==0)
	{
		system("cls");
		print("P1胜利\n");
		exit(0);
	}
	if(handcard2.size()==0)
	{
		system("cls");
		print("P2胜利\n");
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
		addcard(2,card("阻碍",3,0));
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
				if((*cardmine)[i].name=="惩罚")
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
		if(foundbuff(buffmine,"持久")==-1)
		{
			buff s;s.set_up("持久",0);
			buffmine->push_back(s);
			del=1;
		}
		(*buffmine)[foundbuff(buffmine,"持久")].tim+=2+del;
	}
	if(t.effect==12)
	{
		int s=ran(1,cardlong);
		while(s==12 or (15<=s and s<=17 and !timemode)) s=ran(1,cardlong);
		usecard(card("114514",0,s),pos);
		moveto(0,1);print("你使用了【"+cardname[s]+"】");
	}
	if(t.effect==13)
	{
		bool flagg=1;
		while(flagg)
		{
			flagg=0;
			for(int i=0;i<(cardmine->size());i++)
				if((*cardmine)[i].name=="一心同体")
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
			if((*cardmine)[i].name!="一心同体")
				cywakioi.push_back(i);
		if(cywakioi.size()) (*cardmine)[cywakioi[ran(0,cywakioi.size()-1)]]=card("一心同体",1,13);
	}
	if(t.effect==15)
		twelveam=min(twelveam+1,10);
	if(t.effect==16)
	{
		tim++;
		if(tim==5) tim=1;
	}
	if(t.effect==17)
		tim=ran(1,4);
	if(t.effect==18)
	{
		if(handcard1.size()>handcard2.size()) throwcard(&handcard1,ran(1,handcard1.size()));
		else if(handcard2.size()>handcard1.size()) throwcard(&handcard2,ran(1,handcard2.size()));
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
				if(found((*cardmine)[i].speffect,"加边！加边！加边！"))
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
	if(*soulmine=="迷茫") ddd=ran(1,3)-2;
	increasecost(costmine,costmaxmine,2+ddd);
	if(foundbuff(buffmine,"持久")!=-1)
		 increasecost(costmine,costmaxmine,1);
	increasemana(manamine,manamaxmine,10);
	addhandcard(cardmine,drawcard(),handcardmaxmine);
	if(tim==3) addhandcard(cardmine,drawcard(),handcardmaxmine);
	if(*soulmine=="暴戾")
		while((*cardmine)[(cardmine->size())-1].cost%2==0)
		{
			throwcard(cardmine,cardmine->size());
			addhandcard(cardmine,drawcard(),handcardmaxmine);
		}
	if(*soulmine=="谔谔")
	{
		if(ran(1,10)<=6)
		{
			vector<int> ee;ee.clear();
			for(int i=0;i<(cardmine->size());i++)
				if(!found((*cardmine)[i].speffect,"加边！加边！加边！"))
					ee.push_back(i);
			if(ee.size())
				(*cardmine)[ee[ran(0,ee.size()-1)]].speffect.push_back("加边！加边！加边！");	
		}
		bool eee=1;
		for(int i=0;i<(cardmine->size());i++)
			if(found((*cardmine)[i].speffect,"加边！加边！加边！"))
			{
				eee=0;
				break;
			}
		if(eee) increasecost(costmine,costmaxmine,1);
	}
	if(tim==1) increasecost(costmine,costmaxmine,1);
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
				if((*cardmine)[Choose-1].cost>*costmine) print("费用不足无法使用此牌！");
				else
				{
					print(string(24,' '));
					*costmine-=(*cardmine)[Choose-1].cost;
					usecard((*cardmine)[Choose-1],Choose);
					if(Choose>=(cardmine->size())) Choose=cardmine->size();
					lastchoose=Choose;
					if(tim==2)
						if(ran(1,10)<=twelveam)
							increasecost(costmine,costmaxmine,1);
					printground();
				}
				sure_to_use=0;
			}
			if(sure_to_magic==1)
			{
				moveto(10,27);
				print("确定要切换模式吗？");
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
				print("确定要结束回合吗？");
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
				if((*simpleskillsmine)[Choose-1].mana>*manamine) print("魔法不足无法使用该技能！");
				else if(visskill[Choose-1]) print("此回合已经使用过该技能！");
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
				print("确定要切换模式吗？");
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
				print("确定要结束回合吗？");
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
		print("        选择P"+change(i)+"的灵魂特质：");
		char c='T';
		int choose=1,sure_to_choose=0;
		while(1)
		{
			for(int j=1;j<=soullong;j++)
				moveto(4,j),print("  "+change(j)+" "+soullist[j]+" | "+soulexplanation[j]);
			moveto(4,choose);
			if(sure_to_choose==0) print("◎");
			if(sure_to_choose==1) print("●"); 
			green(change(choose)+" "+soullist[choose]+" | "+soulexplanation[choose]);
			
			moveto(0,10);REP(5) print(string(75,' ')+"\n");moveto(0,10); 
			for(int j=0;j<soulskill[choose].size();j++)
			{
				string activeword="";
				if(soulskill[choose][j].active) activeword="("+change(soulskill[choose][j].mana)+"■)";
				print(change(j+1)+".["+soulskill[choose][j].name+"]"+skillexplanation[soulskill[choose][j].effect]+activeword+"\n");
			}
			
			c=getch();
			if(upc(c) or leftc(c)) choose--,sure_to_choose=0;
			if(downc(c) or rightc(c)) choose++,sure_to_choose=0;
			if(usec(c) or surec(c)) sure_to_choose++;
			if('1'<=c and c<='6') choose=c-'0';
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
	printf("作者信息：\nLuogu:时律\nGithub:wzheian21219014\n按任意键继续\n");
	getch();
}
void option()
{
	system("cls");
	int choose=1;
	char c='T';
	while(1)
	{
		moveto(0,2);
		if(firsthand!=3)
			if(choose==1) green("先手玩家 P"+change(firsthand)+"  ");
			else print("先手玩家 P"+change(firsthand)+"  ");	
		else
			if(choose==1) green("先手玩家 随机");
			else print("先手玩家 随机");	
		moveto(0,4);
		if(choose==2)
			if(timemode) green("时间系统 开");
			else green("时间系统 关");
		else
			if(timemode) print("时间系统 开");
			else print("时间系统 关");
		c=getch();
		if(upc(c) or leftc(c)) choose--;
		if(downc(c) or rightc(c)) choose++;
		if(usec(c))
		{
			if(choose==1) firsthand++;
			if(choose==2) timemode=1-timemode;
		}
		if(surec(c))
			break;
		if(firsthand==4) firsthand=1;
		if(choose==3) choose=1;
		if(choose==0) choose=2;
	}
}
int main()
{
	//init_introduce();
	init();
	option();
	choosesoul();
	startgame();
	game();
}
