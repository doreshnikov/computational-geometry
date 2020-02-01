#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <set>
#include <map>

using namespace std;

long double eps = 1e-10;

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

    bool operator<(point_t<T> const &other) const {
        return other.x - x > eps || (abs(x - other.x) < eps && other.y - y > eps);
    }

    bool operator==(point_t<T> const &other) const {
        return abs(x - other.x) < eps && abs(y - other.y) < eps;
    }

    bool operator!=(point_t<T> const &other) const {
        return !(*this == other);
    }

    long double atan() const {
        return atan2(y, x);
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
typedef point_t<ld> point;

int sign(long double value) {
    return (value == 0 ? 0 : value > 0 ? 1 : -1);
}

ld det(ld a, ld b, ld c, ld d) {
    return a * d - b * c;
}

struct line {
    ld a;
    ld b;
    ld c;

    line(point const &p1, point const &p2) {
        a = p2.y - p1.y;
        b = p1.x - p2.x;
        c = -(a * p1.x + b * p1.y);
    }

    [[nodiscard]] bool parallel(line const &other) const {
        return abs(det(a, b, other.a, other.b)) < eps;
    }

    [[nodiscard]] point intersect(line const &other) const {
        ld alpha = det(a, b, other.a, other.b);
        return point(
                det(b, c, other.b, other.c) / alpha,
                det(c, a, other.c, other.a) / alpha
        );
    }
};

ld go(vector<point> const &points, vector<vector<unsigned int>> const &graph, vector<vector<bool>> &vis,
      unsigned int i, unsigned int j) {
    vector<unsigned int> border{i};
    while (true) {
        vis[i][j] = true;
        unsigned int next = graph[i][j];
        auto edge = upper_bound(
                graph[next].begin(), graph[next].end(), i,
                [next, &points](unsigned int p1, unsigned int p2) {
                    return (points[p1] - points[next]).atan() < (points[p2] - points[next]).atan();
                }
        );
        unsigned int idx = (edge - graph[next].begin()) % graph[next].size();
        if (vis[next][idx]) {
            break;
        } else {
            border.push_back(next);
            i = next;
            j = idx;
        }
    }

    ld area = 0;
    for (unsigned int t = 0; t < border.size(); t++) {
        unsigned int r = (t < border.size() - 1 ? t + 1 : 0);
        point p1 = points[border[t]], p2 = points[border[r]];
        area += (p2.x - p1.x) * (p2.y + p1.y) / 2;
    }
    return abs(area);
}

vector<ld> areas(vector<line> const &lines) {
    vector<point> points;
    map<point, unsigned int> ids;
    vector<vector<unsigned int>> graph;

    for (unsigned int i = 0; i < lines.size(); i++) {
        vector<point> cur;
        graph.emplace_back();
        for (unsigned int j = 0; j < lines.size(); j++) {
            if (!lines[i].parallel(lines[j])) {
                point p = lines[i].intersect(lines[j]);
                if (ids.count(p) == 0) {
                    points.push_back(p);
                    ids.insert({p, ids.size()});
                }
                cur.push_back(p);
            }
        }

        sort(cur.begin(), cur.end());
        while (graph.size() < points.size()) {
            graph.emplace_back();
        }
        for (unsigned int j = 1; j < cur.size(); j++) {
            unsigned int k1 = ids[cur[j - 1]], k2 = ids[cur[j]];
            if (points[k1] != points[k2]) {
                graph[k1].emplace_back(k2);
                graph[k2].emplace_back(k1);
            }
        }
    }

    unsigned int n = points.size();
    vector<vector<bool>> vis(n);
    for (unsigned int i = 0; i < n; i++) {
        sort(graph[i].begin(), graph[i].end(), [i, &points](unsigned int p1, unsigned int p2) {
            return (points[p1] - points[i]).atan() < (points[p2] - points[i]).atan();
        });
        vis[i].assign(graph[i].size(), false);
    }

    vector<ld> areas;
    for (unsigned int i = 0; i < n; i++) {
        for (unsigned int j = 0; j < graph[i].size(); j++) {
            if (!vis[i][j]) {
                areas.push_back(go(points, graph, vis, i, j));
            }
        }
    }
    sort(areas.begin(), areas.end());
    unsigned int s = 0;
    while (s < areas.size() && areas[s] < 1e-8) {
        s++;
    }
    if (s < areas.size()) {
        vector<ld> result;
        for (; s < (int) areas.size() - 1; s++) {
            result.push_back(areas[s]);
        }
        return result;
    } else {
        return {};
    }
}

int main() {

    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
#ifndef LOCAL
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif

    unsigned int n;
    cin >> n;

    vector<line> lines;
    for (unsigned int i = 0; i < n; i++) {
        point p1, p2;
        cin >> p1 >> p2;
        lines.emplace_back(p1, p2);
    }

    auto res = areas(lines);
    cout << res.size() << '\n';
    cout.precision(30);
    for (ld area : res) {
        cout << (double) area << '\n';
    }

}
