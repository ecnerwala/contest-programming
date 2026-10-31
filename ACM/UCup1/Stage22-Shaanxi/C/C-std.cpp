#include <iostream>
#include <algorithm>
#include<cmath>
#include<cstring>
#include<cstdio>
#include<cstdlib>
#include<vector>
#include<iomanip>
#include<ctime>
#include<set>
#include<map>
#include<queue>
#include<stack>
#include<bitset>
#include<cassert>
#define sqr(x) ((x)*(x))
#define fz1(i,n) for ((i)=1;(i)<=(n);(i)++)
#define fd1(i,n) for ((i)=(n);(i)>=1;(i)--)
#define fz0g(i,n) for ((i)=0;(i)<=(n);(i)++)
#define fd0g(i,n) for ((i)=(n);(i)>=0;(i)--)
#define fz0k(i,n) for ((i)=0;(i)<(n);(i)++)
#define fd0k(i,n) for ((i)=(((long long)(n))-1);(i)>=0;(i)--)
#define fz(i,x,y) for ((i)=(x);(i)<=(y);(i)++)
#define fd(i,y,x) for ((i)=(y);(i)>=(x);(i)--)
#define fzin fz1(i,n)
#define fzim fz1(i,m)
#define fzjn fz1(j,n)
#define fzjm fz1(j,m)
#define ff(c,itr) for (__typeof((c).begin()) itr=(c).begin();itr!=(c).end();++itr)
#define pb push_back
#define mk make_pair
#define rdst(st,len){static char ss[len];scanf(" %s",ss);(st)=ss;}
#define spln(i,n) (i==n?'\n':' ')
#define fac_init(n){fac[0]=fac[1]=inv[1]=fi[0]=fi[1]=1;fz(i,2,n){fac[i]=1ll*fac[i-1]*i%mod;inv[i]=1ll*(mod-mod/i)*inv[mod%i]%mod;fi[i]=1ll*fi[i-1]*inv[i]%mod;}}
using namespace std;
typedef long long i64;
typedef long double f80;
typedef unsigned int u32;
typedef unsigned long long u64;
inline void read(int &x)
{
	char c;int f=1;
	while(!isdigit(c=getchar()))if(c=='-')f=-1;
	x=(c&15);while(isdigit(c=getchar()))x=(x<<1)+(x<<3)+(c&15);
	x*=f;
}
int n,q,i,j;
struct edg{int p,w,c;};
vector<edg> bi[200005];
int fa[200005],fc[200005],fw[200005];
int bel[200005],lst[200005];
int sz[200005],tp[200005],son[200005],dep[200005];
i64 dis[200005];
int dfn[200005],ti,mp[200005];
int lg[200005],f[19][200005];
void dfs1(int x,int p)
{
	fa[x]=p;sz[x]=1;
	for(auto e:bi[x]){
		if(e.p==p) continue;
		dep[e.p]=dep[x]+1;
		dis[e.p]=dis[x]+e.w;
		fc[e.p]=e.c;
		dfs1(e.p,x);
		sz[x]+=sz[e.p];
		if(sz[son[x]]<sz[e.p]) son[x]=e.p;
	}
}
void dfs2(int x,int t)
{
	tp[x]=t;mp[dfn[x]=++ti]=x;
	f[0][ti]=dfn[fa[x]];
	if(son[x]) dfs2(son[x],t);
	for(auto e:bi[x]){
		if(e.p==fa[x]||e.p==son[x]) continue;
		dfs2(e.p,e.p);
	}
}
int lca(int x,int y)
{
	if(x==y) return x;
	x=dfn[x];y=dfn[y];
	if(x>y) swap(x,y);
	int l=x+1,r=y;
	int t=lg[r-l+1];
	return mp[min(f[t][l],f[t][r-(1<<t)+1])];
}
int nxt(int x,int t)
{
	while(dfn[fa[tp[x]]]>dfn[t]) x=fa[tp[x]];
	if(fa[tp[x]]==t) return tp[x];
	return mp[dfn[t]+1];
}
i64 calc(int x,int y)
{
	return dis[x]+dis[y]-2*dis[lca(x,y)];
}
int tim[200005],col[200005];
vector<int> v[800005];
int vis[800005];
i64 ans[800005];
void update(int x,int l,int r,int ql,int qr,int c)
{
//	if(x==1) cerr<<ql<<' '<<qr<<' '<<c<<endl;
	vis[x]=1;
	if(ql<=l&&r<=qr){v[x].push_back(c);return;}
	int mid=(l+r)/2;
	if(ql<=mid) update(x*2,l,mid,ql,qr,c);
	if(qr>mid) update(x*2+1,mid+1,r,ql,qr,c);
}
struct node
{
	int a,b;i64 c;
};
node operator +(node x,node y)
{
	node t=x;i64 tmp;
	if(y.c>t.c) t=y;
	tmp=calc(x.a,y.a);if(tmp>t.c) t=(node){x.a,y.a,tmp};
	tmp=calc(x.a,y.b);if(tmp>t.c) t=(node){x.a,y.b,tmp};
	tmp=calc(x.b,y.a);if(tmp>t.c) t=(node){x.b,y.a,tmp};
	tmp=calc(x.b,y.b);if(tmp>t.c) t=(node){x.b,y.b,tmp};
	return t;
}
int *buf1[10000005],val1[10000005],tot1;
i64 *buf2[10000005],val2[10000005];int tot2;
node *buf3[1000005],val3[1000005];int tot3;
void upd(int &x,int y)
{
	buf1[++tot1]=&x;val1[tot1]=x;
	x=y;
}
void upd(i64 &x,i64 y)
{
	buf2[++tot2]=&x;val2[tot2]=x;
	x=y;
}
void upd(node &x,node y)
{
	buf3[++tot3]=&x;val3[tot3]=x;
	x=y;
}
int dsu[200005],dsz[200005],dtp[200005];
node fs[200005];
i64 mx[200005],sec[200005];
int mxid[200005],secid[200005];
i64 rc[200005];
int find(int x){return dsu[x]==x?x:find(dsu[x]);}
void merge(int x,int y)
{
	if(dsz[x]>dsz[y]) swap(x,y);
	upd(dsz[y],dsz[x]+dsz[y]);
	upd(dsu[x],y);
	int t=dep[dtp[x]]<dep[dtp[y]]?dtp[x]:dtp[y];
	upd(fs[t],fs[dtp[x]]+fs[dtp[y]]);
	int lt=nxt(dtp[x]^dtp[y]^t,t);
	upd(fs[lt],fs[lt]+fs[dtp[x]^dtp[y]^t]);
	upd(dtp[y],t);
}
int hv[200005];
void solve(int x,int l,int r,i64 cur)
{
	if(!vis[x]) return;vis[x]=0;
	int m1=tot1,m2=tot2,m3=tot3;
	for(int u:v[x]){
		upd(hv[u],1);
		cur=max(cur,rc[u]);
		int id=find(u),t=dtp[id];
		i64 ori=-1;
		if(fa[t]&&fc[fa[t]]==fc[t]){
			int tf=find(fa[t]),tt=dtp[tf];
			ori=max(dis[fs[tt].a],dis[fs[tt].b])-dis[fa[tt]];
			merge(tf,id);
			id=find(u);t=dtp[id];
		}

		if(hv[t]){
			cur=max(cur,fs[t].c);
			i64 len=max(dis[fs[t].a],dis[fs[t].b])-dis[fa[t]];
			int b=bel[t];
			if(mx[b]==ori) upd(mx[b],len);
			else if(mx[b]<len) upd(sec[b],mx[b]),upd(mx[b],len);
			else if(sec[b]<len) upd(sec[b],len);
			if(mx[b]+sec[b]>rc[fa[t]]) upd(rc[fa[t]],mx[b]+sec[b]);
			if(hv[fa[t]]) cur=max(cur,rc[fa[t]]);
		}
		else{
			cur=max(cur,fs[nxt(u,t)].c);
		}
	}
	v[x].clear();

	ans[x]=max(ans[x],cur);
	if(l<r){
		int mid=(l+r)/2;
		solve(x*2,l,mid,cur);
		solve(x*2+1,mid+1,r,cur);
	}

	while(tot1>m1) *buf1[tot1]=val1[tot1],tot1--;
	while(tot2>m2) *buf2[tot2]=val2[tot2],tot2--;
	while(tot3>m3) *buf3[tot3]=val3[tot3],tot3--;
}
void print(int x,int l,int r)
{
	if(l==r){
		printf("%lld\n",ans[x]);
		return;
	}
	ans[x+x]=max(ans[x+x],ans[x]);
	ans[x+x+1]=max(ans[x+x+1],ans[x]);
	int mid=(l+r)/2;
	print(x+x,l,mid);print(x+x+1,mid+1,r);
}
struct rng{int id,l,r;};
vector<rng> vq[200005];
int main()
{
	read(n);read(q);
	fz1(i,n) read(col[i]);
	fz(i,2,n) read(fa[i]);
	fz(i,2,n) read(fc[i]);
	fz(i,2,n) read(fw[i]);
	fz(i,2,n){
		bi[i].push_back((edg){fa[i],fw[i],fc[i]});
		bi[fa[i]].push_back((edg){i,fw[i],fc[i]});
	}
	dfs1(1,0);
	dfs2(1,1);
	fz1(j,18)fz1(i,n-(1<<j)+1) f[j][i]=min(f[j-1][i],f[j-1][i+(1<<(j-1))]);
	fz1(i,n){
		for(auto e:bi[i]){
			if(e.p==fa[i]) continue;
			if(!lst[e.c]) lst[e.c]=e.p;
			bel[e.p]=lst[e.c];
		}
		for(auto e:bi[i]){
			if(e.p==fa[i]) continue;
			lst[e.c]=0;
		}
	}
	fz1(i,n) tim[i]=0;
	fz1(i,q){
		int x,c;read(x);read(c);
		vq[col[x]].push_back((rng){x,tim[x],i-1});
		tim[x]=i;col[x]=c;
	}
	fz1(i,n) vq[col[i]].push_back((rng){i,tim[i],q});
	fz1(i,n) dsu[i]=dtp[i]=i,dsz[i]=1,fs[i]=(node){i,i,0};
	fz1(j,n)if(!vq[j].empty()){
//		cerr<<j<<":\n";
		ff(vq[j],it) update(1,0,q,it->l,it->r,it->id);
		solve(1,0,q,0);
	}
	print(1,0,q);
	return 0;
}
