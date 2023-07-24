#include "test_runner.h"
#include "profile.h"

#include <map>
#include <string>
#include <future>
#include <algorithm>

using namespace std;

struct Stats {
    map<string, int> word_frequences;

    void operator += (const Stats& other) {
        for (const auto& [key, value] : other.word_frequences) {
            word_frequences[key] += value;
        }
    }
};

Stats ExploreLine(const set<string>& key_words, const vector<string>& lines) {
    Stats result;
    vector<string_view> words;
    for (auto& line : lines) {
        string_view line_string_view = line;
        while (true) {
            size_t pos = line_string_view.find(' ');
            words.push_back(line_string_view.substr(0, pos));
            if (pos == line_string_view.npos)
                break;
            else {
                line_string_view.remove_prefix(pos + 1);
                while (line_string_view[0] == ' ')
                    line_string_view.remove_prefix(1);
            }
        }
    }
    for (auto& word : words){
        if (key_words.find(string(word)) != key_words.end())
            ++result.word_frequences[string(word)];
    }
    return result;
}

Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
    vector<vector<string>> threads_packs(4);
    int i = 0;
    for (string line; getline(input, line); )
    {
        threads_packs[i++ % 4].push_back(move(line));
    }
    vector<future<Stats>> futures;
    for (auto& str : threads_packs) {
        futures.push_back(async(ExploreLine, ref(key_words), ref(str)));
    }
    Stats result;
    for (auto& f : futures) {
        result += f.get();
    }
    return result;
}

void TestBasic() {
    const set<string> key_words = {"yangle", "rocks", "sucks", "all"};

    stringstream ss;
    ss << "this new yangle service really rocks\n";
    ss << "It sucks when yangle isn't available\n";
    ss << "10 reasons why yangle is the best IT company\n";
    ss << "yangle rocks others suck\n";
    ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

    const auto stats = ExploreKeyWords(key_words, ss);
    const map<string, int> expected = {
            {"yangle", 6},
            {"rocks", 2},
            {"sucks", 1}
    };
    ASSERT_EQUAL(stats.word_frequences, expected);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestBasic);
}