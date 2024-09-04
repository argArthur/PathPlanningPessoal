Simulador de planejador de caminhos
===================================

![Aplicativo de planejador de caminhos](img/pathplannersim.jpeg?raw=true "planejador de caminhos")

Uso
---

Nesse software foi implementado um simulador de planejador de caminhos de forma inteligente,o algoritmo implementado para isso foi a busca em árvore a partir do Rapdly Exploring Random Tree (RRT).

Autor
-----

Software desenvolvido por Vinicius Casimiro da Silveira durante a graduação no Bacharelado de Ciências da Computação na UNESP, RA: 221026321 

Dependências do sistema
-----------------------

- [MAKE](https://www.gnu.org/software/make/)
- [SFML](https://www.sfml-dev.org/download.php)

Como configurar ambiente
------------------------

### Linux/Unix

Caso esteja num sistema Debian ou derivados basta ter certeza de que as dependências estão sendo satisfeitas:

```bash
sudo apt-get install libsfml-dev make
```

Depois deve-se compilar os códigos a partir do Makefile rodando no terminal:

```bash
make
```

Para executar o aplicativo, o comando abaixo deve ser executado

```bash
./path_plannning_app
```

ou 
```bash
make run
```

Como usar
---------

### Instruções básicas

Para usar o planejador de caminhos é necessário atribuir dois pontos, o ponto de partida e o ponto objetivo, para isso o botão esquerdo do mouse é utilizado para atribuir localidade ao ponto de inicio e o botão esquerdo do mouse para atribuir o local desejado ao ponto de chegada.
Em seguida, caso seja desejado colocar obstaculos para dificutar o caminho é necessário que o botão do meio seja clicado, assim habilitando o posicionamento de um obstáculo, para que esse seja por fim colocado, o botão esquerdo do mouse deve ser pressionado. Caso algum obstáculo queira ser retirado, basta apenas clicá-lo, que ele sumirá.

### Rodando o planejador de caminhos

Quando os pontos de inicio e chegada forem atribuídos, o botão "Play" pode ser clicado e um caminho será gerado e mostrado em amarelo na tela, tudo que está em cinza corresponde à árvore gerada pelo algoritmo.
No terminal também serão mostrados a quantidade de iterações necessárias para achar o caminho e a distância do caminho.

### Edição e adição

Inicialmente para adicionar outros algortimos os arquivos de código do algoritmo(.h e .cpp)  deve ser adicionado na pasta "/algorithms".
Para utilizar o novo uma instância de um ponteiro para esse algoritmo deve ser criada e iniciada com "nullptr" numa variável global e deverá ser intanciada na função runAlgorithms(...). Na função destroyAlgorithms(), a instancia deve ser deletada e iniciada denovo com "nullptr", por fim, para printar o caminho na tela, um método deve ser criado dentro do algoritmo que fará os caminhos e deverá ser chamado na função printAlgorithms(...).


------
License: [GNU General Public License (GPLv3)](LICENSE.md)