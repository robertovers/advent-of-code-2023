#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <regex>
#include <set>


std::vector<std::string> split(const std::string &str, char delim) {
    std::vector<std::string> res;
    std::stringstream ss(str);
    std::string item;

    while (getline (ss, item, delim)) {
        res.push_back (item);
    }
    return res;
}


int main() {
    std::string line;
    std::ifstream input;

    int n, k, id;
    int possible = 0;
    int red, green, blue;
    bool valid;

    input.open("input.txt");

    if (input.is_open()) {
        while (getline(input, line)) {
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

        input.close();
    }

    std::cout << possible << std::endl;
    return 0;
}