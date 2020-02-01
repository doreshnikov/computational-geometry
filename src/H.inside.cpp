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

enum class position {
    INSIDE,
    BORDER,
    OUTSIDE
};

string to_string(position const &p) {
    switch (p) {
        case position::INSIDE:
            return "INSIDE";
        case position::BORDER:
            return "BORDER";
        case position::OUTSIDE:
            return "OUTSIDE";
    }
}

position inside(vector<point> const &polygon, point const &p) {
    if (p.x < polygon[0].x) {
        return position::OUTSIDE;
    }
    point location = p - polygon[0];
    unsigned int l = 0, r = polygon.size();
    while (r - l > 1) {
        unsigned int m = (l + r) / 2;
        point diagonal = polygon[m] - polygon[0];
        ll rotate = location / diagonal;
        if (rotate >= 0) {
            r = m;
        } else {
            l = m;
        }
    }
    if ((polygon[polygon.size() - 1] - polygon[0]) / location == 0 &&
        segment_t{polygon[0], polygon[polygon.size() - 1]}.contains(p)) {
        return position::BORDER;
    }
    if (l == polygon.size() - 1) {
        return position::OUTSIDE;
    } else if (l == 0) {
        if ((polygon[1] - polygon[0]) / location == 0 && segment_t{polygon[0], polygon[1]}.contains(p)) {
            return position::BORDER;
        } else {
            return position::OUTSIDE;
        }
    } else {
        point p1 = polygon[l];
        point p2 = polygon[r];
        ll rotate = (p2 - p1) / (p - p1);
        if (rotate > 0) {
            return position::INSIDE;
        } else if (rotate == 0) {
            return position::BORDER;
        } else {
            return position::OUTSIDE;
        }
    }
}

int main() {

    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
#ifndef LOCAL
    freopen("inside.in", "r", stdin);
    freopen("inside.out", "w", stdout);
#endif

    unsigned int n;
    cin >> n;
    vector<point> points(n);
    for (point &p : points) {
        cin >> p;
    }
    if ((points[2] - points[1]) / (points[1] - points[0]) > 0) {
        reverse(points.begin(), points.end());
    }
    rotate(
            points.begin(),
            min_element(points.begin(), points.end(), [](point const &p1, point const &p2) {
                return p1.x < p2.x || ((p1.x == p2.x) && p1.y < p2.y);
            }),
            points.end()
    );

    unsigned int k;
    cin >> k;
    for (unsigned int i = 0; i < k; i++) {
        point p;
        cin >> p;
        cout << to_string(inside(points, p)) << '\n';
    }

}
