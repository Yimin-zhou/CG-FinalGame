#pragma once

#include <vector>
#include "enemy.h"
#include "projectile.h"
#include "player.h"
#include "boss.h"

class CollisionManager {
public:
    CollisionManager();
    std::vector<std::shared_ptr<Boss>> bosses;
    std::vector<std::shared_ptr<Enemy>> enemies;
    std::vector<std::shared_ptr<Projectile>> projectiles;
    std::shared_ptr<Player> player;

    void SetPlayer(std::shared_ptr<Player> player);

    void CheckCollisions();
    void ResolveCollisions();

private:
    void CheckBossProjectileCollisions();
    void CheckEnemyProjectileCollisions();
    void CheckEnemyPlayerCollisions();
    void CheckEnemyEnemyCollisions();
    void CheckBossPlayerCollisions();
};
