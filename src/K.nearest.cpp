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
    unsigned int id;

    point_t() : x(0), y(0), id(0) {}

    point_t(T const &x, T const &y) : x(x), y(y), id(0) {}

    point_t(point_t<T> const &other) = default;

    T operator*(point_t const &other) const {
        return x * other.x + y * other.y;
    }

    T operator/(point_t const &other) const {
        return x * other.y - y * other.x;
    }

    point_t operator+(point_t const &other) const {
        return point_t(x + other.x, y + other.y);
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
    static unsigned int last_id = 0;
    p.id = last_id++;
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

vector<vector<int>> lines;
vector<vector<int>> l, r, t, b;

unsigned int nearest(point const &p) {
    ll d = min(min(l[p.y][p.x], r[p.y][p.x]), min(t[p.y][p.x], b[p.y][p.x]));
    unsigned int res = 0;
    if (p.x - d >= 0 && lines[p.y][p.x - d] > -1) {
        res = lines[p.y][p.x - d];
    }
    if (p.x + d <= 2000 && lines[p.y][p.x + d] > -1) {
        res = lines[p.y][p.x + d];
    }
    if (p.y - d >= 0 && lines[p.y - d][p.x] > -1) {
        res = lines[p.y - d][p.x];
    }
    if (p.y + d <= 2000 && lines[p.y + d][p.x] > -1) {
        res = lines[p.y + d][p.x];
    }
    d = d * d;
    int h = 1;
    while (h * h < d) {
        if (p.y - h >= 0) {
            int lh = l[p.y - h][p.x], rh = r[p.y - h][p.x];
            if (h * h + lh * lh < d) {
                d = h * h + lh * lh;
                res = lines[p.y - h][p.x - lh];
            }
            if (h * h + rh * rh < d) {
                d = h * h + rh * rh;
                res = lines[p.y - h][p.x + rh];
            }
        }
        if (p.y + h <= 2000) {
            int lh = l[p.y + h][p.x], rh = r[p.y + h][p.x];
            if (h * h + lh * lh < d) {
                d = h * h + lh * lh;
                res = lines[p.y + h][p.x - lh];
            }
            if (h * h + rh * rh < d) {
                d = h * h + rh * rh;
                res = lines[p.y + h][p.x + rh];
            }
        }
        h++;
    }
    return res;
}

int main() {

    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
#ifndef LOCAL
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif

    unsigned int n, m;
    cin >> n >> m;

    vector<point> points(n);
    for (point &p : points) {
        cin >> p;
    }
    vector<point> queries(m);
    for (point &p : queries) {
        cin >> p;
    }

    lines.assign(2001, vector<int>(2001, -1));
    for (point const &p : points) {
        lines[p.y + 1000][p.x + 1000] = p.id;
    }
    l.assign(2001, vector<int>(2001, 2001));
    r.assign(2001, vector<int>(2001, 2001));
    t.assign(2001, vector<int>(2001, 2001));
    b.assign(2001, vector<int>(2001, 2001));
    for (unsigned int i = 0; i < 2001; i++) {
        for (unsigned int j = 0; j < 2001; j++) {
            l[i][j] = (lines[i][j] > -1 ? 0 : (j > 0 ? l[i][j - 1] + 1 : 2001));
            t[j][i] = (lines[j][i] > -1 ? 0 : (j > 0 ? t[j - 1][i] + 1 : 2001));
        }
        for (int j = 2000; j > -1; j--) {
            r[i][j] = (lines[i][j] > -1 ? 0 : (j < 2000 ? r[i][j + 1] + 1 : 2001));
            b[j][i] = (lines[j][i] > -1 ? 0 : (j < 2000 ? b[j + 1][i] + 1 : 2001));
        }
    }

    for (point const &p : queries) {
        cout << nearest(p + point(1000, 1000)) + 1 << '\n';
    }

}
