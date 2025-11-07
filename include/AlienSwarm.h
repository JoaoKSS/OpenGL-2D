#ifndef ALIENSWARM_H
#define ALIENSWARM_H

#include "Alien.h"
#include "Projectile.h"
#include <vector>

/**
 * Classe que gerencia o enxame de alienígenas
 * Move todos os aliens como um bloco e controla seus disparos
 */
class AlienSwarm {
private:
    std::vector<Alien*> aliens; // Lista de alienígenas ativos
    std::vector<Projectile*> shots; // Projéteis dos alienígenas
    
    float direction; // Direção do movimento direita= 1, esquerda= -1
    float speed; // Velocidade horizontal
    float dropDistance; // Distância de descida ao atingir borda
    
    float shootTimer; // Timer para controlar disparos
    float shootInterval; // Intervalo entre disparos
    
    int rows; // Número de linhas
    int cols; // Número de colunas
    
    // Verifica se algum alien atingiu a borda
    bool checkBoundaries();
    
    // Encontra aliens da linha inferior para disparar
    std::vector<Alien*> getBottomRowAliens();

public:
    AlienSwarm(int rows, int cols, float startX, float startY, float spacing);
    ~AlienSwarm();

    void update();
    void draw();
    
    // Atualiza e desenha os projéteis
    void updateShots();
    void drawShots();
    
    // Remove alien destruído e retorna seus pontos
    int removeAlien(Alien* alien);
    
    // Verifica se todos os aliens foram destruídos
    bool isEmpty() const { return aliens.empty(); }
    
    // Verifica se algum alien chegou à altura do jogador
    bool reachedPlayer(float playerY) const;
    
    // Getters
    const std::vector<Alien*>& getAliens() const { return aliens; }
    std::vector<Projectile*>& getShots() { return shots; }
};

#endif // ALIENSWARM_H
