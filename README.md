# Fundamentos-Computacao-Grafica-TF

## Visão Geral
Trabalho final da cadeira de fundamentos de computação gráfica, onde será desenvolvido um jogo em C/C++ utilizando a biblioteca OpenGL

O jogo será baseado em uma aplicação já existente chamada Bhop League. Desenvolveremos um jogo de parkour onde o usuário controlará o personagem virtual em primeira pessoa e terá que
passar por uma série de leveis que desafiarão as habilidades do jogador de maneira dinâmica e divertida.

O que é parkour? 
-A palavra “parkour”, em tradução livre do francês, significa “percurso” e trata-se do deslocamento de um ponto A para o ponto B de modo rápido e direto. Isso faz com que os movimentos sejam, em sua maioria, compostos por saltos e corrida

FONTE : https://vidasaudavel.einstein.br/parkour/#:~:text=A%20palavra%20“parkour”%2C%20em,compostos%20por%20saltos%20e%20corridas.

Definições:
* O percurso do jogo será circular, ou seja o jogador começará no level 1 e terminará no level 1. 
* Não terá telas de carregamento entre fases
* O jogador poderá voltar a qualquer momento para a fase anterior ( exceto se tentar voltar para fase anterior a fase 1 )
* Caso o jogador erre ao pular de uma plataforma para outra durante o percurso de uma fase , ele não será penalizado e poderá tentar novamente quantas vezes quiser.


Segue abaixo um exemplo no jogo Minecraft:
https://www.youtube.com/watch?v=ZCPt78a1eLc


---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
## Contribuição de cada Integrante:
O presente trabalho foi realizado pelos alunos:
- João Vitro do Amaral Spolavore
- João Pedro Sanchotene Silva

Distruição das atividades:
- Malhas Poligonais Complexas [ Realizada por João Spolavore ]
- Transformações Geométricas Controladas pelo Usuário [ Realizada por João Spolavore ]
- Modelos de Interpolação de Phong e Gouraud [ Realizada por João Sanchonete ]
- Movimentação com curva Bézier cúbica [ Realizada por João Sanchonete ]
- Animações baseadas no tempo [ Realizada por João Sanchonete ]
- Camera livre e camera look-at [ Realizada por João Spolavore ]
- Três tipos de testes de intersecção [ Realizada por João Spolavore ]
- Modelos de Iluminação Difusa e Blinn-Phon [ Realizada por João Sanchonete ]
- Mapeamento de texturas em todos os objetos [ Realizada por João Spolavore ]
- Instâncias de objetos [ Realizada por João Spolavore ]

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
## Utilização de auxílio de Inteligência Artificial

  - Durante a realização do trabalho foi utilizado o auxílio do CHAT GPT 3.5 .  A ferramenta foi utilizada para adquirir ferramentas e tecnologias disponíveis em C++, uma vez que nenhum dos membros da dupla tinha pleno conhecimento da linguagem. Pode-se citar a utilização de vector e funções de manipulamento desses vetores como, por exemplo, a função abaixo:

![image](https://github.com/Spolavore/Fundamentos-Computacao-Grafica-TF/assets/75754941/d98a1b95-e7bd-4db3-8938-8eee16fb6a84)

  - Ademais, foi utilizado a mesma tecnologia para a explicação de conceitos matemáticos como, distância entre dois pontos e colisão entre ponto e plano. Em Suma, para as consultas utilizadas, a ferramente se demonstrou acertiva com as respostas - contribuindo para a realização e facilitação do trabalho. Os conceitos matemáticos consultados também se demonstraram acertivos e ajudaram a dupla e compreender o que deveria ser implementado para a realização de uma certa funcionalidade dentro do jogo.


---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
## Implementações dos conceitos de Computação Gráfica

### Interação com o usuários
   O jogo disponibiliza interação com usuário em tempo real, para isso foi utilizada funções disponíveis pela biblioteca GLFW, como , por exemplo:
     * [     glfwSetKeyCallback     ]
     * [ glfwSetMouseButtonCallback ]
     * [  glfwSetCursorPosCallback  ]
     * [  glfwSetScrollCallback     ]
  Cada uma dessas funções recebe como parametro uma outra função Callback -  programadas para responder cada ação específica previstas pelos desenvolvedor. Podemos citar, a função KeyCallback , a qual sempre será chamada quando um usuário pressionar alguma
  tecla do teclado. Essa função ajusta variáveis de controle de movimentação , mudanças na transformação geométrica de objetos específicos da cena virtual, entre outras coisas. Segue um exemplo de controle que essa função realiza

![image](https://github.com/Spolavore/Fundamentos-Computacao-Grafica-TF/assets/75754941/fca8ca5c-81e4-4382-9a13-a84e5e5dd4fd)

Para o controle do mouse e suas derivações, as funções glfwSetScrollCallback , glfwSetMouseButtonCallback, glfwSetCursorPosCallback recebem funções Callback específicas para cada uma das suas aplicações:
  * A função glfwSetCursorPosCallback recebe a função Callback CursorPosCallback - função callback chamada sempre que o usuário movimentar o cursor do mouse em cima da janela OpenGL. Segue um exemplo de uma das fucionalidades implementadas por essa função Callback:

    ![image](https://github.com/Spolavore/Fundamentos-Computacao-Grafica-TF/assets/75754941/72a9ffb5-7518-45a8-bf28-b1710ccf16fc)

 * A função glfwSetScrollCallback recebe como Callback a função ScrollCallback, a qual sempre é chamada quando o usuário movimenta o scroll do mouse ( " rodinha " ). Segue um exemplo de uma das funcionalidades implementadas por essa função Callback:
 
   ![image](https://github.com/Spolavore/Fundamentos-Computacao-Grafica-TF/assets/75754941/b1e2b645-684f-4091-94ff-66e68e96c6b0)

 * A função glfwSetMouseButtonCallback recebe como função callback a função MouseButtonCallback, a qual sempre é chamada quando um dos botões do mouse é clicado. Segue um exemplo de uma das funcionalidades implementadas por essa função Callback:

     ![image](https://github.com/Spolavore/Fundamentos-Computacao-Grafica-TF/assets/75754941/bf8317d9-f3db-4196-b668-af6a53724df4)

### Movimentação com Curva Bézier Cúbica Curva de Bezier e Animações Baseadas no Tempo
 * A função CurvaBezier recebe o tempo em segundos (double), a função utiliza quatro pontos (hard coded) para computar três vetores, estes vetores são somados aos pontos para gerar novos três pontos que geram dois novos vetor, e assim em diante, de forma que ao final temos apenas um ponto. Este ponto é retornado pela função e somado com a posição das esferas para move-las em uma curva de Bezier cúbica, que é calculada utilizando o tempo de duração do programa.
 * 
     ![image](https://github.com/Spolavore/Fundamentos-Computacao-Grafica-TF/assets/84470734/e7ef54c8-0d62-4ad9-864d-29c16ec1b0db)

### Modelos de Interpolação de Phong e Gouraud
  * A única diferença entre ambas interpolações é que a de Phong é calculada no fragment shader, enquanto que a de Gouraud é calculada no vertex shader. Para isso utilizamos uma variável 'Uniform' para informar os shaders caso o objeto devesse ser calculado com Gouraud. O vetex shader de um objeto com interpolação de Gouraud tem faz tudo que o vertex shader de um objeto com interpolação de Phong faz, porém ele também calcula o valor da cor do objeto e envia a cor, ao chegar no fragment shader ele apenas aplica a cor passada pelo vertex shader à cor de saída. Para a interpolação de Phong utilizamos a posição global do objeto, posição do modelo, normal e cordenadas de textura enviadas pelo vertex shader para calcular a cor/textura do objeto.
#### Shader Vertex
![image](https://github.com/Spolavore/Fundamentos-Computacao-Grafica-TF/assets/84470734/72770869-c39b-44ca-bfd8-12fbf51d3b8b)
#### Shader Fragment
![image](https://github.com/Spolavore/Fundamentos-Computacao-Grafica-TF/assets/84470734/36ffa145-fe39-49da-95c3-72e235ebcd77)

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

## Preview do Jogo
- Plataformas para o usuários conseguir passar de nível:
![image](https://github.com/Spolavore/Fundamentos-Computacao-Grafica-TF/assets/75754941/09d11ffa-14dc-4757-9418-4c7ce035ac51)


- Malhas Poligonais Complexas ( cow.obj )
![image](https://github.com/Spolavore/Fundamentos-Computacao-Grafica-TF/assets/75754941/ccc265e6-05f4-4939-99f1-f5670256a506)


- Camera look-at 
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
## Tutorial de Uso do jogo.

### Movimentação:
  - W : para mover-se para frente
  - A : para mover-se para a esquerda
  - S : para mover-se para trás
  - D : para mover-se para direita
  - ESPAÇO : para pular
  - SHIFT-ESQUERDO: para correr

### Atalhos de interação
  - C : para ativar camera look-at
  - X : para movimentar objetos específicos na horizontal ( sentido positivo )
  - Z : para movimentar objetos específicos na horizontal ( sentido negativo )
    
