Douglas C Marques
Processamento Gráfico
Olá, primeiramente gostaria de explicar que não consegui colocar no git, pois quem fez isso da ultima vez tinha sido um colega que não fez comigo agora o segundo trabalho e não consegui colocar lá então pedi ajuda denovo e acabei de enviar para o git o meu projeto.
Video de apresentação

Compilação: 
Abrir a solução InstaFilter.sln (pode ser encontrada dentro da pasta baixada ou clonada OpenGL-InstaFilter-main/InstaFilter) no Visual Studio 2019;
Verificar o modo de Debug da solução, este deve estar em x86 na barra de feramentas superior do Visual Studio. Caso não esteja, deverá ser mudado para x86;
Clicar em Depurar Local do Windows na barra de ferramentas superior do Visual Studio ou depurar através do atalho F5 para executar a aplicação.
Problemas de execução :
Caso apareça algum erro de shader, modifique o número versão do core na primeira linha dos arquivos transformation.fs e transformation.vs, para uma inferior, exemplo: #version 400 core, ou menor;
Caso tenha problema com as dependências, logo depois ao abrir o projeto no VS, faça a configuração do diretório das dependências como ensinado em aula.


