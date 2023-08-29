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

    void GenerateLevel();

    bool IsSpaceAvailable(const cRoom &room);

    void MarkTilesUsed(const cRoom &room);

    void ConnectRooms();

    void generateCorridor(
        cRoom &rooma, cRoom &roomb);
};
