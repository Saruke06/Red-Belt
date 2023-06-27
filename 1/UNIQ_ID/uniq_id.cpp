#include <string>
#include <vector>

using namespace std;

// Реализуйте этот макрос так, чтобы функция main компилировалась


#define TOKENCANC2(number) line_##number

#define TOKENCANC(number) TOKENCANC2(number)

#define UNIQ_ID TOKENCANC(__LINE__)

int main() {
  int UNIQ_ID = 0;
  string UNIQ_ID = "hello";
  vector<string> UNIQ_ID = {"hello", "world"};
  vector<int> UNIQ_ID = {1, 2, 3, 4};
}
