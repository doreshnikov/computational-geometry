#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

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

    point_t operator-() const {
        return point_t(-x, -y);
    }

    [[nodiscard]] T sqr_length() const {
        return x * x + y * y;
    }

    [[nodiscard]] T length() const {
        return sqrt(sqr_length());
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

ull sqr_diameter(vector<point> &polygon) {
    unsigned int x = 0, y = 0;
    ull result = 0;
    for (unsigned int cnt = 0; cnt < polygon.size(); cnt++, x++) {
        point cur = polygon[x];
        while (true) {
            unsigned int next = (y < polygon.size() - 1 ? y + 1 : 0);
            if ((polygon[next] - cur).to<ull>().sqr_length() <= (polygon[y] - cur).to<ull>().sqr_length()) {
                break;
            }
            y = next;
        }
        result = max(result, (polygon[y] - cur).to<ull>().sqr_length());
    }
    return result;
}

int main() {

    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
#ifndef LOCAL
    freopen("diameter.in", "r", stdin);
    freopen("diameter.out", "w", stdout);
#endif

    unsigned int n;
    cin >> n;

    vector<point> points(n);
    for (point &p : points) {
        cin >> p;
    }
    auto polygon = convex_hull(points);

    cout.precision(1000);
    if (polygon.size() == 1) {
        cout << "0\n";
    } else if (polygon.size() == 2) {
        cout << (polygon[0] - polygon[1]).to<double>().length() << '\n';
    } else {
        cout << sqrt((double) sqr_diameter(polygon)) << '\n';
    }

}
