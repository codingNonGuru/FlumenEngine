#include "InputMediator.h"

using namespace engine;

const auto MAXIMUM_KEYS = 64;

const auto CONTENDERS_PER_KEY = 8;

InputMediator::InputMediator()
{
    contenderGroups.Initialize(MAXIMUM_KEYS);
}

void InputMediator::AddContender(SDL_Scancode key, InputContender contender)
{
    auto group = contenderGroups.Get(key);
    if(group == nullptr)
    {
        group = contenderGroups.Add(key);

        group->Contenders.Initialize(CONTENDERS_PER_KEY);
    }

    *group->Contenders.Add() = contender;
}

void InputMediator::RemoveContender(SDL_Scancode key, void *object)
{
    auto group = contenderGroups.Get(key);

    group->Contenders.Remove(object);
}

void InputMediator::GetWinner(SDL_Scancode key)
{
    auto group = contenderGroups.Get(key);

    InputContender *winner = nullptr;
    for(auto &contender : group->Contenders)
    {
        if(winner == nullptr)
        {
            winner = &contender;
        }
        else
        {
            if(contender.Priority > winner->Priority)
            {
                winner = &contender;
            }
        }
    }
}