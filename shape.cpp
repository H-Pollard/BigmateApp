#include "nlohmann\json.hpp"
#include <iostream>
#include "main.h"
#include "shape.h"
#include <string>

using json = nlohmann::json;

// converts from shape object to json
void to_json(json& j, const shapes& s)
{
    j = json{{"_id", s.id},
            {"shape", s.shape},
            {"area", s.area},
            {"colour", s.colour},
            {"coordinates", s.coordinates},
            {"serialiseable", s.serialiseable},
            {"shape_variant", s.shape_variant}};
};

// converts from a json object to a shape class
void from_json(const json& j, shapes& s)
{
    j.at("_id").get_to(s.id);
    j.at("shape").get_to(s.shape);
    j.at("area").get_to(s.area);
    j.at("colour").get_to(s.colour);
    j.at("coordinates")["x"].get_to(s.coordinates[0]);
    j.at("coordinates")["y"].get_to(s.coordinates[1]);
    j.at("serialiseable").get_to(s.serialiseable);
    if (j.at("shape_variant").is_null() == false) {
        j.at("shape_variant").get_to(s.shape_variant);
    }
};

bool shapes::operator==(const shapes &t)
{
    if (id != t.id){
        return false;
    } 
    else if (shape != t.shape){
        return false;
    }
    else if (shape != t.shape){
        return false;
    }
    else if (area != t.area){
        return false;
    }
    else if (colour != t.colour){
        return false;
    }
    else if (coordinates[0] != t.coordinates[0]){
        return false;
    }
    else if (serialiseable != t.serialiseable){
        return false;
    }
    else if (shape_variant != t.shape_variant){
        return false;
    }
    else {
        return true;
    }
};

void shapes::distanceToEdge(int _coords[])
{
    // x position
    // If closer to right edge than to left edge
    if (coordinates[0] > (1920 / 2)) {
        // Distance from right edge
        _coords[0]  = (1920 - coordinates[0]);
    }
    else {
        _coords[0]  = coordinates[0];
    }

    // y position
    // If closer to bottom edge than to top edge
    if (coordinates[1] > (1080 / 2)) {
        // Distance from right edge
        _coords[1]  = (1080 - coordinates[1]);
    }
    else {
        _coords[1]  = coordinates[1];
    }
};

// pass condition return == 2
int square::doesitFit(void)
{
    int outputValue = 0;

    // Checks if both sides fit in screen
    if ((side / 2) <= coordinates[0]) {
        outputValue++;
        if ((side / 2) <= coordinates[1]) {
            outputValue++;
        }
    }

    return outputValue;
};

// pass condition return == 2
int circle::doesitFit(void)
{
    int outputValue = 0;

    if (radius < coordinates[0]) {
        outputValue++;
        if (radius < coordinates[1]) {
            outputValue++;
        }
    }

    return outputValue;
}

// pass condition return == 2
int rectangle::doesItFit(void)
{
    // integer output used to check which side fits
    // if output == 1 the smallest edge fits in the screen
    // if output == 2 both edges fit inside the screen
    int outputValue = 0;

    // Checks if the shape fits in the screen at centoid location
    if (coordinates[0] <= coordinates[1]) {
        if ((base / 2) <= coordinates[0]) {
            outputValue++;
            if ((height / 2) <= coordinates[1]) {
                outputValue++;
            }
        }
    }

    return outputValue;
}

// Sets worse case length for base (smallest edge)
// that still satisfies rectanglecharacteristics base < height
void rectangle::FindEdges (void)
{
    base = sqrt(area) - 0.1;
    height = area / base;
};

// If first iteration did not fit, use distance to edge for next case
int rectangle::ResizeRectangle(int _distToEdge[], int _output)
{
    int outputValue = 0;

    // one of the edges fit in the screen
    if (_output == 1) {
        // if one edge fits 
        if (_distToEdge[0] < _distToEdge[1]) {
            base = (_distToEdge[0] * 2);
            height = area / base;
        }
    }

    return outputValue;
}