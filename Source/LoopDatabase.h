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


class LoopTableData : public Component, public TableListBoxModel {
public:
    explicit LoopTableData(const std::vector<Loop>& _loops);
    ~LoopTableData();
    
    //Overloads from Public TableListBoxModel
    int getNumRows() override { return numRows; }
    void paintRowBackground(Graphics& g, int, int, int, bool rowIsSelected) override;
    void paintCell(Graphics& g, int rowNumber, int columnId, int width,int height, bool) override;
    void sortOrderChanged(int newSortColumnId, bool isForwards) override;
    Component* refreshComponentForCell(int rowNumber, int columnId, bool, Component* existingComponentToUpdate) override;
    int getColumnAutoSizeWidth(int columnId) override;

    //Public Component
    void resized() override;

    void createXmlFromLoopInfo();
    String getAttributeNameForColumnID(const int columnID) const;
    void showTable();
    void debug(const ValueTree& info);
    
    int getRating (const int rowNumber) const{
        return dataList->getChildElement (rowNumber)->getIntAttribute ("Rating");
    }
    
    void setRating (const int rowNumber, const int newRating){
        dataList->getChildElement (rowNumber)->setAttribute ("Rating", newRating);
    }

    
private:
    Font font;
    TableListBox table;
    
    TextButton* debugButton;
    
    std::vector<Loop> loops;
    ValueTree _database, loopData, columns;
    
    UndoManager undo;

    ScopedPointer<XmlElement> DataBase;
    XmlElement* dataList;
    XmlElement* columnList;
    
    const int numColumns = 7;
    int numRows, width;

    static const Identifier databaseID;
    static const Identifier loopId;
    static const Identifier startId;
    static const Identifier endId;
    static const Identifier durId;
    static const Identifier objId;
    static const Identifier audId;
    static const Identifier disId;
    static const Identifier lenId;
    static const Identifier colTitle;
    static const Identifier colTitle2;
    static const Identifier colId;
    static const Identifier colName;
    static const Identifier colWidth;
    
    void configureTable();
    
    class RatingColumnCustomComponent : public Component, public ComboBoxListener {
    public:
        RatingColumnCustomComponent (LoopTableData& owner_)
        : owner (owner_)
        {
            // just put a combo box inside this component
            addAndMakeVisible (comboBox);
            comboBox.addItem ("fab", 1);
            comboBox.addItem ("groovy", 2);
            comboBox.addItem ("hep", 3);
            comboBox.addItem ("neat", 4);
            comboBox.addItem ("wild", 5);
            comboBox.addItem ("swingin", 6);
            comboBox.addItem ("mad for it", 7);
            
            // when the combo is changed, we'll get a callback.
            comboBox.addListener (this);
            comboBox.setWantsKeyboardFocus (false);
        }
        
        void resized() override
        {
        comboBox.setBoundsInset (BorderSize<int> (2));
    }
    
    // Our demo code will call this when we may need to update our contents
    void setRowAndColumn (const int newRow, const int newColumn)
    {
        row = newRow;
        columnId = newColumn;
        comboBox.setSelectedId (owner.getRating (row), dontSendNotification);
    }
    
    void comboBoxChanged (ComboBox* /*comboBoxThatHasChanged*/) override {
    owner.setRating (row, comboBox.getSelectedId());
}

private:
LoopTableData& owner;
ComboBox comboBox;
int row, columnId;
};

class DataSorterUtil {
public:
    DataSorterUtil(const String attributeToSort_, bool forwards) :
    attributeToSort(attributeToSort_), direction(forwards ? 1 : -1){}
    
    int compareElements(XmlElement* first, XmlElement* second) const {
        int result = first->getStringAttribute(attributeToSort).
        compareNatural(second->getStringAttribute(attributeToSort));
        if (result == 0)
            result = first->getStringAttribute("ID")
            .compareNatural(second->getStringAttribute("ID"));
        return direction * result;
    }
private:
    String attributeToSort;
    int direction;
};
    
};






#endif  // LOOPDATABASE_H_INCLUDED
