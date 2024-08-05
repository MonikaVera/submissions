#ifndef WEATHER_H
#define WEATHER_H
#include "land.h"
#include <iostream>

enum weatherTypes {
    RAINY, CLOUDY, SUNNY    
};

class Weather {
    public:
    static Weather* create(const weatherTypes wt);
    virtual void change(Green* land, double* humidityAll)=0;
    virtual void change(Sheer* land, double* humidityAll)=0;
    virtual void change(Lake* land, double* humidityAll)=0;
    virtual ~Weather() {};
};

class Rainy : public Weather {
    public:
    static Rainy* instance();
    void static destroy() { if ( nullptr!=_instance ) delete _instance; _instance = nullptr; }
    void change(Green* land, double* humidityAll) override;
    void change(Sheer* land, double* humidityAll) override;
    void change(Lake* land, double* humidityAll) override;
    private:
    static Rainy* _instance;
};

class Sunny : public Weather {
    public:
    static Sunny* instance();
    void static destroy() { if ( nullptr!=_instance ) delete _instance; _instance = nullptr; }
    void change(Green* land, double* humidityAll) override;
    void change(Sheer* land, double* humidityAll) override;
    void change(Lake* land, double* humidityAll) override;
    private:
    static Sunny* _instance;
};

class Cloudy : public Weather {
    public:
    static Cloudy* instance();
    void static destroy() { if ( nullptr!=_instance ) delete _instance; _instance = nullptr; }
    void change(Green* land, double* humidityAll) override;
    void change(Sheer* land, double* humidityAll) override;
    void change(Lake* land, double* humidityAll) override;
    private:
    static Cloudy* _instance;
};

#endif