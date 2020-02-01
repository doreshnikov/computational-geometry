#include <iostream>
#include <random>
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

    point_t operator+(point_t const &other) const {
        return point_t(x + other.x, y + other.y);
    }

    point_t operator*(T const &number) const {
        return point_t(x * number, y * number);
    }

    [[nodiscard]] T sqr_length() const {
        return sqrt(sqr_length());
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
typedef long double ld;
typedef unsigned long long ull;
typedef point_t<ll> point;

struct circle {
    point_t<ld> center;
    long double radius;

    circle(point_t<ld> const &center, long double sqr_radius) : center(center), radius(sqr_radius) {}

    circle(point_t<ld> const &p1, point_t<ld> const &p2) :
            center((p1 + p2) * 0.5),
            radius((p2 - p1).sqr_length() / 2) {}

    circle(point_t<ld> const &p1, point_t<ld> const &p2, point_t<ld> const &p3) {
        long double s = abs((p2 - p1) / (p3 - p1) / 2);
        long double v1 = (p2 - p3).sqr_length() / 8 / s / s * ((p1 - p2) * (p1 - p3));
        long double v2 = (p3 - p1).sqr_length() / 8 / s / s * ((p2 - p1) * (p2 - p3));
        long double v3 = (p1 - p2).sqr_length() / 8 / s / s * ((p3 - p1) * (p3 - p2));
        center = p1 * v1 + p2 * v2 + p3 * v3;
        radius = (center - p1).sqr_length();
    }

    [[nodiscard]] bool contains(point_t<ld> const &p) const {
        return (p - center).sqr_length() <= radius;
    }
};

circle disc_two_points(vector<point> &points, unsigned int j, unsigned int i) {
    circle res(points[j].to<ld>(), points[i].to<ld>());
    for (unsigned int k = 0; k < j; k++) {
        if (!res.contains(points[k].to<ld>())) {
            res = circle(points[k].to<ld>(), points[j].to<ld>(), points[i].to<ld>());
        }
    }
    return res;
}

circle disc_one_point(vector<point> &points, unsigned int i) {
    shuffle(points.begin(), points.begin() + i, std::mt19937(std::random_device()()));
    circle res(points[0].to<ld>(), points[i].to<ld>());
    for (unsigned int j = 1; j < i; j++) {
        if (!res.contains(points[j].to<ld>())) {
            res = disc_two_points(points, j, i);
        }
    }
    return res;
}

circle min_disc(vector<point> &points) {
    shuffle(points.begin(), points.end(), std::mt19937(std::random_device()()));
    circle res(points[0].to<ld>(), points[1].to<ld>());
    for (unsigned int i = 2; i < points.size(); i++) {
        if (!res.contains(points[i].to<ld>())) {
            res = disc_one_point(points, i);
        }
    }
    return res;
}

int main() {

    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
#ifndef LOCAL
    freopen("tower.in", "r", stdin);
    freopen("tower.out", "w", stdout);
#endif

    unsigned int n;
    cin >> n;
    vector<point> points(n);
    for (point &p : points) {
        cin >> p;
    }

    circle res = points.size() > 1 ? min_disc(points) : circle(points[0].to<ld>(), 0);
    cout.precision(20);
    cout << (double) res.radius << '\n';
    cout << res.center.to<double>() << '\n';

}
