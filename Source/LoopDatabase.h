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

#define ForEachButton(array, index) for(int index = 0; index < array.size(); ++index)

class LoopTableData : public Component, public TableListBoxModel, public ButtonListener {
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
    void writeXmlToDisk(const ValueTree& info);

    void buttonClicked(Button* button) override;

   // void addButton(const String&& type);

    
private:
    Font font;
    TableListBox table;
    
    TextButton* saveButton;
    
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
    static const Identifier loopOrd;
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

    class AuditionDiscardComponent : public Component, public ButtonListener {
    public:
        AuditionDiscardComponent (LoopTableData& owner_, int rows, String name) :
            owner (owner_), numButtons(rows), buttonName(name)
        {
            for (int i = 0; i < numButtons; ++i){
                createButton(buttonName);
            }
        }
 
        void resized() override {
            ForEachButton(buttons, index){
                buttons[index]->setBoundsInset(BorderSize<int>(2));
            }
        }

        void buttonClicked(Button* button) override {
            ForEachButton(buttons, index){
                if (button == buttons[index]){
                    std::cout << "Button Pressed: " << index << std::endl;
                }
            }
        }

    void setRowAndColumn (const int newRow, const int newColumn){
        row = newRow;
        columnId = newColumn;
//        comboBox.setSelectedId (owner.getRating (row), dontSendNotification);
    }


    private:
        LoopTableData& owner;
        OwnedArray<TextButton> buttons;
        int row, columnId;
        const int numButtons;
        bool auditionColumn;
        String buttonName;

        TextButton* createButton(String n){
            TextButton* button = new TextButton(n);
            buttons.add(button);
            addAndMakeVisible(button);
            button->addListener(this);
            return button;
        }
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
