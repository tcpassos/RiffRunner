#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb/stb_image.h>
#include <MidiFile.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <thread>

#include "animated_sprite.hpp"
#include "effects.h"
#include "game_info.h"
#include "hud.h"
#include "light_stripe.hpp"
#include "rectangle_shape.hpp"
#include "resource_manager.h"
#include "scene.h"
#include "song_note.h"
#include "sound.h"
#include "sprite.hpp"
#include "text_renderer.h"
#include "timed_dispatcher.h"

bool fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

void loadNotes(std::string filename, TimedDispatcher<SongNote> &noteDispatcher, int midiKeyOffeset, int pixelsPerSecond, Sprite &track) {
    smf::MidiFile midifile;
    if (!midifile.read(filename)) {
        std::cerr << "Error reading MIDI file" << std::endl;
        return;
    }

    midifile.doTimeAnalysis();
    midifile.linkNotePairs();

    const float scale = 0.9;
    
    for (int trackIndex = 0; trackIndex < midifile.getTrackCount(); trackIndex++) {
        for (int event = 0; event < midifile[trackIndex].size(); event++) {
            int key = midifile[trackIndex][event].getKeyNumber() - midiKeyOffeset;
            if (!midifile[trackIndex][event].isNoteOn() || (key < 0 || key > 4)) continue;

            double noteStart = midifile[trackIndex][event].seconds;
            double noteDuration = midifile[trackIndex][event].getDurationInSeconds();
            if (noteDuration < 0.5) noteDuration = 0.0;

            if (key == 0) noteDispatcher.add(noteStart, SongNote(track, 1, noteDuration * pixelsPerSecond * scale));
            else if (key == 1) noteDispatcher.add(noteStart, SongNote(track, 2, noteDuration * pixelsPerSecond * scale));
            else if (key == 2) noteDispatcher.add(noteStart, SongNote(track, 4, noteDuration * pixelsPerSecond * scale));
            else if (key == 3) noteDispatcher.add(noteStart, SongNote(track, 8, noteDuration * pixelsPerSecond * scale));
            else if (key == 4) noteDispatcher.add(noteStart, SongNote(track, 16, noteDuration * pixelsPerSecond * scale));
        }
        if (!noteDispatcher.isEmpty()) {
            break;
        }
    }
}

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
    Texture2D backgroundTexture = ResourceManager::loadTexture("assets/img/game_background.jpg", "gameBackground");
    Sprite backgroundImage(backgroundTexture);
    backgroundImage.setSize(glm::vec3(windowWidth, windowHeight, 1.0f));
    Texture2D backgroundFrontTexture = ResourceManager::loadTexture("assets/img/game_background_front.png", "gameBackgroundFront");
    Sprite backgroundImageFront(backgroundFrontTexture);
    backgroundImageFront.setSize(glm::vec3(windowWidth, windowHeight, 1.0f));

    // Performance indicator | Score | Special multiplier
    HUD hud;

    // Track
    Texture2D trackTexture = ResourceManager::loadTexture("assets/img/track.jpg", "track");
    Sprite track(trackTexture);
    track.setSize(windowWidth / 4, windowHeight);
    track.setOrigin(track.getSize().x / 2, track.getSize().y);
    track.setPosition(windowWidth / 2, windowHeight);
    track.setColor(glm::vec4(0.6, 0.6, 0.6, 0.6));
    track.getProjection()->setRotation(glm::vec3(glm::radians(-50.0f), 0.0f, 0.0f));

    // Fckin math
    if (windowHeight <= 800) {
        track.getProjection()->moveY(-100.0f);
        track.getProjection()->moveZ(-55.0f);
    }
    else if (windowHeight <= 1080) {
        track.getProjection()->moveY(-300.0f);
        track.getProjection()->moveZ(50.0f);
    }

    // Track lines
    std::vector<RectangleShape> trackLines;
    for (int i = 0; i < 5; i++) {
        float trackLinePosition = track.getBounds().left + (i * track.getSize().x / 5) + track.getSize().x / 10;
        RectangleShape trackLine(track.getSize().x, track.getSize().y);
        trackLine.setSize(1.0f, trackLine.getSize().y);
        trackLine.setOrigin(trackLine.getSize().x / 2, track.getSize().y);
        trackLine.setPosition(trackLinePosition, track.getPosition().y);
        trackLine.setOpacity(0.6f);
        trackLine.setProjection(*track.getProjection());
        trackLines.push_back(trackLine);
    }

    LightStripe trackLineNeonLeft;
    trackLineNeonLeft.setSize(track.getSize().y);
    trackLineNeonLeft.setPosition(track.getBounds().left, track.getBounds().height);
    trackLineNeonLeft.setProjection(*track.getProjection());
    trackLineNeonLeft.setRadius(0.003);

    LightStripe trackLineNeonRight;
    trackLineNeonRight.setSize(track.getSize().y);
    trackLineNeonRight.setPosition(track.getBounds().width, track.getBounds().height);
    trackLineNeonRight.setProjection(*track.getProjection());
    trackLineNeonRight.setRadius(0.003);

    float sideTrackLinesHitEffectTime = 0.0;

    // Input zone
    Rect inputBounds(track.getBounds().left, track.getBounds().height - 40, track.getBounds().width, track.getBounds().height + 15);

    // Fret buttons
    Texture2D fretButtonsTexture = ResourceManager::loadTexture("assets/img/fret_buttons.png", "fret_buttons");
    Sprite fretButtons(fretButtonsTexture);
    fretButtons.setSize(200.0, 10.0);
    fretButtons.setOrigin(0.0f, fretButtons.getSize().y);
    fretButtons.setPosition(track.getBounds().left, track.getBounds().height - 17);
    fretButtons.setProjection(*track.getProjection());

    Texture2D fretButtonGreenTexture = ResourceManager::loadTexture("assets/img/fret_button_green.png", "fret_button_green");
    Sprite fretButtonGreen(fretButtonGreenTexture);
    fretButtonGreen.setSize(fretButtons.getSize());
    fretButtonGreen.setOrigin(fretButtons.getOrigin());
    fretButtonGreen.setPosition(fretButtons.getPosition());
    fretButtonGreen.setProjection(*track.getProjection());

    Texture2D fretButtonRedTexture = ResourceManager::loadTexture("assets/img/fret_button_red.png", "fret_button_red");
    Sprite fretButtonRed(fretButtonRedTexture);
    fretButtonRed.setSize(fretButtons.getSize());
    fretButtonRed.setOrigin(fretButtons.getOrigin());
    fretButtonRed.setPosition(fretButtons.getPosition());
    fretButtonRed.setProjection(*track.getProjection());

    Texture2D fretButtonYellowTexture = ResourceManager::loadTexture("assets/img/fret_button_yellow.png", "fret_button_yellow");
    Sprite fretButtonYellow(fretButtonYellowTexture);
    fretButtonYellow.setSize(fretButtons.getSize());
    fretButtonYellow.setOrigin(fretButtons.getOrigin());
    fretButtonYellow.setPosition(fretButtons.getPosition());
    fretButtonYellow.setProjection(*track.getProjection());

    Texture2D fretButtonBlueTexture = ResourceManager::loadTexture("assets/img/fret_button_blue.png", "fret_button_blue");
    Sprite fretButtonBlue(fretButtonBlueTexture);
    fretButtonBlue.setSize(fretButtons.getSize());
    fretButtonBlue.setOrigin(fretButtons.getOrigin());
    fretButtonBlue.setPosition(fretButtons.getPosition());
    fretButtonBlue.setProjection(*track.getProjection());

    Texture2D fretButtonOrangeTexture = ResourceManager::loadTexture("assets/img/fret_button_orange.png", "fret_button_orange");
    Sprite fretButtonOrange(fretButtonOrangeTexture);
    fretButtonOrange.setSize(fretButtons.getSize());
    fretButtonOrange.setOrigin(fretButtons.getOrigin());
    fretButtonOrange.setPosition(fretButtons.getPosition());
    fretButtonOrange.setProjection(*track.getProjection());

    // Flames
    Texture2D flamesTexture = ResourceManager::loadTexture("assets/img/flames.png", "flames");
    std::vector<AnimatedSprite*> flames;

    for (int i = 0; i < 5; i++) {
        AnimatedSprite* flame = new AnimatedSprite(flamesTexture);
        flame->setSize(40, 40);
        flame->setOrigin(flame->getSize() / 2.0f);
        flame->setPosition(track.getBounds().left + i * 43 + 14, track.getBounds().height - 30);
        flame->setProjection(*track.getProjection(), true);

        EffectShine flameShine;
        flameShine.setIntensity(0.8);
        flameShine.setSpeed(3.0);
        flame->addEffect(&flameShine);

        for (int flameOffset = 0; flameOffset < 8; flameOffset++) {
            flame->addFrame(Frame(0.02f + flameOffset * 0.002f, flame->getSize().x / 8, flame->getSize().y - 1, flameOffset));
        }
        flames.push_back(flame);
    }

    // Sparkles
    Texture2D sparklesTexture = ResourceManager::loadTexture("assets/img/sparkles.png", "sparkles");
    std::vector<AnimatedSprite*> sparkles;

    for (int i = 0; i < 5; i++) {
        AnimatedSprite* sparkle = new AnimatedSprite(sparklesTexture);
        sparkle->setSize(40, 20);
        sparkle->setOrigin(sparkle->getSize() / 2.0f);
        sparkle->setPosition(track.getBounds().left + i * 42 + 17, track.getBounds().height - 25);
        sparkle->setProjection(*track.getProjection(), true);

        EffectShine sparkleShine;
        sparkleShine.setIntensity(1.0);
        sparkleShine.setSpeed(2.0);
        sparkle->addEffect(&sparkleShine);

        for (int sparkleOffset = 0; sparkleOffset < 5; sparkleOffset++) {
            sparkle->addFrame(Frame(0.02f + sparkleOffset * 0.002f, sparkle->getSize().x / 5, sparkle->getSize().y - 1, sparkleOffset));
        }
        sparkles.push_back(sparkle);
    }

    // Song information
    std::string selectedSongFolder = GameInfo::songs[GameInfo::selectedSong].path + "/";
    int pixelsPerSecond;
    int keyOffeset = 0;
    int midiKeyOffeset = 0;

    switch(GameInfo::selectedDifficulty) {
        case 0: // Easy
            pixelsPerSecond = 450;
            midiKeyOffeset = 60;
            break;
        case 1: // Normal
            pixelsPerSecond = 500;
            midiKeyOffeset = 72;
            break;
        case 2: // Hard
            pixelsPerSecond = 600;
            midiKeyOffeset = 84;
            break;
        case 3: // Chuck Norris
            pixelsPerSecond = 600;
            midiKeyOffeset = 96;
            break;
    }

    const float timeToReachDetector = inputBounds.top / pixelsPerSecond;

    // Load notes
    std::vector<SongNote> notes;
    TimedDispatcher<SongNote> noteDispatcher;
    noteDispatcher.setDispatchDelay(-timeToReachDetector);
    loadNotes(selectedSongFolder + "notes.mid", noteDispatcher, midiKeyOffeset, pixelsPerSecond, track);

    // Number of notes / hit notes (used to calculate progress)
    GameInfo::noteCount = noteDispatcher.size();
    GameInfo::hitNotes = 0;

    // Load song
    Sound pluck("assets/sound/click02.wav");
    Sound guitar((selectedSongFolder + "guitar.ogg").c_str());
    
    std::vector<Sound*> backgroundSounds;
    if (fileExists(selectedSongFolder + "song.ogg"))
        backgroundSounds.push_back(new Sound((selectedSongFolder + "song.ogg").c_str()));
    if (fileExists(selectedSongFolder + "rhythm.ogg"))
        backgroundSounds.push_back(new Sound((selectedSongFolder + "rhythm.ogg").c_str()));
    if (fileExists(selectedSongFolder + "vocals.ogg"))
        backgroundSounds.push_back(new Sound((selectedSongFolder + "vocals.ogg").c_str()));
    if (fileExists(selectedSongFolder + "drums_1.ogg"))
        backgroundSounds.push_back(new Sound((selectedSongFolder + "drums_1.ogg").c_str()));
    if (fileExists(selectedSongFolder + "drums_2.ogg"))
        backgroundSounds.push_back(new Sound((selectedSongFolder + "drums_2.ogg").c_str()));
    if (fileExists(selectedSongFolder + "drums_3.ogg"))
        backgroundSounds.push_back(new Sound((selectedSongFolder + "drums_3.ogg").c_str()));
    if (fileExists(selectedSongFolder + "drums_4.ogg"))
        backgroundSounds.push_back(new Sound((selectedSongFolder + "drums_4.ogg").c_str()));

    for (auto backgroundSound : backgroundSounds) {
        backgroundSound->play();
    }
    guitar.play();


    // FPS/Timer text
    TextRenderer textRenderer(windowWidth, windowHeight, Font("assets/fonts/digital-7.ttf", 30));
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
            for (auto backgroundSound : backgroundSounds) {
                backgroundSound->pause();
            }
            guitar.pause();
            double pauseTime = glfwGetTime();

            SceneId nextScene = acceptPause(window);
            if (nextScene != SceneCurrent) {
                Sound::stopAll();
                return nextScene;
            }

            offsetTime += (glfwGetTime() - pauseTime);
            for (auto backgroundSound : backgroundSounds) {
                backgroundSound->play();
            }
            guitar.play();
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
        if (guitar.isFinished()) {
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
                            sideTrackLinesHitEffectTime = 1.5;
                            flames[note->getIndex()]->resetAnimation();
                        }
                        hud.addPoints(note->hold(inputBounds.top + 10));
                        if (!sparkles[note->getIndex()]->isRunning()) {
                            sparkles[note->getIndex()]->resetAnimation();
                        }
                        ++note;
                    // ----------------------------------------------------------------
                    } else {
                    /* For short notes that are pressed into the sensor,
                       the score will increase by 2 points (plus multiplication),
                       the flame animation will be reset, and the note and its sprite will be destroyed */
                        GameInfo::hitNotes++;
                        sideTrackLinesHitEffectTime = 1.0;
                        input &= ~note->getValue(); // Release input
                        hud.incrementPerformance();
                        hud.incrementStreak();
                        hud.addPoints(2);
                        flames[note->getIndex()]->resetAnimation();
                        note = notes.erase(note);
                    }
                    guitar.setVolume(100);
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
                            guitar.setVolume(0);
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

        // Side track lines
        if (sideTrackLinesHitEffectTime > 0.0) {
            float glowIntensity = glm::mix(0.35f, 0.8f, 1.0f - glm::smoothstep(0.0f, 1.0f, glm::clamp(sideTrackLinesHitEffectTime, 0.0f, 1.0f)));
            trackLineNeonLeft.setIntensity(glowIntensity);
            trackLineNeonRight.setIntensity(glowIntensity);

            sideTrackLinesHitEffectTime -= frameTime;
            if (sideTrackLinesHitEffectTime < 0) sideTrackLinesHitEffectTime = 0;
        } else {
            trackLineNeonLeft.setIntensity(0.8);
            trackLineNeonRight.setIntensity(0.8);
        }
        if (hud.isSpecialActive()) {
            trackLineNeonLeft.setColor(glm::vec3(0.0, 0.85, 1.0));
            trackLineNeonRight.setColor(glm::vec3(0.0, 0.85, 1.0));
        } else {
            trackLineNeonLeft.setColor(glm::vec3(1.0, 1.0, 1.0));
            trackLineNeonRight.setColor(glm::vec3(1.0, 1.0, 1.0));
        }
        trackLineNeonLeft.draw(window);
        trackLineNeonRight.draw(window);

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

        // Fret buttons
        fretButtons.draw(window);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) fretButtonGreen.draw(window);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) fretButtonRed.draw(window);
        if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) fretButtonYellow.draw(window);
        if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) fretButtonBlue.draw(window);
        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) fretButtonOrange.draw(window);

        // Draw notes
        for (auto it = notes.rbegin(); it != notes.rend(); ++it) {
            SongNote& note = *it;
            note.move(pixelsPerFrame);
            note.draw(window, hud.isSpecialActive());
        }

        // Sparkles
        for (auto& sparkle : sparkles) {
            sparkle->update();
            if (sparkle->isRunning()) {
                sparkle->draw(window);
            }
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