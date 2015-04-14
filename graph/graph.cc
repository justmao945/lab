#include <cassert>
#include <cstdio>
#include <vector>


using namespace std;

class Graph {
public:

    Graph(int V) { g.resize(V); }
    const vector<int>& adj(int V) const { return g.at(V); }
    size_t size() const { return g.size(); }

    void addEdge(int u, int v) {
        g.at(u).push_back(v);
        g.at(v).push_back(u);
    }

private:    
    vector<vector<int> > g;
};


class Bipartite {
public:
    Bipartite(const Graph& g): g(g) {}

    bool is() const {
        vector<char> color(g.size());
        color[0] = 1;
        bool r = dfs(color, 0);
        for(int i = 0; i < g.size(); i++) {
            printf("%d: %d\n", i, color.at(i));
        }
        return r;
    }
    
    bool dfs(vector<char>& color, int s) const {
        char cs = color.at(s);
        assert(cs != 0);
        for(auto w: g.adj(s)) {
            char& c = color.at(w);
            if(c == 0) { // not visited
                c = cs == 1 ? 2 : 1;
                if(! dfs(color, w)) {
                    return false;
                }
            } else { // visited
                if(c == cs) { // dup color
                    return false;
                }
            }
        }
        return true;
    }

private:
    const Graph& g;
};


// if has no cycle, it is a tree
class Cycle {
public:
    Cycle(const Graph& g): g(g) {}

    bool has() const {
    }

private:
    const Graph& g;
};

int main() {
    Graph g(7);
    g.addEdge(0,1);
    g.addEdge(0,2);
    g.addEdge(0,5);
    g.addEdge(0,6);
    g.addEdge(1,3);
    g.addEdge(2,3);
    g.addEdge(2,4);
    g.addEdge(4,5);
    g.addEdge(4,6);

    //g.addEdge(1,5);
    
    Bipartite bi(g);
    printf("is bipartite: %d\n", bi.is());

    return 0;
}

