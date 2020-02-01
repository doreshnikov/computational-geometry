#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <deque>

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
typedef point_t<ld> point;

int sign(long double value) {
    return (value == 0 ? 0 : value > 0 ? 1 : -1);
}

ld det(ld a, ld b, ld c, ld d) {
    return a * d - b * c;
}

struct semiplane {
    ld a;
    ld b;
    ld c;

    semiplane() : a(0), b(0), c(0) {}

    semiplane(ld a, ld b, ld c) : a(a), b(b), c(c) {}

    [[nodiscard]] point norm() const {
        return point(a, b);
    }

    [[nodiscard]] int where(point const &p) const {
        return sign(p.x * a + p.y * b + c);
    }

    [[nodiscard]] bool parallel(semiplane const &other) const {
        return det(a, b, other.a, other.b) == 0;
    }

    [[nodiscard]] bool is(semiplane const &other) const {
        return det(a, b, other.a, other.b) == 0 &&
               det(a, c, other.a, other.c) == 0 &&
               det(b, c, other.b, other.c) == 0;
    }

    [[nodiscard]] point intersect(semiplane const &other) const {
        ld alpha = det(a, b, other.a, other.b);
        return point(
                det(b, c, other.b, other.c) / alpha,
                det(c, a, other.c, other.a) / alpha
        );
    }

    bool operator==(semiplane const &other) const {
        return a == other.a && b == other.b && c == other.c;
    }
};

ld intersection(vector<semiplane> &lines) {
//    sort(lines.begin(), lines.end(), [](semiplane const &s1, semiplane const &s2) {
//        return atan2(s1.a, s1.b) < atan2(s2.a, s2.b);
//    });
    ld edge = 1000000000;
    vector<semiplane> border{
            semiplane(1, 0, edge),
            semiplane(0, 1, edge),
            semiplane(-1, 0, edge),
            semiplane(0, -1, edge)
    };
    vector<point> points{
            point(-edge, edge),
            point(-edge, -edge),
            point(edge, -edge),
            point(edge, edge)
    };
    for (const auto &line : lines) {
        vector<unsigned int> intersection;
        vector<unsigned int> through;
        for (unsigned int j = 0; j < border.size(); j++) {
            point p1 = points[j], p2 = points[j < border.size() - 1 ? j + 1 : 0];
            int w1 = line.where(p1), w2 = line.where(p2);
            if (w1 == 0 && w2 == 0) {
                if (line.norm() * border[j].norm() < 0) {
                    return 0;
                }
            } else if (w1 == 0) {
                through.push_back(j);
            } else if (w1 * w2 < 0) {
                intersection.push_back(j);
            }
            if (intersection.size() + through.size() == 2) {
                break;
            }
        }
        if (intersection.empty() && through.empty()) {
            if (line.where(points[0]) < 0) {
                return 0;
            }
        } else if (intersection.empty() && through.size() == 1) {
            unsigned int i = through[0], j = i = i < border.size() - 1 ? i + 1 : 0;
            if (line.where(points[j]) < 0) {
                return 0;
            }
        } else {
            vector<semiplane> tborder;
            vector<point> tpoints;
            auto update = [&tborder, &tpoints](semiplane const &line, semiplane const &old) {
                tborder.push_back(line);
                tpoints.push_back(line.intersect(old));
            };
            auto copy_old = [&tborder, &border, &tpoints, &points](unsigned int start, unsigned int finish) {
                std::copy(border.begin() + start, border.begin() + finish, std::back_inserter(tborder));
                std::copy(points.begin() + start, points.begin() + finish, std::back_inserter(tpoints));
            };
            std::copy(through.begin(), through.end(), std::back_inserter(intersection));
            unsigned int i = intersection[0], j = intersection[1];
            if (j < i) {
                swap(i, j);
            }
            bool through_i = (!through.empty() && through[0] == i) || (through.size() > 1 && through[1] == i);
//            bool through_j = find(through.begin(), through.end(), i) != through.end();
            bool through_j = (!through.empty() && through[0] == j) || (through.size() > 1 && through[1] == j);
//            bool through_j = find(through.begin(), through.end(), j) != through.end();
            point loc = points[i < points.size() - 1 ? i + 1 : 0];
            if (line.where(loc) >= 0) {
                update(border[i], line);
                copy_old(i + 1, j + 1 - through_j);
                if (!through_j) {
                    update(line, border[j]);
                }
            } else {
                update(border[j], line);
                copy_old(j + 1, border.size());
                copy_old(0, i + 1 - through_i);
                if (!through_i) {
                    update(line, border[i]);
                }
            }
            border = move(tborder);
            points = move(tpoints);
        }
    }

    for (semiplane const &line: border) {
        if (line.c == edge && abs(line.a) + abs(line.b) == 1) {
            return -1;
        }
    }
    ld area = 0;
    for (
            unsigned int i = 0;
            i < points.

                    size();

            i++) {
        unsigned int j = i < points.size() - 1 ? i + 1 : 0;
        area += (points[j].x - points[i].x) * (points[j].y + points[i].y) / 2;
    }
    return
            abs(area);
}

int main() {

    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
#ifndef LOCAL
    freopen("area.in", "r", stdin);
    freopen("area.out", "w", stdout);
#endif

    unsigned int n;
    cin >> n;

    vector<semiplane> lines(n);
    for (unsigned int i = 0; i < n; i++) {
        cin >> lines[i].a >> lines[i].b >> lines[i].c;
    }
    auto area = intersection(lines);
    if (area < 0) {
        cout << "-1\n";
    } else {
        cout.precision(30);
        cout << (double) area << '\n';
    }

}
