/*
  ==============================================================================

    ShiftyLooping.cpp
    Created: 23 Aug 2014 12:30:11am
    Author:  milrob

  ==============================================================================
*/

#include "JuceHeader.h"
#include "LoopGenerator.h"


class ShiftyLooper : public drow::AudioFilePlayer::Listener, public Timer {
    
    
public:
    ShiftyLooper(drow::AudioFilePlayerExt& afp, const std::vector<Loop>& _loops,
                  Loop& current, std::vector<int> seq) :
                     player(afp),loops(_loops),
                    currentLoop(current), sequence(seq), requestForSeqUpdate(false), seqIdx(0)
    {
        start();
    }
    
    ~ShiftyLooper(){}
    
    void start(){
        startTimer(50);
        shiftyLooping(currentLoop);
    }
    void stop(){
        player.stop();
        stopTimer();
    }
    
    //Loop& getCurrentLoop() const               { return currentLoop; }
    //void setCurrentLoop(Loop& curr)            { currentLoop = curr; }
    void updateSequence(std::vector<int> _seq) { sequence = _seq; }
    
    void fileChanged(drow::AudioFilePlayer* player) override {}
    void audioFilePlayerSettingChanged(drow::AudioFilePlayer* player,
                                       int settingCode) override {}
    void playerStoppedOrStarted(drow::AudioFilePlayer* player) override {}
    
    void timerCallback() override {
        if (player.hasStreamFinished())
            shiftyLooping(currentLoop);
    }
    
private:
    ShiftyLooper(const ShiftyLooper& sl) = delete;
    ShiftyLooper(ShiftyLooper&& sl) = delete;
    ShiftyLooper& operator=(const ShiftyLooper& rhs) = default;
    ShiftyLooper& operator=(ShiftyLooper&& rhs) = delete;
    
    drow::AudioFilePlayerExt& player;
    std::vector<Loop> loops;
    Loop& currentLoop;
    std::vector<int> sequence;
    bool shifting, forward, requestForSeqUpdate;
    int seqIdx;
    Random r;
    
    void shiftyLooping(Loop& loop){
        if (shifting) {
            if (forward) {
                //printCurrentState("Shifting Forward");
                loop = *loop.next;
                player.setLoopTimes(loop.prev->end, loop.end);
                player.setPosition(loop.prev->end);
                player.start();
                player.setLoopTimes(loop.start, loop.end);
            } else {
                loop = *loop.prev;
                if (loop.next->start >= loop.end) return;
                //printCurrentState("Shifting Backwards");
                loop = *loop.prev;
                player.setLoopTimes(loop.next->start, loop.end);
                player.setPosition(loop.next->start);
                player.start();
                player.setLoopTimes(loop.start, loop.end);
            }
        } else {
            player.setLoopTimes(loop.start, loop.end);
            player.setPosition(loop.start);
            player.setLoopBetweenTimes(true);
        }
        
        pivot(currentLoop);
        
    }
    
    void pivot(Loop& loop){

        seqIdx++;
        currentLoop = loops[sequence[seqIdx]];
        
        if (sequence.at(seqIdx+1) > sequence[seqIdx]) {
            shifting = forward = true;
        } else if (sequence.at(seqIdx+1) < sequence[seqIdx]) {
            shifting = true;
            forward = false;
        } else {
            shifting = false;
        }
        
        if (++seqIdx > sequence.size()){
            requestForSeqUpdate = true;
            seqIdx = 0;
        }
        
        shiftyLooping(currentLoop);
    }
};