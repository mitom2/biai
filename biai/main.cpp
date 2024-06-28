#include <SFML/Graphics.hpp>
#include <filesystem>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <thread>
#include <cstdlib>
#include <time.h>
#include <chrono>
#include <windows.h>
#include <fstream>
#include "Graph-Maker/graph.hpp"

std::string generateGraphInput(std::vector<double> x, std::vector<double> y, int graphNum)
{
    std::string path = "measurments.txt";
    std::filesystem::remove(path);
    std::filesystem::copy("graphTemplate.txt", path);
    std::fstream output;
    output.open(path, std::ios::app);
    if (output.good() == false)
        throw std::exception("Could not generate complete result file!");
    output << std::to_string(graphNum) << ".png\"\nDATA:\n";
    for (int i = 0; i < x.size(); i++)
    {
        output << std::to_string(x[i]) << " " << std::to_string(y[i]) << "\n";
    }
    output.close();
    output.open(path + std::to_string(graphNum) + ".txt", std::ios::out);
    if (output.good() == false)
        throw std::exception("Could not generate complete result file!");
    for (int i = 0; i < x.size(); i++)
    {
        output << std::to_string(x[i]) << " " << std::to_string(y[i]) << "\n";
    }
    output.close();
    return path;
}

class Neuron
{
    std::vector<Neuron*> inputs;
    std::vector<double> inputWeights;
    double lastRes;
    double error;

public:
    bool resReady;

    std::vector<double> getWeights()
    {
        return inputWeights;
    }

    Neuron(std::vector<Neuron*>& ins, double weight)
    {
        inputs = ins;
        lastRes = 0.0;
        resReady = false;
        error = 0.0;
        for (std::size_t i = 0; i < inputs.size(); i++)
        {
            inputWeights.push_back(weight);
        }
    }

    Neuron(std::vector<Neuron*>& ins)
    {
        inputs = ins;
        lastRes = 0.0;
        resReady = false;
        error = 0.0;
        for (std::size_t i = 0; i < inputs.size(); i++)
        {
            inputWeights.push_back((((double)(rand() % 10001)) / 100000.0) - 0.05);
        }
    }

    Neuron()
    {
        resReady = false;
        lastRes = 0.0;
        error = 0.0;
    }

    void setError(double val)
    {
        error = val;
    }

    void addError(double val)
    {
        error += val;
    }

    double getError()
    {
        return error;
    }

    void backPropagation(double rate, bool isOutput)
    {
        for (std::size_t i = 0; i < inputs.size(); i++)
        {
            inputs[i]->addError(error * inputWeights[i]);
        }/*//Sigmoid
        if(isOutput==false)
            error *= (lastRes * (1.0 - lastRes));*/
        //ReLu
        if (lastRes <= 0)
        {
            error = 0;
        }
        for (std::size_t i = 0; i < inputs.size(); i++)
        {
            inputWeights[i] += rate * error * inputs[i]->getLastRes();
        }
    }

    virtual double getLastRes()
    {
        return lastRes;
    }

    virtual double activation()
    {/*//Sigmoid
        double sum = 0.0;
        for (std::size_t i = 0; i < inputs.size(); i++)
        {
            if (inputs[i]->resReady == false)
                sum += inputs[i]->activation() * inputWeights[i];
            else
                sum += inputs[i]->getLastRes();
        }
        lastRes = 1.0 / (1.0 + std::exp(-sum));
        resReady = true;
        return lastRes;*/
        //ReLu
        double sum = 0.0;
        for (std::size_t i = 0; i < inputs.size(); i++)
        {
            if (inputs[i]->resReady == false)
                sum += inputs[i]->activation() * inputWeights[i];
            else
                sum += inputs[i]->getLastRes();
        }
        lastRes = sum > 0.0 ? sum : 0.0;
        resReady = true;
        return lastRes;
    }
};

class InputNeuron : public Neuron
{
    double inVal;
    double lR;
public:
    InputNeuron() : Neuron::Neuron()
    {
        inVal = 0.0;
        lR = 0.0;
    }
    double activation()
    {
        lR = inVal;
        return inVal;
    }
    void setValue(double newVal)
    {
        inVal = newVal;
    }
    double getLastRes()
    {
        return lR;
    }
};

class Layer
{
    std::vector<Neuron*> neurons;

public:
    std::vector<Neuron*>* getNeurons()
    {
        return &neurons;
    }

    Layer(int amnt, double initWeight, Layer& prev)
    {
        for (int i = 0; i < amnt; i++)
        {
            neurons.push_back(new Neuron(*(prev.getNeurons()), initWeight));
        }
    }

    Layer(int amnt, Layer& prev)
    {
        for (int i = 0; i < amnt; i++)
        {
            neurons.push_back(new Neuron(*(prev.getNeurons())));
        }
    }

    Layer(std::vector<Neuron*>& initNeurons)
    {
        neurons = initNeurons;
    }

    ~Layer()
    {
        for (std::size_t i = 0; i < neurons.size(); i++)
        {
            delete neurons[i];
        }
    }

    void unready()
    {
        for (std::size_t i = 0; i < neurons.size(); i++)
        {
            neurons[i]->resReady = false;
            neurons[i]->setError(0.0);
        }
    }
};

class Net
{
    std::vector<Layer*> layers;

public:
    Net(std::vector<int>& layersDecl, double initWeight)
    {
        std::vector<Neuron*> n;
        for (int i = 0; i < layersDecl[0]; i++)
        {
            n.push_back(new InputNeuron);
        }
        layers.push_back(new Layer(n));
        for (std::size_t i = 1; i < layersDecl.size(); i++)
        {
            layers.push_back(new Layer(layersDecl[i], initWeight, *layers.back()));
        }
    }
    Net(std::vector<int>& layersDecl)
    {
        std::vector<Neuron*> n;
        for (int i = 0; i < layersDecl[0]; i++)
        {
            n.push_back(new InputNeuron);
        }
        layers.push_back(new Layer(n));
        for (std::size_t i = 1; i < layersDecl.size(); i++)
        {
            layers.push_back(new Layer(layersDecl[i], *layers.back()));
        }
    }
    ~Net()
    {
        for (std::size_t i = 0; i < layers.size(); i++)
        {
            delete layers[i];
        }
    }
    void setInput(std::vector<double>& inputs)
    {
        for (std::size_t i = 0; i < layers[0]->getNeurons()->size(); i++)
        {
            (dynamic_cast<InputNeuron*>(layers[0]->getNeurons()->at(i)))->setValue(inputs[i]);
        }
    }
    double activate()
    {
        for (std::size_t i = 0; i < layers.size(); i++)
        {
            layers[i]->unready();
        }
        std::vector<std::thread> threads;
        for (std::size_t i = 0; i < layers.back()->getNeurons()->size(); i++)
        {
            threads.push_back(std::thread(&Neuron::activation, layers.back()->getNeurons()->at(i)));
        }
        for (std::size_t i = 0; i < threads.size(); i++)
        {
            threads[i].join();
        }
        double avg = 0.0;
        for (std::size_t i = 0; i < layers.back()->getNeurons()->size(); i++)
        {
            avg += layers.back()->getNeurons()->at(i)->getLastRes();
        }
        return avg / (double)(layers.back()->getNeurons()->size());
    }
    int getLastPrediction()
    {
        double max = -10.0;
        int maxPos = 0;
        for (std::size_t i = 0; i < layers.back()->getNeurons()->size(); i++)
        {
            if (layers.back()->getNeurons()->at(i)->getLastRes() > max)
            {
                max = layers.back()->getNeurons()->at(i)->getLastRes();
                maxPos = i;
            }
        }
        return maxPos;
    }
    double getLastOutput(int neuron)
    {
        return layers.back()->getNeurons()->at(neuron)->getLastRes();
    }
    void backPropagation(std::vector<double>& expected, double rate)
    {
        for (std::size_t i = 0; i < layers.back()->getNeurons()->size(); i++)
        {
            layers.back()->getNeurons()->at(i)->setError(expected[i] - layers.back()->getNeurons()->at(i)->getLastRes());
            layers.back()->getNeurons()->at(i)->backPropagation(rate, true);
        }
        //std::vector<std::thread> threads;
        for (int i = layers.size() - 2; i >= 0; i--)
        {
            for (std::size_t o = 0; o < layers[i]->getNeurons()->size(); o++)
            {
                //threads.push_back(std::thread(&Neuron::backPropagation, layers[i]->getNeurons()->at(o), rate
                layers[i]->getNeurons()->at(o)->backPropagation(rate, false);
            }
           /* for (std::size_t o = 0; o < threads.size(); o++)
            {
                threads[o].join();
            }*/
            //threads.clear();
        }
    }
    void save(std::string path)
    {
        std::ofstream file(path);
        if (file.good() == false)
            return;
        for (std::size_t i = 0; i < layers.size(); i++)
        {
            file << "layer ";
            for (std::size_t o = 0; o < layers[i]->getNeurons()->size(); o++)
            {
                file << "neuron ";
                for (std::size_t p = 0; p < layers[i]->getNeurons()->at(o)->getWeights().size(); p++)
                {
                    file << layers[i]->getNeurons()->at(o)->getWeights().at(p) << " ";
                }
            }
        }
        file.close();
    }
};

void processImages()
{
    std::unordered_map<std::string, long> amounts;
    std::unordered_map<std::string, long> ctr;
    for (const std::filesystem::directory_entry& de : std::filesystem::recursive_directory_iterator("img"))
    {
        if (de.is_regular_file() == true)
        {
            if (amounts.count(de.path().parent_path().string()) == 0)
            {
                amounts[de.path().parent_path().string()] = 0;
                ctr[de.path().parent_path().string()] = 0;
                continue;
            }
            amounts[de.path().parent_path().string()]++;
        }
    }
    std::cout << "done\nConverting images to proper input format...";
    sf::RenderTexture rt;
    rt.create(75, 50);
    for (const std::filesystem::directory_entry& de : std::filesystem::recursive_directory_iterator("img"))
    {
        if (de.is_regular_file() == true)
        {
            std::string pth = de.path().string();
            sf::Texture tx;
            if (tx.loadFromFile(pth) == false)
                continue;
            rt.clear();
            sf::Image res;
            res.create(75, 50);
            std::size_t sX = (tx.getSize().x - 75) / 2;
            std::size_t sY = (tx.getSize().y - 50) / 2;
            sf::RectangleShape rs(sf::Vector2f(75, 50));
            rs.setOrigin(0, 0);
            rs.setPosition(0, 0);
            rs.setTexture(&tx, true);
            rt.draw(rs);
            rt.display();
            res = rt.getTexture().copyToImage();
            double min = 255.0;
            double max = 0.0;
            for (std::size_t y = 0; y < 50; y++)
            {
                for (std::size_t x = 0; x < 75; x++)
                {
                    sf::Color cl = res.getPixel(x, y);
                    int avg = (cl.r + cl.g + cl.b) / 3;
                    res.setPixel(x, y, sf::Color(avg, avg, avg));
                    min = min > avg ? avg : min;
                    max = max < avg ? avg : max;
                }
            }
            double range = max - min;
            double modif = 255.0 / range;
            for (std::size_t y = 0; y < 50; y++)
            {
                for (std::size_t x = 0; x < 75; x++)
                {
                    sf::Color cl = res.getPixel(x, y);
                    int col = cl.r;
                    col -= min;
                    col *= modif;
                    res.setPixel(x, y, sf::Color(col, col, col));
                }
            }
            pth = de.path().parent_path().string();
            if (ctr[pth] < 0.15 * ((double)(amounts[pth])))
            {
                ctr[pth]++;
                pth = "testing" + pth.substr(3) + "/"+ std::to_string(ctr[pth]) +".jpeg";
            }
            else
            {
                ctr[pth]++;
                pth = "training" + pth.substr(3) + "/" + std::to_string(ctr[pth] - (int)(0.15 * ((double)(amounts[pth]))) - 1) + ".jpeg";
            }
            res.saveToFile(pth);
        }
    }
    std::cout << "done\n";
}

double test(Net& net, int graphNum, int e)
{
    std::unordered_map<std::string, int> iDs;
    int nextId = 0;
    int infoInterval = 50;
    std::size_t runs = 0;
    std::size_t correct = 0;
    std::size_t correctCurr = 0;
    double avgTimeSum = 0.0;
    double avgTimeDiv = 0.0;
        std::vector<std::pair<std::string, std::string>> dataset;
        runs = 0;
        correct = 0;
        correctCurr = 0;
        for (const std::filesystem::directory_entry& de : std::filesystem::recursive_directory_iterator("testing"))
        {
            if (de.is_directory() == true)
            {
                if (iDs.count(de.path().string()) == 0)
                {
                    iDs[de.path().string()] = nextId;
                    nextId++;
                }
            }
            else if (de.is_regular_file() == true)
            {
                std::string pth = de.path().string();
                dataset.push_back({ de.path().string(),de.path().parent_path().string() });
            }
        }
        std::cout << "Testing started.\n";
        auto timeStart = std::chrono::high_resolution_clock::now();
        while (dataset.size() > 0)
        {
            sf::Image img;
            int in = rand() % dataset.size();
            std::string pth = dataset[in].first;
            if (img.loadFromFile(pth) == true)
            {
                std::vector<double> inputs;
                for (std::size_t y = 0; y < img.getSize().y; y++)
                {
                    for (std::size_t x = 0; x < img.getSize().x; x++)
                    {
                        double avg = img.getPixel(x, y).r;
                        avg /= 255.0;
                        inputs.push_back(avg);
                    }
                }
                net.setInput(inputs);
                net.activate();
                runs++;
                if (iDs[dataset[in].second] == net.getLastPrediction())
                {
                    correct++;
                    correctCurr++;
                }
                //std::cout << "\nRun #" << std::to_string(runs) << ":\n\tExpected: " << std::to_string(iDs[dataset[in].second]) << " (" << dataset[in].second << ")\n\tPrediction: " << std::to_string(net.getLastPrediction()) << "\n";
            
            }
            dataset[in] = dataset.back();
            dataset.pop_back();
            if (runs % infoInterval == 0)
            {
                avgTimeDiv++;
                auto timeStop = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(timeStop - timeStart);
                timeStart = std::chrono::high_resolution_clock::now();
                double remaining = dataset.size();
                double timeUnit = duration.count();
                avgTimeSum += timeUnit;
                timeUnit = avgTimeSum / avgTimeDiv;
                timeUnit /= ((double)(infoInterval));
                remaining *= timeUnit;
                system("cls");
                std::cout << "Test run #" << std::to_string(runs) << " (EPOCH #" << std::to_string(e + 1) << ")\n";// \n\tExpected: " << std::to_string(iDs[dataset[in].second]) << " (" << dataset[in].second << ")\n\tPrediction: " << std::to_string(net.getLastPrediction()) << "\n";
                std::cout << "Remaining runs: " << std::to_string(dataset.size()) << "\nTotal accuracy: " << 100.0 * ((double)correct) / ((double)runs) << "%\nCurrent accuracy: " << 100.0 * ((double)correctCurr) / ((double)infoInterval) << "%\nApprox. remaining execution time: " << remaining / 1000000.0 << " sec (" << remaining / 60000000.0 << " min / " << (remaining / 60000000.0) / 60.0 << " hrs)\n";
                correctCurr = 0;
            }
        }
    return ((double)correct) / ((double)runs);
}

double train(Net& net, double rate, int epochs, int graphNum)
{
    std::unordered_map<std::string, int> iDs;
    int nextId = 0;
    std::cout << "done\nLoading data...";
    int infoInterval = 10;
    std::size_t runs = 0;
    std::size_t correct = 0;
    std::size_t correctCurr = 0;
    double avgTimeSum = 0.0;
    double avgTimeDiv = 0.0;
    std::vector<double> resultX, resultY, resultXt, resultYt;
    double lT = 0.0;
    for (int e = 0; e < epochs; e++)
    {
        resultX.push_back(e);
        resultXt.push_back(e);
        std::vector<std::pair<std::string, std::string>> dataset;
        runs = 0;
        correct = 0;
        correctCurr = 0;
        for (const std::filesystem::directory_entry& de : std::filesystem::recursive_directory_iterator("training"))
        {
            if (de.is_directory() == true)
            {
                if (iDs.count(de.path().string()) == 0)
                {
                    iDs[de.path().string()] = nextId;
                    nextId++;
                }
            }
            else if (de.is_regular_file() == true)
            {
                std::string pth = de.path().string();
                dataset.push_back({ de.path().string(),de.path().parent_path().string() });
            }
        }
        std::cout << "done\nTraining started.\n";
        auto timeStart = std::chrono::high_resolution_clock::now();
        while (dataset.size() > 0)
        {
            sf::Image img;
            int in = rand() % dataset.size();
            std::string pth = dataset[in].first;
            if (img.loadFromFile(pth) == true)
            {
                std::vector<double> inputs;
                for (std::size_t y = 0; y < img.getSize().y; y++)
                {
                    for (std::size_t x = 0; x < img.getSize().x; x++)
                    {
                        double avg = img.getPixel(x, y).r;
                        avg /= 255.0;
                        inputs.push_back(avg);
                    }
                }
                net.setInput(inputs);
                net.activate();
                runs++;
                if (iDs[dataset[in].second] == net.getLastPrediction())
                {
                    correct++;
                    correctCurr++;
                }
                std::vector<double> expected(iDs.size(), 0.0);
                expected[iDs[dataset[in].second]] = 1.0;
                //std::cout << "\nRun #" << std::to_string(runs) << ":\n\tExpected: " << std::to_string(iDs[dataset[in].second]) << " (" << dataset[in].second << ")\n\tPrediction: " << std::to_string(net.getLastPrediction()) << "\n";
                net.backPropagation(expected, rate);
            }
            dataset[in] = dataset.back();
            dataset.pop_back();
            if (runs % infoInterval == 0)
            {
                avgTimeDiv++;
                auto timeStop = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(timeStop - timeStart);
                timeStart = std::chrono::high_resolution_clock::now();
                double remaining = dataset.size();
                double timeUnit = duration.count();
                avgTimeSum += timeUnit;
                timeUnit = avgTimeSum / avgTimeDiv;
                timeUnit /= ((double)(infoInterval));
                remaining *= timeUnit;
                system("cls");
                std::cout << "Training run #" << std::to_string(runs) << " (EPOCH #" << std::to_string(e + 1) << ")\n";// \n\tExpected: " << std::to_string(iDs[dataset[in].second]) << " (" << dataset[in].second << ")\n\tPrediction: " << std::to_string(net.getLastPrediction()) << "\n";
                std::cout << "Remaining runs in epoch: " << std::to_string(dataset.size()) << "\nTotal accuracy in epoch: " << 100.0 * ((double)correct) / ((double)runs) << "%\nCurrent accuracy: " << 100.0 * ((double)correctCurr) / ((double)infoInterval) << "%\nApprox. remaining epoch execution time: " << remaining / 1000000.0 << " sec (" << remaining / 60000000.0 << " min / " << (remaining / 60000000.0) / 60.0 << " hrs)\n";
                correctCurr = 0;
            }
        }
        resultY.push_back(100.0 * ((double)correct) / ((double)runs));
        lT = test(net, graphNum, e);
        resultYt.push_back(lT * 100.0);
    }
    Graph graphMaker;
    graphMaker.run(generateGraphInput(resultX, resultY, graphNum + 1));
    Graph graphMaker2;
    graphMaker2.run(generateGraphInput(resultXt, resultYt, ((graphNum + 100) * 10) + 1));
    return lT;
}

int main()
{
    srand(time(NULL));
    std::cout << "Initializing input images...";
    processImages();
    std::vector<int> lD{ 3750, 1024, 512, 128, 4 };
    double learningRate = 10;
    Net n(lD);
    std::vector<double> resultX, resultY;
    for (int i = 0; i < 8; i++)
    {
        learningRate /= 10;
        double acc = train(n, learningRate, 64, i);
        std::cout << "\n\nAchieved accuracy: " << std::to_string(acc);
        resultX.push_back((i + 1) * 64);
        resultY.push_back(acc);
        Graph graphMaker;
        graphMaker.run(generateGraphInput(resultX, resultY, i * 10000));
        n.save("net" + std::to_string(i) + ".txt");
        std::ofstream slr("learningRateChnage" + std::to_string(i) + ".txt");
        if (slr.good() == true)
        {
            slr << learningRate << "\n";
        }
        slr.close();
    }
}