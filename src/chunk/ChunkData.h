//
// Created by egorv on 5/17/2023.
//

#ifndef SHITCRAFT_CHUNKDATA_H
#define SHITCRAFT_CHUNKDATA_H

#include "block/BlockRegistry.h"

#include "ChunkMeshData.h"
#include "ChunkBlocks.h"
#include "ChunkMeshDataBuilder.h"

class ChunkData {
public:
    ChunkData() = default;

    [[nodiscard]] inline BlockId getBlock(glm::ivec3 relPosition) const {
        return blocks.getLocalUnchecked(relPosition);
    }

    inline void setBlock(glm::ivec3 relPosition, BlockId block) {
        blocks.setLocalUnchecked(relPosition, block);
    }

    inline void updateNeighborData(int thisChunkFace, const ChunkData &neighbor) {
        blocks.updateNeighborData(thisChunkFace, neighbor.blocks);
    }

    inline void updateMesh(const BlockRegistry &blockRegistry) {
        meshData = ChunkMeshDataBuilder(blocks, blockRegistry).build();
    }

    [[nodiscard]] inline const ChunkMeshData &getMeshData() const {
        return meshData;
    }

private:
    ChunkBlocks blocks;
    ChunkMeshData meshData;
};


#endif //SHITCRAFT_CHUNKDATA_H
