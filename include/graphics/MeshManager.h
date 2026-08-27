#pragma once

#include "graphics/Mesh.h"
#include <unordered_map>
#include <psputils.h>
#include <cstring>

namespace MeshManager{

    std::unordered_map<std::string, Mesh> meshes;
    Mesh* loadOBJ(const char* filename) {
        FILE* file = fopen(filename, "r");
        if (!file) {
            printf("Failed to open %s\n", filename);
            return nullptr;
        }

        // Allocate larger temporary buffers (10,000 limit)
        const int MAX_TEMP = 10000;
        auto temp_v  = (float(*)[3])malloc(sizeof(float) * 3 * MAX_TEMP);
        auto temp_vt = (float(*)[2])malloc(sizeof(float) * 2 * MAX_TEMP);
        auto temp_vn = (float(*)[3])malloc(sizeof(float) * 3 * MAX_TEMP);

        int v_count = 0, vt_count = 0, vn_count = 0, f_count = 0;
        
        // Max 30,000 output vertices (10,000 triangles)
        Vertex3D* out_vertices = (Vertex3D*)memalign(16, sizeof(Vertex3D) * 30000);
        memset(out_vertices, 0, sizeof(Vertex3D) * 30000);

        char line[256];
        while (fgets(line, sizeof(line), file)) {
            // 1. Vertex Positions
            if (line[0] == 'v' && line[1] == ' ') {
                if (v_count < MAX_TEMP) {
                    sscanf(line, "v %f %f %f", &temp_v[v_count][0], &temp_v[v_count][1], &temp_v[v_count][2]);
                    v_count++;
                }
            }
            // 2. Texture Coordinates
            else if (line[0] == 'v' && line[1] == 't') {
                if (vt_count < MAX_TEMP) {
                    sscanf(line, "vt %f %f", &temp_vt[vt_count][0], &temp_vt[vt_count][1]);
                    vt_count++;
                }
            }
            // 3. Vertex Normals
            else if (line[0] == 'v' && line[1] == 'n') {
                if (vn_count < MAX_TEMP) {
                    sscanf(line, "vn %f %f %f", &temp_vn[vn_count][0], &temp_vn[vn_count][1], &temp_vn[vn_count][2]);
                    vn_count++;
                }
            }
            // 4. Faces
            else if (line[0] == 'f' && line[1] == ' ') {
                int v_idx[3] = {0}, vt_idx[3] = {0}, vn_idx[3] = {0};
                
                // Try v/vt/vn format first
                int matches = sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                                    &v_idx[0], &vt_idx[0], &vn_idx[0],
                                    &v_idx[1], &vt_idx[1], &vn_idx[1],
                                    &v_idx[2], &vt_idx[2], &vn_idx[2]);

                // Try v//vn format
                if (matches < 9) {
                    matches = sscanf(line, "f %d//%d %d//%d %d//%d",
                                    &v_idx[0], &vn_idx[0],
                                    &v_idx[1], &vn_idx[1],
                                    &v_idx[2], &vn_idx[2]);
                }

                // Try v/vt format
                if (matches < 6) {
                    matches = sscanf(line, "f %d/%d %d/%d %d/%d",
                                    &v_idx[0], &vt_idx[0],
                                    &v_idx[1], &vt_idx[1],
                                    &v_idx[2], &vt_idx[2]);
                }

                // Fallback for raw v format
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

                    // Normals (Explicitly assigned to prevent garbage memory)
                    if (vn_idx[i] > 0 && vn_count > 0) {
                        out_vertices[f_count].nx = temp_vn[vn_idx[i] - 1][0];
                        out_vertices[f_count].ny = temp_vn[vn_idx[i] - 1][1];
                        out_vertices[f_count].nz = temp_vn[vn_idx[i] - 1][2];
                    } else {
                        out_vertices[f_count].nx = 0.0f;
                        out_vertices[f_count].ny = 0.0f;
                        out_vertices[f_count].nz = 1.0f;
                    }

                    // Position
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

        // Flush cache range for GPU DMA
        sceKernelDcacheWritebackInvalidateRange(out_vertices, sizeof(Vertex3D) * f_count);

        Mesh* mesh = (Mesh*)malloc(sizeof(Mesh));
        mesh->vertices = out_vertices;
        mesh->vertexCount = f_count;

        printf("Successfully loaded %s (%d vertices)\n", filename, f_count);

        return mesh;
}

    // Mesh* loadOBJ(const char* filename) {
    //     FILE* file = fopen(filename, "r");
    //     if (!file) {
    //         printf("Failed to open %s\n", filename);
    //         return nullptr;
    //     }

    //     // Allocate temporary buffers on the HEAP to prevent PSP stack overflow
    //     auto temp_v = (float(*)[3])malloc(sizeof(float) * 3 * 2000);
    //     auto temp_vt = (float(*)[2])malloc(sizeof(float) * 2 * 2000);

    //     int v_count = 0, vt_count = 0, f_count = 0;
    //     Vertex3D* out_vertices = (Vertex3D*)memalign(16, sizeof(Vertex3D) * 6000);

    //     char line[256];
    //     while (fgets(line, sizeof(line), file)) {
    //         // 1. Vertex Positions
    //         if (line[0] == 'v' && line[1] == ' ') {
    //             sscanf(line, "v %f %f %f", &temp_v[v_count][0], &temp_v[v_count][1], &temp_v[v_count][2]);
    //             v_count++;
    //         }
    //         // 2. Texture Coordinates
    //         else if (line[0] == 'v' && line[1] == 't') {
    //             sscanf(line, "vt %f %f", &temp_vt[vt_count][0], &temp_vt[vt_count][1]);
    //             vt_count++;
    //         }
    //         // 3. Faces (handles both v/vt/vn AND v/vt)
    //         else if (line[0] == 'f' && line[1] == ' ') {
    //             int v_idx[3], vt_idx[3], vn_idx[3];
    //             int matches = sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
    //                                 &v_idx[0], &vt_idx[0], &vn_idx[0],
    //                                 &v_idx[1], &vt_idx[1], &vn_idx[1],
    //                                 &v_idx[2], &vt_idx[2], &vn_idx[2]);

    //             // Fallback for v/vt format if v/vt/vn failed
    //             if (matches < 9) {
    //                 sscanf(line, "f %d/%d %d/%d %d/%d",
    //                     &v_idx[0], &vt_idx[0],
    //                     &v_idx[1], &vt_idx[1],
    //                     &v_idx[2], &vt_idx[2]);
    //             }

    //             for (int i = 0; i < 3; i++) {
    //                 out_vertices[f_count].u = temp_vt[vt_idx[i] - 1][0];
    //                 out_vertices[f_count].v = 1.0f - temp_vt[vt_idx[i] - 1][1]; // Invert V for PSP coordinate space
    //                 out_vertices[f_count].x = temp_v[v_idx[i] - 1][0];
    //                 out_vertices[f_count].y = temp_v[v_idx[i] - 1][1];
    //                 out_vertices[f_count].z = temp_v[v_idx[i] - 1][2];
    //                 f_count++;
    //             }
    //         }
    //     }

    //     fclose(file);
    //     free(temp_v);
    //     free(temp_vt);

    //     // Write back and invalidate cache for GPU DMA read
    //     sceKernelDcacheWritebackInvalidateRange(out_vertices, sizeof(Vertex3D) * f_count);

    //     Mesh* mesh = (Mesh*)malloc(sizeof(Mesh));
    //     mesh->vertices = out_vertices;
    //     mesh->vertexCount = f_count;

    //     printf("Successfully loaded %s (%d vertices)\n", filename, f_count);
    //     return mesh;
    // }
}