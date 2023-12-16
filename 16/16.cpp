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


typedef struct beam {
    int x;
    int y;
    int dx;
    int dy;
};


int part_one(std::vector<std::string> lines, beam start_beam) {

    std::vector<std::string> lines2 = lines;

    std::vector<std::vector<int> > visited;
    for (int y=0; y<lines.size(); y++) {
        std::vector<int> row;
        for (int x=0; x<lines[0].size(); x++) {
            row.push_back(0);
        }
        visited.push_back(row);
    }

    std::deque<beam> beams;
    beams.push_back(start_beam);

    std::unordered_set<std::string> visited_n;
    std::unordered_set<std::string> visited_e;
    std::unordered_set<std::string> visited_s;
    std::unordered_set<std::string> visited_w;

    while (beams.size() > 0) {
        
        beam b = beams.front();
        beams.pop_front();
        visited[b.y][b.x] = 1;

        std::string key = std::to_string(b.x) + "." + std::to_string(b.y);

        if (b.dx == 1) {
            if (visited_e.find(key) == visited_e.end()) visited_e.insert(key);
        }
        if (b.dx == -1) {
            if (visited_w.find(key) == visited_w.end()) visited_w.insert(key);
        }
        if (b.dy == 1) {
            if (visited_s.find(key) == visited_s.end()) visited_s.insert(key);
        }
        if (b.dy == -1) {
            if (visited_n.find(key) == visited_n.end()) visited_n.insert(key);
        }

        switch (lines[b.y][b.x]) {
            case ('.'):
                break;
            case ('/'):
                if (b.dx != 0) {
                    b.dy = -b.dx;
                    b.dx = 0;
                } else {
                    b.dx = -b.dy;
                    b.dy = 0;
                }
                break;
            case ('\\'):
                if (b.dx != 0) {
                    b.dy = b.dx;
                    b.dx = 0;
                } else {
                    b.dx = b.dy;
                    b.dy = 0;
                }
                break;
            case ('|'):
                if (b.dy == 0) {
                    b.dx = 0;
                    b.dy = -1;
                    beams.push_back({ b.x, b.y, 0, 1 });
                }
                break;
            case ('-'):
                if (b.dx == 0) {
                    b.dy = 0;
                    b.dx = -1;
                    beams.push_back({ b.x, b.y, 1, 0 });
                }
                break;
        }

        b.y += b.dy;
        b.x += b.dx;

        if (b.x >= 0 && b.x < lines[0].size() && b.y >= 0 && b.y < lines.size()) {

            key = std::to_string(b.x) + "." + std::to_string(b.y);

            if (b.dx == 1) {
                if (visited_e.find(key) == visited_e.end()) beams.push_back(b);
            }
            if (b.dx == -1) {
                if (visited_w.find(key) == visited_w.end()) beams.push_back(b);
            }
            if (b.dy == 1) {
                if (visited_s.find(key) == visited_s.end()) beams.push_back(b);
            }
            if (b.dy == -1) {
                if (visited_n.find(key) == visited_n.end()) beams.push_back(b);
            } 
        }
    }

    int visited_count;
    for (int y=0; y<visited.size(); y++) {
        for (int x=0; x<visited[0].size(); x++) {
            visited_count += (int) visited[y][x] == 1;
        }
    }

    return visited_count;
}


int part_two(std::vector<std::string> lines) {

    std::vector<beam> start_beams;
    for (int y=0; y<lines.size(); y++) {
        for (int x=0; x<lines[0].size(); x++) {
            if (y == 0) {
                start_beams.push_back({ x, y, 0, 1 });
            }
            if (y == lines.size()-1) {
                start_beams.push_back({ x, y, 0, -1 });
            }
            if (x == 0) {
                start_beams.push_back({ x, y, 1, 0 });
            }
            if (x == lines[0].size()-1) {
                start_beams.push_back({ x, y, -1, 0 });
            }
        }
    }

    int best = 0;
    for (beam sb: start_beams) {
        int visited_count = part_one(lines, sb);
        if (visited_count > best) best = visited_count;
    }

    return best;
}


int main(int argc, char* argv[]) {
    char* filename = argv[1];
    std::vector<std::string> lines;

    read_file(filename, lines);
    printf("%d\n", part_one(lines, { 0, 0, 1, 0 }));
    printf("%d\n", part_two(lines));
}