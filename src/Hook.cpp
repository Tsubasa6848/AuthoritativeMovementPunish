#include <LoggerAPI.h>
#include <HookAPI.h>
#include <mc/ServerPlayerEventCoordinator.hpp>
#include <mc/Player.hpp>
#include <mc/Vec3.hpp>
#include <LoggerAPI.h>
#include <ScheduleAPI.h>
#include <mc/ItemStack.hpp>
#include <mc/ItemStackBase.hpp>
#include <mc/TridentItem.hpp>

Logger logger("Server");
std::unordered_set<std::string> ExcludeList = {};

TInstanceHook(void, "?sendPlayerMovementAnomaly@ServerPlayerEventCoordinator@@QEAAXAEAVPlayer@@AEBVVec3@@MM@Z", ServerPlayerEventCoordinator, Player* pl, Vec3* pos, float a1, float a2) {
    if (pl->isLoading()) {
        ExcludeList.insert(pl->getRealName());
        auto task = Schedule::delay([pl](){
            ExcludeList.erase(pl->getRealName());
        }, 200);
        return original(this, pl, pos, a1, a2);
    }
    if (ExcludeList.count(pl->getRealName()) == false) {
        logger.warn("Player '{}' may moving abnormally.", pl->getRealName());
    }
    return original(this, pl, pos, a1, a2);
}

TInstanceHook(void, "?releaseUsing@TridentItem@@UEBAXAEAVItemStack@@PEAVPlayer@@H@Z", TridentItem, ItemStack* it, Player* pl, int a1) {
    if (isThrowable() == false && pl->isInWaterOrRain()) {
        ExcludeList.insert(pl->getRealName());
        Schedule::delay([pl](){
            ExcludeList.erase(pl->getRealName());
        }, 40);
    }
    return original(this, it, pl, a1);
}

TInstanceHook(void, "?sendPlayerMovementCorrected@ServerPlayerEventCoordinator@@QEAAXAEAVPlayer@@AEBVVec3@@MM@Z", ServerPlayerEventCoordinator, Player* pl, Vec3* pos, float a1, float a2) {
    if (pl->isLoading()) {
        return original(this, pl, pos, a1, a2);
    }
    if (ExcludeList.count(pl->getRealName()) == false) {
        logger.warn("Player '{}' was detected moving abnormally.", pl->getRealName());
        pl->kick("客户端与服务端位置不匹配");
    }
    return original(this, pl, pos, a1, a2);
}
