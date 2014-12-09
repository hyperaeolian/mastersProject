/*
  ==============================================================================

    LoopDatabase.cpp
    Created: 9 Dec 2014 10:10:52am
    Author:  milrob

  ==============================================================================
*/

#include "LoopDatabase.h"

const Identifier LoopTableData::loopId  = "Loops";
    const Identifier LoopTableData::startId = "Start";
    const Identifier LoopTableData::endId   = "End";
    const Identifier LoopTableData::durId   = "Duration";
const Identifier LoopTableData::actId   = "LoopObj";
    const Identifier LoopTableData::audId   = "Audition";
    const Identifier LoopTableData::disId   = "Discard";
    const Identifier LoopTableData::lenId   = "Length";

LoopTableData::LoopTableData(const std::vector<Loop>& _loops) : loops(_loops){
    loopData = ValueTree(loopId);
    loopData.setProperty(loopId, String("Loop"), &undo);
    loopData.setProperty(startId, String("Start Time"), &undo);
    loopData.setProperty(endId, String("End Time"), &undo);
    loopData.setProperty(durId, String("Duration (secs)"), &undo);
    loopData.setProperty(lenId, String("Samples"), &undo);
    loopData.setProperty(audId, String("Audition"), &undo);
    loopData.setProperty(disId, String("Discard"), &undo);
}


LoopTableData::~LoopTableData(){
    dataList = nullptr;
    columnList = nullptr;
}


void LoopTableData::extract_loop_info(){
    for (int i = 0; i < loops.size(); ++ i){
        ValueTree loop = ValueTree(actId);
        loop.setProperty(startId, String(loops[i].start), &undo);
        loop.setProperty(endId, String(loops[i].end), &undo);
        loop.setProperty(durId, String(loops[i].end - loops[i].start), &undo);
        loop.setProperty(lenId, String(44100 * (loops[i].end - loops[i].start)), &undo);
        loopData.addChild(loop, i, &undo);
    }

}

void LoopTableData::convert_to_xml(const ValueTree& info){
    FileChooser chooser("Save", File::nonexistent, "*.xml");
    if (chooser.browseForFileToSave(false));
        File file(chooser.getResult());
    if (file.existsAsFile()) {
        file.moveToTrash();
    }
    FileOutputStream stream(file);
    ScopedPointer<XmlElement> info_xml = info.createXml();
    info_xml->writeToStream(stream, String::empty);
}

void LoopTableData::print(){
    extract_loop_info();
    convert_to_xml(loopData);
}
