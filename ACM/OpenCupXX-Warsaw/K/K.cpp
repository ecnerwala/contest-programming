#include<bits/extc++.h>
using namespace std;

const int MAXN = 4.1e5;

int N;
char S[MAXN];
int suffMatch[MAXN];
vector<int> suffLastMatches[MAXN]; // which indices are removed after i
set<int> suffMatches;
const int L = MAXN;
int suffMatchesBit[L+1];
int matchLensBit[L+1];

void init() {
	N = 0;
	suffMatch[0] = -1;
}

void add(char c) {
	//cerr << "add " << c << '\n';

	S[N++] = c;
	auto removePref = [&](int pref) {
		assert(suffMatches.count(pref));
		suffMatches.erase(pref);
		suffLastMatches[N-1].push_back(pref);
		for (int a = N-1-pref+1; a; a -= a & (-a)) {
			matchLensBit[a-1] ++;
		}
		for (int a = pref+1; a <= L; a += a & (-a)) {
			suffMatchesBit[a-1] --;
		}
	};

	int cur = suffMatch[N-1];
	while (cur != -1 && S[cur] != S[N-1]) {
		// cur
		cur = suffMatch[cur];
		while (!suffMatches.empty() && *suffMatches.begin() < N-1-cur) {
			removePref(*suffMatches.begin());
		}
	}
	if (cur != -1) {
		for (int p : suffLastMatches[cur]) {
			removePref(N-1-cur + p);
		}
	}
	suffMatch[N] = cur+1;
	suffMatches.insert(N);
	for (int a = N+1; a <= L; a += a & (-a)) {
		suffMatchesBit[a-1] ++;
	}

	if (false) {
		for (int i : suffMatches) {
			for (int j = 0; i+j < N; j++) {
				assert(S[j] == S[i+j]);
			}
		}
		for (int i : suffLastMatches[N-1]) {
			for (int j = 0; i+j < N; j++) {
				assert((S[j] == S[i+j]) == (i+j!=N-1));
			}
		}
	}
}

int get(int k) {
	//cerr << "get " << k << '\n';
	int ans = 1;
	for (int a = k+1; a <= L; a += a & (-a)) {
		ans += matchLensBit[a-1];
	}
	for (int a = N-k+1; a; a -= a & (-a)) {
		ans += suffMatchesBit[a-1];
	}
	//cerr << "get " << k << ' ' << ans << '\n';
	return ans;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	init();
	int Q; cin >> Q;
	int last = 0;
	while (Q--) {
		string op; cin >> op;
		if (op == "add") {
			char c; cin >> c;
			c = char('a' + (c - 'a' + last) % 26);
			add(c);
		} else if (op == "get") {
			int k; cin >> k;
			k = (k - 1 + last) % N + 1;
			int ans = get(k);
			cout << ans << '\n';
			last = ans;
		}
	}

	return 0;
}
