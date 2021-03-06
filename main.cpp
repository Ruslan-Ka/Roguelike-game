#include "common.h"
#include "Image.h"
#include "Enemy.h"
#include "Player.h"
#include <fstream>
#include <unistd.h>
#include <map>

#define GLFW_DLL
#include <GLFW/glfw3.h>

Image floor("./resources/tiles/floor.png");
Image wall("./resources/tiles/wall.png");
Image exit1("./resources/tiles/exit.png");
Image exit2("./resources/tiles/exit1.png");
Image hole("./resources/tiles/hole.png");
Image trap1("./resources/tiles/trap1.png");
Image trap2("./resources/tiles/trap2.png");
Image trap3("./resources/tiles/trap3.png");

Enemy enemy_character;
constexpr GLsizei WINDOW_WIDTH = 1024, WINDOW_HEIGHT = 1024;
const int PLAN_WIDTH = 7, PLAN_HEIGHT = 5;
double b = 0;

struct InputState
{
  bool keys[1024]{}; //массив состояний кнопок - нажата/не нажата
  GLfloat lastX = 400, lastY = 300; //исходное положение мыши
  bool firstMouse = true;
  bool captureMouse         = true;  // Мышка захвачена нашим приложением или нет?
  bool capturedMouseJustNow = false;
} static Input;


GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;


void OnKeyboardPressed(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
		break;
  case GLFW_KEY_1:
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    break;
  case GLFW_KEY_2:
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    break;
	default:
		if (action == GLFW_PRESS)
      Input.keys[key] = true;
		else if (action == GLFW_RELEASE)
      Input.keys[key] = false;
	}
}

void processPlayerMovement(Player &player, char *card, int &check_switch) {
    if (Input.keys[GLFW_KEY_W]) {
    player.mode = 1;
    player.ProcessInput(MovementDir::UP, card, check_switch);
}
    else if (Input.keys[GLFW_KEY_S]) {
        player.mode = 1;
        player.ProcessInput(MovementDir::DOWN, card, check_switch);
        }
    if (check_switch != 0) {
        return;
    }
    if (Input.keys[GLFW_KEY_A]) {
        player.mode = 2;
        player.ProcessInput(MovementDir::LEFT, card, check_switch);
    }
    else if (Input.keys[GLFW_KEY_D]) {
        player.mode = 1;
        player.ProcessInput(MovementDir::RIGHT, card, check_switch);
    }
    if (Input.keys[GLFW_KEY_E]) {
        player.mode = 4;
    }
}

void OnMouseButtonClicked(GLFWwindow* window, int button, int action, int mods)
{
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    Input.captureMouse = !Input.captureMouse;

  if (Input.captureMouse)
  {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    Input.capturedMouseJustNow = true;
  }
  else
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

}

void OnMouseMove(GLFWwindow* window, double xpos, double ypos)
{
  if (Input.firstMouse)
  {
    Input.lastX = float(xpos);
    Input.lastY = float(ypos);
    Input.firstMouse = false;
  }

  GLfloat xoffset = float(xpos) - Input.lastX;
  GLfloat yoffset = Input.lastY - float(ypos);

  Input.lastX = float(xpos);
  Input.lastY = float(ypos);
}


int initGL()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return -1;
	}

	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

  std::cout << "Controls: "<< std::endl;
  std::cout << "press right mouse button to capture/release mouse cursor  "<< std::endl;
  std::cout << "W, A, S, D - movement  "<< std::endl;
  std::cout << "press ESC to exit" << std::endl;

	return 0;
}


void draw_health(Image &screenBuffer, Player &player);
void read_card(std::string &path, char *card)
{
    std::ifstream fin;
    fin.open(path);
    for(int i = 0; i < 32; i++) {
        std::string cur;
        std::getline(fin, cur);
        for (int j = 0; j < 32; j++) {
            card[i * 32 + j] = cur[j];
        }
    }
    fin.close();
}

void read_plan(char *plan)
{
    std::ifstream fin;
    fin.open("./resources/plan.txt");
    for (int i = 0; i < PLAN_HEIGHT; i++) {
        for (int j = 0; j < PLAN_WIDTH; j++) {
            char cur;
            fin >> cur;
            plan[i * PLAN_WIDTH + j] = cur;
        }
    }
    fin.close();
}


void draw(Image &screenBuffer, Point p, char flag)
{
    if(flag == '.' || flag == '@') {
        for(int i = 0; i < tileSize; i++) {
            for(int j = 0; j < tileSize; j++) {
                Pixel pix = floor.GetPixel(i,j);
                screenBuffer.PutPixel(p.x + i, p.y + j, pix);
                screenBuffer.PutPixel_const(p.x + i, p.y + j, pix);
            }
        }
    } else if(flag == '#') {
        for(int i = 0; i < tileSize; i++) {
            for(int j = 0; j < tileSize; j++) {
                Pixel pix = wall.GetPixel(i, j);
                screenBuffer.PutPixel(p.x + i, p.y + j, pix);
                screenBuffer.PutPixel_const(p.x + i, p.y + j, pix);
            }
        }
    } else if(flag == 'x') {
        for(int i = 0; i < tileSize; i++) {
            for(int j = 0; j < tileSize; j++) {
                Pixel pix = exit1.GetPixel(i, j);
                screenBuffer.PutPixel(p.x + i, p.y + j, pix);
                screenBuffer.PutPixel_const(p.x + i, p.y + j, pix);
            }
        }
    } else if(flag == ' ') {
        for(int i = 0; i < tileSize; i++) {
            for(int j = 0; j < tileSize; j++) {
                Pixel pix = hole.GetPixel(i, j);
                screenBuffer.PutPixel(p.x + i, p.y + j, pix);
                screenBuffer.PutPixel_const(p.x + i, p.y + j, pix);
            }
        }
    } else if(flag == 'Q') {
        for(int i = 0; i < tileSize; i++) {
            for(int j = 0; j < tileSize; j++) {
                Pixel pix = exit2.GetPixel(i, j);
                screenBuffer.PutPixel(p.x + i, p.y + j, pix);
                screenBuffer.PutPixel_const(p.x + i, p.y + j, pix);
            }
        }
    } else if(flag == 'E') {
        for(int i = 0; i < tileSize; i++) {
            for(int j = 0; j < tileSize; j++) {
                Pixel pix = floor.GetPixel(i,j);
                screenBuffer.PutPixel(p.x + i, p.y + j, pix);
                screenBuffer.PutPixel_const(p.x + i, p.y + j, pix);
            }
        }
        enemy_character.enemy_coords.push_back(p);
    }
    else if(flag == 'T') {
        screenBuffer.trap_vector.push_back(p);
        for(int i = 0; i < tileSize; i++) {
            for(int j = 0; j < tileSize; j++) {
                Pixel pix = floor.GetPixel(i, j);
                screenBuffer.PutPixel(p.x + i, p.y + j, pix);
                screenBuffer.PutPixel_const(p.x + i, p.y + j, pix);
            }
        }
        for(int i = 0; i < tileSize; i++) {
            for(int j = 0; j < tileSize; j++) {
                Pixel pix = trap1.GetPixel(i, 32 - j - 1);
                Pixel old_pix = screenBuffer.GetPixel(p.x + i, p.y + j);
                pix = blend(old_pix, pix);
                screenBuffer.PutPixel(p.x + i, p.y + j, pix);
                screenBuffer.PutPixel_const(p.x + i, p.y + j, pix);
            }
        }
    }
}

char *draw_card(Image &screenBuffer, std::string &card_path)
{
    char *card = new char[32*32];
    read_card(card_path, card);
    for(int i = 0, cardx = 31; i < WINDOW_HEIGHT; i += tileSize, --cardx) {
        for(int j = 0, cardy = 0; j < WINDOW_WIDTH; j += tileSize, ++cardy) {
            char c = card[cardx * 32 + cardy];
            if (c == '@' || c == 'E') {
                card[cardx * 32 + cardy] = '.';
            }
            Point p {.x = j, .y = i};
            draw(screenBuffer, p, c);
        }
    }
    return card;
}

void  switch_card(Image &screenBuffer, Player &player, char *&card, const char *plan)
{
    enemy_character.enemy_coords.clear();
    enemy_character.coords.clear();
    enemy_character.old_coords.clear();
    enemy_character.mode.clear();
    enemy_character.enemy_status.clear();
    enemy_character.enemy_count.clear();
    enemy_character.last_time.clear();
    enemy_character.enemy_dir.clear();
    enemy_character.health.clear();

    std::string cur_card;
    if(plan[player.Map_pos()] == 'A') {
        cur_card = "./resources/A.txt";
    } else if (plan[player.Map_pos()] == 'B') {
        cur_card = "./resources/B.txt";
    } else if (plan[player.Map_pos()] == 'C') {
        cur_card = "./resources/C.txt";
    } else if (plan[player.Map_pos()] == 'D') {
        cur_card = "./resources/D.txt";
    } else if (plan[player.Map_pos()] == 'E') {
        cur_card = "./resources/E.txt";
    } else if (plan[player.Map_pos()] == 'F') {
        cur_card = "./resources/F.txt";
    } else if (plan[player.Map_pos()] == 'W') {
        cur_card = "./resources/W.txt";
    } else if (plan[player.Map_pos()] == 'G') {
        cur_card = "./resources/G.txt";
    } else if (plan[player.Map_pos()] == 'J') {
        cur_card = "./resources/J.txt";
    } else if (plan[player.Map_pos()] == 'K') {
        cur_card = "./resources/K.txt";
    } else if (plan[player.Map_pos()] == 'L') {
        cur_card = "./resources/L.txt";
    }
    screenBuffer.trap_vector.clear();
    card = draw_card(screenBuffer, cur_card);
    Point new_pos = player.Get_coords();
    int new_health = player.health;
    player = Player(new_pos, player.Map_pos());
    player.health = new_health;
    draw_health(screenBuffer, player);
    b = 0;
    for (int i = 0; i < enemy_character.enemy_coords.size(); i++) {
        enemy_character.coords.push_back(enemy_character.enemy_coords[i]);
        enemy_character.old_coords.push_back(enemy_character.enemy_coords[i]);
        enemy_character.mode.push_back(1);
        enemy_character.enemy_status.push_back(1);
        enemy_character.enemy_count.push_back(0);
        enemy_character.last_time.push_back(0);
        enemy_character.enemy_dir.push_back(0);
        enemy_character.health.push_back(true);
    }
}

void draw_back(Image &screenBuffer, int check_switch)
{
    std::string path;
    if (check_switch == 2) {
        path = "./resources/tiles/lose.jpg";
    } else if(check_switch == 3){
        path = "./resources/tiles/win.jpg";
    }
    Image img(path);
    for (int i = 0; i < WINDOW_WIDTH; i++) {
        for (int j = 0; j < WINDOW_HEIGHT; j++) {
            Pixel pix = img.GetPixel(i,j);
            screenBuffer.PutPixel(i,WINDOW_WIDTH - j - 1,pix);
        }
    }
}

void draw_current_trap(Image &screenBuffer, Image &trap)
{
    for (int k = 0; k < screenBuffer.trap_vector.size(); k++) {
        Point p = screenBuffer.trap_vector[k];
        for(int i = 0; i < tileSize; i++) {
            for(int j = 0; j < tileSize; j++) {
                Pixel pix = floor.GetPixel(i, j);
                screenBuffer.PutPixel(p.x + i, p.y + j, pix);
                screenBuffer.PutPixel_const(p.x + i, p.y + j, pix);
            }
        }
        for(int i = 0; i < tileSize; i++) {
            for(int j = 0; j < tileSize; j++) {
                Pixel pix = trap.GetPixel(i, 32 - j - 1);
                Pixel old_pix = screenBuffer.GetPixel(p.x + i, p.y + j);
                pix = blend(old_pix, pix);
                screenBuffer.PutPixel(p.x + i, p.y + j, pix);
                screenBuffer.PutPixel_const(p.x + i, p.y + j, pix);
            }
        }
    }
}
void draw_trap(Image &screenBuffer)
{
    std::string path;
    if(screenBuffer.Trap() == 1) {
        draw_current_trap(screenBuffer, trap1);
    } else if (screenBuffer.Trap() == 2) {
        draw_current_trap(screenBuffer, trap2);
    } else if (screenBuffer.Trap() == 3) {
        draw_current_trap(screenBuffer, trap3);
    }
}

void draw_health(Image &screenBuffer, Player &player) {
    int health = player.health;
    std::string path;
    for (int k = 0; k < 3; k++) {
        Point p = player.health_point;
        p.x += k*32;
        if (k < health) {
            path = "./resources/tiles/heart2.png";
        } else {
            path = "./resources/tiles/wall.png";
        }
        Image img(path);
        for(int i = 0; i < tileSize; i++) {
            for(int j = 0; j < tileSize; j++) {
                Pixel pix = img.GetPixel(i, 32 - j - 1);
                Pixel old_pix = screenBuffer.GetPixel_const(p.x + i, p.y + j);
                pix = blend(old_pix, pix);
                screenBuffer.PutPixel(p.x + i, p.y + j, pix);
                screenBuffer.PutPixel_const(p.x + i, p.y + j, pix);
            }
        }
    }
}

int main(int argc, char** argv)
{
	if(!glfwInit())
    return -1;
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow*  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "task1 base project", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	
	glfwMakeContextCurrent(window); 

	glfwSetKeyCallback        (window, OnKeyboardPressed);  
	glfwSetCursorPosCallback  (window, OnMouseMove); 
    glfwSetMouseButtonCallback(window, OnMouseButtonClicked);

	if(initGL() != 0) 
		return -1;



  //Reset any OpenGL errors which could be present for some reason
	GLenum gl_error = glGetError();
	while (gl_error != GL_NO_ERROR)
		gl_error = glGetError();


	Image screenBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, 4);


	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);  GL_CHECK_ERRORS;
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); GL_CHECK_ERRORS;

    char *plan = new char[35];
    read_plan(plan);

    std::string map_path;
    map_path = "./resources/A.txt";
    char *card = new char[32*32];
    read_card(map_path, card);

    Point starting_pos{.x = WINDOW_WIDTH / 3, .y = WINDOW_HEIGHT / 2};
    for(int i = 0, cardx = 31; i < WINDOW_HEIGHT; i += tileSize, --cardx) {
        for(int j = 0, cardy = 0; j < WINDOW_WIDTH; j += tileSize, ++cardy) {
            char c = card[cardx * 32 + cardy];
            Point p {.x = j, .y = i};
            if (c == 'E') {
                card[cardx * 32 + cardy] = '.';
            }
            if (c == '@') {
                card[cardx * 32 + cardy] = '.';
                starting_pos = p;
            }
        }
    }
    Player player{starting_pos};

	draw_card(screenBuffer, map_path); // draw current map
	draw_health(screenBuffer, player);
	bool trap = false; //is the player on a trap
	int check_switch = 0;
	bool check_trap = false; // change trap status
	bool check_enemy = false;
    // game loop
    for (int i = 0; i < enemy_character.enemy_coords.size(); i++) {
        enemy_character.coords.push_back(enemy_character.enemy_coords[i]);
        enemy_character.old_coords.push_back(enemy_character.enemy_coords[i]);
        enemy_character.mode.push_back(1);
        enemy_character.enemy_status.push_back(1);
        enemy_character.enemy_count.push_back(0);
        enemy_character.last_time.push_back(0);
        enemy_character.enemy_dir.push_back(0);
        enemy_character.health.push_back(true);
    }
    while (!glfwWindowShouldClose(window))
    {
        check_enemy = false;
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();

        check_trap = screenBuffer.Trap_count(currentFrame);
        if (check_trap) {
            draw_trap(screenBuffer);
        }

        check_switch = 0; // 0 - nothing, 1 - door, 2 - death, 3 - win
        player.mode = 0;
        processPlayerMovement(player, card, check_switch);
        player.Draw(screenBuffer, currentFrame);

        for(int i = 0; i < enemy_character.enemy_coords.size(); i++) {
            if( (31 - (player.coords.y + 16) / 32) * 32 + (player.coords.x + 40) / 32 == ((31 - (enemy_character.coords[i].y + 16) / 32) * 32 + (enemy_character.coords[i].x + 20) / 32)) {
                if (player.mode == 4) {
                    enemy_character.health[i] = false;
                }
            }
        }

        for (int i = 0; i < enemy_character.enemy_coords.size(); i++) {
                enemy_character.ProcessInput(card, currentFrame, i);
                enemy_character.Draw(screenBuffer, currentFrame, i);
        }
        for(int i = 0; i < enemy_character.enemy_coords.size(); i++) {
            if( (31 - (player.coords.y + 16) / 32) * 32 + (player.coords.x + 20) / 32 == ((31 - (enemy_character.coords[i].y + 16) / 32) * 32 + (enemy_character.coords[i].x + 20) / 32)) {
                if (enemy_character.health[i] == true) {
                    check_enemy = true;
                }
            }
        }

        player.Check_trap(card, trap);

        if ((trap && screenBuffer.Trap() == 3) || check_enemy == true) {
            player.mode = 3;
            player.Draw(screenBuffer, currentFrame);
            if(currentFrame - player.immune_count > 1.5) {
                player.health--;
                if (player.health == 0) {
                    check_switch = 2;
                } else {
                    draw_health(screenBuffer, player);
                    player.immune_count = currentFrame;
                    player.mode = 3;
                }
            }
        }
        if (check_switch == 1) {
            double a = 1;
            while (a > 0) {
                for (int i = 0; i < 1024; i++) {
                    for(int j = 0; j < 1024; j++) {
                        Pixel old_pix = screenBuffer.GetPixel(i,j);
                        Pixel pix = old_pix;
                        pix.r = (int)(a*pix.r);
                        pix.g = (int)(a*pix.g);
                        pix.b = (int)(a*pix.b);
                        screenBuffer.PutPixel(i,j,pix);

                    }
                }
                a -= 0.1;
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
                glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
                glfwSwapBuffers(window);
            }
            switch_card(screenBuffer, player, card, plan);
        } else if (check_switch == 2 || check_switch == 3) {
            delete []card;
            draw_back(screenBuffer, check_switch);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
            glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
            glfwSwapBuffers(window);
            if (check_switch == 2 || check_switch == 3) {
                sleep(3);
                break;
            }
        }
        if (b <= 1) {
            for (int i = 0; i < 1024; i++) {
                for(int j = 0; j < 1024; j++) {
                    Pixel old_pix = screenBuffer.GetPixel_const(i,j);
                    Pixel pix = old_pix;
                    pix.r = (int)(b*pix.r);
                    pix.g = (int)(b*pix.g);
                    pix.b = (int)(b*pix.b);
                    screenBuffer.PutPixel(i,j,pix);

                }
            }
            b += 0.1;
        }
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            GL_CHECK_ERRORS;
            glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data());
            GL_CHECK_ERRORS;
            glfwSwapBuffers(window);
  }
    delete []plan;

  glfwTerminate();
  return 0;
}
