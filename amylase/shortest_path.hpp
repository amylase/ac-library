#ifndef AMYLASE_SHORTEST_PATH_HPP
#define AMYLASE_SHORTEST_PATH_HPP 1

#include <array>
#include <vector>
#include <map>
#include <queue>
#include <limits>

namespace amylase {

template <class CostType>
struct dijkstra {
    struct edge { int to; CostType cost; };

    int v;
    std::vector<std::vector<edge> > g;
    dijkstra(int _v): v(_v), g(v) {}

    void add_edge (int from, int to, CostType cost) {
        g[from].push_back((edge){to, cost});
    }

    static CostType inf() {
        return std::numeric_limits<CostType>::max();
    }

    std::vector<CostType> get_distances(int init) {
        std::vector<CostType> min_cost(v, inf());
        using queue_element = std::pair<CostType, int>;
        std::priority_queue<queue_element, std::vector<queue_element>, std::greater<queue_element>> q;
        min_cost[init] = 0;
        q.emplace(0, init);
        while (not q.empty()) {
            CostType cost = q.top().first;
            int pos = q.top().second;
            q.pop();

            if (cost > min_cost[pos]) {
                continue;
            }

            for (int i = 0; i < g[pos].size(); ++i) {
                int npos = g[pos][i].to;
                CostType ncost = cost + g[pos][i].cost;
                if (ncost < min_cost[npos]) {
                    min_cost[npos] = ncost;
                    q.emplace(ncost, npos);
                }
            }
        }
        return min_cost;
    }

    CostType get_distance(int s, int t) {
        return get_distances(s)[t];
    }
};

struct zero_one_bfs {
    struct edge { int to; int cost; };

    int v;
    std::vector<std::vector<edge> > g;
    zero_one_bfs(int _v): v(_v), g(v) {}

    void add_zero_edge(int from, int to) {
        add_edge(from, to, 0);
    }

    void add_one_edge(int from, int to) {
        add_edge(from, to, 1);
    }

    void add_edge(int from, int to, int cost) {
        assert(cost == 0 || cost == 1);
        g[from].push_back((edge){to, cost});
    }

    static int inf() {
        return std::numeric_limits<int>::max();
    }

    std::vector<int> get_distances(int init) {
        std::vector<int> min_cost(v, inf());
        using queue_element = std::pair<int, int>;
        std::deque<queue_element> q;
        min_cost[init] = 0;
        q.emplace_front(0, init);
        while (not q.empty()) {
            int cost = q.front().first;
            int pos = q.front().second;
            q.pop_front();

            if (cost > min_cost[pos]) {
                continue;
            }

            for (int i = 0; i < g[pos].size(); ++i) {
                int edge_cost = g[pos][i].cost;
                int npos = g[pos][i].to;
                int ncost = cost + edge_cost;
                if (ncost < min_cost[npos]) {
                    min_cost[npos] = ncost;
                    if (edge_cost == 0) {
                        q.emplace_front(ncost, npos);
                    } else {
                        q.emplace_back(ncost, npos);
                    }
                }
            }
        }
        return min_cost;
    }

    int get_distance(int s, int t) {
        return get_distances(s)[t];
    }
};

}  // namespace amylase

#endif  // AMYLASE_SHORTEST_PATH_HPP
