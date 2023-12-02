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

    char* digits[9] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

    input.open("input.txt");
    if (input.is_open()) {
        while (getline(input, line)) {

            int n = line.size();
            for (int i=0; i<n; i++) {
                sc = line.substr(i, n);

                if (std::regex_match(sc, std::regex("^[0-9].*"))) {
                    sc = line[i];
                    if (first == "") first = sc;
                    last = sc;
                }
                else {
                    for (int i=0; i<10; i++) {
                        std::string digit = digits[i];
                        std::string regex = "^" + digit + ".*";
                        if (std::regex_match(sc, std::regex(regex))) {
                            if (first == "") first = std::to_string(i+1);
                            last = std::to_string(i+1);
                        }
                    }
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