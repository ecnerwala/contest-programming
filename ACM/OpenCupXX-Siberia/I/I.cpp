#include<bits/stdc++.h>
using namespace std;

using ld = long double;

const int MAXL = 4.1e5;
namespace kactl_sa {
const int maxn = MAXL;
int wa[maxn],wb[maxn],wv[maxn],ws[maxn];
int cmp(int *r,int a,int b,int l) { /// start-hash
	return r[a]==r[b]&&r[a+l]==r[b+l];
}
void da(const char *r,int *sa,int n,int m) {
	int i,j,p,*x=wa,*y=wb,*t;
	for(i=0;i<m;i++) ws[i]=0;
	for(i=0;i<n;i++) ws[x[i]=r[i]]++;
	for(i=1;i<m;i++) ws[i]+=ws[i-1];
	for(i=n-1;i>=0;i--) sa[--ws[x[i]]]=i;
	for(j=1,p=1;p<n;j*=2,m=p){
		for(p=0,i=n-j;i<n;i++) y[p++]=i;
		for(i=0;i<n;i++)
			if(sa[i]>=j) y[p++]=sa[i]-j;
		for(i=0;i<n;i++) wv[i]=x[y[i]];
		for(i=0;i<m;i++) ws[i]=0;
		for(i=0;i<n;i++) ws[wv[i]]++;
		for(i=1;i<m;i++) ws[i]+=ws[i-1];
		for(i=n-1;i>=0;i--) sa[--ws[wv[i]]]=y[i];
		for(t=x,x=y,y=t,p=1,x[sa[0]]=0,i=1;i<n;i++)
			x[sa[i]]=cmp(y,sa[i-1],sa[i],j)?p-1:p++;
	}
} /// end-hash
//height[i]: lcp(sa[i],sa[i-1])
int rank[maxn],height[maxn];
void calheight(const char *r,int *sa,int n) { /// start-hash
	int i,j,k=0;
	for(i=0;i<=n;i++) rank[sa[i]]=i;
	for(i=0;i<n;height[rank[i++]]=k)
		for(k?k--:0,j=sa[rank[i]-1]; r[i+k]==r[j+k]; k++);
} /// end-hash
}

string A, B;
int N, M;
string V;
int L;
int suff_array[MAXL];
using kactl_sa::height;

const int S = 1 << 20;
struct {
	bool hasK = false;
	int a = 0, b = 0;
	int pa = 0, pb = 0, sa = 0, sb = 0;
	long long cntAB = 0, cntBA = 0;
	long long cntAB0 = 0, cntBA0 = 0;
} seg[2 * S];

void setE(int i) {
	seg[S+i] = { false, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };
}
void setA(int i) {
	seg[S+i] = { false, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, };
}
void setB(int i) {
	seg[S+i] = { false, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, };
}
void setK(int i) {
	seg[S+i] = { true, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };
}
void setNode(int i) {
	assert(i < S);
	const auto& l = seg[2*i];
	const auto& r = seg[2*i+1];
	seg[i].hasK = l.hasK || r.hasK;
	seg[i].a = l.a + r.a;
	seg[i].b = l.b + r.b;

	seg[i].pa = l.pa + (l.hasK ? 0 : r.pa);
	seg[i].pb = l.pb + (l.hasK ? 0 : r.pb);

	seg[i].sa = r.sa + (r.hasK ? 0 : l.sa);
	seg[i].sb = r.sb + (r.hasK ? 0 : l.sb);

	seg[i].cntAB = l.cntAB + r.cntAB + 1ll * l.a * r.b;
	seg[i].cntBA = l.cntBA + r.cntBA + 1ll * l.b * r.a;
	seg[i].cntAB0 = l.cntAB0 + r.cntAB0 + 1ll * l.sa * r.pb;
	seg[i].cntBA0 = l.cntBA0 + r.cntBA0 + 1ll * l.sb * r.pa;
}
void updateChain(int i) {
	for (int a = (i+S) / 2; a; a /= 2) {
		setNode(a);
	}
}

vector<int> evts[MAXL];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> A >> B;
	N = int(A.size()), M = int(B.size());
	V = A + '!' + B + '#';
	L = int(V.size());

	V += '\0';
	kactl_sa::da(V.data(), suff_array, L+1, 256);
	kactl_sa::calheight(V.data(), suff_array, L);
	for (int i = 0; i <= L; i++) {
		//cerr << kactl_sa::rank[i] << ' ' << height[i] << ' ' << suff_array[i] << ' ' << V.substr(suff_array[i]) << '\n';
	}

	for (int i = 0; i < N; i++) {
		setA(2 * kactl_sa::rank[i]);
	}
	for (int j = 0; j < M; j++) {
		setB(2 * kactl_sa::rank[N+1+j]);
	}
	for (int i = 1; i <= L; i++) {
		evts[height[i]].push_back(2 * i - 1);
	}
	for (int i = S-1; i; i--) {
		setNode(i);
	}

	cout << fixed << setprecision(13);
	for (int k = 1; k <= min(N, M); k++) {
		for (int v : evts[k-1]) {
			setK(v);
			updateChain(v);
		}
		long long a = seg[1].cntAB - seg[1].cntAB0;
		long long b = seg[1].cntBA - seg[1].cntBA0;
		long long c = seg[1].cntAB0 + seg[1].cntBA0;
		cout << ld(a) / (a + b + c) << ' ' << ld(c) / (a + b + c) << ' ' << ld(b) / (a + b + c) << '\n';

		setE(2 * kactl_sa::rank[N-k]);
		updateChain(2 * kactl_sa::rank[N-k]);
		setE(2 * kactl_sa::rank[N+1+M-k]);
		updateChain(2 * kactl_sa::rank[N+1+M-k]);
	}

	return 0;
}
