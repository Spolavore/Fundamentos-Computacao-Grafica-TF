#version 330 core

// Atributos de fragmentos recebidos como entrada ("in") pelo Fragment Shader.
// Neste exemplo, este atributo foi gerado pelo rasterizador como a
// interpolação da posição global e a normal de cada vértice, definidas em
// "shader_vertex.glsl" e "main.cpp".
in vec4 position_world;
in vec4 normal;

// Posição do vértice atual no sistema de coordenadas local do modelo.
in vec4 position_model;

// Coordenadas de textura obtidas do arquivo OBJ (se existirem!)
in vec2 texcoords;

// Cor do vértices para interpolação de Gouraud
in vec3 vertex_color;
flat in int gouraud_out;

// Matrizes computadas no código C++ e enviadas para a GPU
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Identificador que define qual objeto está sendo desenhado no momento
#define PLATAFORM 1
#define FLOOR 2
#define WOODFLOOR 3
#define COW 4
#define SPHERE 5
uniform int object_id;

// Parâmetros da axis-aligned bounding box (AABB) do modelo
uniform vec4 bbox_min;
uniform vec4 bbox_max;

// Variáveis para acesso das imagens de textura
uniform sampler2D TextureImage0;
uniform sampler2D TextureImage1;
uniform sampler2D TextureImage2;
uniform sampler2D TextureImage3;

// O valor de saída ("out") de um Fragment Shader é a cor final do fragmento.
out vec4 color;

// Constantes
#define M_PI   3.14159265358979323846
#define M_PI_2 1.57079632679489661923

void main()
{
    // se interpolação for de gouraud então só repassamos o valor de vertex_color
    if(gouraud_out == 1){
        color.a = 1;
        color.rgb = vertex_color;
        return;
    }

    // CALCULO DA COR

    // Obtemos a posição da câmera utilizando a inversa da matriz que define o
    // sistema de coordenadas da câmera.
    vec4 origin = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 camera_position = inverse(view) * origin;

    // O fragmento atual é coberto por um ponto que percente à superfície de um
    // dos objetos virtuais da cena. Este ponto, p, possui uma posição no
    // sistema de coordenadas global (World coordinates). Esta posição é obtida
    // através da interpolação, feita pelo rasterizador, da posição de cada
    // vértice.
    vec4 p = position_world;

    // Normal do fragmento atual, interpolada pelo rasterizador a partir das
    // normais de cada vértice.
    vec4 n = normalize(normal);

    vec4 light_position = vec4(4.0, 4.0, 4.0, 1.0);

    // Vetor que define o sentido da fonte de luz em relação ao ponto atual.
    vec4 l = normalize(light_position - p);

    // Vetor que define o sentido da câmera em relação ao ponto atual.
    vec4 v = normalize(camera_position - p);

    // Vetor e angulo utilizados para calcular a iluminação de blinn-phong                  *ERRO
    vec4 half_way = normalize(v + l);
    float anguloEntre_n_h = dot(n, half_way);

    // Vetor que define o sentido da reflexão especular ideal.
    float r_x = -l.x + 2*n.x*max(dot(n,l), 0);
    float r_y = -l.y + 2*n.y*max(dot(n,l), 0);
    float r_z = -l.z + 2*n.z*max(dot(n,l), 0);

    vec4 r = normalize(vec4(r_x, r_y, r_z,0.0)); // PREENCHA AQUI o vetor de reflexão especular ideal

    // Parâmetros que definem as propriedades espectrais da superfície
    vec3 Kd; // Refletância difusa
    vec3 Ks; // Refletância especular
    vec3 Ka; // Refletância ambiente
    float q; // Expoente especular para o modelo de iluminação de Phong

    // Espectro da fonte de iluminação
    vec3 I = vec3(1.0,1.0,1.0); // PREENCH AQUI o espectro da fonte de luz

    // Espectro da luz ambiente
    vec3 Ia = vec3(0.2,0.2,0.2); // PREENCHA AQUI o espectro da luz ambiente

    if (object_id == SPHERE)
    {
        // Propriedades espectrais da esfera
        Kd = vec3(0.8,0.4,0.08);
        Ks = vec3(0.1,0.1,0.1);
        Ka = vec3(0.4,0.2,0.04);
        q = 50.0;

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

        // NOTE: Se você quiser fazer o rendering de objetos transparentes, é
        // necessário:
        // 1) Habilitar a operação de "blending" de OpenGL logo antes de realizar o
        //    desenho dos objetos transparentes, com os comandos abaixo no código C++:
        //      glEnable(GL_BLEND);
        //      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // 2) Realizar o desenho de todos objetos transparentes *após* ter desenhado
        //    todos os objetos opacos; e
        // 3) Realizar o desenho de objetos transparentes ordenados de acordo com
        //    suas distâncias para a câmera (desenhando primeiro objetos
        //    transparentes que estão mais longe da câmera).
        // Alpha default = 1 = 100% opaco = 0% transparente
        color.a = 1;

        // Cor final do fragmento calculada com uma combinação dos termos difuso,
        // especular, e ambiente. Veja slide 129 do documento Aula_17_e_18_Modelos_de_Iluminacao.pdf.
        color.rgb = lambert_diffuse_term + ambient_term + phong_specular_term;

        // Cor final com correção gamma, considerando monitor sRGB.
        // Veja https://en.wikipedia.org/w/index.php?title=Gamma_correction&oldid=751281772#Windows.2C_Mac.2C_sRGB_and_TV.2Fvideo_standard_gammas
        //color.rgb = vec3(gouraud_out,gouraud_out,gouraud_out);
        color.rgb = pow(color.rgb, vec3(1.0,1.0,1.0)/2.2);
        return;
    }

    // Coordenadas de textura U e V
    float U = 0.0;
    float V = 0.0;
    vec3 Kd0;

    if ( object_id == PLATAFORM )
    {
        // PREENCHA AQUI as coordenadas de textura da esfera, computadas com
        // projeção esférica EM COORDENADAS DO MODELO. Utilize como referência
        // o slides 134-150 do documento Aula_20_Mapeamento_de_Texturas.pdf.
        // A esfera que define a projeção deve estar centrada na posição
        // "bbox_center" definida abaixo.

        // Você deve utilizar:
        //   função 'length( )' : comprimento Euclidiano de um vetor
        //   função 'atan( , )' : arcotangente. Veja https://en.wikipedia.org/wiki/Atan2.
        //   função 'asin( )'   : seno inverso.
        //   constante M_PI
        //   variável position_model

        vec4 bbox_center = (bbox_min + bbox_max) / 2.0;

        float rho = 1;
        vec4 plinha = bbox_center + rho*((position_model-bbox_center)/length(position_model-bbox_center)); // ponto na projeção esférica'
        vec4 pvetor =(plinha - bbox_center); // vetor da projeção esférica a partir da origem

        float teta = atan(pvetor.x, pvetor.z);
        float phi = asin(pvetor.y/rho);


        U = (teta + M_PI)/ (2*M_PI);
        V = (phi + (M_PI/2))/ M_PI;

        Kd0  = texture(TextureImage0, vec2(U,V)).rgb;
    }

    else    if ( object_id == COW )
    {
        // PREENCHA AQUI as coordenadas de textura da esfera, computadas com
        // projeção esférica EM COORDENADAS DO MODELO. Utilize como referência
        // o slides 134-150 do documento Aula_20_Mapeamento_de_Texturas.pdf.
        // A esfera que define a projeção deve estar centrada na posição
        // "bbox_center" definida abaixo.

        // Você deve utilizar:
        //   função 'length( )' : comprimento Euclidiano de um vetor
        //   função 'atan( , )' : arcotangente. Veja https://en.wikipedia.org/wiki/Atan2.
        //   função 'asin( )'   : seno inverso.
        //   constante M_PI
        //   variável position_model

        vec4 bbox_center = (bbox_min + bbox_max) / 2.0;

        float rho = 1;
        vec4 plinha = bbox_center + rho*((position_model-bbox_center)/length(position_model-bbox_center)); // ponto na projeção esférica'
        vec4 pvetor =(plinha - bbox_center); // vetor da projeção esférica a partir da origem

        float teta = atan(pvetor.x, pvetor.z);
        float phi = asin(pvetor.y/rho);


        U = (teta + M_PI)/ (2*M_PI);
        V = (phi + (M_PI/2))/ M_PI;


        Kd0  = texture(TextureImage3, vec2(U,V)).rgb;
    }

    else if ( object_id == WOODFLOOR)
    {
        // PREENCHA AQUI as coordenadas de textura do coelho, computadas com
        // projeção planar XY em COORDENADAS DO MODELO. Utilize como referência
        // o slides 99-104 do documento Aula_20_Mapeamento_de_Texturas.pdf,
        // e também use as variáveis minmax* definidas abaixo para normalizar
        // as coordenadas de textura U e V dentro do intervalo [0,1]. Para
        // tanto, veja por exemplo o mapeamento da variável 'p_v' utilizando
        // 'h' no slides 158-160 do documento Aula_20_Mapeamento_de_Texturas.pdf.
        // Veja também a Questão 4 do Questionário 4 no Moodle.

        float minx = bbox_min.x;
        float maxx = bbox_max.x;

        float miny = bbox_min.y;
        float maxy = bbox_max.y;

        float minz = bbox_min.z;
        float maxz = bbox_max.z;


        U = (position_model.x - minx)/(maxx - minx);
        V = (position_model.y - miny) / (maxy - miny);
        Kd0  = texture(TextureImage2, vec2(U,V)).rgb;
    }

    else if ( object_id == FLOOR )
    {
        // Propriedades espectrais do plano
        Kd = vec3(0.08,0.4,0.8);
        Ks = vec3(0.8,0.8,0.8);
        Ka = vec3(0.04,0.2,0.4);
        q = 3.0;

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

        // Termo especular utilizando o modelo de iluminação de Blinn-Phong
        float produto_r_v_pot_q = pow(max(anguloEntre_n_h, 0.0f), q);
        float phong_specular_x = Ks.x * I.x * produto_r_v_pot_q;
        float phong_specular_y = Ks.y * I.y * produto_r_v_pot_q;
        float phong_specular_z = Ks.z * I.z * produto_r_v_pot_q;

        vec3 phong_specular_term  = vec3(phong_specular_x, phong_specular_y, phong_specular_z); // PREENCH AQUI o termo especular de Phong

        color.a = 1;

        color.rgb = lambert_diffuse_term + ambient_term + phong_specular_term;

        color.rgb = pow(color.rgb, vec3(1.0,1.0,1.0)/2.2);
        return;
    }

    // Obtemos a refletância difusa a partir da leitura da imagem TextureImage0

    // Equação de Iluminação
    float lambert = max(0,dot(n,l));

    color.rgb = Kd0 * (lambert + 0.01);

    // NOTE: Se você quiser fazer o rendering de objetos transparentes, é
    // necessário:
    // 1) Habilitar a operação de "blending" de OpenGL logo antes de realizar o
    //    desenho dos objetos transparentes, com os comandos abaixo no código C++:
    //      glEnable(GL_BLEND);
    //      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // 2) Realizar o desenho de todos objetos transparentes *após* ter desenhado
    //    todos os objetos opacos; e
    // 3) Realizar o desenho de objetos transparentes ordenados de acordo com
    //    suas distâncias para a câmera (desenhando primeiro objetos
    //    transparentes que estão mais longe da câmera).
    // Alpha default = 1 = 100% opaco = 0% transparente
    color.a = 1;

    // Cor final com correção gamma, considerando monitor sRGB.
    // Veja https://en.wikipedia.org/w/index.php?title=Gamma_correction&oldid=751281772#Windows.2C_Mac.2C_sRGB_and_TV.2Fvideo_standard_gammas
    color.rgb = pow(color.rgb, vec3(1.0,1.0,1.0)/2.2);
}

