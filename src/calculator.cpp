#include <bits/stdc++.h>
#include "calculator.h"
#include "lib/fastIO.hpp"
using namespace fastIO;
data::data(ll _p,ll _q)
{
	p=_p;q=_q;
	if (q==0)p=0;
	else refresh();
}
void data::refresh()
{
	if (!q){p=0;return;}
	ll _t=std::__gcd(p,q);
	p/=_t;q/=_t;
	if (q<0)p=-p,q=-q;
	if (abs(p)>1e10||abs(q)>1e10)p=0,q=0;
}
std::string data::toString()
{
	std::string s="";
	if (q==1)
	{
		ll tmp=p;
		if (!tmp)s="0";
		while (tmp)
		{
			s=char('0'+tmp%10)+s;
			tmp/=10;
		}
	}
	return s;
}
expr treeNode::gen()
{
	if (type==2)return expr(nodeData.toString(),nodeData);
	expr lExpr=lSon->gen(),rExpr=rSon->gen();
	expr exprAns;
	if (lSon->type==2)exprAns.exprString+=lExpr.exprString;
	else if (priority[lSon->nodeOpr]<priority[nodeOpr])exprAns.exprString+=std::string("(")+lExpr.exprString+std::string(")");
	else if (priority[lSon->nodeOpr]==priority[nodeOpr]&&nodeOpr=='^')exprAns.exprString+=std::string("(")+lExpr.exprString+std::string(")");
	else exprAns.exprString+=lExpr.exprString;
	exprAns.exprString+=std::string(" ")+nodeOpr+std::string(" ");
	if (rSon->type==2)exprAns.exprString+=rExpr.exprString;
	else if (priority[rSon->nodeOpr]<priority[nodeOpr])exprAns.exprString+=std::string("(")+rExpr.exprString+std::string(")");
	else if (priority[rSon->nodeOpr]==priority[nodeOpr]&&(nodeOpr=='-'||nodeOpr=='/'))exprAns.exprString+=std::string("(")+rExpr.exprString+std::string(")");
	else exprAns.exprString+=rExpr.exprString;
	if (nodeOpr=='+')exprAns.ans=lExpr.ans+rExpr.ans;
	if (nodeOpr=='-')exprAns.ans=lExpr.ans-rExpr.ans;
	if (nodeOpr=='*')exprAns.ans=lExpr.ans*rExpr.ans;
	if (nodeOpr=='/')exprAns.ans=lExpr.ans/rExpr.ans;
	if (nodeOpr=='^')exprAns.ans=lExpr.ans^rExpr.ans;
	exprAns.ans.refresh();
	return exprAns;
}
bool treeNode::same(treeNode* x)
{
	int s1=(lSon!=NULL)+(rSon!=NULL),s2=(x->lSon!=NULL)+(x->rSon!=NULL);
	if (s1!=s2)return false;
	if (s1==0)//s1==s2==0
	{
		if (type!=x->type)return false;
		return type==1?(nodeOpr==x->nodeOpr):(nodeData==x->nodeData);
	}else if (s1==1)//s1==s2==1
	{
		treeNode* son1=(lSon==NULL)?rSon:lSon,*son2=((x->lSon)==NULL)?(x->rSon):(x->lSon);
		return son1->same(son2);
	}else //s1==s2==2
	{
		return (lSon->same(x->lSon)&&rSon->same(x->rSon))
			 ||(lSon->same(x->rSon)&&rSon->same(x->lSon));
	}
}
void expr::solve()
{
	/*给定式子，计算它的答案
	 *在生成后缀表达式的同时就建出判定树
	 *把字符栈S2和数字栈S1替换成两个节点栈，然后和后缀表达式求法一样处理
	 *退栈时把S2.top的左右儿子设置为S1的前两个，并把S2.top()扔到S1中视为数字
	 *最后S1剩下的唯一一个节点就是根，否则一定是异常
	 *S2应当维护成一个优先级单调不减的栈，但考虑到乘方计算顺序从右往左，那么乘方采用单调递增维护方式。
	 */
	ans=data(0,0);
	std::stack<treeNode*>S1;
	std::stack<treeNode*>S2;
    if (exprString.length()&&exprString[0]=='-')exprString='0'+exprString;
	#define rm(S) {while (!(S).empty()){delete (S).top();(S).pop();}}
	int l=exprString.length();
	for (int i=0;i<l;i++)
	{
		if (exprString[i]==' '||exprString[i]=='\t')continue;
		if (exprString[i]>='0'&&exprString[i]<='9')
		{
			ll d=0;
			while (i<l&&exprString[i]>='0'&&exprString[i]<='9')d=d*10+exprString[i++]-'0';
			i--;
			S1.push(new treeNode(data(d,1)));
			continue;
		}else if (exprString[i]=='(')
		{
			S2.push(new treeNode('('));
			continue;
		}else if (exprString[i]==')')
		{
			while (!S2.empty()&&S2.top()->nodeOpr!='(')
			{
				if (S1.empty()){rm(S2);return;}
				treeNode* t1=S1.top();S1.pop();
				if (S1.empty()){rm(S2);return;}
				treeNode* t2=S1.top();S1.pop();
				if (S2.empty()){rm(S1);return;}
				treeNode* op=S2.top();S2.pop();
				op->lSon=t2;op->rSon=t1;
				S1.push(op);
			}
			if (S2.empty()||S2.top()->nodeOpr!='('){rm(S1);rm(S2);return;}
			delete S2.top();S2.pop();
			continue;
		}else if (exprString[i]=='/')
		{
			while (!S2.empty()&&S2.top()->nodeOpr!='('&&priority[S2.top()->nodeOpr]>=priority['/'])
			{
				if (S1.empty()){rm(S2);return;}
				treeNode* t1=S1.top();S1.pop();
				if (S1.empty()){rm(S2);return;}
				treeNode* t2=S1.top();S1.pop();
				if (S2.empty()){rm(S1);return;}
				treeNode* op=S2.top();S2.pop();
				op->lSon=t2;op->rSon=t1;
				S1.push(op);
			}
			S2.push(new treeNode('/'));
			continue;
		}else if (exprString[i]=='*')
		{
			if (i+1<l&&exprString[i+1]=='*'&&checkerType%2==0)
			{
				while (!S2.empty()&&S2.top()->nodeOpr!='('&&priority[S2.top()->nodeOpr]>priority['^'])
				{
					if (S1.empty()){rm(S2);return;}
					treeNode* t1=S1.top();S1.pop();
					if (S1.empty()){rm(S2);return;}
					treeNode* t2=S1.top();S1.pop();
					if (S2.empty()){rm(S1);return;}
					treeNode* op=S2.top();S2.pop();
					op->lSon=t2;op->rSon=t1;
					S1.push(op);
				}
				S2.push(new treeNode('^'));
				continue;
			}else
			{
				while (!S2.empty()&&S2.top()->nodeOpr!='('&&priority[S2.top()->nodeOpr]>=priority['*'])
				{
					if (S1.empty()){rm(S2);return;}
					treeNode* t1=S1.top();S1.pop();
					if (S1.empty()){rm(S2);return;}
					treeNode* t2=S1.top();S1.pop();
					if (S2.empty()){rm(S1);return;}
					treeNode* op=S2.top();S2.pop();
					op->lSon=t2;op->rSon=t1;
					S1.push(op);
				}
				S2.push(new treeNode('*'));
				continue;
			}
		}else if (exprString[i]=='^')
		{
            if (checkerType==2){rm(S1);rm(S2);return;}
			while (!S2.empty()&&S2.top()->nodeOpr!='('&&priority[S2.top()->nodeOpr]>priority['^'])
			{
				if (S1.empty()){rm(S2);return;}
				treeNode* t1=S1.top();S1.pop();
				if (S1.empty()){rm(S2);return;}
				treeNode* t2=S1.top();S1.pop();
				if (S2.empty()){rm(S1);return;}
				treeNode* op=S2.top();S2.pop();
				op->lSon=t2;op->rSon=t1;
				S1.push(op);
			}
			S2.push(new treeNode('^'));
			continue;
		}else if (exprString[i]=='+')
		{
			while (!S2.empty()&&S2.top()->nodeOpr!='('&&priority[S2.top()->nodeOpr]>=priority['+'])
			{
				if (S1.empty()){rm(S2);return;}
				treeNode* t1=S1.top();S1.pop();
				if (S1.empty()){rm(S2);return;}
				treeNode* t2=S1.top();S1.pop();
				if (S2.empty()){rm(S1);return;}
				treeNode* op=S2.top();S2.pop();
				op->lSon=t2;op->rSon=t1;
				S1.push(op);
			}
			S2.push(new treeNode('+'));
			continue;
		}else if (exprString[i]=='-')
		{
			if ((i!=0&&(exprString[i-1]<'0'||exprString[i-1]>'9')&&exprString[i-1]!=' '&&exprString[i-1]!='\t')||i==0)
			{
				i++;if (i>=l){rm(S1);rm(S2);return;}
				ll d=0;
				while (i<l&&exprString[i]>='0'&&exprString[i]<='9')d=d*10+exprString[i++]-'0';
				i--;
				S1.push(new treeNode(data(-d,1)));
			}else
			{
				while (!S2.empty()&&S2.top()->nodeOpr!='('&&priority[S2.top()->nodeOpr]>=priority['-'])
				{
					if (S1.empty()){rm(S2);return;}
					treeNode* t1=S1.top();S1.pop();
					if (S1.empty()){rm(S2);return;}
					treeNode* t2=S1.top();S1.pop();
					if (S2.empty()){rm(S1);return;}
					treeNode* op=S2.top();S2.pop();
					op->lSon=t2;op->rSon=t1;
					S1.push(op);
				}
				S2.push(new treeNode('-'));
			}
			continue;
		}else
		{
			rm(S1);rm(S2);
			return;
		}
	}
    while(!S2.empty())
	{
		if (S1.empty()){rm(S2);return;}
		treeNode* t1=S1.top();S1.pop();
		if (S1.empty()){rm(S2);return;}
		treeNode* t2=S1.top();S1.pop();
		if (S2.empty()){rm(S1);return;}
		treeNode* op=S2.top();S2.pop();
		if (op->nodeOpr=='(')return;
		op->lSon=t2;op->rSon=t1;
		S1.push(op);
	}
	if (S1.empty()){rm(S2);return;}
	decisionTree dT;
	dT.root=S1.top();S1.pop();
	if (!S1.empty()){rm(S1);rm(S2);return;}
	ans=dT.gen().ans;
    std::cout<<dT.gen().exprString<<std::endl;
	#undef rm
}
const static expr NUL("",data(0,1));
void decisionTree::init()
{
	register int point[operatorNum];
	for (register int i=0;i<operatorNum;++i)point[i]=i;
	for (register int i=0;i<10*operatorNum;++i)std::swap(point[rnd(operatorNum)],point[rnd(operatorNum)]);
	treeNode** points=new treeNode*[operatorNum];
	for (register int i=0;i<operatorNum;i++)points[i]=new treeNode();
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
		if (stat.top()==1)
		{
			stat.pop();stat.push(2);
			if (nodeStack.top()->lSon!=NULL)
			{
				stat.push(1);
				nodeStack.push(nodeStack.top()->lSon);
				continue;
			}else nodeStack.top()->lSon=new treeNode(data(rnd(20),1));
		}
		if (stat.top()==2)
		{
			stat.pop();stat.push(3);
			if (nodeStack.top()->rSon!=NULL)
			{
				stat.push(1);
				nodeStack.push(nodeStack.top()->rSon);
				continue;
			}else nodeStack.top()->rSon=new treeNode(data(rnd(20),1));
		}
		if (stat.top()==3)
		{
			nodeStack.pop();
			stat.pop();
		}
	}
}
void decisionTree::erase()
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
				nodeStack.push(nodeStack.top()->lSon);
				stat.push(1);
				continue;
			}
		}
		if (stat.top()==2)
		{
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
expr decisionTree::gen()
{
	expr tmpExpr=root->gen();
	while (tmpExpr.ans==data(0,0))
	{
		erase();
		init();
		tmpExpr=root->gen();
	}
	return root->gen();
}
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
    priority['^']=3;
    priority['(']=4;
    srand(time(0)^19260817);
}
int main(int argc,char** argv)
{
    init(argc,argv);
    decisionTree* T=new decisionTree[1005];
    for (register int i=0;i<num;i++)
	{
        expr ans=T[i].gen();
        bool mrk=1;
        do
		{
            mrk=1;
			for (register int j=0;j<i;j++)
				if (T[j].root->same(T[i].root))
				{
					mrk=0;
					break;
				}
			if (mrk==0)
			{
                T[i].erase();
                T[i].init();
				ans=T[i].gen();
			}
		}while (mrk==0);
		register int l=ans.exprString.length();
		for (register int j=0;j<l;j++)print(ans.exprString[j]);
        print('\n');
        std::cout<<ans.exprString<<std::endl;
        std::cout<<ans.ans.p<<'/'<<ans.ans.q<<' '<<std::endl;
        for (int i=0;i<ans.exprString.length();i++)print(ans.exprString[i]);
	}
	delete[] T;
    puts("test?[y/n]");
    char tep=getchar();while (tep!='y'&&tep!='n'&&tep!='Y'&&tep!='N')tep=getchar();
    if (tep=='N'||tep=='n')return 0;
    getchar();
	expr S;
    while (std::getline(std::cin,S.exprString))
	{
		S.solve();
        std::cout<<S.ans.p<<'/'<<S.ans.q<<std::endl;
	}
}
