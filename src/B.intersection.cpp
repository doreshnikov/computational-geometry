#include <cstddef>
#include <gmpxx.h>
#include <iostream>
#include <string>

using namespace std;

#ifdef LOCAL

vector<double> genTest(int id) {
    return {
            0, 2, 2, 1,
            1, 0, 2, 2,

            1, 3, 3, 3,
            2, 3, 2, 4,

            3, 0, 4, 0,
            3, 2, 4, 1
    };
}

#else
#include <tests.h>
#endif

template<typename T = double>
struct point_t {
    T x;
    T y;

    T operator*(point_t const &other) const {
        return x * other.x + y * other.y;
    }

    T operator^(point_t const &other) const {
        return x * other.y - y * other.x;
    }

    point_t operator-(point_t const &other) const {
        return point_t{x - other.x, y - other.y};
    }

    template<typename U>
    point_t<U> to() const {
        return point_t<U>{x, y};
    }
};

struct segment_t {
    point_t<> p1;
    point_t<> p2;

    [[nodiscard]] point_t<> vectorize() const {
        return p2 - p1;
    }

    [[nodiscard]] bool contains(point_t<> const &p) const {
        double x1 = p1.x, y1 = p1.y;
        double x2 = p2.x, y2 = p2.y;
        if (x2 < x1) {
            swap(x1, x2);
        }
        if (y2 < y1) {
            swap(y2, y1);
        }
        return (x1 <= p.x && p.x <= x2 && y1 <= p.y && p.y <= y2);
    }
};

template<typename T>
int sign(T const &value) {
    return (value == 0 ? 0 : (value > 0 ? 1 : -1));
}

template<typename T>
T orientation(point_t<T> const &top, point_t<T> const &a, point_t<T> const &b) {
    return (a - top) ^ (b - top);
}

int orientation_sign(point_t<> const &top, point_t<> const &a, point_t<> const &b) {
    auto angle = orientation<double>(top, a, b);
    if (abs(angle) > 1e-8) {
        return sign(angle);
    } else {
        return sign(orientation<mpq_class>(top.to<mpq_class>(), a.to<mpq_class>(), b.to<mpq_class>()));
    }
}

int over_segment(segment_t const &s1, segment_t const &s2) {
    int sign1 = orientation_sign(s1.p1, s1.p2, s2.p1);
    int sign2 = orientation_sign(s1.p1, s1.p2, s2.p2);
    if (sign1 == 0 && sign2 == 0) {
        return 0;
    } else {
        return sign1 == sign2 ? 1 : -1;
    }
}

bool intersects(vector<double> const &data, int index) {
    segment_t s1{
            {data[index * 8 + 0], data[index * 8 + 1]},
            {data[index * 8 + 2], data[index * 8 + 3]}
    };
    segment_t s2{
            {data[index * 8 + 4], data[index * 8 + 5]},
            {data[index * 8 + 6], data[index * 8 + 7]}
    };

    int over_s1 = over_segment(s1, s2);
    int over_s2 = over_segment(s2, s1);
    if (over_s1 == -1 && over_s2 == -1) {
        return true;
    } else if (over_s1 == 1 || over_s2 == 1) {
        return false;
    } else {
        return s1.contains(s2.p1) || s1.contains(s2.p2) || s2.contains(s1.p1) || s2.contains(s1.p2);
    }
}

int main() {

    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int test_id;
    cin >> test_id;

    auto test = genTest(test_id);
    for (int i = 0; i < test.size() / 8; i++) {
        cout << (intersects(test, i) ? "Y" : "N");
    }
    cout << '\n';

}
