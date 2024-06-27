/** \file point.hpp
 * \brief A file containing Point class definition.
 */

#pragma once
#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

/** \brief Class managing a single drawable point on a graph.
 */
class Point : public sf::Drawable
{
public:

    /** \brief Default constructor.
     *
     * \param x X value of point
     * \param y Y value of point
     * \param scale Information about size transformation
     * \param minX Lowest X displayed on the graph
     * \param minY Lowest Y displayed on the graph
     * \param outputX Output X dimension
     * \param outputY Output Y dimension
     * \param marginX Left margin
     * \param marginY Bottom margin
     * \param titleMargin Top margin
     * \param uX Uncertainty of X
     * \param uY Uncertainty of Y
     * \param size Point size
     * \param isSquare Point shape (true - square, false - circle)
     * \param r Point color R value
     * \param g Point color G value
     * \param b Point color B value
     *
     */
    Point(double x, double y, sf::Vector2f scale, double minX, double minY, double outputX, double outputY, double marginX, double marginY, double titleMargin, double uX=0, double uY=0, double size=3, bool isSquare=false, double r=255, double g=255, double b=255);

    /** \brief Prepares uncertainty to be displayed (or hidden).
     * Sets color for uncertainty lines or hides them.
     * \param displayUX Defines whether uncertainty should be displayed for X or not
     * \param displayUY Defines whether uncertainty should be displayed for Y or not
     * \param xr R color value for X uncertainty
     * \param xg G color value for X uncertainty
     * \param xb B color value for X uncertainty
     * \param yr R color value for Y uncertainty
     * \param yg G color value for Y uncertainty
     * \param yb B color value for Y uncertainty
     *
     */
    void setUncertainty(bool displayUX, bool displayUY, double xr=255, double xg=255, double xb=255, double yr=255, double yg=255, double yb=255);

    /** \brief Returns true (global) position of a point on the rendered texture.
     * \return Global position of a point
     *
     */
    sf::Vector2f getTruePosition() const noexcept;

    /** \brief Makes point invisible on the graph
     */
    void hide() noexcept;

protected:

    /** \brief Draw point to the render target.
    *
    * \param target Render target to draw to
    * \param states Current render states
    *
    */
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::CircleShape pointC;

    sf::RectangleShape pointS;

    bool square;

    sf::VertexArray uncertaintyX;

    sf::VertexArray uncertaintyY;

    bool display;

};
