#include<bits/stdc++.h>
using namespace std;

int RUNTIME = 0;

namespace kactl {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"

#define rep(i, a, b) for(int i = int(a); i < int(b); ++i)
#define per(i, a, b) for(int i = int(b)-1; i >= int(a); --i)
#define trav(a, x) for(auto& a : x)
#define all(x) x.begin(), x.end()
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

const int N=8000010;
char s[N];
int *h;

int sa[N],rk[N],ht[N];
bool t[N<<1];

bool islms(const int i,const bool *t) { /// start-hash
	return i>0&&t[i]&&!t[i - 1];
} /// end-hash

template<class T>
inline void sort(T s,int *sa,const int len,const int sz,const int sigma,
		bool *t,int *b,int *cb,int *p) { /// start-hash
	RUNTIME += len;
	memset(b,0,sizeof(int)*sigma);
	memset(sa,-1,sizeof(int)*len);
	rep(i,0,len) b[(int)s[i]]++;
	cb[0]=b[0];
	rep(i,1,sigma) cb[i]=cb[i-1]+b[i];
	per(i,0,sz) sa[--cb[(int)s[p[i]]]]=p[i];
	rep(i,1,sigma) cb[i]=cb[i-1]+b[i-1];
	rep(i,0,len) if (sa[i]>0&&!t[sa[i]-1]) sa[cb[(int)s[sa[i]-1]]++]=sa[i]-1;
	cb[0]=b[0];
	rep(i,1,sigma) cb[i]=cb[i-1]+b[i];
	per(i,0,len) if (sa[i]>0&&t[sa[i]-1]) sa[--cb[(int)s[sa[i]-1]]]=sa[i]-1;
} /// end-hash

template<class T>
inline void sais(T s,int *sa,const int len,bool *t,int *b,int *b1,
		const int sigma) { /// start-hash
	int p=-1,*cb=b+sigma;
	t[len-1]=1;
	per(i,0,len-1) t[i]=s[i]<s[i+1]||(s[i]==s[i+1]&&t[i+1]);
	int sz=0,cnt=0;
	rep(i,1,len) if (t[i]&&!t[i-1]) b1[sz++]=i;
	sort(s,sa,len,sz,sigma,t,b,cb,b1);
	sz=0;
	rep(i,0,len) if (islms(sa[i],t)) sa[sz++]=sa[i];
	rep(i,sz,len) sa[i]=-1;
	rep(i,0,sz) {
		int x=sa[i];
		rep(j,0,len) {
			if (p==-1||s[x+j]!=s[p+j]||t[x+j]!=t[p+j]) {
				cnt++; p=x;
				break;
			} else if (j>0&&(islms(x+j,t)||islms(p+j,t))) {
				break;
			}
		}
		sa[sz+(x>>=1)]=cnt-1;
	}
	for (int i=len-1,j=len-1;i>=sz;i--) if (sa[i]>=0) sa[j--]=sa[i];
	int *s1=sa+len-sz,*b2=b1+sz;
	if (cnt<sz) sais(s1,sa,sz,t+len,b,b1+sz,cnt);
	else rep(i,0,sz) sa[s1[i]]=i;
	rep(i,0,sz) b2[i]=b1[sa[i]];
	sort(s,sa,len,sz,sigma,t,b,cb,b2);
} /// end-hash

template<class T>
inline void init(T s,const int len,const int sigma) { /// start-hash
	sais(s,sa,len,t,rk,ht,sigma);
} /// end-hash

inline void solve(const char *s,int len) {
	init(s,len+1,124);
	rep(i,0,len+1) rk[sa[i]]=i;
}

#pragma GCC diagnostic pop

} // namespace kactl

const int MAXR = 600;
const int MAXC = 600;
int R, C;
char G[MAXR][MAXC];

const int MAXQ = int(2.1e5);
int Qreal;
int Q;
int QL;
int querySz[MAXQ];
pair<int, int> queryPos[MAXQ];
int ans[MAXQ];
map<string, int> queryInd;
int realQueryInd[MAXQ];

int pref[kactl::N];
int pref2[kactl::N];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> R >> C >> Qreal;
	for (int i = 0; i < R; i++) {
		cin >> G[i];
	}
	QL = 0;
	for (int q = 0; q < Qreal; q++) {
		string query; cin >> query;
		if (int(query.size()) >= R+C) {
			// first R+C are enough to exclude everything
			realQueryInd[q] = -1;
			continue;
		}
		if (queryInd.count(query)) {
			realQueryInd[q] = queryInd[query];
			continue;
		}

		realQueryInd[q] = queryInd[query] = Q++;

		querySz[q] = int(query.size());

		queryPos[q].first = QL;
		for (int z = 0; z < int(query.size()); z++) {
			kactl::s[QL++] = query[z];
		}
		kactl::s[QL++] = '$';

		queryPos[q].second = QL;
		for (int z = 0; z < int(query.size()); z++) {
			kactl::s[QL++] = query[z];
		}
		kactl::s[QL++] = '^';
	}

	const int B = 10;
	for (int iBucket = 0; iBucket < R; iBucket += B) {
		using kactl::s;
		int len = QL;
		for (int i = iBucket; i < min(R, iBucket + B); i++) {
			for (int j = 0; j < C; j++) {
				for (int k = 0; k <= j; k++) {
					s[len++] = G[i][k];
				}
				for (int k = i+1; k < R; k++) {
					s[len++] = G[k][j];
				}
				s[len++] = '@';
			}
		}
		//cerr << "Solve " << iBucket << '\n';
		//cerr << "Solve " << s << '\n';
		s[len] = '\0';
		kactl::solve(kactl::s, len);

		using kactl::sa;
		using kactl::rk;

		for (int z = 0; z <= len; z++) {
			pref[z] = 0;
		}

		int ind = QL;
		for (int i = iBucket; i < min(R, iBucket + B); i++) {
			for (int j = 0; j < C; j++) {
				for (int k = 0; k <= j; k++) {
					pref[rk[ind]] ++;
					ind++;
				}
				for (int k = i+1; k < R; k++) {
					ind++;
				}
				ind++;
			}
		}

		for (int z = 1; z <= len; z++) {
			pref[z] += pref[z-1];
		}

		for (int q = 0; q < Q; q++) {
			int lo, hi; tie(lo, hi) = queryPos[q];

			//cerr << queries[q] << ' ' << pref[rk[hi]] - pref[rk[lo]] << '\n';
			ans[q] += pref[rk[hi]] - pref[rk[lo]];
		}
	}

	{
		using kactl::s;
		int len = QL;
		for (int i = 0; i < R; i++) {
			for (int j = 0; j < C; j++) {
				s[len++] = G[i][j];
			}
			s[len++] = '@';
		}
		//cerr << "Solve " << s << '\n';
		kactl::s[len] = '\0';
		kactl::solve(kactl::s, len);

		using kactl::sa;
		using kactl::rk;

		for (int z = 0; z <= len; z++) {
			pref[z] = 0;
			pref2[z] = 0;
		}

		int ind = QL;
		for (int i = 0; i < R; i++) {
			for (int j = 0; j < C; j++) {
				pref[rk[ind]]++;
				pref2[rk[ind]] += C - j;
				ind++;
			}
			ind++;
		}

		for (int z = 1; z <= len; z++) {
			pref[z] += pref[z-1];
			pref2[z] += pref2[z-1];
		}

		for (int q = 0; q < Q; q++) {
			int lo, hi; tie(lo, hi) = queryPos[q];

			ans[q] -= (pref2[rk[hi]] - pref2[rk[lo]]) - querySz[q] * (pref[rk[hi]] - pref[rk[lo]]);
		}
	}

	for (int q = 0; q < Qreal; q++) {
		if (realQueryInd[q] == -1) {
			cout << 0 << '\n';
		} else {
			cout << ans[realQueryInd[q]] << '\n';
		}
	}

	cerr << "RUNTIME: " << RUNTIME << '\n';

	return 0;
}
