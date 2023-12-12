#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <regex>
#include <set>
#include <map>
#include <tuple>
#include <unordered_set>
#include <cmath>


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


std::vector<int> get_y_gaps(std::vector<std::string>& lines) {
    std::vector<int> result;

    for (int y=0; y<lines.size(); y++) {
        bool no_stars = true;
        for (int x=0; x<lines[0].size(); x++) {
            if (lines[y][x] == '#') no_stars = false;
        }
        if (no_stars) result.push_back(y);
    }
    
    return result; 
}


std::vector<int> get_x_gaps(std::vector<std::string>& lines) {
    std::vector<int> result;

    for (int x=0; x<lines[0].size(); x++) {
        bool no_stars = true;
        for (int y=0; y<lines.size(); y++) {
            if (lines[y][x] == '#') no_stars = false;
        }
        if (no_stars) result.push_back(x);
    }

    return result; 
}


std::vector<std::pair<int, int> > get_galaxies(std::vector<std::string>& lines) {
    std::vector<std::pair<int, int> > galaxies;
    for (int y=0; y<lines.size(); y++) {
        for (int x=0; x<lines[0].size(); x++) {
            if (lines[y][x] == '#') galaxies.push_back({ x, y });
        }
    }
    return galaxies;
}


int get_n_gaps_between(std::vector<int> gaps, int a, int b) {
    int result = 0;
    for (int g: gaps) {
        if (g > a && g < b || g < a && g > b) result++;
    }
    return result;
}


long long int solve(std::vector<std::string>& lines, int factor) {
    long long int result = 0;
    
    std::vector<int> y_gaps = get_y_gaps(lines);
    std::vector<int> x_gaps = get_x_gaps(lines);
    std::vector<std::pair<int, int> > galaxies = get_galaxies(lines);

    for (int i=0; i<galaxies.size(); i++) {
        for (int j=i+1; j<galaxies.size(); j++) {
            if (i==j) continue;
            int dx = abs(galaxies[i].first - galaxies[j].first) + get_n_gaps_between(x_gaps, galaxies[i].first, galaxies[j].first) * (factor-1);
            int dy = abs(galaxies[i].second - galaxies[j].second) + get_n_gaps_between(y_gaps, galaxies[i].second, galaxies[j].second) * (factor-1);
            result += dx + dy;
        }
    }

    return result;
}


int main(int argc, char* argv[]) {
    char* filename = argv[1];
    std::vector<std::string> lines;

    read_file(filename, lines);
    printf("%ld\n", solve(lines, 2));
    printf("%ld\n", solve(lines, 1000000));
}