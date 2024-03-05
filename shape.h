#include "main.h"
#include <array>

using json = nlohmann::json; 

class shapes {
    public:
    std::string id;
    std::string shape;
    float area;
    std::string colour;

    int coordinates[2] = {};

    bool serialiseable;
    std::string shape_variant;
    
    // Used to determne if objects are equal
    bool operator==(const shapes &t); 

    void distanceToEdge(int _coords[]);

    // virtual functions to be redefined in derived classes
    virtual void decomposeArea() {};
    virtual void drawShape() {};
    virtual int doesitFit(void) { return 0; };
};

class circle : public shapes {
    public:
    float radius;

    circle() {};

    circle(shapes _shapes) {
        id = _shapes.id;
        shape = _shapes.shape;
        area = _shapes.area;
        colour = _shapes.colour;
        memcpy(coordinates, _shapes.coordinates, 2); // Copy array  
        serialiseable = _shapes.serialiseable;
        shape_variant = _shapes.shape_variant;
    }

    void decomposeArea(void) override { radius = (sqrt(area / M_PI)); };
    int doesitFit(void);

};

class rectangle : public shapes{

    // Holds smaller edge 
    float base;
    // Holds longer edge
    float height;

    public:
    // default constructor
    rectangle() {};

    rectangle(shapes _shapes) {
        id = _shapes.id;
        shape = _shapes.shape;
        area = _shapes.area;
        colour = _shapes.colour;
        memcpy(coordinates, _shapes.coordinates, 2); // Copy array  
        serialiseable = _shapes.serialiseable;
        shape_variant = _shapes.shape_variant;
    }

    int doesItFit(void);

    int ResizeRectangle(int _distToEdge[], int _output);

    void FindEdges(void);

    // Set height variable
    void setHeight(float _height) { height = _height; };

    // If area is known, and we also know that Area = base x height
    void findBase(void) { base = area / height; };
};

class square : public shapes {
    public:
    float side;

    // default constructor
    square() {};

    // constructor which copys existing shape data from base class to derived class
    square(shapes _shapes) {
        id = _shapes.id;
        shape = _shapes.shape;
        area = _shapes.area;
        colour = _shapes.colour;
        memcpy(coordinates, _shapes.coordinates, 2); // Copy array  
        serialiseable = _shapes.serialiseable;
        shape_variant = _shapes.shape_variant;
    }

    int doesitFit(void) override;

    // If area is known, and we also know that Area = base x height
    void decomposeArea(void) override { side = sqrt(area); };
};

class triangle : public shapes {
    float base;
    float height;
    float angles[3];

    public:
    // default constructor
    triangle() {};

    // constructor which copys existing shape data from base class to derived class
    triangle(shapes _shapes) {
        id = _shapes.id;
        shape = _shapes.shape;
        area = _shapes.area;
        colour = _shapes.colour;
        memcpy(coordinates, _shapes.coordinates, 2); // Copy array  
        serialiseable = _shapes.serialiseable;
        shape_variant = _shapes.shape_variant;
    }
};

// External functions
extern void to_json(json& j, const shapes& s);
extern void from_json(const json& j, shapes& s);