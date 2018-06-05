/* 
OpenGL Template for INM376 / IN3005
City University London, School of Mathematics, Computer Science and Engineering
Source code drawn from a number of sources and examples, including contributions from
 - Ben Humphrey (gametutorials.com), Michal Bubner (mbsoftworks.sk), Christophe Riccio (glm.g-truc.net)
 - Christy Quinn, Sam Kellett and others

 For educational use by Department of Computer Science, City University London UK.

 This template contains a skybox, simple terrain, camera, lighting, shaders, texturing

 Potential ways to modify the code:  Add new geometry types, shaders, change the terrain, load new meshes, change the lighting, 
 different camera controls, different shaders, etc.
 
 Template version 5.0a 29/01/2017
 Dr Greg Slabaugh (gregory.slabaugh.1@city.ac.uk) 
*/


#include "game.h"


// Setup includes
#include <random>
#include "HighResolutionTimer.h"
#include "GameWindow.h"

// Game includes
#include "Camera.h"
#include "Skybox.h"
#include "Plane.h"
#include "Shaders.h"
#include "FreeTypeFont.h"
#include "MatrixStack.h"
#include "OpenAssetImportMesh.h"
#include "Audio.h"
#include "CatmullRom.h"
#include "Cube.h"
#include "Torus.h"

// Constructor
Game::Game()
{
	m_pSkybox = NULL;
	m_pCamera = NULL;
	m_pShaderPrograms = NULL;
	m_pPlanarTerrain = NULL;
	m_pFtFont = NULL;
	m_pRhinoMesh = NULL;
	m_pJeepMesh = NULL;
	m_pHighResolutionTimer = NULL;
	m_pAudio = NULL;
	m_pCatmullRom = NULL;
	m_pCube = NULL;
	m_pTorus = NULL;

	m_dt = 0.0;
	m_framesPerSecond = 0;
	m_frameCount = 0;
	m_elapsedTime = 0.0f;

	m_currentDistance = 0.0f;
	m_cameraSpeed = 0.05f;

	m_rhinoPos = glm::vec3(0.0f, 0.0f, 0.0f);
	m_rhinoRotation = 0.0f;

	m_playerMovement = 0.0f;

	m_thirdPerson = true;
	m_topDown = false;
	m_freeCamera = false;

	m_pickupTimer = 0.0;
	m_pickupPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	m_pickupRotation = 0.0f;

	m_playerScore = 0;
	m_playerLives = 3;

	m_gameOver = false;
}

// Destructor
Game::~Game() 
{ 
	//game objects
	delete m_pCamera;
	delete m_pSkybox;
	delete m_pPlanarTerrain;
	delete m_pFtFont;
	delete m_pRhinoMesh;
	delete m_pJeepMesh;
	delete m_pAudio;
	delete m_pCatmullRom;
	delete m_pCube;
	delete m_pTorus;

	if (m_pShaderPrograms != NULL) {
		for (unsigned int i = 0; i < m_pShaderPrograms->size(); i++)
			delete (*m_pShaderPrograms)[i];
	}
	delete m_pShaderPrograms;

	//setup objects
	delete m_pHighResolutionTimer;
}

// Initialisation:  This method only runs once at startup
void Game::Initialise() 
{
	// Set the clear colour and depth
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClearDepth(1.0f);

	/// Create objects
	m_pCamera = new CCamera;
	m_pSkybox = new CSkybox;
	m_pShaderPrograms = new vector <CShaderProgram *>;
	m_pPlanarTerrain = new CPlane;
	m_pFtFont = new CFreeTypeFont;
	m_pRhinoMesh = new COpenAssetImportMesh;
	m_pJeepMesh = new COpenAssetImportMesh;
	m_pAudio = new CAudio;
	m_pCatmullRom = new CCatmullRom;
	m_pCube = new CCube;
	m_pTorus = new CTorus;

	RECT dimensions = m_gameWindow.GetDimensions();

	int width = dimensions.right - dimensions.left;
	int height = dimensions.bottom - dimensions.top;

	// Set the orthographic and perspective projection matrices based on the image size
	m_pCamera->SetOrthographicProjectionMatrix(width, height); 
	m_pCamera->SetPerspectiveProjectionMatrix(45.0f, (float) width / (float) height, 0.5f, 5000.0f);

	// Load shaders
	vector<CShader> shShaders;
	vector<string> sShaderFileNames;
	sShaderFileNames.push_back("mainShader.vert");
	sShaderFileNames.push_back("mainShader.frag");
	sShaderFileNames.push_back("textShader.vert");
	sShaderFileNames.push_back("textShader.frag");

	for (int i = 0; i < (int) sShaderFileNames.size(); i++) {
		string sExt = sShaderFileNames[i].substr((int) sShaderFileNames[i].size()-4, 4);
		int iShaderType;
		if (sExt == "vert") iShaderType = GL_VERTEX_SHADER;
		else if (sExt == "frag") iShaderType = GL_FRAGMENT_SHADER;
		else if (sExt == "geom") iShaderType = GL_GEOMETRY_SHADER;
		else if (sExt == "tcnl") iShaderType = GL_TESS_CONTROL_SHADER;
		else iShaderType = GL_TESS_EVALUATION_SHADER;
		CShader shader;
		shader.LoadShader("resources\\shaders\\"+sShaderFileNames[i], iShaderType);
		shShaders.push_back(shader);
	}

	// Create the main shader program
	CShaderProgram *pMainProgram = new CShaderProgram;
	pMainProgram->CreateProgram();
	pMainProgram->AddShaderToProgram(&shShaders[0]);
	pMainProgram->AddShaderToProgram(&shShaders[1]);
	pMainProgram->LinkProgram();
	m_pShaderPrograms->push_back(pMainProgram);

	// Create a shader program for fonts
	CShaderProgram *pFontProgram = new CShaderProgram;
	pFontProgram->CreateProgram();
	pFontProgram->AddShaderToProgram(&shShaders[2]);
	pFontProgram->AddShaderToProgram(&shShaders[3]);
	pFontProgram->LinkProgram();
	m_pShaderPrograms->push_back(pFontProgram);

	// You can follow this pattern to load additional shaders

	// Create the skybox
	// Skybox downloaded from http://www.akimbo.in/forum/viewtopic.php?f=10&t=9
	m_pSkybox->Create(2500.0f);
	
	// Create the planar terrain
	m_pPlanarTerrain->Create("resources\\textures\\", "grassfloor01.jpg", 2000.0f, 2000.0f, 50.0f); // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013

	m_pFtFont->LoadSystemFont("arial.ttf", 32);
	m_pFtFont->SetShaderProgram(pFontProgram);

	// Load some meshes in OBJ format
	m_pRhinoMesh->Load("resources\\models\\Rhino\\rhino.obj");
	m_pJeepMesh->Load("resources\\models\\Jeep\\jeep1.obj");

	glEnable(GL_CULL_FACE);

	// Create a cube
	m_pCube->Create("resources\\textures\\woodencrate01.jpg"); // Texture downloaded from https://www.flickr.com/photos/filterforge/9292874886

	// Create a torus
	m_pTorus->Create("resources\\textures\\gold.jpg"); // Texture downloaded from https://pixabay.com/en/background-gold-texture-yellow-1126920/

	// Initialise audio and play background music
	m_pAudio->Initialise();
	m_pAudio->LoadEventSound("resources\\Audio\\Boing.wav");					// Royalty free sound from freesound.org
	m_pAudio->LoadMusicStream("resources\\Audio\\DST-Garote.mp3");	// Royalty free music from http://www.nosoapradio.us/
	m_pAudio->PlayMusicStream();
	
	m_pCatmullRom->CreateCentreline();
	m_pCatmullRom->CreateOffsetCurves();
	m_pCatmullRom->CreateTrack("resources\\textures\\sand.jpg");
}

// Render method runs repeatedly in a loop
void Game::Render() 
{
	// Clear the buffers and enable depth testing (z-buffering)
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// Set up a matrix stack
	glutil::MatrixStack modelViewMatrixStack;
	modelViewMatrixStack.SetIdentity();

	// Use the main shader program 
	CShaderProgram *pMainProgram = (*m_pShaderPrograms)[0];
	pMainProgram->UseProgram();
	pMainProgram->SetUniform("bUseTexture", true);
	pMainProgram->SetUniform("sampler0", 0);
	// Note: cubemap and non-cubemap textures should not be mixed in the same texture unit.  Setting unit 10 to be a cubemap texture.
	int cubeMapTextureUnit = 10; 
	pMainProgram->SetUniform("CubeMapTex", cubeMapTextureUnit);
	

	// Set the projection matrix
	pMainProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());

	// Call LookAt to create the view matrix and put this on the modelViewMatrix stack. 
	// Store the view matrix and the normal matrix associated with the view matrix for later (they're useful for lighting -- since lighting is done in eye coordinates)
	modelViewMatrixStack.LookAt(m_pCamera->GetPosition(), m_pCamera->GetView(), m_pCamera->GetUpVector());
	glm::mat4 viewMatrix = modelViewMatrixStack.Top();
	glm::mat3 viewNormalMatrix = m_pCamera->ComputeNormalMatrix(viewMatrix);

	
	// Set light and materials in main shader program
	glm::vec4 lightPosition1 = glm::vec4(-100, 100, -100, 1); // Position of light source *in world coordinates*
	pMainProgram->SetUniform("light1.position", viewMatrix*lightPosition1); // Position of light source *in eye coordinates*
	pMainProgram->SetUniform("light1.La", glm::vec3(1.0f));		// Ambient colour of light
	pMainProgram->SetUniform("light1.Ld", glm::vec3(1.0f));		// Diffuse colour of light
	pMainProgram->SetUniform("light1.Ls", glm::vec3(1.0f));		// Specular colour of light
	pMainProgram->SetUniform("material1.Ma", glm::vec3(1.0f));	// Ambient material reflectance
	pMainProgram->SetUniform("material1.Md", glm::vec3(0.0f));	// Diffuse material reflectance
	pMainProgram->SetUniform("material1.Ms", glm::vec3(0.0f));	// Specular material reflectance
	pMainProgram->SetUniform("material1.shininess", 15.0f);		// Shininess material property
		

	// Render the skybox and terrain with full ambient reflectance 
	modelViewMatrixStack.Push();
		pMainProgram->SetUniform("renderSkybox", true);
		// Translate the modelview matrix to the camera eye point so skybox stays centred around camera
		glm::vec3 vEye = m_pCamera->GetPosition();
		modelViewMatrixStack.Translate(vEye);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pSkybox->Render(cubeMapTextureUnit);
		pMainProgram->SetUniform("renderSkybox", false);
	modelViewMatrixStack.Pop();

	// Render the planar terrain
	modelViewMatrixStack.Push();
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pPlanarTerrain->Render();
	modelViewMatrixStack.Pop();

	// Turn on diffuse + specular materials
	pMainProgram->SetUniform("material1.Ma", glm::vec3(0.5f));	// Ambient material reflectance
	pMainProgram->SetUniform("material1.Md", glm::vec3(0.5f));	// Diffuse material reflectance
	pMainProgram->SetUniform("material1.Ms", glm::vec3(1.0f));	// Specular material reflectance	

	// Render the Rhino
	modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(m_rhinoPos);
		//modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), -90.0f);
		modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), -m_rhinoRotation);
		//modelViewMatrixStack.Scale(2.0f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pRhinoMesh->Render();
	modelViewMatrixStack.Pop();


	// Render jeeps
	for (int i = 0; i < m_jeeps.size(); i++) {
		if (m_jeeps[i].GetActive() == true) {
			modelViewMatrixStack.Push();
				modelViewMatrixStack.Translate(m_jeeps[i].GetPosition());
				modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 90.0f);
				modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), -m_jeeps[i].GetRotation());
				//modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 180.0f);
				modelViewMatrixStack.Scale(2.5f);
				pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
				pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
				m_pJeepMesh->Render();
			modelViewMatrixStack.Pop();
		}
	}

	// Render boxes
	for (int i = 0; i < m_cubes.size(); i++) {
		if (m_cubes[i].GetActive() == true) {
			modelViewMatrixStack.Push();
				modelViewMatrixStack.Translate(m_cubes[i].GetPosition());
				modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 90.0f);
				modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), -m_cubes[i].GetRotation());
				//modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 180.0f);
				modelViewMatrixStack.Scale(2.5f);
				pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
				pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
				m_pCube->Render();
			modelViewMatrixStack.Pop();
		}
	}

	// Render Torus
	for (int i = 0; i < m_torus.size(); i++) {
		if (m_torus[i].GetActive() == true) {
			modelViewMatrixStack.Push();
				modelViewMatrixStack.Translate(m_torus[i].GetPosition());
				modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 90.0f);
				modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), -m_torus[i].GetRotation());
				//modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 180.0f);
				modelViewMatrixStack.Scale(0.5f);
				pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
				pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
				m_pTorus->Render();
			modelViewMatrixStack.Pop();
		}
	}

	/*
	// Draw centreline
	modelViewMatrixStack.Push();
		//pMainProgram->SetUniform("bUseTexture", true); // turn off texturing
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix",
			m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pCatmullRom->RenderCentreline();
		m_pCatmullRom->RenderOffsetCurves();
		m_pCatmullRom->RenderTrack();
	modelViewMatrixStack.Pop();
	*/

	/*
	// Draw offset curve
	modelViewMatrixStack.Push();
		pMainProgram->SetUniform("bUseTexture", false); // turn off texturing
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix",
			m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pCatmullRom->RenderOffsetCurves();
	modelViewMatrixStack.Pop();
	*/

	// Draw track
	modelViewMatrixStack.Push();
		pMainProgram->SetUniform("bUseTexture", true); // turn off texturing
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix",
			m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pCatmullRom->RenderTrack();
	modelViewMatrixStack.Pop();


	// Draw the 2D graphics after the 3D graphics
	//DisplayFrameRate();
	//DisplayPlayerPos();
	DisplayLivesLeft();
	DisplayScore();

	if (m_gameOver == true)
	{
		DisplayGameover();
	}

	// Swap buffers to show the rendered image
	SwapBuffers(m_gameWindow.Hdc());		

}

// Update method runs repeatedly with the Render method
void Game::Update()
{
	if (m_playerLives <= 0)
	{
		m_gameOver = true;
	}

	if (m_gameOver != true)
	{
		m_currentDistance += m_dt * m_cameraSpeed;

		// work out current point on the path the camera should be at
		glm::vec3 p;
		m_pCatmullRom->Sample(m_currentDistance, p);

		// work out a point further up the path to work out the direction the path is going
		glm::vec3 pNext;
		m_pCatmullRom->Sample((m_currentDistance + 1.0f), pNext);

		// work out the tangent between the camera postion and the direction the path is going - to find the direction the camera should be pointed in
		glm::vec3 T = pNext - p;
		T = glm::normalize(T);

		// N vector in TNB frame - the vector going left and right
		glm::vec3 N = glm::cross(T, glm::vec3(0.0f, 1.0f, 0.0f));
		N = glm::normalize(N);

		// B vector in TNB frame - the vector going up and down
		glm::vec3 B = glm::cross(N, T);
		B = glm::normalize(B);

		if (m_thirdPerson == true) {
			m_pCamera->Set(glm::vec3(p.x, p.y + 25.0f, p.z), (p + (50.0f * T)), glm::vec3(0, 1, 0));
		}
		else if (m_topDown == true) {
			m_pCamera->Set(glm::vec3(p.x, p.y + 50.0f, p.z), (p + (12.0f * T)), glm::vec3(0, 1, 0));
		}
		else if (m_freeCamera == true) {
			// Update the camera using the amount of time that has elapsed to avoid framerate dependent motion
			m_pCamera->Update(m_dt);
		}

		//update rhino/player position
		if (m_thirdPerson == true) {
			m_rhinoPos = (glm::vec3(p.x, p.y + 5.0f, p.z) + (30.0f * T)) + (m_playerMovement * N);
		}
		else if (m_topDown == true) {
			m_rhinoPos = (glm::vec3(p.x, p.y + 5.0f, p.z) + (2.0f * T)) + (m_playerMovement * N);
		}

		//update rhino/player rotation to face the right way
		m_rhinoRotation = atan2(T.z, T.x) * (180.0f / M_PI);

		m_pickupTimer += m_dt;

		glm::vec3 pos;
		m_pCatmullRom->Sample(m_currentDistance + 150.f, pos);

		if (m_pickupTimer > 5000.0) {
			// work out pickup position
			// Randomness for position
			std::random_device rd; // obtain a random number from hardware
			std::mt19937 eng(rd()); // seed the generator
			std::uniform_int_distribution<> distr(-18, 18); // define the range
			m_pickupPosition = /*glm::vec3(m_rhinoPos.x, m_rhinoPos.y - 5.0f, m_rhinoPos.z) + (100.0f * T)*/ glm::vec3(pos.x, pos.y, pos.z) + ((float)distr(eng) * N);
			// work out pickup position relative to player for rotation
			glm::vec3 direction = m_rhinoPos - m_pickupPosition;
			m_pickupRotation = atan2(direction.z, direction.x) * (180.0f / M_PI);
			// number of pickups/obstacle
			std::random_device rd2; // obtain a random number from hardware
			std::mt19937 eng2(rd2()); // seed the generator
			std::uniform_int_distribution<> distr2(1, 2); // define the range
			int randomNumber = distr2(eng2);
			if (randomNumber == 1)
			{
				// type of pickup/obstacle
				// Randomness for what type of pickup
				std::random_device rd3; // obtain a random number from hardware
				std::mt19937 eng3(rd3()); // seed the generator
				std::uniform_int_distribution<> distr3(1, 6); // define the range
				int randomNumber2 = distr3(eng3);
				if (randomNumber2 <= 3)
				{
					CCube cube;
					cube.SetPosition(glm::vec3(m_pickupPosition.x, m_pickupPosition.y + 5.0f, m_pickupPosition.z));
					cube.SetRotation(m_pickupRotation);
					m_cubes.push_back(cube);
				}
				else if (randomNumber2 <= 5)
				{
					m_jeeps.push_back(CJeep(m_pickupPosition, m_pickupRotation));
				}
				else if (randomNumber2 == 6)
				{
					CTorus torus;
					torus.SetPosition(glm::vec3(m_pickupPosition.x, m_pickupPosition.y + 8.0f, m_pickupPosition.z));
					torus.SetRotation(m_pickupRotation);
					m_torus.push_back(torus);
				}
			}
			else if (randomNumber == 2)
			{
				// type of pickup/obstacle
				// Randomness for what type of pickup
				std::random_device rd3; // obtain a random number from hardware
				std::mt19937 eng3(rd3()); // seed the generator
				std::uniform_int_distribution<> distr3(1, 6); // define the range
				int randomNumber2 = distr3(eng3);
				if (randomNumber2 <= 3)
				{
					CCube cube;
					cube.SetPosition(glm::vec3(m_pickupPosition.x, m_pickupPosition.y + 5.0f, m_pickupPosition.z));
					cube.SetRotation(m_pickupRotation);
					CCube cube2;
					cube.SetPosition(glm::vec3(glm::vec3(m_pickupPosition.x, m_pickupPosition.y + 5.0f, m_pickupPosition.z) + (15.0f * N)));
					cube2.SetRotation(m_pickupRotation);
					m_cubes.push_back(cube);
					m_cubes.push_back(cube2);
				}
				else if (randomNumber2 <= 5)
				{
					m_jeeps.push_back(CJeep(m_pickupPosition, m_pickupRotation));
					m_jeeps.push_back(CJeep(glm::vec3(m_pickupPosition + (15.0f * N)), m_pickupRotation));
				}
				else if (randomNumber2 == 6)
				{
					CTorus torus;
					torus.SetPosition(glm::vec3(m_pickupPosition.x, m_pickupPosition.y + 8.0f, m_pickupPosition.z));
					torus.SetRotation(m_pickupRotation);
					CTorus torus2;
					torus2.SetPosition(glm::vec3(glm::vec3(m_pickupPosition.x, m_pickupPosition.y + 8.0f, m_pickupPosition.z) + (15.0f * N)));
					torus2.SetRotation(m_pickupRotation);
					m_torus.push_back(torus);
					m_torus.push_back(torus2);
				}
			}
			m_pickupTimer = 0.0f;
		}

		vector<CJeep>::iterator iter = m_jeeps.begin();
		for (iter; iter != m_jeeps.end(); iter++)
		{
			glm::vec3 distance = glm::vec3(m_rhinoPos - iter->GetPosition());
			if (abs(distance.x) < 10.0f && abs(distance.y) < 5.1f && abs(distance.z) < 10.0f && iter->GetActive() == true)
			{
				m_playerLives -= 1;
				iter->SetActive(false);
			}

			iter->Update(m_dt);

		}

		vector<CCube>::iterator iter2 = m_cubes.begin();
		for (iter2; iter2 != m_cubes.end(); iter2++)
		{
			glm::vec3 distance = glm::vec3(m_rhinoPos - iter2->GetPosition());
			if (abs(distance.x) < 10.0f && abs(distance.y) < 5.1f && abs(distance.z) < 10.0f && iter2->GetActive() == true)
			{
				m_playerScore += 1;
				iter2->SetActive(false);
			}

			iter2->Update(m_dt);
		}

		vector<CTorus>::iterator iter3 = m_torus.begin();
		for (iter3; iter3 != m_torus.end(); iter3++)
		{
			glm::vec3 distance = glm::vec3(m_rhinoPos - iter3->GetPosition());
			if (abs(distance.x) < 10.0f && abs(distance.y) < 5.1f && abs(distance.z) < 10.0f && iter3->GetActive() == true)
			{
				m_playerScore += 10;
				iter3->SetActive(false);
			}

			iter3->Update(m_dt);
		}

	}

	/*
	static float t = 0.0f;
	t += 0.0005f * (float)m_dt;
	if (t > 1.0f) {
		t = 0.0f;
	}
	*/

	m_pAudio->Update();

}

void Game::DisplayLivesLeft()
{
	CShaderProgram *fontProgram = (*m_pShaderPrograms)[1];

	RECT dimensions = m_gameWindow.GetDimensions();
	int height = dimensions.bottom - dimensions.top;

	// Use the font shader program and render the text
	fontProgram->UseProgram();
	glDisable(GL_DEPTH_TEST);
	fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1));
	fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
	fontProgram->SetUniform("vColour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	if (m_playerLives < 0) {
		m_playerLives = 0;
	}
	m_pFtFont->Render(20, height - 20, 20, "Lives: %i", m_playerLives);
}

void Game::DisplayScore()
{
	CShaderProgram *fontProgram = (*m_pShaderPrograms)[1];

	RECT dimensions = m_gameWindow.GetDimensions();
	int height = dimensions.bottom - dimensions.top;
	int width = dimensions.right - dimensions.left;

	// Use the font shader program and render the text
	fontProgram->UseProgram();
	glDisable(GL_DEPTH_TEST);
	fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1));
	fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
	fontProgram->SetUniform("vColour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	m_pFtFont->Render(width - 200, height - 20, 20, "Score: %i", m_playerScore);
}

void Game::DisplayGameover()
{
	CShaderProgram *fontProgram = (*m_pShaderPrograms)[1];

	RECT dimensions = m_gameWindow.GetDimensions();
	int height = dimensions.bottom - dimensions.top;
	int width = dimensions.right - dimensions.left;

	// Use the font shader program and render the text
	fontProgram->UseProgram();
	glDisable(GL_DEPTH_TEST);
	fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1));
	fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
	fontProgram->SetUniform("vColour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	m_pFtFont->Render(width/2, height/2, 20, "GAME OVER");
}

void Game::DisplayFrameRate()
{
	CShaderProgram *fontProgram = (*m_pShaderPrograms)[1];

	RECT dimensions = m_gameWindow.GetDimensions();
	int height = dimensions.bottom - dimensions.top;

	// Increase the elapsed time and frame counter
	m_elapsedTime += m_dt;
	m_frameCount++;

	// Now we want to subtract the current time by the last time that was stored
	// to see if the time elapsed has been over a second, which means we found our FPS.
	if (m_elapsedTime > 1000)
    {
		m_elapsedTime = 0;
		m_framesPerSecond = m_frameCount;

		// Reset the frames per second
		m_frameCount = 0;
    }

	if (m_framesPerSecond > 0) {
		// Use the font shader program and render the text
		fontProgram->UseProgram();
		glDisable(GL_DEPTH_TEST);
		fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1));
		fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
		fontProgram->SetUniform("vColour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		m_pFtFont->Render(20, height - 20, 20, "FPS: %d", m_framesPerSecond);
	}
}

void Game::DisplayPlayerPos()
{
	CShaderProgram *fontProgram = (*m_pShaderPrograms)[1];

	RECT dimensions = m_gameWindow.GetDimensions();
	int height = dimensions.bottom - dimensions.top;
	int width = dimensions.right - dimensions.left;

	// Use the font shader program and render the text
	fontProgram->UseProgram();
	glDisable(GL_DEPTH_TEST);
	fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1));
	fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
	fontProgram->SetUniform("vColour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	m_pFtFont->Render(width - 500, height - 20, 20, "Player Position: %f, %f, %f", m_rhinoPos.x, m_rhinoPos.y, m_rhinoPos.z);
}


// The game loop runs repeatedly until game over
void Game::GameLoop()
{
	/*
	// Fixed timer
	dDt = pHighResolutionTimer->Elapsed();
	if (dDt > 1000.0 / (double) Game::FPS) {
		pHighResolutionTimer->Start();
		Update();
		Render();
	}
	*/
	
	
	// Variable timer
	m_pHighResolutionTimer->Start();
	Update();
	Render();
	m_dt = m_pHighResolutionTimer->Elapsed();

}


WPARAM Game::Execute() 
{
	m_pHighResolutionTimer = new CHighResolutionTimer;
	m_gameWindow.Init(m_hInstance);

	if(!m_gameWindow.Hdc()) {
		return 1;
	}

	Initialise();

	m_pHighResolutionTimer->Start();

	
	MSG msg;

	while(1) {													
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { 
			if(msg.message == WM_QUIT) {
				break;
			}

			TranslateMessage(&msg);	
			DispatchMessage(&msg);
		} else if (m_appActive) {
			GameLoop();
		} 
		else Sleep(200); // Do not consume processor power if application isn't active
	}

	m_gameWindow.Deinit();

	return(msg.wParam);
}

LRESULT Game::ProcessEvents(HWND window,UINT message, WPARAM w_param, LPARAM l_param) 
{
	LRESULT result = 0;

	switch (message) {


	case WM_ACTIVATE:
	{
		switch(LOWORD(w_param))
		{
			case WA_ACTIVE:
			case WA_CLICKACTIVE:
				m_appActive = true;
				m_pHighResolutionTimer->Start();
				break;
			case WA_INACTIVE:
				m_appActive = false;
				break;
		}
		break;
		}

	case WM_SIZE:
			RECT dimensions;
			GetClientRect(window, &dimensions);
			m_gameWindow.SetDimensions(dimensions);
		break;

	case WM_PAINT:
		PAINTSTRUCT ps;
		BeginPaint(window, &ps);
		EndPaint(window, &ps);
		break;

	case WM_KEYDOWN:
		switch(w_param) {
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		case '1':
			m_pAudio->PlayEventSound();
			break;
		case 'A':
			m_playerMovement -= 0.3 * m_dt;
			if (m_playerMovement < -25.0f)
			{
				m_playerMovement = -25.0f;
			}
			break;
		case 'D':
			m_playerMovement += 0.3 * m_dt;
			if (m_playerMovement > 25.0f)
			{
				m_playerMovement = 25.0f;
			}
			break;
		case '3':
			m_thirdPerson = true;
			m_topDown = false;
			m_freeCamera = false;
			break;
		case 'T':
			m_topDown = true;
			m_thirdPerson = false;
			m_freeCamera = false;
			break;
		case 'F':
			m_freeCamera = true;
			m_thirdPerson = false;
			m_topDown = false;
			break;
		case VK_F1:
			m_pAudio->PlayEventSound();
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		result = DefWindowProc(window, message, w_param, l_param);
		break;
	}

	return result;
}

Game& Game::GetInstance() 
{
	static Game instance;

	return instance;
}

void Game::SetHinstance(HINSTANCE hinstance) 
{
	m_hInstance = hinstance;
}

LRESULT CALLBACK WinProc(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
	return Game::GetInstance().ProcessEvents(window, message, w_param, l_param);
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE, PSTR, int) 
{
	Game &game = Game::GetInstance();
	game.SetHinstance(hinstance);

	return game.Execute();
}
