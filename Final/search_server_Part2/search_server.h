#pragma once

#include <istream>
#include <ostream>
#include <future>
#include <set>
#include <list>
#include <vector>
#include <map>
#include <mutex>
#include <string>

using namespace std;

// Реализуйте шаблон Synchronized<T>.
// Метод GetAccess должен возвращать структуру, в которой есть поле T& value.
template <typename T>
class Synchronized {
public:
    explicit Synchronized(T initial = T())
    : value(move(initial))
    {
    }

    struct Access {
        T& ref_to_value;
        lock_guard<mutex> guard;
    };

    Access GetAccess() {
  	    return {value, lock_guard(m)};
    }
private:
    T value;
    mutex m;
};


class InvertedIndex {
public:
    void Add(const string& document);
    vector<pair<size_t, size_t>> Lookup(const string& word) const;

    const string& GetDocument(size_t id) const {
      return docs[id];
    }

    size_t GetSize() const {
    	return docs.size();
    }

private:
    map<string, vector<pair<size_t, size_t>>> index;
    vector<string> docs;
};

class SearchServer {
public:
    SearchServer() = default;
    explicit SearchServer(istream& document_input);
    void UpdateDocumentBase(istream& document_input);
    void AddQueriesStream(istream& query_input, ostream& search_results_output);

private:
    Synchronized<InvertedIndex> index;
    vector<future<void>> futures;
};
