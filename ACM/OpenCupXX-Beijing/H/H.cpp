#include<bits/stdc++.h>
using namespace std;

int go2(const vector<string>& strings, bool z) {
	int ans = 0;

	int numA = 0;
	int numS = 0;
	int numSA = 0;
	int numD = 0;
	int numAD = 0;

	bool hasSDedge = false;
	bool hasSedge = false;
	bool hasDedge = false;

	for (const string& s : strings) {
		if (s == "A") {
			numA++;
			continue;
		}
		for (int j = 0; j+3 <= int(s.size()); j++) {
			if (s[j] == 'S' && s[j+1] == 'A' && s[j+2] == 'D') {
				ans++;
			}
		}
		int prefL;
		if (s[0] == 'D') {
			numD++;
			prefL = 1;
		} else if (s.size() >= 2 && s[0] == 'A' && s[1] == 'D') {
			numAD++;
			prefL = 2;
		} else {
			prefL = 0;
		}

		int suffL;
		if (s.back() == 'S') {
			numS++;
			suffL = 1;
		} else if (s.size() >= 2 && s.back() == 'A' && s[int(s.size()) - 2] == 'S') {
			numSA++;
			suffL = 2;
		} else {
			suffL = 0;
		}

		if (prefL == 0 && suffL == 1) {
			hasSedge = true;
		}
		if (prefL == 0 && suffL == 2) {
			hasDedge = true;
		}
		if (suffL == 0 && prefL == 1) {
			hasDedge = true;
		}
		if (suffL == 0 && prefL == 2) {
			hasSedge = true;
		}
		if (prefL && suffL && prefL + suffL != 3) {
			hasSDedge = true;
		}
	}

	bool useS = false;
	bool useD = false;

	if (z) {
		if (numS && numA && numD) {
			ans ++;
			numS --;
			numD --;
			numA --;
			hasSDedge = true;
			useS = true;
			useD = true;
		}
	}

	if (numS && numAD) {
		int v = min(numS, numAD);
		ans += v;
		numS -= v, numAD -= v;
		useS = true;
	}

	if (numSA && numD) {
		int v = min(numSA, numD);
		ans += v;
		numSA -= v, numD -= v;
		useD = true;
	}

	if (numS && numA && numD) {
		int v = min(numA, min(numS, numD));
		ans += v;
		numS -= v;
		numD -= v;
		numA -= v;
		hasSDedge = true;
		useS = useD = true;
	}

	if (numS || numAD) hasSedge = true;
	if (numSA || numD) hasDedge = true;

	int sub = 0;
	if (useS && !hasSedge && !(hasSDedge && hasDedge)) {
		sub++;
		hasSedge = true;
	}
	if (useD && !hasDedge && !(hasSDedge && hasSedge)) {
		sub++;
		hasDedge = true;
	}
	ans -= sub;

	return ans;
}

int go(const vector<string>& strings) {
	return max(go2(strings, false), go2(strings, true));
}

int value(const string& s) {
	int ans = 0;
	for (int i = 0; i + 3 <= int(s.size()); i++) {
		if (s[i] == 'S' && s[i+1] == 'A' && s[i+2] == 'D') ans++;
	}
	return ans;
}

int slow(const vector<string>& strings) {
	int N = int(strings.size());
	int ans = 0;
	vector<int> P(N);
	iota(P.begin(), P.end(), 0);
	do {
		string v;
		for (int i = 0; i < N; i++) {
			v += strings[P[i]];
		}
		ans = max(ans, value(v));
	} while (next_permutation(P.begin(), P.end()));
	return ans;
}

void test() {
	vector<string> prefs({"", "D", "AD", "SAD"});
	vector<string> suffs({"", "S", "SA", "SAD"});
	vector<string> cnds;
	cnds.push_back("A");
	for (string a : prefs) for (string b : suffs) if (!a.empty() || !b.empty()) cnds.push_back(a+b);
	mt19937 mt(48);
	for (int z = 0; z < 500; z++) {
		int N = (mt() % 7) + 1;
		vector<string> strings(N);
		for (int i = 0; i < N; i++) {
			strings[i] = cnds[mt() % int(cnds.size())];
		}
		//cerr << N << '\n';
		//for (string s : strings) cerr << s << '\n';
		if (slow(strings) != go(strings)) {
			cerr << "Mismatch" << '\n';
			cerr << N << '\n';
			for (string s : strings) cerr << s << '\n';
			cerr << "slow: " << slow(strings) << '\n';
			cerr << "go: " << go(strings) << '\n';
			exit(0);
		}
	}
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	//test();
	int T; cin >> T;
	for (int t = 1; t <= T; t++) {
		//cerr << "Case #" << t << '\n';
		int N; cin >> N;
		vector<string> strings(N);
		for (int i = 0; i < N; i++) {
			cin >> strings[i];
		}
		int ans = go(strings);
		cout << "Case #" << t << ": " << ans << '\n';
	}

	return 0;
}
