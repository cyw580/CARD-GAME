#include<bits/stdc++.h>
using namespace std;
int n;
string s;
int main(){
	freopen("wizard.inf","r",stdin);
	freopen("get.out","w",stdout);
	int k=3;
	cin>>n;
	int a,b,c,d;
	for(int i=1,e,f;i<=n;i++) {
		printf("lib[%d][%d]",k,i);
		cin>>s>>a>>b>>c>>d>>e>>f;
		cout<<"={";
		putchar('\"');
		cout<<s;
		putchar('\"');
		printf(",%d,%d,%d,%d,%d,%d};\n",a,b,c,d,e,f);
	}
	printf("libcnt[%d]=%d;\n",k,n);
	printf("job[%d]",k);
	cin>>a>>b>>c>>d;
	printf("={%d,%d,%d,%d};\n",a,b,c,d);
}
