#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>

using namespace std;

template<typename T>
struct point_t {
    T x;
    T y;

    point_t() : x(0), y(0) {}

    point_t(T const &x, T const &y) : x(x), y(y) {}

    point_t(point_t<T> const &other) = default;

    template<typename U>
    bool operator==(point_t<U> const &other) const {
        return x == other.x && y == other.y;
    }

    T operator*(point_t const &other) const {
        return x * other.x + y * other.y;
    }

    T operator/(point_t const &other) const {
        return x * other.y - y * other.x;
    }

    point_t operator-(point_t const &other) const {
        return point_t(x - other.x, y - other.y);
    }

    [[nodiscard]] T sqr_length() const {
        return x * x + y * y;
    }

    template<typename U>
    [[nodiscard]] point_t<U> to() const {
        return point_t<U>{(U) (x), (U) y};
    }
};

template<typename T>
istream &operator>>(istream &in, point_t<T> &p) {
    return in >> p.x >> p.y;
}

template<typename T>
ostream &operator<<(ostream &out, point_t<T> const &p) {
    return out << p.x << ' ' << p.y;
}

typedef long long ll;
typedef unsigned long long ull;
typedef point_t<ll> point;

struct segment_t {
    point p1;
    point p2;

    [[nodiscard]] point vectorize() const {
        return p2 - p1;
    }

    [[nodiscard]] bool contains(point const &p) const {
        ll x1 = p1.x, y1 = p1.y;
        ll x2 = p2.x, y2 = p2.y;
        if (x2 < x1) {
            swap(x1, x2);
        }
        if (y2 < y1) {
            swap(y2, y1);
        }
        return (x1 <= p.x && p.x <= x2 && y1 <= p.y && p.y <= y2);
    }
};

bool inside(vector<point> const &polygon, point const &p) {
    bool count = false;
    for (unsigned int i = 0; i < polygon.size(); i++) {
        unsigned int j = (i > 0 ? i - 1 : polygon.size() - 1);
        point p1 = polygon[j];
        point p2 = polygon[i];
        if (p2.y < p1.y) {
            swap(p1, p2);
        }
        ll rotate = (p2 - p1) / (p - p1);
        if (rotate == 0 && segment_t{p1, p2}.contains(p)) {
            return true;
        }
        if (p1.y == p2.y || p.y == min(p1.y, p2.y)) {
            continue;
        } else if (rotate > 0 && p1.y < p.y && p.y <= p2.y) {
            count ^= true;
        }
    }
    return count;
}

int main() {

    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
#ifndef LOCAL
    freopen("point.in", "r", stdin);
    freopen("point.out", "w", stdout);
#endif

    unsigned int n;
    point p0;
    cin >> n >> p0;
    vector<point> points(n);
    for (point &p : points) {
        cin >> p;
    }

    cout << (inside(points, p0) ? "YES" : "NO") << '\n';

}
