#ifndef PLAYER_H_
#define PLAYER_H_

#include <ev++.h>

class PlayerContext;
class PlayerPrivate;

class Player
{
public:
    Player(ev::default_loop &loop);

    virtual ~Player();
    
    void On();
    void Off();
    
  private:

    PlayerPrivate *m_pPrivate;// Actions implementation class
    PlayerContext *m_pContext;// Generated class
    
    friend std::ostream & operator<<(std::ostream &os, const Player &player);
    friend std::istream & operator>>(std::istream &is, const Player &player);

};

#endif
