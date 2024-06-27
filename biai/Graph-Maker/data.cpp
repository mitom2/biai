#include "data.hpp"

Data::Data()
{
    //Set default values
    Data::outputFile="OUTPUT.png";
    Data::outputX=768;
    Data::outputY=480;
    Data::pointSize=7;
    Data::squarePoints=false;
    Data::pointR=0;
    Data::pointG=0;
    Data::pointB=0;
    Data::displayConnectingLine=false;
    Data::connectingLineR=128;
    Data::connectingLineG=128;
    Data::connectingLineB=128;
    Data::backgroundR=255;
    Data::backgroundG=255;
    Data::backgroundB=255;
    Data::axisR=0;
    Data::axisG=0;
    Data::axisB=0;
    Data::textR=0;
    Data::textG=0;
    Data::textB=0;
    Data::xAxisDescription="";
    Data::yAxisDescription="";
    Data::title="";
    Data::description="";
    Data::displayGridLines=true;
    Data::gridLinesNumber=7;
    Data::uncertaintyXR=0;
    Data::uncertaintyXG=128;
    Data::uncertaintyXB=0;
    Data::uncertaintyYR=0;
    Data::uncertaintyYG=0;
    Data::uncertaintyYB=128;
    Data::displayUncertaintyX=false;
    Data::displayUncertaintyY=false;
    Data::marginScaleX=0.1;
    Data::marginScaleY=0.1;
    Data::axisSize=1;
    Data::axisScalesNumber=14;
    Data::fontPath="files/fonts/Inter-Regular.otf";
    Data::gridR=192;
    Data::gridG=192;
    Data::gridB=192;
    Data::titleFieldHeight=48;
    Data::showFrom0=false;
    Data::sortData=true;
    Data::alignXAxisToValues = false;
}

bool Data::load(const std::string& path)
{
    //Open input file
    std::ifstream file(path);
    if(file.good()==false)
        return false;

    std::string input; //Buffer for input data
    bool readingData=false; //true if input is config data, false if graph data (points)
    char dataFormat[5]= {'-', '-', '-', '-', '-'}; //Format of points input
    char* currentDataInput=&(dataFormat[4]); //Pointer to the currently expected point parameter (based on dataFormat)
    file>>input;
    try
    {
        while(file) //Load new inputs as long as possible
        {
            if(readingData==true) //Loading points info
            {
                if(*currentDataInput=='-')
                {
                    Data::points.push_back({0, 0, 0, 0, true});
                    currentDataInput=&(dataFormat[0]);
                }
                if(*currentDataInput=='x')
                {
                    Data::points.back().x=std::stod(input);
                }
                else if(*currentDataInput=='u')
                {
                    Data::points.back().uncertaintyX=std::stod(input);
                }
                else if(*currentDataInput=='y')
                {
                    Data::points.back().y=std::stod(input);
                }
                else if(*currentDataInput=='v')
                {
                    Data::points.back().uncertaintyY=std::stod(input);
                }
                else
                {
                    return false;
                }
                currentDataInput++;
            }
            else //Loading config info
            {
                if(input=="OUTPUT-X:")
                {
                    file>>input;
                    if(file.eof()==true)
                        return false;
                    Data::outputX=std::stoi(input);
                }
                else if(input=="OUTPUT-Y:")
                {
                    file>>input;
                    if(file.eof()==true)
                        return false;
                    Data::outputY=std::stoi(input);
                }
                else if(input=="POINT-SIZE:")
                {
                    file>>input;
                    if(file.eof()==true)
                        return false;
                    Data::pointSize=std::stoi(input);
                }
                else if(input=="POINT-SHAPE:")
                {
                    file>>input;
                    if(file.eof()==true)
                        return false;
                    Data::squarePoints=(input=="square") ? true : false;
                }
                else if(input=="POINT-COLOR:")
                {
                    file>>input;
                    if(file.eof()==true)
                        return false;
                    Data::pointR=std::stoi(input);
                    file>>input;
                    if(file.eof()==true)
                        return false;
                    Data::pointG=std::stoi(input);
                    file>>input;
                    if(file.eof()==true)
                        return false;
                    Data::pointB=std::stoi(input);
                }
                else if(input=="DISPLAY-CONNECTING-LINE:")
                {
                    file>>input;
                    if(file.eof()==true)
                        return false;
                    Data::displayConnectingLine=(input=="yes") ? true : false;
                }
                else if(input=="SHOW-FROM-0:")
                {
                    file>>input;
                    if(file.eof()==true)
                        return false;
                    Data::showFrom0=(input=="yes") ? true : false;
                }
                else if(input=="CONNECTING-LINE-COLOR:")
                {
                    file>>input;
                    if(file.eof()==true)
                        return false;
                    Data::connectingLineR=std::stoi(input);
                    file>>input;
                    if(file.eof()==true)
                        return false;
                    Data::connectingLineG=std::stoi(input);
                    file>>input;
                    if(file.eof()==true)
                        return false;
                    Data::connectingLineB=std::stoi(input);
                }
                else if(input=="BACKGROUND-COLOR:")
                {
                    file>>input;
                    if(file.eof()==true)
                        return false;
                    Data::backgroundR=std::stoi(input);
                    file>>input;
                    if(file.eof()==true)
                        return false;
                    Data::backgroundG=std::stoi(input);
                    file>>input;
                    if(file.eof()==true)
                        return false;
                    Data::backgroundB=std::stoi(input);
                }
                else if(input=="AXIS-COLOR:")
                {
                    file>>input;
                    if(file.eof()==true)
                        return false;
                    Data::axisR=std::stoi(input);
                    file>>input;
                    if(file.eof()==true)
                        return false;
                    Data::axisG=std::stoi(input);
                    file>>input;
                    if(file.eof()==true)
                        return false;
                    Data::axisB=std::stoi(input);
                }
                else if(input=="TEXT-COLOR:")
                {
                    file>>input;
                    if(file.eof()==true)
                        return false;
                    Data::textR=std::stoi(input);
                    file>>input;
                    if(file.eof()==true)
                        return false;
                    Data::textG=std::stoi(input);
                    file>>input;
                    if(file.eof()==true)
                        return false;
                    Data::textB=std::stoi(input);
                }
                else if(input=="AXIS-DESCRIPTION-X:")
                {
                    bool fetchingString=false;
                    while(true)
                    {
                        file>>input;
                        if(file.eof()==true)
                            return false;

                        if(fetchingString==false)
                        {
                            if(input[0]=='"')
                                fetchingString=true;
                            else
                                continue;
                        }

                        Data::xAxisDescription+=input+" ";

                        if(Data::containsQuotationMark(input)==true)
                        {
                            Data::xAxisDescription=Data::extractText(Data::xAxisDescription);
                            break;
                        }
                    }
                }
                else if(input=="AXIS-DESCRIPTION-Y:")
                {
                    bool fetchingString=false;
                    while(true)
                    {
                        file>>input;
                        if(file.eof()==true)
                            return false;

                        if(fetchingString==false)
                        {
                            if(input[0]=='"')
                                fetchingString=true;
                            else
                                continue;
                        }

                        Data::yAxisDescription+=input+" ";

                        if(Data::containsQuotationMark(input)==true)
                        {
                            Data::yAxisDescription=Data::extractText(Data::yAxisDescription);
                            break;
                        }
                    }
                }
                else if(input=="GRAPH-TITLE:")
                {
                    bool fetchingString=false;
                    while(true)
                    {
                        file>>input;
                        if(file.eof()==true)
                            return false;

                        if(fetchingString==false)
                        {
                            if(input[0]=='"')
                                fetchingString=true;
                            else
                                continue;
                        }

                        Data::title+=input+" ";

                        if(Data::containsQuotationMark(input)==true)
                        {
                            Data::title=Data::extractText(Data::title);
                            break;
                        }
                    }
                }
                else if(input=="FONT:")
                {
                    bool fetchingString=false;
                    Data::fontPath="";
                    while(true)
                    {
                        file>>input;
                        if(file.eof()==true)
                            return false;

                        if(fetchingString==false)
                        {
                            if(input[0]=='"')
                                fetchingString=true;
                            else
                                continue;
                        }

                        Data::fontPath+=input+" ";

                        if(Data::containsQuotationMark(input)==true)
                        {
                            Data::fontPath=Data::extractText(Data::fontPath);
                            Data::fontPath="files/fonts/"+Data::fontPath+".otf";
                            break;
                        }
                    }
                }
                else if(input=="OUTPUT-FILE:")
                {
                    bool fetchingString=false;
                    Data::outputFile="";
                    while(true)
                    {
                        file>>input;
                        if(file.eof()==true)
                            return false;

                        if(fetchingString==false)
                        {
                            if(input[0]=='"')
                                fetchingString=true;
                            else
                                continue;
                        }

                        Data::outputFile+=input+" ";

                        if(Data::containsQuotationMark(input)==true)
                        {
                            Data::outputFile=Data::extractText(Data::outputFile);
                            break;
                        }
                    }
                }
                else if(input=="GRAPH-DESCRIPTION:")
                {
                    bool fetchingString=false;
                    while(true)
                    {
                        file>>input;
                        if(file.eof()==true)
                            return false;

                        if(fetchingString==false)
                        {
                            if(input[0]=='"')
                                fetchingString=true;
                            else
                                continue;
                        }

                        Data::description+=input+" ";

                        if(Data::containsQuotationMark(input)==true)
                        {
                            Data::description=Data::extractText(Data::description);
                            break;
                        }
                    }
                }
                else if(input=="DISPLAY-GRID-LINES:")
                {
                    file>>input;
                    if(file.eof()==true)
                        return false;
                    Data::displayGridLines=(input=="yes") ? true : false;
                }
                else if(input=="GRID-LINES-COLOR:")
                {
                    file>>input;
                    if(file.eof()==true)
                        return false;
                    Data::gridR=std::stoi(input);
                    file>>input;
                    if(file.eof()==true)
                        return false;
                    Data::gridG=std::stoi(input);
                    file>>input;
                    if(file.eof()==true)
                        return false;
                    Data::gridB=std::stoi(input);
                }
                else if(input=="TITLE-MARGIN:")
                {
                    file>>input;
                    if(file.eof()==true)
                        return false;
                    Data::titleFieldHeight=std::stoi(input);
                }
                else if(input=="GRID-LINES-NUMBER:")
                {
                    file>>input;
                    if(file.eof()==true)
                        return false;
                    Data::gridLinesNumber=std::stoi(input);
                }
                else if(input=="DISPLAY-UNCERTAINTY-X:")
                {
                    file>>input;
                    if(file.eof()==true)
                        return false;
                    Data::displayUncertaintyX=(input=="yes") ? true : false;
                }
                else if(input=="DISPLAY-UNCERTAINTY-Y:")
                {
                    file>>input;
                    if(file.eof()==true)
                        return false;
                    Data::displayUncertaintyY=(input=="yes") ? true : false;
                }
                else if(input=="SORT:")
                {
                    file>>input;
                    if(file.eof()==true)
                        return false;
                    Data::sortData=(input=="yes") ? true : false;
                }
                else if(input=="UNCERTAINTY-X-COLOR:")
                {
                    file>>input;
                    if(file.eof()==true)
                        return false;
                    Data::uncertaintyXR=std::stoi(input);
                    file>>input;
                    if(file.eof()==true)
                        return false;
                    Data::uncertaintyXG=std::stoi(input);
                    file>>input;
                    if(file.eof()==true)
                        return false;
                    Data::uncertaintyXB=std::stoi(input);
                }
                else if(input=="UNCERTAINTY-Y-COLOR:")
                {
                    file>>input;
                    if(file.eof()==true)
                        return false;
                    Data::uncertaintyYR=std::stoi(input);
                    file>>input;
                    if(file.eof()==true)
                        return false;
                    Data::uncertaintyYG=std::stoi(input);
                    file>>input;
                    if(file.eof()==true)
                        return false;
                    Data::uncertaintyYB=std::stoi(input);
                }
                else if(input=="MARGIN-SCALE-X:")
                {
                    file>>input;
                    if(file.eof()==true)
                        return false;
                    Data::marginScaleX=std::stod(input);
                }
                else if(input=="MARGIN-SCALE-Y:")
                {
                    file>>input;
                    if(file.eof()==true)
                        return false;
                    Data::marginScaleY=std::stod(input);
                }
                else if(input=="AXIS-SIZE:")
                {
                    file>>input;
                    if(file.eof()==true)
                        return false;
                    Data::axisSize=std::stod(input);
                }
                else if(input=="AXIS-SCALES-NUMBER:")
                {
                    file>>input;
                    if(file.eof()==true)
                        return false;
                    Data::axisScalesNumber=std::stod(input);
                }
                else if (input == "ALIGN-X-AXIS-TO-VALUES:")
                {
                    file >> input;
                    if (file.eof() == true)
                        return false;
                    Data::alignXAxisToValues = (input == "yes") ? true : false;
                }
                else if(input=="DATA:")
                {
                    readingData=true;
                }
                else if(input=="DATA-FORMAT:")
                {
                    bool fetchingString=false;
                    std::string format="";
                    while(true)
                    {
                        file>>input;
                        if(file.eof()==true)
                            return false;

                        if(fetchingString==false)
                        {
                            if(input[0]=='"')
                                fetchingString=true;
                            else
                                continue;
                        }

                        format+=input+" ";

                        if(Data::containsQuotationMark(input)==true)
                        {
                            format=Data::extractText(format);
                            break;
                        }
                    }

                    //Set currently expected parameter to the first (default) one
                    currentDataInput=&(dataFormat[0]);

                    //Insert parameter info to the array
                    for(size_t i=0; i<format.length(); i++)
                    {
                        if(format[i]=='x')
                            (*currentDataInput)='x';
                        else if (format[i]=='y')
                            (*currentDataInput)='y';
                        else if ((format[i]=='u') && (format[i+1]=='x'))
                        {
                            (*currentDataInput)='u';
                            i++;
                        }
                        else if ((format[i]=='u') && (format[i+1]=='y'))
                        {
                            (*currentDataInput)='v';
                            i++;
                        }
                        else if((format[i]==' '))
                            continue;
                        else
                            return false;

                        currentDataInput++;
                    }
                    currentDataInput=&(dataFormat[4]);
                }
            }
            file>>input;
        }
    }
    catch(...)
    {
        throw std::exception("Graph-Maker data error!");
    }

    file.close(); //Close input file

    //Add internal margins by inserting two points on the edges of margins-to-be
    if(Data::showFrom0==true)
        Data::points.push_back({(Data::displayUncertaintyX==true ? Data::getMinTX() : Data::getMinX())-(((Data::displayUncertaintyX==true ? Data::getMaxTX() : Data::getMaxX())-(Data::displayUncertaintyX==true ? Data::getMinTX() : Data::getMinX()))*0.05), 0, 0, 0, false});
    else
        Data::points.push_back({(Data::displayUncertaintyX==true ? Data::getMinTX() : Data::getMinX())-(((Data::displayUncertaintyX==true ? Data::getMaxTX() : Data::getMaxX())-(Data::displayUncertaintyX==true ? Data::getMinTX() : Data::getMinX()))*0.05), (Data::displayUncertaintyY==true ? Data::getMinTY() : Data::getMinY())-(((Data::displayUncertaintyY==true ? Data::getMaxTY() : Data::getMaxY())-(Data::displayUncertaintyY==true ? Data::getMinTY() : Data::getMinY()))*0.05), 0, 0, false});
    Data::points.push_back({(Data::displayUncertaintyX==true ? Data::getMaxTX() : Data::getMaxX())+(((Data::displayUncertaintyX==true ? Data::getMaxTX() : Data::getMaxX())-(Data::displayUncertaintyX==true ? Data::getMinTX() : Data::getMinX()))*0.05), (Data::displayUncertaintyY==true ? Data::getMaxTY() : Data::getMaxY())+(((Data::displayUncertaintyY==true ? Data::getMaxTY() : Data::getMaxY())-(Data::displayUncertaintyY==true ? Data::getMinTY() : Data::getMinY()))*0.05), 0, 0, false});

    if (Data::alignXAxisToValues == true)
    {
        Data::points.push_back({ (Data::displayUncertaintyX == true ? Data::getMinTX() : Data::getMinX()) < 0 ? std::ceil(Data::displayUncertaintyX == true ? Data::getMinTX() : Data::getMinX()) : std::floor(Data::displayUncertaintyX == true ? Data::getMinTX() : Data::getMinX()), (Data::displayUncertaintyY == true ? Data::getMinTY() : Data::getMinY()) < 0 ? std::ceil(Data::displayUncertaintyY == true ? Data::getMinTY() : Data::getMinY()) : std::floor(Data::displayUncertaintyY == true ? Data::getMinTY() : Data::getMinY()),0,0,false });
        Data::points.push_back({ (Data::displayUncertaintyX == true ? Data::getMaxTX() : Data::getMaxX()) < 0 ? std::floor(Data::displayUncertaintyX == true ? Data::getMaxTX() : Data::getMaxX()) : std::ceil(Data::displayUncertaintyX == true ? Data::getMaxTX() : Data::getMaxX()), (Data::displayUncertaintyY == true ? Data::getMaxTY() : Data::getMaxY()) < 0 ? std::floor(Data::displayUncertaintyY == true ? Data::getMaxTY() : Data::getMaxY()) : std::ceil(Data::displayUncertaintyY == true ? Data::getMaxTY() : Data::getMaxY()),0,0,false });
    }

    //If requested, sort the data from lowest to highest (by X value)
    if(Data::sortData==true)
        std::sort(Data::points.begin(), Data::points.end(), [](const Data::point &a, const Data::point &b)->bool{ return a.x>b.x; });

    return true;
}

std::string Data::extractText(const std::string& source) const
{
    return source.substr(1, source.length()-3); //Create and return a substring without quotation marks, assuming that they are located on the edges of desired text
}

bool Data::containsQuotationMark(const std::string& source) const
{
    for(size_t i=1; i<source.length(); i++)
    {
        if(source[i]=='"')
            return true;
    }
    return false;
}

double Data::getMinX() const
{
    double minP=Data::points.back().x;
    for(size_t i=0; i<Data::points.size(); i++)
    {
        minP=Data::points[i].x<minP ? Data::points[i].x : minP;
    }
    return minP;
}

double Data::getMinY() const
{
    double minP=Data::points.back().y;
    for(size_t i=0; i<Data::points.size(); i++)
    {
        minP=Data::points[i].y<minP ? Data::points[i].y : minP;
    }
    return minP;
}

double Data::getMaxX() const
{
    double maxP=Data::points.back().x;
    for(size_t i=0; i<Data::points.size(); i++)
    {
        maxP=Data::points[i].x>maxP ? Data::points[i].x : maxP;
    }
    return maxP;
}

double Data::getMaxY() const
{
    double maxP=Data::points.back().y;
    for(size_t i=0; i<Data::points.size(); i++)
    {
        maxP=Data::points[i].y>maxP ? Data::points[i].y : maxP;
    }
    return maxP;
}

double Data::getMinTX() const
{
    double minP=Data::points.back().x-Data::points.back().uncertaintyX;
    for(size_t i=0; i<Data::points.size(); i++)
    {
        minP=Data::points[i].x-Data::points[i].uncertaintyX<minP ? Data::points[i].x-Data::points[i].uncertaintyX : minP;
    }
    return minP;
}

double Data::getMinTY() const
{
    double minP=Data::points.back().y-Data::points.back().uncertaintyY;
    for(size_t i=0; i<Data::points.size(); i++)
    {
        minP=Data::points[i].y-Data::points[i].uncertaintyY<minP ? Data::points[i].y-Data::points[i].uncertaintyY : minP;
    }
    return minP;
}

double Data::getMaxTX() const
{
    double maxP=Data::points.back().x+Data::points.back().uncertaintyX;
    for(size_t i=0; i<Data::points.size(); i++)
    {
        maxP=Data::points[i].x+Data::points[i].uncertaintyX>maxP ? Data::points[i].x+Data::points[i].uncertaintyX : maxP;
    }
    return maxP;
}

double Data::getMaxTY() const
{
    double maxP=Data::points.back().y+Data::points.back().uncertaintyY;
    for(size_t i=0; i<Data::points.size(); i++)
    {
        maxP=Data::points[i].y+Data::points[i].uncertaintyY>maxP ? Data::points[i].y+Data::points[i].uncertaintyY : maxP;
    }
    return maxP;
}

Data::ImproperDataException::ImproperDataException(char* message)
{
    Data::ImproperDataException::message=message;
}

char* Data::ImproperDataException::what()
{
    return Data::ImproperDataException::message;
}
