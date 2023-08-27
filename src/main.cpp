#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <wex.h>
#include "cStarterGUI.h"

class cGUI : public cStarterGUI
{
public:
    cGUI()
        : cStarterGUI(
              "Starter",
              {50, 50, 1000, 500}),
          lb(wex::maker::make<wex::label>(fm))
    {
        lb.move(50, 50, 100, 30);
        lb.text("Hello World");

        show();
        run();
    }

private:
    wex::label &lb;
};

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

    // private Transform mapHolder;

    cLevelGenerator1()
    {
        GenerateLevel();
    }

private:
    std::vector<std::vector<bool>> usedTiles;
    std::vector<cRoom> rooms;

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
                if (!usedTiles[x][y])
                {
                    // Generate rooms
                    if (rand() % 100 < 80)
                        continue;

                    int roomWidth = rand()%sizeRange + roomSizeMin;
                    int roomHeight = rand()%sizeRange + roomSizeMin;

                    int startX = x - roomWidth / 2;
                    int startY = y - roomHeight / 2;

                    if (IsSpaceAvailable(startX, startY, roomWidth, roomHeight))
                    {
                        GenerateRoom(startX, startY, roomWidth, roomHeight);
                        rooms.emplace_back(startX, startY, roomWidth, roomHeight);
                        MarkTilesUsed(startX, startY, roomWidth, roomHeight);
                    }
                }
            }
        }

        // ConnectRooms();
    }

    bool IsSpaceAvailable(int x, int y, int width, int height)
    {
        for (int i = x; i < x + width; i++)
        {
            for (int j = y; j < y + height; j++)
            {
                if (i < 0 || i >= mapWidth || j < 0 || j >= mapHeight || usedTiles[i][j])
                {
                    return false;
                }
            }
        }
        return true;
    }

    void MarkTilesUsed(int x, int y, int width, int height)
    {
        for (int i = x; i < x + width; i++)
        {
            for (int j = y; j < y + height; j++)
            {
                usedTiles[i][j] = true;
            }
        }
    }

    void GenerateRoom(int x, int y, int width, int height)
    {
        std::cout << "room at " <<x<<","<<y<<" size "<<width<<" by "<<height<<"\n";

        for (int i = x; i < x + width; i++)
        {
            for (int j = y; j < y + height; j++)
            {
                // Vector3 tilePosition = new Vector3(i, j, 0);

                // if (i == x || i == x + width - 1 || j == y || j == y + height - 1)
                // {
                //     GameObject wallTile = Instantiate(wallPrefab, tilePosition, Quaternion.identity);
                //     wallTile.transform.parent = mapHolder;
                // }
                // else
                // {
                //     GameObject floorTile = Instantiate(floorPrefab, tilePosition, Quaternion.identity);
                //     floorTile.transform.parent = mapHolder;
                // }
            }
        }
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

main()
{
    cLevelGenerator1 levelGenerator;

    cGUI theGUI;
    return 0;
}
