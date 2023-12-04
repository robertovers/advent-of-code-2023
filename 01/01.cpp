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


int part_one(std::vector<std::string>& lines) {
    int result = 0;
    std::string first = "", last;
    std::string sc;

    for (std::string line: lines) {
        for (char& c: line) {
            sc = c;
            if (std::regex_match(sc, std::regex("[0-9]"))) {
                if (first == "") {
                    first = sc;
                }
                last = sc;
            }
        }
        int num = std::stoi(first + last);
        result += num;
        first = "";
    }

    return result;
}


int part_two(std::vector<std::string>& lines) {
    int result = 0;
    std::string line;
    std::ifstream input;
    std::string first = "", last;
    std::string digits[9] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

    for (std::string line: lines) {
        int n = line.size();
        for (int i=0; i<n; i++) {
            std::string sub = line.substr(i, n);

            if (std::regex_match(sub, std::regex("^[0-9].*"))) {
                char s = line[i];
                if (first == "") first = s;
                last = s;
            }
            else {
                for (int i=0; i<9; i++) {
                    std::string regex = "^" + digits[i] + ".*";
                    if (std::regex_match(sub, std::regex(regex))) {
                        if (first == "") first = std::to_string(i+1);
                        last = std::to_string(i+1);
                    }
                }
            }
        }

        int num = std::stoi(first + last);
        result += num;
        first = "";
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