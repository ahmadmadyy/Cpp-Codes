#include <iostream>
#include <string>
#include <memory> 

enum Color 
{
    Red,
    Green,
    Blue,
    Yellow,
    Orange
};

class Candle 
{
    private:
        float height;
        Color color;

    protected:
        void setHeight(float h) { height = h; }
        void setColor(Color c) { color = c; }

    public:
        Candle() : height(0.0), color(Red) {}
        Candle(float h, Color c) : height(h), color(c) {}
        void data_output() const;
        void update_height(float burning_hours);
};

void Candle::data_output() const 
{
    std::string colorStr;
    switch (color) 
    {
        case Red: colorStr = "Red"; break;
        case Green: colorStr = "Green"; break;
        case Blue: colorStr = "Blue"; break;
        case Yellow: colorStr = "Yellow"; break;
        case Orange: colorStr = "Orange"; break;
    }
    std::cout << "Height: " << height << " cm, Color: " << colorStr << std::endl;
}

void Candle::update_height(float burning_hours) 
{
    float burn_amount = burning_hours * 1.5;
    height -= burn_amount;

    if (height <= 0) 
    {
        height = 0;
        std::cout << "The candle has burned down completely." << std::endl;
    } 
    else 
    {
        std::cout << "The candle's current height is: " << height << " cm" << std::endl;
    }
}

int main() 
{
    std::shared_ptr<Candle> candle1 = std::make_shared<Candle>(20.0, Red);
    std::shared_ptr<Candle> candle2 = std::make_shared<Candle>(15.0, Green);

    candle2->data_output();
    candle1->update_height(10);

    return 0;
}
