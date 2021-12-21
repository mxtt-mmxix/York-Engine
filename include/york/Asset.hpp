#if !defined(YORK_ASSET_HPP)
#define YORK_ASSET_HPP

#include <filesystem>
#include <memory>
#include <functional>
#include <string>

#include "Async.hpp"

namespace york {

/**
 * An Asset is any file that can be loaded by the engine.
 */
class Asset {
public:

    enum class Type {
        IMG_PNG,
        SHADER_FRAG_GLSL,
        SHADER_FRAG_SPIRV,
        SHADER_VERT_GLSL,
        SHADER_VERT_SPIRV,
        SHADER_TESC_GLSL,
        SHADER_TESC_SPIRV,
        SHADER_TESE_GLSL,
        SHADER_TESE_SPIRV,
        SHADER_GEOM_GLSL,
        SHADER_GEOM_SPIRV,
        SHADER_COMP_GLSL,
        SHADER_COMP_SPIRV,
        TOML,
        UTF8,
        RAW,
        AUTO
    };

    /**
     * Creates an Asset handle, but does not load it.
     *
     * @param path A relative or absolute filepath to the asset.
     * @param type The type of asset
     */
    explicit Asset(const std::string& path, Type type = Type::AUTO);
    [[nodiscard]] Type getType() const;
    std::reference_wrapper<std::vector<char>> getData();
    static std::reference_wrapper<std::vector<char>> getDataStatic(Asset asset);

    std::vector<char>& operator*();
    std::vector<char>* operator->();

    char& operator[](std::size_t idx);

    ~Asset();

private:
    std::filesystem::path m_filepath;
    Type m_type;
    std::vector<char>& m_data;

};

} // namespace york::asset

#endif // YORK_ASSET_HPP
