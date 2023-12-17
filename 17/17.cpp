#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <regex>
#include <set>
#include <map>
#include <unordered_set>
#include <deque>
#include <tuple>
#include <queue>
#include <limits.h>


bool read_file(std::string filename, std::vector<std::string>& lines) {
    std::ifstream file;
    file.open(filename);

    if (not file.is_open()) return false;

    std::string line;
    while (getline(file, line)) {
        lines.push_back(line);
    }

    file.close();

    return true;
}


const int x_shifts[] = {-1, 0, 1, 0};
const int y_shifts[] = {0, -1, 0, 1};


typedef struct node {
    int x;
    int y;
    int dir;
    int count;
};


std::string node_key(node& n) {
    return std::to_string(n.x) + "." + std::to_string(n.y) + "." + std::to_string(n.dir) + "." + std::to_string(n.count);
}


std::vector<node> vertices_of(node& n, std::vector<std::vector<int> >& G) {
    std::vector<node> vertices;
    for (int d=0; d<4; d++) {
        int next_x = n.x + x_shifts[d];
        int next_y = n.y + y_shifts[d];
        if (next_x >= 0 && next_x < G[0].size()
            && next_y >= 0 && next_y < G.size()
            && (d != n.dir || n.count < 3)
            && (next_x != 0 || next_y != 0))
        {
            node next;
            if (d != n.dir) {
                next = { next_x, next_y, d, 1 };
            } else {
                next = { next_x, next_y, d, n.count+1 };
            }
            vertices.push_back(next);
        }
    }
    return vertices;
}


node min_distance(std::unordered_map<std::string, node>& Q, std::unordered_map<std::string, int>& dist) {
    int min_dist = INT_MAX;
    node min;
    for (auto& [qs, q]: Q) {
        if (dist[node_key(q)] < min_dist) {
            min_dist = dist[node_key(q)];
            min = q;
        }
    }
    return min;
}


int part_one(std::vector<std::string> lines) {

    // Dijkstra's algorithm

    std::vector<std::vector<int> > G;
    std::vector<std::vector<int> > res;
    std::unordered_map<std::string, int> dist;
    std::unordered_map<std::string, node> Q;

    for (int y=0; y<lines.size(); y++) {
        std::vector<int> row_g;
        std::vector<int> row_r;
        for (int x=0; x<lines[0].size(); x++) {
            row_g.push_back(lines[y][x]-48);
            row_r.push_back(INT_MAX);
        }
        G.push_back(row_g);
        res.push_back(row_r);
    }

    node start = { 0, 0, -1, 0 };
    dist[node_key(start)] = 0;
    Q[node_key(start)] = start;

    while (!Q.empty()) {

        node u = min_distance(Q, dist);
        Q.erase(node_key(u));

        std::vector<node> vertices = vertices_of(u, G);

        for (auto& vert: vertices) {
            int distu = dist[node_key(u)] + G[vert.y][vert.x];

            if (dist.find(node_key(vert)) == dist.end() || distu < dist[node_key(vert)]) {
                dist[node_key(vert)] = distu;
                Q[node_key(vert)] = vert;
                if (distu < res[vert.y][vert.x]) {
                    res[vert.y][vert.x] = distu;
                }
            }
        }
        
        for (int y=0; y<lines.size(); y++) {
            for (int x=0; x<lines[0].size(); x++) {
                if (res[y][x] < 200) {
                    printf("%d ", res[y][x]);
                } else {
                    printf(". ");
                }
            }
            printf("\n");
        }

        printf("\n");
    }

    return res[G.size()-1][G[0].size()-1];
}


int part_two(std::vector<std::string>& lines) {
    int result = 0;

    for (std::string line: lines) {
    }

    return result;
}


int main(int argc, char* argv[]) {
    char* filename = argv[1];
    std::vector<std::string> lines;

    read_file(filename, lines);
    printf("%d\n", part_one(lines));
    printf("%d\n", part_two(lines));
}