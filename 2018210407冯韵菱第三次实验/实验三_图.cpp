#include "pch.h"
#include <iostream>
#include <stdio.h>
#include <windows.h>
#include<iomanip>
#include<stack>
#include <vector>
#include <queue>
LARGE_INTEGER nFreq;
LARGE_INTEGER t1;
LARGE_INTEGER t2;			//程序计时
LARGE_INTEGER t3;
LARGE_INTEGER t4;
long double dt;
using namespace std;
struct ArcNode
{
	int adjvex;
	ArcNode *next;	
	int weight;					//权值
};
struct VertexNode			//改为双向  先询问是单向图还是双向图
{
	char vertex;						//顶点
	ArcNode *first;					//弧长
};
struct VEdge
{
	int start;
	int end;
	int weight;
};
struct shor
{
	int poi;							//数组下标位置
	int weight;
};
struct cmp						//定义比较算法
{
	bool operator()(shor a, shor b)
	{
		return a.weight > b.weight;			//小根堆
	}
};
template <class T> class ALGraph
{
public:
	ALGraph();																	//两种构造 一种自己输入  一种使用已输入数据测试
	ALGraph (int x,int y, char a[],int arcs[][3]);					//都是利用的邻接表 建立
	~ALGraph();
	void DFS (int v);														//主调的深搜
	void dfs();																	//深搜主体，使用stack进行非递归实现遍历图
	void BFS (int v);														//主调的广搜
	void bfs();																	//广搜主体，利用队列遍历
	void Prim();																//普利姆，不断选择最小的点并加入
	int mininum(int lowcost[]);										//普利姆的辅助函数，寻找Lowcost中的最小值
	void Kruskal();															//克鲁斯卡尔，不断寻找最小的边 并加入
	void GenSortEdge();													//利用直接插入排序  将边进行赋值并排序
	void shortest();															//寻找最短路径
	int GetMin(int* Length, bool* ifadded);						//第一种  改进前
	int GetMin2(int* Length, bool* ifadded);					//改进后  速度更快
	void PrintPath(int * Length, int*Path,int origin,int end);		//递归打印最短路径
private:
	int Max;
	VertexNode *adjlist;
	int vNum, arcNum;
	bool* visited;
	int* adjvex;
	int* lowcost;
	VEdge* Edgelist;
};
template <class T> ALGraph<T> ::ALGraph()
{
	cout << '\n' << "Please put in the number of the vertice and the number of the arc:	";
	int x, y;
	cin>>x>>y;
	vNum = x;
	arcNum = y;
	Max = vNum;
	adjlist = new VertexNode[Max];
	visited = new bool[Max];
	adjvex = new int[Max];
	lowcost = new int[Max];
	Edgelist = new VEdge[arcNum];
	for (int i = 0;i < Max;i++)
	{
		lowcost[i] = -1;
		visited[i] = 0;
	}
	cout << '\n' << "Please put in the vertices:	";
	for (int i = 0;i < vNum;i++)
	{
		cin >> adjlist[i].vertex;
		adjlist[i].first = NULL;				//读入一个就初始化其指针
	}
	cout <<'\n'<< "Please put in the arcs and its weight:	" << endl;
	for (int i = 0;i < arcNum;i++)
	{
		int k, j, weight;
		cin >> k >> j >> weight;
		ArcNode *s = new ArcNode;
		s->adjvex = j;
		s->next = adjlist[k].first;				//头插
		adjlist[k].first = s;
		s->weight = weight;
	}
}
template <class T> ALGraph<T> ::ALGraph (int x, int y, char a[], int arcs[][3])
{
	vNum = x;
	arcNum = y;
	Max = vNum;
	adjlist = new VertexNode[Max];
	visited = new bool[Max];
	adjvex = new int[Max];
	lowcost = new int[Max];
	Edgelist = new VEdge[arcNum];
	for (int i = 0;i < Max;i++)
	{
		visited[i] = 0;
		lowcost[i] = -1;
	}
	for (int i = 0;i < vNum;i++)
	{
		adjlist[i].vertex = a[i];
		adjlist[i].first = NULL;				//读入一个就初始化其指针
	}
	for (int i = 0;i < arcNum;i++)
	{
		ArcNode *s = new ArcNode;
		s->adjvex = arcs[i][1];
		s->next = adjlist[arcs[i][0]].first;				//头插
		adjlist[arcs[i][0]].first = s;
		s->weight = arcs[i][2];
	}
}
template <class T>
 void ALGraph<T> :: DFS(int v)		//改成了非递归的 可以防止溢出
{
	stack<int> a;
	cout << adjlist[v].vertex;							//加入第一个顶点
	visited[v] = true;
	a.push(v);
	while (!a.empty())										//只要栈不空 就一直循环
	{
		ArcNode* temp = adjlist[a.top()].first;		//指向栈顶元素
		bool ifad = true;										//辅助判断 该栈顶元素是否 还有未加入的点
		while (temp)
		{
			if (visited[temp->adjvex] == 0)			//如果有未加入的点 则输出、入栈、并改为已经访问过
			{
				cout << "->" << adjlist[temp->adjvex].vertex;
				a.push(temp->adjvex);
				visited[temp->adjvex] = 1;
				ifad = false;
				break;
			}
			temp = temp->next;
		}
		if (ifad)													//如果该点已经没有未访问过的节点，则栈顶元素出栈
		{
			if (!a.empty())
				a.pop();
			else
				return;
		}
	}
}
 template <class T>
 void ALGraph<T> ::dfs()
 {
	 cout << '\n' << "To use the DFS:";
	 cout <<'\n'<<"Please put in the starting vertex:	";
	 int startvex;
	 cin >> startvex;
	 DFS(startvex);
 }
 template <class T>
 void ALGraph<T> ::BFS(int v)
 {
	 int *queue=new int [Max];
	 int f = 0, r = 0;
	 for (int i = 0;i < Max;i++)
		 visited[i] = 0;
	 cout << adjlist[v].vertex;
	 visited[v] = 1;
	 queue[++r] = v;
	 while (f != r)
	 {
		 v = queue[++f];
		 ArcNode *p = adjlist[v].first;
		 while (p)
		 {
			 int j = p->adjvex;
			 if (visited[j] == 0)
			 {
				 cout <<"->"<<adjlist[j].vertex;
				 visited[j] = 1;
				 queue[++r] = j;
			 }
			 p = p->next;
		 }
	 }
 }
 template <class T>
 void ALGraph<T> ::bfs()
 {
	 cout << '\n' << "To use the BFS:";
	 cout << '\n' << "Please put in the starting vertex:	";
	 int startvex;
	 cin >> startvex;
	 BFS(startvex);
	 cout << endl;
 }
 template <class T> ALGraph<T>:: ~ALGraph()
 {
	 int i = 0;
	 while (i < vNum)
	 {
		 ArcNode *p = adjlist[i++].first;
		 while (p)
		 {
			 ArcNode *q = p->next;
			 delete p;
			 p = q;
		 }
	 }
	 delete[] adjvex;
	 adjvex = NULL;
	 delete[] visited;
	 visited = NULL;
	 delete[] lowcost;
	 lowcost = NULL;
	 delete[] Edgelist;
	 Edgelist = NULL;
 }
 template <class T>
 int ALGraph<T>::mininum(int lowcost[])
 {
	 int min,i=0;
	 while (i<vNum)
	 {
		 if (lowcost[i] >0)
		 {
			 min = lowcost[i];
			 break;
		 }
		 i++;
	 }
	 int k=i;
	 for (int i = 1;i <vNum;i++)
	 {
		 if (lowcost[i] >0 && lowcost[i] <min)
		 {
			 min = lowcost[i];
			 k = i;
		 }
	 }
	 return k;
 }
 template <class T> 
 void ALGraph<T>:: Prim()
 {
	 for (int i = 0;i < vNum;i++)			//先为数组赋好初值
	 {
		 lowcost[i] = -1;
		 adjvex[i] = 0;
	 }
	 cout<< "Prim(The order of adding adjvexes):" << endl;
	 ArcNode* temp = adjlist[0].first;
	 while(temp!=NULL)				//加进来第一个点
	 {
		 lowcost[temp->adjvex] =temp->weight ;
		 temp = temp->next;
	 }
	 lowcost[0] = 0;
	 cout << "The added adjvex：" << adjlist[0].vertex << endl;
	 for (int i = 1;i < vNum;i++)			//逐渐加入所有的点
	 {
		 int k = mininum(lowcost);
		 cout<<"The added adjvex："<< adjlist[k].vertex << endl;
		 lowcost[k] = 0;				//包含入k点
		 temp = adjlist[k].first;
		 while(temp)					//遍历完k点的所有邻接点
		 {
			 if ((temp->weight <lowcost[temp->adjvex]|| lowcost[temp->adjvex]==-1)&& lowcost[temp->adjvex] != 0)
			 {
				 lowcost[temp->adjvex] = temp->weight;				//如果这个点到集合的值有更小值 则刷新数组
				 temp->adjvex = k;
			 }
			 temp = temp->next;
		 }
	 }
 }
 template <class T>
 void ALGraph<T>::GenSortEdge()
 {
	 int j = 0,i;
	 for (i = 0;i < vNum;i++)
	 {
		 ArcNode *temp = adjlist[i].first;
		 while (temp)
		 {
			 Edgelist[j].start = i;
			 Edgelist[j].end = temp->adjvex;
			 Edgelist[j].weight = temp->weight;
			 temp = temp->next;
			 j++;
		 }
	 }
	 VEdge temp;
	 for (i = 1;i <arcNum;i++)			//用直接插入排序的方法进行调整
	 {
		 temp = Edgelist[i];
		 if (Edgelist[i].weight < Edgelist[i - 1].weight)
		 {
			 Edgelist[i] = Edgelist[i - 1];
			 int j = i - 1;
			 for (j;temp.weight < Edgelist[j].weight;j--)		//后移
				 Edgelist[j + 1] = Edgelist[j];
			 Edgelist[j + 1] = temp;
		 }
	 }
 }
 template <class T>
 void ALGraph<T>::Kruskal()
 {
	 GenSortEdge();				//为边排序
	 cout << '\n' << '\n' << "Kruskal:(The order of adding advexes)" << endl;
	 int* vest=new int [vNum];
	 int k = 0, j = 0,i;
	 for (i = 0;i < vNum;i++)			//初始化不同结合的数组  最开始各为一个集合
		 vest[i] = i;
	 while (k < vNum- 1)
	 {
		 int sn1 = vest[Edgelist[j].start];
		 int sn2 = vest[Edgelist[j].end];	
		 if (sn1 != sn2)				//若为两个集合  不连通
		 {
			 cout << adjlist[Edgelist[j].start].vertex << "->" << adjlist[Edgelist[j].end].vertex << "  weight: " << Edgelist[j].weight << endl;
			 k++;
			 for (i = 0;i < vNum;i++)
			 {
				 if (vest[i] == sn2)				//将加入点的弧 合并
					 vest[i] = sn1;
			 }
		 }
		 j++;
	 }
 }
 template <class T>
 int ALGraph<T>::GetMin(int* Length,bool* ifadded)
 {
	 int min=-1,i=0,k=0;
	 while (i<vNum)
	 {
		 if (ifadded[i])
		 {
			 min = Length[i];
			 break;
		 }
		 i++;
	 }
	 k = i;
	 for (i = 0;i < vNum;i++)
	 {
		 if (ifadded[i] && Length[i] < min)
		 {
			 min = Length[i];
			 k = i;
		 }
	 }
	 if (min==-1)
		 return -1;
	 else
		 return k;
 }
 template <class T>
 int ALGraph<T>::GetMin2(int* Length, bool* ifadded)
 {
	 std::priority_queue<shor, vector<shor>, cmp>a;			//建立优先队列  使时间复杂度变为logn
	 for (int i = 0;i < vNum;i++)
	 {
		 if (ifadded[i])			//如果没有访问过， 入队
		 {
			 shor temp;
			 temp.poi = i;
			 temp.weight = Length[i];
			 a.push(temp);
		 }
	 }
	 if (!a.empty())						//栈如果非空 就返回位置
		 return a.top().poi;
	 else
		 return -1;
 }
 template <class T>
 void ALGraph<T>::PrintPath(int * Length,int*Path,int origin,int end)
 {
	 if (end == origin)					//递归画图 如果起点等于终点 回溯
		 return;
	 else
	 {
		 end = Path[end];					//如果不是，到上一个顶点
		 PrintPath(Length, Path, origin, end);
		 cout<< adjlist[end].vertex << "->";
	 }
 }
 template <class T>
 void ALGraph<T>:: shortest()			//复杂度应该为nlogn 因为肯定得遍历一次  所以改进了寻找最小边
 {
	 int origin, i = 0,end;
	 cout << '\n' << "To get the shortest way:" << endl;
	 cout << "Please put in the started vertex and the end vertex you wanted:  " << endl;
	 cin >> origin>>end;
	 cout << "Please choose the version you wanted:" << endl;
	 cout << "You can compared the speed yourself!!(numeber 1 for old version    2 for improved version) :" << endl;
	 int way;
	 cin >> way;
	 QueryPerformanceCounter(&t3);					//程序计时
	 bool* ifadded = new bool[vNum];
	 int *Length = new int[vNum];
	 int* Path = new int[vNum];
	 for (i = 0;i < vNum;i++)								//初始化 长度为无限 未访问过
	 {
		 Length[i] = 999999;
		 ifadded[i] = true;
	}
	 ArcNode*temp = adjlist[origin].first;			//加入起始节点
	 while (temp)
	 {
		 Length[temp->adjvex] = temp->weight;			//遍历起始节点 加入最短路径 和路线
		 Path[temp->adjvex] = origin;
		 temp = temp->next;
	 }
	 ifadded[origin] = false;				//设置为访问过
	 int k = origin;
	 for (i = 0;i < vNum;i++)
	 {
		 if(way==1)
			k = GetMin(Length, ifadded);				//获取最近的点
		 else
			 k = GetMin2(Length, ifadded);
		 if (k == -1)
			 break;
		 temp = adjlist[k].first;
		 while (temp)							//用最近的点刷新最短路径和最近路线
		 {
			 if (temp->weight + Length[k] < Length[temp->adjvex])
			 {
				 Length[temp->adjvex] = temp->weight + Length[k];
				 Path[temp->adjvex] = k;
			 }
			 temp = temp->next;
		 }
		 ifadded[k] = false;
	 }
	 cout << '\n'<<'\n'<<"The shorted way is:" << endl;
	 PrintPath(Length, Path, origin, end);
	 cout << adjlist[end].vertex<<endl<<endl;		//下面的递归不方便输出终点
	 QueryPerformanceCounter(&t4);						//程序计时
	 dt = (t4.QuadPart - t3.QuadPart) / (long double)nFreq.QuadPart;
	 cout << '\n' << setprecision(12) << "The running time of your program:" << dt << "s" << endl;
 }
int main()
{
	QueryPerformanceFrequency(&nFreq);
	QueryPerformanceCounter(&t1);
	cout << " 888888ba  dP  a88888b. d888888P dP     dP  888888ba   88888888b" << endl;
	cout << " 88    `8b 88 d8'   `88    88    88     88  88    `8b  88        " << endl;
	cout << "a88aaaa8P' 88 88           88    88     88 a88aaaa8P' a88aaaa" << endl;
	cout << " 88        88 88           88    88     88  88   `8b.  88" << endl;
	cout << " 88        88 Y8.   .88    88    Y8.   .8P  88     88  88" << endl;
	cout << " dP        dP  Y88888P'    dP    `Y88888P'  dP     dP  88888888P" << endl;
	int a = 6, b = 18;
	char c[] = { 'A','B','C','D','E','F' };
	int d[][3] = { {0,1,34},{0,2,46},{0,5,19},{1,0,34},{1,4,12},{2,0,46},{2,3,17},{2,5,25} ,{3,2,17},{3,4,38},{3,5,25},{4,1,12},{4,3,38},{4,5,26},{5,0,19},{5,2,25},{5,3,25},{5,4,26} };
	int num;
	while (1)
	{
		cout << "		" << "1.Use the sample" << endl;
		cout << "		" << "2.Input your picture" << endl;
		cout << "		" << "3.Quit" << endl;
		cout << "		" << "Please input the numer before the conduction" << endl;
		cin >> num;
		if (num == 1)
		{
			ALGraph<int>A(a, b, c, d);
			A.dfs();
			A.bfs();
			A.shortest();
			A.Prim();
			A.Kruskal();
		}
		else if (num == 2)
		{
			ALGraph<int>B;
			B.dfs();
			B.bfs();
			B.shortest();
			B.Prim();
			B.Kruskal();
		}
		else if (num == 3)
			break;
		else
		{
			cout << "You put in the wrong number!" << endl;
		}
	}
	QueryPerformanceCounter(&t2);
	dt = (t2.QuadPart - t1.QuadPart) / (long double)nFreq.QuadPart;
	cout <<'\n'<<setprecision(12)<< "The running time of your program:" << dt<< "s" << endl;
	return 0;
}