#include<bits/stdc++.h>
using namespace std;

const int MAXN = int(1e6) + 200;
int N;
char S[MAXN];

int G(int i) {
	char a = S[i % N];
	int res = ('A' <= a && a <= 'Z') ? 2 : 0;
	if ('a' <= a && a <= 'z') a += char('A' - 'a');
	if (a == 'A' || a == 'E' || a == 'O' || a == 'I' || a == 'U' || a == 'Y') res ++;
	return res;
}

const char* vals[4] = {"ciemne", "jasne", "CIEMNE", "JASNE"};

int main() {
	int T; scanf("%d", &T);
	while (T--) {
		scanf("%s", S);
		N = int(strlen(S));
		int l = 0, r = 0;
		while (r < N && G(l) == G(r)) r++;
		if (r == N) {
			printf("%d %s\n", N, vals[G(0)]);
			printf("%d %s\n", N, vals[G(0)]);
			continue;
		}
		pair<int, int> ma(-2 * N, -1);
		pair<int, int> mi(-2 * N, -1);
		for (l = r; l <= N; l = r) {
			while (G(l) == G(r)) r++;
			pair<int, int> ca(r - l, G(l));
			pair<int, int> ci(l - r, G(l));
			ma = max(ma, ca);
			mi = max(mi, ci);
		}
		printf("%d %s\n", ma.first, vals[ma.second]);
		printf("%d %s\n", -mi.first, vals[mi.second]);
	}

	return 0;
}
