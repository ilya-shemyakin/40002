#include "commands.h"
#include <algorithm>
#include <numeric>
#include <stdexcept>
namespace max
{
static bool evenV(const Polygon&p){return p.points.size()%2==0;}
static bool oddV (const Polygon&p){return p.points.size()%2==1;}
static std::function<bool(const Polygon&)> vertexPred(std::size_t k)
{
    return [k](const Polygon&p){return p.points.size()==k;};
}

void area(const PolyVec& figs,std::istream& in,std::ostream& out)
{
    std::string param; in>>param;
    if(param=="EVEN"||param=="ODD")
    {
        bool (*pred)(const Polygon&)=param=="EVEN"?evenV:oddV;
        double sum=std::accumulate(figs.begin(),figs.end(),0.0,
            [=](double acc,const Polygon& p){return acc+(pred(p)?p.area():0.0);});
        out<<sum;
    }
    else if(param=="MEAN")
    {
        if(figs.empty()) throw std::runtime_error("empty");
        double tot=std::accumulate(figs.begin(),figs.end(),0.0,
            [](double a,const Polygon& p){return a+p.area();});
        out<<tot/figs.size();
    }
    else
    {
        std::size_t k=std::stoul(param);
        double sum=std::accumulate(figs.begin(),figs.end(),0.0,
            [&](double acc,const Polygon& p){return acc+(vertexPred(k)(p)?p.area():0.0);});
        out<<sum;
    }
}

void countCmd(const PolyVec& figs,std::istream& in,std::ostream& out)
{
    std::string param; in>>param;
    std::function<bool(const Polygon&)> pred;
    if(param=="EVEN") pred=evenV;
    else if(param=="ODD") pred=oddV;
    else pred=vertexPred(std::stoul(param));
    out<<std::count_if(figs.begin(),figs.end(),pred);
}

static void minmax(const PolyVec& f,std::istream& in,std::ostream& out,bool isMax)
{
    std::string what; in>>what;
    if(f.empty()) throw std::runtime_error("empty");
    if(what=="AREA")
    {
        auto cmp=[](const Polygon&a,const Polygon&b){return a.area()<b.area();};
        auto it=isMax?std::max_element(f.begin(),f.end(),cmp):std::min_element(f.begin(),f.end(),cmp);
        out<<it->area();
    }
    else if(what=="VERTEXES")
    {
        auto cmp=[](const Polygon&a,const Polygon&b){return a.points.size()<b.points.size();};
        auto it=isMax?std::max_element(f.begin(),f.end(),cmp):std::min_element(f.begin(),f.end(),cmp);
        out<<it->points.size();
    }
    else throw std::runtime_error("bad");
}
void maxCmd(const PolyVec& f,std::istream& i,std::ostream& o){minmax(f,i,o,true);}
void minCmd(const PolyVec& f,std::istream& i,std::ostream& o){minmax(f,i,o,false);}

void rects(const PolyVec& f,std::istream&,std::ostream& o)
{
    o<<std::count_if(f.begin(),f.end(),[](const Polygon& p){return p.isRectangle();});
}

void intersectionsCmd(const PolyVec& f,std::istream& in,std::ostream& out)
{
    std::string rest; std::getline(in,rest);
    if(rest.empty()||rest[0]!=' ') throw std::runtime_error("poly");
    rest.erase(0,1);
    Polygon poly;
    if(!parsePolygon(rest,poly)) throw std::runtime_error("parse");
    out<<std::count_if(f.begin(),f.end(),[&](const Polygon& p){return intersects(p,poly);});
}
}
