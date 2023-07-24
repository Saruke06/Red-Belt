#include "test_runner.h"
#include "profile.h"
#include "paginator.h"

#include <functional>
#include <future>
#include <map>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

struct Stats {
    map<string, int> word_frequences;

    void operator += (const Stats& other) {
  	    for (auto& [key, value] : other.word_frequences) {
  	  	    word_frequences[key] += value;
  	    }
    }
};

Stats ExploreLine(const set<string>& key_words, const string& line) {
	Stats result;
    istringstream ss(line);
    string s;

    while (ss >> s) {
    	if (key_words.count(s) > 0)
    		result.word_frequences[s]++;
    }

    return result;
}

Stats ExploreKeyWordsSingleThread(
    const set<string>& key_words, istream& input
) {
    Stats result;
    for (string line; getline(input, line); ) {
        result += ExploreLine(key_words, line);
    }
    return result;
}

Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
    // Реализуйте эту функцию
	vector<string> lines;
	for (string line; getline(input, line); ) {
		lines.push_back(line);
	}

	auto threads = std::thread::hardware_concurrency();

	vector<stringstream> streams(threads);

	for (auto& page : Paginate(lines, threads)) {
		stringstream ss;
		for (auto& s : page) {
			ss << s;
		}
		streams.push_back(move(ss));
	}

	vector<future<Stats>> futures;

	Stats result;
	for (int i = 0; i < threads; ++i) {
		futures.push_back(async(ExploreKeyWordsSingleThread, ref(key_words), ref(streams[i])));
	}

	for (auto& stat : futures) {
		result += stat.get();
	}

	return result;
}
//
//Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
//  vector<string> lines;
//  string line;
//	for (string line; getline(input, line); ) {
//		lines.push_back(line);
//	}
//  vector<stringstream> streams;
//
//  for (auto& file: Paginate(lines,lines.size()/4)){
//      stringstream ss("");
//      for (auto& x: file){
//          ss<<x;
//      }
//      streams.push_back(ss);
//  }
//  vector<future<Stats>> threads;
//  Stats stats;
//  for (int i=0;i<4;i++){
//      threads.push_back(async(ExploreKeyWordsSingleThread,ref(key_words),ref(streams[i])));
//  }
//  for ( auto& x : threads){
//      stats+=x.get();
//  }
//  return stats;
//  // Реализуйте эту функцию
//}


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
