#include "math.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_press_callback(GLFWwindow* window, int xpos, int ypos,int zpos);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float fov = 45.0f;
bool firstMouse = true;
bool DragMouse = false;

bool XYFlag = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float xoffset = 0;
float yoffset = 0;

float xrotate = 0;
float yrotate = 0;

// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_press_callback);
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("6.2.coordinate_systems.vs", "6.2.coordinate_systems.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
		-0.2f, -0.2f, -0.2f, 0.0f, 0.0f,
		0.2f, -0.2f, -0.2f, 1.0f, 0.0f,
		0.2f, 0.2f, -0.2f, 1.0f, 1.0f,
		0.2f, 0.2f, -0.2f, 1.0f, 1.0f,
		-0.2f, 0.2f, -0.2f, 0.0f, 1.0f,
		-0.2f, -0.2f, -0.2f, 0.0f, 0.0f,

		-0.2f, -0.2f, 0.2f, 0.0f, 0.0f,
		0.2f, -0.2f, 0.2f, 1.0f, 0.0f,
		0.2f, 0.2f, 0.2f, 1.0f, 1.0f,
		0.2f, 0.2f, 0.2f, 1.0f, 1.0f,
		-0.2f, 0.2f, 0.2f, 0.0f, 1.0f,
		-0.2f, -0.2f, 0.2f, 0.0f, 0.0f,

		-0.2f, 0.2f, 0.2f, 1.0f, 0.0f,
		-0.2f, 0.2f, -0.2f, 1.0f, 1.0f,
		-0.2f, -0.2f, -0.2f, 0.0f, 1.0f,
		-0.2f, -0.2f, -0.2f, 0.0f, 1.0f,
		-0.2f, -0.2f, 0.2f, 0.0f, 0.0f,
		-0.2f, 0.2f, 0.2f, 1.0f, 0.0f,

		0.2f, 0.2f, 0.2f, 1.0f, 0.0f,
		0.2f, 0.2f, -0.2f, 1.0f, 1.0f,
		0.2f, -0.2f, -0.2f, 0.0f, 1.0f,
		0.2f, -0.2f, -0.2f, 0.0f, 1.0f,
		0.2f, -0.2f, 0.2f, 0.0f, 0.0f,
		0.2f, 0.2f, 0.2f, 1.0f, 0.0f,

		-0.2f, -0.2f, -0.2f, 0.0f, 1.0f,
		0.2f, -0.2f, -0.2f, 1.0f, 1.0f,
		0.2f, -0.2f, 0.2f, 1.0f, 0.0f,
		0.2f, -0.2f, 0.2f, 1.0f, 0.0f,
		-0.2f, -0.2f, 0.2f, 0.0f, 0.0f,
		-0.2f, -0.2f, -0.2f, 0.0f, 1.0f,

		-0.2f, 0.2f, -0.2f, 0.0f, 1.0f,
		0.2f, 0.2f, -0.2f, 1.0f, 1.0f,
		0.2f, 0.2f, 0.2f, 1.0f, 0.0f,
		0.2f, 0.2f, 0.2f, 1.0f, 0.0f,
		-0.2f, 0.2f, 0.2f, 0.0f, 0.0f,
		-0.2f, 0.2f, -0.2f, 0.0f, 1.0f
    };
    // world space positions of our cubes
    glm::vec3 cubePositions[] = {

		//midldle layer
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-0.4f, 0.0f, 0.0f),
		glm::vec3(0.4f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -0.4f),
		glm::vec3(-0.4f, 0.0f, -0.4f),
		glm::vec3(0.4f, 0.0f, -0.4f),
		glm::vec3(0.0f, 0.0f, 0.4f),
		glm::vec3(-0.4f, 0.0f, 0.4f),
		glm::vec3(0.4f, 0.0f, 0.4f),

		//up layer
		glm::vec3(0.0f, 0.4f, 0.0f),
		glm::vec3(-0.4f, 0.4f, 0.0f),
		glm::vec3(0.4f, 0.4f, 0.0f),
		glm::vec3(0.0f, 0.4f, -0.4f),
		glm::vec3(-0.4f, 0.4f, -0.4f),
		glm::vec3(0.4f, 0.4f, -0.4f),
		glm::vec3(0.0f, 0.4f, 0.4f),
		glm::vec3(-0.4f, 0.4f, 0.4f),
		glm::vec3(0.4f, 0.4f, 0.4f),

		//sown layer
		glm::vec3(0.0f, -0.4f, 0.0f),
		glm::vec3(-0.4f, -0.4f, 0.0f),
		glm::vec3(0.4f, -0.4f, 0.0f),
		glm::vec3(0.0f, -0.4f, -0.4f),
		glm::vec3(-0.4f, -0.4f, -0.4f),
		glm::vec3(0.4f, -0.4f, -0.4f),
		glm::vec3(0.0f, -0.4f, 0.4f),
		glm::vec3(-0.4f, -0.4f, 0.4f),
		glm::vec3(0.4f, -0.4f, 0.4f),



    };
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // load and create a texture 
    // -------------------------
    unsigned int texture1, texture2;
    // texture 1
    // ---------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    // texture 2
    // ---------
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // activate shader
        ourShader.use();

        // create transformations
        glm::mat4 view;
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		float radius = 10.0f;
		//float camX = sin(glfwGetTime()) * radius;
		//float camZ = cos(glfwGetTime()) * radius;
		xrotate = xrotate + xoffset;
		yrotate = yrotate + yoffset;
		float camX = sin(xrotate) * 4.0f;
		float camY = sin(yrotate) * 4.0f;
		float camZ = sqrt(16.0f - camX*camX - camY*camY);
		//camZ = cos(xrotate) * 4.0f;
		//camZ = cos(yrotate) * 4.0f;
		if (XYFlag = true)
		{
			//camZ = cos(xrotate) * 4.0f;
			XYFlag = false;
		}
		else
		{
		   // camZ = cos(yrotate) * 4.0f;
			XYFlag = true;
		}
		//std::cout << "camX" << camX << std::endl;
		//std::cout << "camY" << camY << std::endl;
		//std::cout << "camZ" << camZ << std::endl;
		//view = glm::lookAt(glm::vec3(camX, camY, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::lookAt(cameraPos * 4.0f, cameraFront, cameraUp);
		//view = glm::lookAt(glm::vec3(0.0f, camY, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        //view       = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        // pass transformation matrices to the shader
        ourShader.setMat4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        ourShader.setMat4("view", view);

        // render boxes
        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 27; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model;
			float n = (float)glfwGetTime();
			if (i < 9)
			{
				model = glm::rotate(model,n , glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::translate(model, cubePositions[i]);
			}
			else
			{
				//model = glm::rotate(model, n, glm::vec3(0.0f, 0.0f, 1.0f));
				model = glm::translate(model, cubePositions[i]);
			}
            //float angle = 20.0f * i;

			//std::cout << "Failed to load texture" << xrotate << std::endl;
			//model = glm::rotate(model, glm::radians(xrotate), glm::vec3(0.0f, 1.0f, 0.0f));
			//model = glm::rotate(model, glm::radians(yrotate), glm::vec3(1.0f, 0.0f, 0.0f));
            ourShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    //glViewport(0, 0, width, height);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

	if (DragMouse == true)
	{
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		xoffset = xpos - lastX;
		yoffset = ypos - lastY;
		lastX = xpos;
		lastY = ypos;
		float sensitivity = 0.1f; // change this value to your liking
		xoffset *= sensitivity;
		yoffset *= sensitivity;
		if ((xoffset < 0.3f) && (xoffset > - 0.3f))
		{
			xoffset = 0;
		}
		if ((yoffset < 0.3f) && (yoffset > -0.3f))
		{
			yoffset = 0;
		}
		yaw += xoffset;
		pitch += yoffset;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraPos = glm::normalize(front);
		//std::cout << "Failed to load texture" << xpos << std::endl;
	}
}

void mouse_press_callback(GLFWwindow* window,  int button, int action,int reserved)
{
	//button:left 0 riht 1 middle 2
	//action:release 0 press 1
	if ((button == 0) && (action == 1))
	{
		DragMouse = true;
		firstMouse = true;
	}
	else if (action == 0)
	{
		DragMouse = false;
		xoffset = 0;
		yoffset = 0;
	}
}