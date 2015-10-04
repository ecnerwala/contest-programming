#include<bits/stdc++.h>
using namespace std;
#define mp make_pair

int n;
const int ROT = 0, SYM = 1, BHSYM = 2, BVSYM = 3, DIV = 4, MIX = 5;

vector<int> ops;

bool vis[1888][1888];

pair<int, int> operate(int op, pair<int, int>& p) {
	int r = p.first, c = p.second;
	bool inv = op >> 3;
	op &= ~8;
	switch (op) {
		case ROT:
			if (inv) {
				return make_pair(c, n-1-r);
			} else {
				return make_pair(n-1-c, r);
			}
		case SYM:
			return make_pair(r, n-1-c);
		case BHSYM:
			if (r >= n/2) {
				return mp(r, n-1-c);
			} else {
				return p;
			}
		case BVSYM:
			if (r >= n/2) {
				return mp(3*n/2-1-r, c);
			} else {
				return p;
			}
		case DIV:
			if (inv) {
				if (r < n/2) {
					return mp(2*r, c);
				} else {
					return mp(2*(r-n/2) + 1, c);
				}
			} else {
				if (r & 1) {
					return mp(r/2 + n/2, c);
				} else {
					return mp(r/2, c);
				}
			}
		case MIX:
			if (inv) {
				if (c & 1) {
					return mp(r|1, (n/2)*(r&1) + c/2);
				} else {
					return mp(r&~1, (n/2)*(r&1) + c/2);
				}
			} else {
				if (c < n/2) {
					return mp(r&~1, 2*c+(r&1));
				} else {
					return mp(r|1, 2*(c-n/2)+(r&1));
				}
			}
	}
	throw "gg";
}

int parse(const string & s) {
	int x;
	switch (s[0]) {
		case 'i': return -1;
		case 'r': x = ROT; break;
		case 's': x = SYM; break;
		case 'b': x = (s[1] == 'h' ? BHSYM : BVSYM); break;
		case 'd': x = DIV; break;
		case 'm': x = MIX; break;
		default: throw "gg";
	}
	if (s.back() == '-') x |= 8;
	return x;
}
int gcd(int a, int b) {
	return b ? gcd(b, a % b) : a;
}
int lcm(int a, int b) {
	return a / gcd(a, b) * b;
}
int period(){
	int acc = 1;
	for (int r = 0; r < n; r++) {
		for (int c = 0; c < n; c++) {
			if (vis[r][c]) continue;
			pair<int,int> p = mp(r, c);
			pair<int,int> cur = p;
			int x = 0;
			//int c = 0;
			do {
				vis[cur.first][cur.second] = true;
				for (size_t i = 0; i < ops.size(); i++) {
					cur = operate(ops[i], cur);
					//printf("%d, %d\n", cur.first, cur.second);
				}
				x++;
			} while (cur != p);
			//} while (++c < 8);
			//printf("period ok; cur = %d %d, p = %d %d\n", cur.first, cur.second, p.first, p.second);
			acc = lcm(acc, x);
		}
	}
	return acc;
}
int main() {
	srand(time(0));
	cin >> n;
	string tok;
	while (cin >> tok) {
		int x = parse(tok);
		if (x != -1) ops.push_back(x);
	}
	reverse(ops.begin(), ops.end());
	/*
	int p = 1;
	int nn = 5e8 / (n*n + 1) / (1 + ops.size());
	for (int i = 0; i < nn; i++) {
		p = lcm(p, period());
	}
		*/
	cout << period() << "\n";
}
