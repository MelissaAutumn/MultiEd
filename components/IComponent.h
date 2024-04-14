//
// Created by melissaa on 2022-04-24.
//

#ifndef MULTIED_ICOMPONENT_H
#define MULTIED_ICOMPONENT_H

namespace Components {
    class IComponent {
    public:
        //virtual ~IComponent() = 0;

        virtual void Init() = 0;

#if 0
        virtual void Add() = 0;

        virtual void Remove() = 0;
#endif
    };
}
#endif //MULTIED_ICOMPONENT_H
