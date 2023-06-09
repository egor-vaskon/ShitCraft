//
// Created by egorv on 5/17/2023.
//

#ifndef SHITCRAFT_CHUNKMESHDATABUILDER_H
#define SHITCRAFT_CHUNKMESHDATABUILDER_H

#include <array>
#include <vector>

#include "chunk/ChunkVertex.h"
#include "registry/BlockRegistry.h"

#include "Constants.h"
#include "ChunkMeshData.h"
#include "ChunkBlocks.h"

class ChunkMeshDataBuilder {
public:
    ChunkMeshDataBuilder(const ChunkBlocks &blocks, const BlockRegistry &blockCache);

    ChunkMeshData build();
private:
    /**
     * @param originBlockOffset block offset in the direction of positive normal axis
     * @param face block face
     * */
    void build2dMesh(int originBlockOffset, int face);

    [[nodiscard]] inline BlockFace getFace(glm::ivec3 blockCoords, int face) {
        const auto block = blocks.getLocalUnchecked(blockCoords);
        return ((block == 0) ? BlockFace() : blockCache.getBlock(block).getFace(face));
    }

    [[nodiscard]] inline bool hasTranslucentNeighbour(glm::ivec3 coords, int face) {
        return blocks.getAdjacentTo(coords, face) == 0;
    }

    [[nodiscard]] int getVertexAO(glm::ivec3 block, int face, int vertex);

    [[nodiscard]] bool hasEqualAO(glm::ivec3 block, int face, int ao);

    [[nodiscard]] static constexpr int getVertexAO(bool side1, bool side2, bool corner) {
        return (side1 && side2) ? 0 : (3 - (side1 + side2 + corner));
    }

    const ChunkBlocks &blocks;
    const BlockRegistry &blockCache;

    ChunkMeshData meshData;
};


#endif //SHITCRAFT_CHUNKMESHDATABUILDER_H
