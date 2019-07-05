//
// Created by sebastian on 02.07.19.
//

#ifndef RAYTRACECORE_MISSSHADER_H
#define RAYTRACECORE_MISSSHADER_H

#include <iostream>
#include "RayEngine.h"

class BasicMissShader : public MissShader{
public:
    ShaderOutput shade(int id, RayTracerOutput shaderInput, void *dataInput) override {
        std::cout << "OMEGALUL\n";
        return ShaderOutput();
    }

    void *getAssociatedData() override {
        return nullptr;
    }
};

#endif //RAYTRACECORE_MISSSHADER_H
