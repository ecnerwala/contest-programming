#include<bits/extc++.h>
using namespace std;


struct splitmix64_hash {
	static uint64_t splitmix64(uint64_t x) {
		// http://xorshift.di.unimi.it/splitmix64.c
		x += 0x9e3779b97f4a7c15;
		x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
		x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
		return x ^ (x >> 31);
	}

	size_t operator()(uint64_t x) const {
		static const uint64_t FIXED_RANDOM = std::chrono::steady_clock::now().time_since_epoch().count();
		return splitmix64(x + FIXED_RANDOM);
	}
};

template <typename K, typename V, typename Hash = splitmix64_hash>
using hash_map = __gnu_pbds::gp_hash_table<K, V, Hash>;

template <typename K, typename Hash = splitmix64_hash>
using hash_set = hash_map<K, __gnu_pbds::null_type, Hash>;

using ll = int64_t;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define trav(a, x) for(auto& a : x)
#define all(x) x.begin(), x.end()
#define _sz(x) (int)(x).size()

struct FT {
	vector<ll> s;
	FT(int n) : s(n) {}
	void update(int pos, ll dif) { // a[pos] += dif
		for (; pos < _sz(s); pos |= pos + 1) s[pos] += dif;
	}
	ll query(int pos) { // sum of values in [0, pos)
		ll res = 0;
		for (; pos > 0; pos &= pos - 1) res += s[pos-1];
		return res;
	}
};

namespace std {

template<class Fun>
class y_combinator_result {
	Fun fun_;
public:
	template<class T>
	explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

	template<class ...Args>
	decltype(auto) operator()(Args &&...args) {
		return fun_(std::ref(*this), std::forward<Args>(args)...);
	}
};

template<class Fun>
decltype(auto) y_combinator(Fun &&fun) {
	return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

} // namespace std

int main(){
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int n, q;
	cin >> n >> q;
	string s;
	cin >> s;
	vector<vector<int> > ch(n);
	vector<int> par(n, -1);
	for(int i = 1; i < n; i++){
		int p;
		cin >> p;
		p--;
		ch[p].push_back(i);
		par[i] = p;
	}
	vector<int> sz(n, 1);
	y_combinator(
		[&](auto self, int v) -> void {
			for(int w : ch[v]){
				self(w);
				sz[v] += sz[w];
			}
		}
	)(0);
	for(vector<int>& x : ch){
		for(int j = 0; j < (int)x.size(); j++){
			if(sz[x[j]] > sz[x[0]]){
				swap(x[j], x[0]);
			}
		}
	}
	int T = 0;
	vector<int> st(n), en(n);
	y_combinator(
		[&](auto self, int v) -> void {
			st[v] = T;
			T++;
			for(int w : ch[v]){
				self(w);
			}
			en[v] = T;
		}
	)(0);
	vector<int> heavy_anc(n);
	for(int i = 0; i < n; i++) heavy_anc[i] = i;
	y_combinator(
		[&](auto self, int v) -> void {
			if(!ch[v].empty()) heavy_anc[ch[v].front()] = heavy_anc[v];
			for(int w : ch[v]){
				self(w);
			}
		}
	)(0);
	vector<int> isA(n, 0);
	vector<int> isC(n, 0);
	for(int i = 0; i < n; i++){
		isA[i] = int(s[i] != 'C');
		isC[i] = int(s[i] != 'A');
	}

	ll base_score = 0;

	vector<int> diff(n, 0);

	vector<int> childC(n, 0);
	y_combinator(
		[&](auto self, int v, int score) -> void {
			diff[st[v]] -= score;
			childC[st[v]] = isC[v];
			for(int w : ch[v]){
				self(w, score + isA[w]);
				childC[st[v]] += childC[st[w]];
			}
			if(!isC[v]) base_score += childC[st[v]];
		}
	)(0, isA[0]);
	for(int i = 0; i < n; i++){
		diff[i] += childC[i];
	}

	FT A(n+1);
	FT C(n+1);
	for(int i = 0; i < n; i++){
		if(isC[i]){
			C.update(st[i], 1);
		} else {
			A.update(st[i], 1);
			A.update(en[i], -1);
		}
	}

	const int B = 400;
	int NB = n / B + 1;
	vector<int> offset(NB, 0);
	vector<int> sum(NB, 0);
	vector<int> cnt(NB, 0);
	std::vector<int> a_off(NB, 0);
	vector<std::array<int, 2*B+1>> a(NB);
	for (auto& v : a) {
		std::fill(v.begin(), v.end(), 0);
	}
	string s_order(n, '-');
	for(int i = 0; i < n; i++){
		s_order[st[i]] = s[i];
	}

	auto change_a = [&](int block, int v, int delta) -> void {
		v -= a_off[block];
		if (0 <= v && v <= 2*B) {
			a[block][v] += delta;
		}
	};
	auto get_a = [&](int block, int v) -> int {
		int v2 = v - a_off[block];
		if (0 <= v2 && v2 <= 2 * B) {
			return a[block][v2];
		}
		a_off[block] = v - B;
		memset(a[block].data(), 0, sizeof(a[block]));
		for (int i = block * B; i < min(B * (block+1), n); i++) {
			if (s_order[i] == '?') {
				change_a(block, diff[i], 1);
			}
		}
		return a[block][v - a_off[block]];
	};

	ll diff_sum = 0;
	for(int i = 0; i < n; i++){
		int block = i / B;
		if(s_order[i] == '?'){
			// sum[block] += max(diff[i], 0);
			cnt[block] += int(diff[i] > 0);
			change_a(block, diff[i], 1);
			diff_sum += max(diff[i], 0);
		}
	}
	while(q--){
		int v;
		cin >> v;
		v--;
		char type;
		cin >> type;
		int dA = int(type != 'C') - int(s[v] != 'C');
		int dC = int(type != 'A') - int(s[v] != 'A');
		int dq = int(type == '?') - int(s[v] == '?');
		if(dq){
			int i = st[v];
			int block = i / B;

			int cd = diff[i] + offset[block];
			// sum[block] += max(cd, 0) * dq;
			cnt[block] += int(cd > 0) * dq;
			change_a(block, diff[i], dq);
			diff_sum += max(cd, 0) * dq;
		}
		s_order[st[v]] = type;
		vector<tuple<int, int, int> > diff_upds;
		if(dC){
			if(dC == 1){
				A.update(st[v], -1);
				A.update(en[v], 1);
				C.update(st[v], 1);
			}
			base_score += (A.query(st[v]+1) - (C.query(en[v]) - C.query(st[v]) - 1)) * dC;
			if(dC == -1){
				A.update(st[v], 1);
				A.update(en[v], -1);
				C.update(st[v], -1);
			}
			int cur = v;
			while(cur >= 0){
				diff_upds.push_back({st[heavy_anc[cur]], st[cur] + 1, dC});
				cur = par[heavy_anc[cur]];
			}
		}
		if(dA){
			diff_upds.push_back({st[v], en[v], -dA});
		}
		assert(diff_upds.size() <= 25);
		auto add = [&](int i, int del) -> void {
			// cerr << "add " << i << ' ' << del << '\n';
			int block = i / B;
			if(s_order[i] == '?'){
				int cd = diff[i] + offset[block];
				if(cd > 0){
					// sum[block] -= cd;
					cnt[block] -= 1;
					diff_sum -= cd;
				}
				change_a(block, diff[i], -1);
			}
			diff[i] += del;
			if(s_order[i] == '?'){
				int cd = diff[i] + offset[block];
				if(cd > 0){
					// sum[block] += cd;
					cnt[block] += 1;
					diff_sum += cd;
				}
				change_a(block, diff[i], +1);
			}
		};
		auto add_block = [&](int block, int del) -> void {
			// cerr << "add block " << block << ' ' << del << '\n';
			if(del == 1){
				cnt[block] += get_a(block, -offset[block]);
				// sum[block] += cnt[block];
				diff_sum += cnt[block];
			}
			offset[block] += del;
			if(del == -1){
				// sum[block] -= cnt[block];
				diff_sum -= cnt[block];
				cnt[block] -= get_a(block, -offset[block]);
			}
		};
		for(auto [x, y, z] : diff_upds){
			while(x < y && (x % B) != 0){
				add(x, z);
				x++;
			}
			while(x < y && (y % B) != 0){
				y--;
				add(y, z);
			}
			while(x < y){
				add_block(x / B, z);
				x += B;
			}
			assert(x == y);
			// for(int j = x; j < y; j++){
			// 	diff[j] += z;
			// }
		}
		s[v] = type;

		ll ans = base_score + diff_sum;
		// cerr << "sum " << diff_sum << '\n';
		// ll ans = base_score;
		// for(int i = 0; i < n; i++){
		// 	if(s[i] == '?'){
		// 		ans += max(diff[st[i]], 0);
		// 	}
		// }
		cout << ans << '\n';
	}
}
