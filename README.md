# Cpp_TileMapEditor
A simple tile map editor crated in C++ using [SFML](https://www.sfml-dev.org/index.php) and [Niel Lohmanns json](https://github.com/nlohmann/json).
The project was created inside [4coder](http://4coder.net/) and was compiled using [GNU Make](https://www.gnu.org/).
The repo contains a windows .exe inside the source folder for windows users, otherwise you will need to compile it yourself.

# Instructions
Before making any tilemaps to add your own images to paint with simply add any .png files into the SpriteSheets folder and then they will be usable in the program.

## Creating A New Tile Map
First enter a name for the tile map followed by the name of the png file you want to use as the palette. Then enter how many rows and columns the map will use and finally enter the pixel width and height of the tiles in the png file. Then click load, this should open a scaled black box in the painting area and a scaled version of your png image in the selection area.
![Creation](/Images/4.png=100x20)

### Painting 
Simply left click on a tile in the selection area to select it and then left click/drag to paint tiles in the painting view.

### Erasing
Right click/drag to erase any tiles in the painting area.

![Epic Tile Map](/Images/3.png)

## Saving A Tile Map
Click the save button and the tile map will be saved in json format to the TileMaps folder.

## Loading A Tile Map
To load a tile map all you have to do is enter the name of the tile map and click load, if the tile map loads successfully it will automatically fill in the sheet name, rows, cols, width and height.

### Before load:
![Creation](/Images/1.png)
### After load:
![Creation](/Images/2.png)
