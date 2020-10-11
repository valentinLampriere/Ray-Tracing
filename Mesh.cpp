#include "mesh.h"

Mesh Mesh::loadSphere(int nbMeridian, int nbParallels, float radius) {
    Mesh sphere;
    std::vector<Vector3> vertices;

    vertices.push_back(Vector3(0, radius * std::cos(0), 0));
    for (int i = 1; i < nbParallels - 1; i++) {
        float phi = (M_PI / (nbParallels - 1)) * i;
        for (int j = 0; j < nbMeridian; j++) {
            float theta = (2.0f * M_PI / nbMeridian) * j;
            vertices.push_back(Vector3(std::cos(theta) * radius * std::sin(phi), radius * std::cos(phi), std::sin(theta) * radius * std::sin(phi)));
        }
    }
    vertices.push_back(Vector3(0, radius * std::cos(M_PI), 0));

    for (int i = 0; i < nbParallels - 3; i++) {
        int a = i * nbMeridian + 1;
        int b = (i + 1) * nbMeridian + 1;
        for (int j = 0; j < nbMeridian; j++) {
            Triangle t1 = Triangle(vertices[a + j], vertices[a + (j + 1) % nbMeridian], vertices[b + (j + 1) % nbMeridian]);
            Triangle t2 = Triangle(vertices[a + j], vertices[b + (j + 1) % nbMeridian], vertices[b + j]);
            sphere.triangles.push_back(t1);
            sphere.triangles.push_back(t2);
        }
    }
    for (int i = 0; i < nbMeridian; i++) {
        Triangle top = Triangle(vertices[0], vertices[(i + 1) % nbMeridian + 1], vertices[i + 1]);
        Triangle bot = Triangle(vertices[i + nbMeridian * (nbParallels - 3) + 1], vertices[(i + 1) % nbMeridian + nbMeridian * (nbParallels - 3) + 1], vertices[vertices.size()]);
        sphere.triangles.push_back(top);
        sphere.triangles.push_back(bot);
    }
    return sphere;
}
Mesh Mesh::loadOff(string filename) {

}