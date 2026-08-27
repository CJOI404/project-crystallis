#include "graphics/UIRender.h"
#include <stdio.h>
#include <string.h>
#include <pspgu.h>
#include <psputils.h>
#include <stb_image.h>
#include <vector>
#include <fstream>
#include <GlobalDefs.h>

namespace UI {
    FontChar fontData[128];
    char textBuffer[64];

    Texture* fontTexture;

    int textWidth = 256;

    int extract(const std::string& line, const std::string& key) {
        size_t pos = line.find(key + "=");
        if (pos == std::string::npos) return 0;
        return std::stoi(line.substr(pos + key.length() + 1));
    }

    void loadFont(const char* fntPath, const char* texturePath) {

        //parse file for uv data
        FontChar currChar;

        std::ifstream inFS;
        std::string line;

        inFS.open(fntPath);

        //skip beginning metadata
        for (int i = 0; i < 4; i++){
            std::getline(inFS, line);
        }

        while (std::getline(inFS, line)) {
            currChar.id = extract(line, "id");
            currChar.x  = extract(line, "x");
            currChar.y  = extract(line, "y");
            currChar.width  = extract(line, "width");
            currChar.height  = extract(line, "height");
            currChar.xoffset = extract(line, "xoffset");
            currChar.yoffset = extract(line, "yoffset");
            currChar.xadvance = extract(line, "xadvance");
            fontData[currChar.id] = currChar;
        }

        inFS.close(); 

        fontTexture = TextureManager::load(texturePath, textWidth);

        // fontTexture.data = (uint32_t *) stbi_load(texturePath, &(textWidth), &(textWidth), NULL, STBI_rgb_alpha);

        sceKernelDcacheWritebackAll();
    }


    void drawString(int x, int y, uint32_t color, float xScale, float yScale, std::string text) {

        int currentX = x;

        sceGuTexMode(GU_PSM_8888, 0, 0, GU_TRUE);
        sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);
        sceGuTexImage(0, 256, 256, 256, fontTexture->data);

        //enable transparency
        sceGuEnable(GU_BLEND);
        sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);

        sceGuEnable(GU_TEXTURE_2D); 

        for (int i = 0; text[i] != '\0'; i++){

            TextureVertex* vertices = (TextureVertex*)sceGuGetMemory(2 * sizeof(TextureVertex));

            int j = text[i];

            // Top-Left
            vertices[0].u = fontData[j].x; 
            vertices[0].v = fontData[j].y;
            vertices[0].x = currentX + (fontData[j].xoffset * xScale); 
            vertices[0].y = y + (fontData[j].yoffset * yScale); 
            vertices[0].z = 0;

            // Bottom-Right
            vertices[1].u = fontData[j].x + fontData[j].width; 
            vertices[1].v = fontData[j].y + fontData[j].height;
            vertices[1].x = currentX + ((fontData[j].width + fontData[j].xoffset) * xScale); 
            vertices[1].y = y + ((fontData[j].height + fontData[j].yoffset) * yScale); 
            vertices[1].z = 0;

            sceGuDrawArray(GU_SPRITES, GU_COLOR_8888 | GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_2D, 2, 0, vertices);

            currentX += fontData[j].xadvance * xScale;
        }
        sceKernelDcacheWritebackAll();

        currentX = 0;
        sceGuDisable(GU_TEXTURE_2D);
        
    }

    void drawRect(float x, float y, float w, float h, unsigned int colour) {

        Vertex* vertices = (Vertex*)sceGuGetMemory(2 * sizeof(Vertex));

        vertices[0].x = x;
        vertices[0].y = y;

        vertices[1].x = x + w;
        vertices[1].y = y + h;

        sceGuColor(colour);

        sceGuDrawArray(GU_SPRITES, GU_TEXTURE_16BIT | GU_VERTEX_16BIT | GU_TRANSFORM_2D, 2, 0, vertices);
    }

    void drawTri(float x1, float y1, float x2, float y2, float x3, float y3, unsigned int color){
        Vertex* vertices = (Vertex*)sceGuGetMemory(3 * sizeof(Vertex));

        // Set vertex data
        vertices[0].x = x1; vertices[0].y = y1; vertices[0].z = 0.0f;
        vertices[1].x = x2; vertices[1].y = y2; vertices[1].z = 0.0f;
        vertices[2].x = x3; vertices[2].y = y3; vertices[2].z = 0.0f;

        sceGuColor(color);

        sceGuDrawArray(GU_TRIANGLES, GU_TEXTURE_16BIT | GU_VERTEX_16BIT | GU_TRANSFORM_2D, 3, 0, vertices);
    }

    void drawHealthBar(float x, float y, float w, float h, int health, int maxHealth){
        UI::drawRect(x, y, w, h, Colours::LIGHTGREY);
        UI::drawRect(x, y + (h - ((h*2)/3))/2, ((float) health / maxHealth) * w, ((h*2)/3), Colours::LIGHTGREEN);
    }

    void drawButton(float x, float y, float w, float h, std::string text, uint32_t color){
        UI::drawRect(x, y, w, h, color);
        UI::drawString(x + 5, y + 2, 0xFFFFFFFF, 0.35, 0.35, text);
    }
}