#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

/**
 * Classe base para todos os objetos do jogo Player, Alien, Projectile
 */
class GameObject {
protected:
    float x, y; // Posição do objeto
    float width, height; // Dimensões da bounding box
    bool active; // Se o objeto está ativo no jogo

public:
    GameObject(float x, float y, float width, float height);
    virtual ~GameObject();

    virtual void update() = 0;
    virtual void draw() = 0;

    // Getters
    float getX() const { return x; }
    float getY() const { return y; }
    float getWidth() const { return width; }
    float getHeight() const { return height; }
    bool isActive() const { return active; }

    // Setters
    void setPosition(float x, float y);
    void setActive(bool active);

    // Detecção de colisão AABB
    bool checkCollision(const GameObject* other) const;
};

#endif // GAMEOBJECT_H
