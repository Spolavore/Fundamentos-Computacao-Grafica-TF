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


### Colisões
  * O jogo possui 3 tipos de colisões:
        - Colisão ponto cubo - Utilizada para detectar a colisão da câmera virtual ( nesse caso o personagem do jogo ) com as caixas onde ele pode pular. Cada vez que uma caixa é instânciada salva-se a matriz de transalação dela e posteriormente é mandado para o arquivo collisions.cpp onde ele processará o vetor com todas as matrizes de translação de todas as caixas. Dentro de collisions.cpp pega-se a bounding box da caixa ( a mesma para todas as caixas da cena, pois é o mesmo objeto ), multiplicase por uma matriz de escalamento que diminui todas as caixas em 90%, dentro de um loop multiplica por cada uma das matrizes de translação das caixas salvas e faz uma checagem para saber os pontos projetados em volta da câmera ( pela soma de vetor + posição da câmera ) entraram em contato com a bounding box de alguma das diversas caixas.
        ![image](https://github.com/Spolavore/Fundamentos-Computacao-Grafica-TF/assets/75754941/29fe582f-af53-4d82-9398-68673dc3a1da)
        ![image](https://github.com/Spolavore/Fundamentos-Computacao-Grafica-TF/assets/75754941/7f018332-77ec-447c-82b3-049432a03667)

       - Colisão ponto plano - Utilizada para saber se o usuário está em contato com alguma das plataformas presentes entre as fazes do jogo. Processo semelhante ao descrito para colisões entre caixas, entretanto fazemos apenas uma projeção de um ponto abaixo da posição da camera e vemos se esse ponto entro em contato com a bounding box da plataforma.
         ![image](https://github.com/Spolavore/Fundamentos-Computacao-Grafica-TF/assets/75754941/1187d342-1afe-45b2-b0c7-ac648469ab28)
         ![image](https://github.com/Spolavore/Fundamentos-Computacao-Grafica-TF/assets/75754941/74bc99b2-b949-4f68-9a59-239c0cc8a412)

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

## Preview do Jogo
- Plataformas para o usuários conseguir passar de nível:
![Captura de tela 2024-02-07 010443](https://github.com/Spolavore/Fundamentos-Computacao-Grafica-TF/assets/84470734/8aa8c67f-7ebf-40dc-9b2b-5fe5d5186a76)


- Malhas Poligonais Complexas ( cow.obj )
![Captura de tela 2024-02-07 010520](https://github.com/Spolavore/Fundamentos-Computacao-Grafica-TF/assets/84470734/7d1595f5-8ee3-4b4c-84f8-6b65e71a5019)


- Camera look-at
![Captura de tela 2024-02-07 010601](https://github.com/Spolavore/Fundamentos-Computacao-Grafica-TF/assets/84470734/3ef04d48-bfbc-4aaa-bf89-6db350eef86e)

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

# Como rodar o jogo 
Recomenda-se fortemente que se utilize a IDE Codeblock, visto que ela já possui um compilador de C/C++ (Não precisando utilizar o cMake)

## Opção 1
- 1) Faça donwload do zip pelo GitHub
- 2) Extraia o arquivo compactado no seu diretório de preferência
- 3) Acesse a pasta bin -> Acesse Release -> Abra o arquivo main.exe
  ![image](https://github.com/Spolavore/Fundamentos-Computacao-Grafica-TF/assets/75754941/f1848f21-8866-4465-82f2-9ec15026aef5)
  ![image](https://github.com/Spolavore/Fundamentos-Computacao-Grafica-TF/assets/75754941/7657ac91-dc1a-4267-9554-b6f13ea4bf2e)
  ![image](https://github.com/Spolavore/Fundamentos-Computacao-Grafica-TF/assets/75754941/0dac5f44-0d74-43e3-b828-7d96d696c1bc)
  ![image](https://github.com/Spolavore/Fundamentos-Computacao-Grafica-TF/assets/75754941/39b755da-d267-4a8b-b92b-cd4a3cc07439)



## Opção 2
- 1) Faça donwload do zip pelo GitHub
- 2) Extraia o arquivo compactado no seu diretório de preferência
- 3) Abra a IDE Codeblocks
- 4) Vá na aba "File" ( No canto superior esquerdo da IDE )
  
  ![image](https://github.com/Spolavore/Fundamentos-Computacao-Grafica-TF/assets/75754941/b842938b-f6ed-4890-aef5-88e0f650921e)


- 5) Clique em "Open"
- 6) Entre na pasta que você extraiu -> Abra o arquivo Laboratorio_5.cbp
  ![image](https://github.com/Spolavore/Fundamentos-Computacao-Grafica-TF/assets/75754941/22c9e79a-f699-48fa-912a-46ea8a2a961b)
  ![image](https://github.com/Spolavore/Fundamentos-Computacao-Grafica-TF/assets/75754941/26a1d70e-524c-41bd-965a-4dbd9abfe396)

- 7) Clique no ícode azul para ele buildar o programa novamente e logo em seguida clique no botão de Play.
  ![image](https://github.com/Spolavore/Fundamentos-Computacao-Grafica-TF/assets/75754941/caf5fcac-dda2-4809-8aea-ff986bc2f00d)
  ![image](https://github.com/Spolavore/Fundamentos-Computacao-Grafica-TF/assets/75754941/863594bc-20a9-4eb8-b3eb-3c36b633608c)

  


