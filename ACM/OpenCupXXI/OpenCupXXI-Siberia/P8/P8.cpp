#include<bits/extc++.h>

enum class clip_t {
	EMPTY,
	HALF,
	FULL,
};

struct state {
	int perm;
	int position;
	int clip_position;
	clip_t clip;
	friend bool operator == (const state& a, const state& b) {
		return a.perm == b.perm && a.position == b.position && a.clip_position == b.clip_position && a.clip == b.clip;
	}

	friend std::ostream& operator << (std::ostream& o, const state& s) {
		o << "(";
		for (int i = 0; i < 8; i++) o << ((s.perm >> (i * 3)) & 7);
		return o << "," << s.position << "," << s.clip_position << "," << int(s.clip) << ")";
	}
};

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

	size_t operator()(const state& s) const {
		uint64_t v = int(s.clip) | (s.clip_position << 2) | (s.position << 6) | (uint64_t(s.perm) << 10);
		return operator()(v);
	}
};

template <typename K, typename V, typename Hash = splitmix64_hash>
using hash_map = __gnu_pbds::gp_hash_table<K, V, Hash>;

template <typename K, typename Hash = splitmix64_hash>
using hash_set = hash_map<K, __gnu_pbds::null_type, Hash>;

struct pq_t {
	int val;
	state st;
	friend bool operator < (const pq_t& a, const pq_t& b) { return a.val < b.val; }
	friend bool operator > (const pq_t& a, const pq_t& b) { return a.val > b.val; }
};

const std::string MOVES[5] = {
	"Up",
	"Down",
	"Shift-Press",
	"Shift-Release\nCtrl+X",
	"Ctrl+V",
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N; cin >> N;

	int T_UP, T_DOWN, T_PRESS, T_RELEASE, T_CUT, T_PASTE;
	cin >> T_UP >> T_DOWN >> T_PRESS >> T_RELEASE >> T_CUT >> T_PASTE;

	hash_map<int, int> permutation_id;
	vector<int> permutations;
	{
		vector<int> perm(N); iota(perm.begin(), perm.end(), 0);
		do {
			int v = 0;
			for (int i = 0; i < N; i++) {
				v |= perm[i] << (3 * i);
			}
			permutation_id[v] = int(permutations.size());
			permutations.push_back(v);
		} while (next_permutation(perm.begin(), perm.end()));
	}

	int NUM_STATES = int(permutations.size()) * (N+1) * (N+1) * 3;

	auto state_to_int = [&](const state& s) -> int {
		return ((permutation_id[s.perm] * (N+1) + s.position) * (N+1) + s.clip_position) * 3 + int(s.clip);
	};

	// TODO:
	//auto int_to_state = [&](int a) -> state { };

	// state is: permutation, current position
	vector<pair<int, pair<int, int>>> mp(NUM_STATES, {-1, {-1, -1}});
	priority_queue<pq_t, vector<pq_t>, greater<pq_t>> pq;
	auto check_state = [&](state s, int d, int move, int pi) {
		int si = state_to_int(s);
		if (mp[si].first == -1 || d < mp[si].first) {
			mp[si] = {d, {move, pi}};
			pq.push({d, s});
		}
	};

	int start_perm = 0;
	for (int i = 0; i < N; i++) {
		int a; cin >> a; a--;
		start_perm |= a << (3 * i);
	}
	int end_perm = 0;
	for (int i = 0; i < N; i++) {
		int a; cin >> a; a--;
		end_perm |= a << (3 * i);
	}

	state start_state{start_perm, 0, 0, clip_t::EMPTY};
	check_state(start_state, 0, -1, -1);

	auto p8 = [&](int a) { return 1 << (3 * a); };

	while (!pq.empty()) {
		const auto [d, s] = pq.top(); pq.pop();
		int si = state_to_int(s);
		if (mp[si].first != d) continue;
		//cerr << "checking " << d << ' ' << s << '\n';
		if (s.perm == end_perm && s.clip == clip_t::EMPTY) {
			//cerr << mp.size() << ' ' << pq.size() << '\n';
			// we're done
			int num_ops = 0;
			vector<string> ops;
			int cur = si;
			while (true) {
				auto [m, nxt] = mp[cur].second;
				//cerr << "cur state " << cur << ' ' << m << '\n';
				if (m == -1) break;
				ops.push_back(MOVES[m]);
				num_ops++;
				if (m == 3) num_ops++;
				cur = nxt;
			}
			reverse(ops.begin(), ops.end());
			cout << d << ' ' << num_ops << '\n';
			for (string op : ops) cout << op << '\n';
			exit(0);
		}
		do { // UP: decrease pos by 1
			if (s.position == 0) continue;
			state ns = s;
			ns.position--;
			check_state(ns, d + T_UP, 0, si);
		} while (false);

		do { // DOWN: increase pos by 1
			if (s.position == N) continue;
			if (s.clip == clip_t::FULL && s.position == s.clip_position) continue;
			state ns = s;
			ns.position++;
			check_state(ns, d + T_DOWN, 1, si);
		} while (false);

		do { // PRESS SHIFT
			if (s.clip != clip_t::EMPTY) continue;
			state ns = s;
			ns.clip = clip_t::HALF;
			ns.clip_position = s.position;

			check_state(ns, d + T_PRESS, 2, si);
		} while (false);

		do { // RELEASE SHIFT + CUT
			if (s.clip != clip_t::HALF) continue;
			if (s.position == s.clip_position) continue;
			state ns = s;
			ns.clip = clip_t::FULL;
			if (ns.position > ns.clip_position) swap(ns.position, ns.clip_position);
			assert(ns.position < ns.clip_position);
			int lo = s.perm & (p8(ns.position) - 1);
			int mid = (s.perm >> (3 * ns.position)) & (p8(ns.clip_position - ns.position) - 1);
			int hi = s.perm >> (3 * ns.clip_position);

			ns.clip_position = N - (ns.clip_position - ns.position);
			ns.perm = lo | (hi << (3 * ns.position)) | (mid << (3 * ns.clip_position));

			check_state(ns, d + T_RELEASE + T_CUT, 3, si);
		} while (false);

		do { // PASTE
			if (s.clip != clip_t::FULL) continue;
			state ns = s;
			assert(0 <= ns.position && ns.position <= ns.clip_position);
			int lo = s.perm & (p8(ns.position) - 1);
			int hi = (s.perm >> (3 * ns.position)) & (p8(ns.clip_position - ns.position) - 1);
			int mid = s.perm >> (3 * ns.clip_position);

			ns.perm = lo | (mid << (3 * ns.position)) | (hi << (3 * (N - (ns.clip_position - ns.position))));
			ns.position = N - (ns.clip_position - ns.position);
			ns.clip = clip_t::EMPTY;
			ns.clip_position = 0;
			check_state(ns, d + T_PASTE, 4, si);
		} while (false);
	}

	return 0;
}
