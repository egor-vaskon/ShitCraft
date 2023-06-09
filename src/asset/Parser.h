//
// Created by egorv on 4/23/2023.
//

#ifndef SHITCRAFT_PARSER_H
#define SHITCRAFT_PARSER_H

#include <filesystem>
#include <vector>

#include "nlohmann/json.hpp"
#include "nlohmann/json-schema.hpp"

#include "Index.h"
#include "FileReader.h"

namespace asset {
    namespace detail {
        struct ParserState {
            explicit ParserState(std::string_view rootDir);

            [[nodiscard]] ParserState append(std::string_view nameSuffix, std::string_view subDir) const;

            std::filesystem::path directory;
            std::string parentName;
        };

        struct AssetDescription {
            std::string name;
        };
    }

    class Parser {
    public:
        Parser(Index &index, FileReader &reader, std::string_view schemaPath);

        void parse(std::string_view rootDir = ".", std::string_view rootFilename = "assets.json");

    private:
        detail::AssetDescription file(const detail::ParserState &state, std::string_view filename = "assets.json");

        detail::AssetDescription asset(const detail::ParserState &state, const nlohmann::json &root);

        void group(detail::ParserState &&state, const nlohmann::json &root);

        void text(detail::ParserState &&state, const nlohmann::json &root);

        void image(detail::ParserState &&state, const nlohmann::json &root);

        void shader(detail::ParserState &&state, const nlohmann::json &root);

        void block(detail::ParserState &&state, const nlohmann::json &root);

        BlockFace blockFace(const detail::ParserState &state, const nlohmann::json &root);

        std::string resolveRef(const detail::ParserState &state, const nlohmann::json &value);

        nlohmann::json_schema::json_validator validator;

        Index &index;
        FileReader &reader;
    };
} // asset

#endif //SHITCRAFT_PARSER_H
