#pragma once
#include <Windows.h>
namespace offsets
{
    DWORD
        Uworld = 0x914ED78,
        Ulevel = 0x30,
        Matrix = 0x92E4080,
        Mlevel1 = 0xE8,
        Mlevel2 = 0x8,
        Mlevel3 = 0x568,
        Mlevel4 = 0x0,
        Mlevel5 = 0x20,


        GameState = 0x1B0,
        PlayerArray = 0x2F8,
        PlayerCount = 0x300,
        PawnPrivate = 0x358,
        RootComponent = 0x1A8,


        Location = 0x290,

        OwningGameInstance = 0x210,
        playername = 0x3D8,
        playernamecount = 0x3E0,
        USquadComponent = 0x888,
        LocalPlayers = 0xB0,
        localplayercontroller = 0x48,
        ackpawn = 0x3E8,


        healthcomponent = 0x7F0, //Class ADiscoveryCharacter
        currenthealth = 0x600,
        maxhealth = 0x604,
        USkeletalMeshComponent = 0x360,
        MeshArray = 0x720,
        cache = 0x6B0,

        ComponentToWorld = 0x220, // 1 1 1 218

        PlayerCameraManager = 0x400,
        viewoffset = 0x3F0,




        GetSquadIndex = 0x130;  //Class ADiscoveryCharacter

        

}

//    float                                         MinDrawDistance;                                              // 0x0360(0x0004)
//float                                         LDMaxDrawDistance;                                            // 0x0364(0x0004)
//float                                         CachedMaxDrawDistance;                                        // 0x0368(0x0004)

//    uint8                                         bVisibleInRayTracing;                                         // 0x0399(0x0001)

//    int32                                         RayTracingGroupId;                                            // 0x03A4(0x0004)
//int32                                         VisibilityId;                                                 // 0x03A8(0x0004)