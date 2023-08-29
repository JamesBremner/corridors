#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <wex.h>
#include "GraphTheory.h"

#include "corridors.h"
#include "cGUI.h"

    void cLevelGenerator1::GenerateLevel()
    {
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

                //room.print();
                rooms.push_back(room);
                MarkTilesUsed(room);
            }
        }

        ConnectRooms();
    }

        bool cLevelGenerator1::IsSpaceAvailable(const cRoom &room)
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

    void cLevelGenerator1::MarkTilesUsed(const cRoom &room)
    {
        for (int i = room.x; i < room.x + room.w; i++)
        {
            for (int j = room.y; j < room.y + room.h; j++)
            {
                usedTiles[i][j] = true;
            }
        }
    }
void cLevelGenerator1::generateCorridor(
    cRoom &rooma, cRoom &roomb)
{
    if (rooma.y + rooma.h - 1 < roomb.y)
    {
        // from bottom middle to left middle
        corridors.emplace_back(
            rooma.x + rooma.w / 2, rooma.y + rooma.h - 1,
            roomb.x + roomb.w / 2, roomb.y);
    }
    else if (
        rooma.x + rooma.w - 1 < roomb.x)
    {
        // from right middle to left middle
        corridors.emplace_back(
            rooma.x + rooma.w - 1, rooma.y + rooma.h / 2,
            roomb.x, roomb.y + roomb.h / 2);
    }
    else
    {
        // from top middle to bottom middle
        corridors.emplace_back(
            rooma.x + rooma.w / 2, rooma.y,
            roomb.x + roomb.w / 2, roomb.y + roomb.h - 1);
    }
}

    void cLevelGenerator1::ConnectRooms()
    {
        // add vertex at center of each room
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

        // allocate space for all possible edge weights
        gd.edgeWeight.resize(
            2 * gd.g.vertexCount() * gd.g.vertexCount(), INT_MAX);

        // add links between rooms that are 'reasonably' close to each other
        int reasonable = 2 *roomSizeMax;
        for (int kr = 0; kr < rooms.size(); kr++)
            for (int kr2 = kr + 1; kr2 < rooms.size(); kr2++)
            {
                // if( kr == 0)
                //     std::cout << kr2 <<" "<< rooms[kr2].x <<" "<<rooms[kr2].y<<"\n";
                if( abs(rooms[kr].x-rooms[kr2].x) > reasonable ||
                 abs(rooms[kr].y-rooms[kr2].y) > reasonable)
                    continue;

                int cost =  rooms[kr].distance2(rooms[kr2]);
                // if( kr == 0)
                //     std::cout << "add " << cost << "\n";
                gd.edgeWeight[gd.g.add(kr, kr2)] = cost;
            }

        // generate spanning tree to connect all rooms
        gd.startName = "V0";
        spanTree = spanningTree(gd);

        // generate corridor between every pair of linked rooms in spanning tree
        for (auto &l : spanTree.edgeList())
        {
            generateCorridor(rooms[l.first], rooms[l.second]);
        }
    }


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
void cGUI::drawSpan(wex::shapes &S)
{
    S.color(0x0000FF);
    auto &gd = levelGenerator.getGraphData();
    for (auto &l : levelGenerator.getSpan().edgeList())
    {
        int x1 = off + scale * atoi(gd.g.rVertexAttr(l.first, 0).c_str());
        int y1 = off + scale * atoi(gd.g.rVertexAttr(l.first, 1).c_str());
        int x2 = off + scale * atoi(gd.g.rVertexAttr(l.second, 0).c_str());
        int y2 = off + scale * atoi(gd.g.rVertexAttr(l.second, 1).c_str());
        S.line({x1, y1, x2, y2});
    }
}
    main()
    {

        cGUI theGUI;
        return 0;
    }
