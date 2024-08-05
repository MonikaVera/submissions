#include "weather.h"

Sunny* Sunny::_instance = nullptr;
Sunny* Sunny::instance()
{
    if(_instance == nullptr) {
        _instance = new Sunny();
    }
    return _instance;
}

void Sunny::change(Green* land, double* humidityAll) {
    land->changeWater(-6);
    *humidityAll+=7;
}
void Sunny::change(Sheer* land, double* humidityAll) {
    land->changeWater(-3);
    *humidityAll+=3; 
}
void Sunny::change(Lake* land, double* humidityAll) {
    land->changeWater(-10);
    *humidityAll+=10;
}

Cloudy* Cloudy::_instance = nullptr;
Cloudy* Cloudy::instance()
{
    if(_instance == nullptr) {
        _instance = new Cloudy();
    }
    return _instance;
}

void Cloudy::change(Green* land, double* humidityAll) {
    land->changeWater(-2);
    *humidityAll+=7;
}
void Cloudy::change(Sheer* land, double* humidityAll) {
    land->changeWater(-1);
    *humidityAll+=3; 
}
void Cloudy::change(Lake* land, double* humidityAll) {
    land->changeWater(-3);
    *humidityAll+=10;
}

Rainy* Rainy::_instance = nullptr;
Rainy* Rainy::instance()
{
    if(_instance == nullptr) {
        _instance = new Rainy();
    }
    return _instance;
}

void Rainy::change(Green* land, double* humidityAll) {
    land->changeWater(10);
    *humidityAll+=7;
}
void Rainy::change(Sheer* land, double* humidityAll) {
    land->changeWater(5);
    *humidityAll+=3;
}
void Rainy::change(Lake* land, double* humidityAll) {
    land->changeWater(15);
    *humidityAll+=10;
}

Weather* Weather::create(const weatherTypes wt) {
    switch(wt) {
        case RAINY: return Rainy::instance();
        break;
        case SUNNY: return Sunny::instance();
        break;
        case CLOUDY: return Cloudy::instance();
        break;
    }
}
