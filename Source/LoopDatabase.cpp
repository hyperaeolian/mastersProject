/*
  ==============================================================================

    LoopDatabase.cpp
    Created: 9 Dec 2014 10:10:52am
    Author:  milrob

  ==============================================================================
*/

#include "LoopDatabase.h"

const Identifier LoopTableData::databaseID = "Database";

const Identifier LoopTableData::loopId  = "Loops";
const Identifier LoopTableData::startId = "Start";
const Identifier LoopTableData::endId   = "End";
const Identifier LoopTableData::durId   = "Duration";
const Identifier LoopTableData::objId   = "LoopObj";
const Identifier LoopTableData::audId   = "Audition";
const Identifier LoopTableData::disId   = "Discard";
const Identifier LoopTableData::lenId   = "Length";
const Identifier LoopTableData::colTitle  = "Columns";
const Identifier LoopTableData::colTitle2 = "Column";
const Identifier LoopTableData::colId     = "columnId";
const Identifier LoopTableData::colName   = "name";
const Identifier LoopTableData::colWidth  = "width";
const Identifier LoopTableData::loopOrd = "ID";

LoopTableData::LoopTableData(const std::vector<Loop>& _loops) : font(14.0f),
    loops(_loops), width(50)

{
    loopData  = ValueTree(loopId);
    columns   = ValueTree(colTitle);
    _database = ValueTree(databaseID);
    
    createXmlFromLoopInfo();
    
    addAndMakeVisible(table);
    table.setModel(this);
    table.setColour(ListBox::outlineColourId, Colours::grey);
    table.setOutlineThickness(1.5);
    
   // addAndMakeVisible(saveButton);
    
    configureTable();
    table.getHeader().setSortColumnId(1, true);
    table.getHeader().setStretchToFitActive(true);
    table.setMultipleSelectionEnabled(true);
    showTable();
    
}

LoopTableData::~LoopTableData(){
    dataList = nullptr;
    columnList = nullptr;
}

void LoopTableData::configureTable(){
    
    forEachXmlChildElement(*columnList, columnXml){
        table.getHeader().addColumn(columnXml->getStringAttribute("name"),
                                    columnXml->getIntAttribute("columnId"),
                                    columnXml->getIntAttribute("width"), 50, 400,
                                    TableHeaderComponent::defaultFlags);
    }

}

void LoopTableData::resized(){ table.setBoundsInset(BorderSize<int>(8)); }

void LoopTableData::buttonClicked(Button* button){
    if (button == saveButton){
        writeXmlToDisk(_database);
    }
}

void LoopTableData::paintRowBackground(Graphics& g, int, int, int, bool rowIsSelected){
    if (rowIsSelected) g.fillAll(Colours::lightblue);
}


void LoopTableData::paintCell(juce::Graphics &g, int rowNumber, int columnId, int width, int height, bool){
    g.setColour(Colours::black);
    g.setFont(font);
    
    const XmlElement* rowElement = dataList->getChildElement(rowNumber);
    if (rowElement != 0){
        const String text(rowElement->getStringAttribute(getAttributeNameForColumnID(columnId)));
        g.drawText(text, 2, 0, width-4, height, Justification::centredLeft, true);
    }
    g.setColour(Colours::black.withAlpha(0.2f));
    g.fillRect(width-1,0,1,height);
}


void LoopTableData::sortOrderChanged(int newSortColumnId, bool isForwards){
    if (newSortColumnId != 0){
        DataSorterUtil sorter(getAttributeNameForColumnID(newSortColumnId), isForwards);
        dataList->sortChildElements(sorter);
        table.updateContent();
    }
}


Component* LoopTableData::refreshComponentForCell(int rowNumber, int columnId, bool, Component* existingComponentToUpdate){
    if (columnId == 6 || columnId == 7){
        String buttonName = columnId == 6 ? "Audition" : "Discard";
        AuditionDiscardComponent* audOrDiscColumn = (AuditionDiscardComponent*) existingComponentToUpdate;
        if (audOrDiscColumn == 0)
            audOrDiscColumn = new AuditionDiscardComponent(*this, numColumns, buttonName);
        audOrDiscColumn->setRowAndColumn (rowNumber, columnId);
        return audOrDiscColumn;
    }
    else {
        jassert (existingComponentToUpdate == 0);
        return 0;
    }
}

int LoopTableData::getColumnAutoSizeWidth(int columnId){
    if (columnId == 6 || columnId == 7) return 100;
    int widest = 32;
    for (int i = getNumRows(); --i >= 0;){
        const XmlElement* rowElement = dataList->getChildElement(i);
        if (rowElement != 0){
            const String text(rowElement->getStringAttribute(getAttributeNameForColumnID(columnId)));
            widest = jmax (widest, font.getStringWidth (text));
        }
    }
    
    return widest + 8;
}


void LoopTableData::createXmlFromLoopInfo(){
    
    loopData.setProperty(loopId, String("Loop"), &undo);
    loopData.setProperty(startId,String("Start Time"), &undo);
    loopData.setProperty(endId, String("End Time"), &undo);
    loopData.setProperty(durId, String("Duration (secs)"), &undo);
    loopData.setProperty(lenId, String("Samples"), &undo);
    loopData.setProperty(audId, String("Audition"), &undo);
    loopData.setProperty(disId, String("Discard"), &undo);
    
    for (int i = 0; i < loops.size(); ++ i){
        ValueTree loop = ValueTree(objId);
        loop.setProperty(loopOrd, String(i+1), &undo);
        loop.setProperty(startId, String(loops[i].start), &undo);
        loop.setProperty(endId, String(loops[i].end), &undo);
        loop.setProperty(durId, String(loops[i].end - loops[i].start), &undo);
        loop.setProperty(lenId, String(44100 * (loops[i].end - loops[i].start)), &undo);
        loopData.addChild(loop, i, &undo);
    }
    
    std::vector<String> collist = { "ID","Start", "End", "Duration",
                                    "Length", "Audition", "Discard"  };
    
    for (int i = 0; i < numColumns; ++i){
        ValueTree col = ValueTree(colName);
        col.setProperty(colId, String(i+1), &undo);
        col.setProperty(colName, collist[i], &undo);
        col.setProperty(colWidth, String(width), &undo);
        columns.addChild(col, i, &undo);
    }
    
    _database.addChild(columns, 1, &undo);
    _database.addChild(loopData, 2, &undo);
    
    DataBase   = _database.createXml();
    columnList = DataBase->getChildByName("Columns");
    dataList   = DataBase->getChildByName("Loops");
    numRows    = dataList->getNumChildElements();
    
}


String LoopTableData::getAttributeNameForColumnID(const int columnID) const {
    forEachXmlChildElement(*columnList, columnXml){
        if (columnXml->getIntAttribute("columnId") == columnID) {
            return columnXml->getStringAttribute("name");
        }
    }
    
    return String::empty;
}


void LoopTableData::showTable(){
    
    DialogWindow::LaunchOptions options;
    options.content.setNonOwned(&table);
    Rectangle<int> area (0, 0, 600, 450);
    options.content->setSize (area.getWidth(), area.getHeight());
    
    options.dialogTitle                   = "Database of Loops";
    options.dialogBackgroundColour        = Colour(Colours::mediumslateblue);
    options.escapeKeyTriggersCloseButton  = true;
    options.useNativeTitleBar             = true;
    options.resizable                     = true;
    
    const RectanglePlacement placement (RectanglePlacement::xRight + RectanglePlacement::yBottom + RectanglePlacement::doNotResize);
    
    DialogWindow* dw = options.launchAsync();
    dw->centreWithSize(area.getWidth(), area.getHeight());
}


void LoopTableData::writeXmlToDisk(const ValueTree& info){
   
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
