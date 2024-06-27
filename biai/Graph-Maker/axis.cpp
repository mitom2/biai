#include "axis.hpp"

void Axis::setAxis(sf::Vector2f startingPoint, sf::Vector2f endingPoint, sf::Font font, double width, double scalesNumber, double r, double g, double b, double minV, double maxV)
{
    Axis::scale.setPrimitiveType(sf::PrimitiveType::Lines);

    //Determine whether we are dealing with X or Y axis
    if(endingPoint.x==startingPoint.x)
        Axis::vertical=true;
    else Axis::vertical=false;

    //Set proper values for the axis
    Axis::line.setSize(sf::Vector2f(std::max(std::abs(endingPoint.x-startingPoint.x), (float)width), std::max(std::abs(startingPoint.y-endingPoint.y), (float)width)));
    Axis::line.setFillColor(sf::Color(r, g, b));
    if(Axis::vertical==true)
        Axis::line.setOrigin(width/2.f, std::abs(startingPoint.y-endingPoint.y));
    else
        Axis::line.setOrigin(0, width/2.f);

    Axis::line.setPosition(startingPoint); //Put the axis into it's position

    Axis::font=font; //Set the font for scale numbers

    if(Axis::vertical==false) //If it's X axis, add one scale since one will be drawn outside out of view (on the edge of a texture + 1px)
        scalesNumber++;

    double scale=(maxV-minV)/scalesNumber; //Calculate a space between two scales

    for(std::size_t i=0; i<scalesNumber; i++) //Add all numerical scales
    {
        if(Axis::vertical==true)
        {
            std::string scaleText=Axis::convertToString(minV+scale*(scalesNumber-(i)));;
            Axis::scale.append(sf::Vertex(sf::Vector2f(startingPoint.x-3, -(endingPoint.y-startingPoint.y)*i/scalesNumber+endingPoint.y), sf::Color(r, g, b)));
            Axis::scale.append(sf::Vertex(sf::Vector2f(startingPoint.x+3, -(endingPoint.y-startingPoint.y)*i/scalesNumber+endingPoint.y), sf::Color(r, g, b)));
            Axis::texts.push_back(sf::Text(scaleText, Axis::font, 10));
            Axis::texts.back().setOrigin(Axis::texts.back().getGlobalBounds().width, Axis::texts.back().getGlobalBounds().height/2.0);
            Axis::texts.back().setPosition(sf::Vector2f(startingPoint.x-5, -(endingPoint.y-startingPoint.y)*i/scalesNumber+endingPoint.y));
        }
        else if(i+1<scalesNumber)
        {
            std::string scaleText=Axis::convertToString(minV+scale*(i+1));
            Axis::scale.append(sf::Vertex(sf::Vector2f((endingPoint.x-startingPoint.x)*(i+1)/scalesNumber+startingPoint.x, startingPoint.y-3), sf::Color(r, g, b)));
            Axis::scale.append(sf::Vertex(sf::Vector2f((endingPoint.x-startingPoint.x)*(i+1)/scalesNumber+startingPoint.x, startingPoint.y+3), sf::Color(r, g, b)));
            Axis::texts.push_back(sf::Text(scaleText, Axis::font, 10));
            Axis::texts.back().setOrigin(Axis::texts.back().getGlobalBounds().width/2.0, 0);
            Axis::texts.back().setPosition(sf::Vector2f((endingPoint.x-startingPoint.x)*(i+1)/scalesNumber+startingPoint.x, startingPoint.y+5));
        }
        Axis::texts.back().setFillColor(sf::Color(r, g, b)); //Set scale numbers color assuming it's the same one as the one of axis
    }
}

void Axis::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    //Draw axis and it's scale numbers
    target.draw(Axis::scale, states);
    target.draw(Axis::line, states);
    for(std::size_t i=0; i<Axis::texts.size(); i++)
    {
        target.draw(Axis::texts[i]);
    }
}

std::string Axis::convertToString(const double num) const
{
    std::ostringstream strobj;
    strobj<<num;
    return strobj.str();
}
