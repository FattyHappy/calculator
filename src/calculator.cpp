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
		ll tmp=abs(p);
		if (!tmp)s="0";
        if (p<0)s+=')';
		while (tmp)
		{
			s=char('0'+tmp%10)+s;
			tmp/=10;
		}
        if (p<0)s="(-"+s;
	}else
	{
		ll tmp=q;
		while (tmp)s=char(tmp%10+'0')+s,tmp/=10;
		s='/'+s;
		tmp=abs(p);
		while (tmp)
		{
			s=char('0'+tmp%10)+s;
			tmp/=10;
		}
        if (p<0)s='-'+s;
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
    if (nodeOpr=='^'&&checkerType==2)exprAns.exprString+=std::string(" ** ");
    else exprAns.exprString+=std::string(" ")+nodeOpr+std::string(" ");
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
                i++;
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
	#undef rm
}
const static expr NUL("",data(0,1));
void decisionTree::init()
{
	register int point[maxOperatorNum];
	for (register int i=0;i<operatorNum;++i)point[i]=i;
	for (register int i=0;i<10*operatorNum;++i)std::swap(point[rnd(operatorNum)],point[rnd(operatorNum)]);
	treeNode** points=new treeNode*[maxOperatorNum];
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
inline void showHelp()
{
    puts("===========================introduction=============================");
	puts("Several formulas will be generated, including integers in the range [0,20) ");
	puts("and operators: addition[+]/subtraction[-]/multiplication[*]/division[/]/power[**|^].");
	puts("Brackets['(' and ')'] is also supported.");
    puts("Note that the number of operators cannot exceed 10.");
	puts("And the answer and intermediate calculation result won't be larger than 1e10 or smaller than 1e-8.");
    puts("There is no decimals or root symbols, such as \"0.123\" or \"√2\" [2^(0.5)].");
	puts("Give your answer in the format including no more than one \"+\" operator with integers and fractions.");
	puts("Such as 123/45, 1+78/45, 2+33/45.");
    puts("After the program exits, you can see the log file in folder \"./logs\".");
    puts("Start the program with \"start 10\" and enjoy!");
    puts("=============================commands===============================");
    puts("It's not case sensitive.");
    puts("enter \"exit\" or \"quit\" to force exit.");
    puts("enter \"setting\" to view settings in any time.");
	puts("To change power type, enter \"setpower\"+[number], e.g. \"setpower 1\".");
	puts("To change operation numbers, enter \"setopnum\"+[number], e.g. \"setopnum 5\".");
    puts("If you are ready, enter \"start\"+[number of formulas], e.g. \"start 10\".");
    puts("To submit an answer, enter \"answer\"+[your answer], e.g. \"answer 5/2\".");
    puts("To submit an custom formula and ask for an answer, enter \"calc\"+[your formula], e.g \"calc 2^(-3)**3\".");
}
inline void init()
{
    system("if not exist \".\\logs\" mkdir .\\logs");
    time_t __time=time(NULL);
    char addr[20]="gol.";int ptr=4;
    while (__time)addr[ptr++]=__time%10+'0',__time/=10;
    addr[ptr++]='\\';addr[ptr++]='s';addr[ptr++]='g';addr[ptr++]='o';addr[ptr++]='l';
    for (int i=0;i+i<ptr;i++)std::swap(addr[i],addr[ptr-i-1]);
    if (!refreshOut((const char*)addr))
	{
        puts("Failed to open log file!");
        system("pause");
        exit(0);
	}
    priority['+']=priority['-']=1;
	priority['*']=priority['/']=2;
    priority['^']=3;
    priority['(']=4;
    srand(time(0)^19260817);
    puts("Welcome to calculator project.");
    puts("enter \"help\" for more info.");
    puts("enter \"exit\" to force exit.");
    putchar('>');
}
inline std::pair<std::string,std::string>getinput()
{
    std::string s,operation,content;
    getline(std::cin,s);
    register int i,l=s.length();
    for (i=0;i<l;i++)
	{
        if (s[i]!=' '&&s[i]!='\t')
		{
			operation+=s[i]>='A'&&s[i]<='Z'?s[i]+32:s[i];
		}else
		{
            if (operation.length())break;
		}
	}
    for (i++;i<l;i++)content+=s[i];
    return make_pair(operation,content);
}
inline void setting()
{
    printf("power type = %d  ,  operator numbers = %d\n",checkerType,operatorNum);
    puts("power type is used for showing power operator '^' / \"**\".");
    puts("0: '^' or \"**\" is supported.");
    puts("1: '^' only.");
    puts("2:\"**\" only.");
    puts("To set a new power type, use \"setpower\"+[new number], e.g. \"setpower 2\".");
	puts("To set a new operation numbers, use \"setopnum\"+[new number], e.g. \"setopnum 2\".");
}
data getNext(decisionTree* T,int &now)
{
    T[now].erase();
    T[now].init();
	expr ans=T[now].gen();
	bool mrk=1;
	do
	{
		mrk=1;
		for (register int j=0;j<now;j++)
			if (T[j].root->same(T[now].root))
			{
				mrk=0;
				break;
			}
		if (mrk==0)
		{
			T[now].erase();
			T[now].init();
			ans=T[now].gen();
		}
	}while (mrk==0);
    printf("%d:\n",++now);
    printd(now);printc(':');printc('\n');
    std::cout<<ans.exprString<<std::endl;
    prints(ans.exprString);printc('\n');
    return ans.ans;
}
inline void deal()
{
	std::pair<std::string,std::string>wrk;
    decisionTree* T=new decisionTree[1005];
    int now=0,tot=0,AC=0;
    data nowAns(0,0);
    while (wrk=getinput(),true)
	{
        std::string operation=wrk.first,content=wrk.second;
        if (!operation.compare(std::string("exit"))||!operation.compare(std::string("quit")))
		{
            delete[] T;
            exit(0);
		}else if (!operation.compare(std::string("help")))
		{
			showHelp();
		}else if (!operation.compare(std::string("setting")))
		{
            setting();
		}else if (!operation.compare(std::string("setpower")))
		{
            int d=0,l=content.length();
            bool mrk=1;
            for (int i=0;i<l;i++)
			{
				if (content[i]<'0'||content[i]>'9')mrk=0;
				d=d*10+content[i]-'0';
			}
            mrk&=(d>=0&&d<=2);
            if (mrk)checkerType=d;else puts("invalid parameter!");
		}else if (!operation.compare(std::string("setopnum")))
		{
            int d=0,l=content.length();
            bool mrk=1;
            for (int i=0;i<l;i++)
			{
				if (content[i]<'0'||content[i]>'9')mrk=0;
				d=d*10+content[i]-'0';
			}
            mrk&=(d>=1&&d<=10);
            if (mrk)operatorNum=d;else puts("invalid parameter!");
		}else if (!operation.compare(std::string("calc")))
		{
            expr S(content);
            S.solve();
            if (S.ans==data(0,0))puts("Invalid formula! Check if your input.");
            else std::cout<<"Your answer equals "<<S.ans.toString()<<"  ."<<std::endl;
		}else if (!operation.compare(std::string("start")))
		{
			int d=0,l=content.length();
            bool mrk=1;
            for (int i=0;i<l;i++)
			{
				if (content[i]<'0'||content[i]>'9')mrk=0;
				d=d*10+content[i]-'0';
			}
            mrk&=(d>=1&&d<=1000);
            if (mrk){tot=d;nowAns=getNext(T,now);}else puts("invalid parameter!");
		}else if (!operation.compare(std::string("answer")))
		{
            int l=content.length();
            bool mrk=1;int sumPlus=0,sumDiv=0,sumSub=0;
            for (int i=0;i<l;i++)
			{
                if (content[i]=='*'||content[i]=='^'||content[i]=='('||content[i]==')')mrk=0;
                else if (content[i]=='+')
				{
					sumPlus++;
					if (sumPlus>1)mrk=0;
				}else if (content[i]=='-')
				{
                    sumSub++;
                    if (sumSub>1)mrk=0;
				}else if (content[i]=='/')
				{
					sumDiv++;
					if (sumDiv>2)mrk=0;
				}else if ((content[i]<'0'||content[i]>'9')&&content[i]!=' '&&content[i]!='\t')mrk=0;
			}
			prints(std::string("Answer: "+nowAns.toString()+"\n"));
            prints(std::string("Your answer: "));
            if (!mrk)
			{
				puts("Not a valid answer.");
                prints(std::string("invalid and not judged\n"));
                prints(std::string("Incorrect\n"));
			}
            else
			{
                expr S(content);
                if (S.solve(),S.ans==nowAns)
				{
					puts("Congratulations! Your answer is correct!");
					prints(std::string(content+"\n"));
					prints(std::string("Correct\n"));
					AC++;
				}else
				{
					puts("Ooops, your answer is incorrect.");
					prints(std::string(content+"\n"));
					prints(std::string("Incorrect\n"));
				}
			}
			if (now==tot)break;
            nowAns=getNext(T,now);
		}else puts("unknown command.");
		putchar('>');
	}
	delete[] T;
    printf("Totally %d problems. You solved %d of them.\n",tot,AC);
    printf("Correct rate = %.2f%\n",100.0*AC/tot);
    prints(std::string("\nend\n"));
    printd(AC);printc('/');printd(tot);
    system("pause");
}
int main()
{
    init();
    deal();
}
