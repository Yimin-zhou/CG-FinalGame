#include "collisionManager.h"
#include <iostream>

CollisionManager::CollisionManager() : player(nullptr)
{
}

void CollisionManager::SetPlayer(std::shared_ptr<Player> pPlayer)
{
    player = pPlayer;
}

void CollisionManager::CheckCollisions()
{
    //CheckEnemyProjectileCollisions();
    //CheckBossProjectileCollisions();
    CheckEnemyPlayerCollisions();
    CheckBossPlayerCollisions();
    CheckEnemyEnemyCollisions();
    
}

void CollisionManager::ResolveCollisions()
{
    // Implement your logic to resolve collisions (e.g., destroy enemies, decrease health)
}

//void CollisionManager::CheckEnemyProjectileCollisions()
//{
//    for (auto& enemy : enemies)
//    {
//        for (auto it = projectiles.begin(); it != projectiles.end(); )
//        {
//            if (enemy->GetCollider().CheckCollision((*it)->collider))
//            {
//                // Handle collision between enemy and projectile
//                enemy->TakeDamage(1); // example: reduce enemy health by 1
//
//                // Remove the projectile from the list after handling the collision
//                it = projectiles.erase(it);
//            }
//            else
//            {
//                ++it;
//            }
//        }
//    }
//}
//
//void CollisionManager::CheckBossProjectileCollisions()
//{
//    if (bosses.size() <= 0) return;
//    for (auto& boss : bosses)
//    {
//        for (auto it = projectiles.begin(); it != projectiles.end(); )
//        {
//            if (boss->collider.CheckCollision((*it)->collider))
//            {
//				// Handle collision between enemy and projectile
//				boss->TakeDamage(1); // example: reduce enemy health by 1
//				// Remove the projectile from the list after handling the collision
//				it = projectiles.erase(it);
//			}
//            else
//            {
//				++it;
//			}
//		}
//	}
//}

void CollisionManager::CheckEnemyPlayerCollisions()
{
    if (player == nullptr)
    {
        return;
    }

    for (auto& enemy : enemies)
    {
        // Handle collision between enemy and player
        if (enemy->GetCollider().CheckCollision(player->collider))
        {
            // Apply knockback
            glm::vec3 knockbackDirection = player->GetPosition() - enemy->GetPosition();
            float knockbackForce = 0.17f; // Customize this value as needed
            player->ApplyKnockback(knockbackDirection, knockbackForce);

            // Play sound effect
            // You can use your preferred sound library to play a sound effect here
            // Example: SoundManager::PlaySound("player_hit");

            // Apply visual effect, e.g., flashing color on player model
            // Example: player->model->SetColorFlash(1.0, 0.0, 0.0, 0.3);
        }
    }
}

void CollisionManager::CheckBossPlayerCollisions()
{
    if (player == nullptr)
    {
        return;
    }

    for (auto& boss : bosses)
    {
        // Handle collision between boss and player
        if (boss->collider.CheckCollision(player->collider))
        {
            glm::vec3 direction = player->GetPosition() - boss->GetPosition();
            // Apply knockback
            float knockbackForce = 0.3f; // Customize this value as needed
            player->ApplyKnockback(direction, knockbackForce);

            // Play sound effect
            // You can use your preferred sound library to play a sound effect here
            // Example: SoundManager::PlaySound("player_hit");

            // Apply visual effect, e.g., flashing color on player model
            // Example: player->model->SetColorFlash(1.0, 0.0, 0.0, 0.3);


            //// Calculate the vector between the boss and player
            //float distance = glm::length(direction);
            //float overlap = player->collider.GetRadius() + boss->collider.GetRadius() - distance;

            //// Normalize the vector and scale it by the overlap
            //glm::vec3 normalizedDirection = direction / distance;
            //glm::vec3 separation = normalizedDirection * overlap * 1.5f;

            //// Move the boss and player away from each other to prevent clipping
            //player->position.x = player->GetPosition().x - separation.x;
            //player->position.z = player->GetPosition().z - separation.z;
            //boss->position.x = boss->GetPosition().x + separation.x;
            //boss->position.z = boss->GetPosition().z + separation.z;

            //// Update the colliders' positions
            //player->collider.SetPosition(player->GetPosition());
            //boss->collider.SetPosition(boss->GetPosition());
            
        }
    }
}


void CollisionManager::CheckEnemyEnemyCollisions()
{
    for (size_t i = 0; i < enemies.size(); ++i)
    {
        for (size_t j = i + 1; j < enemies.size(); ++j)
        {
            if (enemies[i]->GetCollider().CheckCollision(enemies[j]->GetCollider()))
            {
                // Calculate the vector between the two enemies
                glm::vec3 delta = enemies[j]->GetPosition() - enemies[i]->GetPosition();
                float distance = glm::length(delta);
                float overlap = enemies[i]->GetCollider().GetRadius() + enemies[j]->GetCollider().GetRadius() - distance;

                // Normalize the vector and scale it by the overlap
                glm::vec3 direction = delta / distance;
                glm::vec3 separation = direction * overlap * 0.5f;

                // Move the enemies away from each other to prevent clipping
                enemies[i]->SetPosition(enemies[i]->GetPosition() - separation);
                enemies[j]->SetPosition(enemies[j]->GetPosition() + separation);

                // Update the colliders' positions
                enemies[i]->GetCollider().SetPosition(enemies[i]->GetPosition());
                enemies[j]->GetCollider().SetPosition(enemies[j]->GetPosition());
            }
        }
    }
}
