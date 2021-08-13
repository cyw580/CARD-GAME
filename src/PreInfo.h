void SetPos(int x, int y)
{
        COORD coord;
        coord.X = x;
        coord.Y = y;

        //HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
        //SetConsoleCursorPosition(hout, coord);//������Ĵ���һ������

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
	string name;
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
	if(func==1) return "+4��";
	else if(func==2)return "+1����+1������";
	else if(func==3)return "������+2��";
	else if(func==4)return "����[ȼ��]3�غϣ����ƻ���ʱ����Ч��";
	else if(func==5)return "����[��]���غϽ���";
	else if(func==6)return "����[�ж�]3�غϣ�ְҵΪ�ؾ�����ƻ���ʱ����Ч��";
	else if(func==7)return "���̲����������";
	else if(func==8)return "��������+1";
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
	else if(func==37)return "������ձ�ǻ�÷���";
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
	else if(func==51)return "�Է�������1-2�غ����buff";
	else if(func==52)return "����������1-2�غ����buff";
	else if(func==53)return "���ѡ�������Է�,������1-2�غ����buff";
	else if(func==54)return "����ͶԷ�������1-2�غ����buff";
	else if(func==55)return "��������������2�����Ʋ�λ��Ϊ[����˼ά]";
	else if(func==56)return "������2�غ�[�Ի�]";
	else if(func==57)return "��<��ƣ��>������10��,-3<��ƣ��>�������ƿ�������";
	else if(func==58)return "��ն���������[��ʥ��־] ���ֻ�õ���<������>";
	else if(func==59)return "��ն���������[��ʥ��־] ���ֻ�õ���<������>";
	else if(func==60)return "��ն���������[��ʥ��־] ���ֻ�õ���<������>";
	return "";
}

void previous(){
	//��50 ATK��45 HEAL��35 DEFΪ1�ѱ�׼�����߷��ʵ�����
	//public
	lib[0][1]={"[����]",0,15,0,0,0,0};
	lib[0][2]={"[����II]",0,25,0,0,0,0};
	lib[0][3]={"[�ظ�ħ��]",0,0,20,0,0,0};
	lib[0][4]={"[���׻���]",1,55,0,0,15,0};
	lib[0][5]={"[����]",1,0,0,35,0,0};
	lib[0][6]={"[����]",2,0,75,0,0,0};
	lib[0][7]={"[������]",2,95,0,0,0,0};
	lib[0][8]={"[��������]",2,65,35,25,15,0};
	lib[0][9]={"[ƽ������]",2,25,75,35,25,0};
	lib[0][10]={"[��ͨ����]",3,120,45,20,25,0};
	lib[0][11]={"[�ظ�Ĥ��]",5,0,130,75,0,0};
	lib[0][12]={"[�սᷨ��]",5,240,0,0,10,0};
	libcnt[0]=12;
	fun[2][0][1]={"[�𻵵�ħ��I]",1,0,0,0,0,51};
	fun[2][0][2]={"[�𻵵�ħ��II]",1,0,0,0,0,52};
	fun[2][0][3]={"[�𻵵�ħ��III]",1,0,0,0,0,53};
	fun[2][0][4]={"[�𻵵�ħ��IV]",1,0,0,0,0,54};
	funcnt[2][0]=4;
	fun[3][0][1]={"[�𻵵�ħ��I]",1,0,0,0,0,51};
	fun[3][0][2]={"[�𻵵�ħ��II]",1,0,0,0,0,52};
	fun[3][0][3]={"[�𻵵�ħ��III]",1,0,0,0,0,53};
	fun[3][0][4]={"[�𻵵�ħ��IV]",1,0,0,0,0,54};
	fun[3][0][5]={"[�������I]",1,0,0,0,0,0};
	fun[3][0][6]={"[�������II]",2,0,0,0,0,0};
	fun[3][0][7]={"[�������III]",3,0,0,0,0,0};
	fun[3][0][8]={"[�������IV]",4,0,0,0,0,0};
	funcnt[3][0]=8;
	//travaller
	lib[1][1]={"[������]",3,45,0,0,0,1};
	lib[1][2]={"[��������]",3,0,40,25,0,2};
	lib[1][3]={"[���ݹ�Ӯ]",1,0,0,0,0,3};
	lib[1][4]={"[ȼ������]",3,50,0,0,0,4};
	lib[1][5]={"[���ܿ�ŭ]",2,0,0,0,0,5};
	lib[1][6]={"[�㶾����]",3,45,0,0,0,6};
	lib[1][7]={"[��������]",0,0,0,0,25,7};
	lib[1][8]={"[�ִ�����]",4,0,30,50,0,8};
	lib[1][9]={"[������]",2,0,60,0,0,9};
	lib[1][10]={"[��������]",1,0,0,80,0,21};
	lib[1][11]={"[��ʱ��Ϣ]",2,0,30,0,0,23};
	libcnt[1]=11;
	job[1]={420,4,120,6};
	//caster
	lib[2][1]={"[���Эͬ]",1,55,-40,0,0,0};
	lib[2][2]={"[���һ���]",0,0,-25,0,0,0};
	lib[2][3]={"[����]",2,0,-75,0,0,1};
	lib[2][4]={"[��ȼ���]",2,35,-45,0,0,50};
	lib[2][5]={"[����]",1,0,-60,0,0,5};
	lib[2][6]={"[����]",3,0,120,35,0,0};
	lib[2][7]={"[����ǿ]",1,80,0,0,0,10};
	lib[2][8]={"[����֮��]",2,0,160,0,0,11};
	lib[2][9]={"[����]",0,0,0,0,0,22};
	lib[2][10]={"[����]",0,0,0,0,0,22};
	lib[2][11]={"[����]",1,0,0,0,0,24};
	lib[2][12]={"[����]",2,0,-70,0,0,1};
	libcnt[2]=12;
	job[2]={600,4,80,5};
	//wizard
	lib[3][1]={"[��ȼ]",2,25,0,0,0,50};
	lib[3][2]={"[���浯]",0,45,0,0,30,0};
	lib[3][3]={"[�ױ�]",7,330,0,0,0,0};
	lib[3][4]={"[Ͷ��]",1,0,0,0,0,12};
	lib[3][5]={"[��������]",2,20,0,55,0,0};
	lib[3][6]={"[�����η���]",6,135,35,30,0,12};
	lib[3][7]={"[Ѫɫŭ��]",2,15,-45,0,0,5};
	lib[3][8]={"[������ʴ]",2,0,0,0,0,13};
	lib[3][9]={"[��������]",2,0,35,0,0,12};
	lib[3][10]={"[����һ��]",1,65,0,0,0,0};
	lib[3][11]={"[���ȶ�Ԫ��]",2,145,45,0,40,35};
	lib[3][12]={"[�����Ϸ]",3,185,0,0,20,36};//ȼ���Լ�2�غ�
	lib[3][13]={"[��������]",1,0,-20,0,0,37};//������ձ�ǻ�÷���
	libcnt[3]=13;
	job[3]={320,4,120,7};
	//warrior
	lib[4][1]={"[�������]",2,0,0,95,0,0};
	lib[4][2]={"[����]",1,0,50,55,0,14};
	lib[4][3]={"[����]",1,0,50,55,0,14};
	lib[4][4]={"[���]",1,80,0,0,15,0};
	lib[4][5]={"[çײ]",2,135,0,0,20,0};
	lib[4][6]={"[çײ]",2,135,0,0,20,0};
	lib[4][7]={"[��ˮһս]",0,0,0,0,0,15};
	lib[4][8]={"[�ֶܳ��]",3,125,0,60,0,0};
	lib[4][9]={"[����ȼľ]",4,125,-30,0,0,4};
	lib[4][10]={"[ͣսЭ��]",2,0,10,10,0,16};
	lib[4][11]={"[����]",4,245,0,0,10,0};
	lib[4][12]={"[���Է���]",3,0,0,150,0,0};
	lib[4][13]={"[С�ͼ��Ȱ�]",2,0,55,30,0,0};
	lib[4][14]={"[С�ͼ��Ȱ�]",2,0,55,30,0,0};
	lib[4][15]={"[����װ��]",1,0,0,10,0,25};
	lib[4][16]={"[��ŭ����]",2,20,-30,0,0,17};
	libcnt[4]=16;
	lib[4][libcnt[4]+1]={"[����ʿ��]",1,0,0,0,0,57};
	job[4]={500,4,240,6};
	//goblin
	lib[5][1]={"[��������]",2,0,0,0,0,6};
	lib[5][2]={"[�ɼ�]",1,0,10,0,0,18};
	lib[5][3]={"[����]",1,0,0,0,20,19};
	lib[5][4]={"[����֮��]",3,30,0,0,0,18};
	lib[5][5]={"[����ѵ��]",2,50,0,0,0,19};
	lib[5][6]={"[����ѵ��]",2,50,0,0,0,19};
	lib[5][7]={"[����ѵ��]",2,0,50,0,0,19};
	lib[5][8]={"[����ѵ��]",2,0,0,40,0,19};
	lib[5][9]={"[ȫ��ѵ��]",3,40,35,25,0,19};
	lib[5][10]={"[ȫ��ѵ��]",3,40,35,25,0,19};
	lib[5][11]={"[Ǳ������]",1,25,0,0,0,20};
	lib[5][12]={"[Ǳ������]",1,25,0,0,0,20};
	lib[5][13]={"[Ǳ������]",1,25,0,0,0,20};
	lib[5][14]={"[Ǳ��Σ��]",3,80,0,0,0,20};
	lib[5][15]={"[���潻��]",2,65,0,0,0,0};
	lib[5][16]={"[��׸]",4,0,0,0,0,26};
	lib[5][17]={"[����]",1,0,0,0,20,19};
	lib[5][18]={"[���ٷ���]",3,0,35,0,0,34};
	libcnt[5]=18;
	job[5]={280,3,0,3};
	//demon
	lib[6][1]={"[�������]",1,0,35,0,0,31};
    lib[6][2]={"[�������]",2,0,70,0,0,33};
	lib[6][3]={"[�����ۻ�]",2,60,20,0,10,0};
	lib[6][4]={"[���ħ��]",1,20,20,10,0,30};
    lib[6][5]={"[��Ӱը��]",5,200,20,30,0,29};
    lib[6][6]={"[�������]",1,0,0,0,0,28};
    lib[6][7]={"[���֮��]",2,90,0,0,0,30};
    lib[6][8]={"[�������]",3,100,15,30,0,30};
    lib[6][9]={"[�ڰ���Ϣ]",0,15,0,0,0,0};
    lib[6][10]={"[�������]",4,150,25,10,0,27};
    lib[6][11]={"[����ӿ��]",3,90,40,0,10,0};
    lib[6][12]={"[���ܱ���]",4,130,35,0,10,29};
    lib[6][13]={"[�������]",1,0,35,0,0,31};
    lib[6][14]={"[��Ӱ��]",1,45,0,0,10,0};
    lib[6][15]={"[�����ۻ�]",2,60,20,0,10,0};
    lib[6][16]={"[��Ӱ��]",1,45,0,0,10,0};
	lib[6][17]={"[�ڶ�����]",3,90,50,0,0,33};
    libcnt[6]=17;
	lib[6][libcnt[6]+1]={"[�������]",1,0,0,0,0,27};
	lib[6][libcnt[6]+2]={"[�������]",2,0,0,0,0,32};
	job[6]={400,4,40,5};
	//minister
	lib[7][1]={"[˵��]",1,30,0,0,0,38};//�����»غ�[�Ի�]
	lib[7][2]={"[���ƹ⻷]",2,0,55,0,0,39};//˫�����2�غ�����
	lib[7][3]={"[����ȫ����]",1,0,15,0,15,40};//�������ƿ��һ���������Ч��MISS���������
	lib[7][4]={"[��ʶ����]",2,0,30,20,0,56};//������2�غ�[�Ի�]
	lib[7][5]={"[��ʶ����]",1,0,0,0,0,41};//������������ʱ����ÿ��һ����ATK+10(�����Ƿ�MISS)
	lib[7][6]={"[����]",2,10,75,0,0,43};//ˢ1��[��ʥ��־]�����ƿ�
	lib[7][7]={"[�������]",1,0,50,0,0,43};
	lib[7][8]={"[ڤ��]",2,0,45,15,0,44};//�����һ�����Ʋ�λ��Ϊ[��ʥ��־]
	lib[7][9]={"[��������]",2,0,55,25,0,45};//�������ƿ��һ������Ч��Ϊ"+2��"��ϴ���ƿ�
	lib[7][10]={"[�����ᾧ]",2,0,0,0,0,46};//�����+4�������������һ����λ��Ϊ[�����ᾧ]
	lib[7][11]={"[����˼ά]",2,0,50,10,0,47};//��������������1������ +1������
	lib[7][12]={"[����˼ά]",2,0,20,10,0,55};//��������������2�����Ʊ�Ϊ[����˼ά]
	lib[7][13]={"[��ʶ����]",1,0,0,0,0,41};
	lib[7][14]={"[ʥ��ϴ��]",3,40,70,0,0,49};//����[����]2�غ�
	libcnt[7]=14;
	lib[7][libcnt[7]+1]={"[��ʥ��־]",4,0,140,50,0,42};//������������4��[��ʥ��־]��ֱ��Ӯ����Ϸ
	lib[7][libcnt[7]+2]={"[����]",2,20,0,0,0,48};//��ն���������[��ʥ��־]��ÿһ���ö���+2<������>
	lib[7][libcnt[7]+3]={"[����˼ά]",0,0,10,0,0,0};
	job[7]={480,4,120,6};
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
		printf("\n\t 1.����ְҵ�����ƶ��й��оأ��Ҷ�����Ч");printf("                  ");
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
		printf("\n\t 1.[��ͨ] ÿ�غϿ�ʼ��35%���� +1<�﷨��>");printf("               ");
		printf("\n\t 2.[��������] ��HP��100��ÿ��1��<�﷨��>���ʱ�ָ�20HP");printf("         ");
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
		printf("\n\t<��ɳ�>ÿ�����ʹ��ATK+8%% HP����+10 MAX_DEF+10");printf("             ");SetColor(7);
		printf("\n\t 1.[̰��] �غϿ�ʼʱ��Ϊ3����ʹ���ƺ���80%�����");printf("           ");
		printf("\n\t 2.[����] ÿ�����1��<��ɳ�>ʱ�ָ�20HP");printf("                    ");
		printf("\n\t 3.[��������] �޷��鵽�����ƿ��е���");printf("            ");
		printf("\n\t 4.[����] �������д�͸���ܹ���������");printf("      ");
	}
	else if(x==6){
		printf("HP 400   MAX_DEF 40   ��������4   ��5");printf("   ");SetColor(13);
		printf("\n\t<��ԭ��>�غϿ�ʼʱÿ����Ƕ������5���˺�");printf("             ");SetColor(7);
		printf("\n\t 1.[Դ�����] ����ʱ0����ÿ�غ϶�����1��");printf("               ");
		printf("\n\t 2.[��հ�Ϸ] ������������Ӧ���ò���Ϊ�����ATK��HEAL����");printf("          ");
		printf("\n\t 3.[���] ÿ������ʱ(�����Ƿ���Ϊ���)+1<��ԭ��>");printf("        ");
		printf("\n\t 4.[��������] �޷��鵽�����ƿ��е���");printf("        ");
	}
	else if(x==7){
		printf("HP 480   MAX_DEF 120   ��������4   ��6");printf("   ");SetColor(13);
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