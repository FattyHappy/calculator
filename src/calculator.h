#ifndef __calculator_h__
#define __calculator_h__

const static double eps=1e-8;
const static int operatorNum=10;
const static char operators[7]={'+','-','*','/','^','(',')'};
static int checkerType=0;
/*  checkerType is used for showing '^' / "**"
 *	0: '^' or "**" is supported
 *	1: '^' only
 *	2:"**" only
 *	 '^' = checkerType<2
 *	"**" = checkerType%2==0
 */
static int num=0; //numbers of equations
std::map<char,int>priority;
typedef long long ll;
inline int rnd(int mod){return((ll)rand()<<32^(ll)rand()<<16^rand())%mod;}
inline int getOp(){int x=rnd(5);if (x==4)x=rnd(5);return x;}
inline double sgn(double x){return x<-eps?-1:(x>eps?1:0);}
class data{
/*	p=0, q=0 : nan, Not A Number
 *  p!=0,q=0 : invalid
 *	     q!=0: p/q
 */
public:
    ll p,q;
	data():p(0),q(1){}
	data(ll _p,ll _q);
	~data(){}
    void refresh();
    bool operator ==(const data &b)const
    {
		register bool tmp1=p==0&&q==0,tmp2=b.p==0&&b.q==0;
		if (tmp1||tmp2)return tmp1&&tmp2;
		return p==b.p&&q==b.q;
	}
    data operator +(const data &b)const
	{
		if (q&&b.q)
		{
			data ans(p*b.q+q*b.p,q*b.q);
			ans.refresh();
			return ans;
		}else return data(0,0);
	}
    data operator -(const data &b)const
	{
		if (q&&b.q)
		{
			data ans(p*b.q-q*b.p,q*b.q);
			ans.refresh();
			return ans;
		}else return data(0,0);
	}
    data operator *(const data &b)const
	{
		if (q&&b.q)
		{
			data ans(p*b.p,q*b.q);
			ans.refresh();
			return ans;
		}else return data(0,0);
	}
    data operator /(const data &b)const
	{
		if (q&&b.q)
		{
			data ans(p*b.q,q*b.p);
			ans.refresh();
			return ans;
		}else return data(0,0);
	}
    data operator ^(const data &b)const
	{
		if (q&&b.q)
		{
			if (b.q==1&&abs(b.p)*log(abs(q))<log(1e10)&&abs(b.p)*log(abs(p))<log(1e10))
			{
				data ans(1,1);
				ll mp=p,mq=q;if (b.p<0)std::swap(mp,mq);
				for (int j=1;j<=abs(b.p);j++)ans.p*=mp,ans.q*=mq;
				ans.refresh();
				return ans;
			}else return data(0,0);
		}else return data(0,0);
	}
    std::string toString();
};
class expr{
public:
	std::string exprString;
    data ans;
    expr(){}
    expr(std::string S,data d=data(0,0)):exprString(S),ans(d){}
    ~expr(){}
    bool operator==(const expr &t)const{return !exprString.compare(t.exprString)&&ans==t.ans;}
    void solve();
};
class treeNode{
public:
	treeNode* lSon,*rSon;
    char nodeOpr;
    data nodeData;
    int type;//type=1 : operators type=2 : data
	treeNode():lSon(NULL),rSon(NULL),nodeOpr(operators[getOp()]),type(1){}
	treeNode(char ch):lSon(NULL),rSon(NULL),nodeOpr(ch),type(1){}
	treeNode(data da):lSon(NULL),rSon(NULL),nodeData(da),type(2){}
    ~treeNode(){}
	expr gen();
	bool same(treeNode* x);
};
class decisionTree{
public:
	treeNode* root=NULL;
	inline void init();//随机生成一个判定树
	inline void erase();//销毁申请的空间
	decisionTree(){init();}
	~decisionTree(){erase();}
	expr gen();//根据判定树生成对应计算表达式
};
inline void init(int argc,char** argv);
#endif // __calculator__