#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <cmath>

struct Query {
    int Ai, Bi, Ki, idx;
};

inline long long hilbertOrder(int x, int y, int pow, int rotate) {
    if (pow == 0) {
        return 0;
    }
    int hpow = 1 << (pow - 1);
    int seg = (x < hpow) ? ((y < hpow) ? 0 : 3) : ((y < hpow) ? 1 : 2);
    (seg == 1 || seg == 2) ? x = hpow - x - 1 : 0;
    (seg == 2 || seg == 3) ? y = hpow - y - 1 : 0;
    if (seg & 1) {
        std::swap(x, y);
    }
    //(seg & 1) ? std::swap(x, y) : 0;

    if (rotate) {
        if (y == 0) {
            return seg * hpow * hpow + hilbertOrder(hpow - x - 1, hpow - y - 1, pow - 1, 1);
        } else {
            return seg * hpow * hpow + hilbertOrder(x, y - 1, pow - 1, 0);
        }
    } else {
        if (x == 0) {
            return seg * hpow * hpow + hilbertOrder(hpow - x - 1, hpow - y - 1, pow - 1, 1);
        } else {
            return seg * hpow * hpow + hilbertOrder(x - 1, y, pow - 1, 0);
        }
    }
}

int main() {
    int T;
    std::cin >> T;

    for (int t = 1; t <= T; ++t) {
        int N;
        std::cin >> N;

        std::vector<std::string> words(N);
        int maxWordLength = 0;
        for (int i = 0; i < N; ++i) {
            std::cin >> words[i];
            maxWordLength = std::max(maxWordLength, static_cast<int>(words[i].length()));
        }

        int Q;
        std::cin >> Q;

        std::vector<Query> queries(Q);
        for (int i = 0; i < Q; ++i) {
            std::cin >> queries[i].Ai >> queries[i].Bi >> queries[i].Ki;
            queries[i].Ai--; // Convert to 0-based indexing
            queries[i].Bi--; // Convert to 0-based indexing
            queries[i].idx = i;
        }

        std::vector<int> results(Q, 0);
        std::vector<int> labelFrequency(N+1, 0);
        std::vector<int> freqOfFreq(N+1, 0);
        freqOfFreq[0] = N+1;

        int sqrtBound = 0;
        while ((sqrtBound * (sqrtBound + 1) / 2) <= N) {
            sqrtBound++;
        }
        sqrtBound--;

        auto add = [&](int label) {
            freqOfFreq[std::min(sqrtBound, labelFrequency[label])]--;
            labelFrequency[label]++;
            freqOfFreq[std::min(sqrtBound, labelFrequency[label])]++;
        };

        auto remove = [&](int label) {
            freqOfFreq[std::min(sqrtBound, labelFrequency[label])]--;
            labelFrequency[label]--;
            freqOfFreq[std::min(sqrtBound, labelFrequency[label])]++;
        };

        auto get_answer = [&]() -> int {
            int currentAnswer = 0;
            for (int f = 1; f <= sqrtBound; ++f) {
                int availableFreq = freqOfFreq[f];
                while (availableFreq-- > 0 && f > currentAnswer) {
                    currentAnswer++;
                }
            }
            return currentAnswer;
        };

        std::sort(queries.begin(), queries.end(), [](const Query& a, const Query& b) {
            return a.Ki < b.Ki;
        });

        int currentQueryPointer = 0;
        std::vector<int> aliveIndices(N);
        for (int i = 0; i < N; ++i) {
            aliveIndices[i] = i;
        }
        std::vector<int> labels(N, 0);

        for (int K = 1; K <= maxWordLength; ++K) {
            aliveIndices.erase(std::remove_if(aliveIndices.begin(), aliveIndices.end(),
                [&words, K](int idx) { return static_cast<int>(words[idx].length()) < K; }),
                aliveIndices.end());

            std::vector<std::pair<std::pair<int, char>, int>> combinedValues;
            for (int idx : aliveIndices) {
                char suffixChar = words[idx][words[idx].length() - K];
                combinedValues.emplace_back(std::make_pair(labels[idx], suffixChar), idx);
            }
            std::sort(combinedValues.begin(), combinedValues.end());

            int newLabel = 0;
            for (int i = 0; i < combinedValues.size(); ++i) {
                if (i > 0 && combinedValues[i].first != combinedValues[i - 1].first) {
                    newLabel++;
                }
                labels[combinedValues[i].second] = newLabel;
            }

            std::vector<Query> currentQueries;
            while (currentQueryPointer < Q && queries[currentQueryPointer].Ki == K) {
                Query query = queries[currentQueryPointer];
                query.Ai = std::lower_bound(aliveIndices.begin(), aliveIndices.end(), query.Ai) - aliveIndices.begin();
                query.Bi = std::upper_bound(aliveIndices.begin(), aliveIndices.end(), query.Bi) - aliveIndices.begin() - 1;
                currentQueries.push_back(query);
                currentQueryPointer++;
            }

            std::sort(currentQueries.begin(), currentQueries.end(), [&](const Query& a, const Query& b) {
                return hilbertOrder(a.Ai, a.Bi, 21, 0) < hilbertOrder(b.Ai, b.Bi, 21, 0);
            });

            int currL = currentQueries[0].Ai, currR = currentQueries[0].Ai - 1;
            for (const auto& query : currentQueries) {
                while (currL > query.Ai) {
                    currL--;
                    add(labels[aliveIndices[currL]]);
                }
                while (currR < query.Bi) {
                    currR++;
                    add(labels[aliveIndices[currR]]);
                }
                while (currL < query.Ai) {
                    remove(labels[aliveIndices[currL]]);
                    currL++;
                }
                while (currR > query.Bi) {
                    remove(labels[aliveIndices[currR]]);
                    currR--;
                }
                results[query.idx] = get_answer();
            }

            while (currL <= currR) {
                remove(labels[aliveIndices[currL]]);
                currL++;
            }
        }

        int64_t sum = 0;
        for (const auto& res : results) {
            sum += res;
        }

        std::cout << "Case #" << t << ": " << sum << std::endl;
    }

    return 0;
}

