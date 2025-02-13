//
// Created by sebastian on 02.07.19.
//

#ifndef RAYTRACECORE_OCCLUSIONSHADER_H
#define RAYTRACECORE_OCCLUSIONSHADER_H

#include "RayEngine.h"

/**
 * Default implementation of an occlusion shader.
 */
class BasicOcclusionShader : public OcclusionShader{
public:
    BasicOcclusionShader(){

    }
    BasicOcclusionShader(const BasicOcclusionShader& copy){
        // TODO
    }

    Shader* clone() override{
        return new BasicOcclusionShader(*this);
    }

    ShaderOutput shade(uint64_t id, PipelineInfo *pipelineInfo, OcclusionShaderInput *shaderInput, void* dataInput, RayGeneratorOutput *newRays) override{
        // TODO
        return ShaderOutput();
    }
};

#endif //RAYTRACECORE_OCCLUSIONSHADER_H
