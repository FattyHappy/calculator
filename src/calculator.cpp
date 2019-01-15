#include<bits/stdc++.h>
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
	char getData()
	{

	}
    void setData()
    {

    }
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
    decisionTree()//构造函数：随机生成一个判定树
	{
        register int point[operatorNum];
        for (register int i=0;i<operatorNum;++i)point[i]=i;
        for (register int i=0;i<10*operatorNum;++i)std::swap(point[rnd(10)],point[rnd(10)]);
		root=new treeNode[operatorNum];
        std::vector<int>nonEmptyPoint;
        nonEmptyPoint.push_back(point[0]);nonEmptyPoint.push_back(point[0]);
        for (register int i=1;i<operatorNum;i++)
		{
            register int tmpPos=rnd(nonEmptyPoint.size());
            register treeNode* tmpPoint=&root[nonEmptyPoint[tmpPos]];
            if(tmpPoint->lSon==NULL&&tmpPoint->rSon==NULL) (rnd(2)?tmpPoint->lSon:tmpPoint->rSon)=&root[point[i]];
            else (tmpPoint->lSon==NULL?tmpPoint->lSon:tmpPoint->rSon)=&root[point[i]];
			nonEmptyPoint.erase(nonEmptyPoint.begin()+tmpPos);
            nonEmptyPoint.push_back(point[i]);nonEmptyPoint.push_back(point[i]);
		}
		root=&root[point[0]];
		//=================手动栈模拟dfs=============================
		std::stack<treeNode*>nodeStack;
		std::stack<int>stat;
        nodeStack.push(root);stat.push(1);
        while (!nodeStack.empty())
		{
            if (stat.top()==1)
			{

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

				stat.pop();stat.push(2);
				if (nodeStack.top()->lSon!=NULL)
				{
					stat.push(1);
					nodeStack.push(nodeStack.top()->lSon);
					continue;
				}
			}
            if (stat.top()==2)
			{
                stat.pop();stat.push(3);
                if (nodeStack.top()->rSon!=NULL)
				{
					stat.push(1);
					nodeStack.push(nodeStack.top()->rSon);
					continue;
				}
			}
            if (stat.top()==3)
			{
				delete nodeStack.top();
				nodeStack.pop();
				stat.pop();
			}
		}
    }
	~decisionTree(){erase();}
    expr gen()//根据判定树生成对应计算表达式
    {
        expr tmpExpr=root->gen();
        while (tmpExpr==NUL)//出现除0的情况就是不合法 重造数据
		{
			erase();
			decisionTree();
			tmpExpr=root->gen();
		}
    	return root->gen();
    }
};

inline void init()
{
	priority['+']=priority['-']=1;
	priority['*']=priority['/']=2;
    srand(time(0)^19260817);
}
int main()
{
    init();
    decisionTree now;
    expr g=now.gen();
    std::cout<<g.exprString<<std::endl;
    printf("%f\n",g.ans);
}