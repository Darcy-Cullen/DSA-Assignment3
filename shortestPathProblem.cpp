#include <iostream>
#include <fstream>
#include <cmath>

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
        length = 0;                      // Calculate the length of the string
        while (str[length] != '\0')      // Loop until the null character is found
            length++;                    // Increment the length of the string
        data = new char[length + 1];     // Allocate memory for the string
        for (int i = 0; i < length; i++) // Copy the string
            data[i] = str[i];            // Copy the character
        data[length] = '\0';             // Append the null character
    }

    ~String() // Destructor
    {
        delete[] data;
    }

    int size() const { return length; } // Return the length of the string

    char &operator[](int index) // Overloaded subscript operator
    {
        return data[index]; // Return the character at the specified index
    }

    const char *c_str() const // Return the string as a C-style string
    {
        return data;
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

// graph class
class Graph
{
private:
    Vector<Vertex> vertices;
    Vector<Edge> edges;
    int nVertices, nEdges;

public:
    void readFromFile(const String &filename) // Read graph data from file
    {
        ifstream file(filename.c_str()); // Open the file
        file >> nVertices >> nEdges;     // Read the number of vertices and edges

        for (int i = 0; i < nVertices; i++) // Read the vertices
        {
            Vertex v; // Create a new vertex
            file >> v.label >> v.x >> v.y;
            vertices.addElement(v); // Add the vertex to the vector
        }

        for (int i = 0; i < nEdges; i++) // Read the edges
        {
            Edge e;
            file >> e.start >> e.end >> e.weight;
            edges.addElement(e); // Add the edge to the vector
        }

        file.close();
    }

    Vector<int> shortestPath(int start, int goal)
    {
        // Implement Dijkstra's algorithm here
        // Return the shortest path as a vector of vertex labels
        Vector<int> path;
        // TODO: Implement shortest path algorithm
        return path;
    }

    Vector<int> longestPath(int start, int goal)
    {
        // Implement longest path algorithm here (DFS)
        // Return the longest path as a vector of vertex labels
        Vector<int> path;
        // TODO: Implement longest path algorithm
        return path;
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
};

int main()
{
    String filename;
    cout << "Enter the name of the input file: ";
    char buffer[100];
    cin >> buffer;
    filename = String(buffer);

    Graph graph;
    graph.readFromFile(filename);

    int start, goal;
    cin >> start >> goal;

    cout << "Euclidean distance between start and goal: " << graph.euclideanDistance(start, goal) << endl;

    Vector<int> shortestPath = graph.shortestPath(start, goal);
    cout << "Shortest path: ";
    graph.printPath(shortestPath);
    cout << "Length of shortest path: " << graph.calculatePathLength(shortestPath) << endl;

    Vector<int> longestPath = graph.longestPath(start, goal);
    cout << "Longest path: ";
    graph.printPath(longestPath);
    cout << "Length of longest path: " << graph.calculatePathLength(longestPath) << endl;

    return 0;
}
