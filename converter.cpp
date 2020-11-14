#include <iostream> /// cout cin
#include <iomanip>  /// setw setprecision fixed
#include <vector>   /// Vectors
#include <cstdlib>  /// Standard Library
#include <ctime>    /// time
#include <fstream>  // file I/O
using namespace std;
/// Structs
struct vec3
{
    int x;
    int y;
    int z;
};
/// Constants
/// Function Prototypes
int getIndex(const vec3 &dims, const int &x, const int &y, const int &z);
vec3 getPos(const vec3 &dims, const int &index);
void loadData(const string &filename, vector<vec3> &pos, vector<vec3> &rgb);
void getHighest(vec3 &highest, const vector<vec3> &list);
void getLowest(vec3 &lowest, const vector<vec3> &list);
vec3 getDimensions(vec3 &highest, vec3 &lowest);
void processVoxels(const vec3 &dimensions, const vec3 &lowest, const vec3 &highest, vector<float> &IDs, vector<int> &counters, const vector<vec3> &pos, const vector<vec3> &colors);
void writeData(const string &filename, const vec3 &dimensions, const vector<float> IDs, const vector<int> &counters);
int main(int argc, char *argv[])
{
    /// Stores the dimensions of the 3D structure
    vec3 dimensions;
    /// Stores the highest and lowest x,y,z values
    vec3 highest;
    vec3 lowest;
    /// Stores the RLE data
    vector<float> IDs;
    vector<int> counters;
    /// Variables that store position and color data for a model
    vector<vec3> positions;
    vector<vec3> colors;
    /// Check to see if the user entered a .PLY file
    if (argc != 3)
    {
        cout << "Usage: a.exe <inputfile> <outputfile>" << endl;
        exit(0);
    }
    /// Proceed to load file
    loadData(argv[1], positions, colors);
    /// Find highest and lowest for X,Y,Z
    getHighest(highest, positions);
    getLowest(lowest, positions);
    /// Get the dimensions of the structure and store it in dimensions
    dimensions = getDimensions(highest, lowest);
    /// process voxels
    processVoxels(dimensions, lowest, highest, IDs, counters, positions, colors);
    /// write the results to outputFile.rle
    writeData(argv[2],dimensions, IDs, counters);
    cout << "Application done!" << endl;
    return 0;
}
int getIndex(const vec3 &dims, const int &x, const int &y, const int &z)
{
    return x + y * dims.x + z * (dims.x * dims.y);
}
vec3 getPos(const vec3 &dims, const int &index)
{
    vec3 result;
    result.x = index % dims.x;
    result.y = index / dims.x % dims.y;
    result.z = index / (dims.x * dims.y);
    return result;
}
void loadData(const string &filename, vector<vec3> &pos, vector<vec3> &rgb)
{
    ifstream input;
    /// Temporary variables storing current position and RGB data
    vec3 currPos;
    vec3 currRGB;
    /// Attempt to open the file. If it does not print go fuck yourself
    input.open(filename+".ply");
    if (input.fail())
    {
        cout << "Error: Failed to get input file" << filename << ".ply" << endl;
        exit(0);
    }
    string dummy;
    /// Skips the PLY header
    getline(input, dummy);
    getline(input, dummy);
    getline(input, dummy);
    getline(input, dummy);
    getline(input, dummy);
    getline(input, dummy);
    getline(input, dummy);
    getline(input, dummy);
    getline(input, dummy);
    getline(input, dummy);
    getline(input, dummy);
    /// Iterate through the file and get all of the data and store in vectors
    while (!input.eof())
    {
        /// We swap z and y because MagicaVoxel has z as up and down instead of y like our game. swap this around if you need
        /// Push position data into current position variable
        input >> currPos.x >> currPos.z >> currPos.y;
        /// Push color data into current color variable
        input >> currRGB.x >> currRGB.y >> currRGB.z;
        // Push position and color data into their respective vectors
        pos.push_back(currPos);
        rgb.push_back(currRGB);
    }
    /// Close the input stream
    input.close();
}
void getHighest(vec3 &highest, const vector<vec3> &list)
{
    /// Temporary variable for the current highest position
    vec3 currentHighest = list[0];
    /// Iterate through all the positions and get the highest values
    for (unsigned int i = 0; i < list.size(); i++)
    {
        /// Check if we find a bigger number, if so record it as the highest
        if (list[i].x > currentHighest.x)
        {
            currentHighest.x = list[i].x;
        }
        if (list[i].y > currentHighest.y)
        {
            currentHighest.y = list[i].y;
        }
        if (list[i].z > currentHighest.z)
        {
            currentHighest.z = list[i].z;
        }
    }
    /// After we are done, set the highest to whatever our result is
    highest = currentHighest;
}
void getLowest(vec3 &lowest, const vector<vec3> &list)
{
    /// Temporary variable for the current lowest position
    vec3 currentLowest = list[0];
    /// Iterate through all the positions and get the lowest values
    for (unsigned int i = 0; i < list.size(); i++)
    {
        /// Check if we find a smaller number, if so record it as the lowest
        if (list[i].x < currentLowest.x)
        {
            currentLowest.x = list[i].x;
        }
        if (list[i].y < currentLowest.y)
        {
            currentLowest.y = list[i].y;
        }
        if (list[i].z < currentLowest.z)
        {
            currentLowest.z = list[i].z;
        }
    }
    /// After we are done, set the highest to whatever our result is
    lowest = currentLowest;
}
vec3 getDimensions(vec3 &highest, vec3 &lowest)
{
    /// Stores the result of the calculation
    vec3 result;
    /// Calculate dimensions
    result.x = (highest.x - lowest.x) + 1;
    result.y = (highest.y - lowest.y) + 1;
    result.z = (highest.z - lowest.z) + 1;
    /// return result
    return result;
}
void processVoxels(const vec3 &dimensions, const vec3 &lowest, const vec3 &highest, vector<float> &IDs, vector<int> &counters, const vector<vec3> &pos, const vector<vec3> &colors)
{
/// create colors array
    //vec3* colorsTemp = new vec3[][][];
    cout << "Creating Array..." << endl;
    vector<vector<vector<vec3>>> colorsTemp(dimensions.x, vector<vector<vec3>>(dimensions.y, vector<vec3>(dimensions.z)));
    /// pass in only the colors that are present
    /// the rest are null
    cout << "Assigning Temporary values..." << endl;

    for (unsigned int z = 0; z < dimensions.z; z++)
    {
        
        for (unsigned int y = 0; y < dimensions.y; y++)
        {
            for (unsigned int x = 0; x < dimensions.x; x++)
            {
                colorsTemp[x][y][z].x = -1;
                colorsTemp[x][y][z].y = -1;
                colorsTemp[x][y][z].z = -1;
            }
        }
    }
    cout << "Assigning Colors to array..." << endl;
    cout << "Lowest: " << lowest.x << ", " << lowest.y << ", " << lowest.z << endl;
    cout << "Highest: " << highest.x << ", " << highest.y << ", " << highest.z << endl;
    cout << "Dimensions: " << dimensions.x << ", " << dimensions.y << ", " << dimensions.z << endl;
    for (unsigned int i = 0; i < pos.size(); i++)
    {
        int x = (pos[i].x + abs(lowest.x));
        int y = (pos[i].y + abs(lowest.y));
        int z = (pos[i].z + abs(lowest.z));
        //cout << x << " " << y << " " << z << endl;
        colorsTemp[x][y][z].x = colors[i].x;
        colorsTemp[x][y][z].y = colors[i].y;
        colorsTemp[x][y][z].z = colors[i].z;
    }

    long long currentIndex = 0;
    long long nextIndex = 1;
    int currCounter = 0;
    float currR = 0;
    float currG = 0;
    float currB = 0;
    cout << "Beginning Compression..." << endl;

    for (unsigned int z = 0; z < dimensions.z; z++)
    {
        for (unsigned int y = 0; y < dimensions.y; y++)
        {
            for (unsigned int x = 0; x < dimensions.x; x++)
            {
                currCounter++;
                long long index = getIndex(dimensions,x,y,z);
                long long nextIndex = index + 1;
                vec3 nextVoxel = getPos(dimensions, nextIndex);
                //int nextZ = nextIndex % dimensions.z;
                //int nextY = (nextIndex / dimensions.z) % dimensions.y;
                //int nextX = nextIndex / (dimensions.y * dimensions.z);
                ///cout << nextVoxel.x << " " << nextVoxel.y << " " << nextVoxel.z << endl;
                if (colorsTemp[x][y][z].x == -1 && colorsTemp[x][y][z].y == -1 && colorsTemp[x][y][z].z == -1)
                {
                    currR = -1;
                    currG = -1;
                    currB = -1;
                }
                else
                {
                    currR = colorsTemp[x][y][z].x / 255.0;
                    currG = colorsTemp[x][y][z].y / 255.0;
                    currB = colorsTemp[x][y][z].z / 255.0;
                }
                if (nextIndex >= dimensions.x * dimensions.y * dimensions.z)
                {
                    IDs.push_back(currR);
                    IDs.push_back(currG);
                    IDs.push_back(currB);
                    counters.push_back(currCounter);
                    return;
                }
                if (colorsTemp[x][y][z].x != colorsTemp[nextVoxel.x][nextVoxel.y][nextVoxel.z].x &&
                    colorsTemp[x][y][z].y != colorsTemp[nextVoxel.x][nextVoxel.y][nextVoxel.z].y &&
                    colorsTemp[x][y][z].z != colorsTemp[nextVoxel.x][nextVoxel.y][nextVoxel.z].z)
                {
                    IDs.push_back(currR);
                    IDs.push_back(currG);
                    IDs.push_back(currB);
                    counters.push_back(currCounter);
                    currCounter = 0;
                }
            }
        }
    }
}
void writeData(const string &filename, const vec3 &dimensions, const vector<float> IDs, const vector<int> &counters)
{
    ofstream output;
    output.open(filename+".rle");
    if (output.fail())
    {
        cout << "Error cannot generate file " << filename << ".rle" << endl;
        exit(0);
    }
    output << dimensions.x << " " << dimensions.y << " " << dimensions.z << endl;
    output << fixed << setprecision(5);
    output << IDs[0] << " " << IDs[1] << " " << IDs[2] << " " << counters[0];
    for (unsigned int i = 1; i < IDs.size() / 3; i++)
    {
        output << " " << IDs[i * 3] << " " << IDs[i * 3 + 1] << " " << IDs[i * 3 + 2] << " " << counters[i];
    }
    long long int total = 0;
    for (unsigned int i = 0; i < counters.size(); i++)
    {
        total += counters[i];
    }
    cout << "total blocks: " << total;
    output.close();
}