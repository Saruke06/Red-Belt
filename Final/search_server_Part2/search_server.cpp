#include "search_server.h"
#include "iterator_range.h"
#include "profile.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>

vector<string> SplitIntoWords(const string& line) {
    istringstream words_input(line);
    return {istream_iterator<string>(words_input), istream_iterator<string>()};
}

void UpdateDocumentBaseSingleThread(istream& document_input, Synchronized<InvertedIndex>& index) {
    InvertedIndex new_index;

    for (string current_document; getline(document_input, current_document); ) {
        new_index.Add(move(current_document));
    }

    index.GetAccess().ref_to_value = move(new_index);
}

SearchServer::SearchServer(istream& document_input) {
	UpdateDocumentBaseSingleThread(document_input, index);
}

void SearchServer::UpdateDocumentBase(istream& document_input) {
	futures.push_back(
		async(UpdateDocumentBaseSingleThread, ref(document_input), ref(index))
	);
}

struct TotalDuration {
	string message;
	steady_clock::duration value;
	explicit TotalDuration(const string& msg = "")
		: message(msg + ": ")
		, value(0)
	{
	}
	~TotalDuration() {
		ostringstream os;
		os << message
			<< duration_cast<milliseconds>(value).count()
			<< " ms" << endl;
			cerr << os.str();
	}
};

class AddDuration {
public:
	explicit AddDuration(steady_clock::duration& dest)
		: add_to(dest)
		, start(steady_clock::now())
	{
	}
	explicit AddDuration(TotalDuration& dest)
		: AddDuration(dest.value)
	{
	}
	~AddDuration() {
		add_to += steady_clock::now() - start;
	}
	private:
		steady_clock::duration& add_to;
		steady_clock::time_point start;
};
#define ADD_DURATION(value) \
AddDuration UNIQ_ID(__LINE__){value};

void AddQueriesStreamSingleThread(
    istream& query_input, ostream& search_results_output, Synchronized<InvertedIndex>& index
) {
//	TotalDuration AddQueriesStream("Total AddQueriesStream");
//	ADD_DURATION(AddQueriesStream);
//	TotalDuration forming_result("Total forming_result");
//	TotalDuration sort_time("Total sort_time");
//	TotalDuration index_call("Total index_call");
//	TotalDuration parse("Total parse");

	auto ind = index.GetAccess().ref_to_value;

	vector<pair<size_t, size_t>> docid_count(ind.GetSize(), {0, 0});

    for (string current_query; getline(query_input, current_query); ) {
    	vector<string> words;
    	{
//    		ADD_DURATION(parse);
    		words = SplitIntoWords(current_query);
    	}


        {
//        	ADD_DURATION(index_call);
            for (const auto& word : words) {
                for (const auto docid : ind.Lookup(word)) {
                    docid_count[docid.first].first = docid.first;
                    docid_count[docid.first].second += docid.second;
                }
            }
        }

        {
//        	ADD_DURATION(sort_time);
            partial_sort(
                begin(docid_count),
				Head(docid_count, 5).end(),
                end(docid_count),
                [](pair<size_t, size_t> lhs, pair<size_t, size_t> rhs) {
                    int64_t lhs_docid = lhs.first;
                    auto lhs_hit_count = lhs.second;
                    int64_t rhs_docid = rhs.first;
                    auto rhs_hit_count = rhs.second;
                    return make_pair(lhs_hit_count, -lhs_docid) > make_pair(rhs_hit_count, -rhs_docid);
                }
            );
        }

        {
//            ADD_DURATION(forming_result);

            search_results_output << current_query << ':';
            for (auto [docid, hitcount] : Head(docid_count, 5)) {
            	if (hitcount != 0)
					search_results_output << " {"
						<< "docid: " << docid << ", "
						<< "hitcount: " << hitcount << '}';
            }
            search_results_output << endl;
        }
    	docid_count.assign(docid_count.size(), {0, 0});
    }
}

void SearchServer::AddQueriesStream(istream& query_input, ostream& search_results_output) {
	futures.push_back(
		async(AddQueriesStreamSingleThread, ref(query_input),
				ref(search_results_output), ref(index))
	);
}

void InvertedIndex::Add(const string& document) {
    docs.push_back(document);
    const size_t docid = docs.size() - 1;

    for (const auto& word : SplitIntoWords(document)) {
    	if (index[word].size() == 0) {
    		index[word].push_back({docid, 1});
    	} else if (index[word].back().first != docid) {
    		index[word].push_back({docid, 1});
    	} else {
    		index[word].back().second++;
    	}
    }
}

vector<pair<size_t, size_t>> InvertedIndex::Lookup(const string& word) const {
    if (auto it = index.find(word); it != index.end()) {
        return it->second;
    } else {
        return {};
    }
}
