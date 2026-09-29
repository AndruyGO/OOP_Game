
#ifndef COMMANDS_H
#define COMMANDS_H

enum class Command {
    kNone,
    kMoveUp,
    kMoveDown,
    kMoveLeft,
    kMoveRight,
    kWait,
    kQuit,
    kSelectTarget,
    kUseAreaStrike,
    kUseHeal,
    kUseTeleport,
    kUseFarHit,
    kCancel
};

#endif