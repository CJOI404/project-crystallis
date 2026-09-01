#include "graphics/AssetManagers/UIRender.h"
#include <stdio.h>
#include <string.h>
#include <pspgu.h>
#include <psputils.h>
#include <stb_image.h>
#include <vector>
#include <fstream>
#include <GlobalDefs.h>
#include "graphics/Pipeline/RenderQueue.h"

namespace UI {
    FontChar fontData[128];
    char textBuffer[64];

    Texture* fontTexture;

    int textWidth = 256;
    int textHeight = 128;

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

        fontTexture = TextureManager::load(texturePath, textWidth, textHeight);

        sceKernelDcacheWritebackAll();
    }

    void drawHealthBar(float x, float y, float w, float h, int health, int maxHealth){
        SubmitRect(&g_queue, x, y, w, h, Colours::LIGHTGREY, GraphicsUtils::Layer::UI_3);
        SubmitRect(&g_queue, x, y + (h - ((h*2)/3))/2, ((float) health / maxHealth) * w, ((h*2)/3), Colours::LIGHTGREEN, GraphicsUtils::Layer::UI_3);
    }

    void drawButton(float x, float y, float w, float h, const char* text, uint32_t color){
        SubmitRect(&g_queue, x, y, w, h, color, GraphicsUtils::Layer::UI_3);
        SubmitText(&g_queue, text, x + 5, y + 2, 0.35, 0.35, 0xFFFFFFFF, GraphicsUtils::Layer::UI_3);
    }
}