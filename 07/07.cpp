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


enum hand_type {
    FIVE_KIND = 70,
    FOUR_KIND = 60,
    FULL_HOUSE = 50,
    THREE_KIND = 40,
    TWO_PAIR = 30,
    ONE_PAIR = 20,
    HIGH_CARD = 10
};


typedef struct hand_rank {
    std::string hand_string;
    int bid;
    hand_type type;
    std::tuple<int, int, int, int, int> hand_tuple;
};


std::map<char, int> get_card_values(int p) {
    std::map<char, int> card_values;
    card_values['A'] = 14;
    card_values['K'] = 13;
    card_values['Q'] = 12;
    card_values['T'] = 10;
    card_values['9'] = 9;
    card_values['8'] = 8;
    card_values['7'] = 7;
    card_values['6'] = 6;
    card_values['5'] = 5;
    card_values['4'] = 4;
    card_values['3'] = 3;
    card_values['2'] = 2;
    if (p == 1) card_values['J'] = 11; else card_values['J'] = 1;
    return card_values;
}


std::tuple<int, int, int, int, int> convert_to_tuple(std::string hand, std::map<char, int> card_values) {
    return std::make_tuple(
        card_values[hand[0]],
        card_values[hand[1]],
        card_values[hand[2]],
        card_values[hand[3]],
        card_values[hand[4]]
    );
}


bool operator< (const hand_rank& left, const hand_rank& right) {
    if (left.type == right.type) {
        return left.hand_tuple < right.hand_tuple;
    } else {
        return left.type < right.type;
    }
}


std::map<char, int> get_card_counts(std::string hand) {
    std::map<char, int> counts;
    for (char c: hand) {
        if (counts.find(c) == counts.end()) {
            counts[c] = 1;
        } else {
            counts[c]++;
        }
    }
    return counts;
}


hand_type get_hand_type_one(std::map<char, int>& card_counts) {
    bool seen_three = false, seen_two = false, seen_two_pair = false;
    for (auto const& [card, count]: card_counts) {
        switch (count) {
            case 5:
                return FIVE_KIND;
            case 4:
                return FOUR_KIND;
            case 3:
                if (seen_two) return FULL_HOUSE;
                seen_three = true;
                break;
            case 2:
                if (seen_three) return FULL_HOUSE;
                if (seen_two) seen_two_pair = true;
                seen_two = true;
                break;
        }
    }
    if (seen_three) return THREE_KIND;
    if (seen_two_pair) return TWO_PAIR;
    if (seen_two) return ONE_PAIR;
    return HIGH_CARD;
}


hand_type get_hand_type_two(std::map<char, int>& card_counts) {

    hand_type best_seen = HIGH_CARD;

    int joker_count = 0;
    if (card_counts.find('J') != card_counts.end()) {
        joker_count = card_counts['J'];
    }

    bool seen_three = false, seen_two = false, seen_two_pair = false;
    for (auto const& [card, count]: card_counts) {
        switch (count) {
            case 5:
                best_seen = std::max(best_seen, FIVE_KIND);
                break;
            case 4:
                if (joker_count > 0) best_seen = std::max(best_seen, FIVE_KIND);
                best_seen = std::max(best_seen, FOUR_KIND);
                break;
            case 3:
                if (joker_count == 2) best_seen = std::max(best_seen, FIVE_KIND);
                if (joker_count == 1) best_seen = std::max(best_seen, FOUR_KIND);
                if (seen_two) best_seen = std::max(best_seen, FULL_HOUSE);
                seen_three = true;
                break;
            case 2:
                if (joker_count == 3) best_seen = std::max(best_seen, FIVE_KIND);
                if (card != 'J' && joker_count == 2) best_seen = std::max(best_seen, FOUR_KIND);
                if (joker_count == 1) best_seen = std::max(best_seen, THREE_KIND);
                if (seen_three) best_seen = std::max(best_seen, FULL_HOUSE);
                if (seen_two) seen_two_pair = true;
                seen_two = true;
                break;
        }
    }
    if (joker_count == 3) best_seen = std::max(best_seen, FOUR_KIND);
    if (joker_count == 2) best_seen = std::max(best_seen, THREE_KIND);

    if (seen_two_pair && joker_count == 1) best_seen = std::max(best_seen, FULL_HOUSE);
    if (joker_count == 1) best_seen = std::max(best_seen, ONE_PAIR);

    if (seen_three) best_seen = std::max(best_seen, THREE_KIND);
    if (seen_two_pair) best_seen = std::max(best_seen, TWO_PAIR);
    if (seen_two) best_seen = std::max(best_seen, ONE_PAIR);

    return best_seen;
}


int part_one_two(std::vector<std::string>& lines, int p) {
    int result = 0;
    std::map<char, int> card_values = get_card_values(p);
    std::vector<hand_rank> hands;

    for (std::string line: lines) {

        std::stringstream ss(line);
        std::string hand;
        int bid;

        std::map<char, int> card_counts;
        hand_type type;
        while (ss >> hand >> bid) {
            card_counts = get_card_counts(hand);

            hand_rank hand_r;
            hand_r.bid = bid;
            hand_r.hand_string = hand;
            hand_r.hand_tuple = convert_to_tuple(hand, card_values);
            hand_r.type = get_hand_type_one(card_counts);
            hands.push_back(hand_r);
        }
    }

    std::sort(hands.begin(), hands.end());

    for (int i=0; i<hands.size(); i++) {
        result += (i+1) * hands[i].bid;
    }
    
    return result;
}


int main(int argc, char* argv[]) {
    char* filename = argv[1];
    std::vector<std::string> lines;

    read_file(filename, lines);
    printf("%d\n", part_one_two(lines, 1));
    printf("%d\n", part_one_two(lines, 2));
}