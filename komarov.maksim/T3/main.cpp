#include <fstream>
#include <iostream>
#include <iomanip>
#include <limits>
#include <map>
#include "shapes.h"
#include "commands.h"
#include "iofmtguard.h"
int main(int argc,char* argv[])
{
    using namespace max;
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    if(argc!=2){std::cout<<"ERROR: expected filename as only command-line argument\n";return -1;}
    std::ifstream fin(argv[1]);
    if(!fin){std::cout<<"ERROR: cannot open file\n";return -13;}
    PolyVec figs; std::string line;
    while(std::getline(fin,line))
    {
        Polygon p; if(parsePolygon(line,p)) figs.push_back(p);
    }
    std::map<std::string,std::function<void(std::istream&,std::ostream&)>> cmd;
    cmd["AREA"]=std::bind(area,std::cref(figs),std::placeholders::_1,std::placeholders::_2);
    cmd["COUNT"]=std::bind(countCmd,std::cref(figs),std::placeholders::_1,std::placeholders::_2);
    cmd["MAX"]=std::bind(maxCmd,std::cref(figs),std::placeholders::_1,std::placeholders::_2);
    cmd["MIN"]=std::bind(minCmd,std::cref(figs),std::placeholders::_1,std::placeholders::_2);
    cmd["RECTS"]=std::bind(rects,std::cref(figs),std::placeholders::_1,std::placeholders::_2);
    cmd["INTERSECTIONS"]=std::bind(intersectionsCmd,std::cref(figs),std::placeholders::_1,std::placeholders::_2);
    iofmtguard g(std::cout); std::cout<<std::fixed<<std::setprecision(1);
    std::string c;
    while(std::cin>>c)
    {
        try{
            if(cmd.count(c)==0) throw std::runtime_error("bad");
            cmd[c](std::cin,std::cout); std::cout<<'\n';
        }catch(...){
            std::cout<<"<INVALID COMMAND>\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        }
    }
    return 0;
}


