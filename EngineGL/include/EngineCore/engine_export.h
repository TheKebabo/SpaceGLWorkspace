#pragma once

/**
 * When building the EngineCore DLL, we define BUILDING_MY_ENGINE in CMake.
 * This tells the compiler to "Export" the functions/classes so they are 
 * visible outside the DLL.
 * * When another project includes this header, 
 * BUILDING_MY_ENGINE is NOT defined, so it "Imports" them instead.
 */

 
#define ENGINE_API

// DLL logic
// #ifdef _WIN32
//     #ifdef BUILDING_MY_ENGINE
//         #define ENGINE_API __declspec(dllexport)
//     #else
//         #define ENGINE_API __declspec(dllimport)
//     #endif
// #else
//     // For Linux/Mac, symbols are exported by default, so we define it as nothing.
//     #define ENGINE_API
// #endif