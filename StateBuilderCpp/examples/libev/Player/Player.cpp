#include <iostream>
#include <ObserverStd.h>
#include "Player.h"
#include "PlayerPrivate.h"
#include "PlayerFsm.h" // Generated file

using namespace std;

// Player implementation
Player::Player(ev::default_loop &loop)
{
    m_pPrivate = new PlayerPrivate();
    m_pContext = new PlayerContext(*m_pPrivate, loop);
    m_pContext->SetObserver(fsm::ObserverStd::GetInstance());
    m_pContext->EnterInitialState();
}

Player::~Player()
{
    delete m_pContext;
    delete m_pPrivate;
}

void Player::On()
{
    m_pContext->On();
}

void Player::Off()
{
    m_pContext->Off();
}

// PlayerPrivate implementation
void PlayerPrivate::DoOn()
{
    cout << "DoOn" << endl;
}

void PlayerPrivate::DoOff()
{
    cout << "DoOff" << endl;
}

std::ostream & operator<<(std::ostream &os, const Player &player)
{
      os << *player.m_pContext;
      return os;
};

std::istream & operator>>(std::istream &is, const Player &player)
{
      is >> *player.m_pContext;
      return is;
};