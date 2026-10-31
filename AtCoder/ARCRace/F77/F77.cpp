#include<bits/stdc++.h>
using namespace std;

using ll = long long;

string f(string s) {
	int n = int(s.size()) / 2;
	vector<int> fail(n+1);
	fail[0] = -1;
	for (int i = 1; i <= n; i++) {
		int cnd = fail[i-1];
		while (cnd != -1 && s[cnd] != s[i-1]) {
			cnd = fail[cnd];
		}
		assert(cnd == -1 || s[cnd] == s[i-1]);
		fail[i] = cnd+1;
	}
	int repLen = fail[n];
	return s.substr(0, 2 * n - repLen) + s.substr(0, 2 * n - repLen);
}

map<ll, vector<ll>> mem;
set<ll> reflections;
vector<ll> prefCnt(ll a) {
	if (mem.count(a)) {
		return mem[a];
	}
	auto it = reflections.lower_bound(a);
	assert(it != reflections.begin());
	--it;
	ll md = *it;
	vector<ll> v1 = prefCnt(md);
	vector<ll> v2 = prefCnt(a - md);
	for (int i = 0; i < 26; i++) {
		v1[i] += v2[i];
	}
	return mem[a] = v1;
}

vector<ll> go() {
	string S; cin >> S;
	ll L, R; cin >> L >> R; L--;
	string f0 = S;
	string f1 = f(f0);
	string f2 = f(f1);
	ll a = int(f0.size()) / 2;
	ll b = int(f1.size()) / 2;
	ll c = int(f2.size()) / 2;
	if (a + c == b + b) {
		//cerr << "periodic" << '\n';
		ll period = b - a;
		vector<ll> ans(26);
		for (int i = 0; i < period; i++) {
			ans[S[i] - 'a'] += (R - i + period - 1) / period - (L - i + period - 1) / period;
		}
		return ans;
	}

	//cerr << "fibo" << '\n';
	mem[0] = vector<ll>(26, 0);
	for (int i = 0; i < int(S.size()); i++) {
		mem[i+1] = mem[i];
		mem[i+1][S[i]-'a']++;
	}
	for (ll x = a, y = b; x <= R; y += x, x = y - x) {
		reflections.insert(x);
	}
	vector<ll> rans = prefCnt(R);
	vector<ll> lans = prefCnt(L);
	vector<ll> ans(26);
	for (int i = 0; i < 26; i++) {
		ans[i] = rans[i] - lans[i];
	}
	return ans;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	auto ans = go();
	for (int i = 0; i < 26; i++) {
		cout << ans[i] << " \n"[i+1==26];
	}

	return 0;
}
