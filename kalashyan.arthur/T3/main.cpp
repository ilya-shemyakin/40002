#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <functional>
#include <iomanip>
#include <limits>

struct Point {
    int x, y;
    bool operator==(Point const& o) const { return x==o.x && y==o.y; }
    bool operator< (Point const& o) const { return x<o.x || (x==o.x && y<o.y); }
};

struct Polygon {
    std::vector<Point> points;
};


bool readPolygon(std::string const& line, Polygon& poly) {
    std::istringstream iss(line);
    int n;
    if (!(iss >> n) || n < 3) return false;

    std::vector<std::string> tokens{
        std::istream_iterator<std::string>(iss),
        std::istream_iterator<std::string>()
    };
    if ((int)tokens.size() != n) return false;

    poly.points.clear();
    std::transform(tokens.begin(), tokens.end(), std::back_inserter(poly.points),
        [](std::string const& tok){
            std::istringstream ps(tok);
            char c; int x,y;
            ps >> c >> x >> c >> y >> c;
            return Point{x,y};
        });
    return true;
}

double calculateArea(Polygon const& p) {
    auto it0 = p.points.begin();
    auto it1 = std::next(it0);
    double cross0 = it0->x * p.points.back().y - it0->y * p.points.back().x;
    double sum = std::inner_product(
        it1, p.points.end(),
        it0, cross0,
        std::plus<>(),
        [](Point const& a, Point const& b){
            return a.x * b.y - a.y * b.x;
        }
    );
    return std::abs(sum) * 0.5;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Error: Filename required\n";
        return 1;
    }
    std::ifstream fin(argv[1]);
    if (!fin) {
        std::cerr << "Error: Cannot open file\n";
        return 1;
    }

    std::vector<Polygon> polygons;
    std::string line;
    while (std::getline(fin, line)) {
        Polygon poly;
        if (readPolygon(line, poly))
            polygons.push_back(std::move(poly));
    }

    
    std::string cmd;
    while (std::cin >> cmd) {

        if (cmd == "AREA") {
            std::string arg; std::cin >> arg;

            double total = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                [&](double acc, Polygon const& p){
                    bool take =
                        (arg=="EVEN" || arg=="ODD")
                          ? ((p.points.size()%2==0)==(arg=="EVEN"))
                          : (arg=="MEAN"
                             ? true
                             : p.points.size()==std::stoi(arg));
                    return take ? acc + calculateArea(p) : acc;
                });
            if (arg=="MEAN" && !polygons.empty())
                total /= polygons.size();
            std::cout << "AREA " << arg << "\n"
                      << std::fixed << std::setprecision(1) << total << "\n";
        }

        else if (cmd == "MAX") {
            std::string arg; std::cin >> arg;
            if (arg == "AREA") {
                auto it = std::max_element(polygons.begin(), polygons.end(),
                    [&](Polygon const& a, Polygon const& b){
                        return calculateArea(a) < calculateArea(b);
                    });
                double v = it==polygons.end() ? 0.0 : calculateArea(*it);
                std::cout << "MAX AREA\n" << std::fixed << std::setprecision(1) << v << "\n";
            } else {
                auto it = std::max_element(polygons.begin(), polygons.end(),
                    [&](Polygon const& a, Polygon const& b){
                        return a.points.size() < b.points.size();
                    });
                int v = it==polygons.end() ? 0 : it->points.size();
                std::cout << "MAX VERTEXES\n" << v << "\n";
            }
        }

        else if (cmd == "MIN") {
            std::string arg; std::cin >> arg;
            if (arg == "AREA") {
                auto it = std::min_element(polygons.begin(), polygons.end(),
                    [&](Polygon const& a, Polygon const& b){
                        return calculateArea(a) < calculateArea(b);
                    });
                double v = it==polygons.end() ? 0.0 : calculateArea(*it);
                std::cout << "MIN AREA\n" << std::fixed << std::setprecision(1) << v << "\n";
            } else {
                auto it = std::min_element(polygons.begin(), polygons.end(),
                    [&](Polygon const& a, Polygon const& b){
                        return a.points.size() < b.points.size();
                    });
                int v = it==polygons.end() ? 0 : it->points.size();
                std::cout << "MIN VERTEXES\n" << v << "\n";
            }
        }

        else if (cmd == "COUNT") {
            std::string arg; std::cin >> arg;
            int cnt = std::count_if(polygons.begin(), polygons.end(),
                [&](Polygon const& p){
                    return (arg=="EVEN"||arg=="ODD")
                      ? ((p.points.size()%2==0)==(arg=="EVEN"))
                      : p.points.size()==std::stoi(arg);
                });
            std::cout << "COUNT " << arg << "\n" << cnt << "\n";
        }

        else if (cmd=="PERMS" || cmd=="MAXSEQ") {
            std::string rest;
            std::getline(std::cin, rest);
            
            int n = static_cast<int>(std::count(rest.begin(), rest.end(), '('));
            std::string polyLine = std::to_string(n) + " " + rest;
            Polygon query;
            if (!readPolygon(polyLine, query)) {
                std::cout << "<INVALID COMMAND>\n";
                continue;
            }
            
            auto normQ = query.points;
            std::sort(normQ.begin(), normQ.end());

            if (cmd=="PERMS") {
                int c = static_cast<int>(
                  std::count_if(polygons.begin(), polygons.end(),
                    [&](Polygon const& p){
                      if ((int)p.points.size()!=n) return false;
                      auto tmp = p.points;
                      std::sort(tmp.begin(), tmp.end());
                      return tmp==normQ;
                    }));
                std::cout << "PERMS";
                std::for_each(query.points.begin(), query.points.end(),
                    [&](Point const& pt){
                        std::cout<<" ("<<pt.x<<";"<<pt.y<<")";
                    });
                std::cout << "\n" << c << "\n";
            } else {
                struct S{int cur,best;};
                S r = std::accumulate(polygons.begin(), polygons.end(), S{0,0},
                  [&](S s, Polygon const& p){
                    if ((int)p.points.size()!=n) {
                      s.cur=0; return s;
                    }
                    auto tmp = p.points; std::sort(tmp.begin(), tmp.end());
                    if (tmp==normQ) {
                      s.cur++; s.best = std::max(s.best,s.cur);
                    } else s.cur = 0;
                    return s;
                  });
                std::cout << "MAXSEQ";
                std::for_each(query.points.begin(), query.points.end(),
                    [&](Point const& pt){
                        std::cout<<" ("<<pt.x<<";"<<pt.y<<")";
                    });
                std::cout << "\n" << r.best << "\n";
            }
        }

        else {
            std::cout << "<INVALID COMMAND>\n";
        }
    }
    return 0;
}
