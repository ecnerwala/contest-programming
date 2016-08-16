#include<bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;
#define A first
#define B second

const int MAXR = 200;
const int MAXC = 200;
const int MAXN = MAXR + MAXC;

int R, C;
char res[MAXR][MAXC];

int N; // number of courtiers
int prv[MAXN];
int nxt[MAXN];
int lover[MAXN];

int get_edge(int c) {
	if(0 <= c && c < C) {
		return 0;
	} else if(C <= c && c < C + R) {
		return 1;
	} else if(C + R <= c && c < C + R + C) {
		return 2;
	} else if(C + R + C <= c && c < C + R + C + R) {
		return 3;
	} else assert(false);
}

pii get_coords(int c) {
	if(0 <= c && c < C) {
		return pii(0, 2 * c + 1);
	} else if(C <= c && c < C + R) {
		return pii(2 * (c - C) + 1, 2 * C);
	} else if(C + R <= c && c < C + R + C) {
		return pii(2 * R, 2 * (C + R + C - c) - 1);
	} else if(C + R + C <= c && c < C + R + C + R) {
		return pii(2 * (C + R + C + R - c) - 1, 0);
	} else assert(false);
	return pii(0,0);
}

pii move(pii s, pii sq) {
	assert(s != sq);
	pii t = sq;
	assert(res[sq.A / 2][sq.B / 2]);
	int v = ((s < sq) == (res[sq.A / 2][sq.B / 2] == '/')) ? -1 : 1;
	if(s.A != sq.A) {
		t.B += v;
	} else {
		t.A += v;
	}
	return t;
}

pii center(pii sq) {
	if(sq.A == 0) sq.A ++;
	else if(sq.A == 2 * R) sq.A --;
	if(sq.B == 0) sq.B++;
	else if(sq.B == 2 * C) sq.B--;
	return sq;
}

bool check() {
	for(int i = 0; i < N; i++) {
		pii s = get_coords(i);
		pii t = get_coords(lover[i]);
		pii sq = center(s);
		while(true) {
			s = move(s, sq);
			if(s == t) break;
			sq = pii(2 * s.A - sq.A, 2 * s.B - sq.B);
			if(sq.A < 0 || sq.A > 2 * R || sq.B < 0 || sq.B > 2 * C) return false;
		}
	}
	return true;
}

bool go() {
	memset(res, 0, sizeof(res));

	for(int z = 0; z < N / 2; z++) {
		int a = -1;
		for(int i = 0; i < N; i++) {
			if(nxt[i] == lover[i]) {
				a = i;
			}
		}
		if(a == -1) return false;
		int b = nxt[a];

		// mark as used
		prv[nxt[b]] = prv[a], nxt[prv[a]] = nxt[b];
		prv[a] = nxt[a] = prv[b] = nxt[b] = -1;

		// a -> b
		// we use doubled coordinates for edges, +- 1 from (2r + 1, 2c + 1)
		pii s = get_coords(a), t = get_coords(b);
		pii sq = center(s);
		while(true) {
			// move from s through sq;
			if(res[sq.A / 2][sq.B / 2]) {
				s = move(s, sq);
			} else {
				pii ps = s;
				if(s.A != sq.A) {
					if(s.A < sq.A) s.B++;
					else s.B--;
					res[sq.A / 2][sq.B / 2] = ((s.A < sq.A) == (s.B < sq.B)) ? '/' : '\\';
					s.A = sq.A;
				} else if(s.B != sq.B) {
					if(s.B < sq.B) s.A--;
					else s.A ++;
					res[sq.A / 2][sq.B / 2] = ((s.A < sq.A) == (s.B < sq.B)) ? '/' : '\\';
					s.B = sq.B;
				} else assert(false);
				assert(s == move(ps, sq));
			}

			/*
			cerr << a << ' ' << b << '\n';
			for(int i = 0; i < R; i++) {
				for(int j = 0; j < C; j++) {
					cerr << (res[i][j] ? res[i][j] : ' ');
				}
				cerr << '\n';
			}
			cerr << '\n';
			*/
			if(s == t) break;

			sq = pii(2 * s.A - sq.A, 2 * s.B - sq.B);
			if(sq.A < 0 || sq.A > 2 * R || sq.B < 0 || sq.B > 2 * C) {
				return false;
			}

		}

	}

	for(int i = 0; i < R; i++) {
		for(int j = 0; j < C; j++) {
			if(!res[i][j]) {
				res[i][j] = '/';
			}
		}
	}

	return true;
}

bool slow() {
	memset(res, 0, sizeof(res));
	for(int m = 0; m < (1 << (R * C)); m++) {
		for(int i = 0; i < (R * C); i++) {
			res[i / C][i % C] = (bool(m & (1 << i))) ? '/' : '\\';
		}
		if(check()) {
			return true;
		}
	}
	return false;
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for(int case_num = 1; case_num <= T; case_num ++) {
		cin >> R >> C;
		N = 2 * R + 2 * C;
		for(int i = 0; i < N; i++) nxt[i] = (i + 1) % N, prv[nxt[i]] = i;
		for(int i = 0; i < N / 2; i++) {
			int a, b; cin >> a >> b; a--, b--;
			lover[a] = b;
			lover[b] = a;
		}
		
		bool valid = go();
		cout << "Case #" << case_num << ":\n";
		if(valid) {
			for(int i = 0; i < R; i++) {
				res[i][C] = 0;
				cout << res[i] << '\n';
			}
			assert(check());
		} else {
			cout << "IMPOSSIBLE" << '\n';
		}
	}

	return 0;
}
