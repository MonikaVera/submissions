#include "land.h"
#include "weather.h"
#include <cstdlib>
#include <ctime>

#ifdef MAIN_MODE 
int Land::createRandOrConstNumber() {
    return std::rand()/((RAND_MAX)/100);
}
#else
int Land::createRandOrConstNumber() {
    return 50;
}
#endif

Land* Green::changeLand(double* humidityAll) {
    (which(humidityAll))->change(this, humidityAll);
    if(amountOfWater<16) {
        Sheer *landNew = new Sheer(name,amountOfWater);
        delete this;
        return landNew;
    }
    if(amountOfWater>50) {
        Lake *landNew = new Lake(name,amountOfWater);
        delete this;
        return landNew;
    }
    return this;
}

Land* Sheer::changeLand(double* humidityAll) {
    (which(humidityAll))->change(this, humidityAll);
    if(amountOfWater>15) {
        Green *landNew = new Green(name,amountOfWater);
        delete this;
        return landNew;
    }
    return this;
}

Land* Lake::changeLand(double* humidityAll) {
    (which(humidityAll))->change(this, humidityAll);
    if(amountOfWater<51) {
        Green *landNew = new Green(name,amountOfWater);
        delete this;
        return landNew;
    }
    return this;
}

Weather* Land::which(double* humidityAll) {
      if(*humidityAll>=70) {
        *humidityAll=30;
        return Weather::create(RAINY);
    } else if(*humidityAll<70 && *humidityAll>=40) {
        int random_variable = createRandOrConstNumber();
        if((*humidityAll-40)*3.3>random_variable) {
            *humidityAll=30;
            return Weather::create(RAINY);
        } else {
            return Weather::create(CLOUDY);
        }
    } else if(*humidityAll<40 && *humidityAll>=0){
        return Weather::create(SUNNY);
    } else {
        *humidityAll=0;
    }
}

bool Land::sameType(Land* land) const {
    return((this->isGreen() && land->isGreen()) || (this->isLake() && land->isLake()) || (this->isSheer() && land->isSheer()));
}

void Land::changeWater(double w) {
    amountOfWater+=w;
    if(amountOfWater<0) amountOfWater=0;
}

double Land::getAmountWater() const {
    return amountOfWater;
}

Green::Green(std::string name_, double amountOfWater_) {
    name=name_;
    amountOfWater=amountOfWater_;
    if(amountOfWater<=15 || amountOfWater>50) throw IncorrectAmountOfWater();
}

Sheer::Sheer(std::string name_, double amountOfWater_) {
    name=name_;
    amountOfWater=amountOfWater_;
    if(amountOfWater<0 || amountOfWater>15) throw IncorrectAmountOfWater();
}

Lake::Lake(std::string name_, double amountOfWater_) {
    name=name_;
    amountOfWater=amountOfWater_;
    if(amountOfWater<=50) throw IncorrectAmountOfWater();
}

std::string Land::data() const {
    std::string str="Name: "+name+", type of land: ";
    if(this->isGreen()) {
        str=str+"Green";
    }
    if(this->isSheer()) {
        str=str+"Sheer";
    }
    if(this->isLake()) {
        str=str+"Lake";
    }
    
    std::string water = std::to_string(amountOfWater);
    for(int i=water.length()-1;i>=0;i--) {
        if(water[i]=='0') {
            water.pop_back();
        } else if(water[i]=='.') {
            water.pop_back();
            break;
        } else {
            break;
        }
    }
    str=str+", amount of water: "+water+"km^3";
    return(str);
}