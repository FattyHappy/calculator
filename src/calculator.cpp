#include<bits/stdc++.h>
#include"lib/fastIO.hpp"
using namespace fastIO;
typedef long long ll;
const static double eps=1e-8;
const static int operatorNum=10;
const static char operators[5]={'+','-','*','/','^'};
std::map<char,int>priority;
inline double sgn(double x){return x<-eps?-1:(x>eps?1:0);}
inline int rnd(int mod){return((ll)rand()<<32^(ll)rand()<<16^rand())%mod;}
class expr{
public:
    std::string exprString;
    double ans;
    expr(){ans=0;}
    expr(std::string S,double d=0):exprString(S),ans(d){}
    bool operator==(const expr &t)const{return !exprString.compare(t.exprString)&&!sgn(ans-t.ans);}
    ~expr(){}
};
const static expr NUL("",0);
class treeNode{
public:
    treeNode* lSon,*rSon;
    treeNode(){lSon=rSon=NULL;}
    ~treeNode(){}
    char nodeData;
    expr gen()
    {
        if (nodeData>='0'&&nodeData<='9')return expr(std::string(&nodeData,1),nodeData-'0');
        expr lExpr=lSon->gen(),rExpr=rSon->gen();
        if (nodeData=='/'&&sgn(rExpr.ans)==0||lExpr==NUL||rExpr==NUL)return NUL;
    	expr exprAns;
    	if (lSon->nodeData>='0'&&lSon->nodeData<='9')exprAns.exprString+=lExpr.exprString;
        else if (priority[lSon->nodeData]<priority[nodeData])exprAns.exprString+=std::string("(")+lExpr.exprString+std::string(")");
        else exprAns.exprString+=lExpr.exprString;
        exprAns.exprString+=std::string(" ")+nodeData+std::string(" ");
    	if (rSon->nodeData>='0'&&rSon->nodeData<='9')exprAns.exprString+=rExpr.exprString;
        else if (priority[rSon->nodeData]<priority[nodeData])exprAns.exprString+=std::string("(")+rExpr.exprString+std::string(")");
        else if (priority[rSon->nodeData]==priority[nodeData]&&(nodeData=='-'||nodeData=='/'))exprAns.exprString+=std::string("(")+rExpr.exprString+std::string(")");
        else exprAns.exprString+=rExpr.exprString;
		if (nodeData=='+')exprAns.ans=lExpr.ans+rExpr.ans;
		if (nodeData=='-')exprAns.ans=lExpr.ans-rExpr.ans;
		if (nodeData=='*')exprAns.ans=lExpr.ans*rExpr.ans;
		if (nodeData=='/')exprAns.ans=lExpr.ans/rExpr.ans;
        return exprAns;
    }
};
class decisionTree{
public:
    treeNode* root;
    inline void init()//构造函数：随机生成一个判定树
	{
        register int point[operatorNum];
        for (register int i=0;i<operatorNum;++i)point[i]=i;
        for (register int i=0;i<10*operatorNum;++i)std::swap(point[rnd(10)],point[rnd(10)]);
		treeNode** points=new treeNode*[operatorNum];
		for (register int i=0;i<operatorNum;i++)points[i]=new treeNode;
        std::vector<int>nonEmptyPoint;
        nonEmptyPoint.push_back(point[0]);nonEmptyPoint.push_back(point[0]);
        for (register int i=1;i<operatorNum;i++)
		{
            register int tmpPos=rnd(nonEmptyPoint.size());
            register treeNode* tmpPoint=points[nonEmptyPoint[tmpPos]];
            if(tmpPoint->lSon==NULL&&tmpPoint->rSon==NULL) (rnd(2)?tmpPoint->lSon:tmpPoint->rSon)=points[point[i]];
            else (tmpPoint->lSon==NULL?tmpPoint->lSon:tmpPoint->rSon)=points[point[i]];
			nonEmptyPoint.erase(nonEmptyPoint.begin()+tmpPos);
            nonEmptyPoint.push_back(point[i]);nonEmptyPoint.push_back(point[i]);
		}
		root=points[point[0]];
        delete[] points;
		//=================手动栈模拟dfs=============================
		std::stack<treeNode*>nodeStack;
		std::stack<int>stat;
        nodeStack.push(root);stat.push(1);
        while (!nodeStack.empty())
		{
            //printf("%d %d %d\n",nodeStack.top(),nodeStack.top()->lSon,nodeStack.top()->rSon);
            if (stat.top()==1)
			{
				//puts("init 1");
				stat.pop();stat.push(2);
				if (nodeStack.top()->lSon!=NULL)
				{
					stat.push(1);
					nodeStack.push(nodeStack.top()->lSon);
					continue;
				}else
				{
					nodeStack.top()->lSon=new treeNode;
					nodeStack.top()->lSon->nodeData='0'+rnd(10);
				}
			}
            if (stat.top()==2)
			{
                //puts("init 2");
                stat.pop();stat.push(3);
                if (nodeStack.top()->rSon!=NULL)
				{
					stat.push(1);
					nodeStack.push(nodeStack.top()->rSon);
					continue;
				}else
				{
					nodeStack.top()->rSon=new treeNode;
					nodeStack.top()->rSon->nodeData='0'+rnd(10);
				}
			}
            if (stat.top()==3)
			{
				//puts("init 3");
                nodeStack.top()->nodeData=operators[rnd(4)];
				nodeStack.pop();
				stat.pop();
			}
		}
	}
    inline void erase()//销毁申请的空间
    {
		//=================手动栈模拟dfs=============================
        std::stack<treeNode*>nodeStack;
		std::stack<int>stat;
        nodeStack.push(root);stat.push(1);
        while (!nodeStack.empty())
		{
            if (stat.top()==1)
			{
                //puts("solve 1");
				stat.pop();stat.push(2);
				if (nodeStack.top()->lSon!=NULL)
				{
					nodeStack.push(nodeStack.top()->lSon);
					stat.push(1);
					continue;
				}
			}
            if (stat.top()==2)
			{
                //puts("solve 2");
                stat.pop();stat.push(3);
                if (nodeStack.top()->rSon!=NULL)
				{
					nodeStack.push(nodeStack.top()->rSon);
					stat.push(1);
					continue;
				}
			}
            if (stat.top()==3)
			{
                treeNode* tp=nodeStack.top();
				nodeStack.pop();
				stat.pop();
				delete tp;
			}
		}
    }
    decisionTree(){init();}
	~decisionTree(){erase();}
    expr gen()//根据判定树生成对应计算表达式
    {
        expr tmpExpr=root->gen();
        while (tmpExpr==NUL)//出现除0的情况就是不合法 重造数据
		{
			erase();
			init();
			tmpExpr=root->gen();
		}
    	return root->gen();
    }
};
int num;
inline void init(int argc,char** argv)
{
	assert(argc==3);
    assert(refreshOut((const char*)argv[1]));
    int len = int(strlen(argv[2]));
    num = 0;
	for (int i = 0; i < len; i++)
	{
		assert(argv[2][i]>='0'&&argv[2][i]<='9');
		num = num * 10 + argv[2][i] - '0';
	}
    assert(num>=1&&num<=1000);
	priority['+']=priority['-']=1;
	priority['*']=priority['/']=2;
    srand(time(0)^19260817);
}
int main(int argc,char** argv)
{
    init(argc,argv);
    for (int T=1;T<=num;T++)
	{
        decisionTree now;
        expr ans=now.gen();
		for (int i=0;i<ans.exprString.length();i++)print(ans.exprString[i]);
        print('\n');
        //for (int i=0;i<ans.exprString.length();i++)print(ans.exprString[i]);
	}
}