/*The MIT License (MIT)

Copyright (c) 2016 Lucas Borsatto Simão

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
*/

#ifndef SIR_H_
#define SIR_H_

#include "Header.h"

enum { NONE = 0, GOAL = 1, FAULT = 2, PENALTY = 3 };

//Standard colors of the simulator
struct Color{
	float r;
	float g;
	float b;

	Color(){}
	Color(float r0, float g0, float b0):r(r0),g(g0),b(b0){}
};

//All rigid bodies are searched by the vector of BulletObject
struct BulletObject{
    int id;
    string name;
    Color clr;
    bool hit;
    btRigidBody* body;
    btVector3 halfExt; //Used only for compound shapes
    BulletObject(btRigidBody* b,string n,Color clr0) : name(n),body(b),clr(clr0),id(-1),hit(false),halfExt(btVector3(0,0,0)) {}
};

struct Command{
    float left;
    float right;
    Command(){
        left = right = 0;
    };
    Command(float left, float right){
        this->left = left;
        this->right = right;
    };
    Command(Command *cmd){
        left = cmd->left;
        right = cmd->right;
    };
};

#endif
