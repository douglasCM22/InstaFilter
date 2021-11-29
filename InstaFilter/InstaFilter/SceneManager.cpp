#include "SceneManager.h"
#include <string.h>

//static controllers for mouse and keyboard
static bool keys[1024];
static bool resized;
static GLuint width, height;

int channel = 0;
int emoji = 0;
bool moveObj = false;
bool reset = false;
bool changeImage = false;

double posX, posY = 0;
double posX1, posY1 = 0;
char image[50];

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::initialize(GLuint w, GLuint h)
{
	width = w;
	height = h;

	// GLFW - GLEW - OPENGL general setup -- TODO: config file
	initializeGraphics();
}

void SceneManager::initializeGraphics()
{
	// Init GLFW
	glfwInit();

	// Create a GLFWwindow object that we can use for GLFW's functions
	window = glfwCreateWindow(width, height, "InstaFilter", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	//Setando a callback de redimensionamento da janela
	glfwSetWindowSizeCallback(window, resize);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	// Obtendo as informações de versão
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;
	cout << "Caso a versao suportada seja menor que 450, devera ser mudada como mostrado no README.md" << endl;
	cout << "" << endl;
	cout << "InstaFilter - Aplicador de Filtros!" << endl;
	cout << "Instrucoes:            R           - Resetar filtro e stickers." << endl;
	cout << "                       T           - Trocar imagem." << endl;
	cout << "             MOUSE BOTAO ESQUERDO  - Selecionar um filtro ou sticker." << endl;
	cout << "             MOUSE BOTAO DIREITO   - Colocar sticker no lugar do clique." << endl;
	cout << "                      ESC          - Sair." << endl;
	cout << "" << endl;
	
	// Build and compile our shader program
	addShader("../shaders/transformations.vs", "../shaders/transformations.fs");

	//setup the scene -- LEMBRANDO QUE A DESCRIÇÃO DE UMA CENA PODE VIR DE ARQUIVO(S) DE 
	// CONFIGURAÇÃO
	setupScene();

	resized = true; //para entrar no setup da câmera na 1a vez
}

void SceneManager::addShader(string vFilename, string fFilename)
{
	shader = new Shader(vFilename.c_str(), fFilename.c_str());
}


void SceneManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}

	if (key == GLFW_KEY_R && action == GLFW_PRESS) {
		channel = 0;
		reset = true;
	}
	if (key == GLFW_KEY_T && action == GLFW_PRESS) {
		changeImage = true;
	}
}

void SceneManager::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		//getting cursor position
		glfwGetCursorPos(window, &posX, &posY);
		if (posX >= 90 && posX <= 170 && posY >= 490 && posY <= 570) {
			channel = 1;
		}
		else if (posX >= 180 && posX <= 260 && posY >= 490 && posY <= 570) {
			channel = 2;
		}
		else if (posX >= 270 && posX <= 350 && posY >= 490 && posY <= 570) {
			channel = 3;
		}
		else if (posX >= 360 && posX <= 440 && posY >= 490 && posY <= 570) {
			channel = 4;
		}
		else if (posX >= 450 && posX <= 530 && posY >= 490 && posY <= 570) {
			channel = 5;
		}
		else if (posX >= 540 && posX <= 620 && posY >= 490 && posY <= 570) {
			channel = 6;
		}
		else if (posX >= 630 && posX <= 710 && posY >= 490 && posY <= 570) {
			channel = 7;
		}

		else if (posX >= 195 && posX <= 245 && posY >= 25 && posY <= 75) {
			emoji = 8;
		}
		else if (posX >= 265 && posX <= 315 && posY >= 25 && posY <= 75) {
			emoji = 9;
		}
		else if (posX >= 335 && posX <= 385 && posY >= 25 && posY <= 75) {
			emoji = 10;
		}
		else if (posX >= 405 && posX <= 455 && posY >= 25 && posY <= 75) {
			emoji = 11;
		}
		else if (posX >= 475 && posX <= 525 && posY >= 25 && posY <= 75) {
			emoji = 12;
		}
		else if (posX >= 545 && posX <= 595 && posY >= 25 && posY <= 75) {
			emoji = 13;
		}
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS && emoji != 0) {
		glfwGetCursorPos(window, &posX1, &posY1);
		moveObj = true;
	}
}

void SceneManager::resize(GLFWwindow* window, int w, int h)
{
	width = w;
	height = h;
	resized = true;

	// Define the viewport dimensions
	glViewport(0, 0, width, height);
}


void SceneManager::update()
{
	if (keys[GLFW_KEY_ESCAPE])
		glfwSetWindowShouldClose(window, GL_TRUE);

	//AQUI aplicaremos as transformações nos sprites
	objects[7]->setChannel(channel);

	if (emoji != 0 && moveObj == true) {
		objects[emoji]->setPosition(glm::vec3(posX1, 600 - posY1, 0.0f));
		moveObj = false;
	}

	if (changeImage) {
		cout << "Coloque uma imagem na pasta '/textures' e digite o nome dela abaixo: (Ex.: imagem.png ou imagem.jpg)" << endl;
		cin >> image;
		char dir[100] = "../textures/";
		strcat_s(dir, 100, image);
		unsigned int texID = loadTexture(dir);
		objects[7]->setTexture(texID);
		changeImage = false;
	}

	if (reset) {
		for (int i = 8; i < 14; i++)
		{
			objects[i]->setPosition(glm::vec3(1000.0f, 1000.0f, 0.0f));
		}
		reset = false;
	}
	//altera o angulo do segundo objeto
	//objects[1]->setAngle((float)glfwGetTime());
}

void SceneManager::render()
{
	// Clear the colorbuffer
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (resized) //se houve redimensionamento na janela, redefine a projection matrix
	{
		setupCamera2D();
		resized = false;
	}

	//atualiza e desenha os Sprites
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->update();
		objects[i]->draw(objects[i]->getChannel());
	}
}

void SceneManager::run()
{
	//GAME LOOP
	while (!glfwWindowShouldClose(window))
	{

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		//Update method(s)
		update();

		//Render scene
		render();

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
}

void SceneManager::finish()
{
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
}


void SceneManager::setupScene()
{
	//Criação dos Sprites iniciais -- pode-se fazer métodos de criação posteriormente
	//GreyScale
	Sprite* obj = new Sprite;
	obj->setPosition(glm::vec3(130.0f, 70.0f, 0.0));
	obj->setDimension(glm::vec3(80.0f, 80.0f, 1.0f)); //note que depois podemos reescalar conforme tamanho da sprite
	obj->setShader(shader);
	obj->setChannel(1);
	objects.push_back(obj); //adiciona obj

	//Colorized 1
	obj = new Sprite;
	obj->setPosition(glm::vec3(220.0f, 70.0f, 0.0));
	obj->setDimension(glm::vec3(80.0f, 80.0f, 1.0f));
	obj->setShader(shader);
	obj->setChannel(2);
	objects.push_back(obj); //adiciona obj

	//Colorized 2
	obj = new Sprite;
	obj->setPosition(glm::vec3(310.0f, 70.0f, 0.0));
	obj->setDimension(glm::vec3(80.0f, 80.0f, 1.0f));
	obj->setShader(shader);
	obj->setChannel(3);
	objects.push_back(obj); //adiciona obj

	//Colorized 3
	obj = new Sprite;
	obj->setPosition(glm::vec3(400.0f, 70.0f, 0.0));
	obj->setDimension(glm::vec3(80.0f, 80.0f, 1.0f));
	obj->setShader(shader);
	obj->setChannel(4);
	objects.push_back(obj); //adiciona obj

	//Inverted
	obj = new Sprite;
	obj->setPosition(glm::vec3(490.0f, 70.0f, 0.0));
	obj->setDimension(glm::vec3(80.0f, 80.0f, 1.0f));
	obj->setShader(shader);
	obj->setChannel(5);
	objects.push_back(obj); //adiciona obj

	//Binary
	obj = new Sprite;
	obj->setPosition(glm::vec3(580.0f, 70.0f, 0.0));
	obj->setDimension(glm::vec3(80.0f, 80.0f, 1.0f));
	obj->setShader(shader);
	obj->setChannel(6);
	objects.push_back(obj); // adiciona obj

	//Our Filter
	obj = new Sprite;
	obj->setPosition(glm::vec3(670.0f, 70.0f, 0.0));
	obj->setDimension(glm::vec3(80.0f, 80.0f, 1.0f));
	obj->setShader(shader);
	obj->setChannel(7);
	objects.push_back(obj); // adiciona obj

	//Object
	obj = new Sprite;
	obj->setPosition(glm::vec3(400.0f, 330.0f, 0.0));
	obj->setDimension(glm::vec3(300.0f, 300.0f, 1.0f));
	obj->setShader(shader);
	obj->setChannel(channel);
	objects.push_back(obj); // adiciona obj

	// Emoji1
	obj = new Sprite;
	obj->setPosition(glm::vec3(220.0f, 550.0f, 0.0));
	obj->setDimension(glm::vec3(50.0f, 50.0f, 1.0f));
	obj->setShader(shader);
	obj->setChannel(0);
	objects.push_back(obj); // adiciona obj

	// Emoji2
	obj = new Sprite;
	obj->setPosition(glm::vec3(290.0f, 550.0f, 0.0));
	obj->setDimension(glm::vec3(50.0f, 50.0f, 1.0f));
	obj->setShader(shader);
	obj->setChannel(0);
	objects.push_back(obj); // adiciona obj

	// Emoji3
	obj = new Sprite;
	obj->setPosition(glm::vec3(360.0f, 550.0f, 0.0));
	obj->setDimension(glm::vec3(50.0f, 50.0f, 1.0f));
	obj->setShader(shader);
	obj->setChannel(0);
	objects.push_back(obj); // adiciona obj

	// Emoji4
	obj = new Sprite;
	obj->setPosition(glm::vec3(430.0f, 550.0f, 0.0));
	obj->setDimension(glm::vec3(50.0f, 50.0f, 1.0f));
	obj->setShader(shader);
	obj->setChannel(0);
	objects.push_back(obj); // adiciona obj

	// Emoji5
	obj = new Sprite;
	obj->setPosition(glm::vec3(500.0f, 550.0f, 0.0));
	obj->setDimension(glm::vec3(50.0f, 50.0f, 1.0f));
	obj->setShader(shader);
	obj->setChannel(0);
	objects.push_back(obj); // adiciona obj

	// Emoji6
	obj = new Sprite;
	obj->setPosition(glm::vec3(570.0f, 550.0f, 0.0));
	obj->setDimension(glm::vec3(50.0f, 50.0f, 1.0f));
	obj->setShader(shader);
	obj->setChannel(0);
	objects.push_back(obj); // adiciona obj

	// Sticker
	obj = new Sprite;
	obj->setPosition(glm::vec3(110.0f, 550.0f, 0.0));
	obj->setDimension(glm::vec3(150.0f, 100.0f, 1.0f));
	obj->setShader(shader);
	obj->setChannel(0);
	objects.push_back(obj); // adiciona obj

	// Filtro
	obj = new Sprite;
	obj->setPosition(glm::vec3(43.0f, 70.0f, 0.0));
	obj->setDimension(glm::vec3(100.0f, 100.0f, 1.0f));
	obj->setShader(shader);
	obj->setChannel(0);
	objects.push_back(obj); // adiciona obj

	// Emoji1
	obj = new Sprite;
	obj->setPosition(glm::vec3(220.0f, 550.0f, 0.0));
	obj->setDimension(glm::vec3(50.0f, 50.0f, 1.0f));
	obj->setShader(shader);
	obj->setChannel(0);
	objects.push_back(obj); // adiciona obj

	// Emoji2
	obj = new Sprite;
	obj->setPosition(glm::vec3(290.0f, 550.0f, 0.0));
	obj->setDimension(glm::vec3(50.0f, 50.0f, 1.0f));
	obj->setShader(shader);
	obj->setChannel(0);
	objects.push_back(obj); // adiciona obj

	// Emoji3
	obj = new Sprite;
	obj->setPosition(glm::vec3(360.0f, 550.0f, 0.0));
	obj->setDimension(glm::vec3(50.0f, 50.0f, 1.0f));
	obj->setShader(shader);
	obj->setChannel(0);
	objects.push_back(obj); // adiciona obj

	// Emoji4
	obj = new Sprite;
	obj->setPosition(glm::vec3(430.0f, 550.0f, 0.0));
	obj->setDimension(glm::vec3(50.0f, 50.0f, 1.0f));
	obj->setShader(shader);
	obj->setChannel(0);
	objects.push_back(obj); // adiciona obj

	// Emoji5
	obj = new Sprite;
	obj->setPosition(glm::vec3(500.0f, 550.0f, 0.0));
	obj->setDimension(glm::vec3(50.0f, 50.0f, 1.0f));
	obj->setShader(shader);
	obj->setChannel(0);
	objects.push_back(obj); // adiciona obj

	// Emoji6
	obj = new Sprite;
	obj->setPosition(glm::vec3(570.0f, 550.0f, 0.0));
	obj->setDimension(glm::vec3(50.0f, 50.0f, 1.0f));
	obj->setShader(shader);
	obj->setChannel(0);
	objects.push_back(obj); // adiciona obj

	//Carregamento das texturas (pode ser feito intercalado na criação)
	//Futuramente, utilizar classe de materiais e armazenar dimensoes, etc
	unsigned int texID = loadTexture("../textures/party.jpg");
	objects[0]->setTexture(texID);
	objects[1]->setTexture(texID);
	objects[2]->setTexture(texID);
	objects[3]->setTexture(texID);
	objects[4]->setTexture(texID);
	objects[5]->setTexture(texID);
	objects[6]->setTexture(texID);
	objects[7]->setTexture(texID);

	texID = loadTexture("../textures/Emoji1.png");
	objects[8]->setTexture(texID);
	objects[16]->setTexture(texID);
	texID = loadTexture("../textures/Emoji2.png");
	objects[9]->setTexture(texID);
	objects[17]->setTexture(texID);
	texID = loadTexture("../textures/Emoji3.png");
	objects[10]->setTexture(texID);
	objects[18]->setTexture(texID);
	texID = loadTexture("../textures/Emoji4.png");
	objects[11]->setTexture(texID);
	objects[19]->setTexture(texID);
	texID = loadTexture("../textures/Emoji5.png");
	objects[12]->setTexture(texID);
	objects[20]->setTexture(texID);
	texID = loadTexture("../textures/Emoji6.png");
	objects[13]->setTexture(texID);
	objects[21]->setTexture(texID);

	texID = loadTexture("../textures/Emojis.png");
	objects[14]->setTexture(texID);
	texID = loadTexture("../textures/Filtros.png");
	objects[15]->setTexture(texID);

	//Definindo a janela do mundo (ortho2D)
	ortho2D[0] = 0.0f; //xMin
	ortho2D[1] = 800.0f; //xMax
	ortho2D[2] = 0.0f; //yMin
	ortho2D[3] = 600.0f; //yMax

	//Habilita transparência
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void SceneManager::setupCamera2D() //TO DO: parametrizar aqui
{
	float zNear = -1.0, zFar = 1.0; //estão fixos porque não precisamos mudar

	projection = glm::ortho(ortho2D[0], ortho2D[1], ortho2D[2], ortho2D[3], zNear, zFar);

	//Obtendo o identificador da matriz de projeção para enviar para o shader
	GLint projLoc = glGetUniformLocation(shader->ID, "projection");
	//Enviando a matriz de projeção para o shader
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

unsigned int SceneManager::loadTexture(string filename)
{
	unsigned int texture1;

	// load and create a texture 
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
										   // set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load image, create texture and generate mipmaps
	int width, height, nrChannels;

	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		if (nrChannels == 3) //jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else //png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		std::cout << "Aperte I para tentar carregar a imagem novamente e certifique-se de que a mesma está na pasta '/textures'." << std::endl;
	}
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
	glActiveTexture(GL_TEXTURE0);

	return texture1;
}