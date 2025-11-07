#include "Game.h"
#include <GL/glut.h>
#include <stdio.h>
#include <string>

// Inicialização do singleton
Game* Game::instance = nullptr;

/**
 * Construtor do jogo
 */
Game::Game(int width, int height)
    : player(nullptr), swarm(nullptr), 
      state(PLAYING), score(0),
      windowWidth(width), windowHeight(height),
      backgroundMusic(nullptr), shootSound(nullptr), explosionSound(nullptr) {
    
    // Inicializa estado das teclas
    for (int i = 0; i < 256; i++) {
        keyState[i] = false;
        specialKeyState[i] = false;
    }
}

Game::~Game() {
    if (player != nullptr) {
        delete player;
    }
    if (swarm != nullptr) {
        delete swarm;
    }
    cleanupAudio();
}

/**
 * Retorna a instância do jogo (Singleton)
 */
Game* Game::getInstance() {
    return instance;
}

/**
 * Define a instância do jogo
 */
void Game::setInstance(Game* game) {
    instance = game;
}

/**
 * Inicializa o jogo
 */
void Game::init() {
    // Inicializa o sistema de áudio
    initAudio();
    
    // Cria o jogador no centro inferior
    player = new Player(windowWidth / 2, 50, 30);
    
    // Cria o enxame de alienígenas (5 linhas x 10 colunas)
    float startX = 100;
    float startY = windowHeight - 100;
    float spacing = 60;
    swarm = new AlienSwarm(5, 10, startX, startY, spacing);
    
    state = PLAYING;
    score = 0;
}

/**
 * Inicializa o sistema de áudio SDL2_mixer
 */
void Game::initAudio() {
    // Inicializa SDL áudio
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf("Erro ao inicializar SDL: %s\n", SDL_GetError());
        return;
    }
    
    // Inicializa SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Erro ao inicializar SDL_mixer: %s\n", Mix_GetError());
        return;
    }
    
    // Carrega música de fundo
    backgroundMusic = Mix_LoadMUS("assets/music.mp3");
    if (backgroundMusic == nullptr) {
        printf("Erro ao carregar música: %s\n", Mix_GetError());
    } else {
        // Toca música em loop infinito
        Mix_PlayMusic(backgroundMusic, -1);
        Mix_VolumeMusic(64); // Volume médio (0-128)
    }
    
    // Carrega efeitos sonoros
    shootSound = Mix_LoadWAV("assets/shoot.wav");
    if (shootSound == nullptr) {
        printf("Erro ao carregar som de tiro: %s\n", Mix_GetError());
    }
    
    explosionSound = Mix_LoadWAV("assets/explosion.wav");
    if (explosionSound == nullptr) {
        printf("Erro ao carregar som de explosão: %s\n", Mix_GetError());
    }
}

/**
 * Limpa recursos de áudio
 */
void Game::cleanupAudio() {
    // Para a música
    Mix_HaltMusic();
    
    // Libera recursos
    if (backgroundMusic != nullptr) {
        Mix_FreeMusic(backgroundMusic);
        backgroundMusic = nullptr;
    }
    if (shootSound != nullptr) {
        Mix_FreeChunk(shootSound);
        shootSound = nullptr;
    }
    if (explosionSound != nullptr) {
        Mix_FreeChunk(explosionSound);
        explosionSound = nullptr;
    }
    
    // Fecha SDL_mixer e SDL
    Mix_CloseAudio();
    SDL_Quit();
}

/**
 * Atualiza o estado do jogo
 */
void Game::update() {
    if (state != PLAYING) {
        return; // Não atualiza se o jogo terminou
    }
    
    // Processa input do jogador
    processInput();
    
    // Atualiza entidades
    player->update();
    swarm->update();
    
    // Verifica colisões
    checkCollisions();
    
    // Verifica condições de vitória/derrota
    checkGameState();
}

/**
 * Desenha tudo na tela
 */
void Game::draw() {
    // Desenha entidades
    if (player != nullptr) {
        player->draw();
    }
    
    if (swarm != nullptr) {
        swarm->draw();
    }
    
    // Desenha HUD
    drawHUD();
    
    // Desenha mensagens de fim de jogo
    if (state == GAME_OVER) {
        drawGameOver();
    } else if (state == WIN) {
        drawWin();
    }
}

/**
 * Processa input do jogador
 */
void Game::processInput() {
    if (player == nullptr || !player->isAlive()) {
        return;
    }
    
    // Movimentação
    if (specialKeyState[GLUT_KEY_LEFT]) {
        player->moveLeft();
    }
    if (specialKeyState[GLUT_KEY_RIGHT]) {
        player->moveRight();
    }
    
    // Disparo
    if (keyState[' ']) {
        if (player->canShoot()) {
            player->shoot();
            playShootSound(); // Toca som de tiro
        }
    }
}

/**
 * Verifica colisões
 */
void Game::checkCollisions() {
    if (player == nullptr || swarm == nullptr) {
        return;
    }
    
    // Tiro do jogador vs Aliens
    Projectile* playerShot = player->getShot();
    if (playerShot != nullptr && playerShot->isActive()) {
        const std::vector<Alien*>& aliens = swarm->getAliens();
        
        for (auto alien : aliens) {
            if (playerShot->checkCollision(alien)) {
                // Alien foi atingido
                playExplosionSound(); // Toca som de explosão
                int points = swarm->removeAlien(alien);
                addScore(points);
                playerShot->setActive(false);
                break; // Sai do loop após primeira colisão
            }
        }
    }
    
    // Colisão: Tiros dos aliens vs. Jogador
    std::vector<Projectile*>& alienShots = swarm->getShots();
    for (auto shot : alienShots) {
        if (shot->isActive() && shot->checkCollision(player)) {
            // Jogador foi atingido
            player->loseLife();
            shot->setActive(false);
            break; // Sai do loop após primeira colisão
        }
    }
}

/**
 * Verifica condições de vitória/derrota
 */
void Game::checkGameState() {
    if (state != PLAYING) {
        return;
    }
    
    // Verifica derrota
    if (player == nullptr || !player->isAlive()) {
        state = GAME_OVER;
        return;
    }
    
    // Verifica se aliens chegaram ao jogador
    if (swarm != nullptr && swarm->reachedPlayer(player->getY())) {
        state = GAME_OVER;
        return;
    }
    
    // Verifica vitória
    if (swarm != nullptr && swarm->isEmpty()) {
        state = WIN;
        return;
    }
}

/**
 * Desenha o HUD (placar e vidas)
 */
void Game::drawHUD() {
    glColor3f(1.0f, 1.0f, 1.0f); // Branco
    
    // Desenha score
    glRasterPos2f(10, windowHeight - 20);
    std::string scoreText = "Score: " + std::to_string(score);
    for (char c : scoreText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
    
    // Desenha vidas
    glRasterPos2f(10, windowHeight - 45);
    std::string livesText = "Lives: " + std::to_string(player ? player->getLives() : 0);
    for (char c : livesText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

/**
 * Desenha mensagem de Game Over
 */
void Game::drawGameOver() {
    glColor3f(1.0f, 0.0f, 0.0f); // Vermelho
    
    const char* text = "GAME OVER";
    int textWidth = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, 
                                     (const unsigned char*)text);
    
    glRasterPos2f((windowWidth - textWidth) / 2, windowHeight / 2);
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
    
    // Pontuação final
    glColor3f(1.0f, 1.0f, 1.0f);
    std::string finalScore = "Final Score: " + std::to_string(score);
    glRasterPos2f(windowWidth / 2 - 80, windowHeight / 2 - 40);
    for (char c : finalScore) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

/**
 * Desenha mensagem de vitória
 */
void Game::drawWin() {
    glColor3f(0.0f, 1.0f, 0.0f); // Verde
    
    const char* text = "YOU WIN!";
    int textWidth = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, 
                                     (const unsigned char*)text);
    
    glRasterPos2f((windowWidth - textWidth) / 2, windowHeight / 2);
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
    
    // Pontuação final
    glColor3f(1.0f, 1.0f, 1.0f);
    std::string finalScore = "Final Score: " + std::to_string(score);
    glRasterPos2f(windowWidth / 2 - 80, windowHeight / 2 - 40);
    for (char c : finalScore) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

/**
 * Adiciona pontos ao placar
 */
void Game::addScore(int points) {
    score += points;
}

/**
 * Handlers de teclado
 */
void Game::handleKeyDown(unsigned char key) {
    keyState[key] = true;
}

void Game::handleKeyUp(unsigned char key) {
    keyState[key] = false;
}

void Game::handleSpecialKeyDown(int key) {
    specialKeyState[key] = true;
}

void Game::handleSpecialKeyUp(int key) {
    specialKeyState[key] = false;
}

/**
 * Toca som de tiro
 */
void Game::playShootSound() {
    if (shootSound != nullptr) {
        Mix_PlayChannel(-1, shootSound, 0);
    }
}

/**
 * Toca som de explosão
 */
void Game::playExplosionSound() {
    if (explosionSound != nullptr) {
        Mix_PlayChannel(-1, explosionSound, 0);
    }
}
