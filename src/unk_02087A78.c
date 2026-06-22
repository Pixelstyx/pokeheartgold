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

static BOOL isFrontierBrain(u8 trainerClass);

MapEvents *sub_02087A78(OverlayManager *man) { // GetMapEvents?
    FieldSystem *fieldSystem = OverlayManager_GetArgs(man);
    return fieldSystem->mapEvents;
}

void sub_02087A84(FieldSystem* fieldSystem, FieldSystemUnkSub0 *unkSub0, FieldSystemUnkSub4 *unkSub4) {
    fieldSystem->unk0 = unkSub0;
    fieldSystem->unk4 = unkSub4;
}

static void sub_02087A8C(OverlayManager *man, u32 arg1) {
    Heap_Create(HEAP_ID_3, HEAP_ID_126, 0x10000);
    Heap126Data *data = OverlayManager_CreateAndGetData(man, sizeof(Heap126Data), HEAP_ID_126);
    MI_CpuFill8(data, 0, sizeof(Heap126Data));
    data->fieldSystem = OverlayManager_GetArgs(man);
    data->saveData = data->fieldSystem->saveData;
    data->unk_02087A8C = Heap_Alloc(HEAP_ID_126, sizeof(UnkStruct_02087A8C));
    MI_CpuFill8(data->unk_02087A8C, 0, sizeof(UnkStruct_02087A8C));
    data->unk_02087A8C->unk868 = &data->unk18;
    data->unk_02087A8C->saveData = data->saveData;
    data->unk_02087A8C->unk0 = arg1;
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
                if (data->unk18 == 1) {
                    *arg1 = 1;
                    data->unk0 = 0;
                } else {
                    return TRUE;
                }
            }
            break;
        case 1:
            if (sub_02087C38(data, HEAP_ID_126)) {
                *arg1 = 0;
                data->unk0 = 0;
            }
            break;
    }
    return FALSE;
}

static BOOL sub_02087BAC(OverlayManager *man, int *state_unused) {
    FieldSystem *fieldSystem = OverlayManager_GetData(man);
    if (sub_0202FC48() == TRUE) {
        sub_0202FC24();
    }
    Heap_Free(fieldSystem->mapEvents);
    OverlayManager_FreeData(man);
    GF_SndHandleSetPlayerVolume(1, 127);
    GF_SndHandleSetPlayerVolume(7, 127);
    Heap_Destroy(HEAP_ID_126);
    return TRUE;
}

static BOOL sub_02087BE8(Heap126Data *data, enum HeapID heapID) {
    if (data->unk0 == 0) {
        if (data->unk_02087A8C->unk0 == 0) {
            data->overlayManager = OverlayManager_New(&_021028B4, data, heapID);
        } else {
            data->overlayManager = OverlayManager_New(&_021028C4, data, heapID);
        }
        data->unk0++;
    } else if (OverlayManager_Run(data->overlayManager)) {
        OverlayManager_Delete(data->overlayManager);
        return TRUE;
    }
    return FALSE;
}

static BOOL sub_02087C38(Heap126Data* data, enum HeapID heapID) {
    switch (data->unk0) {
    case 0:
        if (data->unk_02087A8C->unk0) {
            UnloadOverlayByID(FS_OVERLAY_ID(OVY_39));
            UnloadOVY38();
        }
        data->unk0++;
        break;
    case 1:
        s32 sp4;
        data->battleSetup = BattleSetup_New(heapID, 0);
        if (sub_0202FC48() == FALSE) {
            sub_0202FC90(data->saveData, heapID, &sp4, data->battleSetup, data->unk_02087A8C->unk86C);
        } else {
            sub_020304F0(data->battleSetup, data->saveData);
            sp4 = 1;
        }
        data->battleSetup->bagCursor = BagCursor_New(heapID);
        data->battleSetup->gameStats = Save_GameStats_Get(data->saveData);
        if (HandleLoadOverlay(FS_OVERLAY_ID(OVY_40), OVY_LOAD_ASYNC) == 1) {
            ov40_02244920(sub_0202FC5C(), data->battleSetup, heapID);
            UnloadOverlayByID(FS_OVERLAY_ID(OVY_40));
        }
        data->unk_02087A8C->unk874 = 1;
        if (sp4 != 1) {
            Heap_Free(data->battleSetup->bagCursor);
            BattleSetup_Delete(data->battleSetup);
            data->unk0 = 0;
            return 1;
        }
        data->unk0++;
        break;
    case 2:
        GF_SndHandleSetPlayerVolume(1, 127);
        GF_SndHandleSetPlayerVolume(7, 127);
        sub_02005B68(1);
        if (isFrontierBrain(data->battleSetup->trainer[1].data.trainerClass) == TRUE) {
            Sound_SetSceneAndPlayBGM(5, SEQ_GS_BA_BRAIN, 1);
        } else {
            Sound_SetSceneAndPlayBGM(5, SEQ_GS_VS_TRAINER, 1);
        }
        data->overlayManager = OverlayManager_New(&gOverlayTemplate_Battle, data->battleSetup, heapID);
        data->unk0++;
        break;
    default:
        if (OverlayManager_Run(data->overlayManager)) {
            data->unk_02087A8C->unk874 = data->battleSetup->unk1C4;
            if (data->unk_02087A8C->unk0 && data->unk_02087A8C->unk874 == 0) {
                *(data->unk_02087A8C->unk878) = 1;
            }
            Heap_Free(data->battleSetup->bagCursor);
            BattleSetup_Delete(data->battleSetup);
            OverlayManager_Delete(data->overlayManager);
            sub_02005B68(0);
            Sound_SetScene(0);
            u16 bgmSeq = FieldBGM_GetEffective(data->fieldSystem, data->fieldSystem->location->mapId);
            Sound_SetFieldBGM(FieldBGM_GetForMapHeader(data->fieldSystem, data->fieldSystem->location->mapId));
            sub_02055198(NULL, bgmSeq);
            data->unk0 = 0;
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

const OverlayManagerTemplate gOverlayManagerTemplate_0 = {
    .init = sub_02087B10,
    .exec = sub_02087B64,
    .exit = sub_02087BAC,
    .ovy_id = 0xFFFFFFFF,
};

const OverlayManagerTemplate gOverlayManagerTemplate_1 = {
    .init = sub_02087B58,
    .exec = sub_02087B64,
    .exit = sub_02087BAC,
    .ovy_id = 0xFFFFFFFF,
};

const OverlayManagerTemplate gOverlayManagerTemplate_2 = {
    .init = sub_02087B4C,
    .exec = sub_02087B64,
    .exit = sub_02087BAC,
    .ovy_id = 0xFFFFFFFF,
};

const OverlayManagerTemplate gOverlayManagerTemplate_3 = {
    .init = sub_02087B40,
    .exec = sub_02087B64,
    .exit = sub_02087BAC,
    .ovy_id = 0xFFFFFFFF,
};

const OverlayManagerTemplate gOverlayManagerTemplate_4 = {
    .init = sub_02087B34,
    .exec = sub_02087B64,
    .exit = sub_02087BAC,
    .ovy_id = 0xFFFFFFFF,
};

const OverlayManagerTemplate gOverlayManagerTemplate_5 = {
    .init = sub_02087B28,
    .exec = sub_02087B64,
    .exit = sub_02087BAC,
    .ovy_id = 0xFFFFFFFF,
};

const OverlayManagerTemplate gOverlayManagerTemplate_6 = {
    .init = sub_02087B1C,
    .exec = sub_02087B64,
    .exit = sub_02087BAC,
    .ovy_id = 0xFFFFFFFF,
};

const OverlayManagerTemplate *gOverlayManagerTemplates[7] = {
    &gOverlayManagerTemplate_0,
    &gOverlayManagerTemplate_1,
    &gOverlayManagerTemplate_2,
    &gOverlayManagerTemplate_3,
    &gOverlayManagerTemplate_4,
    &gOverlayManagerTemplate_5,
    &gOverlayManagerTemplate_6
};

const OverlayManagerTemplate *sub_02087E10(int index) { // I don't actually know if this is the right struct, but it fits
    return gOverlayManagerTemplates[index];
}

void UnkStruct_02087A8C_HasUnionCaveFlypoint(UnkStruct_02087A8C *unk_02087A8C) { // sub_02087E1C
    Save_VarsFlags_FlypointFlagAction(Save_VarsFlags_Get(unk_02087A8C->saveData), FLAG_ACTION_CHECK, FLYPOINT_UNION_CAVE);
}

static BOOL isFrontierBrain(u8 trainerClass) {
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