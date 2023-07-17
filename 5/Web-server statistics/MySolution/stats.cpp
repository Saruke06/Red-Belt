#include "stats.h"

HttpRequest ParseRequest(string_view line) {
	HttpRequest result;
	size_t pos = line.find_first_not_of(' ');
	line.remove_prefix(pos);

	pos = line.find(' ');
	result.method = line.substr(0, pos);
	line.remove_prefix(pos + 1);

	pos = line.find(' ');
	result.uri = line.substr(0, pos);
	line.remove_prefix(pos + 1);

	pos = line.find(' ');
	result.protocol = line.substr(0, pos);
	return result;
}

void Stats::AddMethod(string_view method) {
	if (method != "GET" && method != "POST" &&
		method != "PUT" && method != "DELETE") {
		methods["UNKNOWN"]++;
	} else {
		methods[method]++;
	}
}

void Stats::AddUri(string_view uri) {
	if (uri != "/" && uri != "/order" &&
		uri != "/product" && uri != "/basket" && uri != "/help") {
		uris["unknown"] += 1;
	} else {
		uris[uri]++;
	}
}

const map<string_view, int>& Stats::GetMethodStats() const {
	return methods;
}

const map<string_view, int>& Stats::GetUriStats() const {
	return uris;
}
