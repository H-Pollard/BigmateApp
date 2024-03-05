#include "nlohmann\json.hpp"
#include <iostream>
#include <fstream>
#include <string.h>
#include "shape.h"
#include "main.h"
#include <algorithm>
#include <unordered_map> 
#include <vector>

using json = nlohmann::json; 
using string = std::string;

int HashFunc(std::string _key, int _size_of_data)
{
    int intKey = 0;

    // Add ASCII values of each char
    for (int i = 0; i < _key.size(); i++)
    {
        intKey += _key[i];
    }

    return (intKey % _size_of_data);
}

// Process shape properties
void FindProperties(std::vector<shapes*> *PropertiesVector, std::unordered_map<int, shapes*> _ShapeMap)
{
    shapes *_shapes = new shapes;
    
    // used to display data to ensure it has been parsed correctly
    for (auto lShape : _ShapeMap)
    {
        // added for readability
        _shapes = lShape.second;

        if (_shapes->shape == "circle") {

            // Create pointer to derived class to store in hash map
            // Each new shape is a new pointer, is this good practice??
            // TODO smart pointers?
            circle *_circle = new circle(*_shapes);

            // Find radius of the circle
            _circle->decomposeArea();

            PropertiesVector->push_back(_circle);

        }
        else if (_shapes->shape == "square") {
            square *_square = new square(*_shapes);

            // check if either coordinate is close to edge
            _square->decomposeArea();

            PropertiesVector->push_back(_square);
        }
        else if (_shapes->shape == "rectangle") {
            rectangle *_rectangle = new rectangle(*_shapes);

            int distToEdge[] = {0, 0};
            int outputFit = 0;

            // Find abs distance to edge of screen
            _rectangle->distanceToEdge(&distToEdge[0]);

            // Set worst case function for smallest edge size 
            _rectangle->FindEdges();
            outputFit = _rectangle->doesItFit();

            // shape does not fit in screen, resize so one of the edges of the 
            // rectangle sits on the edge of the screen
            if (_rectangle->doesItFit() == 1) {
                if (_rectangle->coordinates)
                _rectangle->ResizeRectangle(distToEdge, outputFit);
                outputFit = _rectangle->doesItFit();
            }

            PropertiesVector->push_back(_rectangle);
        }
    }

    free(_shapes);
}

// Instantiates all JSON objects and checks if there are any duplicates
// pointers are created for each data type, memory gets freed after shapes have been displayed
// unordered map is used to check for duplicates
void instantiateObjects(string jsonFile, std::unordered_map<int, shapes*> *_ShapeMap)
{
    // temp variables 
    int keyValue = 0;
    shapes tmpShape;

    // Stream data from file into program
    std::ifstream jsonFstream("shapes.json");
    json outputData = json::parse(jsonFstream);
    int sizeOfData = outputData.size() * 3 / 2;

    // Instantiate all shape objects 
    for (const auto& item : outputData.items())
    {
        // Create new pointer to store data
        shapes *_shapes = new shapes;

        from_json(item.value(), tmpShape);

        // Copy object values to pointer 
        *_shapes = tmpShape;

        // Add to unorder map, used to check if there is duplicate data 
        keyValue = HashFunc(_shapes->id, sizeOfData);

        // // Checks if there is already an entry at this key
        // if (ShapeMap.find(keyValue) == ShapeMap.end()) {
        //     ShapeMap.insert(std::make_pair(HashFunc(_shapes->id, sizeOfData), _shapes));
        // }
        // else if (ShapeMap.at(keyValue)->id == _shapes->id) {
        //     std::cout << "Objects are equal: " << _shapes->area << std::endl; 
        // }
        // else {
        //     ShapeMap.insert(std::make_pair(HashFunc(_shapes->id, sizeOfData), _shapes));
        // }

        // add instantiated object to unordered map
        _ShapeMap->insert(std::make_pair(HashFunc(_shapes->id, sizeOfData), _shapes));
    }
};

void displayShapes(std::vector<shapes*> displayShape)
{
    shapes *lShape = new shapes;
    // used to display data to ensure it has been parsed correctly
    for (const auto &lShape : displayShape)
    {

        if (lShape->shape == "circle") {
            circle *localCircle = new circle;
            localCircle = static_cast<circle*>(lShape); // derived pointer 

            if (localCircle->doesitFit() != 2)
            {
                std::cout << "Circle ID = " << localCircle->id << " doesn't Fit" << std::endl;
            } 
            else {
                // draw
            }

            // memory of shape
            free(localCircle);
        }

        if (lShape->shape == "square") {
            square *localSquare = new square;
            localSquare = static_cast<square*>(lShape); // derived pointer 

            if (localSquare->doesitFit() != 2)
            {
                std::cout << "Circle ID = " << localSquare->id << " doesn't Fit" << std::endl;
            } 
            else {
                // draw
            }

            // Free pointers described above
            free(localSquare);
        }

        if (lShape->shape == "rectangle") {
            rectangle *localRectangle = new rectangle;
            localRectangle = static_cast<rectangle*>(lShape); // derived pointer 

            if (localRectangle->doesitFit() != 2)
            {
                std::cout << "Circle ID = " << localRectangle->id << " doesn't Fit" << std::endl;
            } 
            else {
                // draw
            }

            // Free pointers described above
            free(localRectangle);
        }
    }
}

int main()
{
    string jsonFileName = "shapes";

    // Create temp variables for processing
    std::unordered_map<int, shapes*> ShapeMap;
    std::vector<shapes*> ShapePropertiesVector;

    // uMap used to deterine if 
    instantiateObjects(jsonFileName, &ShapeMap);

    // Find properties of shapes  
    FindProperties(&ShapePropertiesVector, ShapeMap);

    // Display shapes on screen
    displayShapes(ShapePropertiesVector);

    return 0;
}