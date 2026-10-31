#include <bits/stdc++.h>
using namespace std;
#define rep(i,a,n) for (int i=a;i<n;i++)
#define per(i,a,n) for (int i=n-1;i>=a;i--)
#define pb push_back
#define mp make_pair
#define all(x) (x).begin(),(x).end()
#define fi first
#define se second
#define SZ(x) ((int)(x).size())
typedef vector<int> VI;
typedef long long ll;
typedef pair<int,int> PII;
typedef double dbl;
mt19937 mrand(random_device{}()); 
const ll mod=1000000007;
int rnd(int x) { return mrand() % x;}
ll powmod(ll a,ll b) {ll res=1;a%=mod; assert(b>=0); for(;b;b>>=1){if(b&1)res=res*a%mod;a=a*a%mod;}return res;}
ll gcd(ll a,ll b) { return b?gcd(b,a%b):a;}
// head

typedef long long db;
const db EPS = 0;
  
inline int sign(db a) { return a < -EPS ? -1 : a > EPS; }
  
inline int cmp(db a, db b){ return sign(a-b); }
  
struct P {
	db x, y;
	P() {}
	P(db _x, db _y) : x(_x), y(_y) {}
	P operator+(P p) { return {x + p.x, y + p.y}; }
	P operator-(P p) { return {x - p.x, y - p.y}; }
	P operator*(db d) { return {x * d, y * d}; }
	P operator/(db d) { return {x / d, y / d}; }
 
	bool operator<(P p) const { 
		int c = cmp(x, p.x);
		if (c) return c == -1;
		return cmp(y, p.y) == -1;
	}
 
	bool operator==(P o) const{
		return cmp(x,o.x) == 0 && cmp(y,o.y) == 0;
	}
 	dbl distTo(P p) { return (*this-p).abs(); }
	dbl abs() { return sqrt(abs2());}

	db dot(P p) { return x * p.x + y * p.y; }
	db det(P p) { return x * p.y - y * p.x; }
	 	void read() { cin>>x>>y; }
	void write() {cout<<"("<<x<<","<<y<<")"<<endl;}
	db abs2() { return x * x + y * y; }
	P rot90() { return P(-y,x);}
	int quad() const { return sign(y) == 1 || (sign(y) == 0 && sign(x) >= 0); }
};
  
#define cross(p1,p2,p3) ((p2.x-p1.x)*(p3.y-p1.y)-(p3.x-p1.x)*(p2.y-p1.y))
#define crossOp(p1,p2,p3) sign(cross(p1,p2,p3))
  
bool chkLL(P p1, P p2, P q1, P q2) {
	db a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2);
	return sign(a1+a2) != 0;
}
 
P isLL(P p1, P p2, P q1, P q2) {
	db a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2);
	return (p1 * a2 + p2 * a1) / (a1 + a2);
}
    
bool intersect(db l1,db r1,db l2,db r2){
	if(l1>r1) swap(l1,r1); if(l2>r2) swap(l2,r2); 
	return !( cmp(r1,l2) == -1 || cmp(r2,l1) == -1 );
}
  
bool isSS(P p1, P p2, P q1, P q2){
	return intersect(p1.x,p2.x,q1.x,q2.x) && intersect(p1.y,p2.y,q1.y,q2.y) && 
	crossOp(p1,p2,q1) * crossOp(p1,p2,q2) <= 0 && crossOp(q1,q2,p1)
			* crossOp(q1,q2,p2) <= 0;
}

const int N=10100;
int n,L,q;
P p[N];
dbl md[N];

int main() {
	scanf("%d%d%d",&n,&L,&q);
	P s=P(0,0),t=P(L,0);
	rep(i,1,n+1) {
		int x,y;
		scanf("%d%d",&x,&y);
		p[i]=P(x,y);
	}
	for (int i=1;i<=n;i++) {
		if (p[i].y>0) {
			md[i]=p[i].distTo(t);
			for (int j=1;j<=n;j++) if (j!=i&&crossOp(p[i],s,p[j])==1&&crossOp(p[i],t,p[j])==1) {
				md[i]=max(md[i],p[i].distTo(p[j])+p[j].distTo(t));
			}
		} else {
			md[i]=p[i].distTo(t);
			for (int j=1;j<=n;j++) if (j!=i&&crossOp(p[i],s,p[j])==-1&&crossOp(p[i],t,p[j])==-1) {
				md[i]=max(md[i],p[i].distTo(p[j])+p[j].distTo(t));
			}
		}
		md[i]+=p[i].distTo(s);
	}
	rep(i,0,q) {
		int u,v;
		scanf("%d%d",&u,&v);
		if (isSS(s,t,p[u],p[v])) {
			dbl f1=min(s.distTo(p[u])+t.distTo(p[v]),s.distTo(p[v])+t.distTo(p[u]))+p[u].distTo(p[v]);
			printf("%.10f\n",min(min(md[u],md[v]),f1));
		} else printf("%d\n",L);
	}
}
