#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"
#include "Projectile.h"

/**
 * Classe que representa o jogador
 */
class Player : public GameObject {
private:
    float speed; // Velocidade de movimentação
    int lives; // Vidas restantes
    Projectile* shot; // Projétil do jogador apenas um ativo por vez

public:
    Player(float x, float y, float size, int lives = 3);
    ~Player();

    void update() override;
    void draw() override;

    // Movimentação
    void moveLeft();
    void moveRight();

    // Disparo
    bool canShoot() const;
    void shoot();

    // Gerenciamento de vidas
    void loseLife();
    int getLives() const { return lives; }
    bool isAlive() const { return lives > 0; }

    // Acesso ao projétil
    Projectile* getShot() { return shot; }
    void updateShot();
    void drawShot();

    // Reset de posição após perder vida
    void reset();
};

#endif // PLAYER_H
