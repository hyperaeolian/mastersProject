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


class LoopTable : public Component, public TableListBoxModel
{
public:
    LoopTable(const std::vector<Loop>& loops) :
        font(14.0f), loopData(loops)
    {
        loadData();
            columnList = {"Loop", "Start", "End", "Length",
                          "Duration (s)", "Audition", "Discard"};
            //Create table and add it to this component
            addAndMakeVisible(table);
            table.setModel(this);
            
            //give table a border
            table.setColour(ListBox::outlineColourId, Colours::cornflowerblue);
            table.setOutlineThickness(2);
        
            
            //Add columns to the table header
            for (int i = 0; i < columnList.size(); ++i) {
                table.getHeader().addColumn(columnList[i], i+1, 40, 50, 400,
                                            TableHeaderComponent::defaultFlags);
                table.getHeader().setColumnVisible(i+1, false);
            }
        
        table.getHeader().setSortColumnId(1, true); //sort by column id
        
        //stretch to fit
        //table.getHeader().setStretchToFitActive(true);
        table.setMultipleSelectionEnabled(true);
        
    }
    
    int getNumRows() override { return numRows; }
    
    void paintRowBackground(Graphics& g, int, int, int, bool rowIsSelected) override {
        if (rowIsSelected) {
            g.fillAll(Colours::lightcoral);
        }
    }
    
    void paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool) override {
        g.setColour(Colours::black);
        g.setFont(font);
        const String rowElement = data[rowNumber];
        if (rowNumber != 0){
            g.drawText(rowElement, 2, 0, width-4, height, Justification::centred, true);
        }
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
        if (columnId == 6 || columnId == 7){
            AuditionDiscardColumns* audDisc = (AuditionDiscardColumns*) existingComponentToUpdate;
            if (audDisc == 0)
                audDisc = new AuditionDiscardColumns(*this);
            
            audDisc->setRowAndColumn(rowNumber, columnId);
            return audDisc;
        } else {
            jassert(existingComponentToUpdate == 0);
            return 0;
        }
        Component* foo = new Component();
        return foo;
    }
    
    int getColumnAutoSizeWidth(int columnId) override {
        if (columnId == 6 || columnId == 7) return 100;
        int widest = 32;
        for (int i = data.size() ; --i >=0;){
            const String rowElement = data[i];
            if (i != 0) {
                widest = jmax(widest, font.getStringWidth(rowElement));
            }
        }
        return widest + 8;
    }
    
    void resized() override {
        table.setBoundsInset(BorderSize<int>(8));
    }
    
    void loadData(){
        int id = 1;
        for (auto x : loopData){
            String s;
            s << String(x.start) << " " << String(x.end) << " "
              << String((x.end - x.start) * 44100) << " "
              << String(x.end - x.start) << " ";
            data.insert(std::pair<int, juce::String>(id, s));
            id++;
        }
        numRows = loopData.size();
    }
    
private:
    TableListBox table;
    Font font;
    std::vector<Loop> loopData;

    int numRows;
    std::vector<std::string> columnList;
    std::map<int, juce::String> data;
    
    class AuditionDiscardColumns :
        public Component, public ButtonListener
    {
        
    public:
        AuditionDiscardColumns(LoopTable& _owner) : owner(_owner){
            addAndMakeVisible(auditionButton = new TextButton("Audition Button"));
            auditionButton->setButtonText("play");
            auditionButton->addListener(this);
            
            addAndMakeVisible(discardButton = new TextButton("Discard Button"));
            discardButton->setButtonText("discard");
            discardButton->addListener(this);
        }
        
        ~AuditionDiscardColumns(){
            auditionButton = nullptr;
            discardButton = nullptr;
        }
        
        void resized() override {
            auditionButton->setBoundsInset(BorderSize<int>(2));
            discardButton->setBoundsInset(BorderSize<int>(2));
        }
        
        void setRowAndColumn(const int newRow, const int newColumn){
            row = newRow;
            columnId = newColumn;
        }
        
        void buttonClicked(Button* buttonThatWasClicked) override {
            if (buttonThatWasClicked == auditionButton){
                //playLoop routine
            } else if (buttonThatWasClicked == discardButton){
                //discard the Loop
                //need undo functionality
            }
        }
        
    private:
            LoopTable& owner;
            ScopedPointer<TextButton> auditionButton;
            ScopedPointer<TextButton> discardButton;
            int row, columnId;
    };
   
};

class TableWindow : public DocumentWindow {
public:
    TableWindow(const String& name, Colour backgroundColour, int buttonsNeeded, std::vector<Loop> l):
    DocumentWindow(name, backgroundColour, buttonsNeeded), table(new LoopTable(l)){
        setContentOwned(table, true);
        openTableWindow();
    }
    
    
    void openTableWindow(){
       
        DialogWindow::LaunchOptions options;
        //Label* label = new Label();
        //label->setText("Table", dontSendNotification);
        //label->setColour(Label::textColourId, Colours::whitesmoke);
        //options.content.setNonOwned(label);
        Label* label = new Label();
        label->setColour(Label::textColourId, Colours::whitesmoke);
        label->setText("The Table will go here", sendNotification);
        options.content.setNonOwned(label);
        
        //int margin = 10;
        //Rectangle<int> area(0,0,table.getWidth() + margin, table.getHeight() + margin);
        options.content->setSize(800, 600);
        
        options.dialogTitle = "Loop List";
        options.dialogBackgroundColour = Colours::floralwhite;
        options.escapeKeyTriggersCloseButton = true;
        options.useNativeTitleBar = true;
        options.resizable = true;
        
        
        const RectanglePlacement placement(RectanglePlacement::xRight + RectanglePlacement::yBottom + RectanglePlacement::doNotResize);
        DialogWindow* dw = options.launchAsync();
        
        
        bool native = true;
        Rectangle<int> area (0, 0, 800, 600);
        const RectanglePlacement place ((native ? RectanglePlacement::centred : RectanglePlacement::xRight)
                                            + RectanglePlacement::yTop + RectanglePlacement::doNotResize);
        Rectangle<int> result (place.appliedTo (area, Desktop::getInstance().getDisplays().getMainDisplay().userArea.reduced (20)));
        dw->setBounds (result);
        dw->setVisible(true);
    }
    
    void closedButtonPressed(){
        delete table;
        delete this;
    }
    
private:
    LoopTable* table;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TableWindow);
};
