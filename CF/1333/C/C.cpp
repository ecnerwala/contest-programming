#include<bits/stdc++.h>
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

template <typename T> T& setmax(T& a, const T& b) { if (b > a) a = b; return a; }
template <typename T> T& setmin(T& a, const T& b) { if (b < a) a = b; return a; }

int scan_int(istream& i) { int a; i >> a; return a; }

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N = scan_int(cin);
	int64_t pref = 0;
	hash_map<int64_t, int> lastInd;
	lastInd[0] = 1;
	int64_t ans = 0;
	int ind = 0;
	for (int i = 1; i <= N; i++) ans += i - setmax(ind, exchange(lastInd[pref += scan_int(cin)], i+1));
	cout << ans << '\n';

	return 0;
}
