#include<bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef unsigned long long ull;

typedef pair<int, int> pii;
#define A first
#define B second

const int MAXN = 450;
const int MAXM = 60050;

struct bs{
	static const int L = 7;
	static const int B = 64;

	ull v[L];

	struct iter {
		bs &par;
		int ind;
		ull val;

		int operator * () {
			//assert(val);
			return __builtin_ctzll(val) + ind * B;
		}

		iter& operator ++ () {
			//assert(val);
			val &= val - 1;
			//assert(ind);
			while(!val && ind < L) {
				ind++;
				if(ind == L) val = 0;
				else val = par.v[ind];
			}
			return *this;
		}

		bool operator==(const iter &a) {
			return &(a.par) == &par && a.ind == ind && a.val == val;
		}

		bool operator !=(const iter &a) {
			return !operator==(a);
		}

		iter(bs &p, int i) : par(p), ind(i - 1) {
			val = 1;
			operator++();
		}
	};

	iter begin() { return iter(*this, 0); }
	iter end() { return iter(*this, L); }

	bs& operator &=(const bs &b) {
		for(int i = 0; i < L; i++) v[i] &= b.v[i];
		return *this;
	}

	bs operator &(const bs &b) {
		bs res(*this);
		res &= b;
		return res;
	}

	void set(unsigned int a) {
		v[a / B] |= (1ull << (a % B));
	}
	
	void unset(unsigned int a) {
		v[a / B] &= ~(1ull << (a % B));
	}

	bs() {
		memset(v, 0, sizeof(v));
	}
};

int N, M;

const int DICT = 26;

bs nxt[MAXN][30];
bs prv[MAXN][30];

bs unvis[MAXN];

int len[MAXN][MAXN]; // backtracking
pii back[MAXN][MAXN]; // backtracking
int last[MAXN][MAXN]; // backtracking
const pii START = pii(-1, -1);

#define ITER(i, v) for(__typeof((v).begin()) i = (v).begin(); i != (v).end(); ++i) 

int main() {
	cin >> N >> M;

	for(int i = 0; i < N; i++) {
		for(int j = 0; j < N; j++) {
			len[i][j] = -1;
			unvis[i].set(j);
		}
	}

	queue<pii> q;
	for(int i = 0; i < N; i++) {
		len[i][i] = 0;
		back[i][i] = START;
		unvis[i].unset(i);
		q.push(pii(i, i));
	}

	for(int i = 0; i < M; i++) {
		int x, y; char t;
		cin >> x >> y >> t;
		x--, y--;
		int c = t - 'a';
		nxt[x][c].set(y);
		prv[y][c].set(x);
		len[x][y] = 1;
		back[x][y] = START;
		last[x][y] = c;
		unvis[x].unset(y);
		q.push(pii(x, y));
	}

	while(!q.empty()) {
		int l = q.front().A, r = q.front().B; q.pop();
		//cerr << l << ' ' << r << '\n';
		//cerr << back[l][r].A << ' ' << back[l][r].B << '\n';
		//cerr << last[l][r] << '\n';
		for(int c = 0; c < DICT; c++) {
			ITER(it, prv[l][c]) {
				int a = *it;
				bs cur = unvis[a] & nxt[r][c];
				ITER(jt, cur) {
					int b = *jt;
					assert(len[a][b] == -1);
					len[a][b] = len[l][r] + 2;
					back[a][b] = pii(l, r);
					last[a][b] = c;
					unvis[a].unset(b);
					q.push(pii(a, b));
				}
			}
		}
	}

	int D;
	cin >> D;
	int prv; cin >> prv, prv--;
	for(int i = 1; i < D; i++) {
		int cur; cin >> cur, cur--;
		if(len[prv][cur] != -1) {
			cout << len[prv][cur] << ' ';
			stack<char> st;
			pii pos(prv, cur);
			while(pos.A != pos.B) {
				char c = 'a' + last[pos.A][pos.B];
				cout << c;
				st.push(c);
				pos = back[pos.A][pos.B];
			}
			if(len[prv][cur] % 2) st.pop();
			while(!st.empty()) cout << st.top(), st.pop();
			cout << '\n';
		} else {
			cout << -1 << '\n';
		}
		prv = cur;
	}
}
