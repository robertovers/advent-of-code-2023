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
            && (d != n.dir || n.count < 3))
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


node min_distance(std::unordered_map<std::string, node> Q, std::vector<std::vector<int> >& dist) {
    int min_dist = INT_MAX;
    node min;
    for (auto& [qs, q]: Q) {
        if (dist[q.y][q.x] < min_dist) {
            min_dist = dist[q.y][q.x];
            min = q;
        }
    }
    return min;
}


int part_one(std::vector<std::string> lines) {

    // Dijkstra's algorithm

    std::vector<std::vector<int> > dist;
    std::vector<std::vector<int> > G;
    std::unordered_map<std::string, node> Q;

    for (int y=0; y<lines.size(); y++) {
        std::vector<int> row_d;
        std::vector<int> row_g;
        for (int x=0; x<lines[0].size(); x++) {
            row_d.push_back(INT_MAX);
            row_g.push_back(lines[y][x]-48);
        }
        dist.push_back(row_d);
        G.push_back(row_g);
    }

    dist[0][0] = 0;
    node start = { 0, 0, -1, 0 };
    Q[node_key(start)] = start;

    while (!Q.empty()) {

        node u = min_distance(Q, dist);
        Q.erase(node_key(u));

        std::vector<node> vertices = vertices_of(u, G);

        for (auto& vert: vertices) {
            int distu = dist[u.y][u.x] + G[vert.y][vert.x];

            if (distu < dist[vert.y][vert.x]) {
                dist[vert.y][vert.x] = distu;
                Q[node_key(vert)] = vert;
            }
        }
        
        for (int y=0; y<lines.size(); y++) {
            for (int x=0; x<lines[0].size(); x++) {
                if (dist[y][x] < 200) {
                    printf("%d ", dist[y][x]);
                } else {
                    printf(". ");
                }
            }
            printf("\n");
        }

        printf("\n");
    }

    return dist[G.size()-1][G[0].size()-1];
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