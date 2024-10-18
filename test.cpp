#include <iostream>
#include <fstream>
#include <cmath>
#include <climits>

using namespace std;

// Custom string class
class String
{
private:
    char *data;
    int length;

public:
    String() : data(nullptr), length(0) {} // Default constructor

    String(const char *str) // Constructor
    {
        length = 0;
        while (str[length] != '\0')
            length++;
        data = new char[length + 1];
        for (int i = 0; i <= length; i++)
            data[i] = str[i];
    }

    ~String() // Destructor
    {
        delete[] data;
    }

    int size() const { return length; }

    char &operator[](int index)
    {
        return data[index];
    }

    const char *c_str() const
    {
        return data ? data : "";
    }
};

// Custom vector class
template <typename T> // Template class so can store elements of any data type
class Vector
{
private:
    T *data;
    int capacity;
    int vecSize;

public:
    Vector() : data(nullptr), capacity(0), vecSize(0) {} // Default constructor

    Vector(int size, T initialValue) : capacity(size), vecSize(size)
    {
        data = new T[size];
        for (int i = 0; i < size; i++)
        {
            data[i] = initialValue;
        }
    }

    ~Vector() // Destructor
    {
        delete[] data;
    }

    void addElement(const T &value) // Add an element to the end of the vector
    {
        if (vecSize == capacity) // If the vector is full
        {
            int new_capacity; // Calculate the new capacity
            if (capacity == 0)
            { // If the vector is empty

                new_capacity = 1; // Set the new capacity to 1
            }
            else
            {
                new_capacity = capacity * 2; // Double the capacity
            }

            T *new_data = new T[new_capacity]; // Allocate memory for the new vector
            for (int i = 0; i < vecSize; i++)  // Copy the elements to the new vector
            {
                new_data[i] = data[i]; // Copy the element
            }

            delete[] data;   // Deallocate memory for the old vector
            data = new_data; // Update the pointer to the new vector
            capacity = new_capacity;
        }
        data[vecSize++] = value; // Add the element to the end of the vector
    }

    void removeLastElement()
    {
        if (vecSize > 0)
        {
            vecSize--;
        }
    }

    T &operator[](int index)
    {
        return data[index];
    }

    const T &operator[](int index) const
    {
        return data[index];
    }

    int size() const
    {
        return vecSize;
    }
};

struct Vertex
{
    int label;
    double x, y;
};

struct Edge
{
    int start, end;
    double weight;
};

class Graph
{
private:
    Vector<Vertex> vertices;
    Vector<Edge> edges;
    int nVertices, nEdges;
    int startVertex, goalVertex;

    int getVertexIndex(int label) const
    {
        for (int i = 0; i < vertices.size(); i++)
        {
            if (vertices[i].label == label)
                return i;
        }
        return -1;
    }

public:
    void readFromFile(const String &filename)
    {
        ifstream file(filename.c_str());
        if (!file.is_open())
        {
            cout << "Error: Unable to open file '" << filename.c_str() << "'" << endl;
            return;
        }

        file >> nVertices >> nEdges;

        for (int i = 0; i < nVertices; i++)
        {
            Vertex v;
            file >> v.label >> v.x >> v.y;
            vertices.addElement(v);
        }

        for (int i = 0; i < nEdges; i++)
        {
            Edge e;
            file >> e.start >> e.end >> e.weight;
            edges.addElement(e);
        }

        file >> startVertex >> goalVertex;

        file.close();

        if (getVertexIndex(startVertex) == -1 || getVertexIndex(goalVertex) == -1)
        {
            cout << "Error: Invalid start or goal vertex" << endl;
            startVertex = goalVertex = -1;
        }
    }

    void printGraphInfo()
    {
        cout << "The number of vertexes in the graph: " << nVertices << endl;
        cout << "The number of edges in the graph: " << nEdges << endl;
        cout << "The start vertexes: " << startVertex << endl;
        cout << "The end vertexes: " << goalVertex << endl;
    }

    Vector<int> shortestPath(int start, int goal)
    {
        Vector<double> dist(nVertices, INT_MAX);
        Vector<int> prev(nVertices, -1);
        Vector<bool> visited(nVertices, false);

        int startIndex = getVertexIndex(start);
        int goalIndex = getVertexIndex(goal);

        dist[startIndex] = 0;

        for (int i = 0; i < nVertices; i++)
        {
            int u = -1;
            double minDist = INT_MAX;
            for (int j = 0; j < nVertices; j++)
            {
                if (!visited[j] && dist[j] < minDist)
                {
                    u = j;
                    minDist = dist[j];
                }
            }

            if (u == -1 || u == goalIndex)
                break;

            visited[u] = true;

            for (int j = 0; j < edges.size(); j++)
            {
                if (edges[j].start == vertices[u].label)
                {
                    int v = getVertexIndex(edges[j].end);
                    double alt = dist[u] + edges[j].weight;
                    if (alt < dist[v])
                    {
                        dist[v] = alt;
                        prev[v] = u;
                    }
                }
            }
        }

        Vector<int> path;
        for (int v = goalIndex; v != -1; v = prev[v])
        {
            path.addElement(vertices[v].label);
        }

        Vector<int> reversedPath;
        for (int i = path.size() - 1; i >= 0; i--)
        {
            reversedPath.addElement(path[i]);
        }

        return reversedPath;
    }

    Vector<int> longestPath() // Find the longest path in the graph
    {
        Vector<bool> visited(nVertices, false); // Create a vector to keep track of visited vertices
        Vector<int> currentPath;                // Create a vector to store the current path
        Vector<int> longestPath;                // Create a vector to store the longest path
        double maxLength = -1;                  // Initialise the maximum length to -1 (no path found yet)

        int startIndex = getVertexIndex(startVertex); // Get the index of the start vertex
        if (startIndex == -1)                         // If the start vertex is not found
        {
            cout << "Error: Start vertex not found" << endl; // Print an error message
            return longestPath;
        }

        visited[startIndex] = true;          // Mark the start vertex as visited
        currentPath.addElement(startVertex); // Add the start vertex to the current path

        dfsLongestPath(startIndex, visited, currentPath, longestPath, 0, maxLength); // Call the DFS function

        return longestPath;
    }

    void dfsLongestPath(int currentIndex, Vector<bool> &visited, Vector<int> &currentPath,
                        Vector<int> &longestPath, double currentLength, double &maxLength)
    {
        if (vertices[currentIndex].label == goalVertex) // If the goal vertex is reached
        {
            if (currentLength > maxLength) // If the current path is longer than the longest path found so far
            {
                maxLength = currentLength; // Update the maximum length
                longestPath = currentPath; // Update the longest path
            }
            return;
        }

        for (int i = 0; i < edges.size(); i++) // Loop through the edges
        {
            if (edges[i].start == vertices[currentIndex].label) // If the edge starts at the current vertex
            {
                int nextIndex = getVertexIndex(edges[i].end); // Get the index of the next vertex
                if (nextIndex != -1 && !visited[nextIndex])   // If the next vertex is valid and not visited yet
                {
                    visited[nextIndex] = true;                         // Mark the next vertex as visited
                    currentPath.addElement(vertices[nextIndex].label); // Add the next vertex to the current path

                    dfsLongestPath(nextIndex, visited, currentPath, longestPath, // Recursively call the function
                                   currentLength + edges[i].weight, maxLength);

                    visited[nextIndex] = false;      // Unmark the next vertex
                    currentPath.removeLastElement(); // Remove the next vertex from the current path
                }
            }
        }
    }

    double calculatePathLength(const Vector<int> &path)
    {
        double length = 0;
        for (int i = 0; i < path.size() - 1; i++) // Loop through the path
        {
            int start = path[i];                   // Get the start vertex
            int end = path[i + 1];                 // Get the end vertex
            for (int j = 0; j < edges.size(); j++) // Loop through the edges
            {
                if (edges[j].start == start && edges[j].end == end) // If the edge connects the start and end vertices
                {
                    length += edges[j].weight; // Add the weight of the edge to the length
                    break;
                }
            }
        }
        return length;
    }

    double euclideanDistance(int start, int goal)
    {
        Vertex startVert, goalVert;
        for (int i = 0; i < vertices.size(); i++) // Loop through the vertices
        {
            if (vertices[i].label == start) // If the vertex label matches the start label
                startVert = vertices[i];    // Set the start vertex
            if (vertices[i].label == goal)  // If the vertex label matches the goal label
                goalVert = vertices[i];     // Set the goal vertex
        }
        return sqrt(pow(startVert.x - goalVert.x, 2) + pow(startVert.y - goalVert.y, 2)); // Calculate the Euclidean distance
    }

    void printPath(const Vector<int> &path) // Print the path
    {
        for (int i = 0; i < path.size(); i++) // Loop through the path
        {
            cout << path[i];         // Print the vertex label
            if (i < path.size() - 1) // If not the last vertex
                cout << " -> ";      // Print the arrow
        }
        cout << endl;
    }

    int getStartVertex() const { return startVertex; }
    int getGoalVertex() const { return goalVertex; }
};

int main()
{
    char buffer[100];
    cout << "Enter the name of the input file: ";
    cin >> buffer; // Read until whitespace
    String filename(buffer);

    Graph graph;
    graph.readFromFile(filename);

    int start = graph.getStartVertex();
    int goal = graph.getGoalVertex();

    if (start == -1 || goal == -1)
    {
        cout << "Error: Invalid start or goal vertex. Exiting program." << endl;
        return 1;
    }

    cout << "===========================================" << endl;
    graph.printGraphInfo();
    cout << "===========================================" << endl;

    cout << "The Euclidean distance between the start and the goal vertexes: " << graph.euclideanDistance(start, goal) << endl;

    Vector<int> shortestPath = graph.shortestPath(start, goal);
    cout << "Shortest path: ";
    if (shortestPath.size() > 0)
    {
        graph.printPath(shortestPath);
        cout << "The length of the shortest path: " << graph.calculatePathLength(shortestPath) << endl;
    }
    else
    {
        cout << "No path found" << endl;
    }

    Vector<int> longestPath = graph.longestPath(); // Find the longest path
    cout << "Longest path: ";
    if (longestPath.size() > 0)
    {
        graph.printPath(longestPath);
        cout << "The length of the longest path: " << graph.calculatePathLength(longestPath) << endl;
    }
    else
    {
        cout << "No path found" << endl;
    }

    cout << "===========================================" << endl;

    return 0;
}