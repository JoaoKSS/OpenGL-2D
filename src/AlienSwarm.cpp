#include "AlienSwarm.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>

// Constantes
extern int width;
extern int height;

/**
 * Construtor do enxame de alienígenas
 */
AlienSwarm::AlienSwarm(int rows, int cols, float startX, float startY, float spacing)
    : direction(1.0f), speed(2.0f), dropDistance(20.0f), 
      shootTimer(0), shootInterval(60.0f), rows(rows), cols(cols) {
    
    // Inicializa o gerador de números aleatórios
    srand(static_cast<unsigned>(time(nullptr)));
    
    // Cria o grid de alienígenas
    float alienSize = 30.0f;
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            float x = startX + col * spacing;
            float y = startY - row * spacing;
            
            // Aliens de linhas diferentes valem pontos diferentes
            int points = (rows - row) * 10; // Linha superior vale mais
            
            Alien* alien = new Alien(x, y, alienSize, points);
            aliens.push_back(alien);
        }
    }
}

AlienSwarm::~AlienSwarm() {
    // Libera memória dos aliens
    for (auto alien : aliens) {
        delete alien;
    }
    
    // Libera memória dos tiros
    for (auto shot : shots) {
        delete shot;
    }
}

/**
 * Atualiza o enxame
 */
void AlienSwarm::update() {
    if (aliens.empty()) return;
    
    // Move todos os aliens horizontalmente
    float dx = speed * direction;
    
    for (auto alien : aliens) {
        alien->move(dx, 0);
    }
    
    // Verifica se atingiu as bordas
    if (checkBoundaries()) {
        // Inverte direção e desce
        direction *= -1;
        
        for (auto alien : aliens) {
            alien->move(0, -dropDistance);
        }
    }
    
    // Atualiza os aliens individualmente
    for (auto alien : aliens) {
        alien->update();
    }
    
    // Lógica de disparo
    shootTimer++;
    if (shootTimer >= shootInterval) {
        shootTimer = 0;
        
        // Pega aliens da linha inferior
        std::vector<Alien*> bottomAliens = getBottomRowAliens();
        
        if (!bottomAliens.empty()) {
            // Escolhe um alien aleatório para atirar
            int index = rand() % bottomAliens.size();
            Alien* shooter = bottomAliens[index];
            
            // Cria projétil
            Projectile* shot = new Projectile(
                shooter->getX(), 
                shooter->getY() - shooter->getHeight() / 2, 
                -10.0f, // Velocidade negativa para baixo
                false // Não é do jogador
            );
            shots.push_back(shot);
        }
    }
    
    // Atualiza projéteis
    updateShots();
}

/**
 * Desenha o enxame
 */
void AlienSwarm::draw() {
    for (auto alien : aliens) {
        alien->draw();
    }
    
    drawShots();
}

/**
 * Atualiza os projéteis
 */
void AlienSwarm::updateShots() {
    // Atualiza cada projétil
    for (auto shot : shots) {
        shot->update();
    }
    
    // Remove projéteis inativos
    shots.erase(
        std::remove_if(shots.begin(), shots.end(),
            [](Projectile* shot) {
                if (!shot->isActive()) {
                    delete shot;
                    return true;
                }
                return false;
            }),
        shots.end()
    );
}

/**
 * Desenha os projéteis
 */
void AlienSwarm::drawShots() {
    for (auto shot : shots) {
        shot->draw();
    }
}

/**
 * Verifica se algum alien atingiu a borda
 */
bool AlienSwarm::checkBoundaries() {
    for (auto alien : aliens) {
        float alienLeft = alien->getX() - alien->getWidth() / 2;
        float alienRight = alien->getX() + alien->getWidth() / 2;
        
        if (alienLeft <= 0 || alienRight >= ::width) {
            return true;
        }
    }
    return false;
}

/**
 * Encontra aliens da linha inferior (para disparar)
 */
std::vector<Alien*> AlienSwarm::getBottomRowAliens() {
    std::vector<Alien*> bottomAliens;
    
    // Para cada coluna, encontra o alien mais baixo
    for (int col = 0; col < cols; col++) {
        Alien* lowest = nullptr;
        float lowestY = ::height;
        
        for (auto alien : aliens) {
            // Verifica se está aproximadamente na mesma coluna
            float startX = aliens[0]->getX();
            float spacing = (aliens.size() >= 2) ? 
                           (aliens[1]->getX() - aliens[0]->getX()) : 50.0f;
            
            int alienCol = static_cast<int>((alien->getX() - startX) / spacing + 0.5f);
            
            if (alienCol == col && alien->getY() < lowestY) {
                lowestY = alien->getY();
                lowest = alien;
            }
        }
        
        if (lowest != nullptr) {
            bottomAliens.push_back(lowest);
        }
    }
    
    return bottomAliens;
}

/**
 * Remove alien destruído e retorna seus pontos
 */
int AlienSwarm::removeAlien(Alien* alien) {
    int points = alien->getPoints();
    
    // Remove da lista
    aliens.erase(
        std::remove(aliens.begin(), aliens.end(), alien),
        aliens.end()
    );
    
    delete alien;
    return points;
}

/**
 * Verifica se algum alien chegou à altura do jogador
 */
bool AlienSwarm::reachedPlayer(float playerY) const {
    for (auto alien : aliens) {
        if (alien->getY() <= playerY) {
            return true;
        }
    }
    return false;
}
