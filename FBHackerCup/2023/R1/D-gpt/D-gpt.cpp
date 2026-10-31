// Generated with GPT-4
#include<bits/stdc++.h>
using namespace std;

const int MOD = 1000000007;
const int MAXN = 1000005;

struct Node {
    long long val_no_update;
    int idx_no_update;
    long long val_with_update;
    int idx_with_update;
    bool lazy_flag;
};

Node tree[4*MAXN];

void buildTree(int arr[], int left, int right, int idx) {
    if(left == right) {
        tree[idx].val_no_update = arr[left] % MOD;
        tree[idx].idx_no_update = left;
        tree[idx].val_with_update = (arr[left] * 1000000006LL) % MOD;
        tree[idx].idx_with_update = left;
        tree[idx].lazy_flag = false;
        return;
    }
    
    int mid = (left + right) / 2;
    buildTree(arr, left, mid, 2*idx);
    buildTree(arr, mid+1, right, 2*idx+1);

    // Combine children info
    // For no update
    if(tree[2*idx].val_no_update >= tree[2*idx+1].val_no_update) {
        tree[idx].val_no_update = tree[2*idx].val_no_update;
        tree[idx].idx_no_update = tree[2*idx].idx_no_update;
    } else {
        tree[idx].val_no_update = tree[2*idx+1].val_no_update;
        tree[idx].idx_no_update = tree[2*idx+1].idx_no_update;
    }

    // For with update
    if(tree[2*idx].val_with_update >= tree[2*idx+1].val_with_update) {
        tree[idx].val_with_update = tree[2*idx].val_with_update;
        tree[idx].idx_with_update = tree[2*idx].idx_with_update;
    } else {
        tree[idx].val_with_update = tree[2*idx+1].val_with_update;
        tree[idx].idx_with_update = tree[2*idx+1].idx_with_update;
    }

    tree[idx].lazy_flag = false;
}

void push(int idx, int left, int right) {
    if(tree[idx].lazy_flag) {
        swap(tree[idx].val_no_update, tree[idx].val_with_update);
        swap(tree[idx].idx_no_update, tree[idx].idx_with_update);
        
        if(left != right) {  // not a leaf node
            tree[2*idx].lazy_flag = !tree[2*idx].lazy_flag;
            tree[2*idx+1].lazy_flag = !tree[2*idx+1].lazy_flag;
        }

        tree[idx].lazy_flag = false;
    }
}

void update(int left_bound, int right_bound, int left, int right, int idx) {
    push(idx, left, right);  // push updates if any

    if(left_bound > right || right_bound < left) return;

    if(left_bound <= left && right <= right_bound) {
        tree[idx].lazy_flag = true;
        push(idx, left, right);
        return;
    }

    int mid = (left + right) / 2;
    update(left_bound, right_bound, left, mid, 2*idx);
    update(left_bound, right_bound, mid+1, right, 2*idx+1);

    // Update based on children after possible changes
    if(tree[2*idx].val_no_update >= tree[2*idx+1].val_no_update) {
        tree[idx].val_no_update = tree[2*idx].val_no_update;
        tree[idx].idx_no_update = tree[2*idx].idx_no_update;
    } else {
        tree[idx].val_no_update = tree[2*idx+1].val_no_update;
        tree[idx].idx_no_update = tree[2*idx+1].idx_no_update;
    }

    if(tree[2*idx].val_with_update >= tree[2*idx+1].val_with_update) {
        tree[idx].val_with_update = tree[2*idx].val_with_update;
        tree[idx].idx_with_update = tree[2*idx].idx_with_update;
    } else {
        tree[idx].val_with_update = tree[2*idx+1].val_with_update;
        tree[idx].idx_with_update = tree[2*idx+1].idx_with_update;
    }
}

Node query(int left_bound, int right_bound, int left, int right, int idx) {
    push(idx, left, right);  // push updates if any

    if(left_bound > right || right_bound < left) return {0, -1, 0, -1, false};

    if(left_bound <= left && right <= right_bound) return tree[idx];

    int mid = (left + right) / 2;
    Node left_result = query(left_bound, right_bound, left, mid, 2*idx);
    Node right_result = query(left_bound, right_bound, mid+1, right, 2*idx+1);

    Node combined_result;

    if(left_result.val_no_update >= right_result.val_no_update) {
        combined_result.val_no_update = left_result.val_no_update;
        combined_result.idx_no_update = left_result.idx_no_update;
    } else {
        combined_result.val_no_update = right_result.val_no_update;
        combined_result.idx_no_update = right_result.idx_no_update;
    }

    if(left_result.val_with_update >= right_result.val_with_update) {
        combined_result.val_with_update = left_result.val_with_update;
        combined_result.idx_with_update = left_result.idx_with_update;
    } else {
        combined_result.val_with_update = right_result.val_with_update;
        combined_result.idx_with_update = right_result.idx_with_update;
    }

    return combined_result;
}

// Rest of the main function remains unchanged...
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    for(int t=1; t<=T; t++) {
        int N;
        cin >> N;

        int arr[MAXN];
        for(int i=1; i<=N; i++) {
            cin >> arr[i];
        }

        // Build segment tree
        buildTree(arr, 1, N, 1);

        int Q;
        cin >> Q;

        int64_t ans = 0;  // using int64_t for ans to prevent overflow
        for(int q=0; q<Q; q++) {
            int L, R;
            cin >> L >> R;

            // Update segment tree
            update(L, R, 1, N, 1);

            // Query the maximum value and its index
            Node maxNode = query(1, N, 1, N, 1);

            ans += maxNode.idx_no_update;  // Since we need to sum up the indices
        }

        cout << "Case #" << t << ": " << ans << "\n";
    }
    
    return 0;
}
