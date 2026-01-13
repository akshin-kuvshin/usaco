// author: Danila "akshin_" Axyonov

#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
using namespace std;
using pii = pair<int, int>;

void solve();
int count_party_size_and_fill_root(
    int                                 i,
    const vector<unordered_set<int>>&   children,
    int root_i, vector<int>&            root,
    const vector<vector<int>>&          parties,
    bool                                is_start
);

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;
    vector<int> parent(N);
    vector<unordered_set<int>> children(N);
    for (int i = 0; i < N; ++i) {
        cin >> parent[i];
        --parent[i];
        children[parent[i]].insert(i);
    }
    int M;
    cin >> M;
    vector<pii> party_info(M);
    {
        char c;
        for (auto& [i, t] : party_info) {
            cin >> i >> c;
            --i;
            switch (c) {
                case 'C': t = 0; break;
                case 'O': t = 1; break;
                case 'W': t = 2; break;
                default: return 1;
            }
        }
    }

    vector<vector<int>> parties(N);
    for (auto [i, t] : party_info)
        parties[i].push_back(t);

    vector<int> party_size(N),
                root(N, -1);
    {
        vector<bool> used(N);
        for (int i = 0; i < N; ++i) {
            if (parties[i].empty())
                continue;
            party_size[i] = count_party_size_and_fill_root(i, children, i, root, parties, true);
        }
    }

    vector<int> cnt(3);
    for (int i = 0; i < N; ++i) {
        if (parties[i].empty())
            continue;
        cnt[parties[i].back()] += party_size[i];
    }

    vector<string> answers(M);
    for (int j = M - 1; j >= 0; --j) {
        for (auto cnt_i : cnt)
            answers[j] += to_string(cnt_i) + ' ';
        answers[j].pop_back();

        auto [i, t] = party_info[j];

        cnt[t] -= party_size[i];
        parties[i].pop_back();

        if (not parties[i].empty())
            cnt[parties[i].back()] += party_size[i];
        else {
            root[i] = -1;
            for (int p = parent[i]; p != -1; p = root[p])
                if (p == root[p]) {
                    root[i] = p;
                    party_size[p] += party_size[i];
                    cnt[parties[p].back()] += party_size[i];
                    break;
                }
        }
    }

    for (auto& answer : answers)
        cout << answer << '\n';

    return 0;
}

int count_party_size_and_fill_root(
    int                                 i,
    const vector<unordered_set<int>>&   children,
    int root_i, vector<int>&            root,
    const vector<vector<int>>&          parties,
    bool                                is_start
) {
    if (not is_start and not parties[i].empty())
        return 0;

    root[i] = root_i;

    int res = 1;
    for (auto child : children[i])
        res += count_party_size_and_fill_root(child, children, root_i, root, parties, false);
    return res;
}
