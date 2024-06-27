#include "point.hpp"

Point::Point(double x, double y, sf::Vector2f scale, double minX, double minY, double outputX, double outputY, double marginX, double marginY, double titleMargin, double uX, double uY, double size, bool isSquare, double r, double g, double b)
{
    //Set default values
    Point::display=true;
    Point::uncertaintyX.setPrimitiveType(sf::PrimitiveType::Lines);
    Point::uncertaintyY.setPrimitiveType(sf::PrimitiveType::Lines);
    square=isSquare;

    //Calculate positions
    double sX=x*scale.x-minX*scale.x+outputX*marginX;
    double sY=-y*scale.y+minY*scale.y+(outputY-titleMargin)*(1-marginY);

    //Prevent points being drawn outside the visible part
    if(sX<=0)
        sX=1;
    if(sY<=0)
        sY=1;

    //Set selcted points in their respective places and set their color+size
    if(isSquare==false)
    {
        Point::pointC.setRadius(size/2.f);
        Point::pointC.setOrigin(size/2.f, size/2.f);
        Point::pointC.setPosition(sf::Vector2f(sX, sY+titleMargin));
        Point::pointC.setFillColor(sf::Color(r, g, b));
    }
    else
    {
        Point::pointS.setSize(sf::Vector2f(size, size));
        Point::pointS.setOrigin(size/2.f, size/2.f);
        Point::pointS.setPosition(sf::Vector2f(sX, sY+titleMargin));
        Point::pointS.setFillColor(sf::Color(r, g, b));
    }

    //Prepare uncertainty to be drawn (add the lines)
    Point::uncertaintyX.append(sf::Vertex(sf::Vector2f(sf::Vector2f(sX-uX*scale.x, sY+titleMargin)), sf::Color(0, 0, 0)));
    Point::uncertaintyX.append(sf::Vertex(sf::Vector2f(sf::Vector2f(sX+uX*scale.x, sY+titleMargin)), sf::Color(0, 0, 0)));
    Point::uncertaintyY.append(sf::Vertex(sf::Vector2f(sf::Vector2f(sX, sY-uY*scale.y+titleMargin)), sf::Color(0, 0, 0)));
    Point::uncertaintyY.append(sf::Vertex(sf::Vector2f(sf::Vector2f(sX, sY+uY*scale.y+titleMargin)), sf::Color(0, 0, 0)));
}

void Point::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(Point::display==false)
        return;
    if(Point::square==true)
        target.draw(Point::pointS, states);
    else target.draw(Point::pointC, states);
    target.draw(Point::uncertaintyX);
    target.draw(Point::uncertaintyY);
}

void Point::setUncertainty(bool displayUX, bool displayUY, double xr, double xg, double xb, double yr, double yg, double yb)
{
    //If needed, set color of uncertainty X. Remove it otherwise.
    if(displayUX==true)
    {
        for(size_t i=0; i<Point::uncertaintyX.getVertexCount(); i++)
        {
            Point::uncertaintyX[i].color.r=xr;
            Point::uncertaintyX[i].color.g=xg;
            Point::uncertaintyX[i].color.b=xb;
        }
    }
    else
        Point::uncertaintyX.clear();

    //If needed, set color of uncertainty Y. Remove it otherwise.
    if(displayUY==true)
    {
        for(size_t i=0; i<Point::uncertaintyY.getVertexCount(); i++)
        {
            Point::uncertaintyY[i].color.r=yr;
            Point::uncertaintyY[i].color.g=yg;
            Point::uncertaintyY[i].color.b=yb;
        }
    }
    else
        Point::uncertaintyY.clear();
}

sf::Vector2f Point::getTruePosition() const noexcept
{
    return Point::square==true ? Point::pointS.getPosition() : Point::pointC.getPosition();
}

void Point::hide() noexcept
{
    Point::display=false;
}
