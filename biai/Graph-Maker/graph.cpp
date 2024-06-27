#include "graph.hpp"

Graph::Graph()
{
    //Set default values
    Graph::graphRenderTexture.create(768, 480);
    Graph::graphTexture=&(Graph::graphRenderTexture.getTexture());
    Graph::graphRenderTexture.setSmooth(true);
    Graph::gridLines.setPrimitiveType(sf::PrimitiveType::Lines);
}

bool Graph::saveToFile(const std::string& path) const
{
    sf::Image img=Graph::graphTexture->copyToImage(); //Convert sf::Texture to sf::Image
    return img.saveToFile(path); //Save sf::Image to file
}

bool Graph::loadData(const std::string& path)
{
    try
    {
        if(Graph::data.load(path)==false) //Try loading the data
            return false;
        Graph::reload(); //Begin generating the graph
    }
    catch(Data::ImproperDataException& exc)
    {
        Graph::createErrorFile("Error: "+std::string(exc.what())+"\n"); //Send error to the file
        return false;
    }
    return true;
}

void Graph::reload()
{
    //Set proper values of sf::RenderTexture desired by user
    Graph::graphRenderTexture.create(Graph::data.outputX, Graph::data.outputY);
    Graph::graphRenderTexture.clear(sf::Color(Graph::data.backgroundR, Graph::data.backgroundG, Graph::data.backgroundB));
    Graph::graphRenderTexture.setSmooth(true);

    Graph::gridLines.clear(); //Remove preexisting grid

    //Load font
    if(Graph::font.loadFromFile(Graph::data.fontPath)==false)
    {
        Graph::createErrorFile("Error: could not load font '"+Graph::data.fontPath+"'\n");
    }

    //Create grid
    for(int i=0; i<Graph::data.gridLinesNumber; i++)
    {
        Graph::gridLines.append(sf::Vertex(sf::Vector2f(Graph::data.outputX*Graph::data.marginScaleX, Graph::data.titleFieldHeight+(i+1)*((Graph::data.outputY-Graph::data.titleFieldHeight)*(1.0-Graph::data.marginScaleY))/(Graph::data.gridLinesNumber+1)), sf::Color(Graph::data.gridR, Graph::data.gridG, Graph::data.gridB)));
        Graph::gridLines.append(sf::Vertex(sf::Vector2f(Graph::data.outputX, Graph::data.titleFieldHeight+(i+1)*(Graph::data.outputY-Graph::data.titleFieldHeight)*(1.0-Graph::data.marginScaleY)/(Graph::data.gridLinesNumber+1)), sf::Color(Graph::data.gridR, Graph::data.gridG, Graph::data.gridB)));
        Graph::gridLines.append(sf::Vertex(sf::Vector2f(Graph::data.outputX*Graph::data.marginScaleX+(i+1)*Graph::data.outputX*(1.0-Graph::data.marginScaleX)/(Graph::data.gridLinesNumber+1), Graph::data.titleFieldHeight), sf::Color(Graph::data.gridR, Graph::data.gridG, Graph::data.gridB)));
        Graph::gridLines.append(sf::Vertex(sf::Vector2f(Graph::data.outputX*Graph::data.marginScaleX+(i+1)*Graph::data.outputX*(1.0-Graph::data.marginScaleX)/(Graph::data.gridLinesNumber+1), Graph::data.titleFieldHeight+(Graph::data.outputY-Graph::data.titleFieldHeight)*(1.0-Graph::data.marginScaleY)), sf::Color(Graph::data.gridR, Graph::data.gridG, Graph::data.gridB)));
    }
    Graph::graphRenderTexture.draw(Graph::gridLines); //Draw grid

    //Create X and Y axis, then draw them
    Graph::xAxis.setAxis(sf::Vector2f(Graph::data.outputX*(Graph::data.marginScaleX), (Graph::data.outputY-Graph::data.titleFieldHeight)*(1.0-Graph::data.marginScaleY)+Graph::data.titleFieldHeight), sf::Vector2f(Graph::data.outputX, Graph::data.titleFieldHeight+(Graph::data.outputY-Graph::data.titleFieldHeight)*(1.0-Graph::data.marginScaleY)), Graph::font, Graph::data.axisSize, Graph::data.axisScalesNumber, Graph::data.axisR, Graph::data.axisG, Graph::data.axisB, Graph::data.displayUncertaintyX==true ? Graph::data.getMinTX() : Graph::data.getMinX(), Graph::data.displayUncertaintyX==true ? Graph::data.getMaxTX() : Graph::data.getMaxX());
    Graph::yAxis.setAxis(sf::Vector2f(Graph::data.outputX*(Graph::data.marginScaleX), (Graph::data.outputY-Graph::data.titleFieldHeight)*(1.0-Graph::data.marginScaleY)+Graph::data.titleFieldHeight), sf::Vector2f(Graph::data.outputX*(Graph::data.marginScaleX), Graph::data.titleFieldHeight), Graph::font, Graph::data.axisSize, Graph::data.axisScalesNumber, Graph::data.axisR, Graph::data.axisG, Graph::data.axisB, Graph::data.displayUncertaintyY==true ? Graph::data.getMinTY() : Graph::data.getMinY(), Graph::data.displayUncertaintyY==true ? Graph::data.getMaxTY() : Graph::data.getMaxY());
    Graph::graphRenderTexture.draw(Graph::xAxis);
    Graph::graphRenderTexture.draw(Graph::yAxis);

    //Remove preexisting points and line connecting them
    Graph::points.clear();
    Graph::connectingLine.clear();
    Graph::connectingLine.setPrimitiveType(sf::PrimitiveType::LineStrip);

    //Calculate proper scale for X and Y size transformation
    sf::Vector2f scale=Graph::calculateScale(sf::Vector2f(Graph::data.outputX, Graph::data.outputY-Graph::data.titleFieldHeight), Graph::data.displayUncertaintyX==true ? Graph::data.getMinTX() : Graph::data.getMinX(), Graph::data.displayUncertaintyX==true ? Graph::data.getMaxTX() : Graph::data.getMaxX(), Graph::data.displayUncertaintyY==true ? Graph::data.getMinTY() : Graph::data.getMinY(), Graph::data.displayUncertaintyY==true ? Graph::data.getMaxTY() : Graph::data.getMaxY());

    //Add points to the graph
    for(size_t i=0; i<Graph::data.points.size(); i++)
    {
        //Create new point and add it to the vector
        Graph::points.push_back(Point(Graph::data.points[i].x, Graph::data.points[i].y, scale, Graph::data.displayUncertaintyX==true ? Graph::data.getMinTX() : Graph::data.getMinX(), Graph::data.displayUncertaintyY==true ? Graph::data.getMinTY() : Graph::data.getMinY(), Graph::data.outputX, Graph::data.outputY, Graph::data.marginScaleX, Graph::data.marginScaleY, Graph::data.titleFieldHeight, Graph::data.points[i].uncertaintyX, Graph::data.points[i].uncertaintyY, Graph::data.pointSize, Graph::data.squarePoints, Graph::data.pointR, Graph::data.pointG, Graph::data.pointB));

        //Set proper visibility
        if(Graph::data.points[i].display==false)
        {
            Graph::points.back().hide(); //Hide point
        }
        else
        {
            //Prepare uncertainty or hide it
            Graph::points.back().setUncertainty(Graph::data.displayUncertaintyX, Graph::data.displayUncertaintyY, Graph::data.uncertaintyXR, Graph::data.uncertaintyXG, Graph::data.uncertaintyXB, Graph::data.uncertaintyYR, Graph::data.uncertaintyYG, Graph::data.uncertaintyYB);

            //Add a line connecting this point to the previous one
            Graph::connectingLine.append(sf::Vertex(Graph::points.back().getTruePosition(), sf::Color(Graph::data.connectingLineR, Graph::data.connectingLineG, Graph::data.connectingLineB)));
        }
    }

    //Set font to the texts
    Graph::title.setFont(Graph::font);
    Graph::description.setFont(Graph::font);
    Graph::xAxisDescription.setFont(Graph::font);
    Graph::yAxisDescription.setFont(Graph::font);

    //Set font size for texts
    Graph::title.setCharacterSize(16);
    Graph::description.setCharacterSize(12);
    Graph::xAxisDescription.setCharacterSize(10);
    Graph::yAxisDescription.setCharacterSize(10);

    //Set color for texts
    Graph::title.setFillColor(sf::Color(Graph::data.textR, Graph::data.textG, Graph::data.textB));
    Graph::description.setFillColor(sf::Color(Graph::data.textR, Graph::data.textG, Graph::data.textB));
    Graph::xAxisDescription.setFillColor(sf::Color(Graph::data.textR, Graph::data.textG, Graph::data.textB));
    Graph::yAxisDescription.setFillColor(sf::Color(Graph::data.textR, Graph::data.textG, Graph::data.textB));

    //Set proper text contents
    Graph::title.setString(Graph::data.title);
    Graph::description.setString(Graph::data.description);
    Graph::xAxisDescription.setString(Graph::data.xAxisDescription);
    Graph::yAxisDescription.setString(Graph::data.yAxisDescription);

    //Calculate origin point of texts
    Graph::title.setOrigin(Graph::title.getGlobalBounds().width/2.f, 0);
    Graph::description.setOrigin(Graph::description.getGlobalBounds().width/2.f, 0);
    Graph::xAxisDescription.setOrigin(Graph::xAxisDescription.getGlobalBounds().width/2.f, Graph::xAxisDescription.getGlobalBounds().height/2.f);
    Graph::yAxisDescription.setOrigin(Graph::yAxisDescription.getGlobalBounds().width/2.f, Graph::yAxisDescription.getGlobalBounds().height/2.f);

    //Set texts in their respective positions
    Graph::title.setPosition(Graph::data.outputX/2.f, 5);
    Graph::description.setPosition(Graph::data.outputX/2.f, Graph::title.getGlobalBounds().top+Graph::title.getCharacterSize()*1.2);
    Graph::xAxisDescription.setPosition(((Graph::data.outputX-Graph::data.outputX*(Graph::data.marginScaleX))/2.f)+Graph::data.outputX*(Graph::data.marginScaleX), Graph::data.titleFieldHeight+(Graph::data.outputY-Graph::data.titleFieldHeight)*(1.0-Graph::data.marginScaleY)+((Graph::data.outputY-Graph::data.titleFieldHeight)-(Graph::data.outputY-Graph::data.titleFieldHeight)*(1.0-Graph::data.marginScaleY))*3.0/4.0);
    Graph::yAxisDescription.setPosition(Graph::data.outputX*(Graph::data.marginScaleX)/4.0, Graph::data.titleFieldHeight+(Graph::data.outputY-Graph::data.titleFieldHeight)*(1.0-Graph::data.marginScaleY)/2.f);

    //Rotate the Y axis description
    Graph::yAxisDescription.setRotation(270);

    //Draw the line between points if it is requested
    if(Graph::data.displayConnectingLine==true)
        Graph::graphRenderTexture.draw(Graph::connectingLine);

    //Draw points
    for(size_t i=0; i<Graph::points.size(); i++)
    {
        Graph::graphRenderTexture.draw(Graph::points[i]);
    }

    //Draw texts
    Graph::graphRenderTexture.draw(Graph::title);
    Graph::graphRenderTexture.draw(Graph::description);
    Graph::graphRenderTexture.draw(Graph::xAxisDescription);
    Graph::graphRenderTexture.draw(Graph::yAxisDescription);

    //Reder graph to the texture
    Graph::graphRenderTexture.display();
}

bool Graph::run(const std::string& path)
{
    Graph::clearErrorFile(); //Reset error file

    //Try loading new data
    if(Graph::loadData(path)==false)
    {
        Graph::createErrorFile("Error: data loading unsuccessful\n");
        return false;
    }

    //Try saving generated graph to file
    if(Graph::saveToFile(Graph::data.outputFile)==false)
    {
        Graph::createErrorFile("Error: could not save graph\n");
        return false;
    }
    return true;
}

sf::Vector2f Graph::calculateScale(const sf::Vector2f outputSize, const double minTX, const double maxTX, const double minTY, const double maxTY) const
{
    double differenceX=maxTX-minTX;
    double differenceY=maxTY-minTY;
    return sf::Vector2f(outputSize.x*(1.0-Graph::data.marginScaleX)/differenceX, outputSize.y*(1.0-Graph::data.marginScaleY)/differenceY);
}

void Graph::createErrorFile(std::string message) const
{
    std::fstream file; //Create new std::fstream
    file.open("error.txt", std::ios::out | std::ios::app); //Open 'error.txt' file
    if(file.good()==false) //If not successful, abort
        return;
    file<<message; //Write error message to the file
    file.close(); //Close the file
}

void Graph::clearErrorFile() const
{
    //(Re)Create an empty 'error.txt' file
    std::ofstream file;
    file.open("error.txt");
    if(file.good()==false)
        return;
    file<<"";
    file.close();
}
