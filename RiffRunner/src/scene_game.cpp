#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb/stb_image.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "animation.h"
#include "sound.h"
#include "hud.h"
#include "rectangle_shape.h"
#include "resource_manager.h"
#include "scene.h"
#include "song_note.h"
#include "sprite.h"
#include "timed_dispatcher.h"

// ======================================================================================
// Game scene
// ======================================================================================
SceneId acceptGame(GLFWwindow* window) {
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    // Game background
    Texture2D backgroundTexture = ResourceManager::LoadTexture("resources/img/game_background.jpg", "gameBackground");
    Sprite backgroundImage(backgroundTexture);
    backgroundImage.setSize(glm::vec3(windowWidth, windowHeight, 1.0f));

    // Performance indicator | Score | Special multiplier
    HUD hud;

    // Track
    Texture2D trackTexture = ResourceManager::LoadTexture("resources/img/track.jpg", "track");
    Sprite track(trackTexture);
    track.setOrigin(track.getSize() / 2.0f);
    track.setPosition(windowWidth / 2, windowHeight / 2);
    track.getProjection()->setRotation(glm::vec3(glm::radians(-50.0f), 0.0f, 0.0f));
    track.getProjection()->moveY(-100.0f);
    track.getProjection()->moveZ(-55.0f);

    // Track lines
    std::vector<RectangleShape> trackLines;
    for (int i = 0; i < 6; i++) {
        float trackLinePosition = track.getBounds().left + (track.getSize().x / 5) * i;
        RectangleShape trackLine(track.getSize().x, track.getSize().y);
        trackLine.setSize(1.0f, trackLine.getSize().y);
        trackLine.setOrigin(trackLine.getSize() / 2.0f);
        trackLine.setPosition(trackLinePosition, track.getPosition().y);
        trackLine.setProjection(*track.getProjection());

        trackLines.push_back(trackLine);
    }

    // Input zone
    std::vector<RectangleShape> detectors;
    for (int i = 0; i < 5; i++) {
        float detectorWidth = track.getSize().x / 5;
        float detectorPosition = track.getBounds().left + detectorWidth * i + detectorWidth / 2;
        RectangleShape detector(track.getSize().x, track.getSize().y);
        detector.setSize(detectorWidth, 15.0f);
        detector.setOrigin(detector.getSize() / 2.0f);
        detector.setPosition(detectorPosition, track.getBounds().height - 20);
        detector.setProjection(*track.getProjection());
        detectors.push_back(detector);
    }
    detectors[0].setColor(glm::vec4(0.0f, 1.0f, 0.0f, 0.5f));
    detectors[1].setColor(glm::vec4(1.0f, 0.0f, 0.0f, 0.5f));
    detectors[2].setColor(glm::vec4(1.0f, 1.0f, 0.0f, 0.5f));
    detectors[3].setColor(glm::vec4(0.0f, 0.0f, 1.0f, 0.5f));
    detectors[4].setColor(glm::vec4(1.0f, 0.65f, 0.0f, 0.5f));
    Rect inputBounds(detectors[0].getBounds().left, detectors[0].getBounds().top, detectors[0].getBounds().width * 5, detectors[0].getBounds().height);

    // Flames
    Texture2D flamesTexture = ResourceManager::LoadTexture("resources/img/flames.png", "flames");
    Sprite flames(flamesTexture);
    flames.setSize(80.0f, 80.0f);
    flames.setPosition(210.0f, 450.0f);
    flames.setColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.8f));
    Animation flamesAnimation(flames);
    for (int flameOffset = 0; flameOffset < 8; flameOffset++) {
        flamesAnimation.addFrame(*(new Frame(10.0f + flameOffset*10.0f, flames.getSize().x / 8, flames.getSize().y, flameOffset)));
    }

    // Load notes
    std::vector<SongNote> notes;
    TimedDispatcher<SongNote> noteDispatcher;
    noteDispatcher.setDispatchDelay(-1.0);

    std::ifstream file("resources/music/Aerosmith - Dream On/seq1.txt");
    std::string line;
    while (getline(file, line)) {
        std::istringstream iss(line);
        double noteStart, noteDuration;
        int noteValue;
        iss >> noteStart >> noteDuration >> noteValue;
        noteDispatcher.add(noteStart, SongNote(track, noteValue, noteDuration));
    }
    file.close();

    // Load song
    Sound background("resources/music/Aerosmith - Dream On/background.ogg");
    Sound song("resources/music/Aerosmith - Dream On/song.ogg");
    background.play();
    song.play();

    // Start song timer
    const double timerStart = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        glViewport(0, 0, windowWidth, windowHeight);
        glfwPollEvents();

        // Exit on ESC
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // Score
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            hud.decrementPerformance();
            hud.clearStreak();
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            hud.incrementPerformance();
            hud.incrementStreak();
            hud.addPoints(1);
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            flamesAnimation.reset();
        }
        flamesAnimation.update();

        if (hud.getPerformance() == 0)
            return SceneFailure;

        // Update input
        unsigned int input = 0;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) input |= 1;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) input |= 2;
        if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) input |= 4;
        if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) input |= 8;
        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) input |= 16;

        // Clear color buffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // ==========================================================

        // Background
        backgroundImage.draw(window);
        
        // HUD
        hud.draw(window);

        // Track
        Rect trackTextureRect = track.getTextureRect();
        trackTextureRect.top--;
        trackTextureRect.height--;
        track.setTextureRect(trackTextureRect);        
        track.draw(window);

        // Track lines
        for (auto& trackLine : trackLines) {
            trackLine.draw(window);
        }

        // Detectors
        for (auto& detector : detectors) {
            detector.draw(window);
        }

        // Put new notes on screen
        double elapsedTime = glfwGetTime() - timerStart;
        std::vector<SongNote> newNotes = noteDispatcher.get(elapsedTime);
        notes.insert(notes.begin(), newNotes.begin(), newNotes.end());
        
        // Process input
        for (auto note = notes.begin(); note != notes.end(); ) {
            if (note->intersects(inputBounds)) {
                if (note->checkInput(input)) {
                    if (note->hasTail()) {
                        // TODO: Atualizar tamanho da nota
                        // TODO: Incrementar o streak/performance somente uma vez
                        hud.addPoints(1);
                        ++note;
                    } else {
                        note = notes.erase(note);
                        hud.incrementPerformance();
                        hud.incrementStreak();
                        hud.addPoints(2);
                    }
                } else {
                    ++note;
                }
            } else if (note->isAfter(inputBounds)) {
                note->disable();
                if (note->getBounds().top > inputBounds.height + 100) {
                    note = notes.erase(note);
                } else {
                    ++note;
                }
            }
            else {
                ++note;
            }
        }

        // Draw notes
        for (auto note : notes) {
            note.move(1);
            note.draw(window);
        }

        // Flames
        if (flamesAnimation.isRunning()) {
            flames.draw(window);
        }

        // ==========================================================
        // Switch buffers
        glfwSwapBuffers(window);
    }

    return SceneExit;
}