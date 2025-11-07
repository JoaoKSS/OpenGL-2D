#ifndef ALIEN_H
#define ALIEN_H

#include "GameObject.h"

/**
 * Classe que representa um alienígena individual
 */
class Alien : public GameObject {
private:
    int points; // Pontos que o jogador ganha ao destruir este alien

public:
    Alien(float x, float y, float size, int points = 10);
    
    void update() override;
    void draw() override;
    
    // Movimentação controlada pelo AlienSwarm
    void move(float dx, float dy);
    
    int getPoints() const { return points; }
};

#endif // ALIEN_H
