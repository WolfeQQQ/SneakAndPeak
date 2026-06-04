#pragma once
#include "screen.h"
#include "raylib.h"
#include "tilemap.h"
#include "../shared/contstants.h"

/**
 * @class TutorialScreen
 * @brief Shows the tutorial screen that explains all the machanics and controls
 */
class TutorialScreen : public Screen {
    private:
        Tilemap bgMap;
        Shader visionShader;
        RenderTexture2D lightMask;
        Camera2D camera;

        int playerPosLoc;
        int resolutionLoc;
        int radiusLoc;
        int softnessLoc;

        Texture2D texW, texA, texS, texD;
        Texture2D texArrUp, texArrDown, texArrLeft, texArrRight;
        Texture2D texShift;

    public:
        TutorialScreen();
        ~TutorialScreen();
        AppState update() override;
        void draw() override;
};