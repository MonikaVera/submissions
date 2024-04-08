import json
import sys;

def findLink(startPoint, endPoint, links) :
    for link in links:
        if(startPoint==(link["points"])[0] and endPoint==(link["points"])[1]) :
            return link

def findPossibleCircuit(start, end, links, possibleCircuits, demand) :
    for index, possibleCircuit in enumerate(possibleCircuits) :
        if possibleCircuit[0]==start and possibleCircuit[len(possibleCircuit)-1]==end:
            allLinks=True
            for i in range(0, len(possibleCircuit)-1, 1) :
                link = findLink(possibleCircuit[i], possibleCircuit[i+1], links)
                if(int(link["capacity"])<demand) :
                    allLinks=False 
            if(allLinks) :
                return (index, possibleCircuit)

def changeCircuitCapacity(circuit, links, demand) : 
    for i in range(0, len(circuit)-1, 1) :
        link = findLink(circuit[i], circuit[i+1], links)
        (link["capacity"])=int(link["capacity"])+demand  

def findReservedCircuitIndex(reservedCircuits, demandIndex) :
    for reservedCircuit in reservedCircuits :
        if(reservedCircuit["demandIndex"]==demandIndex) :
            return reservedCircuit["circuitIndex"]

def readFromData(data) :
    for key, value in data:
        if(key=='links') : 
            links=value
        if(key=='possible-circuits') : 
            possibleCircuits=value
        if(key=='simulation') :
            for key2, value2 in value.items():
                if(key2=='duration') :
                    duration=value2
                if(key2=='demands') :
                    demands=value2
    return (links, possibleCircuits, duration, demands)

def printData(eventIndex, name, start, end, stTime, success) :
    print(str(eventIndex) + ". igény " + name + ": " + start + "<->" + end + " st:" + str(stTime)  + success)

try:
    with open(sys.argv[1], "r") as read_file:
        data = json.load(read_file)

    dataRead = readFromData(data.items())
    if not(dataRead is None) :
        (links, possibleCircuits, duration, demands) = dataRead
    
    reservedCircuits = []
    eventIndex = 0
    for i in range(1,duration+1,1) :
        for index, item in enumerate(demands) :
            if(int(item["start-time"])==i) :
                eventIndex=eventIndex+1
                result = findPossibleCircuit((item["end-points"])[0], (item["end-points"])[1], links, possibleCircuits, int(item["demand"]))
                if(result is None) :
                    printData(eventIndex, "foglalás", (item["end-points"])[0], (item["end-points"])[1], i, " - sikertelen")
                else :
                    (j, possibleCircuit) = result
                    changeCircuitCapacity(possibleCircuit, links, int(item["demand"])*(-1))
                    reservedCircuits.append({"demandIndex":index, "circuitIndex":j})
                    printData(eventIndex, "foglalás", (item["end-points"])[0], (item["end-points"])[1], i, " - sikeres")
            if(int(item["end-time"])==i) :
                eventIndex=eventIndex+1
                circuitIndex = findReservedCircuitIndex(reservedCircuits, index)
                if(not(circuitIndex is None)) :
                    changeCircuitCapacity(possibleCircuits[circuitIndex], links, int(item["demand"]))
                    reservedCircuits.remove({"demandIndex":index, "circuitIndex":circuitIndex})
                    printData(eventIndex, "felszabadítás", (item["end-points"])[0], (item["end-points"])[1], i, "")
except Exception as e:
    print(e)    