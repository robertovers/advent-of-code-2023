#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <regex>
#include <set>
#include <map>
#include <tuple>


bool read_lines(std::string filename, std::vector<std::string>& lines) {
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


std::vector<std::string> split(const std::string &str, char delim) {
    std::vector<std::string> res;
    std::stringstream ss(str);
    std::string item;

    while (getline (ss, item, delim)) {
        res.push_back (item);
    }
    return res;
}


int part_one(std::vector<std::string>& lines) {
    int possible = 0;
    int n, k, id;
    int red, green, blue;
    bool valid;

    for (std::string line: lines) {
        valid = true;
        n = line.size();

        std::string games;
        std::stringstream ss_line(line.substr(5, n));
        ss_line >> id; 

        std::getline(ss_line, games);
        games = games.substr(2, games.size());

        std::vector<std::string> subsets = split(games, ';');

        for (std::string& subset: subsets) {

            std::vector<std::string> hands = split(subset, ',');

            for (std::string& hand: hands) {
                red = 0, green = 0, blue = 0;
                std::string colour;
                std::stringstream ss(hand);

                ss >> k >> colour;

                if (colour == "red") {
                    red += k;
                } else if (colour == "green") {
                    green += k;
                } else {
                    blue += k;
                }

                if (red > 12 || green > 13 || blue > 14) {
                    valid = false;
                }
            }
        }

        if (valid) possible += id;
    }

    return possible;
}


int part_two(std::vector<std::string>& lines) {
    int result = 0;
    int n, k, id;
    int red, green, blue;

    for (std::string line: lines) {
        n = line.size();
        red = 0, green = 0, blue = 0;

        std::string games;
        std::stringstream ss_line(line.substr(5, n));
        ss_line >> id; 

        std::getline(ss_line, games);
        games = games.substr(2, games.size());
        std::vector<std::string> subsets = split(games, ';');

        for (std::string subset: subsets) {
            std::vector<std::string> hands = split(subset, ',');

            for (std::string hand: hands) {
                std::string colour;
                std::stringstream ss(hand);

                ss >> k >> colour;

                if (colour == "red" && k > red) {
                    red = k;
                } else if (colour == "green" && k > green) {
                    green = k;
                } else if (colour == "blue" && k > blue) {
                    blue = k;
                }
            }
        }

        result += red * blue * green;

    }

    return result;
}


int main(int argc, char* argv[]) {
    char* filename = argv[1];
    std::vector<std::string> lines;

    read_lines(filename, lines);
    printf("%d\n", part_one(lines));
    printf("%d\n", part_two(lines));
}