#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb/stb_image.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>

#include "animated_sprite.h"
#include "sound.h"
#include "hud.h"
#include "rectangle_shape.h"
#include "resource_manager.h"
#include "scene.h"
#include "song_note.h"
#include "sprite.h"
#include "text_renderer.h"
#include "timed_dispatcher.h"
#include "game_config.h"

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

    Rect inputBounds(track.getBounds().left, track.getBounds().height - 15, track.getBounds().width, track.getBounds().height);

    // Flames
    Texture2D flamesTexture = ResourceManager::LoadTexture("resources/img/flames.png", "flames");
    std::vector<AnimatedSprite*> flames;

    for (int i = 0; i < 5; i++) {
        AnimatedSprite* flame = new AnimatedSprite(flamesTexture);
        flame->setSize(40, 40);
        flame->setOrigin(flame->getSize() / 2.0f);
        flame->setPosition(track.getBounds().left + i * 45 + 10, track.getSize().y - 30);
        flame->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.8f));
        flame->setProjection(*track.getProjection(), true);
        for (int flameOffset = 0; flameOffset < 8; flameOffset++) {
            flame->addFrame(Frame(0.01f + flameOffset * 0.005f, flame->getSize().x / 8, flame->getSize().y - 1, flameOffset));
        }
        flames.push_back(flame);
    }

    // Song information
    std::string selectedSongFolder = "resources/music/" + selectedSong + "/";
    std::string notesFilename;
    int pixelsPerSecond;

    switch(selectedDifficulty) {
        case 0:
            pixelsPerSecond = 400;
            notesFilename = selectedSongFolder + "seq1.txt";
            break;
        case 1:
            pixelsPerSecond = 500;
            notesFilename = selectedSongFolder + "seq2.txt";
            break;
        case 2:
            pixelsPerSecond = 600;
            notesFilename = selectedSongFolder + "seq3.txt";
            break;
        case 3:
            pixelsPerSecond = 700;
            notesFilename = selectedSongFolder + "seq4.txt";
            break;
    }

    const float timeToReachDetector = inputBounds.top / pixelsPerSecond;

    // Load notes
    std::vector<SongNote> notes;
    TimedDispatcher<SongNote> noteDispatcher;
    noteDispatcher.setDispatchDelay(-timeToReachDetector);

    std::ifstream file(notesFilename);
    //std::ifstream file("resources/music/seq.txt");
    std::string line;
    while (getline(file, line)) {
        std::istringstream iss(line);
        double noteStart, noteDuration;
        int noteValue;
        iss >> noteStart >> noteDuration >> noteValue;
        unsigned int tailLength = noteDuration - 1.0;

        if (noteValue & 1) noteDispatcher.add(noteStart, SongNote(track, 1, noteDuration * pixelsPerSecond));
        if (noteValue & 2) noteDispatcher.add(noteStart, SongNote(track, 2, noteDuration * pixelsPerSecond));
        if (noteValue & 4) noteDispatcher.add(noteStart, SongNote(track, 4, noteDuration * pixelsPerSecond));
        if (noteValue & 8) noteDispatcher.add(noteStart, SongNote(track, 8, noteDuration * pixelsPerSecond));
        if (noteValue & 16) noteDispatcher.add(noteStart, SongNote(track, 16, noteDuration * pixelsPerSecond));
    }
    file.close();

    // Load song
    Sound background((selectedSongFolder + "background.ogg").c_str());
    Sound song((selectedSongFolder + "song.ogg").c_str());
    background.play();
    song.play();

    // Song timer
    TextRenderer timerText(windowWidth, windowHeight);
    timerText.Load("resources/fonts/digital-7.ttf", 30);
    const double timerStart = glfwGetTime();
    double timerCurrent = timerStart;

    while (!glfwWindowShouldClose(window)) {
        glViewport(0, 0, windowWidth, windowHeight);
        glfwPollEvents();

        // Update the amount of pixels objects shoud move
        double timerPrevious = timerCurrent;
        timerCurrent = glfwGetTime() - timerStart;
        float elapsetTime = timerCurrent - timerPrevious;
        int pixelsPerFrame = elapsetTime * pixelsPerSecond;

        // Exit on ESC
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            Sound::stopAll();
            return SceneMusicSelector;
        }

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

        // Timer
        std::stringstream timerString;
        timerString << std::fixed << std::setprecision(2) << timerCurrent;
        timerText.RenderText(timerString.str(), 70.0f, 20.0f, 1.0f);
        
        // HUD
        hud.draw(window);

        // Track
        Rect trackTextureRect = track.getTextureRect();
        float wtfFactor = pixelsPerFrame * 0.92;
        trackTextureRect.top -= wtfFactor;
        trackTextureRect.height -= wtfFactor;
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
        std::vector<SongNote> newNotes = noteDispatcher.get(timerCurrent);
        notes.insert(notes.begin(), newNotes.begin(), newNotes.end());
        
        // Process input
        for (auto note = notes.begin(); note != notes.end(); ) {
            // Before detector
            if (note->isBefore(inputBounds)) {
                ++note;
            // On detector
            } else if (note->intersects(inputBounds) && note->checkInput(input)) {
                // Long note
                if (note->hasTail()) {
                    note->update(inputBounds.top - 5);
                    ++note;
                // Short note
                } else {
                    hud.incrementPerformance();
                    hud.incrementStreak();
                    hud.addPoints(2);
                    flames[note->getIndex()]->resetAnimation();
                    note = notes.erase(note);
                }
            // Afer detector
            } else {
                note->disable();
                if (note->getBounds().top > track.getSize().y + 100) {
                    note = notes.erase(note);
                } else {
                    ++note;
                }
            }
        }

        // Draw notes
        for (auto note : notes) {
            note.move(pixelsPerFrame);
            note.draw(window);
        }

        // Flames
        for (auto& flame : flames) {
            flame->update();
            if (flame->isRunning()) {
                flame->draw(window);
            }
        }

        // ==========================================================
        // Switch buffers
        glfwSwapBuffers(window);
    }

    return SceneExit;
}