#version 460 core
out vec4 FragColor;
flat in vec3 ourColor; // flat

uniform int u_smoothMode;

void main() {
    FragColor = vec4(ourColor, 1.0);
    
    if (u_smoothMode == 1) {
        if (gl_PointCoord.x > 0.0) {
            float dist = length(gl_PointCoord - 0.5);
            float alpha = 1.0 - smoothstep(0.5, 0.5, dist);
            if (alpha <= 0.0) discard;
            FragColor.a = alpha;
        }
    }

}