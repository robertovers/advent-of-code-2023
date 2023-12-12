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


typedef struct node {
    int x;
    int y;
    int distance;
    int last_direction;
    char symbol;
    size_t previous_index;
};


const int x_shifts[] = {-1, 0, 1, 0};
const int y_shifts[] = {0, -1, 0, 1};


std::string node_to_string(node& n) {
    return std::to_string(n.x) + std::to_string(n.y);
}


bool _valid_north(node& current, node& next) {
    if (next.y < current.y) {
        return next.symbol == '|' || next.symbol == '7' || next.symbol == 'F';
    }
    return false;
}


bool _valid_south(node& current, node& next) {
    if (next.y > current.y) {
        return next.symbol == '|' || next.symbol == 'J' || next.symbol == 'L';
    }
    return false;
}


bool _valid_east(node& current, node& next) {
    if (next.x > current.x) {
        return next.symbol == '-' || next.symbol == 'J' || next.symbol == '7';
    }
    return false;
}


bool _valid_west(node& current, node& next) {
    if (next.x < current.x) {
        return next.symbol == '-' || next.symbol == 'F' || next.symbol == 'L';
    }
    return false;
}


bool valid(node& current, node& next) {
    //printf("(%d,%d) %c %d : (%d,%d) %c %d\n", current.y, current.x, current.symbol, current.last_direction, next.y, next.x, next.symbol, next.last_direction);
    if (next.last_direction != current.last_direction && next.last_direction % 2 == current.last_direction % 2) {
        return false;
    }
    switch (current.symbol) {
        case '|':
            return _valid_south(current, next) || _valid_north(current, next); 
        case '-':
            return _valid_east(current, next) || _valid_west(current, next);
        case 'L':
            return _valid_north(current, next) || _valid_east(current, next);
        case 'J':
            return _valid_north(current, next) || _valid_west(current, next);
        case '7':
            return _valid_south(current, next) || _valid_west(current, next);
        case 'F':
            return _valid_south(current, next) || _valid_east(current, next);
    }
    return false;
}


node find_start_node(std::vector<std::string>& lines) {
    node start_node;
    for (int y=0; y<lines.size(); y++) {
        for (int x=0; x<lines.size(); x++) {
            if (lines[y][x] == 'S') {
                start_node = { x, y, 0, -1, 'S', 0 };
                return start_node;
            }
        }
    }
    return start_node;
}


void replace_start_node(std::vector<std::string>& lines, node& start_node) {
    for (char c: "|-LJ7F") {
        int valid_count = 0;
        for (int d=0; d<4; d++) {
            start_node.symbol = c;
            int next_x = start_node.x + x_shifts[d];
            int next_y = start_node.y + y_shifts[d];
            node next = { next_x, next_y, 1, d, lines[next_y][next_x], 0 };
            valid_count += (int) valid(start_node, next);
        }
        if (valid_count == 2) return;
    }
}


std::vector<node> recover_path(std::vector<node> visited, size_t last) {
    std::vector<node> path;
    path.push_back(visited[last]);
    size_t current = last;
    while (current != 0) {
        path.push_back(visited[current]);
        current = visited[current].previous_index;
    }
    return path;
}


int part_one(std::vector<std::string> lines) {

    std::deque<node> to_visit;

    node start_node = find_start_node(lines);
    replace_start_node(lines, start_node);
    lines[start_node.y][start_node.x] = start_node.symbol;
    printf("start node: (%d,%d)\n", start_node.y, start_node.x);
    to_visit.push_back(start_node);

    while (!to_visit.empty()) {
        node current = to_visit.back();
        to_visit.pop_back();

        if (current.distance > 2 && current.x == start_node.x && current.y == start_node.y) {
            return current.distance / 2;
        }

        for (int d=0; d<4; d++) {
            int next_x = current.x + x_shifts[d];
            int next_y = current.y + y_shifts[d];
            if (next_y > lines.size() || next_y < 0 || next_x > lines[0].size() || next_x < 0) continue;
            node next = { next_x, next_y, current.distance+1, d, lines[next_y][next_x], 0 };
            if (valid(current, next)) to_visit.push_back(next);
        }
    }

    return -1;
}


int part_two(std::vector<std::string> lines) {

    std::ofstream path_file;
    path_file.open("path.txt");

    std::deque<node> to_visit;
    std::vector<node> visited;

    node start_node = find_start_node(lines);
    replace_start_node(lines, start_node);
    lines[start_node.y][start_node.x] = start_node.symbol;
    to_visit.push_back(start_node);

    node current;
    size_t last;

    while (!to_visit.empty()) {
        current = to_visit.back();
        to_visit.pop_back();

        if (current.distance > 2 && current.x == start_node.x && current.y == start_node.y) {
            last = visited.size() - 1;
            break;
        }

        visited.push_back(current);

        if (current.distance > 0) {
            size_t prev_index = current.previous_index;
        }

        for (int d = 0; d < 4; d++) {
            int next_x = current.x + x_shifts[d];
            int next_y = current.y + y_shifts[d];
            if (next_y >= lines.size() || next_y < 0 || next_x >= lines[0].size() || next_x < 0) continue;
            size_t prev_index = visited.size() - 1;
            node next = {next_x, next_y, current.distance + 1, d, lines[next_y][next_x], prev_index};
            if (valid(current, next)) to_visit.push_back(next);
        }
    }

    std::vector<node> path = recover_path(visited, last);
    path.push_back(start_node);

    int result = 0;
    for (int y=0; y<lines.size(); y++) {
        std::string row_string = "";

        // find the pipes in this row
        std::vector<node> pipes;
        for (node& n: path) {
            if (n.y == y) {
                pipes.push_back(n);
            }
        }

        // use the even-odd rule by checking how many of the above pipes
        // are crossed by each tile if you were to draw a line to the left
        for (int x=0; x<lines[0].size(); x++) {
            int crossed = 0;
            if (std::find_if(pipes.begin(), pipes.end(), [x=x](const node& m) -> bool { return m.x == x; }) == pipes.end()) {
                for (node& p: pipes) {
                    if (x > p.x && (p.symbol == '|' || p.symbol == 'J' || p.symbol == 'L')) {
                        crossed++;
                    }
                }
            }

            if (crossed % 2 == 1) {
                result++;
                row_string += "1";
            } else {
                row_string += "0";
            }
        }

        for (node& p: path) {
            if (p.y == y) {
                row_string[p.x] = lines[p.y][p.x];
            }
        }

        path_file << row_string << std::endl;
    }

    path_file.close();

    return result;
}


int main(int argc, char* argv[]) {
    char* filename = argv[1];
    std::vector<std::string> lines;

    read_file(filename, lines);
    printf("%d\n", part_one(lines));
    printf("%d\n", part_two(lines));
}