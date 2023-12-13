#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <regex>
#include <set>
#include <map>
#include <tuple>


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


int part_one(std::vector<std::string> lines) {

    std::vector<std::vector<std::string> > patterns;
    std::vector<std::string> current_pattern;
    for (std::string line: lines) {
        if (line != "") {
            current_pattern.push_back(line);
        } else {
            patterns.push_back(current_pattern);
            current_pattern.clear();
        }
    }
    patterns.push_back(current_pattern);

    int result = 0;
    for (auto& p: patterns) {

        // find vertical mirrors
        for (int x=0; x<p[0].size()-1; x++) {
            bool mirror = true;
            for (int y=0; y<p.size(); y++) {
                int left_it = x;
                int right_it = x+1;
                if (p[y][left_it] != p[y][right_it]) { mirror = false; break; }
                while (left_it >= 0 && right_it < p[0].size() && p[y][left_it] == p[y][right_it]) {
                    left_it--;
                    right_it++;
                }
                if (left_it != -1 && right_it != p[0].size()) { mirror = false; break; }; 
            }
            if (mirror) result += x+1;
        }

        // find horizontal mirrors
        for (int y=0; y<p.size()-1; y++) {
            bool mirror = true;
            for (int x=0; x<p[0].size(); x++) {
                int up_it = y;
                int down_it = y+1;
                if (p[up_it][x] != p[down_it][x]) { mirror = false; break; }
                while (up_it >= 0 && down_it < p.size() && p[up_it][x] == p[down_it][x]) {
                    up_it--;
                    down_it++;
                }
                if (up_it != -1 && down_it != p.size()) { mirror = false; break; }; 
            }
            if (mirror) result += 100 * (y+1);
        }
    }

    return result;
}


int part_two(std::vector<std::string> lines) {

    int result = 0;

    return result;
}


int main(int argc, char* argv[]) {
    char* filename = argv[1];
    std::vector<std::string> lines;

    read_file(filename, lines);
    printf("%d\n", part_one(lines));
    printf("%d\n", part_two(lines));
}