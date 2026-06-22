#include "unk_02087A78.h"

#include "field_bgm.h"
#include "field_system.h"
#include "global.h"
#include "launch_application.h"
#include "overlay_manager.h"
#include "poke_overlay.h"
#include "sound_02004A44.h"
#include "sys_flags.h"
#include "unk_0202FBCC.h"

#include "battle/battle_setup.h"
#include "frontier/overlay_80.h"

#include "constants/sndseq.h"

FS_EXTERN_OVERLAY(OVY_39);
FS_EXTERN_OVERLAY(OVY_40);

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

static BOOL sub_02087BE8(Heap126Data *data, enum HeapID heapID);
static BOOL sub_02087C38(Heap126Data *data, enum HeapID heapID);
static BOOL IsFrontierBrain(u8 trainerClass);

UnkStruct_02087A8C *sub_02087A78(OverlayManager *man) {
    Heap126Data *data = OverlayManager_GetArgs(man);
    return data->unk_02087A8C;
}

void sub_02087A84(UnkStruct_02087A8C_2 *unkStruct, BOOL unkBool, int unkVal) {
    unkStruct->unk0 = unkBool;
    unkStruct->unk4 = unkVal;
}

static void sub_02087A8C(OverlayManager *man, u32 arg1) {
    Heap_Create(HEAP_ID_3, HEAP_ID_126, 0x10000);

    Heap126Data *data = OverlayManager_CreateAndGetData(man, sizeof(Heap126Data), HEAP_ID_126);
    MI_CpuFill8(data, 0, sizeof(Heap126Data));

    data->fieldSystem = OverlayManager_GetArgs(man);
    data->saveData = data->fieldSystem->saveData;
    data->unk_02087A8C = Heap_Alloc(HEAP_ID_126, sizeof(UnkStruct_02087A8C));

    MI_CpuFill8(data->unk_02087A8C, 0, sizeof(UnkStruct_02087A8C));

    data->unk_02087A8C->unk868 = &data->unk_02087A8C_2;
    data->unk_02087A8C->saveData = data->saveData;
    data->unk_02087A8C->unk0 = arg1; // arg1 == eventType?
    data->unk_02087A8C->unk81C[data->unk_02087A8C->unk6D8] = sub_02087FF8(data->unk_02087A8C, data->unk_02087A8C->unk0);
}

static BOOL sub_02087B10(OverlayManager *man, int *state_unused) {
    sub_02087A8C(man, 0);
    return TRUE;
}

static BOOL sub_02087B1C(OverlayManager *man, int *state_unused) {
    sub_02087A8C(man, 1);
    return TRUE;
}

static BOOL sub_02087B28(OverlayManager *man, int *state_unused) {
    sub_02087A8C(man, 2);
    return TRUE;
}

static BOOL sub_02087B34(OverlayManager *man, int *state_unused) {
    sub_02087A8C(man, 3);
    return TRUE;
}

static BOOL sub_02087B40(OverlayManager *man, int *state_unused) {
    sub_02087A8C(man, 4);
    return TRUE;
}

static BOOL sub_02087B4C(OverlayManager *man, int *state_unused) {
    sub_02087A8C(man, 5);
    return TRUE;
}

static BOOL sub_02087B58(OverlayManager *man, int *state_unused) {
    sub_02087A8C(man, 6);
    return TRUE;
}

static BOOL sub_02087B64(OverlayManager *man, int* arg1) {
    Heap126Data *data = OverlayManager_GetData(man);
    switch (*arg1) {
        case 0:
            if (sub_02087BE8(data, HEAP_ID_126)) {
                if (data->unk_02087A8C_2.unk0 == TRUE) {
                    *arg1 = 1;
                    data->state = 0;
                } else {
                    return TRUE;
                }
            }
            break;
        case 1:
            if (sub_02087C38(data, HEAP_ID_126)) {
                *arg1 = 0;
                data->state = 0;
            }
            break;
    }
    return FALSE;
}

static BOOL sub_02087BAC(OverlayManager *man, int *state_unused) {
    Heap126Data *data = OverlayManager_GetData(man);

    if (sub_0202FC48() == TRUE) { // BattleRecording_Exists?
        sub_0202FC24(); // BattleRecording_Free?
    }

    Heap_Free(data->unk_02087A8C);
    OverlayManager_FreeData(man);
    GF_SndHandleSetPlayerVolume(1, 127);
    GF_SndHandleSetPlayerVolume(7, 127);
    Heap_Destroy(HEAP_ID_126);
    return TRUE;
}

static BOOL sub_02087BE8(Heap126Data *data, enum HeapID heapID) {
    if (data->state == 0) {
        if (data->unk_02087A8C->unk0 == 0) {
            data->overlayManager = OverlayManager_New(&_021028B4, data, heapID);
        } else {
            data->overlayManager = OverlayManager_New(&_021028C4, data, heapID);
        }
        data->state++;
    } else if (OverlayManager_Run(data->overlayManager)) {
        OverlayManager_Delete(data->overlayManager);
        return TRUE;
    }
    return FALSE;
}

static BOOL sub_02087C38(Heap126Data* data, enum HeapID heapID) {
    switch (data->state) {
    case 0:
        if (data->unk_02087A8C->unk0) {
            UnloadOverlayByID(FS_OVERLAY_ID(OVY_39));
            UnloadOVY38();
        }
        data->state++;
        break;
    case 1:
        int v0;

        data->battleSetup = BattleSetup_New(heapID, 0);

        if (sub_0202FC48() == FALSE) { // BattleRecording_Exists?
            sub_0202FC90(data->saveData, heapID, &v0, data->battleSetup, data->unk_02087A8C->unk86C);
        } else {
            sub_020304F0(data->battleSetup, data->saveData);
            v0 = 1;
        }

        data->battleSetup->bagCursor = BagCursor_New(heapID);
        data->battleSetup->gameStats = Save_GameStats_Get(data->saveData);

        if (HandleLoadOverlay(FS_OVERLAY_ID(OVY_40), OVY_LOAD_ASYNC) == TRUE) {
            ov40_02244920(sub_0202FC5C(), data->battleSetup, heapID); // sub_0202FC5C == BattleRecording_Get()?
            UnloadOverlayByID(FS_OVERLAY_ID(OVY_40));
        }

        data->unk_02087A8C->unk874 = 1;

        if (v0 != 1) {
            Heap_Free(data->battleSetup->bagCursor);
            BattleSetup_Delete(data->battleSetup);
            data->state = 0;
            return TRUE;
        }

        data->state++;
        break;
    case 2:
        GF_SndHandleSetPlayerVolume(1, 127);
        GF_SndHandleSetPlayerVolume(7, 127);
        sub_02005B68(1);

        if (IsFrontierBrain(data->battleSetup->trainer[1].data.trainerClass) == TRUE) {
            Sound_SetSceneAndPlayBGM(5, SEQ_GS_BA_BRAIN, 1); // SOUND_SCENE_BATTLE ?
        } else {
            Sound_SetSceneAndPlayBGM(5, SEQ_GS_VS_TRAINER, 1); // SOUND_SCENE_BATTLE ?
        }

        data->overlayManager = OverlayManager_New(&gOverlayTemplate_Battle, data->battleSetup, heapID);
        data->state++;
        break;
    default:
        if (OverlayManager_Run(data->overlayManager)) {
            data->unk_02087A8C->unk874 = data->battleSetup->unk1C4; // recordingStopped?

            if (data->unk_02087A8C->unk0 && data->unk_02087A8C->unk874 == 0) {
                *(data->unk_02087A8C->unk878) = 1;
            }

            Heap_Free(data->battleSetup->bagCursor);
            BattleSetup_Delete(data->battleSetup);
            OverlayManager_Delete(data->overlayManager);

            sub_02005B68(0);
            Sound_SetScene(0); // SOUND_SCENE_NONE ?
            u16 bgmID = FieldBGM_GetEffective(data->fieldSystem, data->fieldSystem->location->mapId);
            Sound_SetFieldBGM(FieldBGM_GetForMapHeader(data->fieldSystem, data->fieldSystem->location->mapId));
            sub_02055198(NULL, bgmID);

            data->state = 0;

            if (data->unk_02087A8C->unk0) {
                LoadOVY38();
                HandleLoadOverlay(FS_OVERLAY_ID(OVY_39), OVY_LOAD_ASYNC);
            }

            return TRUE;
        }
        break;
    }
    return FALSE;
}

const OverlayManagerTemplate gOverlayManagerTemplate_02087A78_0 = {
    .init = sub_02087B10,
    .exec = sub_02087B64,
    .exit = sub_02087BAC,
    .ovy_id = 0xFFFFFFFF,
};

const OverlayManagerTemplate gOverlayManagerTemplate_02087A78_1 = {
    .init = sub_02087B58,
    .exec = sub_02087B64,
    .exit = sub_02087BAC,
    .ovy_id = 0xFFFFFFFF,
};

const OverlayManagerTemplate gOverlayManagerTemplate_02087A78_2 = {
    .init = sub_02087B4C,
    .exec = sub_02087B64,
    .exit = sub_02087BAC,
    .ovy_id = 0xFFFFFFFF,
};

const OverlayManagerTemplate gOverlayManagerTemplate_02087A78_3 = {
    .init = sub_02087B40,
    .exec = sub_02087B64,
    .exit = sub_02087BAC,
    .ovy_id = 0xFFFFFFFF,
};

const OverlayManagerTemplate gOverlayManagerTemplate_02087A78_4 = {
    .init = sub_02087B34,
    .exec = sub_02087B64,
    .exit = sub_02087BAC,
    .ovy_id = 0xFFFFFFFF,
};

const OverlayManagerTemplate gOverlayManagerTemplate_02087A78_5 = {
    .init = sub_02087B28,
    .exec = sub_02087B64,
    .exit = sub_02087BAC,
    .ovy_id = 0xFFFFFFFF,
};

const OverlayManagerTemplate gOverlayManagerTemplate_02087A78_6 = {
    .init = sub_02087B1C,
    .exec = sub_02087B64,
    .exit = sub_02087BAC,
    .ovy_id = 0xFFFFFFFF,
};

static const OverlayManagerTemplate *gOverlayManagerTemplates[7] = {
    &gOverlayManagerTemplate_02087A78_0,
    &gOverlayManagerTemplate_02087A78_1,
    &gOverlayManagerTemplate_02087A78_2,
    &gOverlayManagerTemplate_02087A78_3,
    &gOverlayManagerTemplate_02087A78_4,
    &gOverlayManagerTemplate_02087A78_5,
    &gOverlayManagerTemplate_02087A78_6
};

const OverlayManagerTemplate *sub_02087E10(int index) {
    return gOverlayManagerTemplates[index];
}

BOOL UnkStruct_02087A8C_HasUnionCaveFlypoint(UnkStruct_02087A8C *unk_02087A8C) {
    return Save_VarsFlags_FlypointFlagAction(Save_VarsFlags_Get(unk_02087A8C->saveData), FLAG_ACTION_CHECK, FLYPOINT_UNION_CAVE);
}

static BOOL IsFrontierBrain(u8 trainerClass) {
    int sFrontierBrainTrainerClasses[5] = {
        TRAINERCLASS_TOWER_TYCOON,
        TRAINERCLASS_HALL_MATRON,
        TRAINERCLASS_FACTORY_HEAD,
        TRAINERCLASS_ARCADE_STAR,
        TRAINERCLASS_CASTLE_VALET
    };

    for (u32 i = 0; i < 5; i++) {
        if (trainerClass == sFrontierBrainTrainerClasses[i]) {
            return TRUE;
        }
    }
    return FALSE;
}