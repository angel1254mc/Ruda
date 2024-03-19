#pragma once
#include <math.h>
#include "vertex.h"

struct Triangle {
    Vertex* vertices[3];
    float vert_pos[9];
    float color[3];
    float normal[3];
    

    Triangle(Vertex* v1, Vertex* v2, Vertex* v3, float r, float g, float b)
    {
        vertices[0] = v1;
        vertices[1] = v2;
        vertices[2] = v3;

        color[0] = r;
        color[1] = g;
        color[2] = b;
        setPosition();
        calculateNormal();
    };
    void calculateNormal()
    {
        float edge1[3] = {vertices[0]->position[0] - vertices[1]->position[0], vertices[0]->position[1] - vertices[1]->position[1], vertices[0]->position[2] - vertices[1]->position[2]};
        float edge2[3] = {vertices[2]->position[0] - vertices[1]->position[0], vertices[2]->position[1] - vertices[1]->position[1], vertices[2]->position[2] - vertices[1]->position[2]};
	
	    float norm[3] = {edge1[1] * edge2[2] - edge1[2] * edge2[1], edge1[2] * edge2[0] - edge1[0] * edge2[2], edge1[0] * edge2[1] - edge1[1] * edge2[0]};
        float magnitude = sqrt((norm[0] * norm[0]) + (norm[1] * norm[1]) + (norm[2] * norm[2]));
        float unit[3] = {norm[0]/magnitude, norm[1]/magnitude, norm[2]/magnitude};
	    normal[0] = unit[0];
        normal[1] = unit[1];
        normal[2] = unit[2];
    }

    void setPosition() {
        vert_pos[0] = vertices[0]->position[0];
        vert_pos[1] = vertices[0]->position[1];
        vert_pos[2] = vertices[0]->position[2];
        vert_pos[3] = vertices[1]->position[0];
        vert_pos[4] = vertices[1]->position[1];
        vert_pos[5] = vertices[1]->position[2];
        vert_pos[6] = vertices[2]->position[0];
        vert_pos[7] = vertices[2]->position[1];
        vert_pos[8] = vertices[2]->position[2];
    }
};