#include <iostream>
#include <string>
#include <cassert>
#include <cstring>

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "Image.h"
#include "MatrixStack.h"

#include "Hit.h"
#include "Camera.h"
#include "Scene.h"
#include "Shape.h"
#include "Sphere.h"
#include "Plane.h"
#include "Mesh.h"
#include "Material.h"

// This allows you to skip the `std::` in front of C++ standard library
// functions. You can also say `using std::cout` to be more selective.
// You should never do this in a header file.
using namespace std;
using namespace glm;

int sceneNum = 1;
int imageWidth = 512;
int imageHeight = 512;

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        cout << "Usage: A6 <Scene> <Image Size> <Output File>" << endl;
        return 0;
    }

    sceneNum = atoi(argv[1]);

    if (argc >= 3)
    {
        imageWidth = abs(atoi(argv[2]));
        imageHeight = abs(atoi(argv[2]));
    }

    if (argc < 3)
    {
        cout << "NOTICE: Image Size not specified, setting to 512 x 512" << endl;
    }

    string outFile = "output.png";

    if (argc < 4)
    {
        cout << "NOTICE: No output file specified, using output.png..." << endl;
    }
    else if (string(argv[3]).length() < 5 || string(argv[3]).find(".png") == string::npos)
    {
        cout << "ERROR: Must include valid output file name (.png)" << endl;
        return -1;
    }

    if (argc >= 4)
    {
        outFile = string(argv[3]);
    }

    if (argc > 4)
    {
        cout << "ERROR: too many arguments" << endl;
    }

    // Load geometry
    vector<float> posBuf; // list of vertex positions
    vector<float> norBuf; // list of vertex normals
    vector<float> texBuf; // list of vertex texture coords
    tinyobj::attrib_t attrib;
    vector<tinyobj::shape_t> shapes;
    vector<tinyobj::material_t> materials;
    string meshName = "../resources/bunny.obj";
    string warnStr, errStr;

    if (sceneNum == 6 || sceneNum == 7)
    {
        bool rc = tinyobj::LoadObj(&attrib, &shapes, &materials, &warnStr, &errStr, meshName.c_str());

        if (!rc)
        {
            cerr << errStr << endl;
        }
        else
        {
            // Some OBJ files have different indices for vertex positions, normals,
            // and texture coordinates. For example, a cube corner vertex may have
            // three different normals. Here, we are going to duplicate all such
            // vertices.
            // Loop over shapes
            for (size_t s = 0; s < shapes.size(); s++)
            {
                // Loop over faces (polygons)
                size_t index_offset = 0;
                for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
                {
                    size_t fv = shapes[s].mesh.num_face_vertices[f];
                    // Loop over vertices in the face.
                    for (size_t v = 0; v < fv; v++)
                    {
                        // access to vertex
                        tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                        posBuf.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
                        posBuf.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
                        posBuf.push_back(attrib.vertices[3 * idx.vertex_index + 2]);
                        if (!attrib.normals.empty())
                        {
                            norBuf.push_back(attrib.normals[3 * idx.normal_index + 0]);
                            norBuf.push_back(attrib.normals[3 * idx.normal_index + 1]);
                            norBuf.push_back(attrib.normals[3 * idx.normal_index + 2]);
                        }
                        if (!attrib.texcoords.empty())
                        {
                            texBuf.push_back(attrib.texcoords[2 * idx.texcoord_index + 0]);
                            texBuf.push_back(attrib.texcoords[2 * idx.texcoord_index + 1]);
                        }
                    }
                    index_offset += fv;
                    // per-face material (IGNORE)
                    shapes[s].mesh.material_ids[f];
                }
            }
        }
        cout << "Number of vertices: " << posBuf.size() / 3 << endl;
    }

    auto img = make_shared<Image>(imageWidth, imageHeight);

    shared_ptr<Camera> camera = make_shared<Camera>();

    camera->setPosition(vec3(0, 0, 5));
    camera->setFOVY(45);
    camera->setTarget(vec3(0, 0, 0));

    if (sceneNum == 1 || sceneNum == 2)
    {
        Scene scene;
        scene.setCamera(camera);
        scene.addLight(Light(vec3(-2, 1, 1), 1));

        Material m0(vec3(0.1, 0.1, 0.1), vec3(1, 0, 0), vec3(1, 1, .5), 100);
        Material m1(vec3(0.1, 0.1, 0.1), vec3(0, 1, 0), vec3(1, 1, .5), 100);
        Material m2(vec3(0.1, 0.1, 0.1), vec3(0, 0, 1), vec3(1, 1, .5), 100);

        shared_ptr<Sphere> s0 = make_shared<Sphere>(vec3(-.5, -1, 1), 0, vec3(1, 1, 1), vec3(1, 1, 1));
        shared_ptr<Sphere> s1 = make_shared<Sphere>(vec3(.5, -1, -1), 0, vec3(1, 1, 1), vec3(1, 1, 1));
        shared_ptr<Sphere> s2 = make_shared<Sphere>(vec3(0, 1, 0), 0, vec3(1, 1, 1), vec3(1, 1, 1));

        s0->setMaterial(m0);
        s1->setMaterial(m1);
        s2->setMaterial(m2);

        scene.addObject(s0);
        scene.addObject(s1);
        scene.addObject(s2);

        scene.drawImage(img);
    }
    else if (sceneNum == 3)
    {
        Scene scene;
        scene.setCamera(camera);
        scene.addLight(Light(vec3(1, 2, 2), .5));
        scene.addLight(Light(vec3(-1, 2, -1), .5));

        Material m0(vec3(0.1, 0.1, 0.1), vec3(1, 0, 0), vec3(1, 1, .5), 100);
        Material m1(vec3(0.1, 0.1, 0.1), vec3(0, 1, 0), vec3(1, 1, .5), 100);
        Material planeMaterial(vec3(0.1, 0.1, 0.1), vec3(1, 1, 1), vec3(0, 0, 0), 0);

        shared_ptr<Sphere> s0 = make_shared<Sphere>(vec3(.5, 0, .5), 0, vec3(1, 1, 1), vec3(.5, .6, .2));
        shared_ptr<Sphere> s1 = make_shared<Sphere>(vec3(-.5, 0, -.5), 0, vec3(1, 1, 1), vec3(1, 1, 1));
        shared_ptr<Plane> p0 = make_shared<Plane>(vec3(0, -1, 0), vec3(0, 1, 0));

        s0->setMaterial(m0);
        s1->setMaterial(m1);
        p0->setMaterial(planeMaterial);

        scene.addObject(s0);
        scene.addObject(s1);
        scene.addObject(p0);

        scene.drawImage(img);
    }
    else if (sceneNum == 4 || sceneNum == 5)
    {
        Scene scene;
        scene.setCamera(camera);
        scene.addLight(Light(vec3(-1, 2, 1), .5));
        scene.addLight(Light(vec3(.5, -.5, 0), .5));

        Material m0(vec3(0.1, 0.1, 0.1), vec3(1, 0, 0), vec3(1, 1, .5), 100);
        Material m1(vec3(0.1, 0.1, 0.1), vec3(0, 0, 1), vec3(1, 1, .5), 100);
        Material planeMaterial(vec3(0.1, 0.1, 0.1), vec3(1, 1, 1), vec3(0, 0, 0), 0);
        Material reflectiveMaterial = Material();

        shared_ptr<Sphere> s0 = make_shared<Sphere>(vec3(0.5, -0.7, 0.5), 0, vec3(1, 1, 1), vec3(.3, .3, .3));
        shared_ptr<Sphere> s1 = make_shared<Sphere>(vec3(1.0, -0.7, 0.0), 0, vec3(1, 1, 1), vec3(.3, .3, .3));
        shared_ptr<Plane> p0 = make_shared<Plane>(vec3(0, -1, 0), vec3(0, 1, 0));
        shared_ptr<Plane> p1 = make_shared<Plane>(vec3(0, 0, -3), vec3(0, 0, 1));

        shared_ptr<Sphere> rs0 = make_shared<Sphere>(vec3(-0.5, 0.0, -0.5), 0, vec3(1, 1, 1), vec3(1.0, 1.0, 1.0));
        shared_ptr<Sphere> rs1 = make_shared<Sphere>(vec3(1.5, 0.0, -1.5), 0, vec3(1, 1, 1), vec3(1.0, 1.0, 1.0));

        s0->setMaterial(m0);
        s1->setMaterial(m1);
        p0->setMaterial(planeMaterial);
        p1->setMaterial(planeMaterial);
        rs0->setMaterial(reflectiveMaterial);
        rs1->setMaterial(reflectiveMaterial);

        scene.addObject(s0);
        scene.addObject(s1);
        scene.addObject(p0);
        scene.addObject(p1);
        scene.addObject(rs0);
        scene.addObject(rs1);

        scene.drawImage(img);
    }
    else if (sceneNum == 6)
    {
        Scene scene;
        scene.setCamera(camera);
        scene.addLight(Light(vec3(-1.0, 1.0, 1.0), 1));

        Material m0(vec3(0.1, 0.1, 0.1), vec3(0, 0, 1), vec3(1, 1, .5), 100);

        shared_ptr<Mesh> m = make_shared<Mesh>(posBuf, norBuf, mat4(1.0f));

        m->setMaterial(m0);

        scene.addObject(m);

        scene.drawImage(img);
    }
    else if (sceneNum == 7)
    {
        Scene scene;
        scene.setCamera(camera);
        scene.addLight(Light(vec3(1, 1, 2), 1));

        shared_ptr<MatrixStack> M = make_shared<MatrixStack>();

        M->translate(.3, -1.5, 0);
        M->rotate(radians(20.0f), vec3(1, 0, 0));
        M->scale(1.5);

        mat4 E = M->topMatrix();

        Material m0(vec3(0.1, 0.1, 0.1), vec3(0, 0, 1), vec3(1, 1, .5), 100);

        shared_ptr<Mesh> m = make_shared<Mesh>(posBuf, norBuf, E);

        m->setMaterial(m0);

        scene.addObject(m);

        scene.drawImage(img);
    }
    else if (sceneNum == 8)
    {
        camera->setPosition(vec3(-3, 0, 0));
        camera->setFOVY(60);
        camera->setTarget(vec3(0, 0, 0));

        Scene scene;
        scene.setCamera(camera);
        scene.addLight(Light(vec3(-2, 1, 1), 1));

        Material m0(vec3(0.1, 0.1, 0.1), vec3(1, 0, 0), vec3(1, 1, .5), 100);
        Material m1(vec3(0.1, 0.1, 0.1), vec3(0, 1, 0), vec3(1, 1, .5), 100);
        Material m2(vec3(0.1, 0.1, 0.1), vec3(0, 0, 1), vec3(1, 1, .5), 100);

        shared_ptr<Sphere> s0 = make_shared<Sphere>(vec3(-.5, -1, 1), 0, vec3(1, 1, 1), vec3(1, 1, 1));
        shared_ptr<Sphere> s1 = make_shared<Sphere>(vec3(.5, -1, -1), 0, vec3(1, 1, 1), vec3(1, 1, 1));
        shared_ptr<Sphere> s2 = make_shared<Sphere>(vec3(0, 1, 0), 0, vec3(1, 1, 1), vec3(1, 1, 1));

        s0->setMaterial(m0);
        s1->setMaterial(m1);
        s2->setMaterial(m2);

        scene.addObject(s0);
        scene.addObject(s1);
        scene.addObject(s2);

        scene.drawImage(img);
    }

    img->writeToFile(outFile);
    return 0;
}
