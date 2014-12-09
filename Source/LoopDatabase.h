/*
  ==============================================================================

    LoopDatabase.h
    Created: 9 Dec 2014 10:10:52am
    Author:  milrob

  ==============================================================================
*/

#ifndef LOOPDATABASE_H_INCLUDED
#define LOOPDATABASE_H_INCLUDED


#include "JuceHeader.h"
#include "LoopGenerator.h"


class LoopTableData  {
public:
    explicit LoopTableData(const std::vector<Loop>& _loops);
    ~LoopTableData();
    
    void extract_loop_info();
    void convert_to_xml(const ValueTree& info);
    void print();
    
private:
    std::vector<Loop> loops;
    ValueTree loopData;
    
    UndoManager undo;
    
    XmlElement* dataList;
    XmlElement* columnList;

    static const Identifier loopId;
    static const Identifier startId;
    static const Identifier endId;
    static const Identifier durId;
    static const Identifier actId;
    static const Identifier audId;
    static const Identifier disId;
    static const Identifier lenId;
    
};




#endif  // LOOPDATABASE_H_INCLUDED
