#include<iostream>
#include<string>
#include<random>
const int N = 1e5+7; //数组的大小 100007
int a[N];
int temp[N];

int n; // 需要排序的个数 
int read() //
{	int x=0,f=1;
	char ch=getchar();
	while(ch<'0'||ch>'9')
	{
	if(ch=='-')f=-1;
	ch=getchar();
	}
	while(ch>='0'&&ch<='9')x=x*10+ch-'0',ch=getchar();return x*f;
}
void Write(int x)
{
	if(x<0){putchar('-'),Write(-x);return;}
	if(x<10){putchar(x+'0');return;}
	Write(x/10),putchar(x%10+'0');
}
void read_data() //读入数据 
{
	std::cin>>n;
	
	for(int i=1;i<=n;i++) a[i]=read();
} 
void output()
{
	for(int i=1;i<=n;i++)
	{
		Write(a[i]);
		putchar(' '); 
	}
	std::cout<<'\n';
//	getchar(); 
}
int randint(int l, int r){ // 生成在 [l, r] 之间的随机数
	return rand() % (r - l + 1) + l;
}
void quick_sort(int left, int right)
{
	if(left>=right) return;
	
	int mid = randint(left,right);
	
	if(right==left+1)
	{
		if(a[left]>a[right]) std::swap(a[left],a[right]);
		return;
	} 
	int index1=left,index2=right;
	
	for(int i=left;i<=right;i++)
	{
		if(i==mid) continue;
		if(a[i]<a[mid]) temp[index1++]=a[i];
		else temp[index2--]=a[i]; 
	}
	temp[index1]=a[mid];
	for(int i=left;i<=right;i++) a[i]=temp[i];
//	output(); 
	quick_sort(left,index1-1);
	quick_sort(index2+1,right);
}

int main()
{
	read_data();
	quick_sort(1,n);
	output();
} 