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
    }
    //     void ConnectRooms()
    //     {
    //         List<Edge> edges = new List<Edge>();

    //         for (int i = 0; i < rooms.Count; i++)
    //         {
    //             for (int j = i + 1; j < rooms.Count; j++)
    //             {
    //                 Vector2Int roomA = rooms[i];
    //                 Vector2Int roomB = rooms[j];
    //                 float distance = Vector2Int.Distance(roomA, roomB);
    //                 edges.Add(new Edge(i, j, distance));
    //             }
    //         }

    //         edges.Sort((a, b) = > a.distance.CompareTo(b.distance));

    //         List<int> takenIndices = new List<int>();
    //         List<int> connectedIndices = new List<int>();

    //         foreach (Edge edge in edges)
    //         {
    //             if (!takenIndices.Contains(edge.from) || !takenIndices.Contains(edge.to))
    //             {
    //                 connectedIndices.Add(edge.from);
    //                 connectedIndices.Add(edge.to);
    //                 takenIndices.Add(edge.from);
    //                 takenIndices.Add(edge.to);

    //                 Vector2Int roomA = rooms[edge.from];
    //                 Vector2Int roomB = rooms[edge.to];

    //                 GenerateCorridor(roomA, roomB);
    //             }
    //         }
    //     }

    //     void GenerateCorridor(Vector2Int pointA, Vector2Int pointB)
    //     {
    //         Vector2Int start = pointA;
    //         Vector2Int end = pointB;

    //         if (start.x > end.x)
    //         {
    //             Vector2Int temp = start;
    //             start = end;
    //             end = temp;
    //         }

    //         int corridorWidth = Mathf.Abs(end.x - start.x) + 1;
    //         int corridorHeight = Mathf.Abs(end.y - start.y) + 1;

    //         int corridorX = start.x;
    //         int corridorY = start.y;

    //         for (int i = 0; i < corridorWidth; i++)
    //         {
    //             for (int j = 0; j < corridorHeight; j++)
    //             {
    //                 Vector3 tilePosition = new Vector3(corridorX + i, corridorY + j, 0);
    //                 GameObject corridorTile = Instantiate(corridorPrefab, tilePosition, Quaternion.identity);
    //                 corridorTile.transform.parent = mapHolder;
    //             }
    //         }
    //     }

    //     struct Edge
    //     {
    //     public
    //         int from;
    //     public
    //         int to;
    //     public
    //         float distance;

    //     public
    //         Edge(int from, int to, float distance)
    //         {
    //             this.from = from;
    //             this.to = to;
    //             this.distance = distance;
    //         }
    //     }
};
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
                int scale = 10;
                int off = 10;
                wex::shapes S(ps);
                S.penThick(5);
                auto &gd = levelGenerator.getGraphData();
                for (auto &l : levelGenerator.getSpan().edgeList())
                {
                    int x1 = off + scale * atoi(gd.g.rVertexAttr(l.first, 0).c_str());
                    int y1 = off + scale * atoi(gd.g.rVertexAttr(l.first, 1).c_str());
                    int x2 = off + scale * atoi(gd.g.rVertexAttr(l.second, 0).c_str());
                    int y2 = off + scale * atoi(gd.g.rVertexAttr(l.second, 1).c_str());
                    S.line({x1, y1, x2, y2});
                }

                S.color(0xFFAAAA);
                S.fill();
                for (auto &room : levelGenerator.getRooms())
                {
                    S.rectangle({off + scale * room.x, off + scale * room.y,
                                 scale * (room.w - 1), scale * (room.h - 1)});
                }
            });

        show();
        run();
    }

private:
    cLevelGenerator1 levelGenerator;
};

main()
{

    cGUI theGUI;
    return 0;
}
