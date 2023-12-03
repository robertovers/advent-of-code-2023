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

    std::map<std::tuple<int, int>, int> nums;

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
                for (int x=0; x<current.size(); x++) {
                    nums[std::make_tuple(ax+x, ay)] = stoi(current);
                }
            }
            ix++;
        }
    }

    for (int iy=0; iy < items.size(); iy++) {
        for (int ix=0; ix < n; ix++) {
            if (items[iy][ix] == '*') {
                std::set<int> gears;
                for (int ay=iy-1; ay < std::min(rows, iy+2); ay++) {
                    for (int ax=ix-1; ax < std::min(n, ix+2); ax++) {
                        auto gear_value_it = nums.find(std::make_tuple(ax, ay));
                        if (gear_value_it != nums.end()) {
                            int gear_value = nums[std::make_tuple(ax, ay)];
                            if (gears.find(gear_value) == gears.end())
                                gears.insert(nums[std::make_tuple(ax, ay)]);
                        }
                    }
                }
                if (gears.size() == 2) {
                    int product = 1;
                    for (auto& g: gears) {
                        product *= g;
                    }
                    result += product;
                }
            }
        }
    }

    std::cout << result << std::endl;
    return 0;
}