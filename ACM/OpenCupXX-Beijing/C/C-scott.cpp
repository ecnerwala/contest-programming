#include <bits/stdc++.h>
using namespace std;

#include <cassert>
#include <iostream>

template <int MOD_> struct modnum {
    static constexpr int MOD = MOD_;
    static_assert(MOD_ > 0, "MOD must be positive");

private:
    using ll = long long;

    int v;

    static int minv(int a, int m) {
        a %= m;
        assert(a);
        return a == 1 ? 1 : int(m - ll(minv(m, a)) * ll(m) / a);
    }

public:

    modnum() : v(0) {}
    modnum(ll v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
    explicit operator int() const { return v; }
    friend std::ostream& operator << (std::ostream& out, const modnum& n) { return out << int(n); }
    friend std::istream& operator >> (std::istream& in, modnum& n) { ll v_; in >> v_; n = modnum(v_); return in; }

    friend bool operator == (const modnum& a, const modnum& b) { return a.v == b.v; }
    friend bool operator != (const modnum& a, const modnum& b) { return a.v != b.v; }

    modnum inv() const {
        modnum res;
        res.v = minv(v, MOD);
        return res;
    }
    modnum neg() const {
        modnum res;
        res.v = v ? MOD-v : 0;
        return res;
    }

    modnum operator- () const {
        return neg();
    }
    modnum operator+ () const {
        return modnum(*this);
    }

    modnum& operator ++ () {
        v ++;
        if (v == MOD) v = 0;
        return *this;
    }
    modnum& operator -- () {
        if (v == 0) v = MOD;
        v --;
        return *this;
    }
    modnum& operator += (const modnum& o) {
        v += o.v;
        if (v >= MOD) v -= MOD;
        return *this;
    }
    modnum& operator -= (const modnum& o) {
        v -= o.v;
        if (v < 0) v += MOD;
        return *this;
    }
    modnum& operator *= (const modnum& o) {
        v = int(ll(v) * ll(o.v) % MOD);
        return *this;
    }
    modnum& operator /= (const modnum& o) {
        return *this *= o.inv();
    }

    friend modnum operator ++ (modnum& a, int) { modnum r = a; ++a; return r; }
    friend modnum operator -- (modnum& a, int) { modnum r = a; --a; return r; }
    friend modnum operator + (const modnum& a, const modnum& b) { return modnum(a) += b; }
    friend modnum operator - (const modnum& a, const modnum& b) { return modnum(a) -= b; }
    friend modnum operator * (const modnum& a, const modnum& b) { return modnum(a) *= b; }
    friend modnum operator / (const modnum& a, const modnum& b) { return modnum(a) /= b; }
};

template <typename T> T pow(T a, long long b) {
    assert(b >= 0);
    T r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}

template <typename U, typename V> struct pairnum {
    U u;
    V v;

    pairnum() : u(0), v(0) {}
    pairnum(long long val) : u(val), v(val) {}
    pairnum(const U& u_, const V& v_) : u(u_), v(v_) {}

    friend std::ostream& operator << (std::ostream& out, const pairnum& n) { return out << '(' << n.u << ',' << ' ' << n.v << ')'; }
    friend std::istream& operator >> (std::istream& in, pairnum& n) { long long val; in >> val; n = pairnum(val); return in; }

    friend bool operator == (const pairnum& a, const pairnum& b) { return a.u == b.u && a.v == b.v; }
    friend bool operator != (const pairnum& a, const pairnum& b) { return a.u != b.u || a.v != b.v; }

    pairnum inv() const {
        return pairnum(u.inv(), v.inv());
    }
    pairnum neg() const {
        return pairnum(u.neg(), v.neg());
    }
    pairnum operator- () const {
        return pairnum(-u, -v);
    }
    pairnum operator+ () const {
        return pairnum(+u, +v);
    }

    pairnum& operator += (const pairnum& o) {
        u += o.u;
        v += o.v;
        return *this;
    }
    pairnum& operator -= (const pairnum& o) {
        u -= o.u;
        v -= o.v;
        return *this;
    }
    pairnum& operator *= (const pairnum& o) {
        u *= o.u;
        v *= o.v;
        return *this;
    }
    pairnum& operator /= (const pairnum& o) {
        u /= o.u;
        v /= o.v;
        return *this;
    }

    friend pairnum operator + (const pairnum& a, const pairnum& b) { return pairnum(a) += b; }
    friend pairnum operator - (const pairnum& a, const pairnum& b) { return pairnum(a) -= b; }
    friend pairnum operator * (const pairnum& a, const pairnum& b) { return pairnum(a) *= b; }
    friend pairnum operator / (const pairnum& a, const pairnum& b) { return pairnum(a) /= b; }
};

using ll = long long;
using num = pairnum<modnum<int(1e9)+7>, modnum<int(1e9)+9> >;

ll key(num x){
    return (ll(int(x.u)) << 32) ^ ll(int(x.v));
}

const int MAXN = 5010;
int n;
ll hash_val[MAXN][MAXN];
short nv[MAXN][MAXN];
bool fv[MAXN][MAXN];
int sf[MAXN];
num stuff[MAXN];
int sstuff[MAXN];
int mnloc[MAXN];
int nplace[MAXN];
vector <int> event[MAXN];

inline bool cmp (int x, int y)
{
    int mlen = min (n - x, n - y);
    if (hash_val[x][mlen] == hash_val[y][mlen]) return x < y;
    int lo = 0, hi = mlen - 1;
    while (lo < hi)
    {
        int mid = (lo + hi + 1) / 2;
        if (hash_val[x][mid] == hash_val[y][mid])
            lo = mid;
        else
            hi = mid - 1;
    }
    return sstuff[x+lo] < sstuff[y+lo];
}

int mbest (int x, int y)
{
    int mlen = min (n - x, n - y);
    if (hash_val[x][mlen] == hash_val[y][mlen]) return mlen;
    int lo = 0, hi = mlen - 1;
    while (lo < hi)
    {
        int mid = (lo + hi + 1) / 2;
        if (hash_val[x][mid] == hash_val[y][mid])
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

void solve(int t){
    num A(434535352, 34938228);
    cin >> n;
    ll X, Y, Z;
    cin >> X >> Y >> Z;
    for(int i = 0; i < n; i++){
        int x;
        cin >> x;
        stuff[i] = num(x);
        sstuff[i] = x;
    }
    vector<ll> dp(n+1, 1e18);
    dp[0] = 0;
    vector<num> pref;
    vector<num> pA;
    pref.push_back(num(0));
    pA.push_back(num(1));

    for(int i = 0; i < n; i++){
        pA.push_back(pA.back() * A);
        pref.push_back(pref.back() * A + stuff[i]);
    }
    for(int i = 0; i <= n; i++){
        for(int j = 0; i + j <= n; j++){
            hash_val[i][j] = key(pref[i+j] - pref[i] * pA[j]);
        }
    }

    for (int i = 0; i < n; i++)
        sf[i] = i;
    sort (sf, sf + n, cmp);
    for (int i = 0; i < MAXN; i++)
        event[i].clear();
    for (int i = 0; i < n - 1; i++)
    {
        mnloc[i] = mbest (sf[i], sf[i+1]);
        event[mnloc[i]].push_back (i);
    }


    for (int len = n; len >= 1; len--)
    {
        for (int x : event[len])
        {
            //cout << len << " " << x << endl;
            int clo = x, chi = x + 1;
            while (clo > 0 && mnloc[clo-1] >= len)
                clo--;
            while (chi < n - 1 && mnloc[chi] > len)
                chi++;
            //cout << x << " " << clo << " " << chi << "\n";
            sort (sf + clo, sf + chi + 1);

            /*int lnum = x - clo + 1;
            int rnum = chi - x;
            int lstart = clo;
            int rstart = x + 1;

            int lcur = 0, rcur = 0;
            while (lcur < lnum && rcur < rnum)
            {
                if (sf[lstart+lcur] < sf[rstart+rcur])
                {
                    nplace[lcur+rcur] = sf[lstart+lcur];
                    lcur++;
                }
                else
                {
                    nplace[lcur+rcur] = sf[rstart+rcur];
                    rcur++;
                }
            }
            while (lcur < lnum)
            {
                nplace[lcur+rcur] = sf[lstart+lcur];
                lcur++;
            }
            while (rcur < rnum)
            {
                nplace[lcur+rcur] = sf[rstart+rcur];
                rcur++;
            }
            for (int i = 0; i < lcur + rcur; i++)
                sf[lstart+i] = nplace[i];*/
        }

        int cloc = 0;
        cout << len << "\n";
        while (cloc < n)
        {
            int cend = cloc;
            while (cend < n - 1 && mnloc[cend] >= len)
                cend++;
            cend++;

            int floc = cloc;
            for (int nloc = cloc; nloc < cend; nloc++)
            {
                int nind = sf[nloc];
                fv[nind][len] = (sf[cloc] <= sf[nloc] - len);
                while (sf[floc] + len <= sf[nloc])
                {
                    nv[len][sf[floc]] = sf[nloc];
                    floc++;
                }
            }
            for (int i = floc; i < cend; i++)
                nv[len][sf[i]] = MAXN;
            //cout << cloc << " " << cend << "\n";
            cloc = cend;
        }
    }

    for(int i = 0; i < n; i++){
        dp[i+1] = min(dp[i+1], dp[i] + X);
        for(int len = min (n - i, i); len >= 0; len--){
            if (!fv[i][len]) continue;
            int cur = i;
            int saved = 0;
            while(cur < n){
                saved++;
                int nval = nv[len][cur];
                cur = cur + len;
                ll ntot = dp[i] + Y + Z * saved + X * (cur - i - len * saved);
                if (dp[cur] + Y <= ntot) break;
                dp[cur] = min(dp[cur], ntot);
                cur = nval;
            }
        }
    }
    cout << "Case #" << t << ": ";
    cout << dp[n] << '\n';
}


int main(){
    ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    int T;
    cin >> T;
    for(int t = 1; t <= T; t++) solve(t);
}
