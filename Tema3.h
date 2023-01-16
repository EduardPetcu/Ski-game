#pragma once

#include <string>
#include <unordered_map>

#include "components/simple_scene.h"
#include "components/transform.h"
#include "lab_m1/Tema3/lab_camera.h"
#define NUMBER_OF_OBJECTS 40
#define NR_TYPE_OF_OBJECTS 4

namespace m1
{
    struct object_info {
        float obj_sizes;
        int materialShininess, materialKd, materialKs;
        std::string name_of_obj;
    };

    class Tema3 : public gfxc::SimpleScene
    {
     public:
        Tema3();
        ~Tema3();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, Texture2D *texture1, bool moveTexture, int obj_type);
        Texture2D *CreateRandomTexture(unsigned int width, unsigned int height);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void rotateOY(float angle);
        void InitArrays();
        void TransForward(float distance, glm::vec3& forwardObj, glm::vec3& position);
        void GenerateObstacles(float deltaTimeSeconds);
        bool checkCollision(glm::vec3 carPos, glm::vec3 other, int obj_type);
        void Init_size_obj_types();
        void ResetGame();
        void Init_PillarLight(glm::vec3 position, int pos);
        void Init_GiftLight(glm::vec3 position, int i);
        int selectRandomColor();
        void Jump(float deltaTimeSeconds);
        void Fall(float deltaTimeSeconds);
        std::unordered_map<std::string, Texture2D *> mapTextures;
        
    protected:
        glm::vec3 forwardObj = glm::vec3(0, 0, -1);
        glm::vec3 upObj = glm::vec3(0, 1, 0);
        glm::vec3 rightObj = glm::vec3(1, 0, 0);

        glm::vec3 posChar;
        bool mix = false, turn = false;
        float textCord = 0.0f, XCoord = 0.0f;
        glm::vec3 posPlane = glm::vec3(0, 0.1, -3);
        glm::vec3 posBox = posPlane + glm::vec3(0, 0.6, -0.2);
        implemented::CameraTema3* camera;
        glm::mat4 projectionMatrix;
        float angleStep = 0.0f;
        float speedX = 15.0f, speedYZ = 15.5f;
        float initialSpeedX = 15.0f, initialSpeedYZ = 15.5f;
        float timeSpent = 15.0f;
        float my_rand_X[NUMBER_OF_OBJECTS], my_rand_Y[NUMBER_OF_OBJECTS], my_rand_Z[NUMBER_OF_OBJECTS];
        int type_of_obj[NUMBER_OF_OBJECTS];
        float delay;
        //std::string name_of_obj[NUMBER_OF_OBJECTS];
        int score = 0, count = 0, scoreAdder = 50, nrOfLights;
        glm::vec3 lightPosition[2 * NUMBER_OF_OBJECTS], lightDirection[2 * NUMBER_OF_OBJECTS];
        int nr_of_lights = 0;
        object_info obj_info[NR_TYPE_OF_OBJECTS + 3];
        int isSpot[2 * NUMBER_OF_OBJECTS];
        int isGift[NUMBER_OF_OBJECTS];
        int highScore = 0;
        glm::ivec3 light_colors[2 * NUMBER_OF_OBJECTS];
        bool gamePaused = false;
        float jumpHeight = 5.0f, rotateAngleOX = 0;
        bool isJumping = false;
        bool isFalling = false, isFlipping = false;
    };
}   // namespace m1
