#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "AlienSwarm.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

/**
 * Estados possíveis do jogo
 */
enum GameState {
    MENU,
    PLAYING,
    GAME_OVER,
    WIN
};

/**
 * Classe principal que gerencia o estado do jogo
 */
class Game {
private:
    static Game* instance;  // Singleton
    
    Player* player;
    AlienSwarm* swarm;
    
    GameState state;
    int score;
    
    // Dimensões da janela
    int windowWidth;
    int windowHeight;
    
    // Estado das teclas
    bool keyState[256];
    bool specialKeyState[256];
    
    Mix_Music* menuMusic;
    Mix_Music* backgroundMusic;
    Mix_Chunk* shootSound;
    Mix_Chunk* explosionSound;

public:
    Game(int width, int height);
    ~Game();
    
    // Singleton pattern
    static Game* getInstance();
    static void setInstance(Game* game);
    
    // Inicialização
    void init();
    void initAudio();
    void cleanupAudio();
    void restart();
    
    // Loop principal
    void update();
    void draw();
    
    // Detecção de colisões
    void checkCollisions();
    
    // Verifica condições de vitória/derrota
    void checkGameState();
    
    // Desenha placar e vidas
    void drawHUD();
    
    // Desenha mensagens de fim de jogo
    void drawMenu();
    void drawGameOver();
    void drawWin();
    
    // Controle de teclado
    void handleKeyDown(unsigned char key);
    void handleKeyUp(unsigned char key);
    void handleSpecialKeyDown(int key);
    void handleSpecialKeyUp(int key);
    
    // Processa input do jogador
    void processInput();
    
    // Getters
    GameState getState() const { return state; }
    int getScore() const { return score; }
    Player* getPlayer() { return player; }
    
    // Atualiza dimensões da janela
    void updateWindowSize(int width, int height);
    
    // Adiciona pontos
    void addScore(int points);
    
    // Sons
    void playShootSound();
    void playExplosionSound();
};

#endif // GAME_H
