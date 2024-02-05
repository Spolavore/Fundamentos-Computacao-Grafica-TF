#version 330 core

// Atributos de vértice recebidos como entrada ("in") pelo Vertex Shader.
// Veja a função BuildTrianglesAndAddToVirtualScene() em "main.cpp".
layout (location = 0) in vec4 model_coefficients;
layout (location = 1) in vec4 normal_coefficients;
layout (location = 2) in vec2 texture_coefficients;

// Matrizes computadas no código C++ e enviadas para a GPU
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform int gouraud;

// Identificador que define qual objeto está sendo desenhado no momento
uniform int object_id;

// Atributos de vértice que serão gerados como saída ("out") pelo Vertex Shader.
// ** Estes serão interpolados pelo rasterizador! ** gerando, assim, valores
// para cada fragmento, os quais serão recebidos como entrada pelo Fragment
// Shader. Veja o arquivo "shader_fragment.glsl".
out vec4 position_world;
out vec4 position_model;
out vec4 normal;
out vec2 texcoords;
out vec3 vertex_color;      // contem a cor do vértice caso a interpolação seja de gouraud


void main()
{
    gl_Position = projection * view * model * model_coefficients;

    // Agora definimos outros atributos dos vértices que serão interpolados pelo
    // rasterizador para gerar atributos únicos para cada fragmento gerado.

    // Posição do vértice atual no sistema de coordenadas global (World).
    position_world = model * model_coefficients;

    // Posição do vértice atual no sistema de coordenadas local do modelo.
    position_model = model_coefficients;

    // Normal do vértice atual no sistema de coordenadas global (World).
    // Veja slides 123-151 do documento Aula_07_Transformacoes_Geometricas_3D.pdf.
    normal = inverse(transpose(model)) * normal_coefficients;
    normal.w = 0.0;

    // Coordenadas de textura obtidas do arquivo OBJ (se existirem!)
    texcoords = texture_coefficients;

    // caso o objeto seja interpolado com gouraud fazemos o calculo aqui no shader_vertex
    if(gouraud == 1){
        vec4 origin = vec4(0.0, 0.0, 0.0, 1.0);
        vec4 camera_position = inverse(view) * origin;

        vec4 n = normalize(normal);

        vec4 light_position = vec4(4.0, 4.0, 4.0, 1.0);

        // Vetor que define o sentido da fonte de luz em relação ao ponto atual.
        vec4 l = normalize(light_position - position_world);                                           //MODIFICADO

        // Vetor que define o sentido da iluminação spotlight.
        vec4 v = normalize(camera_position - position_model);

        // Vetor que define o sentido da reflexão especular ideal.
        float r_x = -l.x + 2*n.x*dot(n,l);
        float r_y = -l.y + 2*n.y*dot(n,l);
        float r_z = -l.z + 2*n.z*dot(n,l);
        vec4 r = vec4(r_x, r_y, r_z,0.0);

        // Parâmetros que definem as propriedades espectrais da superfície
        vec3 Kd = vec3(0.8,0.4,0.08); // Refletância difusa
        vec3 Ks = vec3(0.0,0.0,0.0); // Refletância especular
        vec3 Ka = vec3(0.4,0.2,0.04); // Refletância ambiente
        float q = 30.0; // Expoente especular para o modelo de iluminação de Phong

        // Espectro da fonte de iluminação
        vec3 I = vec3(1.0,1.0,1.0); // PREENCH AQUI o espectro da fonte de luz

        // Espectro da luz ambiente
        vec3 Ia = vec3(0.2,0.2,0.2); // PREENCHA AQUI o espectro da luz ambiente

        // Termo difuso utilizando a lei dos cossenos de Lambert
        float lambert_x = Kd.x * I.x * max(0,dot(n,l));
        float lambert_y = Kd.y * I.y * max(0,dot(n,l));
        float lambert_z = Kd.z * I.z * max(0,dot(n,l));
        vec3 lambert_diffuse_term = vec3(lambert_x, lambert_y, lambert_z); // PREENCHA AQUI o termo difuso de Lambert

        // Termo ambiente
        float ambiente_x = Ka.x * Ia.x;
        float ambiente_y = Ka.y * Ia.y;
        float ambiente_z = Ka.z * Ia.z;
        vec3 ambient_term = vec3(ambiente_x, ambiente_y, ambiente_z); // PREENCHA AQUI o termo ambiente

        // Termo especular utilizando o modelo de iluminação de Phong
        float produto_r_v_pot_q = pow(dot(r,v), q);
        float phong_specular_x = Ks.x * I.x * produto_r_v_pot_q;
        float phong_specular_y = Ks.y * I.y * produto_r_v_pot_q;
        float phong_specular_z = Ks.z * I.z * produto_r_v_pot_q;
        vec3 phong_specular_term  = vec3(phong_specular_x, phong_specular_y, phong_specular_z); // PREENCH AQUI o termo especular de Phong

        // Cor final do fragmento calculada com uma combinação dos termos difuso,
        // especular, e ambiente. Veja slide 129 do documento Aula_17_e_18_Modelos_de_Iluminacao.pdf.
        vertex_color.rgb = lambert_diffuse_term + ambient_term + phong_specular_term;

        // Cor final com correção gamma, considerando monitor sRGB.
        // Veja https://en.wikipedia.org/w/index.php?title=Gamma_correction&oldid=751281772#Windows.2C_Mac.2C_sRGB_and_TV.2Fvideo_standard_gammas

        //vertex_color.rgb = vec3(1.0,1.0,1.0);
        vertex_color.rgb = pow(vertex_color.rgb, vec3(1.0,1.0,1.0)/2.2);
    }

}

