//
// Created by sebastian on 02.07.19.
//

#ifndef RAYTRACECORE_RAYENGINE_H
#define RAYTRACECORE_RAYENGINE_H

#include <cstdint>
#include <vector>
#include <unordered_map>

/**
 * Contains all data structures
 */
class DataManagementUnit;

/**
 * Contains x, y and z coordinates representing a vector in 3 dimensions
 */
struct Vector3D{
    double x;
    double y;
    double z;
};

/**
 * RayGeneratorOutput contains the data generated by the RayGeneratorShader. It is used in the ray tracer.
 * rayOrigin:           the point in space the ray is shot from
 * rayDirection:        the direction the ray is shot in
 */
struct RayGeneratorOutput{
    Vector3D rayOrigin;
    Vector3D rayDirection;
};

/**
 * Contains the color represented as rgb generated by the pipeline
 */
struct ShaderOutput{
    uint8_t color[3];
};

/**
 * RayTracerOutput contains the data that is generated by the ray tracer.
 * intersectionPoint:   the intersection point of the ray generated by the RayGeneratorShader and the geometry bound to
 *                      this pipeline
 * normal:              the (interpolated) normal of the triangle hit at intersectionPoint
 * map:                 the (interpolated) mapping coordinates of the triangle hit at intersectionPoint
 * objectId:            the object id of the object hit
 */
struct RayTracerOutput{
    Vector3D intersectionPoint;
    Vector3D normal;
    Vector3D map;
    long objectId;
};

/**
 * Template for the Ray Generator Shader to be implemented. It generates the ray used for ray tracing.
 * getAssociatedData:   returns a pointer to the data that is fed to this RayGeneratorShader on execution
 * shade:               the template for the shader that is used in the pipeline
 */
class RayGeneratorShader{
public:
    virtual void* getAssociatedData() = 0;
    virtual RayGeneratorOutput shade(int id, void* dataInput) = 0;
    virtual ~RayGeneratorShader() = default;
};

/**
 * Template for the Occlusion Shader to be implemented. It is called when a ray hits anything.
 * getAssociatedData:   returns a pointer to the data that is fed to this RayGeneratorShader on execution
 * shade:               the template for the shader that is used in the pipeline
 */
class OcclusionShader{
public:
    virtual void* getAssociatedData() = 0;
    virtual ShaderOutput shade(int id, RayTracerOutput shaderInput, void* dataInput) = 0;
    virtual ~OcclusionShader() = default;
};

/**
 * Template for the Pierce Shader to be implemented. It is called on every object that is hit by a ray.
 * getAssociatedData:   returns a pointer to the data that is fed to this RayGeneratorShader on execution
 * shade:               the template for the shader that is used in the pipeline
 */
class PierceShader{
public:
    virtual void* getAssociatedData() = 0;
    virtual ShaderOutput shade(int id, RayTracerOutput shaderInput, void* dataInput) = 0;
    virtual ~PierceShader() = default;
};

/**
 * Template for the Hit Shader to be implemented. It is called on the closest object hit.
 * getAssociatedData:   returns a pointer to the data that is fed to this RayGeneratorShader on execution
 * shade:               the template for the shader that is used in the pipeline
 */
class HitShader{
public:
    virtual void* getAssociatedData() = 0;
    virtual ShaderOutput shade(int id, RayTracerOutput shaderInput, void* dataInput) = 0;
    virtual ~HitShader() = default;
};

/**
 * Template for the Miss Shader to be implemented. It is called whenever a ray hits no geometry.
 * getAssociatedData:   returns a pointer to the data that is fed to this RayGeneratorShader on execution
 * shade:               the template for the shader that is used in the pipeline
 */
class MissShader{
public:
    virtual void* getAssociatedData() = 0;
    virtual ShaderOutput shade(int id, RayTracerOutput shaderInput, void* dataInput) = 0;
    virtual ~MissShader() = default;
};

/**
 * Template for the Control Shader to be implemented. It is called after a ray has been computed. It is used for data
 * write back and has the ability to call another RayGeneratorShader.
 * getAssociatedData:   returns a pointer to the data that is fed to this RayGeneratorShader on execution
 * shade:               the template for the shader that is used in the pipeline
 */
class ControlShader{
public:
    virtual void* getAssociatedData() = 0;
    virtual int shade(int id, ShaderOutput shaderInput, void* dataInput) = 0;
    virtual ~ControlShader() = default;
};

/**
 * Contains all the information needed that defines a pipeline.
 * Pipeline Model:
 *                                      OcclusionShader
 * RayGeneratorShader -> Ray Tracer ->  HitShader       -> ControlShader
 *                                      PierceShader
 *                                      MissShader
 */
class Pipeline{
private:
    RayGeneratorShader* rayGeneratorShader;
    OcclusionShader* occlusionShader;
    PierceShader* pierceShader;
    HitShader* hitShader;
    MissShader* missShader;
    ControlShader* controlShader;
    int width, height;

public:
    Pipeline();
    ~Pipeline();

    void setResolution(int width, int height);

    bool addRayGeneratorShader(RayGeneratorShader *rayGeneratorShader);
    bool addOcclusionShader(OcclusionShader *occlusionShader);
    bool addPierceShader(PierceShader *pierceShader);
    bool addHitShader(HitShader *hitShader);
    bool addMissShader(MissShader *missShader);
    bool addControlShader(ControlShader *controlShader);
};

/**
 * Contains all the information required to construct a 3d model based on a 3d triangle mesh.
 * vertices:        a list of coordinates that are used as vertices
 * normals:         defines a normal per vertex
 * map:             defines mapping coordinates per vertex
 * ids:             a list of vertex ids that form a triangle
 */
class Object{
private:
    std::vector<double>* vertices;
    std::vector<double>* normals;
    std::vector<double>* map;
    std::vector<uint64_t>* ids;

public:
    Object(std::vector<double>* vertices, std::vector<double>* normals, std::vector<double>* map,
           std::vector<uint64_t>* ids);
    ~Object();
};

/**
 *
 */
class RayEngine{
private:
    DataManagementUnit* dataManagementUnit;

public:
    struct ObjectParameter{
        double bounding;
    };

    RayEngine();
    ~RayEngine();

    int addPipeline(Pipeline const &pipeline);
    bool removePipeline(int id);

    int runPipeline(int id);
    int runAll();

    bool bindGeometryToPipeline(int pipelineId, std::vector<int>* objectIds);

    int addObject(Object const &object, Vector3D position, Vector3D orientation, ObjectParameter objectParameter);

    bool removeObject(int id);

    bool moveObject(int id, Vector3D newPosition);

    bool turnObject(int id, Vector3D newOrientation);

    bool scaleObject(int id, double newScaleFactor);

    bool manipulateObject(int id, Vector3D newPosition, Vector3D newOrientation, double newScaleFactor);

    bool updateObject(int id, Object const &object);
};

#endif //RAYTRACECORE_RAYENGINE_H
