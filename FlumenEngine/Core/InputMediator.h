#pragma once

#include "SDL2/SDL.h"

#include "FlumenCore/Singleton.h"
#include "FlumenCore/Container/Map.hpp"
#include "FlumenCore/Container/Pool.hpp"

namespace engine
{
    class InputMediator : public core::Singleton <InputMediator>
    {
        friend class core::Singleton <InputMediator>;

        struct InputContender
        {
            void *Object;

            int Priority;

            bool operator==(const void *object) {return Object == object;}
        };

        struct ContenderGroup
        {
            container::Pool <InputContender> Contenders;  
        };

        container::StaticMap <ContenderGroup, SDL_Scancode> contenderGroups;

        InputMediator();

    public:
        void AddContender(SDL_Scancode, InputContender);

        void RemoveContender(SDL_Scancode, void *);

        void GetWinner(SDL_Scancode);
    };
}