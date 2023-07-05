#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>
#include <map>

using namespace std;

class ReadingManager {
public:
  ReadingManager()
      : users_to_pages({}),
		rating(1001, 0) {}

  void Read(int user_id, int page_count) {
	  if (users_to_pages.find(user_id) == users_to_pages.end()) {
		  AddUser(user_id);
	  }

	  const int now = users_to_pages[user_id];
	  for (int i = now + 1; i <= page_count; ++i) {
		  --rating[i];
	  }
	  users_to_pages[user_id] = page_count;
  }

  double Cheer(int user_id) const {
    if (users_to_pages.find(user_id) == users_to_pages.end()) {
      return 0;
    }
    const int user_count = GetUserCount();
    if (user_count == 1) {
      return 1;
    }

    int page_count = rating[users_to_pages.at(user_id)];
    return (page_count * 1.0) / (user_count - 1);
  }

private:
  map<int, int> users_to_pages;
  vector<int> rating;

  int GetUserCount() const {
    return users_to_pages.size();
  }

  void AddUser(int user_id) {
	  users_to_pages[user_id] = 0;
	  for(size_t i = 1; i < rating.size(); ++i) {
		  ++rating[i];
	  }
  }
};


int main() {
  // Для ускорения чтения данных отключается синхронизация
  // cin и cout с stdio,
  // а также выполняется отвязка cin от cout
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  ReadingManager manager;

  int query_count;
  cin >> query_count;

  for (int query_id = 0; query_id < query_count; ++query_id) {
    string query_type;
    cin >> query_type;
    int user_id;
    cin >> user_id;

    if (query_type == "READ") {
      int page_count;
      cin >> page_count;
      manager.Read(user_id, page_count);
    } else if (query_type == "CHEER") {
      cout << setprecision(6) << manager.Cheer(user_id) << "\n";
    }
  }

  return 0;
}
