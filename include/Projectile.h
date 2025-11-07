#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "GameObject.h"

/**
 * Classe para os projéteis tanto do jogador quanto dos alienígenas
 */
class Projectile : public GameObject {
private:
    float speed;       // Velocidade do projétil
    bool fromPlayer;   // true se for do jogador e false se for do alien

public:
    Projectile(float x, float y, float speed, bool fromPlayer);
    
    void update() override;
    void draw() override;
    
    bool isFromPlayer() const { return fromPlayer; }
};

#endif // PROJECTILE_H
