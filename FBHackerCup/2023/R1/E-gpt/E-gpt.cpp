#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <tuple>

using namespace std;

int main() {
    int T;
    cin >> T;

    for (int t = 1; t <= T; ++t) {
        int N;
        cin >> N;

        vector<string> words(N);
        for (int i = 0; i < N; ++i) cin >> words[i];

        int Q;
        cin >> Q;
        struct Query {
            int l, r, k, idx;
        };

        vector<Query> queries(Q);
        for (int i = 0; i < Q; ++i) {
            cin >> queries[i].l >> queries[i].r >> queries[i].k;
            queries[i].l--; queries[i].r--;
            queries[i].idx = i;
        }

        sort(queries.begin(), queries.end(), [](const Query& a, const Query& b) {
            return a.k < b.k;
        });

        int max_word_length = 0;
        for (auto& word : words) max_word_length = max(max_word_length, (int)word.length());

        vector<int> alive_indices(N);
        for (int i = 0; i < N; ++i) alive_indices[i] = i;

        vector<int> labels(N, 0);
        vector<int> freq(N + 1, 0);
        vector<int> freqOfFreq(N + 1, 0);
        vector<int64_t> answers(Q, 0);

        int next_query_ptr = 0;

        // Compute sqrtBound
        int sqrtBound = 0;
        while (sqrtBound * (sqrtBound + 1) / 2 <= N) {
            sqrtBound++;
        }
        sqrtBound--;

        auto add = [&](int label) {
            freqOfFreq[min(freq[label], sqrtBound)]--;
            freq[label]++;
            freqOfFreq[min(freq[label], sqrtBound)]++;
        };

        auto remove = [&](int label) {
            freqOfFreq[min(freq[label], sqrtBound)]--;
            freq[label]--;
            freqOfFreq[min(freq[label], sqrtBound)]++;
        };

        for (int k = 1; k <= max_word_length; ++k) {
            alive_indices.erase(remove_if(alive_indices.begin(), alive_indices.end(),
                [&words, k](int i) { return words[i].length() < k; }), alive_indices.end());

            vector<pair<pair<int, char>, int>> sorted_labels(alive_indices.size());
            for (size_t i = 0; i < alive_indices.size(); ++i) {
                sorted_labels[i] = {{labels[alive_indices[i]], words[alive_indices[i]][words[alive_indices[i]].length() - k]}, alive_indices[i]};
            }

            sort(sorted_labels.begin(), sorted_labels.end());

            int label = 0;
            for (size_t i = 0; i < sorted_labels.size(); ++i) {
                if (i > 0 && sorted_labels[i].first != sorted_labels[i - 1].first) label++;
                labels[sorted_labels[i].second] = label;
            }

            vector<Query> current_queries;
            while (next_query_ptr < Q && queries[next_query_ptr].k == k) {
                auto& query = queries[next_query_ptr];
                query.l = lower_bound(alive_indices.begin(), alive_indices.end(), query.l) - alive_indices.begin();
                query.r = upper_bound(alive_indices.begin(), alive_indices.end(), query.r) - alive_indices.begin() - 1;
                current_queries.push_back(query);
                next_query_ptr++;
            }

            sort(current_queries.begin(), current_queries.end(), [&alive_indices](const Query& a, const Query& b) {
                const int blockSize = 512;
                if (a.l / blockSize != b.l / blockSize) {
                    return a.l < b.l;
                }
                return a.r < b.r;
            });

            int moL = 0, moR = -1;
            int64_t current_ans = 0;

            for (const auto& query : current_queries) {

                while (moR < query.r) {
                    moR++;
                    add(labels[alive_indices[moR]]);
                }
                while (moL > query.l) {
                    moL--;
                    add(labels[alive_indices[moL]]);
                }
                while (moL < query.l) {
                    remove(labels[alive_indices[moL]]);
                    moL++;
                }
                while (moR > query.r) {
                    remove(labels[alive_indices[moR]]);
                    moR--;
                }

                current_ans = 0;
                for (int f = 1; f <= sqrtBound; ++f) {
                    int tempFreqOfFreq = freqOfFreq[f];
                    while (f > current_ans && tempFreqOfFreq) {
                        current_ans++;
                        tempFreqOfFreq--;
                    }
                }

                answers[query.idx] = current_ans;
            }

            // Reset interval
            for (int i = moL; i <= moR; i++) {
                remove(labels[alive_indices[i]]);
            }
            moL = 0; moR = -1;
        }

        int64_t total = 0;
        for (int64_t ans : answers) total += ans;

        cout << "Case #" << t << ": " << total << "\n";
    }

    return 0;
}
