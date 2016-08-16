#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

ll get_m(ll N) {
	if(N == 0) return 1;
	return (N % 2 == 0) ? ((1ll << (N - 1)) + (1ll << (N / 2 - 1))) : (1ll << (N - 1));
}

string flip(string a) {
	for(char &i : a) {
		i ^= '0' ^ '1';
	}
	reverse(a.begin(), a.end());
	return a;
}

ll to_num(string a) {
	ll res = 0;
	for(char i : a) {
		res <<= 1;
		res += (i == '1');
	}
	return res;
}

ll decode(string a, ll n) {
	if(n == 0) return 0;
	assert(n == ll(a.size()));
	if(n % 2) {
		if(a[n / 2] != '0') a = flip(a);
		a.erase(a.begin() + n / 2);
		return to_num(a);
	} else if(a[n / 2 - 1] == a[n / 2]) {
		if(a[n / 2] != '0') a = flip(a);
		a.erase(a.begin() + n / 2 - 1);
		a.erase(a.begin() + n / 2 - 1);
		return to_num(a);
	} else if(a[n / 2 - 1] == '1') {
		a.erase(a.begin() + n / 2 - 1);
		a.erase(a.begin() + n / 2 - 1);
		return decode(a, n - 2) + (1ll << (n - 2)) + get_m(n - 2);
	} else {
		a.erase(a.begin() + n / 2 - 1);
		a.erase(a.begin() + n / 2 - 1);
		return decode(a, n - 2) + (1ll << (n - 2));
	}
}

string to_string(ll v, ll n) {
	string res;
	for(ll i = n - 1; i >= 0; i--) {
		res += (v & (1ll << i)) ? '1' : '0';
	}
	return res;
}

string encode(ll v, ll n) {
	if(n == 0) return "";
	string res;
	if(n % 2) {
		res = to_string(v, n - 1);
		res.insert(res.begin() + n / 2, '0');
	} else if(v < (1ll << (n - 2))) {
		res = to_string(v, n - 2);
		res.insert(res.begin() + n / 2 - 1, '0');
		res.insert(res.begin() + n / 2, '0');
	} else if(v < (1ll << (n - 2)) + get_m(n - 2)) {
		res = encode(v - (1ll << (n - 2)), n - 2);
		res.insert(res.begin() + n / 2 - 1, '0');
		res.insert(res.begin() + n / 2, '1');
	} else {
		res = encode(v - (1ll << (n - 2)) - get_m(n - 2), n - 2);
		res.insert(res.begin() + n / 2 - 1, '1');
		res.insert(res.begin() + n / 2, '0');
	}
	assert(ll(res.size()) == n);
	return res;
}

void test(ll v, ll N) {
	v %= get_m(N);
	string s = encode(v, N);
	assert(v == decode(s, N));
	assert(v == decode(flip(s), N));
	cerr << v << ' ' << s << ' ' << flip(s) << '\n';
}

ll lrand() {
	ll r = 0;
	for (int i = 0; i < 20; i++) {
		r = r << 1;
		r ^= rand();
	}
	return r;
}
int main() {
	ios_base::sync_with_stdio(0);
	for(int i = 1; i < 10; i++) for(ll v = 0; v < 2 * get_m(i); v++) test(v, i);
	cerr << "small tests done\n";
	while(true) test(lrand(), 60);
	cerr << "small tests done\n";
	ll v, N, Q;
	cin >> v >> N >> Q;
	if(v == 0) {
		cout << get_m(N) << '\n';
		for(int i = 0; i < Q; i++) {
			ll val; cin >> val;
			val %= get_m(N);
			cout << encode(val, N) << '\n';
		}
	} else {
		for(int i = 0; i < Q; i++) {
			string s; cin >> s;
			cout << decode(s, N) << '\n';
		}
	}

	return 0;
}
