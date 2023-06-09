//
// Created by egorv on 4/26/2023.
//

#include "renderer/ChunkRenderer.h"
#include "camera/CameraControls.h"
#include "WorldScene.h"
#include "GameContext.h"

constexpr double P_FOV_RAD = glm::pi<double>() / 3.0;
constexpr double P_NEAR = 0.1;
constexpr double P_FAR = 1000.0;

constexpr float P_FOV_RAD_F = static_cast<float>(P_FOV_RAD);
constexpr float P_NEAR_F = static_cast<float>(P_NEAR);
constexpr float P_FAR_F = static_cast<float>(P_FAR);

WorldScene::WorldScene() {
    auto cameraControls = std::make_shared<CameraControls>(camera);

    scheduleForUpdates(cameraControls);
    addEventConsumer(cameraControls);

    world.setActiveRegion(glm::ivec3(0, 0, 0), glm::ivec3(24, 10, 24));
    renderer.resetActiveChunks(world.getActiveRegionMin(), world.getActiveRegionSize());

    camera.moveAbsolute(
            glm::dvec3(2 * world.getActiveRegionMin() + world.getActiveRegionSize() - 1) / 2.0 * CHUNK_SIDE_SCALE);

    world.setPlayerPosition(camera.getPosition() / BLOCK_SIDE_SCALE);
}

void WorldScene::onPreDraw() {
    Perspective perspective{winAspectRatio, P_FOV_RAD, P_NEAR, P_FAR};
    ViewFrustrum frustrum(perspective, glm::vec3(0), camera.getFront(), camera.getRight());

    const auto view = camera.getViewMatrix();

    RenderState state{frustrum, view, projMat, camera.getPosition()};
    renderer.draw(state);
}

void WorldScene::onUpdate(uint64_t deltaMs) {
    world.processPlayerPositionChange(camera.getPosition() / BLOCK_SIDE_SCALE);
    world.dispatchChunkLoads();

    renderer.setActiveChunksMin(world.getActiveRegionMin());

    while (true) {
        auto meshUpdateReq = world.dequeueMeshUpdateRequest();

        if (meshUpdateReq.has_value()) {
            auto req = meshUpdateReq.value();
            renderer.updateChunk(req.position, req.chunk ? &req.chunk->getMeshData() : nullptr);
        } else
            break;
    }
}

bool WorldScene::onHandleEvent(const SDL_Event &event) {
    if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
        auto winWidth = static_cast<double>(event.window.data1), winHeight = static_cast<double>(event.window.data2);
        winAspectRatio = winWidth / winHeight;

        viewportSize = glm::vec2(winWidth, winHeight);
        renderer.resize(viewportSize);

        projMat = glm::perspective(P_FOV_RAD_F, static_cast<float>(winAspectRatio), P_NEAR_F, P_FAR_F);
    }

    if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button == SDL_BUTTON_LEFT)
            world.setTargetBlockAndQueueUpdate(camera.getPosition() / BLOCK_SIDE_SCALE, camera.getFront(), 0);
        else if (event.button.button == SDL_BUTTON_RIGHT)
            world.setTargetBlockAndQueueUpdate(camera.getPosition() / BLOCK_SIDE_SCALE, camera.getFront(), 3);
    }

    return false;
}