/** \file graph.hpp
 *  \brief A file containing Graph class definition.
 */

#pragma once
#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include "data.hpp"
#include "point.hpp"
#include "axis.hpp"

/** \brief A main class responsible for creating the graph.
 */
class Graph
{

public:

    /** \brief Default constructor.
     */
    Graph();

    /** \brief Function starting the generation of a graph.
     *
     * \param path Path to the file with input data
     * \return true if graph was generated, false otherwise
     *
     */
    bool run(const std::string& path);

protected:

    /** \brief Loads data from a file.
     *
     * \param path Path to a file with data
     * \return true if data was loaded, false otherwise
     *
     */
    bool loadData(const std::string& path);

    /** \brief Saves generated graph to file.
     *
     * \param path Path to the output file
     * \return true if graph was saved, false otherwise
     *
     */
    bool saveToFile(const std::string& path) const;

    /** \brief Reloads the graph, essentially rendering it.
     */
    void reload();

    /** \brief Calculates scale for a graph.
     * A scale needs to be calculated in order to fit all the data in a designated area.
     * \param outputSize Designated size of a graph (excluding external margins)
     * \param minTX Lowest X value to be displayed
     * \param maxTX Highest X value to be displayed
     * \param minTY Lowest Y value to be displayed
     * \param maxTY Highest Y value to be displayed
     * \return Calculated scales for x and y transformation
     *
     */
    sf::Vector2f calculateScale(const sf::Vector2f outputSize, const double minTX, const double maxTX, const double minTY, const double maxTY) const;

    /** \brief Writes an error message to external file.
     * Creates 'error.txt' file if needed and writes an error message to it. If the file already exists, the message is added at the end.
     * \param message Error message
     *
     */
    void createErrorFile(std::string message) const;

    /** \brief Clears 'error.txt' file from all contents or creates it if it do not exist.
     */
    void clearErrorFile() const;

    sf::RenderTexture graphRenderTexture;
    const sf::Texture* graphTexture;
    Data data;
    std::vector<Point> points;
    sf::VertexArray connectingLine;
    sf::VertexArray gridLines;
    Axis xAxis;
    Axis yAxis;
    sf::Text title,description,xAxisDescription,yAxisDescription;
    sf::Font font;
};
