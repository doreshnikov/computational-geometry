#include <cstddef>
#include <gmpxx.h>
#include <iostream>
#include <string>

using namespace std;

#ifdef LOCAL

pair<string, string> genTest(int id) {
    return {"2", "3"};
}

#else
#include <tests.h>
#endif

int main() {

    int test_id;
    cin >> test_id;

    auto test = genTest(test_id);
    mpz_class a(test.first), b(test.second);
    cout << mpz_class(a + b).get_str() << '\n';

}
