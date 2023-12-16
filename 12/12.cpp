#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <regex>
#include <set>
#include <map>
#include <tuple>
#include <unordered_map>


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


std::vector<std::string> split(const std::string &str, char delim) {
    std::vector<std::string> res;
    std::stringstream ss(str);
    std::string item;

    while (getline (ss, item, delim)) {
        res.push_back (item);
    }
    return res;
}


long arrangements(std::string& springs, std::vector<int>& lens, int i, int cur, int j, std::unordered_map<std::string, long>& memo) {
    //printf("%d %d %d %d\n", i, cur, j, lens[j]);
    std::string key = std::to_string(i) + "." + std::to_string(cur) + "." + std::to_string(j);
    if (memo.find(key) != memo.end()) return memo[key];

    if (i == springs.size()) {
        // end of string & final group -> arrangement found
        if ((j == lens.size() && cur == 0) || (j == lens.size()-1 && cur == lens[j])) {
            memo[key] = 1;
            return 1;
        }
        memo[key] = 0;
        return 0;
    } else if ((j == lens.size() && cur == 0) || (j == lens.size()-1 && cur == lens[j])) {
        // final group, check if any more # remaining
        int rem = (springs.find("#", i) == std::string::npos);
        memo[key] = rem;
        return rem;
    }

    long result = 0;

    if (cur == lens[j] && j < lens.size()) {
        if (springs[i] == '#') {
            memo[key] = 0; 
            return 0;  // extended past current group
        }
        result += arrangements(springs, lens, i+1, 0, j+1, memo);  // end of current group
    } else {
        if (springs[i] == '#') {
            result += arrangements(springs, lens, i+1, cur+1, j, memo);
        } else if (springs[i] == '.') {
            if (cur != 0 && cur < lens[j]) {
                memo[key] = 0; 
                return 0;  // current group too small 
            }
            result += arrangements(springs, lens, i+1, 0, j, memo);
        } else {
            if (j < lens.size())
                result += arrangements(springs, lens, i+1, cur+1, j, memo);  // '?' -> '#'
            if (cur == 0 || cur == lens[j]) {
                result += arrangements(springs, lens, i+1, cur, j, memo);  // '?' -> '.'
            }
        }
    }

    memo[key] = result; 
    return result;
}


void unfold(std::string& springs, std::vector<int>& nums) {
    std::string spring_p = springs;
    std::vector<int> num_p = nums;
    for (int i=0; i<4; i++) {
        springs += "?" + spring_p;
        for (auto x: num_p) {
            nums.push_back(x);
        }
    }
}


long solve(std::vector<std::string>& lines, bool part2) {
    long result = 0;

    for (std::string line: lines) {
        std::vector<std::string> items = split(line, ' ');
        std::string springs = items[0];
        std::vector<int> nums;

        int i;
        char c;
        std::stringstream ss(items[1]);
        while (ss >> i) {
            nums.push_back(i);
            ss >> c;
        }

        if (part2) unfold(springs, nums);

        std::unordered_map<std::string, long> memo;

        long k = arrangements(springs, nums, 0, 0, 0, memo);
        printf("%s\n%ld\n", springs.c_str(), k);
        result += k;
    }

    return result;
}


int main(int argc, char* argv[]) {
    char* filename = argv[1];
    std::vector<std::string> lines;

    read_file(filename, lines);
    printf("%ld\n", solve(lines, false));
    printf("%ld\n", solve(lines, true));
}