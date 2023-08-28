#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <wex.h>
#include "GraphTheory.h"
#include "cStarterGUI.h"

class cRoom
{
public:
    int x;
    int y;
    int w;
    int h;
    cRoom(int X, int Y, int W, int H)
        : x(X), y(Y), w(W), h(H)
    {
    }
    void print() const
    {
        std::cout << "room at " << x << "," << y << " size " << w << " by " << h << "\n";
    }
    void center(int &xc, int &yc) const
    {
        xc = x + w / 2;
        yc = y + h / 2;
    }
    int distance2(const cRoom &other) const
    {
        int x, y, xo, yo;
        center(x, y);
        other.center(xo, yo);
        int dx = x - xo;
        int dy = y - yo;
        return dx * dx + dy * dy;
    }
};

class cCorridor
{
public:
    int x1, y1, x2, y2;
    cCorridor(int xs, int ys, int xe, int ye) : x1(xs), y1(ys), x2(xe), y2(ye)
    {
    }
};

class cLevelGenerator1
{
public:
    //  GameObject floorPrefab;
    //  GameObject wallPrefab;
    //  GameObject corridorPrefab;
    int mapWidth = 100;
    int mapHeight = 100;
    int roomSizeMin = 5;
    int roomSizeMax = 15;
    int seed = 12345;

    cLevelGenerator1()
    {
        GenerateLevel();
    }

    std::vector<cRoom> getRooms()
    {
        return rooms;
    }
    std::vector<cCorridor> getCorridors()
    {
        return corridors;
    }
    raven::graph::cGraph getSpan()
    {
        return spanTree;
    }
    raven::graph::sGraphData &getGraphData()
    {
        return gd;
    }

private:
    std::vector<std::vector<bool>> usedTiles;
    std::vector<cRoom> rooms;
    std::vector<cCorridor> corridors;
    raven::graph::sGraphData gd;
    raven::graph::cGraph spanTree;

    void GenerateLevel()
    {
        // mapHolder = new GameObject("Generated Map").transform;
        usedTiles.resize(mapHeight, std::vector<bool>(mapWidth, false));

        new bool[mapWidth, mapHeight];

        srand(seed);

        int sizeRange = roomSizeMax - roomSizeMin;

        for (int x = 0; x < mapWidth; x++)
        {
            for (int y = 0; y < mapHeight; y++)
            {
                if (usedTiles[x][y])
                    continue;

                if (rand() % 100 < 80)
                    continue;

                cRoom room(
                    x, y,
                    rand() % sizeRange + roomSizeMin,
                    rand() % sizeRange + roomSizeMin);

                if (!IsSpaceAvailable(room))
                    continue;

                room.print();
                rooms.push_back(room);
                MarkTilesUsed(room);
            }
        }

        ConnectRooms();
    }

    bool IsSpaceAvailable(const cRoom &room)
    {
        for (int i = room.x; i < room.x + room.w; i++)
        {
            for (int j = room.y; j < room.y + room.h; j++)
            {
                if (i < 0 || i >= mapWidth || j < 0 || j >= mapHeight || usedTiles[i][j])
                {
                    return false;
                }
            }
        }
        return true;
    }

    void MarkTilesUsed(const cRoom &room)
    {
        for (int i = room.x; i < room.x + room.w; i++)
        {
            for (int j = room.y; j < room.y + room.h; j++)
            {
                usedTiles[i][j] = true;
            }
        }
    }

    void ConnectRooms()
    {

        for (int kr = 0; kr < rooms.size(); kr++)
        {
            int xc, yc;
            rooms[kr].center(xc, yc);
            gd.g.wVertexAttr(
                gd.g.add(
                    "V" + std::to_string(kr)),
                {std::to_string(xc),
                 std::to_string(yc)});
        }
        gd.edgeWeight.resize(
            2 * gd.g.vertexCount() * gd.g.vertexCount(), INT_MAX);
        for (int kr = 0; kr < rooms.size(); kr++)
            for (int kr2 = kr + 1; kr2 < rooms.size(); kr2++)
            {
                gd.edgeWeight[gd.g.add(kr, kr2)] =
                    rooms[kr].distance2(rooms[kr2]);
            }
        gd.startName = "V0";
        spanTree = spanningTree(gd);

        for (auto &l : spanTree.edgeList())
        {
            generateCorridor(rooms[l.first],rooms[l.second]);
        }
    }

    void generateCorridor(
        cRoom &rooma, cRoom &roomb);
};

void cLevelGenerator1::generateCorridor(
    cRoom &rooma, cRoom &roomb)
{
    if (rooma.y+rooma.h-1 < roomb.y)
    {
        // from bottom middle to left middle
        corridors.emplace_back(
            rooma.x + rooma.w / 2, rooma.y + rooma.h - 1,
            roomb.x + roomb.w / 2, roomb.y);
    }
    else if ( 
     rooma.x+rooma.w - 1 < roomb.x )
    {
        //from right middle to left middle
        corridors.emplace_back(
            rooma.x+rooma.w-1, rooma.y+rooma.h/2,
            roomb.x,roomb.y+roomb.h/2 );
    }
    else
    {
        // from top middle to bottom middle
        corridors.emplace_back(
            rooma.x+rooma.w/2,rooma.y,
            roomb.x+roomb.w/2,roomb.y+roomb.h-1 );
    }
}
class cGUI : public cStarterGUI
{
public:
    cGUI()
        : cStarterGUI(
              "Rooms",
              {50, 50, 1000, 500})
    {

        fm.events().draw(
            [this](PAINTSTRUCT &ps)
            {
                scale = 10;
                off = 10;
                wex::shapes S(ps);
                S.penThick(5);

                drawRooms(S);

                // S.color(0x0000FF);
                // auto &gd = levelGenerator.getGraphData();
                // for (auto &l : levelGenerator.getSpan().edgeList())
                // {
                //     int x1 = off + scale * atoi(gd.g.rVertexAttr(l.first, 0).c_str());
                //     int y1 = off + scale * atoi(gd.g.rVertexAttr(l.first, 1).c_str());
                //     int x2 = off + scale * atoi(gd.g.rVertexAttr(l.second, 0).c_str());
                //     int y2 = off + scale * atoi(gd.g.rVertexAttr(l.second, 1).c_str());
                //     S.line({x1, y1, x2, y2});
                // }

                drawCorridors(S);
            });

        show();
        run();
    }

private:
    cLevelGenerator1 levelGenerator;

    int scale;
    int off;

    void drawRooms(wex::shapes &S);
    void drawCorridors(wex::shapes &S);
};

void cGUI::drawRooms(wex::shapes &S)
{
    S.color(0xFFAAAA);
    S.fill();
    for (auto &room : levelGenerator.getRooms())
    {
        S.rectangle({off + scale * room.x, off + scale * room.y,
                     scale * (room.w - 1), scale * (room.h - 1)});
    }
}

void cGUI::drawCorridors(wex::shapes &S)
{
    S.color(0x00FF00);
    for (auto &corridor : levelGenerator.getCorridors())
    {
        S.line({off + scale * corridor.x1, off + scale * corridor.y1,
               off + scale * corridor.x2, off + scale * corridor.y2});
    }
}

main()
{

    cGUI theGUI;
    return 0;
}
