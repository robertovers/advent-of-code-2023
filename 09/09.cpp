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


std::vector<int> calculate_diffs(std::vector<int>& nums) {
    std::vector<int> diffs;
    for (int i=1; i<nums.size(); i++) {
        diffs.push_back(nums[i]-nums[i-1]);
    }
    return diffs;
}


bool all_zero(std::vector<int>& nums) {
    for (int i: nums) {
        if (i != 0) return false;
    }
    return true;
}


std::vector<std::vector<int> > calculate_sequences(std::vector<int>& nums) {
    std::vector<std::vector<int> > sequences;
    sequences.push_back(nums);

    std::vector<int> diffs = calculate_diffs(nums);
    sequences.push_back(diffs);

    while (not all_zero(diffs)) {
        diffs = calculate_diffs(diffs);
        sequences.push_back(diffs);
    }

    return sequences;
}


void extrapolate_sequences(std::vector<std::vector<int> >& seqs) {
    seqs.back().push_back(0);
    for (int i=seqs.size()-2; i>=0; i--) {
        seqs[i].push_back(seqs[i].back() + seqs[i+1].back()); 
    }
}


void extrapolate_sequences_back(std::vector<std::vector<int> >& seqs) {
    seqs.back().push_back(0);
    for (int i=seqs.size()-2; i>=0; i--) {
        seqs[i].insert(seqs[i].begin(), seqs[i][0] - seqs[i+1][0]); 
    }
}


int part_one(std::vector<std::string>& lines) {
    int result = 0;

    for (std::string line: lines) {

        std::vector<int> nums;
        std::stringstream ss(line);

        int i;
        while (ss >> i) {
            nums.push_back(i);
        }

        std::vector<std::vector<int> > seqs = calculate_sequences(nums);
        extrapolate_sequences(seqs);

        result += seqs[0].back();
    }

    return result;
}


int part_two(std::vector<std::string>& lines) {
    int result = 0;

    for (std::string line: lines) {

        std::vector<int> nums;
        std::stringstream ss(line);

        int i;
        while (ss >> i) {
            nums.push_back(i);
        }

        std::vector<std::vector<int> > seqs = calculate_sequences(nums);
        extrapolate_sequences_back(seqs);

        result += seqs[0][0];
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