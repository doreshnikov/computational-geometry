#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>

using namespace std;

template<typename T>
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
ostream &operator<<(ostream &out, point_t<T> &p) {
    return out << p.x << ' ' << p.y;
}

typedef point_t<long long> point;

unsigned long long sqr_length = numeric_limits<long long>::max();
long long length = numeric_limits<int>::max();
point res1, res2;

void relax(point const &p1, point const &p2) {
    unsigned long long sqr_dist = (p2 - p1).to<unsigned long long>().sqr_length();
    if (sqr_length > sqr_dist) {
        sqr_length = sqr_dist;
        length = sqrt(sqr_length);
        res1 = p1;
        res2 = p2;
    }
}

vector<point> collect(vector<point> const &points, unsigned int l, unsigned int r) {
    vector<point> result;
    if (r - l == 1) {
        result.push_back(points[l]);
    } else {
        unsigned int m = (l + r) / 2;
        auto ldata = collect(points, l, m);
        auto rdata = collect(points, m, r);

        unsigned int il = 0, ir = 0, hwinl = 0, hwinr = 0;
        while (il < ldata.size() || ir < rdata.size()) {
            if ((il < ldata.size() && ldata[il].y <= rdata[ir].y) || ir == rdata.size()) {
                if (ldata[il].x > points[m].x - length) {
                    while (hwinl < rdata.size() && rdata[hwinl].y <= ldata[il].y - length) hwinl++;
                    while (hwinr < rdata.size() && rdata[hwinr].y < ldata[il].y + length) hwinr++;
                    for (unsigned int near = hwinl; near < hwinr; near++) {
                        relax(ldata[il], rdata[near]);
                    }
                }
                result.push_back(ldata[il++]);
            } else {
                result.push_back(rdata[ir++]);
            }
        }
    }
    return result;
}

int main() {

    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
#ifndef LOCAL
    freopen("rendezvous.in", "r", stdin);
    freopen("rendezvous.out", "w", stdout);
#endif

    unsigned int n;
    cin >> n;

    vector<point> points(n);
    for (point &p : points) {
        cin >> p;
    }
    sort(points.begin(), points.end(), [](point const &p1, point const &p2) {
        return p1.x < p2.x;
    });
    collect(points, 0, n);
    cout << res1 << ' ' << res2 << '\n';

}
