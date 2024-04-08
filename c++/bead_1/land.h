#define MAIN_MODE //itt kell manualisan valtani

#ifndef LAND_H
#define LAND_H
#include <iostream>

class Weather;

class Land {
    protected:
    std::string name;
    double amountOfWater;
    static Weather* which(double* humidityAll);
    private:
    static int createRandOrConstNumber();
    public:
    void changeWater(double w);
    double getAmountWater() const;
    virtual Land* changeLand(double* humidityAll)=0;
    virtual ~Land() {};
    std::string data() const;
    class IncorrectAmountOfWater : std::exception {};
    virtual bool isGreen() const {return false;}
    virtual bool isSheer() const {return false;}
    virtual bool isLake() const {return false;}
    bool sameType(Land* land) const;
};

class Green : public Land {
    public:
    Green(std::string name_, double amountOfWater_);
    Land* changeLand(double* humidityAll) override;
    bool isGreen() const override {return true;} ;
};

class Sheer : public Land {
    public:
    Sheer(std::string name_, double amountOfWater_);
    Land* changeLand(double* humidityAll) override;
    bool isSheer() const override {return true;} ;
};

class Lake : public Land {
    public:
    Lake(std::string name_, double amountOfWater_);
    Land* changeLand(double* humidityAll) override;
    bool isLake() const override {return true;} ;
};

#endif