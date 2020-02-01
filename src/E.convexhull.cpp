#include <iostream>
#include <string>
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

vector<point> convex_hull(vector<point> &points) {
    point p = *min_element(points.begin(), points.end(), [](point const &p1, point const &p2) {
        return p1.y < p2.y || (p1.y == p2.y && p1.x > p2.x);
    });
    sort(points.begin(), points.end(), [&p](point const &p1, point const &p2) {
        ll rotate = (p1 - p) / (p2 - p);
        return rotate > 0 || (rotate == 0 && (p1 - p).to<ull>().sqr_length() < (p2 - p).to<ull>().sqr_length());
    });
    vector<point> result{p};
    for (unsigned int i = 1; i < points.size(); i++) {
        point next = points[i];
        while (result.size() >= 2) {
            point v_prev = result.back() - result[result.size() - 2];
            point v_next = next - result.back();
            ll rotate = v_next / v_prev;
            if (rotate >= 0) {
                result.pop_back();
            } else {
                break;
            }
        }
        result.push_back(next);
    }
    return result;
}

int main() {

    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
#ifndef LOCAL
    freopen("hull.in", "r", stdin);
    freopen("hull.out", "w", stdout);
#endif

    unsigned int n;
    cin >> n;

    vector<point> points(n);
    for (point &p : points) {
        cin >> p;
    }

    auto res = convex_hull(points);
    cout << res.size() << '\n';
    for (point const &p: res) {
        cout << p << '\n';
    }

}
