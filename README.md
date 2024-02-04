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
O presente trabalho foi realizado pelos alunos
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

Ademais, foi utilizado a mesma tecnologia para a explicação de conceitos matemáticos como, distância entre dois pontos e colisão entre ponto e plano. Em Suma, para as consultas utilizadas, a ferramente se demonstrou acertiva com as respostas - contribuindo para a realização e facilitação do trabalho. Os conceitos matemáticos consultados também se demonstraram acertivos e ajudaram a dupla e compreender o que deveria ser implementado para a realização de uma certa funcionalidade dentro do jogo.


