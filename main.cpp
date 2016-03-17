#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <boost/regex.hpp>
#include <memory>
using std::cout;
using std::endl;


// typedef std::unique_ptr<Point> PointPointer;

class Point
{
  private:
    double m_x;
    double m_y;
  public:
    Point()
    {
      m_x = 0;
      m_y = 0;
    }
    Point(double x, double y)
    {
	m_x = x;
	m_y = y;
    }
    
    void print()
    {
      cout << "x: " << m_x << ", y: " << m_y;
    }
    
    double getX(){return m_x;};
    
    double getY(){return m_y;}
};

template<typename T, typename ...Args>
std::unique_ptr<T> make_unique( Args&& ...args )
{
    return std::unique_ptr<T>( new T( std::forward<Args>(args)... ) );
};


typedef std::vector<std::unique_ptr<Point>> points;

class PointParser
{
  private:
    PointParser() {};
  public:
    static std::unique_ptr<Point> parsePoint(std::string input)
    {
      boost::regex expr{"(<POINT,(\\d*\\.?\\d*),(\\d*\\.?\\d*)>)"};
      boost::smatch what;
      if(boost::regex_search(input, what, expr)){
	double longitude = std::atof(std::string(what[2]).c_str());
	double latitude = std::atof(std::string(what[3]).c_str());
	return make_unique<Point>(longitude, latitude);
      }
    }
};

class Polygon
{
  private:
    points* m_p;
    int m_i;
  public:
    Polygon(points* p)
    {
      m_p = p;
      m_i = 2;
    }
    
    void writeAllPoints()
    {
      for(std::vector<std::unique_ptr<Point>>::iterator i = m_p -> begin(); i != m_p -> end(); ++i){
	std::cout << (*i)->getX() << std::endl;
      }      
    }
};

class PolygonParser
{
  private:
    PolygonParser() {};
  public:
    static Polygon parsePolygon(std::string input)
    {
      points p;
      boost::regex expr{"(<POINT,(\\d*\\.?\\d*),(\\d*\\.?\\d*)>)"};
      boost::regex_token_iterator<std::string::iterator> it{input.begin(), input.end(), expr, 1};
      boost::regex_token_iterator<std::string::iterator> end;
      while(it != end){
	p.push_back(PointParser::parsePoint(*it++));
      }
//       for(std::vector<std::unique_ptr<Point>>::iterator i = p.begin(); i != p.end(); ++i){
// 	std::cout << (*i)->getX() << std::endl;
//       }
      return Polygon(&p);
    }
};


int main()
{
  auto p = PointParser::parsePoint("<POINT,16.56,87.64>");
//   std::cout << p->getX() << std::endl;
  auto poly = PolygonParser::parsePolygon("<POLYGON,<POINT,16.56,87.64>,<POINT,11.22,33.444>>");
  poly.writeAllPoints();
}