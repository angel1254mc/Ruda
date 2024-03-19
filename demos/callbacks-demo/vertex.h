#pragma once
struct Vertex {

    float position[3];
    float normal[3];
    float color[3];

    Vertex(float x_pos, float y_pos, float z_pos, float x_norm, float y_norm, float z_norm, float red, float green, float blue)
    {
        setPosition(x_pos, y_pos, z_pos);
        setNormal(x_norm, y_norm, z_norm);
        setColor(red, green, blue);
    };
    Vertex(float position[3], float normal[3], float color[3]) { Vertex(position[0], position[1], position[2], normal[0], normal[1], normal[2], color[0], color[1], color[2]); };
    Vertex() = default;

    // ------- SETTERS ------- //

    void setPosition(float new_x, float new_y, float new_z)
    {
        position[0] = new_x;
        position[1] = new_y;
        position[2] = new_z;
    }

    void setPosition(float new_position[3]) {setPosition(new_position[0], new_position[1], new_position[2]);};

    void setNormal(float new_x, float new_y, float new_z)
    {
        normal[0] = new_x;
        normal[1] = new_y;
        normal[2] = new_z;
    }

    void setNormal(float new_normal[3]) {setNormal(new_normal[0], new_normal[1], new_normal[2]);};

    void setColor(float new_r, float new_g, float new_b)
    {
        color[0] = new_r;
        color[1] = new_g;
        color[2] = new_b;
    }

    void setColor(float new_color[3]) {setColor(new_color[0], new_color[1], new_color[2]);};

    // ------- OP OVERLOADS ------- //
    void operator=(Vertex v) {

        setPosition(v.position);
        setNormal(v.normal);
        setColor(v.color);

    };
};