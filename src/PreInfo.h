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
    int x, y, width, height; //�������洰�ں�������Ϳ�ȡ��߶ȵı���
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
	printf("                                        ");
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
}lib[11][1005],fun[11][11][105];//fun[mode][occ][card]

int libcnt[11],funcnt[11][11];//funcnt[mode][occ]

struct spocc{
	int maxhp,cardcnt,maxdef,maxcost;
}job[11];

string Card::Intro(){
	if(func==1) return "+4��";
	else if(func==2)return "+1����+1������";
	else if(func==3)return "������+2��";
	else if(func==4)return "����[ȼ��]3�غϣ����ƻ���ʱ����Ч��";
	else if(func==5)return "����[��]���غϽ���";
	else if(func==6)return "����[�ж�]3�غϣ�ְҵΪ�ؾ�����ƻ���ʱ����Ч��";
	else if(func==7)return "���̲����������";
	else if(func==8)return "��������+1(���Ϊ6)";
	else if(func==9)return "HP����+40";
	else if(func==10)return "-1������";
	else if(func==11)return "HP����-60";
	else if(func==12)return "+2<�﷨��>���";
	else if(func==13)return "������ʧ����HP��25%";
	else if(func==14)return "�����»غ�[��]";
	else if(func==15)return "�������С�,���(35*��+25)�˺�,���ܻ���/buffӰ��";
	else if(func==16)return "����-3��";
	else if(func==17)return "���ڱ��غϡ��������»غ϶�[��]";
	else if(func==18)return "+2��";
	else if(func==19)return "+1<��ɳ�>���";
	else if(func==20)return "��͸���ܵ���ʵ�˺�";
	else if(func==21)return "���ֻ��50����";
	else if(func==22)return "HP����-20,+1<������>���";
	else if(func==23)return "����[����]3�غ�";
	else if(func==24)return "HP����-100,���<������>���;�ָ�����Ѫ";
	else if(func==25)return "+2����-1<��ƣ��>���";
	else if(func==26)return "��Ҫ̫̰�ģ�";
	else if(func==27)return "ˢ1��[�������]�����ƿ�;������ʱ���ķѲ���Ч";
	else if(func==28)return "+3��,ˢ3��[�������]���ƿ�;������ʱ���ķѲ���Ч";
	else if(func==29)return "������ʱ���ķѲ���Ч";
	else if(func==30)return "ˢ1��[�������]���ƿ�";
	else if(func==31)return "�������Buff�����ָֻ�20HP��ˢ1��[�������]���ƿ�";
	else if(func==32)return "���������[�������]��ÿ��[�������]ʹ������60ATK";
	else if(func==33)return "ˢ1��[�������]���ƿ�";
	else if(func==34)return "+2<��ɳ�>���";
	else if(func==35)return "����[����]2�غ�";
	else if(func==36)return "����[ȼ��]2�غ�";
	else if(func==37)return "������ձ�ǻ�á�,��һ����";
	else if(func==38)return "�����»غ�[�Ի�]";
	else if(func==39)return "˫��[����]2�غ�";
	else if(func==40)return "�������ƿ��һ���������Ч��MISS���������";
	else if(func==41)return "������������ʱ����ÿ��һ����ATK+10(�����Ƿ�MISS)";
	else if(func==42)return "�غϽ���ʱ,��������ȫ��[��ʥ��־],ֱ�ӻ�ʤ";
	else if(func==43)return "ˢ1��[��ʥ��־]�����ƿ�";
	else if(func==44)return "�����һ�����Ʋ�λ��Ϊ[��ʥ��־]";
	else if(func==45)return "�������ƿ��һ������Ч��Ϊ\"+2��\"��ϴ���ƿ�";
	else if(func==46)return "�����+4�������������һ����λ��Ϊ[�����ᾧ]";
	else if(func==47)return "��������������1������ +1������";
	else if(func==48)return "��ն���������[��ʥ��־],ÿ��һ���ö���+2<������>";
	else if(func==49)return "������2�غ�[����]";
	else if(func==50)return "����[ȼ��]2�غ�";
	else if(func==51)return "����������1-2�غ����buff";
	else if(func==52)return "����������1-2�غ����buff";
	else if(func==53)return "���ѡ����������,������1-2�غ����buff";
	else if(func==54)return "����Ͷ���������1-2�غ����buff";
	else if(func==55)return "��������������2�����Ʋ�λ��Ϊ[����˼ά]";
	else if(func==56)return "������2�غ�[�Ի�]";
	else if(func==57)return "(����)-2<��ƣ��>�������ƿ�������";
	else if(func==58)return "-3<��ƣ��>";
	else if(func==59)return "(����)-3<��ԭ��>,����hp����-80,����ƿ��[�������]";
	else if(func==60)return "(����)+2��,��������+1,��ǿ�����ƿ���,ʹ�ú���Ʊ�Ϊ[��������]";
	else if(func==61)return "(����)+1<������>,+60hp����";
	else if(func==62)return "(����)+3<�﷨��>,+1�����ޣ����ƿ�ˢ��2��[�����ɳ�]";
	else if(func==63)return "(����)+2<��ɳ�>,����ƿ���[��׸],������[��׸]ˢ������ƿ�";
	else if(func==64)return "(����)+3<������>,���ƿ���ˢ��[��ʥ��־][�������]��2��";
	else if(func==65)return "���ѡ�����ƿ�һ����,�����Ч��MISS����Ϊ�����Ҳ��ķ�";
	else if(func==66)return "25% +1<�﷨��>,�ƿ�������[�����ɳ�] +15ATK";
	else if(func==67)return "����[��]���غϽ���,��һ����";
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
	lib[0][9]=(Card){2,25,75,35,25,0,9};
	lib[0][10]=(Card){3,120,45,20,25,0,10};
	lib[0][11]=(Card){5,0,135,80,0,0,11};
	lib[0][12]=(Card){5,240,0,0,10,0,12};
	libcnt[0]=12;
	fun[2][0][1]=(Card){1,0,0,0,0,51,13};
	fun[2][0][2]=(Card){1,0,0,0,0,52,14};
	fun[2][0][3]=(Card){1,0,0,0,0,53,15};
	fun[2][0][4]=(Card){1,0,0,0,0,54,16};
	funcnt[2][0]=4;
	//travaller
	lib[1][1]=(Card){3,45,0,0,0,1,17};
	lib[1][2]=(Card){3,0,40,25,0,2,18};
	lib[1][3]=(Card){1,0,0,0,0,3,19};
	lib[1][4]=(Card){3,50,0,0,0,4,20};
	lib[1][5]=(Card){2,0,0,0,0,5,21};
	lib[1][6]=(Card){3,35,0,0,0,6,22};
	lib[1][7]=(Card){0,0,0,0,25,7,23};
	lib[1][8]=(Card){4,0,30,50,0,8,24};
	lib[1][9]=(Card){2,0,60,0,0,9,25};
	lib[1][10]=(Card){1,0,0,80,0,21,26};
	lib[1][11]=(Card){2,0,30,0,0,23,27};
	libcnt[1]=11;
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
	lib[2][8]=(Card){2,0,160,0,0,11,36};
	lib[2][9]=(Card){0,0,0,0,0,22,37};
	lib[2][10]=lib[2][9];
	lib[2][11]=(Card){1,0,0,0,0,24,38};
	lib[2][12]=lib[2][3];
	libcnt[2]=12;
	fun[1][2][1]=(Card){1,1,70,0,0,61,39};
	funcnt[2][1]=1;
	job[2]={600,4,80,5};
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
	lib[3][11]=(Card){2,155,50,0,40,35,50};
	lib[3][12]=(Card){3,185,0,0,20,36,51};
	lib[3][13]=(Card){1,0,-20,0,0,37,52};
	libcnt[3]=13;
	fun[1][3][1]=(Card){2,0,120,0,0,62,53};
	fun[1][3][2]=(Card){2,70,0,0,0,66,54};
	funcnt[1][3]=2;
	job[3]={330,4,120,7};
	//warrior
	lib[4][1]=(Card){2,0,0,95,0,0,55};
	lib[4][2]=(Card){1,0,50,55,0,14,56};
	lib[4][3]=lib[4][2];
	lib[4][4]=(Card){1,80,0,0,15,0,57};
	lib[4][5]=(Card){2,135,0,0,20,0,58};
	lib[4][6]=lib[4][5];
	lib[4][7]=(Card){0,0,0,0,0,15,59};
	lib[4][8]=(Card){3,125,0,60,0,0,60};
	lib[4][9]=(Card){4,125,-30,0,0,4,61};
	lib[4][10]=(Card){2,0,10,10,0,16,62};
	lib[4][11]=(Card){4,245,0,0,10,0,63};
	lib[4][12]=(Card){3,0,0,150,0,0,64};
	lib[4][13]=(Card){2,0,55,30,0,0,65};
	lib[4][14]=lib[4][13];
	lib[4][15]=(Card){1,0,0,10,0,25,66};
	lib[4][16]=(Card){2,20,-30,0,0,17,67};
	lib[4][17]=(Card){2,0,50,0,0,58,68};
	libcnt[4]=17;
	fun[1][4][1]=(Card){1,0,0,0,0,57,69};
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
	lib[5][16]=(Card){4,0,0,0,0,26,81};
	lib[5][17]=lib[5][3];
	lib[5][18]=(Card){3,0,35,0,0,34,82};
	libcnt[5]=18;
	fun[1][5][1]=(Card){3,0,75,0,0,63,83};
	funcnt[1][5]=1;
	job[5]={280,3,0,3};
	//demon
	lib[6][1]=(Card){1,0,45,0,0,31,84};
	lib[6][2]=(Card){2,0,70,0,0,33,85};
	lib[6][3]=(Card){2,65,20,0,10,0,86};
	lib[6][4]=(Card){1,25,20,10,0,30,87};
	lib[6][5]=(Card){5,230,30,30,0,29,88};
	lib[6][6]=(Card){1,0,0,0,0,28,89};
	lib[6][7]=(Card){2,95,0,0,0,30,90};
	lib[6][8]=(Card){3,110,20,30,0,30,91};
	lib[6][9]=(Card){0,15,0,0,0,0,92};
	lib[6][10]=(Card){4,160,25,10,0,27,93};
	lib[6][11]=(Card){3,95,40,0,10,0,94};
	lib[6][12]=(Card){4,130,35,0,10,29,95};
	lib[6][13]=lib[6][1];
	lib[6][14]=(Card){1,45,0,0,10,0,96};
	lib[6][15]=lib[6][3];
	lib[6][16]=lib[6][14];
	lib[6][17]=(Card){3,95,50,0,0,33,97};
	libcnt[6]=17;
	lib[6][libcnt[6]+1]=(Card){1,0,0,0,0,27,98};
	lib[6][libcnt[6]+2]=(Card){2,0,0,0,0,32,99};
	fun[1][6][1]=(Card){2,80,45,0,0,59,100};
	funcnt[1][6]=1;
	job[6]={400,4,40,5};
	//minister
	lib[7][1]=(Card){1,30,0,0,0,38,101};
	lib[7][2]=(Card){2,0,65,0,0,39,102};
	lib[7][3]=(Card){1,0,20,0,15,40,103};
	lib[7][4]=(Card){2,0,40,20,0,56,104};
	lib[7][5]=(Card){1,0,0,0,0,41,105};
	lib[7][6]=(Card){2,10,70,0,0,43,106};
	lib[7][7]=(Card){1,0,45,0,0,43,107};
	lib[7][8]=(Card){2,0,55,15,0,44,108};
	lib[7][9]=(Card){3,0,65,30,0,45,109};
	lib[7][10]=(Card){2,0,0,0,0,46,110};
	lib[7][11]=(Card){2,0,60,10,0,47,111};
	lib[7][12]=(Card){2,0,20,10,0,55,112};
	lib[7][13]=lib[7][5];
	lib[7][14]=(Card){3,40,60,0,0,49,113};
	libcnt[7]=14;
	lib[7][libcnt[7]+1]=(Card){5,0,180,60,0,42,114};
	lib[7][libcnt[7]+2]=(Card){2,20,0,0,0,48,115};
	lib[7][libcnt[7]+3]=(Card){0,0,15,0,0,0,116};
	fun[1][7][1]=(Card){2,0,120,0,0,64,117};
	fun[1][7][2]=(Card){0,0,0,0,30,65,118};
	funcnt[1][7]=2;
	job[7]={450,4,150,6};

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
	else return "[δ����]";
}


string occ_intro(int x){
	if(x==1)return "û�г�����Ҳͬ��û������";
	else if(x==2)return "�����������Ѫ��ȡ��֮����";
	else if(x==3)return "���������壬����������Ҫ";
	else if(x==4)return "��ʵ�ķ�����ǿ�������";
	else if(x==5)return "���������ݣ�ͨ�������������";
	else if(x==6)return "������������������Ŷ�������Ч����Ʒ�ʽ";
	else if(x==7)return "�ܵ���ʥ��ף����ǿ����������������";
	else if(x==8)return "�����Ҷ����棬�����һ��";
	return "";
}

void occ_func(int x){
	SetColor(7,0);
	if(x==1){
		printf("HP 420   MAX_DEF 120   ��������4   ��6");printf(" ");
		printf("\n\t");printf("                                                ");
		printf("\n\t 1.����ְҵ�����ƶ��й��о�,�Ҷ�����Ч");printf("                  ");
		printf("\n\t 2.[ʰ��] ���غϿ�ʼʱû�з�������1�����");printf("                 ");
		printf("\n");printf("                                                        ");
		printf("\n");printf("                                               ");
	}else if(x==2){
		printf("HP 600   MAX_DEF 80   ��������4   ��5");printf("  ");SetColor(13);
		printf("\n\t<������> ÿ�����ʹATK����6");printf("                     ");;SetColor(7);
		printf("\n\t 1.ÿ���Լ����ƶ��ܵ��˺���+1<������>");printf("               ");
		printf("\n\t 2.һЩְҵ�����ƻ�ͨ�������Լ����������");printf("               ");
		printf("\n");printf("                                                        ");
		printf("\n");printf("                                               ");
	}else if(x==3){
		printf("HP 320   MAX_DEF 120   ��������4   ��7");printf(" ");SetColor(13);
		printf("\n\t<�﷨��>�»غϻ�õ��ڱ�������ġ�����ձ��");printf("          ");SetColor(7);
		printf("\n\t 1.[��ͨ] ÿ�غϿ�ʼ��45%���� +1<�﷨��>");printf("               ");
		printf("\n\t 2.[��������] ��HP��100,ÿ��1��<�﷨��>���ʱ�ָ�20HP");printf("         ");
		printf("\n");printf("                                                        ");
		printf("\n");printf("                                               ");
	}else if(x==4){
		printf("HP 500   MAX_DEF 240   ��������4   ��6");printf(" ");SetColor(13);
		printf("\n\t<��ƣ��>ÿ�����ʹ����������4%%");printf("                      ");SetColor(7);
		printf("\n\t 1.[��ս״̬] �غϿ�ʼʱ-1<��ƣ��>");printf("                ");
		printf("\n\t 2.[��������] ʹ�ù����ƺ�+2<��ƣ��>");printf("                       ");
		printf("\n\t 3.[װ������] ÿ�غϿ�ʼʱ��û�л��� �򻤼�+45");printf("                 ");
		printf("\n\t 4.[��η] �޷��鵽�����ƿ���������");printf("      ");
	}else if(x==5){
		printf("HP 280   MAX_DEF 0   ��������3   ��3");printf("   ");SetColor(13);
		printf("\n\t<��ɳ�>ÿ�����ʹ��ATK+9%% HP����+10 MAX_DEF+10");printf("             ");SetColor(7);
		printf("\n\t 1.[̰��] �غϿ�ʼʱ��Ϊ3��,ʹ���ƺ���80%�����");printf("           ");
		printf("\n\t 2.[����] ÿ�����1��<��ɳ�>ʱ�ָ�15HP");printf("                    ");
		printf("\n\t 3.[��������] �޷��鵽�����ƿ��е���");printf("            ");
		printf("\n\t 4.[����] �������д�͸���ܹ���������");printf("      ");
	}
	else if(x==6){
		printf("HP 400   MAX_DEF 40   ��������4   ��5");printf("   ");SetColor(13);
		printf("\n\t<��ԭ��>�غϿ�ʼʱÿ����Ƕ������5���˺�");printf("             ");SetColor(7);
		printf("\n\t 1.[Դ�����] ����ʱ0��,ÿ�غ϶�����1��");printf("               ");
		printf("\n\t 2.[��հ�Ϸ] ������������Ӧ���ò���Ϊ���,ATK��HEAL����");printf("          ");
		printf("\n\t 3.[���] ÿ������ʱ(�����Ƿ���Ϊ���)+1<��ԭ��>");printf("        ");
		printf("\n\t 4.[��������] �޷��鵽�����ƿ��е���");printf("        ");
	}
	else if(x==7){
		printf("HP 450   MAX_DEF 120   ��������4   ��6");printf("   ");SetColor(13);
		printf("\n\t<������>ÿ����Ƕ�������2%�鵽[��ʥ��־]�ĸ���");printf("             ");SetColor(7);
		printf("\n\t 1.[��] ������ȫ��[��ʥ��־]��ֱ�ӻ�ʤ");printf("               ");
		printf("\n\t 2.[����] ���غϽ���ʱû�з��� ��+1<������>");printf("                 ");
		printf("\n\t 3.[���˳��] ��Ķ����ƿ�������[����]��");printf("         ");
		printf("\n\t 4.[��ƽ] �޷��鵽�����ƿ���ATK��80����");printf("          ");
	}
	else if(x==8){
		printf("HP ???   MAX_DEF ???   ��������?   ��?");printf(" ");
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
		printf("���ػ�ȡ����:�غϿ�ʼʱ����2��HP��210");printf("            ");
	}
	else if(x==2){
		printf("���ػ�ȡ����:�غϿ�ʼʱ���HP��500");printf("              ");
	}
	else if(x==3){
		printf("���ػ�ȡ����:�غϿ�ʼʱHP��160");printf("                   ");
	}
	else if(x==4){
		printf("���ػ�ȡ����:�غϿ�ʼʱ<��ƣ��>��8");printf("               ");
	}
	else if(x==5){
		printf("���ػ�ȡ����:�غϿ�ʼʱ���HP��320��HP��240");printf("     ");
	}
	else if(x==6){
		printf("���ػ�ȡ����:�غϿ�ʼʱ<��ԭ��>��5");printf("               ");
	}
	else if(x==7){
		printf("���ػ�ȡ����:�غϿ�ʼʱ<������>��4��HP��300");printf("      ");
	}
	else if(x==8){
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
	if(id==1)      return "˫�����˺�����20%          ";
	else if(id==2) return "˫�����˺�����20%          ";
	else if(id==3) return "˫��ʧ��������10%          ";
	else if(id==4) return "˫��ʧ���ʼ���10%          ";
	else if(id==5) return "˫��ÿ�غϻظ�5%���hp��hp ";
	else if(id==6) return "˫��ÿ�غ���ʧ5%���hp��hp ";
	else if(id==7) return "˫��ÿ�غ϶�����1�����   ";
	else           return "������Ч��                 ";
}