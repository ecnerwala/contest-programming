#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <random>
#include <set>
#include <vector>
using namespace std;

// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0200r0.html
template<class Fun> class y_combinator_result {
    Fun fun_;
public:
    template<class T> explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}
    template<class ...Args> decltype(auto) operator()(Args &&...args) { return fun_(std::ref(*this), std::forward<Args>(args)...); }
};
template<class Fun> decltype(auto) y_combinator(Fun &&fun) { return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun)); }


template<typename A, typename B> ostream& operator<<(ostream &os, const pair<A, B> &p) { return os << '(' << p.first << ", " << p.second << ')'; }
template<typename T_container, typename T = typename enable_if<!is_same<T_container, string>::value, typename T_container::value_type>::type> ostream& operator<<(ostream &os, const T_container &v) { os << '{'; string sep; for (const T &x : v) os << sep << x, sep = ", "; return os << '}'; }

void dbg_out() { cerr << endl; }
template<typename Head, typename... Tail> void dbg_out(Head H, Tail... T) { cerr << ' ' << H; dbg_out(T...); }
#ifdef NEAL_DEBUG
#define dbg(...) cerr << '[' << __FILE__ << ':' << __LINE__ << "] (" << #__VA_ARGS__ << "):", dbg_out(__VA_ARGS__)
#else
#define dbg(...)
#endif

struct segment {
    int minimum;

    segment(int _minimum = numeric_limits<int>::max()) : minimum(_minimum) {}

    void join(const segment &other) {
        minimum = min(minimum, other.minimum);
    }

    void join(const segment &seg0, const segment &seg1) {
        *this = seg0;
        join(seg1);
    }
};

struct persistent_basic_seg_tree {
    struct node {
        segment seg;
        std::array<int, 2> ch{-1, -1};
    };

    int tree_n = 0;
    vector<node> tree;
    int reserve_size = INT32_MAX;

    persistent_basic_seg_tree(int n = -1, int max_updates = 0) {
        if (n >= 0)
            init(n, max_updates);
    }

    void init(int n, int max_updates = 0) {
        tree_n = 1;

        while (tree_n < n)
            tree_n *= 2;

        reserve_size = INT32_MAX;

        if (max_updates > 0) {
            int tree_height = 32 - __builtin_clz(tree_n);
            reserve_size = 2 * tree_n + max_updates * tree_height;
            tree.reserve(reserve_size);
        }

        tree.assign(2 * tree_n, {});

        for (int i = 1; i < tree_n; i++) {
            tree[i].ch = {2 * i + 0, 2 * i + 1};
        }
    }

    int left(int position) const { return tree[position].ch[0]; }
    int right(int position) const { return tree[position].ch[1]; }
    segment& seg(int position) { return tree[position].seg; }
    const segment& seg(int position) const { return tree[position].seg; }

    // Builds our tree from an array in O(n).
    void build(const vector<segment> &initial) {
        int n = int(initial.size());
        assert(n <= tree_n);

        for (int i = 0; i < n; i++)
            seg(tree_n + i) = initial[i];

        for (int position = tree_n - 1; position > 0; position--)
            seg(position).join(seg(left(position)), seg(right(position)));
    }

    int make_copy(int position) {
        assert(0 <= position && position < int(tree.size()));
        tree.push_back(tree[position]);
        assert(int(tree.size()) <= reserve_size);
        return int(tree.size()) - 1;
    }

    // Directly assigning `tree[position].left = make_copy(left(position))` results in segmentation faults, because the
    // address for `tree[position]` can be computed before calling `make_copy`, which may reallocate `tree`.
    void set_left(int position, int result) { tree[position].ch[0] = result; }
    void set_right(int position, int result) { tree[position].ch[1] = result; }

    template<typename T_range_op>
    int _update_tree(int position, int start, int end, int index, T_range_op &&range_op) {
        position = make_copy(position);

        if (end - start == 1 && start == index) {
            range_op(position, end - start);
            return position;
        }

        if (left(position) < 0 || right(position) < 0)
            return position;

        int mid = (start + end) / 2;

        if (index < mid)
            set_left(position, _update_tree(left(position), start, mid, index, range_op));
        else
            set_right(position, _update_tree(right(position), mid, end, index, range_op));

        seg(position).join(seg(left(position)), seg(right(position)));
        return position;
    }

    int set(int root, int index, const segment &new_seg) {
        assert(root > 0 && 0 <= index && index < tree_n);

        return _update_tree(root, 0, tree_n, index, [&](int position, int) -> void {
            seg(position) = new_seg;
        });
    }
};


const int BITS = 21;

void run_case(int test_case) {
    int N;
    int64_t XA, YA, ZA, XB, YB, ZB, XC, YC, ZC;
    cin >> N >> XA >> YA >> ZA >> XB >> YB >> ZB >> XC >> YC >> ZC;

    vector<int> A(N + 1, 0), B(N + 1, -1), C(N + 1, 0);
    int64_t PA = 0, PB = 0, PC = 0;

    for (int i = 1; i <= N; i++) {
        PA = (PA * XA + YA) % ZA;
        PB = (PB * XB + YB) % ZB;
        PC = (PC * XC + YC) % ZC;
        A[i] = min(i, 1 + int(PA));
        B[i] = max(A[i], i - int(PB));
        C[i] = min(i - 1, int(PC));
    }

    vector<int> SG(N + 1, 0);
    persistent_basic_seg_tree tree(1 << BITS, N + 1);
    vector<segment> initial(1 << BITS, -1);
    initial[0] = 0;
    tree.build(initial);
    vector<int> roots(N + 1, 1);

    for (int k = 1; k <= N; k++) {
        int add = SG[C[k]];
        int low = k - B[k], high = k - A[k];
        int ans = 0;

        int cur = roots[high];
        for (int bit = BITS - 1; bit >= 0; bit--) {
            int d = (add >> bit) & 1;
            int c_d = tree.tree[cur].ch[d];
            int c_nd = tree.tree[cur].ch[!d];
            if (tree.seg(c_d).minimum < low) {
                cur = c_d;
            } else {
                ans |= 1 << bit;
                cur = c_nd;
            }
        }

        SG[k] = ans;
        roots[k] = tree.set(roots[k - 1], SG[k], segment(k));
    }

    vector<int> smallest(N + 1, -1);

    for (int k = N; k > 0; k--)
        smallest[SG[k]] = k;

    vector<int> K2(N + 1, 0);

    for (int k = 1; k <= N; k++)
        K2[k] = smallest[SG[k]];

    int64_t K2_sum = accumulate(K2.begin() + 1, K2.end(), INT64_C(0));
    cout << "Case #" << test_case << ": " << K2_sum << '\n';
}

int main() {
    int tests;
    cin >> tests;

    for (int tc = 1; tc <= tests; tc++) {
        run_case(tc);
        cout << flush;
    }
}
