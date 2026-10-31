#include <bits/stdc++.h>
using namespace std;

namespace atcoder {

namespace internal {

// @param n `0 <= n`
// @return minimum non-negative `x` s.t. `n <= 2**x`
int ceil_pow2(int n) {
    int x = 0;
    while ((1U << x) < (unsigned int)(n)) x++;
    return x;
}

// @param n `1 <= n`
// @return minimum non-negative `x` s.t. `(n & (1 << x)) != 0`
constexpr int bsf_constexpr(unsigned int n) {
    int x = 0;
    while (!(n & (1 << x))) x++;
    return x;
}

// @param n `1 <= n`
// @return minimum non-negative `x` s.t. `(n & (1 << x)) != 0`
int bsf(unsigned int n) {
#ifdef _MSC_VER
    unsigned long index;
    _BitScanForward(&index, n);
    return index;
#else
    return __builtin_ctz(n);
#endif
}

}  // namespace internal

}  // namespace atcoder

namespace atcoder {

template <class S, S (*op)(S, S), S (*e)()> struct segtree {
  public:
    segtree() : segtree(0) {}
    explicit segtree(int n) : segtree(std::vector<S>(n, e())) {}
    explicit segtree(const std::vector<S>& v) : _n(int(v.size())) {
        log = internal::ceil_pow2(_n);
        size = 1 << log;
        d = std::vector<S>(2 * size, e());
        for (int i = 0; i < _n; i++) d[size + i] = v[i];
        for (int i = size - 1; i >= 1; i--) {
            update(i);
        }
    }

    void set(int p, S x) {
        assert(0 <= p && p < _n);
        p += size;
        d[p] = x;
        for (int i = 1; i <= log; i++) update(p >> i);
    }

    S get(int p) const {
        assert(0 <= p && p < _n);
        return d[p + size];
    }

    S prod(int l, int r) const {
        assert(0 <= l && l <= r && r <= _n);
        S sml = e(), smr = e();
        l += size;
        r += size;

        while (l < r) {
            if (l & 1) sml = op(sml, d[l++]);
            if (r & 1) smr = op(d[--r], smr);
            l >>= 1;
            r >>= 1;
        }
        return op(sml, smr);
    }

    S all_prod() const { return d[1]; }

    template <bool (*f)(S)> int max_right(int l) const {
        return max_right(l, [](S x) { return f(x); });
    }
    template <class F> int max_right(int l, F f) const {
        assert(0 <= l && l <= _n);
        assert(f(e()));
        if (l == _n) return _n;
        l += size;
        S sm = e();
        do {
            while (l % 2 == 0) l >>= 1;
            if (!f(op(sm, d[l]))) {
                while (l < size) {
                    l = (2 * l);
                    if (f(op(sm, d[l]))) {
                        sm = op(sm, d[l]);
                        l++;
                    }
                }
                return l - size;
            }
            sm = op(sm, d[l]);
            l++;
        } while ((l & -l) != l);
        return _n;
    }

    template <bool (*f)(S)> int min_left(int r) const {
        return min_left(r, [](S x) { return f(x); });
    }
    template <class F> int min_left(int r, F f) const {
        assert(0 <= r && r <= _n);
        assert(f(e()));
        if (r == 0) return 0;
        r += size;
        S sm = e();
        do {
            r--;
            while (r > 1 && (r % 2)) r >>= 1;
            if (!f(op(d[r], sm))) {
                while (r < size) {
                    r = (2 * r + 1);
                    if (f(op(d[r], sm))) {
                        sm = op(d[r], sm);
                        r--;
                    }
                }
                return r + 1 - size;
            }
            sm = op(d[r], sm);
        } while ((r & -r) != r);
        return 0;
    }

  private:
    int _n, size, log;
    std::vector<S> d;

    void update(int k) { d[k] = op(d[2 * k], d[2 * k + 1]); }
};

}  // namespace atcoder

namespace atcoder {

template <class S,
          S (*op)(S, S),
          S (*e)(),
          class F,
          S (*mapping)(F, S),
          F (*composition)(F, F),
          F (*id)()>
struct lazy_segtree {
  public:
    lazy_segtree() : lazy_segtree(0) {}
    explicit lazy_segtree(int n) : lazy_segtree(std::vector<S>(n, e())) {}
    explicit lazy_segtree(const std::vector<S>& v) : _n(int(v.size())) {
        log = internal::ceil_pow2(_n);
        size = 1 << log;
        d = std::vector<S>(2 * size, e());
        lz = std::vector<F>(size, id());
        for (int i = 0; i < _n; i++) d[size + i] = v[i];
        for (int i = size - 1; i >= 1; i--) {
            update(i);
        }
    }

    void set(int p, S x) {
        assert(0 <= p && p < _n);
        p += size;
        for (int i = log; i >= 1; i--) push(p >> i);
        d[p] = x;
        for (int i = 1; i <= log; i++) update(p >> i);
    }

    S get(int p) {
        assert(0 <= p && p < _n);
        p += size;
        for (int i = log; i >= 1; i--) push(p >> i);
        return d[p];
    }

    S prod(int l, int r) {
        assert(0 <= l && l <= r && r <= _n);
        if (l == r) return e();

        l += size;
        r += size;

        for (int i = log; i >= 1; i--) {
            if (((l >> i) << i) != l) push(l >> i);
            if (((r >> i) << i) != r) push((r - 1) >> i);
        }

        S sml = e(), smr = e();
        while (l < r) {
            if (l & 1) sml = op(sml, d[l++]);
            if (r & 1) smr = op(d[--r], smr);
            l >>= 1;
            r >>= 1;
        }

        return op(sml, smr);
    }

    S all_prod() { return d[1]; }

    void apply(int p, F f) {
        assert(0 <= p && p < _n);
        p += size;
        for (int i = log; i >= 1; i--) push(p >> i);
        d[p] = mapping(f, d[p]);
        for (int i = 1; i <= log; i++) update(p >> i);
    }
    void apply(int l, int r, F f) {
        assert(0 <= l && l <= r && r <= _n);
        if (l == r) return;

        l += size;
        r += size;

        for (int i = log; i >= 1; i--) {
            if (((l >> i) << i) != l) push(l >> i);
            if (((r >> i) << i) != r) push((r - 1) >> i);
        }

        {
            int l2 = l, r2 = r;
            while (l < r) {
                if (l & 1) all_apply(l++, f);
                if (r & 1) all_apply(--r, f);
                l >>= 1;
                r >>= 1;
            }
            l = l2;
            r = r2;
        }

        for (int i = 1; i <= log; i++) {
            if (((l >> i) << i) != l) update(l >> i);
            if (((r >> i) << i) != r) update((r - 1) >> i);
        }
    }

    template <bool (*g)(S)> int max_right(int l) {
        return max_right(l, [](S x) { return g(x); });
    }
    template <class G> int max_right(int l, G g) {
        assert(0 <= l && l <= _n);
        assert(g(e()));
        if (l == _n) return _n;
        l += size;
        for (int i = log; i >= 1; i--) push(l >> i);
        S sm = e();
        do {
            while (l % 2 == 0) l >>= 1;
            if (!g(op(sm, d[l]))) {
                while (l < size) {
                    push(l);
                    l = (2 * l);
                    if (g(op(sm, d[l]))) {
                        sm = op(sm, d[l]);
                        l++;
                    }
                }
                return l - size;
            }
            sm = op(sm, d[l]);
            l++;
        } while ((l & -l) != l);
        return _n;
    }

    template <bool (*g)(S)> int min_left(int r) {
        return min_left(r, [](S x) { return g(x); });
    }
    template <class G> int min_left(int r, G g) {
        assert(0 <= r && r <= _n);
        assert(g(e()));
        if (r == 0) return 0;
        r += size;
        for (int i = log; i >= 1; i--) push((r - 1) >> i);
        S sm = e();
        do {
            r--;
            while (r > 1 && (r % 2)) r >>= 1;
            if (!g(op(d[r], sm))) {
                while (r < size) {
                    push(r);
                    r = (2 * r + 1);
                    if (g(op(d[r], sm))) {
                        sm = op(d[r], sm);
                        r--;
                    }
                }
                return r + 1 - size;
            }
            sm = op(d[r], sm);
        } while ((r & -r) != r);
        return 0;
    }

  private:
    int _n, size, log;
    std::vector<S> d;
    std::vector<F> lz;

    void update(int k) { d[k] = op(d[2 * k], d[2 * k + 1]); }
    void all_apply(int k, F f) {
        d[k] = mapping(f, d[k]);
        if (k < size) lz[k] = composition(f, lz[k]);
    }
    void push(int k) {
        all_apply(2 * k, lz[k]);
        all_apply(2 * k + 1, lz[k]);
        lz[k] = id();
    }
};

}  // namespace atcoder

using ll = int64_t;

ll MAX = ll(1e14);

namespace lsegtree {
    struct S {
        ll v;
        int idx;
    };

    S op(S a, S b){
        return tie(a.v, a.idx) < tie(b.v, b.idx) ? a : b;
    }

    S e(){
        return S{MAX, -1};
    }

    struct F {
        ll add;
    };

    S mapping(F f, S x){
        return {x.v + f.add, x.idx};
    }

    F composition(F f, F g){
        return F{f.add + g.add};
    }
    F id(){
        return F{0};
    }

    using segtree = atcoder::lazy_segtree<S, op, e, F, mapping, composition, id>;
}

namespace rseg {
    struct S {
        ll v;
        int idx;
    };

    S op(S a, S b){
        return tie(a.v, a.idx) < tie(b.v, b.idx) ? a : b;
    }

    S e(){
        return {MAX, -1};
    }

    using segtree = atcoder::segtree<S, op, e>;
}

namespace sumseg {
    ll op (ll a, ll b) {
        return a + b;
    }
    ll e (){
        return 0;
    }

    using segtree = atcoder::segtree<ll, op, e>;
}

int main(){
    ios_base::sync_with_stdio(false), cin.tie(nullptr);
    int N;
    cin >> N;
    vector<int> l(N), r(N);
    for(int i = 0; i < N; i++){
        cin >> l[i] >> r[i];
        if(i) assert(r[i] - l[i] >= r[i-1] - l[i-1]);
    }
    vector<int> ival_ord(N);
    for(int i = 0; i < N; i++){
        ival_ord[i] = i;
    }
    sort(ival_ord.begin(), ival_ord.end(), [&](int x, int y){
        return tie(l[x], r[x]) < tie(l[y], r[y]);
    });
    vector<int> pos(N);
    for(int i = 0; i < N; i++){
        pos[ival_ord[i]] = i;
    }
    set<int> cur_intervals;

    {
        vector<int> which;
        for(int idx = 0; idx < N; idx++){
            int i = ival_ord[idx];
            while(!which.empty() && r[i] <= r[which.back()]){
                which.pop_back();
            }
            if(which.empty() || l[i] > l[which.back()]){
                which.push_back(i);
            }
        }
        for(int x : which){
            cur_intervals.insert(pos[x]);
        }
    }
    lsegtree::segtree val_segtree(N);
    rseg::segtree rseg(N);
    for(int i = 0; i < N; i++){
        rseg.set(i, {r[ival_ord[i]], i});
    }
    for(int idx : cur_intervals){
        int i = ival_ord[idx];
        val_segtree.set(idx, {r[i] - l[i], i});
        rseg.set(idx, rseg::e());
    }
    vector<int> ans;

    vector<int> cc;
    for(int i = 0; i < N; i++){
        cc.push_back(l[i]);
        cc.push_back(r[i]);
    }
    sort(cc.begin(), cc.end());
    cc.resize(unique(cc.begin(), cc.end()) - cc.begin());
    sumseg::segtree filled_sum(cc.size() - 1);
    set<int> filled_which;
    for(int i = 0; i < cc.size() - 1; i++){
        filled_which.insert(i);
    }

    auto get_cc = [&](int x) -> int {
        return lower_bound(cc.begin(), cc.end(), x) - cc.begin();
    };

    while(true){
        auto q = val_segtree.all_prod();
        for(int i = 0; i < N; i++){
            auto [a, b] = val_segtree.get(i);
            // cerr << a << ',' << b << ' ';
        }
        // cerr << '\n';
        if(q.idx < 0){
            assert(ans.size() == N);
            break;
        }
        int i = pos[q.idx];
        assert(cur_intervals.count(i));
        // cerr << "best " << i << '\n';
        int pl = -1;
        int nr = 2e9;
        {
            auto it = cur_intervals.find(i);
            if(it != cur_intervals.begin()){
                pl = l[ival_ord[*prev(it)]];
            }
            if(next(it) != cur_intervals.end()){
                nr = r[ival_ord[*next(it)]];
            }
        }
        // cerr << pl << ' ' << nr << '\n';
        cur_intervals.erase(i);
        val_segtree.set(i, lsegtree::e());
        // remove this interval from pareto and add new ones
        while(true){
            int s = -1;
            int e = N;
            while (s + 1 < e){
                int m = (s + e) / 2;
                if(l[ival_ord[m]] > pl){
                    e = m;
                } else {
                    s = m;
                }
            }
            // cerr << "e " << e << '\n';
            int pareto_idx = rseg.prod(e, N).idx;
            if(pareto_idx == -1){
                break;
            }
            // cerr << rseg.prod(e, N).v << '\n';
            // cerr << pareto_idx << '\n';
            if(r[ival_ord[pareto_idx]] >= nr){
                break;
            }
            // found new pareto interval;
            rseg.set(pareto_idx, rseg::e());
            ll cost = (r[ival_ord[pareto_idx]] - l[ival_ord[pareto_idx]]) - filled_sum.prod(get_cc(l[ival_ord[pareto_idx]]), get_cc(r[ival_ord[pareto_idx]]));
            val_segtree.set(pareto_idx, {cost, ival_ord[pareto_idx]});
            cur_intervals.insert(pareto_idx);
            // cerr << "adding " << pareto_idx << " " << cost << '\n';
            pl = l[ival_ord[pareto_idx]];
        }
        // deal with removal of interval
        ans.push_back(ival_ord[i]);
        int L = l[ival_ord[i]];
        int R = r[ival_ord[i]];

        int Lcc = get_cc(L);
        int Rcc = get_cc(R);
        while(true){
            auto it = filled_which.lower_bound(Lcc);
            if(it == filled_which.end() || *it >= Rcc) break;
            int pos = *it;
            filled_sum.set(pos, cc[pos + 1] - cc[pos]);

            int POS = cc[pos];
            int set_end = -1;
            {
                int s = -1;
                int e = N;
                while(s + 1 < e){
                    int m = (s + e) / 2;
                    if(l[ival_ord[m]] > POS){
                        e = m;
                    } else {
                        s = m;
                    }
                }
                set_end = e;
            }
            {
                int s2 = -1;
                int e2 = set_end;
                while(s2 + 1 < e2){
                    int m2 = (s2 + e2) / 2;
                    auto x = cur_intervals.lower_bound(m2);
                    if(x == cur_intervals.end() || r[ival_ord[*x]] > POS){
                        e2 = m2;
                    } else {
                        s2 = m2;
                    }
                }

                val_segtree.apply(e2, set_end, {-(cc[pos + 1] - cc[pos])} );
                // cerr << "decrement " << e2 << ' ' << set_end << '\n';
            }
            filled_which.erase(it);
        }
    }
    for(int i = 0; i < N; i++){
        cout << (ans[i]+1) << " \n"[i == N-1];
    }
}
