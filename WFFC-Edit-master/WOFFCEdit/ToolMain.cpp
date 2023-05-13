#include "ToolMain.h"
#include "resource.h"
#include <vector>
#include <sstream>

//
//ToolMain Class
ToolMain::ToolMain()
{

	m_currentChunk = 0;		//default value
	m_selectedObject = 0;	//initial selection ID
	m_sceneGraph.clear();	//clear the vector for the scenegraph
	m_databaseConnection = NULL;

	//zero input commands
	m_toolInputCommands.forward		= false;
	m_toolInputCommands.back		= false;
	m_toolInputCommands.left		= false;
	m_toolInputCommands.right		= false;
	m_toolInputCommands.mouse_X = 0;
	m_toolInputCommands.mouse_Y = 0;
	m_toolInputCommands.mouse_LB_Down = false;
	m_toolInputCommands.mouse_RB_Down = false;
	m_toolInputCommands.multipleSelect = false;
	m_toolInputCommands.up = false;
	m_toolInputCommands.down = false;
	m_toolInputCommands.mouse_MB_Down = false;
	m_toolInputCommands.focusCamera = false;
	m_toolInputCommands.objectUp = false;
	m_toolInputCommands.objectDown = false;
	m_toolInputCommands.objectRight = false;
	m_toolInputCommands.objectLeft = false;
	m_toolInputCommands.objectForward = false;
	m_toolInputCommands.objectBackward = false;
}


ToolMain::~ToolMain()
{
	sqlite3_close(m_databaseConnection);		//close the database connection
}


int ToolMain::getCurrentSelectionID()
{
	return m_selectedObject;
}

void ToolMain::onActionInitialise(HWND handle, int width, int height)
{
	//window size, handle etc for directX
	m_width		= width;
	m_height	= height;
	
	m_d3dRenderer.Initialize(handle, m_width, m_height);

	//database connection establish
	int rc;
	rc = sqlite3_open_v2("database/test.db",&m_databaseConnection, SQLITE_OPEN_READWRITE, NULL);

	if (rc) 
	{
		TRACE("Can't open database");
		//if the database cant open. Perhaps a more catastrophic error would be better here
	}
	else 
	{
		TRACE("Opened database successfully");
	}

	onActionLoad();
}

void ToolMain::onActionLoad()
{
	//load current chunk and objects into lists
	if (!m_sceneGraph.empty())		//is the vector empty
	{
		m_sceneGraph.clear();		//if not, empty it
	}

	//SQL
	int rc;
	char *sqlCommand;
	char *ErrMSG = 0;
	sqlite3_stmt *pResults;								//results of the query
	sqlite3_stmt *pResultsChunk;

	//OBJECTS IN THE WORLD
	//prepare SQL Text
	sqlCommand = "SELECT * from Objects";				//sql command which will return all records from the objects table.
	//Send Command and fill result object
	rc = sqlite3_prepare_v2(m_databaseConnection, sqlCommand, -1, &pResults, 0 );
	
	//loop for each row in results until there are no more rows.  ie for every row in the results. We create and object
	while (sqlite3_step(pResults) == SQLITE_ROW)
	{	
		SceneObject newSceneObject;
		newSceneObject.ID = sqlite3_column_int(pResults, 0);
		newSceneObject.chunk_ID = sqlite3_column_int(pResults, 1);
		newSceneObject.model_path		= reinterpret_cast<const char*>(sqlite3_column_text(pResults, 2));
		newSceneObject.tex_diffuse_path = reinterpret_cast<const char*>(sqlite3_column_text(pResults, 3));
		newSceneObject.posX = sqlite3_column_double(pResults, 4);
		newSceneObject.posY = sqlite3_column_double(pResults, 5);
		newSceneObject.posZ = sqlite3_column_double(pResults, 6);
		newSceneObject.rotX = sqlite3_column_double(pResults, 7);
		newSceneObject.rotY = sqlite3_column_double(pResults, 8);
		newSceneObject.rotZ = sqlite3_column_double(pResults, 9);
		newSceneObject.scaX = sqlite3_column_double(pResults, 10);
		newSceneObject.scaY = sqlite3_column_double(pResults, 11);
		newSceneObject.scaZ = sqlite3_column_double(pResults, 12);
		newSceneObject.render = sqlite3_column_int(pResults, 13);
		newSceneObject.collision = sqlite3_column_int(pResults, 14);
		newSceneObject.collision_mesh = reinterpret_cast<const char*>(sqlite3_column_text(pResults, 15));
		newSceneObject.collectable = sqlite3_column_int(pResults, 16);
		newSceneObject.destructable = sqlite3_column_int(pResults, 17);
		newSceneObject.health_amount = sqlite3_column_int(pResults, 18);
		newSceneObject.editor_render = sqlite3_column_int(pResults, 19);
		newSceneObject.editor_texture_vis = sqlite3_column_int(pResults, 20);
		newSceneObject.editor_normals_vis = sqlite3_column_int(pResults, 21);
		newSceneObject.editor_collision_vis = sqlite3_column_int(pResults, 22);
		newSceneObject.editor_pivot_vis = sqlite3_column_int(pResults, 23);
		newSceneObject.pivotX = sqlite3_column_double(pResults, 24);
		newSceneObject.pivotY = sqlite3_column_double(pResults, 25);
		newSceneObject.pivotZ = sqlite3_column_double(pResults, 26);
		newSceneObject.snapToGround = sqlite3_column_int(pResults, 27);
		newSceneObject.AINode = sqlite3_column_int(pResults, 28);
		newSceneObject.audio_path = reinterpret_cast<const char*>(sqlite3_column_text(pResults, 29));
		newSceneObject.volume = sqlite3_column_double(pResults, 30);
		newSceneObject.pitch = sqlite3_column_double(pResults, 31);
		newSceneObject.pan = sqlite3_column_int(pResults, 32);
		newSceneObject.one_shot = sqlite3_column_int(pResults, 33);
		newSceneObject.play_on_init = sqlite3_column_int(pResults, 34);
		newSceneObject.play_in_editor = sqlite3_column_int(pResults, 35);
		newSceneObject.min_dist = sqlite3_column_double(pResults, 36);
		newSceneObject.max_dist = sqlite3_column_double(pResults, 37);
		newSceneObject.camera = sqlite3_column_int(pResults, 38);
		newSceneObject.path_node = sqlite3_column_int(pResults, 39);
		newSceneObject.path_node_start = sqlite3_column_int(pResults, 40);
		newSceneObject.path_node_end = sqlite3_column_int(pResults, 41);
		newSceneObject.parent_id = sqlite3_column_int(pResults, 42);
		newSceneObject.editor_wireframe = sqlite3_column_int(pResults, 43);
		newSceneObject.name = reinterpret_cast<const char*>(sqlite3_column_text(pResults, 44));

		newSceneObject.light_type = sqlite3_column_int(pResults, 45);
		newSceneObject.light_diffuse_r = sqlite3_column_double(pResults, 46);
		newSceneObject.light_diffuse_g = sqlite3_column_double(pResults, 47);
		newSceneObject.light_diffuse_b = sqlite3_column_double(pResults, 48);
		newSceneObject.light_specular_r = sqlite3_column_double(pResults, 49);
		newSceneObject.light_specular_g = sqlite3_column_double(pResults, 50);
		newSceneObject.light_specular_b = sqlite3_column_double(pResults, 51);
		newSceneObject.light_spot_cutoff = sqlite3_column_double(pResults, 52);
		newSceneObject.light_constant = sqlite3_column_double(pResults, 53);
		newSceneObject.light_linear = sqlite3_column_double(pResults, 54);
		newSceneObject.light_quadratic = sqlite3_column_double(pResults, 55);
	

		//send completed object to scenegraph
		m_sceneGraph.push_back(newSceneObject);
	}

	//THE WORLD CHUNK
	//prepare SQL Text
	sqlCommand = "SELECT * from Chunks";				//sql command which will return all records from  chunks table. There is only one tho.
														//Send Command and fill result object
	rc = sqlite3_prepare_v2(m_databaseConnection, sqlCommand, -1, &pResultsChunk, 0);


	sqlite3_step(pResultsChunk);
	m_chunk.ID = sqlite3_column_int(pResultsChunk, 0);
	m_chunk.name = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 1));
	m_chunk.chunk_x_size_metres = sqlite3_column_int(pResultsChunk, 2);
	m_chunk.chunk_y_size_metres = sqlite3_column_int(pResultsChunk, 3);
	m_chunk.chunk_base_resolution = sqlite3_column_int(pResultsChunk, 4);
	m_chunk.heightmap_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 5));
	m_chunk.tex_diffuse_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 6));
	m_chunk.tex_splat_alpha_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 7));
	m_chunk.tex_splat_1_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 8));
	m_chunk.tex_splat_2_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 9));
	m_chunk.tex_splat_3_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 10));
	m_chunk.tex_splat_4_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 11));
	m_chunk.render_wireframe = sqlite3_column_int(pResultsChunk, 12);
	m_chunk.render_normals = sqlite3_column_int(pResultsChunk, 13);
	m_chunk.tex_diffuse_tiling = sqlite3_column_int(pResultsChunk, 14);
	m_chunk.tex_splat_1_tiling = sqlite3_column_int(pResultsChunk, 15);
	m_chunk.tex_splat_2_tiling = sqlite3_column_int(pResultsChunk, 16);
	m_chunk.tex_splat_3_tiling = sqlite3_column_int(pResultsChunk, 17);
	m_chunk.tex_splat_4_tiling = sqlite3_column_int(pResultsChunk, 18);


	//Process REsults into renderable
	m_d3dRenderer.BuildDisplayList(&m_sceneGraph);
	//build the renderable chunk 
	m_d3dRenderer.BuildDisplayChunk(&m_chunk);

}

void ToolMain::onActionSave()
{
	//SQL
	int rc;
	char *sqlCommand;
	char *ErrMSG = 0;
	sqlite3_stmt *pResults;								//results of the query
	

	//OBJECTS IN THE WORLD Delete them all
	//prepare SQL Text
	sqlCommand = "DELETE FROM Objects";	 //will delete the whole object table.   Slightly risky but hey.
	rc = sqlite3_prepare_v2(m_databaseConnection, sqlCommand, -1, &pResults, 0);
	sqlite3_step(pResults);

	//Populate with our new objects
	std::wstring sqlCommand2;
	int numObjects = m_sceneGraph.size();	//Loop thru the scengraph.

	for (int i = 0; i < numObjects; i++)
	{
		std::stringstream command;
		command << "INSERT INTO Objects " 
			<<"VALUES(" << m_sceneGraph.at(i).ID << ","
			<< m_sceneGraph.at(i).chunk_ID  << ","
			<< "'" << m_sceneGraph.at(i).model_path <<"'" << ","
			<< "'" << m_sceneGraph.at(i).tex_diffuse_path << "'" << ","
			<< m_sceneGraph.at(i).posX << ","
			<< m_sceneGraph.at(i).posY << ","
			<< m_sceneGraph.at(i).posZ << ","
			<< m_sceneGraph.at(i).rotX << ","
			<< m_sceneGraph.at(i).rotY << ","
			<< m_sceneGraph.at(i).rotZ << ","
			<< m_sceneGraph.at(i).scaX << ","
			<< m_sceneGraph.at(i).scaY << ","
			<< m_sceneGraph.at(i).scaZ << ","
			<< m_sceneGraph.at(i).render << ","
			<< m_sceneGraph.at(i).collision << ","
			<< "'" << m_sceneGraph.at(i).collision_mesh << "'" << ","
			<< m_sceneGraph.at(i).collectable << ","
			<< m_sceneGraph.at(i).destructable << ","
			<< m_sceneGraph.at(i).health_amount << ","
			<< m_sceneGraph.at(i).editor_render << ","
			<< m_sceneGraph.at(i).editor_texture_vis << ","
			<< m_sceneGraph.at(i).editor_normals_vis << ","
			<< m_sceneGraph.at(i).editor_collision_vis << ","
			<< m_sceneGraph.at(i).editor_pivot_vis << ","
			<< m_sceneGraph.at(i).pivotX << ","
			<< m_sceneGraph.at(i).pivotY << ","
			<< m_sceneGraph.at(i).pivotZ << ","
			<< m_sceneGraph.at(i).snapToGround << ","
			<< m_sceneGraph.at(i).AINode << ","
			<< "'" << m_sceneGraph.at(i).audio_path << "'" << ","
			<< m_sceneGraph.at(i).volume << ","
			<< m_sceneGraph.at(i).pitch << ","
			<< m_sceneGraph.at(i).pan << ","
			<< m_sceneGraph.at(i).one_shot << ","
			<< m_sceneGraph.at(i).play_on_init << ","
			<< m_sceneGraph.at(i).play_in_editor << ","
			<< m_sceneGraph.at(i).min_dist << ","
			<< m_sceneGraph.at(i).max_dist << ","
			<< m_sceneGraph.at(i).camera << ","
			<< m_sceneGraph.at(i).path_node << ","
			<< m_sceneGraph.at(i).path_node_start << ","
			<< m_sceneGraph.at(i).path_node_end << ","
			<< m_sceneGraph.at(i).parent_id << ","
			<< m_sceneGraph.at(i).editor_wireframe << ","
			<< "'" << m_sceneGraph.at(i).name << "'" << ","

			<< m_sceneGraph.at(i).light_type << ","
			<< m_sceneGraph.at(i).light_diffuse_r << ","
			<< m_sceneGraph.at(i).light_diffuse_g << ","
			<< m_sceneGraph.at(i).light_diffuse_b << ","
			<< m_sceneGraph.at(i).light_specular_r << ","
			<< m_sceneGraph.at(i).light_specular_g << ","
			<< m_sceneGraph.at(i).light_specular_b << ","
			<< m_sceneGraph.at(i).light_spot_cutoff << ","
			<< m_sceneGraph.at(i).light_constant << ","
			<< m_sceneGraph.at(i).light_linear << ","
			<< m_sceneGraph.at(i).light_quadratic

			<< ")";
		std::string sqlCommand2 = command.str();
		rc = sqlite3_prepare_v2(m_databaseConnection, sqlCommand2.c_str(), -1, &pResults, 0);
		sqlite3_step(pResults);	
	}
	MessageBox(NULL, L"Objects Saved", L"Notification", MB_OK);
}

void ToolMain::onActionSaveTerrain()
{
	m_d3dRenderer.SaveDisplayChunk(&m_chunk);
}

void ToolMain::Tick(MSG *msg)
{
	if (m_toolInputCommands.mouse_LB_Down)
	{
		m_selectedObject = m_d3dRenderer.MousePicking();
		m_toolInputCommands.mouse_LB_Down = false;
	}

	if (m_toolInputCommands.mouse_RB_Down)
	{
		m_selectedObject = 0;
	}

	if (m_toolInputCommands.multipleSelect && m_toolInputCommands.mouse_LB_Down)
	{
		m_d3dRenderer.MultiSelect();
	}

	if (m_toolInputCommands.objectForward)
	{
		TransformObjectForward();
		UpdateDisplayList();
	}

	if (m_toolInputCommands.objectBackward)
	{
		TransformObjectBackward();
		UpdateDisplayList();
	}

	if (m_toolInputCommands.objectRight)
	{
		TransformObjectRight();
		UpdateDisplayList();
	}

	if (m_toolInputCommands.objectLeft)
	{
		TransformObjectLeft();
		UpdateDisplayList();
	}

	if (m_toolInputCommands.objectUp)
	{
		TransformObjectUp();
		UpdateDisplayList();
	}

	if (m_toolInputCommands.objectDown)
	{
		TransformObjectDown();
		UpdateDisplayList();
	}

	//Renderer Update Call
	m_d3dRenderer.Tick(&m_toolInputCommands);
	
}

void ToolMain::UpdateInput(MSG * msg)
{

	switch (msg->message)
	{
		//Global inputs,  mouse position and keys etc
	case WM_KEYDOWN:
		m_keyArray[msg->wParam] = true;
		break;

	case WM_KEYUP:
		m_keyArray[msg->wParam] = false;
		break;

	case WM_MOUSEMOVE:
		m_toolInputCommands.mouse_X = GET_X_LPARAM(msg->lParam);
		m_toolInputCommands.mouse_Y = GET_Y_LPARAM(msg->lParam);
		break;

	case WM_LBUTTONDOWN:	
		m_toolInputCommands.mouse_LB_Down = true;
		break;

	case WM_LBUTTONUP:
		m_toolInputCommands.mouse_LB_Down = false;
		break;

	case WM_RBUTTONDOWN:
		m_toolInputCommands.mouse_RB_Down = true;
		break;

	case WM_RBUTTONUP:
		m_toolInputCommands.mouse_RB_Down = false;
		break;

	case WM_MBUTTONDOWN:
		m_toolInputCommands.mouse_MB_Down = true;
		break;
	case WM_MBUTTONUP:
		m_toolInputCommands.mouse_MB_Down = false;
		break;
	}
	//here we update all the actual app functionality that we want.  This information will either be used int toolmain, or sent down to the renderer (Camera movement etc
	//WASD movement
	if (m_keyArray['W'])
	{
		m_toolInputCommands.forward = true;
	}
	else m_toolInputCommands.forward = false;
	
	if (m_keyArray['S'])
	{
		m_toolInputCommands.back = true;
	}
	else m_toolInputCommands.back = false;
	if (m_keyArray['A'])
	{
		m_toolInputCommands.left = true;
	}
	else m_toolInputCommands.left = false;

	if (m_keyArray['D'])
	{
		m_toolInputCommands.right = true;
	}
	else m_toolInputCommands.right = false;
	//rotation
	if (m_keyArray['E'])
	{
		m_toolInputCommands.down = true;
	}
	else m_toolInputCommands.down = false;
	if (m_keyArray['Q'])
	{
		m_toolInputCommands.up = true;
	}
	else
	{
		m_toolInputCommands.up = false;
	}

	if (m_keyArray['M'])
	{
		m_toolInputCommands.multipleSelect = true;
	}
	else m_toolInputCommands.multipleSelect = false;

	//Duplicate the currently selected object
	if (GetAsyncKeyState(VK_CONTROL) && m_keyArray['V'])
	{
		m_keyArray['V'] = false;
		PasteObject();
	}
	// Focus the camera on the selected object
	if (m_keyArray['F'])
	{
		m_toolInputCommands.focusCamera = true;
	}
	else
	{
		m_toolInputCommands.focusCamera = false;
	}

	if (m_keyArray['I'])
	{
		m_toolInputCommands.objectForward = true;
	}
	else
	{
		m_toolInputCommands.objectForward = false;
	}
	if (m_keyArray['K'])
	{
		m_toolInputCommands.objectBackward = true;
	}
	else m_toolInputCommands.objectBackward = false;

	if (m_keyArray['J'])
	{
		m_toolInputCommands.objectLeft = true;
	}
	else m_toolInputCommands.objectLeft = false;

	if (m_keyArray['L'])
	{
		m_toolInputCommands.objectRight = true;
	}
	else m_toolInputCommands.objectRight = false;

	if (m_keyArray['O'])
	{
		m_toolInputCommands.objectUp = true;
	}
	else m_toolInputCommands.objectUp = false;

	if (m_keyArray['U'])
	{
		m_toolInputCommands.objectDown = true;
	}
	else m_toolInputCommands.objectDown = false;
}

void ToolMain::PasteObject()
{
	int currentID = -1;
	int newID = 0;

	for (const auto& obj : m_sceneGraph)
	{
		if (obj.ID == m_selectedObject)
		{
			currentID = &obj - &m_sceneGraph[0];
		}
		newID = max(newID, obj.ID);
	}
	if (currentID == -1) return; // Error: selected object not found

	// Create a new object based on the selected object and modify its ID and position
	SceneObject newObject = m_sceneGraph[currentID];
	newObject.ID = newID + 1;
	newObject.posY += 10;

	// Add the new object to the scene graph and update the selected object and display list
	m_sceneGraph.push_back(newObject);
	m_d3dRenderer.BuildDisplayList(&m_sceneGraph);
}

void ToolMain::SetTranslate()
{
	m_transform = Transform::Translate;
}

void ToolMain::SetRotate()
{
	m_transform = Transform::Rotate;
}

void ToolMain::SetScale()
{
	m_transform = Transform::Scale;
}

void ToolMain::UpdateDisplayList()
{
	m_d3dRenderer.UpdateDisplayList(m_selectedObject, &m_sceneGraph);
}

void ToolMain::TransformObjectForward()
{
	if (m_selectedObject != -1)
	{
		SceneObject* object = &m_sceneGraph.at(m_selectedObject); // Get the selected object
		DirectX::SimpleMath::Vector3 ForwardVector = m_d3dRenderer.CameraForwardVector(); // Get the camera forward vector so that the object can move forward on the correct axis

		switch(m_transform)
		{
		case Transform::Translate:
			object->posX += ForwardVector.x;
			object->posZ += ForwardVector.z;
			break;

		case Transform::Rotate:
			object->rotZ -= 1;
			break;

		case Transform::Scale:
			object->scaZ += 1;
			break;

		default:
			break;
		}
	}
}

void ToolMain::TransformObjectBackward()
{
	if (m_selectedObject != -1)
	{
		SceneObject* object = &m_sceneGraph.at(m_selectedObject); // Get the selected object
		DirectX::SimpleMath::Vector3 ForwardVector = m_d3dRenderer.CameraForwardVector(); // Get the camera forward vector so that the object can move forward on the correct axis

		switch (m_transform)
		{
		case Transform::Translate:
			object->posX -= ForwardVector.x;
			object->posZ -= ForwardVector.z;
			break;

		case Transform::Rotate:
			object->rotZ += 1;
			break;

		case Transform::Scale:
			object->scaZ -= 1;
			break;

		default:
			break;
		}
	}
}

void ToolMain::TransformObjectRight()
{
	if (m_selectedObject != -1)
	{
		SceneObject* object = &m_sceneGraph.at(m_selectedObject); // Get the selected object
		DirectX::SimpleMath::Vector3 RightVector = m_d3dRenderer.CameraRightVector(); // Get the camera right vector so that the object can move forward on the correct axis

		switch (m_transform)
		{
		case Transform::Translate:
			object->posX += RightVector.x;
			object->posZ += RightVector.z;
			break;

		case Transform::Rotate:
			object->rotX += 1;
			break;

		case Transform::Scale:
			object->scaX += 1;
			break;

		default:
			break;
		}
	}
}

void ToolMain::TransformObjectLeft()
{
	if (m_selectedObject != -1)
	{
		SceneObject* object = &m_sceneGraph.at(m_selectedObject); // Get the selected object
		DirectX::SimpleMath::Vector3 RightVector = m_d3dRenderer.CameraRightVector(); // Get the camera right vector so that the object can move forward on the correct axis

		switch (m_transform)
		{
		case Transform::Translate:
			object->posX -= RightVector.x;
			object->posZ -= RightVector.z;
			break;

		case Transform::Rotate:
			object->rotX -= 1;
			break;

		case Transform::Scale:
			object->scaX -= 1;
			break;

		default:
			break;
		}
	}
}

void ToolMain::TransformObjectUp()
{
	if (m_selectedObject != -1)
	{
		SceneObject* object = &m_sceneGraph.at(m_selectedObject); // Get the selected object

		switch (m_transform)
		{
		case Transform::Translate:
			object->posY += 1;
			break;

		case Transform::Rotate:
			object->rotY += 1;
			break;

		case Transform::Scale:
			object->scaY += 1;
			break;

		default:
			break;
		}
	}
}

void ToolMain::TransformObjectDown()
{
	if (m_selectedObject != -1)
	{
		SceneObject* object = &m_sceneGraph.at(m_selectedObject); // Get the selected object

		switch (m_transform)
		{
		case Transform::Translate:
			object->posY -= 1;
			break;

		case Transform::Rotate:
			object->rotY -= 1;
			break;

		case Transform::Scale:
			object->scaY -= 1;
			break;

		default:
			break;
		}
	}
}

float ToolMain::GetCameraMovementSpeed()
{
	return m_d3dRenderer.GetCameraMovementSpeed();
}

void ToolMain::SetCameraMovementSpeed(float cameraMovementSpeed)
{
	m_d3dRenderer.SetCameraMovementSpeed(cameraMovementSpeed);
}

float ToolMain::GetCameraRotationRate()
{
	return m_d3dRenderer.GetCameraRotationRate();
}

void ToolMain::SetCameraRotationRate(float cameraRotationRate)
{
	m_d3dRenderer.SetCameraRotationRate(cameraRotationRate);
}