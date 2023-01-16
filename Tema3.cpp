#include "lab_m1/Tema3/Tema3.h"

#include <vector>
#include <string>
#include <iostream>
#include "lab_m1/Tema3/transform3D.h"

using namespace std;
using namespace m1;


Tema3::Tema3()
{
}


Tema3::~Tema3()
{
}
void Tema3::Init_size_obj_types() {
    obj_info[0].obj_sizes = 0.75f;
    obj_info[1].obj_sizes = 1.25f;
    obj_info[2].obj_sizes = 0.5f;
    obj_info[3].obj_sizes = 0.5f;

    for (int i = 0; i < NR_TYPE_OF_OBJECTS + 3; i++) {
        obj_info[i].materialKd = 0.5f;
        obj_info[i].materialKs = 0.5f;
        obj_info[i].materialShininess = 30;
    }
    obj_info[0].name_of_obj = "un brad";
    obj_info[1].name_of_obj = "o piatra";
    obj_info[2].name_of_obj = "un stalp";
    obj_info[3].name_of_obj = "cadou";
}
void Tema3::Init_PillarLight(glm::vec3 position, int i) {
    lightPosition[i] = position + glm::vec3(-2.5f, 1.5f , 1.5f);
    lightPosition[NUMBER_OF_OBJECTS + i] = position + glm::vec3(2.5f, 1.5f, 1.5f);
    lightDirection[i] = glm::vec3(0, -1, 0);
    lightDirection[NUMBER_OF_OBJECTS + i] = glm::vec3(0, -1, 0);
    //lightDirection[nr_of_lights + 1] = glm::vec3(0, -1, 0);
    isSpot[i] = 1;
    isSpot[NUMBER_OF_OBJECTS + i] = 1;
    nr_of_lights += 2;
    light_colors[i] = glm::vec3(1, 1, 1);
    light_colors[i + NUMBER_OF_OBJECTS] = glm::vec3(1, 1, 1);
}
void Tema3::Init_GiftLight(glm::vec3 position, int i) {
    lightPosition[i] = position;
    nr_of_lights++;
}
void Tema3::InitArrays() {
    for (int i = 0; i < NUMBER_OF_OBJECTS; i++) {
        my_rand_X[i] = 1000.0f;
        my_rand_Y[i] = 1000.0f;
        my_rand_Z[i] = 1000.0f;
        lightPosition[i] = glm::vec3(100, 0, 0);
        lightDirection[i] = glm::vec3(0, -1, 0);
    }
    for (int i = 0; i < 2 * NUMBER_OF_OBJECTS; i++) {
        light_colors[i] = glm::vec3(0.0f, 0.0f, 0.0f);
        isSpot[i] = 0;
    }
}
void Tema3::ResetGame() {
    posPlane = glm::vec3(0, 0.1, -3);
    posChar = posPlane + glm::vec3(0, -25.5f, -0.2f);
    camera->position = glm::vec3(posPlane + glm::vec3(0, -20.0f, -40.0f));
    speedX = initialSpeedX;
    speedYZ = initialSpeedYZ;
    count = 0;
    scoreAdder = 50;
    score = 0;
    isJumping = false;
    isFalling = false;
    jumpHeight = 5.0f;
    rotateAngleOX = 0;
    InitArrays();
    delay = 3.0f;
}
void Tema3::Init()
{
    const string sourceTextureDir = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "textures");
    camera = new implemented::CameraTema3();
    camera->Set(posPlane + glm::vec3(0, -20.0f, -40.0f), glm::vec3(posPlane.x, posPlane.y - 25.0f, posPlane.z), glm::vec3(0, 1, 0));
    srand(time(0));
    // Load textures

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "crate.jpg").c_str(), GL_REPEAT);
        mapTextures["crate"] = texture;
    }
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "gift.png").c_str(), GL_REPEAT);
        mapTextures["gift"] = texture;
    }
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "snow.png").c_str(), GL_REPEAT);
        mapTextures["snow"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "ski.png").c_str(), GL_REPEAT);
        mapTextures["ski"] = texture;
    }
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "XmasTree.png").c_str(), GL_REPEAT);
        mapTextures["XmasTree"] = texture;
    }
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "wood.png").c_str(), GL_REPEAT);
        mapTextures["wood"] = texture;
    }
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "stone.png").c_str(), GL_REPEAT);
        mapTextures["stone"] = texture;
    }
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "pillar.png").c_str(), GL_REPEAT);
        mapTextures["pillar"] = texture;
    }
    // Load meshes
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("cone");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "cone.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }   
    {
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader *shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
    InitArrays();
    Init_size_obj_types();
    projectionMatrix = glm::perspective(RADIANS(30), window->props.aspectRatio, 0.01f, 200.0f);
    posChar = posPlane + glm::vec3(0, -25.5f, -0.2f);
}

void Tema3::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}
bool Tema3::checkCollision(glm::vec3 skiPos, glm::vec3 other, int obj_type) {
    float distance = sqrt(
        (skiPos.x - other.x) * (skiPos.x - other.x) +
        (skiPos.y - other.y) * (skiPos.y - other.y) +
        (skiPos.z - other.z) * (skiPos.z - other.z)
    );
    return distance < (0.4f + obj_info[obj_type].obj_sizes);
}
int Tema3::selectRandomColor() {
    int rand_color = rand() % 6;
    if (rand_color == 0)
        return 2;
    if (rand_color == 1)
        return 3;
    if (rand_color == 2)
        return 5;
    if (rand_color == 3)
        return 6;
    if (rand_color == 4)
        return 10;
    if (rand_color == 5)
        return 15;
}
void Tema3::GenerateObstacles(float deltaTimeSeconds) {
       
    for (int i = 0; i < NUMBER_OF_OBJECTS; i++) {
        if (my_rand_Y[i] >= posPlane.y + 15 || my_rand_X[i] > posPlane.x + 45 || my_rand_X[i] < posPlane.x - 45) {
            if (type_of_obj[i] == 0) {
                nr_of_lights--;
                lightPosition[i] = glm::vec3(100, 0, 0);
            }
            if (type_of_obj[i] == 2) {
                nr_of_lights-= 2;
                lightPosition[i] = glm::vec3(100, 0, 0);
                lightPosition[i + NUMBER_OF_OBJECTS] = glm::vec3(100, 0, 0);
            }
            if (type_of_obj[i] == 3) {
                //isGift[i] = 0;
                nr_of_lights--;
                lightPosition[i] = glm::vec3(100, 0, 0);
            }
            type_of_obj[i] = rand() % NR_TYPE_OF_OBJECTS;
            my_rand_X[i] = posPlane.x + (71 * i / NUMBER_OF_OBJECTS - 35);
            my_rand_Y[i] = posPlane.y - (rand() % 30) - 10;
            my_rand_Z[i] = my_rand_Y[i] / tan(RADIANS(30));
            if (type_of_obj[i] == 0) {
                my_rand_Y[i] -= 21.5f;
                my_rand_Z[i] -= 0.75f;
                isSpot[i] = selectRandomColor();
            }
            if (type_of_obj[i] == 1) {
                my_rand_X[i] += 0.5f;
                my_rand_Y[i] -= 24;
            }
            if (type_of_obj[i] == 2) {
                my_rand_Y[i] -= 20;
                my_rand_Z[i] -= 0.5f;
            }
            if (type_of_obj[i] == 3) {
                my_rand_Y[i] -= 23.75f;
                isSpot[i] = selectRandomColor();
            }
        }
        if (type_of_obj[i] == 0) {
            Init_GiftLight(glm::vec3(my_rand_X[i], my_rand_Y[i] - 2, my_rand_Z[i] + 2.5f), i);
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix *= transform3D::Translate(my_rand_X[i], my_rand_Y[i] + 4, my_rand_Z[i] - 0.75f);
            modelMatrix *= transform3D::RotateOX(RADIANS(-30));
            modelMatrix *= transform3D::Scale(2.5f, 2.5f, 2.5f);
            RenderSimpleMesh(meshes["cone"], shaders["LabShader"], modelMatrix, mapTextures["XmasTree"], false, 0);

            modelMatrix = glm::mat4(1);
            modelMatrix *= transform3D::Translate(my_rand_X[i], my_rand_Y[i], my_rand_Z[i] + 0.75f);
            modelMatrix *= transform3D::RotateOX(RADIANS(-30));
            modelMatrix *= transform3D::Scale(0.75f, 4.5f, 0.75f);
            RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, mapTextures["wood"], false, 0);
        }
        if (type_of_obj[i] == 1) {
            // 45 = 50 * 1.8 / 2
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix *= transform3D::Translate(my_rand_X[i] - 0.5f, my_rand_Y[i], my_rand_Z[i]);
            modelMatrix *= transform3D::Scale(1.5f, 1.5f, 1.5f);
            RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix, mapTextures["stone"], false, 1);

            modelMatrix = glm::mat4(1);
            modelMatrix *= transform3D::Translate(my_rand_X[i] + 0.5f, my_rand_Y[i], my_rand_Z[i]);
            modelMatrix *= transform3D::Scale(2.5f, 1.5f, 1.5f);
            RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix, mapTextures["stone"], false, 1);
        }
        if (type_of_obj[i] == 2) {
            // 45 = 50 * 1.8 / 2
            Init_PillarLight(glm::vec3(my_rand_X[i], my_rand_Y[i], my_rand_Z[i] - 0.5f), i);
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix *= transform3D::Translate(my_rand_X[i], my_rand_Y[i], my_rand_Z[i] + 0.5f);
            modelMatrix *= transform3D::RotateOX(RADIANS(-30));
            modelMatrix *= transform3D::Scale(1.0f, 7.5f, 1.0f);
            RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, mapTextures["pillar"], false, 2);

            modelMatrix = glm::mat4(1);
            modelMatrix *= transform3D::Translate(my_rand_X[i], my_rand_Y[i] + 3.5, my_rand_Z[i] - 0.5f);
            modelMatrix *= transform3D::RotateOX(RADIANS(-30));
            modelMatrix *= transform3D::RotateOZ(RADIANS(90));
            modelMatrix *= transform3D::Scale(1.0f, 4.5f, 1.0f);
            RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, mapTextures["pillar"], false, 2);
        }
        if (type_of_obj[i] == 3) {
            Init_GiftLight(glm::vec3(my_rand_X[i], my_rand_Y[i], my_rand_Z[i]), i);
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix *= transform3D::Translate(my_rand_X[i], my_rand_Y[i], my_rand_Z[i]);
            modelMatrix *= transform3D::RotateOX(RADIANS(-30));
            RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, mapTextures["gift"], false, 3);
        }
    }
}
void Tema3::Jump(float deltaTimeSeconds) {
    posChar.z -= min (10 * deltaTimeSeconds, jumpHeight);
    jumpHeight -= min (10 * deltaTimeSeconds, jumpHeight);
    if (jumpHeight <= 0) {
        isJumping = false;
        isFalling = true;
    }
}
void Tema3::Fall(float deltaTimeSeconds) {
    posChar.z += min (10 * deltaTimeSeconds, 5.0f - jumpHeight);
    jumpHeight += min (10 * deltaTimeSeconds, 5.0f - jumpHeight);
    if (jumpHeight >= 5.0f) {
        isJumping = false;
        isFalling = false;
        jumpHeight = 5.0f;
    }
}

void Tema3::Update(float deltaTimeSeconds)
{
    if (isJumping) {
        Jump(deltaTimeSeconds);
    }
    if (isFalling) {
        Fall(deltaTimeSeconds);
    }
    if (isFlipping) {
        if (rotateAngleOX < 2 * M_PI)
            rotateAngleOX += min(20 * deltaTimeSeconds, 2.0f * (float)M_PI - rotateAngleOX);
        else {
            isFlipping = false;
            rotateAngleOX = 0;
        }
    }
    GenerateObstacles(deltaTimeSeconds);
    // BOX
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, posChar);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(-30) + rotateAngleOX, glm::vec3(1, 0, 0));

        modelMatrix = glm::rotate(modelMatrix, angleStep, glm::vec3(0, 1, 0));

        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, mapTextures["crate"], false, 4);
    }

    // SKI 1
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, posChar + glm::vec3(0.2f, -0.3f, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(-30) + rotateAngleOX, glm::vec3(1, 0, 0));

        modelMatrix = glm::translate(modelMatrix, cos(angleStep) * glm::vec3(-0.25f, 0, 0));
        modelMatrix = glm::rotate(modelMatrix, angleStep, glm::vec3(0, 1, 0));
        modelMatrix = glm::translate(modelMatrix, -cos(angleStep) * glm::vec3(-0.25f, 0, 0));

        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f, 0.01f, 1.5f));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, mapTextures["ski"], false, 5);
    }

    // SKI 2
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, posChar + glm::vec3(-0.2f, -0.3f, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(-30) + rotateAngleOX, glm::vec3(1, 0, 0));

        modelMatrix = glm::translate(modelMatrix, -cos(angleStep) * glm::vec3(-0.25f, 0, 0));
        modelMatrix = glm::rotate(modelMatrix, angleStep, glm::vec3(0, 1, 0));
        modelMatrix = glm::translate(modelMatrix, cos(angleStep) * glm::vec3(-0.25f, 0, 0));

        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f, 0.01f, 1.5f));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, mapTextures["ski"], false, 5);
    }

    // Snow plane
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, posPlane);
        //modelMatrix = glm::translate(modelMatrix)
        modelMatrix = glm::translate(modelMatrix, glm::vec3(50 * 0.9f, 0, 50 * 0.9f));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(-30), glm::vec3(1, 0, 0));
        modelMatrix = glm::translate(modelMatrix, -glm::vec3(50 * 0.9f, 0, 50 * 0.9f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1.8f, 0, 1.8f));
        RenderSimpleMesh(meshes["plane"], shaders["LabShader"], modelMatrix, mapTextures["snow"], true, 6);
    }

    if (delay <= 0 && !gamePaused) {
        camera->position.x -= speedX * deltaTimeSeconds * sin(angleStep);
        camera->position.y -= speedYZ * deltaTimeSeconds * sin(RADIANS(30.0f));
        camera->position.z -= speedYZ * deltaTimeSeconds * cos(RADIANS(30.0f));
        XCoord -= speedX / 1.81f * deltaTimeSeconds * sin(angleStep);
        textCord += speedYZ * deltaTimeSeconds * 0.55f;
        posPlane.x -= speedX * deltaTimeSeconds * sin(angleStep);
        posPlane.y += speedYZ * deltaTimeSeconds * sin(RADIANS(-30.0f));
        posPlane.z -= speedYZ * deltaTimeSeconds * cos(RADIANS(30.0f));
        posChar.x -= speedX * deltaTimeSeconds * sin(angleStep);
        posChar.y += speedYZ * deltaTimeSeconds * sin(RADIANS(-30.0f));
        posChar.z -= speedYZ * deltaTimeSeconds * cos(RADIANS(30.0f));
    }
    else {
        delay -= deltaTimeSeconds;
    }
    for (int i = 0; i < NUMBER_OF_OBJECTS; ++i) {
        if (checkCollision(posChar, glm::vec3(my_rand_X[i], my_rand_Y[i] - 2, my_rand_Z[i]), type_of_obj[i]) && type_of_obj[i] == 0 ||
            checkCollision(posChar, glm::vec3(my_rand_X[i], my_rand_Y[i], my_rand_Z[i]), type_of_obj[i]) && type_of_obj[i] == 0 ||
            checkCollision(posChar, glm::vec3(my_rand_X[i], my_rand_Y[i] - 0, my_rand_Z[i]), type_of_obj[i]) && type_of_obj[i] == 1 ||
            checkCollision(posChar, glm::vec3(my_rand_X[i], my_rand_Y[i] - 4, my_rand_Z[i]), type_of_obj[i]) && type_of_obj[i] == 2 ||
            checkCollision(posChar, glm::vec3(my_rand_X[i], my_rand_Y[i] - 2, my_rand_Z[i]), type_of_obj[i]) && type_of_obj[i] == 2) {
            delay = 1.0f;
            my_rand_Y[i] = 1000;
            cout << "Ai lovit " << obj_info[type_of_obj[i]].name_of_obj << "\n\n";
            speedX = initialSpeedX;
            speedYZ = initialSpeedYZ;
            count = 0;
            if (score > highScore)
                highScore = score;
            score = 0;
            scoreAdder = 50;
        }
        if (checkCollision(posChar, glm::vec3(my_rand_X[i], my_rand_Y[i], my_rand_Z[i]), type_of_obj[i]) && type_of_obj[i] == 3) {
            score += scoreAdder;
            count++;
            my_rand_Y[i] = 1000;
            if (count >= 5) {
                count = 0;
                scoreAdder += 10;
                speedX += 1.0f;
                speedYZ += 1.0f;
            }
        }
    }
}


void Tema3::FrameEnd()
{
    //DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Tema3::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, Texture2D* texture1, bool moveTexture, int obj_type)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    glUseProgram(shader->program);

    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));

    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    GLint varTextCord = glGetUniformLocation(shader->program, "Ycoord");
    glUniform1f(varTextCord, textCord);

    GLint varXCoord = glGetUniformLocation(shader->program, "Xcoord");
    glUniform1f(varXCoord, XCoord);

    GLboolean varturn = glGetUniformLocation(shader->program, "turn");
    glUniform1f(varturn, moveTexture);

    int light_direction = glGetUniformLocation(shader->program, "light_direction");
    glUniform3fv(light_direction, 2 * NUMBER_OF_OBJECTS, glm::value_ptr(lightDirection[0]));

    int light_position = glGetUniformLocation(shader->program, "light_position");
    glUniform3fv(light_position, 2 * NUMBER_OF_OBJECTS, glm::value_ptr(lightPosition[0]));

    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
    int eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

    int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(material_shininess, 30);

    int material_kd = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(material_kd, 0.5f);

    int material_ks = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(material_ks, 0.5f);

    int lightColor = glGetUniformLocation(shader->program, "light_color");
    glUniform3iv(lightColor, 2 * NUMBER_OF_OBJECTS, glm::value_ptr(light_colors[0]));

    int gift_check = glGetUniformLocation(shader->program, "isGift");
    glUniform1iv(gift_check, NUMBER_OF_OBJECTS, isGift);

    int spot_array = glGetUniformLocation(shader->program, "isSpot");
    glUniform1iv(spot_array, 2 * NUMBER_OF_OBJECTS, isSpot);

    float angle = glGetUniformLocation(shader->program, "angleSpot");
    glUniform1f(angle, 0);

    int nr_lights = glGetUniformLocation(shader->program, "nrOfLights");
    glUniform1i(nr_lights, 2 * NUMBER_OF_OBJECTS);

    if (texture1)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    }

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


Texture2D* Tema3::CreateRandomTexture(unsigned int width, unsigned int height)
{
    GLuint textureID = 0;
    unsigned int channels = 3;
    unsigned int size = width * height * channels;
    unsigned char* data = new unsigned char[size];

    // TODO(student): Generate random texture data
    for (int i = 0; i < size; ++i) {
        data[i] = rand() % 255;
    }
    glGenTextures(1, &textureID);
    // TODO(student): Generate and bind the new texture ID
    glBindTexture(GL_TEXTURE_2D, textureID);

    if (GLEW_EXT_texture_filter_anisotropic) {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    CheckOpenGLError();

    // Use glTexImage2D to set the texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    // TODO(student): Generate texture mip-maps
    glGenerateMipmap(GL_TEXTURE_2D);
    CheckOpenGLError();

    // Save the texture into a wrapper Texture2D class for using easier later during rendering phase
    Texture2D* texture = new Texture2D();
    texture->Init(textureID, width, height, channels);

    SAFE_FREE_ARRAY(data);
    return texture;
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema3::OnInputUpdate(float deltaTime, int mods)
{
    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT) && mix)
    {
        //glm::vec3 up = glm::vec3(0, 1, 0);
        //glm::vec3 right = GetSceneCamera()->m_transform->GetLocalOXVector();
        //glm::vec3 forward = GetSceneCamera()->m_transform->GetLocalOZVector();
        //forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));

        if (window->KeyHold(GLFW_KEY_W)) camera->position.z += deltaTime * 25;
        if (window->KeyHold(GLFW_KEY_A)) camera->position.x += deltaTime * 25;
        if (window->KeyHold(GLFW_KEY_S)) camera->position.z -= deltaTime * 25;
        if (window->KeyHold(GLFW_KEY_D)) camera->position.x -= deltaTime * 25;
        if (window->KeyHold(GLFW_KEY_E)) camera->position.y -= deltaTime * 25;
        if (window->KeyHold(GLFW_KEY_Q)) camera->position.y += deltaTime * 25;
    }

}

void Tema3::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_SPACE)
    {
        if (mix == false)
            mix = true;
        else
            mix = false;
    }
    if (key == GLFW_KEY_TAB) {
        cout << "=========================================\n";
        cout << "Scorul tau este: " << score << "\n";
        cout << "Highscore-ul tau este: " << highScore <<"\n";
        cout << "=======================================\n\n";
    }
    if (key == GLFW_KEY_R) {
        cout << "Jocul a fost resetat si\n";
        cout << "va incepe in 3 secunde.\n\n";
        ResetGame();
    }
    if (key == GLFW_KEY_P) {
        if (gamePaused) {
            gamePaused = false;
            cout << "Game unpaused!\n\n";
        }
        else {
            gamePaused = true;
            cout << "Game paused!\n\n";
        }
    }
    if (key == GLFW_KEY_UP && delay <= 0 && !gamePaused) {
        isJumping = true;
    }
    if (key == GLFW_KEY_W && isJumping) {
        isFlipping = true;
    }
}


void Tema3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    glm::ivec2 resolution = window->GetResolution();
    float distLimit = resolution.x / 2;
    angleStep = (mouseX - resolution.x / 2) / distLimit * RADIANS(45);
    //cout << resolution.x << " " << resolution.y << "\n";
    //cout << angleStep << "\n";
    //cout << forwardObj << "\n";
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (window->GetSpecialKeyState() == 0) {
            camera->RotateFirstPerson_OY(-sensivityOY * deltaX);
            camera->RotateFirstPerson_OX(-sensivityOX * deltaY);
        }

        if (window->GetSpecialKeyState() & GLFW_MOD_CONTROL) {
            camera->RotateThirdPerson_OX(-sensivityOX * deltaY);
            camera->RotateThirdPerson_OY(-sensivityOY * deltaX);
        }
    }
}


void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema3::OnWindowResize(int width, int height)
{
}
