#ifndef SETTINGS_H
#define SETTINGS_H

class Settings
{
public:
    Settings();

    static Settings s;

    // Lighting settings
    float occlusionDistanceScale = 2.0;
    float occlusionStrengthScale = 0.25;


    int shadowSteps = 200;
    float shadowMultiplier = 0.9;
    float shadowThreshold = 0.10;

    float blockScale = 0.2;

    int noMaterials = 5;

    // Landscape chunk settings
    static const int chunkSize = 32;
    int worldSizeXZ = 16;
    int worldSizeY = 6;



};

#endif // SETTINGS_H
