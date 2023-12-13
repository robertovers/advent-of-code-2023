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


std::vector<std::string> split(const std::string &str, char delim) {
    std::vector<std::string> res;
    std::stringstream ss(str);
    std::string item;

    while (getline (ss, item, delim)) {
        res.push_back (item);
    }
    return res;
}


typedef struct spring_group {
    int start;
    int length;
    char type;
};


bool is_valid(std::string springs, std::vector<int> nums) {
    int current = 0;
    int group = 0;

    while (current < springs.size()) {
        if (springs[current] == '?') return true;
        if (springs[current] == '#') {
            int len = 1;
            while (current+1 < springs.size() && springs[current+1] == '#') {
                len++;
                current++;
            }
            if (springs[current+1] == '?' && len <= nums[group]) return true;
            if (group >= nums.size() || len != nums[group]) return false;
            group++;
        }
        current++;
    }
    if (group != nums.size()) return false;
    return true;
}


int get_next_unknown(std::string springs, int from) {
    int current = from;
    while (current < springs.size() && springs[current] != '?') current++;
    if (current >= springs.size()) return -1;
    return current;
}


int get_damage_count(std::string springs) {
    int count = 0;
    for (char c: springs) {
        if (c == '#') {
            count++;
        }
    }
    return count;
}


int valid_arrangements(std::string springs, std::vector<spring_group> spring_groups, std::vector<int> nums, int from) {
    if (!is_valid(springs, nums)) return 0;

    int next_unknown = get_next_unknown(springs, from);

    if (next_unknown == -1) {
        if (is_valid(springs, nums)) return 1;
        return 0;
    }

    int result = 0;

    std::string if_damaged = springs;
    if_damaged[next_unknown] = '#';
    result += valid_arrangements(if_damaged, spring_groups, nums, next_unknown+1);

    std::string if_good = springs;
    if_good[next_unknown] = '.';
    result += valid_arrangements(if_good, spring_groups, nums, next_unknown+1);

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


int solve(std::vector<std::string>& lines, int part) {
    int result = 0;

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

        if (part == 2) unfold(springs, nums);

        std::vector<spring_group> spring_groups;
        int j=0;
        while (j<springs.size()) {
            int start = j;
            int len = 1;
            char current = springs[j];
            while (j+1 < springs.size() && springs[j+1] == current) {
                len++;
                j++;
            }
            if (current != '.') {
                spring_groups.push_back({ start, len, springs[current] });
            }
            j++;
        }

        int arrangements = valid_arrangements(springs, spring_groups, nums, 0);
        printf("%s\n%d\n", springs.c_str(), arrangements);
        result += arrangements;
    }

    return result;
}


int main(int argc, char* argv[]) {
    char* filename = argv[1];
    std::vector<std::string> lines;

    read_file(filename, lines);
    printf("%d\n", solve(lines, 1));
    printf("%d\n", solve(lines, 2));
}