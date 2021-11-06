#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

struct MapData
{
    MapData(const std::string& tileset, sf::Vector2u tileSize, std::vector<int>& tiles, unsigned int width, unsigned int height):name_(tileset), tileSize_(tileSize),tiles_(tiles),width_(width),height_(height)
    {
        
    }
    const std::string name_;
    sf::Vector2u tileSize_;
    std::vector<int>& tiles_;
    unsigned int width_;
    unsigned int height_;
    
};

class TileMap: public sf::Drawable, public sf::Transformable
{
    public:
    MapData mData;
    TileMap();
    bool load(MapData mData);
    bool load(const std::string& tileset, sf::Vector2u tileSize, std::vector<int>& tiles, unsigned int width, unsigned int height);
    
    private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
};