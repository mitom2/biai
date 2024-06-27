/** \file axis.hpp
 * \brief A file containing Axis class definition.
 */

#pragma once
#include <iostream>
#include <vector>
#include <sstream>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

/** \brief Class managing a single (X or Y) axis on a graph.
 */
class Axis : public sf::Drawable
{
public:

    /** \brief Creates and prepares axis to be displayed.
     *
     * \param startingPoint Position of beginning end of an axis
     * \param endingPoint Position of ending end of end axis
     * \param font Font for numbers on a scale
     * \param width Width of axis (strongly recommended to keep it at 1)
     * \param scalesNumber Number of numerical scales on the axis
     * \param r R color value of axis
     * \param g G color value of axis
     * \param b B color value of axis
     * \param minV lowest value located in dimension
     * \param maxV highest value located in dimension
     *
     */
    void setAxis(sf::Vector2f startingPoint, sf::Vector2f endingPoint, sf::Font font, double width=1, double scalesNumber=3, double r=255, double g=255, double b=255, double minV=0, double maxV=0);

protected:

    /** \brief Draw axis to the render target.
    *
    * \param target Render target to draw to
    * \param states Current render states
    *
    */
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    /** \brief Convert double to std::string (using scientific notation if needed).
    *
    * \param num Number to convert
    * \return std::string with number
    *
    */
    std::string convertToString(const double num) const;

    sf::VertexArray scale;
    sf::RectangleShape line;
    bool vertical;
    sf::Font font;
    std::vector<sf::Text> texts;
};
