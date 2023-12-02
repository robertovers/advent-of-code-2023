#include <iostream>
#include <fstream>
#include <string>
#include <regex>


int main() {
    std::string line;
    std::ifstream input;
    std::string first = "", last;
    std::string sc;
    int num, sum;

    input.open("input.txt");
    if (input.is_open()) {
        while (getline(input, line)) {

            for (char& c: line) {
                sc = c;
                if (std::regex_match(sc, std::regex("[0-9]"))) {
                    if (first == "") {
                        first = sc;
                    }
                    last = sc;
                }
            }
            num = std::stoi(first + last);
            sum += num;
            first = "";
        }
        input.close();
    }
    std::cout << sum << std::endl;
    return 0;
}