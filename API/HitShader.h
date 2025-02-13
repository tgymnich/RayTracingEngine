//
// Created by sebastian on 02.07.19.
//

#ifndef RAYTRACECORE_HITSHADER_H
#define RAYTRACECORE_HITSHADER_H

//#include "RayEngine.h"

/**
 * Default implementation of a hit shader. Shades light based on the Phong shading model. Considers textures.
 */
class BasicHitShader : public HitShader{
public:
    BasicHitShader(){

    }
    BasicHitShader(const BasicHitShader& copy){
        // TODO
    }

    Shader* clone() override{
        return new BasicHitShader(*this);
    }

    ShaderOutput shade(uint64_t id, PipelineInfo *pipelineInfo, HitShaderInput *shaderInput, void* dataInput, RayGeneratorOutput *newRays) override{
        Vector3D Ka = {1, 1, 1};
        Vector3D Kd = {1, 1, 1};
        Vector3D Ks = {1, 1, 1};
        uint8_t pix[3];

        auto shaderInputInfo = shaderInput->intersectionInfo;

        unsigned char *image = shaderInputInfo->material->map_Kd.image;

        double_t diffuse = 1, specular = 0.5, exponent = 8, ambient = 0.1;

        if (image == nullptr) {
            if (shaderInputInfo->material->illum == 2) {
                exponent = shaderInputInfo->material->Ns;
                Ka = shaderInputInfo->material->Ka;
                Kd = shaderInputInfo->material->Kd;
                Ks = shaderInputInfo->material->Ks;
            }
            pix[0] = 255;
            pix[1] = 255;
            pix[2] = 255;
        } else {
            int w = shaderInputInfo->material->map_Kd.w;
            int h = shaderInputInfo->material->map_Kd.h;
            double x = fmod(shaderInputInfo->texture.x, 1.0);
            double y = -fmod(shaderInputInfo->texture.y, 1.0);
            if (x < 0) x = 1 + x;
            if (y < 0) y = 1 + y;

            uint64_t pixelCoordinate = ((uint64_t) ((w - 1) * x)) + w * ((uint64_t) ((h - 1) * y));

            pix[0] = image[pixelCoordinate * 3];
            pix[1] = image[pixelCoordinate * 3 + 1];
            pix[2] = image[pixelCoordinate * 3 + 2];

            Ka = shaderInputInfo->material->Ka;
            Kd = shaderInputInfo->material->Kd;
            Ks = shaderInputInfo->material->Ks;
        }

        Vector3D light{};
        light.x = -0.707107;
        light.y = 0.707107;
        light.z = 0;

        ShaderOutput shaderOutput;

        if (shaderInputInfo->distance == std::numeric_limits<double_t>::max()) return shaderOutput;

        Vector3D n{}, l{}, v{}, r{};
        double_t nl;

        l.x = light.x;
        l.y = light.y;
        l.z = light.z;

        n = shaderInputInfo->normal;

        v.x = -1.0 * (shaderInputInfo->rayOrigin.x - pipelineInfo->cameraPosition.x);
        v.y = -1.0 * (shaderInputInfo->rayOrigin.y - pipelineInfo->cameraPosition.y);
        v.z = -1.0 * (shaderInputInfo->rayOrigin.z - pipelineInfo->cameraPosition.z);


        double_t length = sqrt(l.x * l.x + l.y * l.y + l.z * l.z);
        l.x /= length;
        l.y /= length;
        l.z /= length;

        length = sqrt(n.x * n.x + n.y * n.y + n.z * n.z);
        n.x /= length;
        n.y /= length;
        n.z /= length;

        length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
        v.x /= length;
        v.y /= length;
        v.z /= length;

        nl = fmax(n.x * l.x + n.y * l.y + n.z * l.z, 0);

        r.x = (2 * nl * n.x) - l.x;
        r.y = (2 * nl * n.y) - l.y;
        r.z = (2 * nl * n.z) - l.z;

        length = sqrt(r.x * r.x + r.y * r.y + r.z * r.z);
        r.x /= length;
        r.y /= length;
        r.z /= length;

        double_t dot = fmax(v.x * r.x + v.y * r.y + v.z * r.z, 0);

        shaderOutput.color[0] = (uint8_t) fmin((ambient * Ka.x + diffuse * nl * Kd.x + specular * powf(dot, exponent) * Ks.x) * pix[0],
                                  255);
        shaderOutput.color[1] = (uint8_t) fmin((ambient * Ka.y + diffuse * nl * Kd.y + specular * powf(dot, exponent) * Ks.y) * pix[1],
                                  255);
        shaderOutput.color[2] = (uint8_t) fmin((ambient * Ka.z + diffuse * nl * Kd.z + specular * powf(dot, exponent) * Ks.z) * pix[2],
                                  255);

        return shaderOutput;
    }
};

#endif //RAYTRACECORE_HITSHADER_H
