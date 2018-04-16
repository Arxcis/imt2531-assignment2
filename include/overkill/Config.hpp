#pragma once
#include <glm/glm.hpp>
#include <string>

namespace overkill::C
{
    using Tag = std::string;
    using ID  = int;

    constexpr int VersionMajor = 4;
    constexpr int VersionMinor = 1;
    constexpr int WinWidth = 1600;
    constexpr int WinHeight = 900;
    constexpr char WinName[] = "Assignment 2 - Cube";
    constexpr float FOV = 90.0f;
    constexpr float MaxFOV = 90.8f;
    constexpr float MinFOV = 88.1f;
    constexpr float ZoomSensitivity = 0.1f;
    constexpr float LookSensitivity = 4;
    constexpr float AspectRatio = (float)WinWidth / (float)WinHeight;
    constexpr float NearClip = 0.1f;
    constexpr float FarClip  = 100.0f;
    constexpr float CameraOffset = -3;      //How far back the camera is from the center of the scene.
    const glm::vec4 ClearColor { 0.0f, 0.0f, 0.0f, 1.0f};
    constexpr char PathBaseShader[] = "assets/shaders/base.shader";
    constexpr char PathBaseTexture[] = "assets/textures/Checkers.jpg";
    constexpr float PI = 3.14159265359f;

    constexpr char TexturesFolder[]  = "assets/textures/";
    constexpr char TexturesExtension[]  = ".jpg";

    constexpr char ShadersFolder[]   = "assets/shaders/";
    constexpr char ShadersExtension[]   = ".shader";

    constexpr char MaterialsFolder[] = "assets/materials/";
    constexpr char MaterialsExtension[] = ".yml";

    constexpr char ModelsFolder[]    = "assets/models/";
    constexpr char ModelsExtension[]    = ".yml";

    constexpr char ScenesFolder[]    = "assets/scenes/";
    constexpr char ScenesExtension[]    = ".yml";

       
}
