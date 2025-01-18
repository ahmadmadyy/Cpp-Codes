#include <iostream>
#include <string>

class Property 
{
    private:
        float area;
        std::string location;
        std::string street;

    protected:
        void setArea(float a) { area = a; }
        void setLocation(std::string l) { location = l; }
        void setStreet(std::string s) { street = s; }

    public:
        static int count;

        Property() : area(0.0), location(""), street("") 
        {
            count++;
        }

        Property(float a, std::string l, std::string s) : area(a), location(l), street(s) 
        {
            count++;
        }

        virtual void print() const = 0;

        virtual ~Property() 
        {
            count--;
        }

        float getArea() const { return area; }
        std::string getLocation() const { return location; }
        std::string getStreet() const { return street; }
    };

int Property::count = 0;

class House : public Property 
{
    private:
        int floors;
        bool furnished;

    public:
        House() : Property(), floors(0), furnished(false) 
        {
            std::cout << "Object created" << std::endl;
        }

        House(float a, std::string l, std::string s, int f, bool fur): Property(a, l, s), floors(f), furnished(fur) 
        {
            std::cout << "Object created" << std::endl;
        }

        void print() const override 
        {
            std::cout << "Area: " << getArea() << " sq.m, Location: " << getLocation()
                << ", Street: " << getStreet()
                << ", Floors: " << floors
                << ", Furnished: " << (furnished ? "Yes" : "No") << std::endl;
        }
};

int main() 
{
    House h1(200.0, "Steinbuhl", "Allersberger Strasse", 2, true);
    h1.print();

    House h2;
    h2.print();

    std::cout << "Total Properties: " << Property::count << std::endl;

    return 0;
}
