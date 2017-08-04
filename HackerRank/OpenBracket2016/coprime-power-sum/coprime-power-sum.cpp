#define NDEBUG
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll MOD = ll(1e9 + 7);

struct num {
    ll v;
    num() : v() {}
    num(ll val) : v(val % MOD) { assert(v >= 0); }
};

inline num operator * (const num &a, const num &b) { return num(a.v * b.v); }
inline num operator + (const num &a, const num &b) { return num(a.v + b.v); }
inline num operator - (const num &a, const num &b) { return num(a.v - b.v + MOD); }
inline num operator - (const num &a) { return num(MOD - a.v); }
inline num& operator += (num &a, const num &b) { a.v += b.v; a.v %= MOD; return a; }
inline num& operator *= (num &a, const num &b) { a.v *= b.v; a.v %= MOD; return a; }
inline ostream& operator << (ostream &o, const num &n) { return o << n.v; }

inline bool operator == (const num &a, const num &b) { return a.v == b.v; }
inline bool operator != (const num &a, const num &b) { return a.v != b.v; }

ll inv(ll a, ll m) { return (a == 1) ? 1 : m - inv(m % a, a) * m / a; }

num pmod(num a, ll b) {
    num r = 1;
    while(b) {
        if(b&1) r *= a;
        a *= a; b /= 2;
    }
    return r;
}

num powersum(num n, ll k) {
    if(n == 0) return 0;
    if(k == 0) {
        return n;
    } else if(k == 1) {
        return 500000004 //inv(2, MOD)
            * n
            * (n + 1);
    } else if(k == 2) {
        return 166666668 //inv(6, MOD)
            * n * (n + 1) * (2 * n + 1);
    } else if(k == 3) {
        return 250000002 //inv(4, MOD)
            * n * n * (n + 1) * (n + 1);
    } else if(k == 4) {
        return 233333335 //inv(30, MOD)
            * n
            * (n + 1)
            * (2 * n + 1)
            * (3 * n * n + 3 * n - 1);
    } else if(k == 5) {
        return  83333334 //inv(12, MOD)
            * n
            * n
            * (n + 1)
            * (n + 1)
            * (2 * n * n + 2 * n - 1);
    } else if(k == 6) {
        return  23809524 //inv(42, MOD)
            * n
            * (n + 1)
            * (2 * n + 1)
            * (((3 * n + 6) * n * n * n - 3 * n + 1));
    } else if(k == 7) {
        return  41666667 //inv(24, MOD)
            * n * n
            * (n+1) * (n+1)
            * ((((3 * n + 6) * n - 1) * n - 4) * n + 2);
    } else if(k == 8) {
        return 411111114 //inv(90, MOD)
            * n
            * (n+1)
            * (2 * n + 1)
            * ((((((5 * n + 15) * n + 5) * n - 15) * n - 1) * n + 9) * n - 3);
    } else if(k == 9) {
        return 850000006 //inv(20, MOD)
            * n * n
            * (n+1) * (n+1)
            * (n * n + n - 1)
            * ((((2 * n + 4) * n - 1) * n - 3) * n + 3);
    } else if(k == 10) {
        return 469696973 //inv(66, MOD)
            * n
            * (n+1)
            * (2 * n + 1)
            * (n * n + n - 1)
            * ((((((3*n+9)*n + 2)*n - 11)*n + 3)*n + 10)*n - 5);
    } else { assert(false); }
    return 0;
}

num slowpowersum(ll n, int k) {
    num res = 0;
    for(ll i = 1; i <= n; i++) {
        res += pmod(i, k);
    }
    return res;
}

const int MAXN = 100;
int N, K;
ll M;
ll S[MAXN];

const int MAXR = 2e6;

ll R;
num dp1[MAXR]; // powersum(N / i)
num dp2[MAXR]; // powersum(i)

num go_map() {
    assert(N <= 50);
    assert(N < MAXN);
    assert(K <= 10);

    unordered_map<ll, num> mp[2];
    mp[0][M] = 1;

    for(int z = 0; z < N; z++) {
        int d = z & 1;
        mp[!d] = mp[d];

        ll v = S[z];
        num f = -pmod(v, K);

        for(auto it : mp[d]) {
            mp[!d][it.first / v] += it.second * f;
        }
    }
    num res = 0;
    for(auto it : mp[N & 1]) {
        res += it.second * powersum(it.first, K);
    }
    return res;
}

num go() {
    assert(N <= 50);
    assert(N < MAXN);
    assert(K <= 10);

    memset(dp1, 0, sizeof(dp1));
    memset(dp2, 0, sizeof(dp2));

    dp1[1] = 1;

    // R is ceil(sqrt(M))
    R = 0; while(R * R < M) R++;

    assert(M / R <= R);

    assert(R < MAXR);

    for(int z = 0; z < N; z++) {
        ll v = S[z];
        num f = -pmod(v, K);
        for(ll i = v; i <= R; i++) {
            dp2[i / v] += dp2[i] * f;
        }
        for(ll i = R; i >= 1; i--) {
            if(v >= R || i * v >= R) {
                dp2[M / i / v] += dp1[i] * f;
            } else {
                dp1[i * v] += dp1[i] * f;
            }
        }
    }

    num res = 0;
    for(ll i = 1; i <= R; i++) {
        //cerr << dp1[i] << ' ' << M/i << '\n';
        res += dp1[i] * powersum(M / i, K);
    }
    num v = 0;
    for(ll i = 1; i <= R; i++) {
        //cerr << dp2[i] << ' ' << i << '\n';
        v += pmod(i, K);
        res += dp2[i] * v;
    }
    return res;
}

num go_exp() {
    num res = 0;
    for(ll m = 0; m < (1ll << N); m++) {
        ll d = M;
        num v = 1;
        for(int i = 0; i < N; i++) {
            if(m & (1ll << i)) {
                d /= S[i];
                v *= -pmod(S[i], K);
            }
        }
        res += v * powersum(d, K);
    }
    return res;
}

num go_slow() {
    num res = 0;
    for (ll i = 1; i <= M; i++) {
        for (int z = 0; z < N; z++) {
            if (i % S[z] == 0) goto bad;
        }
        res += pmod(i, K);
        bad:;
    }
    return res;
}

void test_powersum() {
    for(int k = 0; k <= 10; k++) {
        for(ll n = 1; n < 10000000; n *= 21, n /= 10) {
            num fast = powersum(n, k);
            num slow = slowpowersum(n, k);
            cerr << "testing k=" << k << " n=" << n << '\n';
            cerr << "fast=" << fast << " slow=" << slow << '\n';
            if(fast != slow) {
                assert(fast == slow);
            }
        }
    }
    cerr << "tests done!\n";
}

void test() {
    const ll primes[] = {
        2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101,103,107,109,113,127,131,137,139,149,151,157,163,167,173,179,181,191,193,197,199,211,223,227,229,233,239,241,251,257,263,269,271,277,281,283,293,307,311,313,317,331,337,347,349,353,359,367,373,379,383,389,397,401,409,419,421,431,433,439,443,449,457,461,463,467,479,487,491,499,503,509,521,523,541
    };
    for(int i = 0; i < 100; i++) {
        S[i] = primes[i];
    }
    for(M = 1; M <= 1000000; M *= 3) {
        for(N = 1; N < 15; N++) {
            for(K = 0; K <= 10; K++) {
                cerr << N << ' ' << K << ' ' << M << '\n';
                ll v = go().v;
                fprintf( stderr, "go(%d, %d, %lld) = %lld\n", N, K, M, v);
                if(M * N <= ll(1e6)) {
                    ll s = go_slow().v;
                    fprintf( stderr, "go_slow(%d, %d, %lld) = %lld\n", N, K, M, s);
                    assert(v == s);
                }
                if(N <= 15) {
                    ll e = go_exp().v;
                    fprintf( stderr, "go_exp(%d, %d, %lld) = %lld\n", N, K, M, e);
                    assert(v == e);
                }
            }
        }
    }
    cerr << "diff done!\n";
}

int main() {
    //test_powersum();
    //test();
    int Q; scanf("%d", &Q);
    for(int q = 0; q < Q; q++) {
        scanf("%d %d %lld", &N, &K, &M);
        assert(N <= 50);
        assert(N < MAXN);
        assert(K <= 10);
        for(int i = 0; i < N; i++) scanf("%lld", &S[i]);
        num v = go_map();
        printf("%lld\n", v.v);
    }
    return 0;
}
