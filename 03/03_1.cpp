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

    std::vector< std::vector<char> > items;
    int rows = 0;

    if (input.is_open()) {
        while (getline(input, line)) {
            std::vector<char> row;
            for (int i=0; i<line.size(); i++) {
                row.push_back(line[i]);
            }
            items.push_back(row);
            rows++;
        }
    }

    int n = items[0].size();

    std::string current, next;

    for (int iy=0; iy<items.size(); iy++) {
        int ix = 0;
        while (ix < n) {
            current = "";
            next = items[iy][ix];
            int ax=ix, ay=iy;
            while (std::regex_match(next, std::regex("[0-9]"))) {
                current += items[iy][ix];
                if (ix+1 < n) {
                    next = items[iy][ix+1];
                    ix++;
                } else {
                    next="";
                }
            }

            if (current != "") {
                int len = current.size();
                int num = stoi(current);
                int start_y = std::max(0, ay-1);
                int end_y = std::min(rows, ay+2);
                int start_x = std::max(0, ax-1);
                int end_x = std::min(n, ax+len+1);
                for (int iy=start_y; iy<end_y; iy++) {
                    for (int ix=start_x; ix<end_x; ix++) {
                        current = items[iy][ix];
                        if (not std::regex_match(current, std::regex("[0-9]|\\."))) {
                            result += num;
                        }
                    }
                }
            }
            ix++;
        }
    }

    std::cout << result << std::endl;
    return 0;
}