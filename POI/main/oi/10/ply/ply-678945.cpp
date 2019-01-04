#include<bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;
typedef pair<pii, pii> ppp;
#define A first
#define B second

ppp dfs() {
	char c;
	scanf(" %c", &c);
	if(c == 'X') {
		return ppp(pii(1, 1), pii(1, 2));
	} else if(c == 'S') {
		int n; scanf("%d", &n);
		ppp cur = dfs();
		for(int i = 1; i < n; i++) {
			ppp nxt = dfs();
			ppp val = ppp( pii(
						min(cur.A.A + nxt.A.A, cur.A.B + nxt.B.A - 1),
						min(cur.A.A + nxt.A.B, cur.A.B + nxt.B.B - 1)
					   ), pii(
						min(cur.B.A + nxt.A.A, cur.B.B + nxt.B.A - 1),
						min(cur.B.A + nxt.A.B, cur.B.B + nxt.B.B - 1)
					   ));
			cur = val;
		}
		return cur;
	} else if(c == 'R') {
		int n; scanf("%d", &n);
		ppp cur = dfs();
		for(int i = 1; i < n; i++) {
			ppp nxt = dfs();
			ppp val = ppp( pii(
						cur.A.A + nxt.A.A,
						min(cur.A.A + nxt.A.B, cur.A.B + nxt.A.A)
					   ), pii(
						min(cur.B.A + nxt.A.A, cur.A.A + nxt.B.A),
						min(min(cur.B.B + nxt.A.A, cur.B.A + nxt.A.B), min(cur.A.B + nxt.B.A, cur.A.A + nxt.B.B))
					   ));
			cur = val;
		}
		cur.A.A += 2;
		cur.B.A += 1;
		cur.A.B += 1;
		ppp res = ppp(pii(cur.B.B, cur.B.A), pii(cur.A.B, cur.A.A));
		res.A.B = min(res.A.B, res.B.B);
		res.B.A = min(res.B.A, res.B.B);
		res.A.A = min(res.A.A, min(res.A.B, res.B.A));
		return res;
	} else assert(false);
	return ppp();
}

int main() {
	cout << dfs().A.A << '\n';
}
