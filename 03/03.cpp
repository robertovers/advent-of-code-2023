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


int part_one(std::vector<std::string>& lines) {
    int result = 0;
    int n_lines = lines.size();
    int n = lines[0].size();

    std::string current, next;

    for (int iy=0; iy<lines.size(); iy++) {
        int ix = 0;
        while (ix < n) {
            current = "";
            next = lines[iy][ix];
            int ax=ix, ay=iy;
            while (std::regex_match(next, std::regex("[0-9]"))) {
                current += lines[iy][ix];
                if (ix+1 < n) {
                    next = lines[iy][ix+1];
                    ix++;
                } else {
                    next="";
                }
            }

            if (current != "") {
                int len = current.size();
                int num = stoi(current);
                int start_y = std::max(0, ay-1);
                int end_y = std::min(n_lines, ay+2);
                int start_x = std::max(0, ax-1);
                int end_x = std::min(n, ax+len+1);
                for (int iy=start_y; iy<end_y; iy++) {
                    for (int ix=start_x; ix<end_x; ix++) {
                        current = lines[iy][ix];
                        if (not std::regex_match(current, std::regex("[0-9]|\\."))) {
                            result += num;
                        }
                    }
                }
            }
            ix++;
        }
    }

    return result;
}


int part_two(std::vector<std::string>& lines) {
    int result = 0;
    int n_lines = lines.size();
    int n = lines[0].size();

    std::map<std::tuple<int, int>, int> nums;

    std::string current, next;

    for (int iy=0; iy<lines.size(); iy++) {
        int ix = 0;
        while (ix < n) {
            current = "";
            next = lines[iy][ix];
            int ax=ix, ay=iy;
            while (std::regex_match(next, std::regex("[0-9]"))) {
                current += lines[iy][ix];
                if (ix+1 < n) {
                    next = lines[iy][ix+1];
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

    for (int iy=0; iy < lines.size(); iy++) {
        for (int ix=0; ix < n; ix++) {
            if (lines[iy][ix] == '*') {
                std::set<int> gears;
                for (int ay=iy-1; ay < std::min(n_lines, iy+2); ay++) {
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

    return result;
}


int main(int argc, char* argv[]) {
    char* filename = argv[1];
    std::vector<std::string> lines;

    read_file(filename, lines);
    printf("%d\n", part_one(lines));
    printf("%d\n", part_two(lines));
}