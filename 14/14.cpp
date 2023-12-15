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


void slide_north(std::vector<std::string>& lines) {
    for (int y=0; y<lines.size(); y++) {
        for (int x=0; x<lines[0].size(); x++) {
            if (lines[y][x] == 'O') {
                int yr = y;
                while (yr-1 >= 0 && lines[yr-1][x] == '.') {
                    lines[yr-1][x] = 'O';
                    lines[yr][x] = '.';
                    yr--;
                }
            }
        }
    } 
}


void slide_west(std::vector<std::string>& lines) {
    for (int x=0; x<lines[0].size(); x++) {
        for (int y=0; y<lines.size(); y++) {
            if (lines[y][x] == 'O') {
                int xr = x;
                while (xr-1 >= 0 && lines[y][xr-1] == '.') {
                    lines[y][xr-1] = 'O';
                    lines[y][xr] = '.';
                    xr--;
                }
            }
        }
    } 
}


void slide_south(std::vector<std::string>& lines) {
    for (int y=lines.size()-1; y>=0; y--) {
        for (int x=0; x<lines[0].size(); x++) {
            if (lines[y][x] == 'O') {
                int yr = y;
                while (yr+1 < lines.size() && lines[yr+1][x] == '.') {
                    lines[yr+1][x] = 'O';
                    lines[yr][x] = '.';
                    yr++;
                }
            }
        }
    } 
}


void slide_east(std::vector<std::string>& lines) {
    for (int x=lines[0].size()-1; x>=0; x--) {
        for (int y=0; y<lines.size(); y++) {
            if (lines[y][x] == 'O') {
                int xr = x;
                while (xr+1 < lines[0].size() && lines[y][xr+1] == '.') {
                    lines[y][xr+1] = 'O';
                    lines[y][xr] = '.';
                    xr++;
                }
            }
        }
    } 
}


std::string map_to_string(std::vector<std::string> lines) {
    std::string result;
    for (auto line: lines) {
        result += line + "x";
    }
    return result;
}


std::vector<std::string> map_from_string(std::string str) {
    std::vector<std::string> result;
    int i = 0;
    while (i < str.size()) {
        std::string line = "";
        while (str[i] != 'x') {
            line += str[i];
            i++;
        }
        result.push_back(line);
        i++;
    }
    return result;
}


int part_one(std::vector<std::string> lines) {
    int result = 0;

    slide_north(lines);

    for (int y=0; y<lines.size(); y++) {
        for (int x=0; x<lines[0].size(); x++) {
            if (lines[y][x] == 'O') {
                result += lines.size() - y;
            }
        }
    }

    return result;
}


int part_two(std::vector<std::string> lines) {
    int result = 0;

    std::map<std::string, int> seen;
    std::vector<std::string> repeating;

    int cycles = 1000000000;

    for (int i=0; i<cycles; i++) {

        bool all_repeats = true;
        std::string str = map_to_string(lines);

        if (seen.find(str) != seen.end() && std::find(repeating.begin(), repeating.end(), str) == repeating.end()) {
            repeating.push_back(str);
            all_repeats = false;
        } else if (seen.find(str) == seen.end()) {
            all_repeats = false;
            seen[str] = i;
        }

        if (all_repeats) {
            int repeats_from = seen[repeating[0]];
            lines = map_from_string(repeating[(cycles - repeats_from) % repeating.size()]);
            break;
        }

        slide_north(lines);
        slide_west(lines);
        slide_south(lines);
        slide_east(lines);

    }

    for (int y=0; y<lines.size(); y++) {
        for (int x=0; x<lines[0].size(); x++) {
            if (lines[y][x] == 'O') {
                result += lines.size() - y;
            }
        }
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