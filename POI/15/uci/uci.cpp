#include<bits/stdc++.h>
using namespace std;

const int MAXN = 200, MAXM = 200;

int N, M;
int MOD;
int X, Y;
char G[MAXN][MAXM];
int cnt[MAXN][MAXM];

inline bool is_open(const int &x1, const int &y1, const int &x2, const int &y2) {
	return !(cnt[x2+1][y2+1] - cnt[x1][y2+1] - cnt[x2+1][y1] + cnt[x1][y1]);
}
inline bool can_vert(const int &y, const int &x1, const int &x2) { return is_open(x1, y, x2, y); }
inline bool can_horz(const int &x, const int &y1, const int &y2) { return is_open(x, y1, x, y2); }

// directions:
// 0 is along increasing y1 to increasing x1
// 1 is along increasing x1 to decreasing y2
// 2 is along decreasing y2 to decreasing x2
// 3 is along decreasing x2 to increasing y1
struct state {
	char x1, x2, y1, y2;
	char dir;

};
bool operator == (const state &s, const state &t) {
	return s.dir == t.dir && s.x1 == t.x1 && s.x2 == t.x2 && s.y1 == t.y1 && s.y2 == t.y2;
}
struct num {
	int v;
	num(int a = 0) : v(a) {}

	num& operator += (const num &o) {
		v += o.v;
		v -= (v >= MOD) ? MOD : 0;
		return *this;
	}

	operator int() const { return v; }
};

struct dp_map {
	int S;
	num v[4 * 100 * 100 * 100 + 20];
	vector< int > vals;
	inline int stoi(const state &s) const {
		assert(s.x2 - s.x1 + s.y2 - s.y1 == S);
		return ((s.dir * (M-Y) + s.y2 - Y) * (Y+1) + s.y1) * N + s.x2;
	}
	inline state itos(int i) {
		state s;
		s.x2 = char(i % N); i /= N;
		s.y1 = char(i % (Y+1)); i /= (Y+1);
		s.y2 = char(i % (M-Y)+Y); i /= (M-Y);
		s.dir = char(i);
		s.x1 = char(s.x2 + s.y2 - s.y1 - S);
		return s;
	}
	inline num& operator [] (const state &s) {
		assert(s.x2 - s.x1 + s.y2 - s.y1 == S);
		int i = stoi(s);
		if(v[i] == 0) { vals.push_back(i); }
		return v[i];
	}
	inline void clear() {
		//memset(v, 0, sizeof(v));
		vals.clear();
	}
};

dp_map mp[2];

int main() {
	scanf("%d %d %d", &N, &M, &MOD);
	if(MOD == 1) {
		printf("%d\n", 0);
		return 0;
	}
	scanf("%d %d", &Y, &X);
	X--, Y--;
	for(int i = 0; i < N; i++) scanf("%s", G[i]);
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < M; j++) {
			cnt[i+1][j+1] = cnt[i+1][j] + cnt[i][j+1] - cnt[i][j] + (G[i][j] == '*');
		}
	}

	state start;
	start.dir = 0;
	start.x1 = 0, start.x2 = char(N-1), start.y1 = 0, start.y2 = char(M-1);

	mp[0].clear();
	mp[1].clear();

	mp[0].S = (N - 1) + (M - 1);
	mp[0][start] = 1;

	num res;

	for(int z = 0; true; z ^= 1) {
		bool did_change = false;
		mp[!z].S = mp[z].S - 1;
		for(vector<int>::iterator it = mp[z].vals.begin(); it != mp[z].vals.end(); it++) {
			state cur = mp[z].itos(*it);
			num n = mp[z].v[*it];
			if(n == 0) continue;
			mp[z].v[*it] = 0;

			assert(cur.dir < 4);
			assert(cur.x1 <= X);
			assert(cur.x2 >= X);
			assert(cur.y1 <= Y);
			assert(cur.y2 >= Y);

			if(cur.dir == 0) {
				if(cur.y1 == Y) {
					if(can_vert(cur.y1, X, cur.x2)) res += n;
				} else {
					if(can_vert(cur.y1, cur.x1, cur.x2)) {
						state nxt = cur;
						nxt.y1++;
						nxt.dir++;
						mp[!z][nxt] += n;
						did_change = true;
					}
					if(cur.x1 < X) {
						state nxt = cur;
						nxt.x1++;
						mp[!z][nxt] += n;
						did_change = true;
					}
				}
			} else if(cur.dir == 1) {
				if(cur.x1 == X) {
					if(can_horz(cur.x1, cur.y1, Y)) res += n;
				} else {
					if(can_horz(cur.x1, cur.y1, cur.y2)) {
						state nxt = cur;
						nxt.x1++;
						nxt.dir++;
						mp[!z][nxt] += n;
						did_change = true;
					}
					if(cur.y2 > Y) {
						state nxt = cur;
						nxt.y2--;
						mp[!z][nxt] += n;
						did_change = true;
					}
				}
			} else if(cur.dir == 2) {
				if(cur.y2 == Y) {
					if(can_vert(cur.y2, cur.x1, X)) res += n;
				} else {
					if(can_vert(cur.y2, cur.x1, cur.x2)) {
						state nxt = cur;
						nxt.y2--;
						nxt.dir++;
						mp[!z][nxt] += n;
						did_change = true;
					}
					if(cur.x2 > X) {
						state nxt = cur;
						nxt.x2--;
						mp[!z][nxt] += n;
						did_change = true;
					}
				}
			} else if(cur.dir == 3) {
				if(cur.x2 == X) {
					if(can_horz(cur.x2, Y, cur.y2)) res += n;
				} else {
					if(can_horz(cur.x2, cur.y1, cur.y2)) {
						state nxt = cur;
						nxt.x2--;
						nxt.dir=0;
						mp[!z][nxt] += n;
						did_change = true;
					}
					if(cur.y1 < Y) {
						state nxt = cur;
						nxt.y1++;
						mp[!z][nxt] += n;
						did_change = true;
					}
				}
			} else assert(false);
		}
		mp[z].clear();
		if(!did_change) break;
	}

	printf("%d\n", int(res) % MOD);

	return 0;
}
