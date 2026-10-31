#include <bits/stdc++.h>
#include<bits/extc++.h>

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
using namespace std;

int main() {
    //int N = 2020;
    int N = int(3e7);
    //vector<int> res = {0,3,6}; res.reserve(N);
    vector<int> res = {16,11,15,0,1,7}; res.reserve(N);
    vector<int> last_time(N+20, -1);
    for (int i = 0; i < int(res.size()); i++) {
        last_time[res[i]] = i;
    }
    int last = 0;
    for (int i = int(res.size())+1; i < N; i++) {
        //res.push_back(last);
        auto& a = last_time[last];
        int v = (a == -1 ? 0 : i-1-a);
        a = i-1;
        last = v;
    }
    cout << last << '\n';
}
