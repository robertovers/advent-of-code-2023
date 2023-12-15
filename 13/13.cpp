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


std::vector<int> get_vertical_mirrors(std::vector<std::string> pattern) {
    std::vector<int> mirrors;

    for (int x=0; x<pattern[0].size()-1; x++) {
        bool mirror = true;
        for (int y=0; y<pattern.size(); y++) {
            int left_it = x;
            int right_it = x+1;
            if (pattern[y][left_it] != pattern[y][right_it]) { mirror = false; break; }
            while (left_it >= 0 && right_it < pattern[0].size() && pattern[y][left_it] == pattern[y][right_it]) {
                left_it--;
                right_it++;
            }
            if (left_it != -1 && right_it != pattern[0].size()) { mirror = false; break; }; 
        }
        if (mirror) mirrors.push_back(x+1);
    }

    return mirrors;
}


std::vector<int> get_horizontal_mirrors(std::vector<std::string> pattern) {
    std::vector<int> mirrors;

    for (int y=0; y<pattern.size()-1; y++) {
        bool mirror = true;
        for (int x=0; x<pattern[0].size(); x++) {
            int up_it = y;
            int down_it = y+1;
            if (pattern[up_it][x] != pattern[down_it][x]) { mirror = false; break; }
            while (up_it >= 0 && down_it < pattern.size() && pattern[up_it][x] == pattern[down_it][x]) {
                up_it--;
                down_it++;
            }
            if (up_it != -1 && down_it != pattern.size()) { mirror = false; break; }; 
        }
        if (mirror) mirrors.push_back(y+1);
    }

    return mirrors;
}


void flip_char(int x, int y, std::vector<std::string>& pattern) {
    if (pattern[y][x] == '#') {
        pattern[y][x] = '.';
    } else {
        pattern[y][x] = '#';
    }
}


int part_one(std::vector<std::string> lines) {

    std::vector<std::vector<std::string> > patterns;
    std::vector<std::string> current_pattern;

    for (std::string line: lines) {
        if (line != "") {
            current_pattern.push_back(line);
        } else {
            patterns.push_back(current_pattern);
            current_pattern.clear();
        }
    }
    patterns.push_back(current_pattern);

    int result = 0;
    for (auto& p: patterns) {
        std::vector<int> v_mirrors = get_vertical_mirrors(p);
        std::vector<int> h_mirrors = get_horizontal_mirrors(p);
        for (int x: v_mirrors) result += x;
        for (int x: h_mirrors) result += 100 * x;
    }

    return result;
}


int part_two(std::vector<std::string> lines) {

    std::vector<std::vector<std::string> > patterns;
    std::vector<std::string> current_pattern;
    for (std::string line: lines) {
        if (line != "") {
            current_pattern.push_back(line);
        } else {
            patterns.push_back(current_pattern);
            current_pattern.clear();
        }
    }
    patterns.push_back(current_pattern);

    int result = 0;
    for (auto& p: patterns) {

        for (auto l: p) {
            printf("%s\n", l.c_str());
        }
        printf("\n");

        std::vector<std::pair<int, int> > smudges = { {-1, -1}, {-1, -1} };
        std::vector<std::pair<int, int> > last_incorrect = { {-1, -1}, {-1, -1} };

        // find vertical mirrors
        for (int x=0; x<p[0].size()-1; x++) {
            int correct_matches = 0;
            for (int y=0; y<p.size(); y++) {
                int left_it = x;
                int right_it = x+1;
                int correct_matches_row = 0;
                while (left_it >= 0 && right_it < p[0].size()) {
                    if (p[y][left_it] == p[y][right_it]) {
                        correct_matches_row++;
                    } else {
                        std::pair<int, int> last_incorrect_1 = { left_it, y };
                        std::pair<int, int> last_incorrect_2 = { right_it, y };
                        last_incorrect[0] = last_incorrect_1;
                        last_incorrect[1] = last_incorrect_2;
                    }
                    left_it--;
                    right_it++;
                }
                correct_matches_row = 2 * correct_matches_row + std::max(left_it+1, (int) p[0].size()-right_it);
                correct_matches += correct_matches_row;
            }
            printf("%d %d\n", x, correct_matches);
            if (correct_matches == p.size() * p[0].size() - 2) {
                std::pair<int, int> smudge_1 = { last_incorrect[0].first, last_incorrect[0].second };
                std::pair<int, int> smudge_2 = { last_incorrect[1].first, last_incorrect[1].second };
                smudges[0] = smudge_1;
                smudges[1] = smudge_2;
                printf("smudge: (%d,%d) (%d,%d)\n", smudge_1.first, smudge_1.second, smudge_2.first, smudge_2.second); 
                break;
            };
        }

        // find horizontal mirrors
        for (int y=0; y<p.size()-1; y++) {
            int correct_matches = 0;
            for (int x=0; x<p[0].size(); x++) {
                int up_it = y;
                int down_it = y+1;
                int correct_matches_col = 0;
                while (up_it >= 0 && down_it < p.size()) {
                    if (p[up_it][x] == p[down_it][x]) {
                        correct_matches_col++;
                    } else {
                        std::pair<int, int> last_incorrect_1 = { x, up_it };
                        std::pair<int, int> last_incorrect_2 = { x, down_it };
                        last_incorrect[0] = last_incorrect_1;
                        last_incorrect[1]= last_incorrect_2;
                    }
                    up_it--;
                    down_it++;
                }
                correct_matches_col = 2 * correct_matches_col + std::max(up_it+1, (int) p.size()-down_it);
                correct_matches += correct_matches_col;
            }
            printf("%d %d\n", y, correct_matches);
            if (correct_matches == p.size() * p[0].size() - 2) {
                std::pair<int, int> smudge_1 = { last_incorrect[0].first, last_incorrect[0].second };
                std::pair<int, int> smudge_2 = { last_incorrect[1].first, last_incorrect[1].second };
                smudges[0] = smudge_1;
                smudges[1] = smudge_2;
                printf("smudge: (%d,%d) (%d,%d)\n", smudge_1.first, smudge_1.second, smudge_2.first, smudge_2.second); 
                break;
            };
        }

        for (int i=0; i<2; i++) {
            std::vector<int> old_h_mirrors = get_horizontal_mirrors(p);
            std::vector<int> old_v_mirrors = get_vertical_mirrors(p);
            flip_char(smudges[i].first, smudges[i].second, p);
            std::vector<int> new_h_mirrors = get_horizontal_mirrors(p);
            std::vector<int> new_v_mirrors = get_vertical_mirrors(p);
            flip_char(smudges[i].first, smudges[i].second, p);

            // check smudge only creates one new mirror
            if ((old_h_mirrors != new_h_mirrors && new_h_mirrors.size() >= old_h_mirrors.size())
                || (old_v_mirrors != new_v_mirrors && new_v_mirrors.size() >= old_v_mirrors.size())) {

                for (int x: new_v_mirrors) {
                    if (std::find(old_v_mirrors.begin(), old_v_mirrors.end(), x) == old_v_mirrors.end())
                        result += x;
                }
                for (int x: new_h_mirrors) {
                    if (std::find(old_h_mirrors.begin(), old_h_mirrors.end(), x) == old_h_mirrors.end())
                        result += 100 * x;
                }
                break;
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