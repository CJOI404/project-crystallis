#pragma once

#include "graphics/Assets/Mesh.h"
#include <unordered_map>
#include <psputils.h>
#include <cstring>

namespace MeshManager{

    extern std::unordered_map<std::string, Mesh> meshes;
    Mesh* loadOBJ(const char* filename);

    void unload(const char* filename);

}