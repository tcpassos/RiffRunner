#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb/stb_image.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <thread>

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
#include "game_info.h"

// ======================================================================================
//      Keyboard input callback
// ======================================================================================
void key_callback_game(GLFWwindow* window, int key, int scancode, int action, int mods) {
    unsigned int* input = (unsigned int*)glfwGetWindowUserPointer(window);

    /* KEY MAPPING */
    // A = 00001 = 1
    // S = 00010 = 2
    // J = 00100 = 4
    // K = 01000 = 8
    // L = 10000 = 16

    if (key == GLFW_KEY_A && action == GLFW_PRESS) *input |= 1;
    if (key == GLFW_KEY_S && action == GLFW_PRESS) *input |= 2;
    if (key == GLFW_KEY_J && action == GLFW_PRESS) *input |= 4;
    if (key == GLFW_KEY_K && action == GLFW_PRESS) *input |= 8;
    if (key == GLFW_KEY_L && action == GLFW_PRESS) *input |= 16;

    if (key == GLFW_KEY_A && action == GLFW_RELEASE) *input &= ~1;
    if (key == GLFW_KEY_S && action == GLFW_RELEASE) *input &= ~2;
    if (key == GLFW_KEY_J && action == GLFW_RELEASE) *input &= ~4;
    if (key == GLFW_KEY_K && action == GLFW_RELEASE) *input &= ~8;
    if (key == GLFW_KEY_L && action == GLFW_RELEASE) *input &= ~16;
}

// ======================================================================================
//      Game scene
// ======================================================================================
SceneId acceptGame(GLFWwindow* window) {
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    // Game input callback
    unsigned int input = 0;
    glfwSetKeyCallback(window, key_callback_game);
    glfwSetWindowUserPointer(window, &input);

    // Game background
    Texture2D backgroundTexture = ResourceManager::LoadTexture("resources/img/game_background.jpg", "gameBackground");
    Sprite backgroundImage(backgroundTexture);
    backgroundImage.setSize(glm::vec3(windowWidth, windowHeight, 1.0f));
    Texture2D backgroundFrontTexture = ResourceManager::LoadTexture("resources/img/game_background_front.png", "gameBackgroundFront");
    Sprite backgroundImageFront(backgroundFrontTexture);
    backgroundImageFront.setSize(glm::vec3(windowWidth, windowHeight, 1.0f));

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
    Rect inputBounds(track.getBounds().left, track.getBounds().height - 40, track.getBounds().width, track.getBounds().height + 15);

    std::vector<RectangleShape> detectors;
    for (int i = 0; i < 5; i++) {
        float detectorWidth = track.getSize().x / 5;
        float detectorHeight = (inputBounds.height - inputBounds.top) * 0.2;
        float detectorPosition = track.getBounds().left + detectorWidth * i + detectorWidth / 2;
        RectangleShape detector(track.getSize().x, track.getSize().y);
        detector.setSize(detectorWidth, detectorHeight);
        detector.setOrigin(detector.getSize().x / 2.0f, detector.getSize().y);
        detector.setPosition(detectorPosition, inputBounds.height - 30);
        detector.setProjection(*track.getProjection());
        detectors.push_back(detector);
    }
    detectors[0].setColor(glm::vec4(0.0f, 1.0f, 0.0f, 0.3f));
    detectors[1].setColor(glm::vec4(1.0f, 0.0f, 0.0f, 0.3f));
    detectors[2].setColor(glm::vec4(1.0f, 1.0f, 0.0f, 0.3f));
    detectors[3].setColor(glm::vec4(0.0f, 0.0f, 1.0f, 0.3f));
    detectors[4].setColor(glm::vec4(1.0f, 0.65f, 0.0f, 0.3f));

    // Flames
    Texture2D flamesTexture = ResourceManager::LoadTexture("resources/img/flames.png", "flames");
    std::vector<AnimatedSprite*> flames;

    for (int i = 0; i < 5; i++) {
        AnimatedSprite* flame = new AnimatedSprite(flamesTexture);
        flame->setSize(40, 40);
        flame->setOrigin(flame->getSize() / 2.0f);
        flame->setPosition(track.getBounds().left + i * 45 + 10, track.getSize().y - 30);
        flame->setEffect(EffectShine);
        flame->setEffectIntensity(0.8f);
        flame->setEffectSpeed(3.0f);
        flame->setProjection(*track.getProjection(), true);
        for (int flameOffset = 0; flameOffset < 8; flameOffset++) {
            flame->addFrame(Frame(0.02f + flameOffset * 0.002f, flame->getSize().x / 8, flame->getSize().y - 1, flameOffset));
        }
        flames.push_back(flame);
    }

    // Sparkles
    Texture2D sparkleTexture = ResourceManager::LoadTexture("resources/img/sparkle.png", "sparkle");
    std::vector<Sprite*> sparkles;

    for (int i = 0; i < 5; i++) {
        Sprite* sparkle = new AnimatedSprite(sparkleTexture);
        sparkle->setSize(55, 15);
        sparkle->setOrigin(sparkle->getSize() / 2.0f);
        sparkle->setPosition(track.getBounds().left + i * 40 + 20, track.getSize().y - 22);
        sparkle->setOpacity(0.0f);
        sparkle->setEffect(EffectShine);
        sparkle->setEffectIntensity(1.2f);
        sparkle->setEffectSpeed(6.0f);
        sparkle->setProjection(*track.getProjection());
        sparkles.push_back(sparkle);
    }

    // Song information
    std::string selectedSongFolder = "resources/music/" + GameInfo::selectedSong + "/";
    std::string notesFilename;
    int pixelsPerSecond;

    switch(GameInfo::selectedDifficulty) {
        case 0: // Easy
            pixelsPerSecond = 450;
            notesFilename = selectedSongFolder + "seq1.txt";
            break;
        case 1: // Normal
            pixelsPerSecond = 500;
            notesFilename = selectedSongFolder + "seq2.txt";
            break;
        case 2: // Hard
            pixelsPerSecond = 550;
            notesFilename = selectedSongFolder + "seq3.txt";
            break;
        case 3: // Chuck Norris
            pixelsPerSecond = 600;
            notesFilename = selectedSongFolder + "seq4.txt";
            break;
    }

    const float timeToReachDetector = inputBounds.top / pixelsPerSecond;

    // Load notes
    std::vector<SongNote> notes;
    TimedDispatcher<SongNote> noteDispatcher;
    noteDispatcher.setDispatchDelay(-timeToReachDetector);

    std::ifstream file(notesFilename);
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

    // Number of notes / hit notes (used to calculate progress)
    GameInfo::noteCount = noteDispatcher.size();
    GameInfo::hitNotes = 0;

    // Load song
    Sound background((selectedSongFolder + "background.ogg").c_str());
    Sound song((selectedSongFolder + "song.ogg").c_str());
    Sound pluck("resources/sound/click02.wav");
    background.play();
    song.play();

    // FPS/Timer text
    TextRenderer textRenderer(windowWidth, windowHeight);
    textRenderer.load("resources/fonts/digital-7.ttf", 30);
    textRenderer.setHorizontalAlignment(TextLeft);
    double offsetTime = glfwGetTime();
    double currentTime = offsetTime;

// ======================================================================================
//      Main loop
// ======================================================================================

    while (!glfwWindowShouldClose(window)) {
        glViewport(0, 0, windowWidth, windowHeight);
        glfwPollEvents();

        // Update the amount of pixels objects shoud move
        double previousTime = currentTime;
        currentTime = glfwGetTime() - offsetTime;
        float frameTime = currentTime - previousTime;
        float pixelsPerFrame = frameTime * pixelsPerSecond;
        int framesPerSecond = 1 / frameTime;

        // Activate special
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            hud.activateSpecial();
        }

        // Pause
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            song.pause();
            background.pause();
            double pauseTime = glfwGetTime();

            SceneId nextScene = acceptPause(window);
            if (nextScene != SceneCurrent) {
                Sound::stopAll();
                return nextScene;
            }

            offsetTime += (glfwGetTime() - pauseTime);
            song.play();
            background.play();
            // Re-bind key callback
            glfwSetKeyCallback(window, key_callback_game);
            glfwSetWindowUserPointer(window, &input);
        }

        // Fail
        if (hud.getPerformance() == 0) {
            Sound::stopAll();
            return SceneFailure;
        }

        // Finish
        if (song.isFinished()) {
            Sound::stopAll();
            GameInfo::finalScore = hud.getScore();
            return SceneResults;
        }

        // Put new notes on screen
        std::vector<SongNote> newNotes = noteDispatcher.get(currentTime);
        notes.insert(notes.begin(), newNotes.begin(), newNotes.end());

        // Process missclick notes
        unsigned int validInputMask = 0;
        for (auto note = notes.rbegin(); note != notes.rend(); ++note) {
            if (note->isBefore(inputBounds)) break;
            validInputMask |= note->getValue();
        }
        if ((input & ~validInputMask) != 0) { // missclick
            input = 0; // release all notes
            hud.decrementPerformance();
            hud.clearStreak();
            pluck.playOnce();
        }

// ======================================================================================
//      Input witchcraft
// ======================================================================================

        for (auto note = notes.begin(); note != notes.end(); ) {
            sparkles[note->getIndex()]->setOpacity(0.0f);

            /* ========= Note BEFORE detector ========= */
            if (note->isBefore(inputBounds)) {
                ++note;

            /* ========= Note ON detector ============= */
            } else if (note->intersects(inputBounds)) {
                if (note->checkInput(input)) { // If player hits the note

                    // ----------------------------------------------------------------
                    /* For sustained notes, it will gradually increment the score as the player holds it down.
                       The flame animation will trigger and the sprite with sparks will be displayed */
                    if (note->hasTail()) {
                        if (note->getState() == NoteUnpressed) {
                            GameInfo::hitNotes++;
                            flames[note->getIndex()]->resetAnimation();
                        }
                        hud.addPoints(note->hold(inputBounds.top + 15));
                        sparkles[note->getIndex()]->setOpacity(0.8f);
                        ++note;
                    // ----------------------------------------------------------------
                    } else {
                    /* For short notes that are pressed into the sensor,
                       the score will increase by 2 points (plus multiplication),
                       the flame animation will be reset, and the note and its sprite will be destroyed */
                        GameInfo::hitNotes++;
                        input &= ~note->getValue(); // Release input
                        hud.incrementPerformance();
                        hud.incrementStreak();
                        hud.addPoints(2);
                        flames[note->getIndex()]->resetAnimation();
                        note = notes.erase(note);
                    }
                    song.setVolume(100);
                } else {
                    ++note;
                }

            /* ========= Note AFTER detector ========== */
            } else {
                /* If the note has passed the sensor, it will
                   decrement the score and reset the multiplier only
                   if it is not a sustained note that has already been pressed. */
                if (note->getBounds().height > track.getSize().y) {
                    if (note->getState() != NoteDisabled) {
                        input &= ~note->getValue(); // Release input
                        note->disable();
                        if (!note->isConsumed()) {
                            hud.decrementPerformance();
                            hud.decrementPerformance();
                            hud.clearStreak();
                            song.setVolume(0);
                        }
                    }
                }
                /* The note is destroyed as soon as it leaves the screen. */
                if (note->getBounds().top > track.getSize().y + 100) {
                    note = notes.erase(note);
                } else {
                    ++note;
                }
            }
        }

// ======================================================================================
// Draw things
// ======================================================================================

        // Clear color buffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Background
        backgroundImage.draw(window);

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

        // Draw notes
        for (auto note : notes) {
            note.move(pixelsPerFrame);
            note.draw(window, hud.isSpecialActive());
        }

        // Sparkles
        for (auto& sparkle : sparkles) {
            sparkle->draw(window);
        }

        // Flames
        for (auto& flame : flames) {
            flame->update();
            if (flame->isRunning()) {
                flame->draw(window);
            }
        }

        // Background front
        backgroundImageFront.draw(window);

        // HUD
        hud.update(currentTime);
        hud.draw(window);

        // Timer
        std::stringstream timerString;
        timerString << "Timer: " << std::fixed << std::setprecision(2) << currentTime;
        textRenderer.renderText(timerString.str(), 10.0f, 10.0f);

        // FPS
        std::stringstream fpsString;
        fpsString << "FPS: " << std::fixed << framesPerSecond;
        textRenderer.renderText(fpsString.str(), 10.0f, 40.0f);

        // ==========================================================
        // Switch buffers
        glfwSwapBuffers(window);
    }

    return SceneExit;
}