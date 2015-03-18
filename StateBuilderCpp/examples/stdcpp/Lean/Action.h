#ifndef ACTION_H
#define ACTION_H

namespace carefusion {

class Action
{
public:
    Action(int iActionMax = 10000000);

    virtual ~Action();
    
    void On();
    void Off();
    bool HasReachedMaxActions();
    int GetActionCount() const {return _actionCount;};
    void reset(){_actionCount = 0;}
private:
  int _actionCount; 
  int _actionCountMax;
};

} // namespace carefusion {
#endif
