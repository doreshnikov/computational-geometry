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
    unsigned int id{0};

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
    static unsigned int last_id = 1;
    p.id = last_id++;
    return in >> p.x >> p.y;
}

template<typename T>
ostream &operator<<(ostream &out, point_t<T> const &p) {
    return out << p.x << ' ' << p.y;
}

typedef long long ll;
typedef unsigned long long ull;

struct point {
    point_t<ll> data;
    point *prev;
    point *next;

    point() : data(), prev(nullptr), next(nullptr) {}

    explicit point(point *prev) : data(), prev(prev), next(nullptr) {}

    explicit point(point_t<ll> const &data, point *prev) : data(data), prev(prev), next(nullptr) {}
};

int sign(long long value) {
    return (value == 0 ? 0 : value < 0 ? -1 : 1);
}

bool inside(point_t<ll> const &p1, point_t<ll> const &p2, point_t<ll> const &p3, point_t<ll> const &p) {
    bool v1 = sign((p2 - p1) / (p3 - p1)) * sign((p2 - p1) / (p - p1)) >= 0;
    bool v2 = sign((p3 - p2) / (p1 - p2)) * sign((p3 - p2) / (p - p2)) >= 0;
    bool v3 = sign((p1 - p3) / (p2 - p3)) * sign((p1 - p3) / (p - p3)) >= 0;
    return v1 && v2 && v3;
}

bool check_ear(point *vertex, unsigned int n) {
    point_t<ll> p1 = vertex->prev->data, p2 = vertex->data, p3 = vertex->next->data;
    if ((p2 - p1) / (p3 - p2) < 0) {
        return false;
    }
    point *cur = vertex->next->next;
    for (unsigned int i = 0; i < n - 3; i++) {
        if (inside(p1, p2, p3, cur->data)) {
            return false;
        }
        cur = cur->next;
    }
    return true;
}

void triangulate(point *head, unsigned int n) {
    point *cur = head;
    while (n >= 3) {
        if (check_ear(cur, n)) {
            cout << cur->prev->data.id << ' ' << cur->data.id << ' ' << cur->next->data.id << '\n';
            cur->prev->next = cur->next;
            cur->next->prev = cur->prev;
            delete cur;
            n--;
            cur = cur->prev;
        } else {
            cur = cur->next;
        }
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

    auto *head = new point();
    point *cur = head;
    for (unsigned int i = 0; i < n; i++) {
        cin >> cur->data;
        if (i < n - 1) {
            cur->next = new point(cur);
            cur = cur->next;
        } else {
            cur->next = head;
            head->prev = cur;
        }
    }
    triangulate(head, n);

}
