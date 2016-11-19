#include <algorithm>
#include <array>
#include <atomic>
#include <bitset>
#include <cassert>
#include <cctype>
#include <cerrno>
#include <cfenv>
#include <cfloat>
#include <chrono>
#include <cinttypes>
#include <ciso646>
#include <climits>
#include <clocale>
#include <cmath>
#include <complex>
#include <condition_variable>
#include <csetjmp>
#include <csignal>
#include <cstdarg>
#include <cstdbool>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctgmath>
#include <ctime>
#include <cwchar>
#include <cwctype>
#include <deque>
#include <exception>
#include <forward_list>
#include <fstream>
#include <functional>
#include <future>
#include <initializer_list>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <list>
#include <locale>
#include <map>
#include <memory>
#include <mutex>
#include <new>
#include <numeric>
#include <ostream>
#include <queue>
#include <random>
#include <ratio>
#include <regex>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <system_error>
#include <thread>
#include <tuple>
#include <typeindex>
#include <typeinfo>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <valarray>
#include <vector>
using namespace std;

struct Point {
    int x;
    int y;
    Point() : x(0), y(0) {}
    Point(int a, int b) : x(a), y(b) {}
};

bool operator<(const Point &a, const Point &b) {
    if(a.x != b.x) {
        return a.x < b.x;
    }
    else {
        return a.y < b.y;
    }
}

struct CrossPoint {
    enum {X, Y, XY} type;
    pair<int, int> k;
    
    union {
        int x;
        pair<int, int> x0;
    };
    union {
        int y;
        pair<int, int> y0;
    };
    
    int comp_e(const pair<int, int> &a, const pair<int, int> &b) const {
        return a.first * b.second - a.second * b.first;
    }
    
    bool operator<(const CrossPoint &b) const {
        if(type != b.type) {
            return type < b.type;
        }
        else {
            if(type == X) {
                return x < b.x;
            }
            else if(type == Y) {
                return y < b.y;
            }
            else {
                int c = comp_e(x0, b.x0);
                if(c) {
                    return c < 0;
                }
                c = comp_e(y0, b.y0);
                if(c) {
                    return c < 0;
                }
                return comp_e(k, b.k);
            }
        }
    }
    CrossPoint() {
        memset(this, sizeof(CrossPoint), 0);
    }
};

CrossPoint getCrossPoint(const Point &a, const Point &b) {
    CrossPoint cp;
    if(a.x == b.x) {
        cp.type = CrossPoint::X;
        cp.x = a.x;
    }
    else if(a.y == b.y) {
        cp.type = CrossPoint::Y;
        cp.y = a.y;
    }
    else {
        cp.type = CrossPoint::XY;
        cp.x0 = { a.y*b.x - a.x*b.y, b.y - a.y };
        cp.y0 = { a.y*b.x - a.x*b.y, b.x - a.x };
        cp.k = { b.y - a.y, b.x - a.x };
    }
    
    return cp;
}

ostream & operator<<(ostream &out, const CrossPoint &cp) {
    switch(cp.type) {
        case CrossPoint::X:
            out << "x=" << cp.x;
            break;
        case CrossPoint::Y:
            out << "y=" << cp.y;
            break;
        default:
            out << "x=" << 1.0 * cp.x0.first / cp.x0.second << ", y=" << 1.0 * cp.y0.first / cp.y0.second << ", k=" << 1.0 * cp.k.first / cp.k.second;
            break;
    }
    
    return out;
}

class Solution {
public:
    int maxPoints(vector<Point>& points) {
        if(points.empty()) {
            return 0;
        }
        
        // auto cp1 = getCrossPoint(Point(2,2), Point(4,4));
        // auto cp2 = getCrossPoint(Point(2,2), Point(6,6));
        
        // cout << (cp1 < cp2) << '\t' << (cp2 < cp1) << '\t' << cp1 << '\t' << cp2 << endl;
        
        // set<CrossPoint> sc;
        // sc.insert(cp1);
        // sc.insert(cp2);
        
        // for(auto &cp : sc) {
        //     cout << sc.size() << '\t' << cp << endl;
        // }
        
        // cout << "???" << endl;
        
        map<Point, int> pc;
        for(auto &p : points) {
            ++ pc[p];
        }
        
        if(pc.size() == 1) {
            return pc.begin()->second;
        }
        
        map<CrossPoint, set<Point>> pm;
        for(int i = 0; i < points.size(); ++ i) {
            for(int j = i+1; j < points.size(); ++ j) {
                if(points[i].x != points[j].x || points[i].y != points[j].y) {
                    // cout << "**" << pm.size() << endl;
                    auto cp = getCrossPoint(points[i], points[j]);
                    // !!!!!!!! just use auto &ps = pm[cp] will cause map size increase
                    // !!!!!!!! even if a previous cp is get from [2, 2] and [4, 4]
                    // !!!!!!!! and current cp is get from [2, 2] and [6, 6]
                    auto &ps = pm[cp];
                    //auto it = pm.find(cp);
                    //if(it == pm.end()) {
                    //    it = pm.insert(make_pair(cp, set<Point>())).first;
                    //}
                    //auto &ps = it->second;
                    // cout << cp << '\t' << points[i].x << ' ' << points[i].y << ' ' << points[j].x << ' ' << points[j].y << "; size=" << ps.size() << endl;
                    ps.insert(points[i]);
                    ps.insert(points[j]);
                    // cout << cp << '\t' << points[i].x << ' ' << points[i].y << ' ' << points[j].x << ' ' << points[j].y << "; size=" << ps.size() << endl;
                    // cout << "--" << pm.size() << endl;
                }
                
                        // cout << "---------------" << endl;
                        // int i = 1;
                        // // for(auto &kv : pm) {
                        // for(auto it = begin(pm); it != end(pm); ++ it) {
                        //     // cout << kv.first << '\t' << kv.second.size() << endl;
                        //     cout << pm.size() << ":\t" << (i ++) << ":\t" << it->first << '\t' << it->second.size() << endl;
                        //     // cout << pm.size() << endl;
                        //     // if(kv.second.size() > mp) {
                        //         // mp = kv.second.size();
                        //     // }
                        // }
                        // cout << "***************" << endl;
            }
        }
        
        // cout << endl << pm.size() << endl << endl;
        
        // cout << distance(begin(pm), end(pm)) << endl;
        
        int mp = 0;
        // int i = 1;
        for(auto &kv : pm) {
        // for(auto it = begin(pm); it != end(pm); ++ it) {
            // cout << kv.first << '\t' << kv.second.size() << endl;
            // cout << pm.size() << ":\t" << (i ++) << ":\t" << it->first << '\t' << it->second.size() << endl;
            // cout << pm.size() << endl;
            int c = 0;
            for(auto &p : kv.second) {
                c += pc[p];
            }
            if(c > mp) {
                mp = c;
            }
        }
        
        return max(mp, 1);
    }
};

int main() {
    vector<Point> points{{1,3}, {2,6}, {3,9}, {2,2}, {4,4},{6,6},{8,8},{0,0},{0,0}};
    cout << Solution().maxPoints(points) << endl;

    return 0;
}
