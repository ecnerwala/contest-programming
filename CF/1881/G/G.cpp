#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>

using namespace std;

// Node structure to hold segment information.
struct Node {
    char first, second, second_last, last;
    bool is_beautiful;
    int lazy;

    Node() : first(0), second(0), second_last(0), last(0), is_beautiful(true), lazy(0) {}
};

class SegmentTree {
private:
    string s;
    vector<Node> tree;

    // Internal function to combine child nodes.
    Node combine(const Node& left, const Node& right) {
        if (!left.first) return right;
        if (!right.first) return left;
        assert(left.first && left.last);
        assert(right.first && right.last);
        Node result;
        result.first = left.first;
        result.second = left.second ? left.second : right.first;
        result.second_last = right.second_last ? right.second_last : left.last;
        result.last = right.last;

        result.is_beautiful = left.is_beautiful && right.is_beautiful &&
                              (left.last != right.first) &&
                              (left.second_last ? left.second_last != right.first : true) &&
                              (right.second ? left.last != right.second : true);

        return result;
    }

    // Internal function to apply a lazy update.
    void apply_lazy(int node, int start, int end) {
        if (tree[node].lazy) {
            // Update the current node's characters.
            tree[node].first = (tree[node].first - 'a' + tree[node].lazy) % 26 + 'a';
            tree[node].second = tree[node].second ? (tree[node].second - 'a' + tree[node].lazy) % 26 + 'a' : 0;
            tree[node].second_last = tree[node].second_last ? (tree[node].second_last - 'a' + tree[node].lazy) % 26 + 'a' : 0;
            tree[node].last = (tree[node].last - 'a' + tree[node].lazy) % 26 + 'a';

            if (start != end) {
                // Propagate the laziness to the child nodes.
                tree[2 * node + 1].lazy += tree[node].lazy;
                tree[2 * node + 1].lazy %= 26;
                tree[2 * node + 2].lazy += tree[node].lazy;
                tree[2 * node + 2].lazy %= 26;
            }

            tree[node].lazy = 0;  // Clear the lazy value for the current node.
        }
    }

    // Internal function to build the segment tree.
    void buildTree(int node, int start, int end) {
        if (start == end) {
            // Leaf node will have a single character.
            tree[node].first = tree[node].last = s[start];
            tree[node].is_beautiful = true;  // Single character is always beautiful.
        } else {
            int mid = (start + end) / 2;
            // Recur for the left child.
            buildTree(2 * node + 1, start, mid);
            // Recur for the right child.
            buildTree(2 * node + 2, mid + 1, end);
            // Internal node will combine information of its children.
            tree[node] = combine(tree[2 * node + 1], tree[2 * node + 2]);
        }
    }

    // Internal function to update a range of values.
    void updateRange(int node, int start, int end, int l, int r, int val) {
        apply_lazy(node, start, end);

        if (start > end || start > r || end < l) {
            // Out of range.
            return;
        }

        if (start >= l && end <= r) {
            // Segment is fully within range. Update the characters in this segment.
            tree[node].lazy = val;
            apply_lazy(node, start, end);
            return;
        }

        // Partially in the range. We will update the children and combine their results.
        int mid = (start + end) / 2;
        updateRange(2 * node + 1, start, mid, l, r, val);
        updateRange(2 * node + 2, mid + 1, end, l, r, val);
        tree[node] = combine(tree[2 * node + 1], tree[2 * node + 2]);
    }

    // Internal function to query the segment tree.
    Node queryRange(int node, int start, int end, int l, int r) {
        if (start > end || start > r || end < l) {
            // Out of range, return true (since we are using AND in combination, it acts as a neutral element).
            return Node();
        }

        apply_lazy(node, start, end);

        if (start >= l && end <= r) {
            // Segment is fully within range.
            return tree[node];
        }

        // Partially in the range, query the children and combine their results.
        int mid = (start + end) / 2;
        Node left_beautiful = queryRange(2 * node + 1, start, mid, l, r);
        Node right_beautiful = queryRange(2 * node + 2, mid + 1, end, l, r);
		return combine(left_beautiful, right_beautiful);
    }

public:
    // Constructor to build the segment tree.
    SegmentTree(const string& s) : s(s) {
        tree.resize(4 * s.size());
        buildTree(0, 0, s.size() - 1);
    }

    // Function to update the values in the range [l, r].
    void update(int l, int r, int val) {
        updateRange(0, 0, s.size() - 1, l, r, val);
    }

    // Function to query the range [l, r].
    bool query(int l, int r) {
        return queryRange(0, 0, s.size() - 1, l, r).is_beautiful;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n, m;
        cin >> n >> m;
        string s;
        cin >> s;

        SegmentTree segTree(s);

        for (int i = 0; i < m; ++i) {
            int queryType;
            cin >> queryType;
            if (queryType == 1) {
                int l, r, x;
                cin >> l >> r >> x;
                // Adjusting since our segment tree is 0-indexed but input is 1-indexed.
                segTree.update(l - 1, r - 1, x);
            } else if (queryType == 2) {
                int l, r;
                cin >> l >> r;
                // Query and output the result
                if (segTree.query(l - 1, r - 1)) {  // adjusting indices
                    cout << "YES\n";
                } else {
                    cout << "NO\n";
                }
            }
        }
    }
    return 0;
}
