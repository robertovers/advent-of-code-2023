#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <regex>
#include <set>
#include <map>
#include <tuple>
#include <deque>
#include <unordered_set>


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


std::vector<int> xdirs = { -1, 0, 1, 0 };
std::vector<int> ydirs = { 0, -1, 0, 1 };

int best = 0;

void dfs(std::vector<std::string>& lines, int x, int y,
         std::unordered_set<std::string>& path)
{
    if (y == lines.size()-1 && x == lines[0].size()-2) {
        if ((int) path.size() > best) {
            best = (int) path.size();
            printf("%d\n", (int) path.size());
        }
        //std::ofstream file;
        //file.open("path.txt");
        //auto mlines = lines;
        //for (const auto& [lx, ly]: path) {
        //    mlines[ly][lx] = 'O';
        //}
        //for (auto l: mlines) {
        //    file << l << std::endl;
        //}
        //file.close();
    }
    for (int d=0; d<4; d++) {
        int nx = x + xdirs[d];
        int ny = y + ydirs[d];
        if (ny > lines.size() || ny < 0 || nx > lines[0].size() || nx < 0)
            continue;
        std::string key = std::to_string(nx) + "-" + std::to_string(ny);
        if (lines[ny][nx] != '#' && path.find(key) == path.end()) {
            path.insert(key);
            dfs(lines, nx, ny, path);
            path.erase(key);
        }
    }
}


int part_one(std::vector<std::string> lines) {

    std::unordered_set<std::string> path;
    dfs(lines, 1, 0, path);

    return best;
}


int main(int argc, char* argv[]) {
    char* filename = argv[1];
    std::vector<std::string> lines;

    read_file(filename, lines);
    printf("%d\n", part_one(lines));
}