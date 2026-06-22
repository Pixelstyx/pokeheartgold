#ifndef POKEHEARTGOLD_UNK_02087A78_H
#define POKEHEARTGOLD_UNK_02087A78_H

#include "field_system.h"
#include "global.h"
#include "overlay_manager.h"
#include "save.h"
#include "trainer_data.h"

extern const OverlayManagerTemplate gOverlayManagerTemplate_02087A78_0;

typedef struct UnkStruct_02087A8C_2 {
    BOOL unk0;
    int unk4;
    u8 padding[0x8];
} UnkStruct_02087A8C_2;

typedef struct UnkStruct_02087A8C {
    u32 unk0;
    u8 unk4[0x6D4];
    u32 unk6D8;
    u8 unk6DC[0x140];
    void *unk81C[5]; // I'm just assuming this goes all the way up until told otherwise.
    SaveData *saveData;
    u8 unk834[0x34];
    UnkStruct_02087A8C_2 *unk868;
    u32 unk86C;
    u32 unk870;
    u32 unk874;
    u8* unk878;
    u8 unk879;
    u16 unk87A;
    u8 unk87C[0x38F8];
} UnkStruct_02087A8C; // Size: 0x4170

typedef struct Heap126Data {
    u32 state;
    u32 unk4;
    OverlayManager *overlayManager;
    SaveData *saveData;
    BattleSetup *battleSetup;
    UnkStruct_02087A8C *unk_02087A8C;
    UnkStruct_02087A8C_2 unk_02087A8C_2;
    FieldSystem *fieldSystem;
} Heap126Data; // Size: 0x2C

UnkStruct_02087A8C *sub_02087A78(OverlayManager *man);
void sub_02087A84(UnkStruct_02087A8C_2 *unkStruct, BOOL unkBool, int unkVal);

const OverlayManagerTemplate *sub_02087E10(int index);
BOOL UnkStruct_02087A8C_HasUnionCaveFlypoint(UnkStruct_02087A8C *unk_02087A8C);

// TODO: Move to relevant headers.
void *sub_02087FF8(UnkStruct_02087A8C*, int);

extern const OverlayManagerTemplate _021028B4;
extern const OverlayManagerTemplate _021028C4;

void LoadOVY38(); // LoadHttpOverlay?
void UnloadOVY38(); // UnloadHttpOverlay?
void sub_0202FC90(SaveData *saveData, enum HeapID heapID, s32*, BattleSetup *battleSetup, s32);
void sub_020304F0(BattleSetup *battleSetup, SaveData *saveData);
int sub_0202FC5C();
void ov40_02244920(int, BattleSetup *battleSetup, enum HeapID heapID);
void sub_02005B68(int);

void Sound_SetFieldBGM(u16 seqNo);

#endif // POKEHEARTGOLD_UNK_02087A78_H
