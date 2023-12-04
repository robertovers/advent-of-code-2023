#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <regex>
#include <set>
#include <map>
#include <tuple>


int main() {
    std::string line;
    std::ifstream input;
    int result = 0;

    input.open("input.txt");

    if (input.is_open()) {
        while (getline(input, line)) {

            std::stringstream ss(line);
            std::set<int> w_numbers;

            line = line.substr(10, line.size());

            int delim = line.find("|");

            for (int i=0; i<delim; i+=3) {
                std::string num_s = line.substr(i, 3);
                int num = stoi(num_s);
                w_numbers.insert(num);
            }

            int score = 0;
            for (int i=delim+2; i<line.size(); i+=3) {
                std::string num_s = line.substr(i, 3);
                int num = stoi(num_s);
                if (w_numbers.find(num) != w_numbers.end()) {
                    if (score == 0) {
                        score += 1;
                    } else {
                        score *= 2;
                    }
                }
            }

            result += score;

        }
    }

    std::cout << result << std::endl;
    return 0;
}