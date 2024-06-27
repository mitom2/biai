/** \file data.hpp
 *  \brief A file containing Data class definition.
 */

#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>

/** \brief Class responsible for handling all data operations
 */
class Data
{
public:

    /** \brief An exception thrown when data can not be interpreted properly.
     *
     */
    class ImproperDataException : public std::exception
    {
        char* message;
    public:

        /** \brief Default constructor.
         *
         * \param message Message to be saved in error file
         *
         */
        ImproperDataException(char* message);

        /** \brief Returns message intended to be saved in error file.
         * \return Error message
         */
        char* what();
    };

    /** \brief Struct defining a single point to be placed on a graph.
     */
    struct point
    {
        double x;
        double y;
        double uncertaintyX;
        double uncertaintyY;
        bool display;
    };

    /** \brief Default constructor.
     */
    Data();

    /** \brief Loads data from input file.
     *
     * \param path Path to the input file
     * \exception Data::ImproperDataException data can not be interpreted properly
     * \return true if data was loaded without issues, false otherwise
     *
     */
    bool load(const std::string& path);

    /** \brief Lowest X value in data.
     *
     * \return Lowest X value in data
     *
     */
    double getMinX() const;

    /** \brief Lowest Y value in data.
     *
     * \return Lowest Y value in data
     *
     */
    double getMinY() const;

    /** \brief Highest X value in data.
     *
     * \return Highest X value in data
     *
     */
    double getMaxX() const;

    /** \brief Highest Y value in data.
     *
     * \return Highest Y value in data
     *
     */
    double getMaxY() const;

    /** \brief Lowest possible X value adjusted for uncertainty.
     *
     * \return Lowest X value
     *
     */
    double getMinTX() const;

    /** \brief Lowest possible Y value adjusted for uncertainty.
     *
     * \return Lowest Y value
     *
     */
    double getMinTY() const;

    /** \brief Highest possible X value adjusted for uncertainty.
     *
     * \return Highest X value
     *
     */
    double getMaxTX() const;

    /** \brief Highest possible Y value adjusted for uncertainty.
     *
     * \return Highest Y value
     *
     */
    double getMaxTY() const;

    std::string outputFile;
    int outputX;
    int outputY;
    int pointSize;
    bool squarePoints;
    short pointR;
    short pointG;
    short pointB;
    bool displayConnectingLine;
    short connectingLineR;
    short connectingLineG;
    short connectingLineB;
    short backgroundR;
    short backgroundG;
    short backgroundB;
    short axisR;
    short axisG;
    short axisB;
    short textR;
    short textG;
    short textB;
    std::string xAxisDescription;
    std::string yAxisDescription;
    std::string title;
    std::string description;
    std::string fontPath;
    int titleFieldHeight;
    bool displayGridLines;
    short gridR;
    short gridG;
    short gridB;
    int gridLinesNumber;
    short uncertaintyXR;
    short uncertaintyXG;
    short uncertaintyXB;
    short uncertaintyYR;
    short uncertaintyYG;
    short uncertaintyYB;
    std::vector<Data::point> points;
    bool displayUncertaintyX;
    bool displayUncertaintyY;
    double marginScaleX;
    double marginScaleY;
    double axisSize;
    double axisScalesNumber;
    bool showFrom0;
    bool sortData;
    bool alignXAxisToValues;

protected:

    /** \brief Extracts a text from between quotation marks.
     *
     * \param source Source of the text
     * \return Extracted text
     *
     */
    std::string extractText(const std::string& source) const;

    /** \brief Checks if input contains quotation marks.
     *
     * \param source Text to be checked
     * \return true if quotation mark was found, false otherwise
     *
     */
    bool containsQuotationMark(const std::string& source) const;
};
