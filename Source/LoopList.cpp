/*
  ==============================================================================

    LoopList.cpp
    Created: 22 Aug 2014 7:40:16pm
    Author:  milrob

  ==============================================================================
*/

#include "JuceHeader.h"
#include "LoopGenerator.h"
#include <algorithm>

class TableWindow : public DocumentWindow {
public:
    TableWindow(const String& name, Colour backgroundColour, int buttonsNeeded):
    DocumentWindow(name, backgroundColour, buttonsNeeded) {}
    
    void closedButtonPressed(){ delete this; }
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TableWindow);
};

class LoopTable : public Component, public TableListBoxModel {
public:
    LoopTable(const std::vector<Loop>& loops) :
        font(14.0f), loopData(loops)
    {
            columnList = {"Loop", "Start", "End", "Length",
                          "Duration (s)", "Audition", "Discard"};
            setNumRows(loopData.size());
            //Create table and add it to this component
            addAndMakeVisible(table);
            table.setModel(this);
            
            //give table a border
            table.setColour(ListBox::outlineColourId, Colours::cornflowerblue);
            table.setOutlineThickness(2);
            table.setOpaque(true);
            
            //Add columns to the table header
            for (int i = 0; i < columnList.size(); ++i) {
                table.getHeader().addColumn(columnList[i], i+1, 0, 50, 400,
                                            TableHeaderComponent::defaultFlags);
            }
        
        table.getHeader().setSortColumnId(1, true); //sort by column id
        table.getHeader().setColumnVisible(7, false);
        
        //stretch to fit
        table.getHeader().setStretchToFitActive(true);
        table.setMultipleSelectionEnabled(true);
        
    }
    
    int getNumRows() override { return numRows; }
    void setNumRows(int rows)  { numRows = rows; }
    
    void paintRowBackground(Graphics& g, int, int, int, bool rowIsSelected) override {
        if (rowIsSelected) {
            g.fillAll(Colours::lightcoral);
        }
    }
    
    void paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool) override {
        g.setColour(Colours::black);
        g.setFont(font);
        //Draw loop data here
        
        g.setColour((Colours::black.withAlpha(0.2f)));
        g.fillRect(width - 1, 0, 1, height);
    }
    
    void sortOrderChanged(int newSortcolumnId, bool isForwards) override {
        if (newSortcolumnId != 0) {
            table.updateContent();
        }
    }
    
    Component* refreshComponentForCell(int rowNumber, int columnId, bool,
                                       Component* existingComponentToUpdate) override
    {
        //add Discard and Audition buttons here
        Component* foo = new Component();
        return foo;
    }
    
    void resized() override {
        table.setBoundsInset(BorderSize<int>(8));
    }
    
    void openTableWindow(){
        DialogWindow::LaunchOptions options;
        Label* label = new Label();
        label->setText("Table", dontSendNotification);
        label->setColour(Label::textColourId, Colours::whitesmoke);
        options.content.setNonOwned(label);
        
        int margin = 10;
        Rectangle<int> area(0,0,table.getWidth() + margin, table.getHeight() + margin);
        options.content->setSize(area.getWidth(), area.getHeight());
        
        options.dialogTitle = "Loop List";
        options.dialogBackgroundColour = Colours::floralwhite;
        options.escapeKeyTriggersCloseButton = true;
        options.useNativeTitleBar = true;
        options.resizable = true;
        
        const RectanglePlacement placement(RectanglePlacement::xRight + RectanglePlacement::yBottom + RectanglePlacement::doNotResize);
        DialogWindow* dw = options.launchAsync();
        dw->centreWithSize(area.getWidth(), area.getHeight());
    }
    
    
private:
    TableListBox table;
    Font font;
    std::vector<Loop> loopData;
    
//    XmlElement* columnList;
//    XmlElement* dataList;
    int numRows;
    std::vector<std::string> columnList;
    
    OwnedArray<TextButton> auditionButtons;
    OwnedArray<ToggleButton> keepOrDiscardButtons;
    
   
};
