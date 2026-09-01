#include "graphics/AssetManagers/MeshManager.h"
#include <stb_image.h>

namespace MeshManager{

    std::unordered_map<std::string, Mesh> meshes;

    Mesh* loadOBJ(const char* filename) {
        FILE* file = fopen(filename, "r");
        if (!file) {
            printf("Failed to open %s\n", filename);
            return nullptr;
        }

        //check if already loaded
        auto it = meshes.find(filename);

        if (it != meshes.end()){
            printf("%s already loaded, returning reference\n", filename);
            fflush(stdout);
            it->second.refCount++;
            return &it->second;
        }

        //allocate mesh data buffers
        const int MAX_TEMP = 2000;
        auto temp_v  = (float(*)[3])malloc(sizeof(float) * 3 * MAX_TEMP);
        auto temp_vt = (float(*)[2])malloc(sizeof(float) * 2 * MAX_TEMP);
        auto temp_vn = (float(*)[3])malloc(sizeof(float) * 3 * MAX_TEMP);

        int v_count = 0, vt_count = 0, vn_count = 0, f_count = 0;
        

        //TODO: While 30,000 vertice limit is way more than enough, I would prefer to switch to a smaller buffer that resizes
        //      which would save both a bit of mem and get rid of the limit
        Vertex3D* out_vertices = (Vertex3D*)memalign(16, sizeof(Vertex3D) * 30000);
        memset(out_vertices, 0, sizeof(Vertex3D) * 30000);

        char line[256];
        while (fgets(line, sizeof(line), file)) {
            //vertex positions
            if (line[0] == 'v' && line[1] == ' ') {
                if (v_count < MAX_TEMP) {
                    sscanf(line, "v %f %f %f", &temp_v[v_count][0], &temp_v[v_count][1], &temp_v[v_count][2]);
                    v_count++;
                }
            }
            //texture uvs
            else if (line[0] == 'v' && line[1] == 't') {
                if (vt_count < MAX_TEMP) {
                    sscanf(line, "vt %f %f", &temp_vt[vt_count][0], &temp_vt[vt_count][1]);
                    vt_count++;
                }
            }
            //vertex normals
            else if (line[0] == 'v' && line[1] == 'n') {
                if (vn_count < MAX_TEMP) {
                    sscanf(line, "vn %f %f %f", &temp_vn[vn_count][0], &temp_vn[vn_count][1], &temp_vn[vn_count][2]);
                    vn_count++;
                }
            }
            //faces
            else if (line[0] == 'f' && line[1] == ' ') {
                int v_idx[3] = {0}, vt_idx[3] = {0}, vn_idx[3] = {0};
            
                //try v/vt/vn format first
                int matches = sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                                    &v_idx[0], &vt_idx[0], &vn_idx[0],
                                    &v_idx[1], &vt_idx[1], &vn_idx[1],
                                    &v_idx[2], &vt_idx[2], &vn_idx[2]);

                //try v//vn format
                if (matches < 9) {
                    matches = sscanf(line, "f %d//%d %d//%d %d//%d",
                                    &v_idx[0], &vn_idx[0],
                                    &v_idx[1], &vn_idx[1],
                                    &v_idx[2], &vn_idx[2]);
                }

                //try v/vt format
                if (matches < 6) {
                    matches = sscanf(line, "f %d/%d %d/%d %d/%d",
                                    &v_idx[0], &vt_idx[0],
                                    &v_idx[1], &vt_idx[1],
                                    &v_idx[2], &vt_idx[2]);
                }

                //fallback for raw v format
                if (matches < 6) {
                    sscanf(line, "f %d %d %d", &v_idx[0], &v_idx[1], &v_idx[2]);
                }

                for (int i = 0; i < 3; i++) {
                    // UVs
                    if (vt_idx[i] > 0 && vt_count > 0) {
                        out_vertices[f_count].u = temp_vt[vt_idx[i] - 1][0];
                        out_vertices[f_count].v = 1.0f - temp_vt[vt_idx[i] - 1][1];
                    } else {
                        out_vertices[f_count].u = 0.0f;
                        out_vertices[f_count].v = 0.0f;
                    }

                    //normals (explicitly assigned to prevent garbage memory)
                    if (vn_idx[i] > 0 && vn_count > 0) {
                        out_vertices[f_count].nx = temp_vn[vn_idx[i] - 1][0];
                        out_vertices[f_count].ny = temp_vn[vn_idx[i] - 1][1];
                        out_vertices[f_count].nz = temp_vn[vn_idx[i] - 1][2];
                    } else {
                        out_vertices[f_count].nx = 0.0f;
                        out_vertices[f_count].ny = 0.0f;
                        out_vertices[f_count].nz = 1.0f;
                    }

                    //position
                    if (v_idx[i] > 0) {
                        out_vertices[f_count].x = temp_v[v_idx[i] - 1][0];
                        out_vertices[f_count].y = temp_v[v_idx[i] - 1][1];
                        out_vertices[f_count].z = temp_v[v_idx[i] - 1][2];
                    }

                    f_count++;
                }
            }
        }

        fclose(file);
        free(temp_v);
        free(temp_vt);
        free(temp_vn);

        sceKernelDcacheWritebackInvalidateRange(out_vertices, sizeof(Vertex3D) * f_count);

        Mesh mesh;
        mesh.vertices = out_vertices;
        mesh.vertexCount = f_count;
        mesh.refCount = 1;

        auto result = meshes.emplace(filename, mesh);

        printf("Successfully loaded %s (%d vertices)\n", filename, f_count);

        return &result.first->second;
    }

    void unload(const char* filename){
            auto it = meshes.find(filename);

        if (it == meshes.end()){
            printf("DID NOT UNLOAD. COULD NOT FIND %s\n", filename);
            fflush(stdout);
            return;
        } 

        it->second.refCount--;
        if (it->second.refCount <= 0) {
            // stbi_image_free(it->second.data);
            delete it->second.vertices;
            meshes.erase(it);

            printf("UNLOADED %s\n", filename);
            fflush(stdout);
        } else {
            printf("DID NOT UNLOAD: %s, REFCOUNT: %d\n", filename, it->second.refCount);
            fflush(stdout);
        }

    }  

}