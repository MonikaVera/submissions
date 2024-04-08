#include <iostream>
#include "weather.h"
#include "land.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <fstream>

#define MAX_ROUNDS 500

using namespace std;

class NoInfiniteLoopsException : exception {};
class FileNotFound : std::exception {};
class IncorrectInputException : std::exception {};
class IncorrectHumidityException : std::exception {};

std::vector<Land*> read(std::string fileName, double* humidityAll) {
    std::vector<Land*> lands;
    std::ifstream file;
    file.open(fileName);
    if(file.fail()) throw FileNotFound();
    std::string name;
    std::string type;
    double amountOfWater;
    int lengthOfInput;
    file>>lengthOfInput;
    if(file.fail()) throw IncorrectInputException();
    for(int i=0; i<lengthOfInput; i++) {
        file>>name>>type>>amountOfWater;
        if(file.fail()) throw IncorrectInputException();
        if(type=="t") {
            Land* newLand = new Lake(name, amountOfWater);
            lands.push_back(newLand);
        } else if(type=="z") {
            Land* newLand = new Green(name, amountOfWater); 
            lands.push_back(newLand);
        } else if(type=="p") {
            Land* newLand = new Sheer(name, amountOfWater);
            lands.push_back(newLand);
        } else {
            throw IncorrectInputException();
        }
    }
    file>>*humidityAll;
    if(file.fail()) throw IncorrectInputException();
    if(*humidityAll<0 || *humidityAll>100) throw IncorrectHumidityException();
    file.close();
    return lands;    
}

void populating(vector<Land*>* lands, double* humidityAll, string file) {
    try {
        std::srand(std::time(nullptr));
        string fileName;
        if(file=="cin") {
            cout<<"Give me the name of the file!"<<endl;
            cin>>fileName;
        } else {
            fileName=file;
        }
        *lands = read(fileName, humidityAll);
    } catch(FileNotFound) {
        cerr<<"File not found!"<<endl;
    } catch(IncorrectInputException) {
        cerr<<"The input is not correct"<<endl;
    } catch(IncorrectHumidityException) {
        cerr<<"Incorrect amount of humidity!"<<endl;
    } catch(Land::IncorrectAmountOfWater) {
        cerr<<"The amount of the water is incorrect!"<<endl;
    }
}

int core(vector<Land*>* lands, double* humidityAll) {
    try {
        bool right=false;
        int i=0;
        while(!right) { 
            if(i<=MAX_ROUNDS) {
                right=true;
                for(int i=0; i<lands->size(); i++) {
                    lands->at(i)=(lands->at(i))->changeLand(humidityAll);
                    right=true;
                    for(int i=1; i<lands->size(); i++) {
                        if(!(lands->at(i-1))->sameType(lands->at(i))) right=false;
                    }
                    if(right) {
                        break;
                    }
                }
                for(int i=0; i<lands->size(); i++) {
                    cout<<(lands->at(i))->data()<<endl;
                }
                cout<<endl;
                i++;  
            } else {
                throw NoInfiniteLoopsException();
            }  
        }
        return i;
    } catch(NoInfiniteLoopsException) {
        cerr<<"There can be only " << MAX_ROUNDS << " rounds!"<<endl;
    }
}

void destroyAll(vector<Land*>*lands) {
    for(int i=0; i<lands->size(); i++) {
        delete lands->at(i);
    }
    Sunny::destroy();
    Rainy::destroy();
    Cloudy::destroy();
}


#ifdef MAIN_MODE

int main() {
    double humidityAll;
    vector<Land*>lands;
    populating(&lands, &humidityAll, "cin");
    core(&lands, &humidityAll);
    destroyAll(&lands);
    return 0;
}

#else
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("1", "testWeather") {
    double humidityAll=0;
    Green* green = new Green("Green", 30);
    Lake* lake = new Lake("Lake", 60);
    Sheer* sheer = new Sheer("Sheer", 10);
    Weather* r = Weather::create(RAINY);
    Weather* s = Weather::create(SUNNY);
    Weather* c = Weather::create(CLOUDY);
    r->change(green, &humidityAll);
    CHECK(green->getAmountWater()==40);
    CHECK(humidityAll==7);
    r->change(lake, &humidityAll);
    CHECK(lake->getAmountWater()==75);
    CHECK(humidityAll==17);
    r->change(sheer, &humidityAll);
    CHECK(sheer->getAmountWater()==15);
    CHECK(humidityAll==20);

    s->change(green, &humidityAll);
    CHECK(green->getAmountWater()==34);
    CHECK(humidityAll==27);
    s->change(lake, &humidityAll);
    CHECK(lake->getAmountWater()==65);
    CHECK(humidityAll==37);
    s->change(sheer, &humidityAll);
    CHECK(sheer->getAmountWater()==12);
    CHECK(humidityAll==40);

    c->change(green, &humidityAll);
    CHECK(green->getAmountWater()==32);
    CHECK(humidityAll==47);
    c->change(lake, &humidityAll);
    CHECK(lake->getAmountWater()==62);
    CHECK(humidityAll==57);
    c->change(sheer, &humidityAll);
    CHECK(sheer->getAmountWater()==11);
    CHECK(humidityAll==60);

    Sunny::destroy();
    Rainy::destroy();
    Cloudy::destroy();
    delete green;
    delete sheer;
    delete lake;
}

TEST_CASE("2", "testLand") {
    double humidityAll=98;
    Land* green = new Green("Green", 49);
    Land* sheer = new Sheer("Sheer", 13);
    green=green->changeLand(&humidityAll);
    CHECK(green->isLake()==true);
    CHECK(green->getAmountWater()==59);
    humidityAll=1;
    green=green->changeLand(&humidityAll);
    CHECK(green->isGreen()==true);
    CHECK(green->getAmountWater()==49);
    green=green->changeLand(&humidityAll);
    CHECK(green->isGreen()==true);
    CHECK(green->getAmountWater()==43);

    humidityAll=98;
    sheer=sheer->changeLand(&humidityAll);
    CHECK(sheer->isGreen()==true);
    CHECK(sheer->getAmountWater()==18);
    humidityAll=31;
    sheer=sheer->changeLand(&humidityAll);
    CHECK(sheer->isSheer()==true);
    CHECK(sheer->getAmountWater()==12);
    humidityAll=0;
    sheer=sheer->changeLand(&humidityAll);
    CHECK(sheer->isSheer()==true);
    CHECK(sheer->getAmountWater()==9);
    delete green;
    delete sheer;
}

TEST_CASE("3", "inp.txt")
{
    double humidityAll;
    vector<Land*>lands;
    populating(&lands, &humidityAll, "inp.txt");
    CHECK(lands.size()==2);
    CHECK(lands[0]->data()=="Name: Bean, type of land: Sheer, amount of water: 14km^3");
    CHECK(lands[1]->data()=="Name: Dean, type of land: Green, amount of water: 30km^3");
    CHECK(humidityAll==20);
    lands[0]->changeLand(&humidityAll);
    CHECK(lands[0]->data()=="Name: Bean, type of land: Sheer, amount of water: 11km^3");
    CHECK(humidityAll==23);
}

TEST_CASE("4", "sample.txt") {
    double humidityAll;
    vector<Land*>lands;
    populating(&lands, &humidityAll, "sample.txt");
    CHECK(41==core(&lands, &humidityAll));
    CHECK(lands[0]->isSheer()==true);
    destroyAll(&lands);
}

TEST_CASE("5", "in.txt") {
    double humidityAll;
    vector<Land*>lands;
    populating(&lands, &humidityAll, "in.txt");
    CHECK(120==core(&lands, &humidityAll));
    CHECK(lands[0]->isSheer()==true);
    destroyAll(&lands);
}

TEST_CASE("6", "in2.txt") {
    double humidityAll;
    vector<Land*>lands;
    populating(&lands, &humidityAll, "in2.txt");
    CHECK(1==core(&lands, &humidityAll));
    CHECK(lands[0]->isGreen()==true);
    destroyAll(&lands);
}

TEST_CASE("7", "in3.txt") {
    double humidityAll;
    vector<Land*>lands;
    populating(&lands, &humidityAll, "in3.txt");
    CHECK(1==core(&lands, &humidityAll));
    CHECK(lands.size()==0);
    destroyAll(&lands);
}

TEST_CASE("8", "exceptions") {
    CHECK_THROWS(Green("Dean", 10));
    CHECK_THROWS(Green("Dean", 80));
    CHECK_THROWS(Lake("Dean", 22));
    CHECK_THROWS(Sheer("Dean", 22));
    double *humidity;
    CHECK_THROWS(read("in4.txt", humidity));
    CHECK_THROWS(read("in5.txt", humidity));
}

#endif